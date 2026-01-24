#include "imports.hpp"


class HVL
{
private:
	union HvlEnlightenment
	{
		UINT64 AsUINT64;
		struct
		{
			unsigned : 6;
			unsigned running : 1;
			unsigned : 25;
			unsigned : 32;
		};
	};
	static QWORD NextHzRate;
	static QWORD HzRate;
	static PVOID pFunc;
	static PVOID pCallback;
	static FnPtr* oCallBack;
	static HvlEnlightenment* pHvlEnlightenment;
	static UINT64 pHvlLongSpinCountMask;

	static UINT64 NAKED CallBack();

public:
	static void Start(PVOID pFunc, QWORD HzRate);
	static void End();
};

QWORD HVL::NextHzRate = 0;
QWORD HVL::HzRate = 0;
PVOID HVL::pFunc = 0;
PVOID HVL::pCallback = 0;
FnPtr* HVL::oCallBack = 0;
HVL::HvlEnlightenment* HVL::pHvlEnlightenment = 0;
UINT64 HVL::pHvlLongSpinCountMask = 0;


UINT64 NAKED HVL::CallBack()
{
	__asm {
		push rax
		push rdx
		push rcx

		rdtsc
		shl rdx, 32
		add rax, rdx

		mov rcx, [NextHzRate]
		cmp rax, rcx
		jb skipCall
		
		push rax
		call pFunc
		pop rax

		add rax, [HzRate]
		mov[NextHzRate], rax

	skipCall:
		pop rcx
		pop rdx
		pop rax
		xor dil, dil
		ret;
	}
}

void HVL::Start(PVOID _pFunc, QWORD _HzRate)
{
	pFunc = _pFunc;
	HzRate = _HzRate;
	if (!pCallback)
	{
		QWORD kernel_base = Utils::GetKernelBase();
		QWORD kernel_text_base, kernel_text_size;
		if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, ".text", &kernel_text_base, &kernel_text_size)))
			return;

		pHvlLongSpinCountMask = Utils::deref(2, Utils::sig_scan(kernel_text_base, kernel_text_size, "85 1D ? ? ? ? 75 ? 8B 05 ? ? ? ? A8 ? 74 ? E8 ? ? ? ? 84 C0 74 ? 8B CB E8 ? ? ? ? EB ? F3 90 48 8B 47 ? 48 85 C0 75 ? EB ? 48 83 C6 ? 48 83 ED ? 75 ? 48 8D 84 24"));
		pHvlEnlightenment = (HvlEnlightenment*)Utils::deref(2, Utils::sig_scan(kernel_text_base, kernel_text_size, "8B 05 ? ? ? ? A8 ? 74 ? E8 ? ? ? ? 84 C0 74 ? 8B CB E8 ? ? ? ? EB ? F3 90 48 8B 47 ? 48 85 C0 75 ? EB ? 48 83 C6 ? 48 83 ED ? 75 ? 48 8D 84 24"));
		pCallback = (PVOID)Utils::deref(3, Utils::sig_scan(kernel_text_base, kernel_text_size, "48 8B 05 ? ? ? ? 4C 8B C5"));
	}
	if (pCallback)
	{
		oCallBack = (FnPtr*)*(QWORD*)pCallback;
		*(QWORD*)pCallback = (QWORD)&HVL::CallBack;
		pHvlEnlightenment->running = true;
		*(DWORD*)pHvlLongSpinCountMask = 0x0;
	}
	return;
}

void HVL::End()
{
	if (pCallback)
	{
		*(DWORD*)pHvlLongSpinCountMask = 0xFFFFFFFFUL;
		pHvlEnlightenment->running = false;
		*(QWORD*)pCallback = (QWORD)oCallBack;
	}
	return;
}

void test()
{
	printf("HVL Test Function Executed!\n");
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto Hz = MSR::read_pstate(0).get_frequency_mhz();
	Hz /= 1000;
	HVL::Start((PVOID)test, Hz);
	Sleep(100);
	HVL::End();


	return STATUS_SUCCESS;
}
