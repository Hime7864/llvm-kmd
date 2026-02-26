#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
UINT32 SVM::cpuMHz = 0;
xAPIC_REGISTERS* SVM::vaApicBase = 0;

void VmSync()
{
	for (int i = 0; i < 10; i++)
	{
		_mm_lfence();
		_mm_mfence();
		auto start = __rdtsc();
		__asm {
			push rax
			push rbx
			vmmcall
			pop rbx
			pop rax
		}
		auto end = __rdtsc();
		auto delta = end - start;
	}
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("SVM Hypervisor Driver Loaded\n");
	SVM::Initialize();
	
	SVM::LaunchVm();
	
	SVM::Cleanup();

	Sleep(500);

	KeIpiGenericCall(VmSync, 0);
	printf("SVM Hypervisor Driver Unloaded\n");
	return STATUS_SUCCESS;
}

void NAKED SVM::VmLoop(VCORE* vCore, PHYSICAL_ADDRESS vmcb)
{
	__asm {
		mov rax, rsp
		lea rsp, [rcx + 0x3A00]
		push rax
	loop:
		lea rcx, [rcx + 0x04D0]
		call SaveCtx
		lea rcx, [rcx - 0x04D0]
		push rcx

		push rdx

		call LoadCtx

		pop rax

		//vmload
		vmrun
		//vmsave

		push rcx
		mov rcx, [rsp + 0x08]
		push rdx
		push rax

		//rdtsc
		//mov[rcx + 0x9A8], eax
		//mov[rcx + 0x9AC], edx
		// aperf
		push rcx
		mov ecx, 0xE8ul
		rdmsr
		pop rcx
		mov[rcx + 0x9B8], eax
		mov[rcx + 0x9BC], edx
		// mperf
		push rcx
		mov ecx, 0xE7ul
		rdmsr
		pop rcx
		mov[rcx + 0x9C0], eax
		mov[rcx + 0x9C4], edx


		pop rax
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

void SVM::ControlArea()
{
	auto core_idx = CPUID::current_core_number();
	auto vCore = &vCpu[core_idx];
	auto controlArea = &vCore->vmcb.ControlArea;
	auto msrPm = &vCore->msrpm;
	auto storage = &vCore->storage;

	controlArea->TlbControl.GuestASID = CPUID::current_core_number() + 1;
	controlArea->TlbControl.FlushGuestNonGlobalTLB = true;
	controlArea->Intercept.MSR_Prot = true;

	controlArea->Intercept.VMRUN = true;
	controlArea->Intercept.VMMCALL = true;

	controlArea->NestedPagingControl.NP_Enable = 1;
	controlArea->NestedCr3 = gCr3;
	return;
}

void __attribute__((preserve_most)) SVM::VmExit(VCORE* vCore)
{
	auto vmcb = &vCore->vmcb;
	auto ssa = &vmcb->SaveStateArea;
	auto ca = &vmcb->ControlArea;
	auto msrPm = &vCore->msrpm;

	auto storage = &vCore->storage;
	auto gCtx = &storage->gCtx;

	if(!storage->tsc_first_sight)
		storage->tsc_first_sight = __rdtsc();

	auto exitCode = ca->ExitCode;
	auto exitInfo1 = ca->ExitInfo1;
	auto exitInfo2 = ca->ExitInfo2;
	auto exitInfoIntercept = ca->ExitInfoIntercept;

	if (exitCode == VMEXIT_INTR)
	{
		storage->tsc_step = 0;
		storage->tsc_first_sight = __rdtsc();
		ca->TscOffset = 0;
		ca->Intercept.INTR = false;
		ca->Intercept.RDTSC = false;
		ca->Intercept.RDTSCP = false;
		ca->NextRip = 0;
		msrPm->read(MSR::_MSR_APERF, true);
		msrPm->read(MSR::_MSR_MPERF, true);
	}
	else
	{
		auto init_tsc = ca->TscOffset;
		ca->Intercept.INTR = true;
		if (!ca->Intercept.RDTSC)
		{
			storage->tsc_step = 0;
			storage->tsc_first_sight = __rdtsc();
		}
		ca->Intercept.RDTSC = true;
		ca->Intercept.RDTSCP = true;
		switch (exitCode)
		{
		case VMEXIT_VMMCALL:
		{
			if (!storage->loaded)
			{
				msrPm->read(MSR::_MSR_TSC, true);

				// MSR Shadows
				msrPm->read(MSR::_MSR_EFER, true);
				msrPm->write(MSR::_MSR_EFER, true);
				storage->efer.data = MSR::EFER();
				storage->efer.data.svme = false;

				msrPm->read(MSR::_MSR_HSAVE_PA, true);
				msrPm->write(MSR::_MSR_HSAVE_PA, true);
				storage->hsave.data = 0x0;

				storage->loaded = true;
			}
		}break;
		case VMEXIT_RDTSC:
		case VMEXIT_RDTSCP:
		{
			auto start = __rdtsc();
			auto delta = __rdtsc() - start;
			storage->tsc_step += delta;
			storage->tsc_first_sight += storage->tsc_step;
			ssa->Rax = storage->tsc_first_sight & 0xFFFFFFFFull;
			gCtx->Rdx = (storage->tsc_first_sight >> 32) & 0xFFFFFFFFull;
			if (exitCode == VMEXIT_RDTSCP)
			{
				gCtx->Rcx = CPUID::current_core_number() & 0xFFFFFFFFull;
			}

		}break;
		case VMEXIT_MSR:
		{
			if (ssa->CPL == 0)
			{
				switch ((UINT32)gCtx->Rcx)
				{
				case MSR::_MSR_APERF:
				{
					auto aperf = storage->aperf_init;
					if (storage->aperf_last)
					{
						aperf = storage->aperf_last;
						storage->aperf_last = 0;
					}
					ssa->Rax = aperf & 0xFFFFFFFFull;
					gCtx->Rdx = (aperf >> 32) & 0xFFFFFFFFull;
				}break;
				case MSR::_MSR_MPERF:
				{
					auto mperf = storage->mperf_init;
					if (storage->mperf_last)
					{
						mperf = storage->mperf_last;
						storage->mperf_last = 0;
					}
					ssa->Rax = mperf & 0xFFFFFFFFull;
					gCtx->Rdx = (mperf >> 32) & 0xFFFFFFFFull;
				}break;
				case MSR::_MSR_EFER:
				{
					if (exitInfo1.MSR.isWrite)
					{
						auto efer = MSR::EFER();
						auto tsc = __rdtsc();
						MSR::EFER(efer);
						storage->tsc_step = (__rdtsc() - tsc);
						storage->efer.data.AsUINT64 = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
					}
					else
					{
						auto tsc = __rdtsc();
						MSR::EFER();
						storage->tsc_step = (__rdtsc() - tsc);
						ssa->Rax = storage->efer.data.AsUINT64 & 0xFFFFFFFF;
						gCtx->Rdx = (storage->efer.data.AsUINT64 >> 32) & 0xFFFFFFFF;
					}
					storage->aperf_last = storage->aperf_init;
					storage->mperf_last = storage->mperf_init;
				}break;
				case MSR::_MSR_HSAVE_PA:
				{
					if (exitInfo1.MSR.isWrite)
					{
						auto hsave_pa = MSR::HSAVE_PA();
						auto tsc = __rdtsc();
						MSR::HSAVE_PA(hsave_pa);
						storage->tsc_step = (__rdtsc() - tsc);
						storage->hsave.data = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
					}
					else
					{
						auto tsc = __rdtsc();
						MSR::HSAVE_PA();
						storage->tsc_step = (__rdtsc() - tsc);
						ssa->Rax = storage->hsave.data & 0xFFFFFFFF;
						gCtx->Rdx = (storage->hsave.data >> 32) & 0xFFFFFFFF;
					}
					storage->aperf_last = storage->aperf_init;
					storage->mperf_last = storage->mperf_init;
				}break;
				case MSR::_MSR_TSC:
				{
					auto tsc_probe = __rdtsc();
					MSR::TSC();
					storage->tsc_step = (__rdtsc() - tsc_probe);
					storage->tsc_first_sight += storage->tsc_step;
					ssa->Rax = storage->tsc_first_sight & 0xFFFFFFFFull;
					gCtx->Rdx = (storage->tsc_first_sight >> 32) & 0xFFFFFFFFull;
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
		default:
			break;
		}
	}
	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}
