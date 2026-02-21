#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	SEGMENT_REGISTER gdtr;
	__sgdt(&gdtr);
	printf("GDTR Base: 0x%llx, Limit: 0x%04x\n", gdtr.Base, gdtr.Limit);
	SEGMENT_REGISTER idtr;
	__sidt(&idtr);
	printf("IDTR Base: 0x%llx, Limit: 0x%04x\n", idtr.Base, idtr.Limit);

	auto gdtEntry = (SEGMENT_DESCRIPTOR*)gdtr.Base;
	auto idtEntry = (IDT_GATE*)idtr.Base;

	auto ist = (INTERUPT_STACK_TABLE*)gdtEntry[4].base();
	printf("ist->IOPB: %x\n", ist->IOPB);

	printf("idtEntry[2].dpl %x\n", idtEntry[2].dpl);
	printf("idtEntry[2].ist %x\n", idtEntry[2].ist);
	printf("idtEntry[2].p %x\n", idtEntry[2].p);
	printf("idtEntry[2].selector %x\n", idtEntry[2].selector);
	printf("idtEntry[2].type %x\n", idtEntry[2].type);

	return STATUS_SUCCESS;
}