#include "imports.hpp"

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


void do_stuff()
{

	return;
}



PVOID pCallback = nullptr;
FnPtr* oCallBack = nullptr;

char* test_str = "Hello from hypervisor callback!\n";

HvlEnlightenment* pHvlEnlightenment = nullptr;

QWORD tsc_call = 0;

void CallBack_timeout()
{
	if (!tsc_call)
		tsc_call = __rdtsc() + MSR::read_pstate(0).get_frequency_mhz();

	if (tsc_call < __rdtsc())
	{
		do_stuff();
		tsc_call = __rdtsc() + MSR::read_pstate(0).get_frequency_mhz();
	}
	return;
}

UINT64 NAKED CallBack()
{
	__asm {
		push rax
		
		call CallBack_timeout
		
		pop rax

		//mov rax, [rsp]// our ret
		xor dil, dil
		ret;
	}
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	


	QWORD kernel_base = Utils::GetKernelBase();
	QWORD kernel_text_base, kernel_text_size;
	if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, ".text", &kernel_text_base, &kernel_text_size)))
		return STATUS_UNSUCCESSFUL;

	//auto sig = 8B 0D ? ? ? ? 48 03 43;
	auto pHvlLongSpinCountMask = Utils::deref(2, Utils::sig_scan(kernel_text_base, kernel_text_size, "85 1D ? ? ? ? 75 ? 8B 05 ? ? ? ? A8 ? 74 ? E8 ? ? ? ? 84 C0 74 ? 8B CB E8 ? ? ? ? EB ? F3 90 48 8B 47 ? 48 85 C0 75 ? EB ? 48 83 C6 ? 48 83 ED ? 75 ? 48 8D 84 24"));
	pHvlEnlightenment = (HvlEnlightenment*)Utils::deref(2, Utils::sig_scan(kernel_text_base, kernel_text_size, "8B 05 ? ? ? ? A8 ? 74 ? E8 ? ? ? ? 84 C0 74 ? 8B CB E8 ? ? ? ? EB ? F3 90 48 8B 47 ? 48 85 C0 75 ? EB ? 48 83 C6 ? 48 83 ED ? 75 ? 48 8D 84 24"));
	auto pHvcallCodeVa = Utils::deref(3, Utils::sig_scan(kernel_text_base, kernel_text_size, "48 8B 05 ? ? ? ? 4C 8B C5"));
	
	//set pHvlLongSpinCountMask to zero
	//set enlightenment.running to true
	//set the hypercall callback to our function
	
	pCallback = (PVOID)pHvcallCodeVa;
	oCallBack = (FnPtr*)(*(QWORD*)pCallback);
	
	*(QWORD*)pCallback = (QWORD)&CallBack;
	pHvlEnlightenment->running = true;
	*(DWORD*)pHvlLongSpinCountMask = 0x0;

	Sleep(5000);

	*(DWORD*)pHvlLongSpinCountMask = 0xFFFFFFFFUL;
	pHvlEnlightenment->running = false;
	*(QWORD*)pCallback = (QWORD)oCallBack;

	//printf("running %i\n", pHvlEnlightenment->running);
	//printf("oCallBack %p\n", oCallBack);
	//for (int i = 5; i >= 0; i--)
	//{
	//	printf("CallBack %p\n", *(QWORD*)pCallback);
	//	printf("exiting in %d...\n", i);
	//	if (i)
	//		Sleep(1000);
	//}
	//pHvlEnlightenment->running = false;
	//*(QWORD*)pCallback = (QWORD)oCallBack;
	//
	//printf("running %i\n", pHvlEnlightenment->running);
	//printf("CallBack %p\n", *(QWORD*)pCallback);
	return STATUS_SUCCESS;
}
