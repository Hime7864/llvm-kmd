#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
volatile UINT64 syncRequest = 0;
volatile UINT64 syncRelease = 0;
volatile UINT64 syncArrived = 0;

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
	controlArea->Intercept.NMI = true;
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
		//stgi
		vmrun
		//clgi
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
	case VMEXIT_NMI:
	{
		if (syncRequest)
		{
			storage->counter++;
			__sync_add_and_fetch(&syncArrived, 1);
			while (syncRelease == 0) { _mm_pause(); };
		}
		else// pass through nmi
		{
			ca->EventInjection.VECTOR = 2;
			ca->EventInjection.TYPE = 2;
			ca->EventInjection.EV = 0;
			ca->EventInjection.V = 1;
		}
		ca->NextRip = 0;
	}break;
	case VMEXIT_VMMCALL:
	{
		if (ssa->Rax == 1)
		{
			syncRelease = 0;
			_mm_clflush((const PVOID) & syncRelease);
			syncRequest = 1;
			_mm_clflush((const PVOID)&syncRequest);
			MSR_ICR icr;
			icr.AsUINT64 = 0;
			icr.msg_type = ICR_MSG_TYPE_NMI;
			icr.level = ICR_LEVEL_ASSERT;
			icr.trigger_mode = ICR_TRIGGER_EDGE;
			icr.dest_shorthand = ICR_DEST_ALL_EXCLUDING;
			MSR::ICR(icr);
			while (syncArrived != (vCoreCount - 1)) { _mm_pause(); };
			syncArrived = 0;
			_mm_clflush((const PVOID)&syncArrived);
			syncRequest = 0;
			_mm_clflush((const PVOID)&syncRequest);
			syncRelease = 1;
			_mm_clflush((const PVOID)&syncRelease);
		}
	}break;
	case VMEXIT_CPUID:
	{
		if (ssa->Rax == 0xDEADBEEF)
		{
			ssa->Rax = storage->counter;
		}
		else
		{
			_cpuid(ssa->Rax, &ssa->Rax, &gCtx->Rbx, &gCtx->Rcx, &gCtx->Rdx);
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
	ca->NextRip = 0;
	return;
}
