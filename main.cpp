#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("[+] Driver loaded %X\n", __readcr3());
	return STATUS_SUCCESS;
}