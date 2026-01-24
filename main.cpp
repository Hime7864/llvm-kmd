#include "imports.hpp"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	QWORD host_driver_base = 0;
	QWORD host_driver_size = 0;
	Utils::SelfModuleBase(&host_driver_base, &host_driver_size);
	printf("DriverEntry %p %p\n", host_driver_base, host_driver_size);
	return STATUS_SUCCESS;
}
