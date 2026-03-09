#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	FWA::Initialize();
	auto alloc = FWA::ReservePages(1);
	auto pages = FWA::pages_free();
	printf("[+] Allocated page at %llX, %u pages free\n", alloc, pages);
	FWA::Cleanup(1);
	printf("[+] Driver loaded %X\n", __readcr3());
	return STATUS_SUCCESS;
}