#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
UINT64 SVM::syncRequest = 0;
UINT64 SVM::syncRelease = 0;
UINT64 SVM::syncArrived = 0;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("SVM Hypervisor Driver Loaded\n");
	SVM::Initialize();

	SVM::LaunchVm();
	
	//SVM::Cleanup();
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
	//controlArea->Intercept.NMI = true;
	controlArea->Intercept.MSR_Prot = true;

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
		lea rsp, [rcx + 0x3800]
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

	switch (exitCode)
	{
	case VMEXIT_VMMCALL:
	{
		ssa->Rax = 0xDEADBEEF;
	}break;
	case VMEXIT_CPUID:
	{
		_cpuid(ssa->Rax, &ssa->Rax, &gCtx->Rbx, &gCtx->Rcx, &gCtx->Rdx);
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
					MSR_EFER efer = { 0 };
					efer.AsUINT64 = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
					storage->efer = efer;
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
					storage->hsave = gCtx->Rax;
				}
				else
				{
					ssa->Rax = storage->hsave;
				}
			}break;
			default:
				break;
			}
		}
		else
		{
			ca->EventInjection.VECTOR = 13;// #GP
			ca->EventInjection.TYPE = 3;// Exception
			ca->EventInjection.V = true;
		}
	}break;
	default:
		break;
	}


	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}