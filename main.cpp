#include "imports.hpp"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("DriverEntry called.\n");
	FWA::Initialize();

	printf("FW Allocator initialized. Free pages: %u\n", FWA::pages_free());

	FWA::Cleanup();
	return STATUS_SUCCESS;
}
