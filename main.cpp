#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
UINT32 SVM::cpuMHz = 0;
xAPIC_REGISTERS* SVM::vaApicBase = 0;

void IpiVmmcall()
{
	__asm {vmmcall}
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("Loading\n");

	SVM::Initialize();
	
	SVM::LaunchVm();
	
	SVM::Cleanup();

	printf("Online\n");

	Sleep(1000);

	KeIpiGenericCall(IpiVmmcall, 0);

	printf("Cleanup\n");
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

		vmrun

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

void SVM::ControlArea()
{
	auto core_idx = CPUID::current_core_number();
	auto vCore = &vCpu[core_idx];
	auto ca = &vCore->vmcb.ControlArea;
	auto msrPm = &vCore->msrpm;
	auto storage = &vCore->storage;

	ca->TlbControl.GuestASID = CPUID::current_core_number() + 1;
	ca->TlbControl.FlushGuestNonGlobalTLB = true;
	ca->Intercept.MSR_Prot = true;

	ca->NestedPagingControl.NP_Enable = 1;
	ca->NestedCr3 = gCr3;

	ca->Intercept.VMRUN = true;
	ca->Intercept.VMMCALL = true;
	
	return;
}

void __attribute__((preserve_most)) SVM::VmExit(VCORE* vCore)
{
	auto vmcb = &vCore->vmcb;
	auto ssa = &vmcb->SaveStateArea;
	auto ca = &vmcb->ControlArea;
	auto msrPm = &vCore->msrpm;

	auto exitCode = ca->ExitCode;
	auto exitInfo1 = ca->ExitInfo1;
	auto exitInfo2 = ca->ExitInfo2;
	auto exitInfoIntercept = ca->ExitInfoIntercept;

	auto storage = &vCore->storage;
	auto gCtx = &storage->gCtx;

	ca->Intercept.INTR = true;
	if (__rdtsc() - storage->tsc_exit > 100000ull)
	{
		storage->tsc = __rdtsc();
		storage->aperf = MSR::APERF();
		storage->mperf = MSR::MPERF();
	}


	switch (exitCode)
	{
	case VMEXIT_VMMCALL:
	{
		if (!ca->Intercept.RDTSC)
		{
			//tsc spoof
			ca->Intercept.RDTSC = true;
			ca->Intercept.RDTSCP = true;
			msrPm->read(MSR::_MSR_TSC, true);
			msrPm->read(MSR::_MSR_MPERF_READ_ONLY, true);
			msrPm->read(MSR::_MSR_MPERF, true);
			msrPm->write(MSR::_MSR_MPERF, true);
			msrPm->read(MSR::_MSR_APERF_READ_ONLY, true);
			msrPm->read(MSR::_MSR_APERF, true);
			msrPm->write(MSR::_MSR_APERF, true);

			storage->tsc = __rdtsc();
			storage->aperf = MSR::APERF();
			storage->mperf = MSR::MPERF();
			storage->tsc_exit = __rdtsc();

			// MSR Shadows
			msrPm->read(MSR::_MSR_EFER, true);
			msrPm->write(MSR::_MSR_EFER, true);
			storage->efer = MSR::EFER();
			storage->efer.svme = false;

			msrPm->read(MSR::_MSR_HSAVE_PA, true);
			msrPm->write(MSR::_MSR_HSAVE_PA, true);
			storage->hsave = 0x0;
		}
	}break;
	case VMEXIT_INTR:
	{
		storage->tsc = __rdtsc();
		storage->aperf = MSR::APERF();
		storage->mperf = MSR::MPERF();
		ca->Intercept.INTR = false;
	}break;
	case VMEXIT_RDTSC:
	{
		//_mm_mfence();
		//_mm_lfence();

		auto tsc = __rdtsc();
		__rdtsc();
		tsc = (__rdtsc() - tsc);

		auto aperf = MSR::APERF();
		__rdtsc();
		aperf = (MSR::APERF() - aperf);

		auto mperf = MSR::MPERF();
		__rdtsc();
		mperf = (MSR::MPERF() - mperf);

		storage->tsc += tsc;
		storage->aperf += aperf;
		storage->mperf += mperf;

		ssa->Rax = storage->tsc & 0xFFFFFFFF;
		gCtx->Rdx = (storage->tsc >> 32) & 0xFFFFFFFF;
	}break;
	case VMEXIT_RDTSCP:
	{
		//_mm_mfence();
		//_mm_lfence();

		auto tsc = __rdtsc();
		__rdtsc();
		tsc = (__rdtsc() - tsc);

		auto aperf = MSR::APERF();
		__rdtsc();
		aperf = (MSR::APERF() - aperf);

		auto mperf = MSR::MPERF();
		__rdtsc();
		mperf = (MSR::MPERF() - mperf);

		storage->tsc += tsc;
		storage->aperf += aperf;
		storage->mperf += mperf;

		ssa->Rax = storage->tsc & 0xFFFFFFFF;
		gCtx->Rdx = (storage->tsc >> 32) & 0xFFFFFFFF;
		gCtx->Rcx = 0;
	}break;
	case VMEXIT_MSR:
	{
		if (ssa->CPL == 0)
		{
			auto msr = gCtx->Rcx;
			switch (msr)
			{
			case MSR::_MSR_EFER:
			{
				if (exitInfo1.MSR.isWrite)
				{
					auto data = MSR::EFER();
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::EFER(data);
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::EFER(data);
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::EFER(data);
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					storage->efer.AsUINT64 = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
				}
				else
				{
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::EFER();
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::EFER();
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::EFER();
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					ssa->Rax = storage->efer.AsUINT64 & 0xFFFFFFFF;
					gCtx->Rdx = (storage->efer.AsUINT64 >> 32) & 0xFFFFFFFF;
				}
			}break;
			case MSR::_MSR_HSAVE_PA:
			{
				if (exitInfo1.MSR.isWrite)
				{
					auto data = MSR::HSAVE_PA();
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::HSAVE_PA(data);
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::HSAVE_PA(data);
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::HSAVE_PA(data);
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					storage->hsave = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);
				}
				else
				{
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::HSAVE_PA();
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::HSAVE_PA();
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::HSAVE_PA();
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					ssa->Rax = storage->hsave & 0xFFFFFFFF;
					gCtx->Rdx = (storage->hsave >> 32) & 0xFFFFFFFF;
				}
			}break;
			case MSR::_MSR_TSC:
			{
				if (exitInfo1.MSR.isWrite)
				{
					auto data = MSR::TSC();
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::TSC(data);
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::TSC(data);
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::TSC(data);
					mperf = (MSR::MPERF() - mperf);

					storage->tsc = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;
				}
				else
				{
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::TSC();
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::TSC();
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::TSC();
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					ssa->Rax = storage->tsc & 0xFFFFFFFF;
					gCtx->Rdx = (storage->tsc >> 32) & 0xFFFFFFFF;
				}
			}break;
			case MSR::_MSR_APERF:
			{
				if (exitInfo1.MSR.isWrite)
				{
					auto data = MSR::APERF();
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::APERF(data);
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::APERF(data);
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::APERF(data);
					mperf = (MSR::MPERF() - mperf);

					storage->aperf = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;
				}
				else
				{
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::APERF();
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::APERF();
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::APERF();
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					ssa->Rax = storage->aperf & 0xFFFFFFFF;
					gCtx->Rdx = (storage->aperf >> 32) & 0xFFFFFFFF;
				}
			}break;
			case MSR::_MSR_APERF_READ_ONLY:
			{
				//_mm_mfence();
				//_mm_lfence();

				auto tsc = __rdtsc();
				MSR::APERF();
				tsc = (__rdtsc() - tsc);

				auto aperf = MSR::APERF();
				MSR::APERF();
				aperf = (MSR::APERF() - aperf);

				auto mperf = MSR::MPERF();
				MSR::APERF();
				mperf = (MSR::MPERF() - mperf);

				storage->tsc += tsc;
				storage->aperf += aperf;
				storage->mperf += mperf;

				ssa->Rax = storage->aperf & 0xFFFFFFFF;
				gCtx->Rdx = (storage->aperf >> 32) & 0xFFFFFFFF;
			}break;
			case MSR::_MSR_MPERF:
			{
				if (exitInfo1.MSR.isWrite)
				{
					auto data = MSR::MPERF();
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::MPERF(data);
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::MPERF(data);
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::MPERF(data);
					mperf = (MSR::MPERF() - mperf);

					storage->aperf = gCtx->Rdx << 32 | (ssa->Rax & 0xFFFFFFFF);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;
				}
				else
				{
					//_mm_mfence();
					//_mm_lfence();

					auto tsc = __rdtsc();
					MSR::MPERF();
					tsc = (__rdtsc() - tsc);

					auto aperf = MSR::APERF();
					MSR::MPERF();
					aperf = (MSR::APERF() - aperf);

					auto mperf = MSR::MPERF();
					MSR::MPERF();
					mperf = (MSR::MPERF() - mperf);

					storage->tsc += tsc;
					storage->aperf += aperf;
					storage->mperf += mperf;

					ssa->Rax = storage->mperf & 0xFFFFFFFF;
					gCtx->Rdx = (storage->mperf >> 32) & 0xFFFFFFFF;
				}
			}break;
			case MSR::_MSR_MPERF_READ_ONLY:
			{
				//_mm_mfence();
				//_mm_lfence();

				auto tsc = __rdtsc();
				MSR::MPERF();
				tsc = (__rdtsc() - tsc);

				auto aperf = MSR::APERF();
				MSR::MPERF();
				aperf = (MSR::APERF() - aperf);

				auto mperf = MSR::MPERF();
				MSR::MPERF();
				mperf = (MSR::MPERF() - mperf);

				storage->tsc += tsc;
				storage->aperf += aperf;
				storage->mperf += mperf;

				ssa->Rax = storage->mperf & 0xFFFFFFFF;
				gCtx->Rdx = (storage->mperf >> 32) & 0xFFFFFFFF;
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
	
	storage->tsc_exit = __rdtsc();
	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}
