#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	SEGMENT_REGISTER gdtr;
	__sgdt(&gdtr);
	printf("GDTR Base: 0x%llx, Limit: 0x%04x\n", gdtr.Base, gdtr.Limit);
	SEGMENT_REGISTER idtr;
	__sidt(&idtr);
	printf("IDTR Base: 0x%llx, Limit: 0x%04x\n", idtr.Base, idtr.Limit);
	return STATUS_SUCCESS;
}