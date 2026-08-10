#include "fwa.hpp"

PHYSICAL_MEMORY_RANGE FWA::fw_range[31];
UINT64 FWA::fw_range_count = 0;
UINT64 FWA::page_idx = 0;

static void LogPeSections(UINT64 pe_base, UINT64 pe_phys)
{
    UINT32 e_lfanew = *(UINT32*)(pe_base + 0x3C);
    UINT64 nt = pe_base + e_lfanew;
    if (*(UINT32*)nt != 0x4550)
        return;

    UINT16 number_of_sections = *(UINT16*)(nt + 0x6);
    UINT16 size_of_optional_header = *(UINT16*)(nt + 0x14);
    UINT64 section_header = nt + 0x18 + size_of_optional_header;

    DbgPrintEx(0, 0, "[smm-dtc] PE 0x%llx sections=%u\n", pe_phys, number_of_sections);

    for (UINT16 i = 0; i < number_of_sections; ++i)
    {
        UINT64 sec = section_header + (i * 0x28);

        char name[9] = {};
        for (int n = 0; n < 8; ++n)
            name[n] = ((char*)sec)[n];

        UINT32 virtual_size = *(UINT32*)(sec + 0x8);
        UINT32 virtual_addr = *(UINT32*)(sec + 0xC);
        UINT32 raw_size = *(UINT32*)(sec + 0x10);
        UINT32 raw_ptr = *(UINT32*)(sec + 0x14);
        UINT32 chars = *(UINT32*)(sec + 0x24);

        DbgPrintEx(0, 0,
            "[smm-dtc]   [%u] %-8s VA=0x%x VSize=0x%x Raw=0x%x Ptr=0x%x Char=0x%x Phys=0x%llx\n",
            i, name, virtual_addr, virtual_size, raw_size, raw_ptr, chars,
            pe_phys + virtual_addr);
    }
}

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

                    UINT32 e_lfanew = *(UINT32*)(current + 0x3C);
                    if (*(UINT16*)current == 0x5A4D &&                  // 'MZ' Hdr
                        e_lfanew < 0x1000 - 0x58 &&
                        *(UINT16*)(current + e_lfanew + 4) == 0x8664) // AMD64
                    {
                        auto sizeofimage = *(UINT32*)(current + e_lfanew + 0x50);
                        UINT64 pe_phys = (current - rva) + low;

                        DbgPrintEx(0, 0, "[smm-dtc] Found firmware region at 0x%llx with size 0x%x\n",
                            pe_phys, sizeofimage);
                        LogPeSections(current, pe_phys);

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
