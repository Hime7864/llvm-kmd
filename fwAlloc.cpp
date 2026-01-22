#include "imports.hpp"

PHYSICAL_MEMORY_RANGE FWA::fw_range[31];
UINT64 FWA::fw_range_count;
UINT64 FWA::page_idx;

bool NAKED FWA::is_zero_page(PVOID page)
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

void FWA::Initialize()
{
	page_idx = 0;
	fw_range_count = 0;

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

void FWA::Cleanup()
{
	UINT32 current_page_idx = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		if (current_page_idx > page_idx)
		{
			auto va = MmMapIoSpace(fw_range[i].BaseAddress.QuadPart, fw_range[i].NumberOfBytes.QuadPart, MmNonCached);
			if (va)
			{
				RtlFillMemory((PVOID)va, (SIZE_T)fw_range[i].NumberOfBytes.QuadPart, 0);
				MmUnmapIoSpace((PVOID)va, fw_range[i].NumberOfBytes.QuadPart);
			}
			current_page_idx += fw_range[i].NumberOfBytes.QuadPart >> 12;
		}
		else
		{
			auto delta = page_idx - current_page_idx;
			auto page_offset = delta << 12;
			auto va = MmMapIoSpace(fw_range[i].BaseAddress.QuadPart, page_offset, MmNonCached);
			if (va)
			{
				RtlFillMemory((PVOID)va, (SIZE_T)page_offset, 0);
				MmUnmapIoSpace((PVOID)va, page_offset);
			}
			return;
		}
	}
	return;
}

UINT32 FWA::pages_free()
{
	if (!fw_range_count)
		return 0;
	UINT32 total_pages = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		total_pages += (UINT32)(fw_range[i].NumberOfBytes.QuadPart >> 12);
	}
	return total_pages - page_idx;
}


PHYSICAL_ADDRESS FWA::ReservePages(SIZE_T pages)
{
	if (!fw_range_count)
		return 0;
	UINT32 current_page_idx = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		if (current_page_idx > page_idx)
			current_page_idx += fw_range[i].NumberOfBytes.QuadPart >> 12;
		else
		{
			auto delta = page_idx - current_page_idx;
			auto page_offset = delta << 12;
			if (page_offset > fw_range[i].NumberOfBytes.QuadPart)
				return 0;
			page_idx += pages;
			return fw_range[i].BaseAddress.QuadPart + page_offset;
		}
	}
	return 0;
}

PHYSICAL_ADDRESS FWA::ReserveContiguousPages(SIZE_T pages)
{
	if (!fw_range_count)
		return 0;
	UINT32 current_page_idx = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		if (current_page_idx > page_idx)
			current_page_idx += fw_range[i].NumberOfBytes.QuadPart >> 12;
		else
		{
			auto delta = page_idx - current_page_idx;
			auto page_offset = delta << 12;
			if (page_offset > fw_range[i].NumberOfBytes.QuadPart)
			{
				current_page_idx += fw_range[i].NumberOfBytes.QuadPart >> 12;
			}
			else
			{
				page_idx += pages;
				return fw_range[i].BaseAddress.QuadPart + page_offset;
			}
		}
	}
	return 0;
}