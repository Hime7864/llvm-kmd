#include "imports.hpp"


class ALIGN(512) FW_MEMORY
{
private:
	PHYSICAL_MEMORY_RANGE fw_range[31];
	UINT32 fw_range_count = 0;

	bool NAKED is_zero_page(PVOID page)
	{
		__asm 
		{
			vpxord zmm0, zmm0, zmm0
			mov eax, 4096 / 64
		loop:
			vporq zmm0, zmm0, [rdx]
			add rdx, 64
			vptestmq k1, zmm0, zmm0
			kortestw k1, k1
			jnz nonzero
			dec eax 
			jnz loop
			mov al, 1
			ret
		nonzero:
			xor eax, eax
			ret
		}
	}
	
	void locate_ranges()
	{
		if (!this)
			return;

		PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
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
					if (is_zero_page((PVOID)current))
					{
						if (!start_rva)
							start_rva = current;
					}
					else
					{
						if (fw_range_count < 31 && start_rva && (current - start_rva) > 0x200000)
						{
							fw_range[fw_range_count].BaseAddress.QuadPart = (start_rva - rva) + low;
							fw_range[fw_range_count].NumberOfBytes.QuadPart = current - start_rva;
							fw_range_count++;
						}
						start_rva = 0;
						UINT64 hdr_offset = *(BYTE*)(current + 0x3C);
						if (*(UINT16*)current == 0x5A4D && // 'MZ' Hdr
							*(UINT16*)(current + hdr_offset + 4) == 0x8664)// AMD64
						{
							auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
							current += (sizeofimage & ~0xFFF) + (sizeofimage & 0xFFF ? 0x1000 : 0) - 4096;

							should_exit = true;
						}
					}
				}
				MmUnmapIoSpace((PVOID)rva, size);
				if (should_exit)
					return;
			}
			pmr++;
		} while (true);
		return;
	}

public:

	UINT32 GetRangeCount()
	{
		if (!fw_range_count)
			locate_ranges();

		return fw_range_count;
	}

	PHYSICAL_MEMORY_RANGE* GetRanges()
	{
		if (!fw_range_count)
			locate_ranges();

		return fw_range;
	}
};



NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	return STATUS_SUCCESS;
}