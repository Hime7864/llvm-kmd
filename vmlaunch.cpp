#include "imports.hpp"

void NAKED SVM::SaveCtx(VCORE* vCore)
{
	__asm {
		pushfq
		mov[rcx + 0x78], rax
		mov[rcx + 0x80], rcx
		mov[rcx + 0x88], rdx
		mov[rcx + 0xB8], r8
		mov[rcx + 0xC0], r9
		mov[rcx + 0xC8], r10
		mov[rcx + 0xD0], r11
		//movaps xmmword ptr[rcx + 0x1A0], xmm0
		//movaps xmmword ptr[rcx + 0x1B0], xmm1
		//movaps xmmword ptr[rcx + 0x1C0], xmm2
		//movaps xmmword ptr[rcx + 0x1D0], xmm3
		//movaps xmmword ptr[rcx + 0x1E0], xmm4
		//movaps xmmword ptr[rcx + 0x1F0], xmm5
		mov[rcx + 0x90], rbx
		mov[rcx + 0xA0], rbp
		mov[rcx + 0xA8], rsi
		mov[rcx + 0xB0], rdi
		mov[rcx + 0xD8], r12
		mov[rcx + 0xE0], r13
		mov[rcx + 0xE8], r14
		mov[rcx + 0xF0], r15
		//fnstcw word ptr[rcx + 0x100]
		//movaps xmmword ptr[rcx + 0x200], xmm6
		//movaps xmmword ptr[rcx + 0x210], xmm7
		//movaps xmmword ptr[rcx + 0x220], xmm8
		//movaps xmmword ptr[rcx + 0x230], xmm9
		//movaps xmmword ptr[rcx + 0x240], xmm10
		//movaps xmmword ptr[rcx + 0x250], xmm11
		//movaps xmmword ptr[rcx + 0x260], xmm12
		//movaps xmmword ptr[rcx + 0x270], xmm13
		//movaps xmmword ptr[rcx + 0x280], xmm14
		//movaps xmmword ptr[rcx + 0x290], xmm15
		//stmxcsr dword ptr[rcx + 0x118]
		//stmxcsr dword ptr[rcx + 0x34]
		lea rax, [rsp + 0x10]
		mov[rcx + 0x98], rax
		mov rax, [rsp + 0x8]
		mov[rcx + 0xF8], rax
		mov eax, [rsp]
		mov[rcx + 0x44], eax
		add rsp, 8
		retn
	}
}

void NAKED SVM::LoadCtx(VCORE* vCore)
{
	__asm {
		mov rdx, [rcx + 0x88]
		mov r8, [rcx + 0xB8]
		mov r9, [rcx + 0xC0]
		mov r10, [rcx + 0xC8]
		mov r11, [rcx + 0xD0]
		//movaps xmm0, xmmword ptr[rcx + 0x1A0]
		//movaps xmm1, xmmword ptr[rcx + 0x1B0]
		//movaps xmm2, xmmword ptr[rcx + 0x1C0]
		//movaps xmm3, xmmword ptr[rcx + 0x1D0]
		//movaps xmm4, xmmword ptr[rcx + 0x1E0]
		//movaps xmm5, xmmword ptr[rcx + 0x1F0]
		mov rbx, [rcx + 0x90]
		mov rbp, [rcx + 0xA0]
		mov rsi, [rcx + 0xA8]
		mov rdi, [rcx + 0xB0]
		mov r12, [rcx + 0xD8]
		mov r13, [rcx + 0xE0]
		mov r14, [rcx + 0xE8]
		mov r15, [rcx + 0xF0]
		//fldcw word ptr[rcx + 0x100]
		//movaps xmm6, xmmword ptr[rcx + 0x200]
		//movaps xmm7, xmmword ptr[rcx + 0x210]
		//movaps xmm8, xmmword ptr[rcx + 0x220]
		//movaps xmm9, xmmword ptr[rcx + 0x230]
		//movaps xmm10, xmmword ptr[rcx + 0x240]
		//movaps xmm11, xmmword ptr[rcx + 0x250]
		//movaps xmm12, xmmword ptr[rcx + 0x260]
		//movaps xmm13, xmmword ptr[rcx + 0x270]
		//movaps xmm14, xmmword ptr[rcx + 0x280]
		//movaps xmm15, xmmword ptr[rcx + 0x290]
		//ldmxcsr dword ptr[rcx + 0x118]
		//ldmxcsr dword ptr[rcx + 0x34]
		mov eax, [rcx + 0x44]
		push rax
		popfq
		mov rax, [rcx + 0x78]
		mov rcx, [rcx + 0x80]
		ret
	}
}

