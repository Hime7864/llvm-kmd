#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("%i\n", MSR::PSTATE(0).get_frequency_mhz());
	return STATUS_SUCCESS;
}