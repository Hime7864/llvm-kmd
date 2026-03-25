#include <intrinsics.hpp>

NTSTATUS DriverEntry()
{
	DbgPrintEx(0, 0, "Driver loaded successfully. %i %i %i\n", 1, 2, 3);
	return STATUS_SUCCESS;
}