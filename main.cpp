#include "imports.hpp"



NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	FWA::Initialize();

	printf("total fw pages: %u\n", FWA::pages_total());

	FWA::Cleanup();

	return STATUS_SUCCESS;
}
