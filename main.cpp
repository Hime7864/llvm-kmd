#include "imports.hpp"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("DriverEntry called\n");
	return STATUS_SUCCESS;
}
