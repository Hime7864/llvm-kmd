#include <intrinsics.hpp>

NTSTATUS DriverEntry()
{
	DbgPrintEx(0, 0, "[llvm-kmd]: Hello, World\n");
    return STATUS_SUCCESS;
}