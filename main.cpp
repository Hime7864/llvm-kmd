#include "imports.hpp"

UINT64 SVM::dCr3 = 0;
UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;

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

	controlArea->TlbControl.GuestASID = CPUID::current_core_number() + 1;
	controlArea->TlbControl.FlushGuestNonGlobalTLB = 1;
	controlArea->Intercept.VMRUN = 1;
	//controlArea->Intercept.CPUID = 1;
	//controlArea->Intercept.VMMCALL = 1;
	//controlArea->Intercept.MSR_Prot = 1;

	//msrPm->read(MSR::_MSR_EFER, true);
	//msrPm->write(MSR::_MSR_EFER, true);

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

		lea rcx, [rcx + 0x0500]
		call SaveCtx
		lea rcx, [rcx - 0x0500]
		push rcx

		push rdx

		lea rcx, [rcx + 0x30]
		call LoadCtx

		pop rax
		vmload
		vmrun
		vmsave


		push rcx
		mov rcx, [rsp + 0x08]

		push rdx

		// Serialize tsc
		mfence
		lfence
		rdtsc
		mov[rcx + 0x18], eax
		mov[rcx + 0x1C], edx

		// mperf
		push rcx
		mov ecx, 0xE7ul
		rdmsr
		pop rcx
		mov[rcx + 0x20], eax
		mov[rcx + 0x24], edx

		// aperf
		push rcx
		mov ecx, 0xE8ul
		rdmsr
		pop rcx
		mov[rcx + 0x28], eax
		mov[rcx + 0x2C], edx

		pop rdx

		lea rcx, [rcx + 0x30]
		call SaveCtx
		pop rax
		mov[rcx + 0x80], rax

		pop rcx
		lea rcx, [rcx + 0x0500]
		call LoadCtx
		lea rcx, [rcx - 0x0500]

		call VmExit
		//jmp loop

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

	printf("VMEXIT: code=%llx\n", ca->ExitCode);

	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}