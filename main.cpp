#include "imports.hpp"


bool NAKED is_zero_page2(PVOID page)
{
	__asm
	{
		vpxord zmm0, zmm0, zmm0
		mov eax, 4096 / 64
		loop:
		vporq zmm0, zmm0, [rcx]
			add rcx, 64
			vptestmq k1, zmm0, zmm0
			kortestw k1, k1
			jnz nonzero
			dec eax
			jnz loop
			mov al, 1
			ret
			nonzero :
		xor eax, eax
			ret
	}
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
	pmr += 3;
	do
	{
		if (&pmr[0] && !pmr[0].NumberOfBytes.QuadPart)
			break;
		if (&pmr[1] && !pmr[1].NumberOfBytes.QuadPart)
			break;

		auto high = pmr[1].BaseAddress.QuadPart;
		auto low = pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart;
		auto size = high - low;

		if (size > 0x40000000)
			size = 0x40000000;

		auto rva = (UINT64)MmMapIoSpace(low, size, MmNonCached);
		if (rva)
		{
			UINT64 start_rva = 0x0;
			bool should_exit = false;
			for (UINT64 current = rva; current < rva + size; current += 4096)
			{
				__invlpg((PVOID)current);
				if (is_zero_page2((PVOID)current))
				{
					if (!start_rva)
						start_rva = current;
				}
				else
				{
					start_rva = 0;
					UINT64 hdr_offset = *(BYTE*)(current + 0x3C);
					if (*(UINT16*)current == 0x5A4D && // 'MZ' Hdr
						*(UINT16*)(current + hdr_offset + 4) == 0x8664)// AMD64
					{
						auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
						printf("Found image at PA: 0x%llx, Size: 0x%x\n", (current - rva) + low, sizeofimage);
						should_exit = true;
						break;
					}
				}
			}
			MmUnmapIoSpace((PVOID)rva, size);
			if (should_exit)
				return STATUS_SUCCESS;
		}
		pmr++;
	} while (true);
	return STATUS_SUCCESS;
}