void SVM::LaunchCore(int affinity)
{
	auto core_idx = CPUID::current_core_number();
	auto vCore = &vCpu[core_idx];
	auto controlArea = &vCore->vmcb.ControlArea;
	auto saveArea = &vCore->vmcb.SaveStateArea;
	auto storage = &vCore->storage;
	auto ctx = &storage->gCtx;

	ControlArea();

	auto result = RtlCaptureContext(ctx);
	if (ctx->Rax == result)
	{
		printf("Core %i Online\n", core_idx);
		return;
	}

	saveArea->Rax = ctx->Rax;
	saveArea->RSP = ctx->Rsp;
	saveArea->Rip = ctx->Rip;
	saveArea->RFLAGS = ctx->EFlags;

	saveArea->ES.selector = ctx->SegEs;
	saveArea->CS.selector = ctx->SegCs;
	saveArea->SS.selector = ctx->SegSs;
	saveArea->DS.selector = ctx->SegDs;

	storage->vmcb = Utils::LinearTranslate(hCr3, &vCore->vmcb);
	MSR::HSAVE_PA(Utils::LinearTranslate(hCr3, &vCore->hsave));

	controlArea->MsrPmBasePa = Utils::LinearTranslate(hCr3, (UINT64)&vCore->msrpm);

	auto efer = MSR::EFER();
	efer.svme = true;
	MSR::EFER(efer);

	saveArea->G_PAT = MSR::PAT().AsUINT64;
	saveArea->EFER = MSR::EFER().AsUINT64;

	saveArea->CR0 = __readcr0();
	saveArea->CR2 = __readcr2();
	saveArea->CR3 = __readcr3();
	saveArea->CR4 = __readcr4();

	SEGMENT_REGISTER idtr{ 0 };
	__sidt(&idtr);
	saveArea->IDTR.base = idtr.Base;
	saveArea->IDTR.limit = idtr.Limit;

	SEGMENT_REGISTER gdtr{ 0 };
	__sgdt(&gdtr);
	saveArea->GDTR.base = gdtr.Base;
	saveArea->GDTR.limit = gdtr.Limit;

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

	auto old = __readcr3();
	__writecr3(hCr3);

	VmLoop(vCore, storage->vmcb);
	
	__writecr3(old);

	return;
}

void SVM::CoreSynchronization(TSC_SYNC* sync)
{
	auto efer = MSR::EFER();
	MSR::EFER({ (0xDEAD01ULL << 32) | (sync->efer.read & 0xFFFFFFFF) });
	MSR::EFER({ (0xDEAD02ULL << 32) | (sync->efer.write & 0xFFFFFFFF) });
	MSR::EFER(efer);

	auto hsave = MSR::HSAVE_PA();
	MSR::HSAVE_PA({ (0xDEAD01ULL << 32) | (sync->hsave.read & 0xFFFFFFFF) });
	MSR::HSAVE_PA({ (0xDEAD02ULL << 32) | (sync->hsave.write & 0xFFFFFFFF) });
	MSR::HSAVE_PA(hsave);
	return;
}


void efer_read_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 20000; i++)
	{
		auto count = __rdtsc();
		MSR::EFER();
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 efer_read()
{
	auto efer_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(efer_read_ipi, efer_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > efer_tsc[i])
			min = efer_tsc[i];
	}

	ExFreePool(efer_tsc);
	return min;
}

void efer_write_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	const auto efer = MSR::EFER();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 20000; i++)
	{
		auto count = __rdtsc();
		MSR::EFER(efer);
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 efer_write()
{
	auto efer_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(efer_write_ipi, efer_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > efer_tsc[i])
			min = efer_tsc[i];
	}

	ExFreePool(efer_tsc);
	return min;
}

void hsave_read_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 20000; i++)
	{
		auto count = __rdtsc();
		MSR::HSAVE_PA();
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 hsave_read()
{
	auto hsave_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(hsave_read_ipi, hsave_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > hsave_tsc[i])
			min = hsave_tsc[i];
	}

	ExFreePool(hsave_tsc);
	return min;
}

void hsave_write_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	const auto hsave = MSR::HSAVE_PA();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 20000; i++)
	{
		auto count = __rdtsc();
		MSR::HSAVE_PA(hsave);
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 hsave_write()
{
	auto hsave_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(hsave_write_ipi, hsave_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > hsave_tsc[i])
			min = hsave_tsc[i];
	}

	ExFreePool(hsave_tsc);
	return min;
}

void SVM::LaunchVm()
{
	KeIpiGenericCall(LaunchCore, nullptr);
	auto sync = (TSC_SYNC*)ExAllocatePool(NonPagedPool, 0x1000);
	Sleep(100);
	sync->efer.read = efer_read() - 80;
	Sleep(100);
	sync->efer.write = efer_write() - 80;
	Sleep(100);
	sync->hsave.read = hsave_read() - 80;
	Sleep(100);
	sync->hsave.write = hsave_write() - 80;
	Sleep(100);
	
	printf("TSC EFER Read: %llu\n", sync->efer.read);
	printf("TSC EFER Write: %llu\n", sync->efer.write);
	printf("TSC HSAVE Read: %llu\n", sync->hsave.read);
	printf("TSC HSAVE Write: %llu\n", sync->hsave.write);

	KeIpiGenericCall(CoreSynchronization, sync);
	RtlFillMemory(sync, 0x1000, 0x0);
	ExFreePool(sync);
	return;
}