#include "intrinsics.hpp"

PHYSICAL_MEMORY_RANGE FWA::fw_range[31];
UINT64 FWA::fw_range_count = 0;
UINT64 FWA::page_idx = 0;

void FWA::Initialize()
{
	if (page_idx || fw_range_count)
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
			UINT64 first_image_end = 0;
			UINT64 second_image_base = 0;
			for (UINT64 current = rva; current < rva + size; current += 4096)
			{
				__invlpg((PVOID)current);

				UINT64 hdr_offset = *(BYTE*)(current + 0x3C);
				if (*(UINT16*)current == 0x5A4D && // 'MZ' Hdr
					*(UINT16*)(current + hdr_offset + 4) == 0x8664)// AMD64
				{
					auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
					auto aligned_size = (sizeofimage + 0xFFF) & ~0xFFF;
					auto pa_base = (current - rva) + low;
					auto pa_end = pa_base + aligned_size;

					if (!first_image_end)
					{
						first_image_end = pa_end;
					}
					else
					{
						second_image_base = pa_base;
						if (second_image_base > first_image_end)
						{
							fw_range[0].BaseAddress.QuadPart = first_image_end;
							fw_range[0].NumberOfBytes.QuadPart = second_image_base - first_image_end;
							fw_range_count = 1;
						}
						break;
					}

					current += aligned_size - 4096;
				}
			}
			MmUnmapIoSpace((PVOID)rva, size);
			if (fw_range_count)
				return;
		}
		pmr++;
	} while (true);
	return;
}

void FWA::Cleanup(bool zeroMemory)
{
	if (!fw_range_count)
		return;
	UINT64 idx = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		auto range_base = fw_range[i].BaseAddress.QuadPart;
		auto range_size = fw_range[i].NumberOfBytes.QuadPart;

		if (idx + (range_size >> 12) > page_idx)
		{
			auto block_offset = page_idx - idx;
			auto pages_left_in_block = (range_size >> 12) - block_offset;

			auto va = MmMapIoSpace(range_base, block_offset << 12, MmNonCached);
			if (va)
			{
				if (zeroMemory)
					RtlFillMemory((PVOID)va, block_offset << 12, 0);
				MmUnmapIoSpace((PVOID)va, block_offset << 12);

				page_idx = 0;
				fw_range_count = 0;
				return;
			}
			return;
		}
		else
		{
			auto va = MmMapIoSpace(range_base, range_size, MmNonCached);
			if (va)
			{
				if (zeroMemory)
					RtlFillMemory((PVOID)va, (SIZE_T)range_size, 0);
				MmUnmapIoSpace((PVOID)va, range_size);
			}
			idx += (range_size >> 12);
		}
	}
	page_idx = 0;
	fw_range_count = 0;
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
	return total_pages - (UINT32)page_idx;
}

UINT32 FWA::pages_total()
{
	if (!fw_range_count)
		return 0;
	UINT32 total_pages = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		total_pages += (UINT32)(fw_range[i].NumberOfBytes.QuadPart >> 12);
	}
	return total_pages;
}


PHYSICAL_ADDRESS FWA::ReservePages(SIZE_T pages)
{
	if (!fw_range_count)
		return 0;
	UINT64 idx = 0;
	for (int i = 0; i < fw_range_count; i++)
	{
		auto range_base = fw_range[i].BaseAddress.QuadPart;
		auto range_size = fw_range[i].NumberOfBytes.QuadPart;

		if (idx + (range_size >> 12) > page_idx)// in this block
		{
			auto block_offset = page_idx - idx;
			auto pages_left_in_block = (range_size >> 12) - block_offset;
			if (pages_left_in_block > pages)
			{
				page_idx += pages;
				return range_base + (block_offset << 12);
			}
			page_idx += pages_left_in_block;
			idx += (range_size >> 12);
		}
		else
			idx += (range_size >> 12);
	}
	return 0;
}