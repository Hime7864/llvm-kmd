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
		
		lea rax, [rcx + 0x3800]
		mov rbx, rsp
		mov rsp, rax
		push rbx

		push rcx
		push rdx
		mov rax, rdx

		//loop :
		vmload
		vmrun
		vmsave
		push rax
		push rcx

		mov rcx, [rsp + 18h]
		call VmExit
		pop rcx
		pop rax
		//jmp loop

		pop rdx
		pop rcx

		pop rbx
		mov rsp, rbx
		ret
	}
}

void __attribute__((preserve_most)) SVM::VmExit(VCORE* vCore)
{
	auto vmcb = &vCore->vmcb;
	auto ssa = &vmcb->SaveStateArea;
	auto ca = &vmcb->ControlArea;
	
	auto exitCode = ca->ExitCode;
	printf("exitCode: %i\n", exitCode);

	return;
}