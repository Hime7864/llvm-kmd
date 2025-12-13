#include "imports.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	DbgPrintEx(0, 0, "Driver loaded!\n");

	return STATUS_SUCCESS;
}