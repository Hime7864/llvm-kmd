#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
xAPIC_REGISTERS* SVM::vaApicBase = 0;

volatile UINT64 syncRequest = 0;
volatile UINT64 syncArrived = 0;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("SVM Hypervisor Driver Loaded\n");
	SVM::Initialize();
	
	SVM::LaunchVm();
	
	SVM::Cleanup();
	printf("SVM Hypervisor Driver Unloaded\n");
	return STATUS_SUCCESS;
}

void SVM::ControlArea()
{
	auto core_idx = CPUID::current_core_number();
	auto vCore = &vCpu[core_idx];
	auto controlArea = &vCore->vmcb.ControlArea;
	auto msrPm = &vCore->msrpm;
	auto storage = &vCore->storage;

	controlArea->TlbControl.GuestASID = CPUID::current_core_number() + 1;
	controlArea->TlbControl.FlushGuestNonGlobalTLB = true;
	controlArea->Intercept.VMRUN = true;
	controlArea->Intercept.VMMCALL = true;
	controlArea->Intercept.VMLOAD = true;
	controlArea->Intercept.VMSAVE = true;
	controlArea->Intercept.NMI = true;
	controlArea->Intercept.MSR_Prot = true;
	controlArea->Intercept.CPUID = true;
	//controlArea->VirtualApic.V_NMI_ENABLE = 1;

	// MSR Shadows
	msrPm->read(MSR::_MSR_EFER, true);
	msrPm->write(MSR::_MSR_EFER, true);
	storage->efer = MSR::EFER();
	storage->efer.svme = false;

	msrPm->read(MSR::_MSR_HSAVE_PA, true);
	msrPm->write(MSR::_MSR_HSAVE_PA, true);
	storage->hsave = 0x0;

	controlArea->NestedPagingControl.NP_Enable = 1;
	controlArea->NestedCr3 = gCr3;
	return;
}

void NAKED SVM::VmLoop(VCORE* vCore, PHYSICAL_ADDRESS vmcb)
{
	__asm {
		mov rax, rsp
		lea rsp, [rcx + 0x2800]
		push rax

		loop:

		lea rcx, [rcx + 0x04D0]
		call SaveCtx
		lea rcx, [rcx - 0x04D0]
		push rcx

		push rdx

		call LoadCtx

		pop rax
		vmload
		vmrun
		vmsave

		push rcx
		mov rcx, [rsp + 0x08]
		push rdx

		rdtsc
		mov[rcx + 0x9B8], eax
		mov[rcx + 0x9BC], edx

		pop rdx
		call SaveCtx
		pop rax
		mov[rcx + 0x80], rax

		pop rcx
		lea rcx, [rcx + 0x04D0]
		call LoadCtx
		lea rcx, [rcx - 0x04D0]

		call VmExit
		jmp loop

		pop rax
		mov rsp, rax
		ret
	}
}

VOID NAKED SVM::NmiStub()
{
	__asm {
		call NmiHandler
		iretq
	}
}

void __attribute__((preserve_most)) SVM::NmiHandler()
{
	auto vCore = &vCpu[CPUID::current_core_number()];
	auto vmcb = &vCore->vmcb;
	auto ca = &vmcb->ControlArea;
	auto ssa = &vmcb->SaveStateArea;
	__sync_fetch_and_sub(&syncRequest, 1);
	if(ssa->CPL == 3)
		ca->TscOffset = 0;
	return;
}


