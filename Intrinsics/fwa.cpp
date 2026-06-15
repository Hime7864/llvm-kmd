#include "fwa.hpp"

PHYSICAL_MEMORY_RANGE FWA::fw_range[31];
UINT64 FWA::fw_range_count = 0;
UINT64 FWA::page_idx = 0;

void FWA::Initialize()
{
    if (page_idx || fw_range_count)
        return;

    PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
    pmr += 2;
    do
    {
        if (&pmr[0] && !pmr[0].NumberOfBytes.QuadPart)
            break;
        if (&pmr[1] && !pmr[1].NumberOfBytes.QuadPart)
            break;

        auto high = pmr[1].BaseAddress.QuadPart - 0x1000;
        auto low = pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart + 0x1000;
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
                _mm_invlpg((PVOID)current);
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
                    if (*(UINT16*)current == 0x5A4D &&                  // 'MZ' Hdr
                        *(UINT16*)(current + hdr_offset + 4) == 0x8664) // AMD64
                    {
                        auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
                        


                        //{
                        //    BYTE* ptr_data = (BYTE*)((UINT64)current + 0x1700);
                        //    DbgPrintEx(0, 0, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                        //        ptr_data[0], ptr_data[1], ptr_data[2], ptr_data[3], 
                        //        ptr_data[4], ptr_data[5], ptr_data[6], ptr_data[7],
                        //        ptr_data[8], ptr_data[9], ptr_data[10], ptr_data[11],
                        //        ptr_data[12], ptr_data[13], ptr_data[14], ptr_data[15]
                        //    );
                        //}
                        
                        current += (sizeofimage & ~0xFFF) + (sizeofimage & 0xFFF ? 0x1000 : 0) - 4096;

                        //DbgPrintEx(0, 0, "[smm-dtc] Found firmware region at 0x%llx with size 0x%llx\n", (current - rva) + low, size - (current - rva));



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

        if (idx + (range_size >> 12) > page_idx) // in this block
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
