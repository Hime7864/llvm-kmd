#include "imports.hpp"

UINT64 SVM::hCr3 = 0;
UINT64 SVM::gCr3 = 0;
VCORE* SVM::vCpu = 0;
UINT32 SVM::vCoreCount = 0;
xAPIC_REGISTERS* SVM::vaApicBase = 0;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NtImports::fn_DbgPrintEx(0, 0, "Loading\n");

	SVM::Initialize();
	
	SVM::LaunchVm();

	SVM::Cleanup();

	NtImports::fn_DbgPrintEx(0, 0, "Cleanup\n");
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

	if(ca->NextRip)
		ssa->Rip = ca->NextRip;
	return;
}
