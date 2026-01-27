#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NtImports::fn_DbgPrintEx(0, 0, "Driver loaded!\n");
	return STATUS_SUCCESS;
}