void SVM::broadcast_nmi()
{
	while (syncRequest) { _mm_pause(); }
	__sync_lock_test_and_set(&syncRequest, vCoreCount);
	auto apic_base = MSR::APIC_BASE();
	if (apic_base.extd)
	{
		MSR_ICR icr;
		icr.AsUINT64 = 0;
		icr.MT = ICR_MT::Nmi;
		icr.TGM = ICR_TGM::edge;
		icr.L = ICR_L::assert;
		icr.DSH = ICR_DSH::AllExcludingSelf;
		MSR::ICR(icr);
	}
	else
	{
		xAPIC_REGISTERS::ICR_LOW icr_low;
		icr_low.AsUINT32 = 0;
		icr_low.MT = ICR_MT::Nmi;
		icr_low.TGM = ICR_TGM::edge;
		icr_low.L = ICR_L::assert;
		icr_low.DSH = ICR_DSH::AllExcludingSelf;
		vaApicBase->WriteICR(icr_low);
	}
	NmiHandler();
	return;
}


void __attribute__((preserve_most)) SVM::VmExit(VCORE* vCore)
{
	
	auto vmcb = &vCore->vmcb;
	auto ssa = &vmcb->SaveStateArea;
	auto ca = &vmcb->ControlArea;

	auto storage = &vCore->storage;
	auto gCtx = &storage->gCtx;

	auto exitCode = ca->ExitCode;
	auto exitInfo1 = ca->ExitInfo1;
	auto exitInfo2 = ca->ExitInfo2;
	

	if (exitCode == VMEXIT_NMI)
	{
		if (syncRequest)
		{
			auto tr = __str();
			vCore->hGdt[(tr >> 3) / 2].access = 0x89;
			__ltr(tr);
			__asm { cli }
			__asm { stgi }
			__asm { clgi }
			__asm { sti }

			if(ca->TscOffset)
				ca->TscOffset -= 4000ll + (__rdtsc() - storage->tsc);
		}
		else
		{
			ca->EventInjection.VECTOR = SVM_EVENTINJ_VECTOR_NMI;
			ca->EventInjection.TYPE = SVM_EVENTINJ_TYPE_NMI;
			ca->EventInjection.EV = SVM_EVENTINJ_ERROR_CODE_INVALID;
			ca->EventInjection.V = SVM_EVENTINJ_VALID;
		}
		ca->NextRip = 0;
	}
	else
	{
		ca->TscOffset -= 1000ll - (__rdtsc() - storage->tsc);
		switch (exitCode)
		{
		case VMEXIT_CPUID:
		{
			_cpuid(ssa->Rax, &ssa->Rax, &gCtx->Rbx, &gCtx->Rcx, &gCtx->Rdx);
			if (ssa->CPL == 3 && CPUID::current_core_number() == 1)
			{
				broadcast_nmi();
			}
		}break;
		case VMEXIT_MSR:
		{
			if (ssa->CPL == 0)
			{
				switch ((UINT32)gCtx->Rcx)
				{
				case MSR::_MSR_EFER:
				{
					if (exitInfo1.MSR.isWrite)
					{
						storage->efer.AsUINT64 = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
					}
					else
					{
						ssa->Rax = storage->efer.AsUINT64 & 0xFFFFFFFF;
						gCtx->Rdx = (storage->efer.AsUINT64 >> 32) & 0xFFFFFFFF;
					}
				}break;
				case MSR::_MSR_HSAVE_PA:
				{
					if (exitInfo1.MSR.isWrite)
					{
						storage->hsave = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
					}
					else
					{
						ssa->Rax = storage->hsave & 0xFFFFFFFF;
						gCtx->Rdx = (storage->hsave >> 32) & 0xFFFFFFFF;
					}
				}break;
				default:
					break;
				}
			}
			else
			{
				ca->EventInjection.VECTOR = SVM_EVENTINJ_VECTOR_GP;
				ca->EventInjection.TYPE = SVM_EVENTINJ_TYPE_EXCEPTION;
				ca->EventInjection.EV = SVM_EVENTINJ_ERROR_CODE_INVALID;
				ca->EventInjection.V = SVM_EVENTINJ_VALID;
			}
		}break;
		case VMEXIT_VMMCALL:
		{
			ssa->Rax = ca->TscOffset;
			gCtx->Rbx = 0;
		}break;
		default:

			break;
		}
	}

	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}
