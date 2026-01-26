#include "imports.hpp"



NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	QWORD base, size;
	Utils::LocateSelf(&base, &size);
	printf("driver base: %p size: %i bytes\n", base, size);
	auto pages = size / 0x1000;
	for(int i=0;i<pages;i++)
	{
		printf("  page[%02d] %010X\n", i, Utils::LinearTranslate(base + (i * 0x1000)));
	}

	return STATUS_SUCCESS;
}
