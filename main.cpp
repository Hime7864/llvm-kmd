#include "imports.hpp"

void ipicall()
{
	printf("Core %u called!\n", CPUID::current_core_number());
	return;
}

void test_callback()
{
	PsTerminateSystemThread(0);
	return;
}

QWORD NOINLINE NAKED CallOnAllCores(PVOID func, PVOID arg)
{
	__asm {
		sub rsp, 8h
		mov rax, fn_PsTerminateSystemThread
		mov[rsp], rax
		mov rax, fn_KeIpiGenericCall
		jmp rax
	}
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto test = CallOnAllCores(ipicall, nullptr);
	return STATUS_SUCCESS;
}