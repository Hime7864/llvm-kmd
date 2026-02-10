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
		push rcx
		push rdx

		// Save host context
		lea rcx, [rcx + 0x04F0]
		call SaveCtx
		lea rcx, [rcx - 0x04F0]

		// Load guest context
		lea rcx, [rcx + 0x0020]
		call LoadCtx

		// VmRun guest
		mov rax, [rsp]
		vmload
		//vmrun
		vmsave

		// Save guest context
		push rcx
		mov rcx, [rsp + 0x10]
		lea rcx, [rcx + 0x0020]
		call SaveCtx
		pop rax
		mov [rcx + 0x60], rax
		lea rcx, [rcx - 0x0020]

		// Restore host context
		lea rcx, [rcx + 0x04F0]
		call LoadCtx
		lea rcx, [rcx - 0x04F0]

		call VmExit

		pop rdx
		pop rcx

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


	ssa->Rip = ca->NextRip;
	return;
}