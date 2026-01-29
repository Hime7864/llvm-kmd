#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UINT64 base, size;
	Utils::LocateSelf(&base, &size);
	DbgPrintEx(0, 0, "[llvm-kmd] Driver loaded!\n Base: %p\n Size: %i KiBs\n", base, size / 1024);
	return STATUS_SUCCESS;
}