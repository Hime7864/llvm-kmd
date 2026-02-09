#include "imports.hpp"

void exitThread()
{
	auto method = NtImports::fn_PsTerminateSystemThread;
	__asm
	{
		mov rax, [method]
		xor ecx, ecx
		call rax
		ret
	}
}

void SVM::LaunchCore()
{
	auto core_idx = CPUID::current_core_number();
	auto vCore = &vCpu[core_idx];
	auto saveArea = &vCore->vmcb.SaveStateArea;
	auto storage = &vCore->storage;
	auto ctx = &storage->ctx;

	ControlArea();

	RtlCaptureContext(ctx);

	saveArea->Rax = ctx->Rax;
	saveArea->RSP = ctx->Rsp;
	//saveArea->Rip = ctx->Rip;
	saveArea->Rip = (UINT64)exitThread;
	saveArea->RFLAGS = ctx->EFlags;

	saveArea->ES.selector = ctx->SegEs;
	saveArea->CS.selector = ctx->SegCs;
	saveArea->SS.selector = ctx->SegSs;
	saveArea->DS.selector = ctx->SegDs;

	storage->vmcb = Utils::LinearTranslate(&vCore->vmcb);
	MSR::HSAVE_PA(Utils::LinearTranslate(&vCore->hsave));

	auto efer = MSR::EFER();
	efer.svme = true;
	MSR::EFER(efer);

	saveArea->G_PAT = MSR::PAT().AsUINT64;
	saveArea->EFER = MSR::EFER().AsUINT64;

	saveArea->CR0 = __readcr0();
	saveArea->CR2 = __readcr2();
	saveArea->CR3 = __readcr3();
	saveArea->CR4 = __readcr4();

	SEGMENT_REGISTER gdtr{ 0 };
	__sgdt(&gdtr);
	saveArea->GDTR.base = gdtr.Base;
	saveArea->GDTR.limit = gdtr.Limit;

	SEGMENT_REGISTER idtr{ 0 };
	__sidt(&idtr);
	saveArea->IDTR.base = idtr.Base;
	saveArea->IDTR.limit = idtr.Limit;

	saveArea->ES.base = gdtr.base(saveArea->ES.selector);
	saveArea->CS.base = gdtr.base(saveArea->CS.selector);
	saveArea->SS.base = gdtr.base(saveArea->SS.selector);
	saveArea->DS.base = gdtr.base(saveArea->DS.selector);

	saveArea->ES.limit = gdtr.limit(saveArea->ES.selector);
	saveArea->CS.limit = gdtr.limit(saveArea->CS.selector);
	saveArea->SS.limit = gdtr.limit(saveArea->SS.selector);
	saveArea->DS.limit = gdtr.limit(saveArea->DS.selector);

	saveArea->ES.attrib = gdtr.attribute(saveArea->ES.selector);
	saveArea->CS.attrib = gdtr.attribute(saveArea->CS.selector);
	saveArea->SS.attrib = gdtr.attribute(saveArea->SS.selector);
	saveArea->DS.attrib = gdtr.attribute(saveArea->DS.selector);

	__vmsave(storage->vmcb);


	return;
	auto old = __readcr3();
	__writecr3(hCr3);

	VmLoop(vCore, storage->vmcb);

	__writecr3(old);

	return;
}

void SVM::LaunchVm()
{
	HANDLE thread_handle = 0;
	_OBJECT_ATTRIBUTES object_attribues{ };
	InitializeObjectAttributes(&object_attribues, nullptr, OBJ_KERNEL_HANDLE, 0, nullptr);
	PsCreateSystemThread(&thread_handle, 0, &object_attribues, 0, 0, (PKSTART_ROUTINE)&LaunchCore, nullptr);

	//KeIpiGenericCall(LaunchCore, nullptr);
	return;
}