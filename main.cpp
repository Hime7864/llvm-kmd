#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	dTlbPoison.Initialize();

	dTlbPoison.ReserveRvaPoison(67ull, FWA::ReservePages(1));

	dTlbPoison.CommitRvaPoison(67ull);

	dTlbPoison.Cleanup();
	return STATUS_SUCCESS;
}