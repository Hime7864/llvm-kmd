#include "dtlb.hpp"

UINT64 DTLB::HostedGetVirtual(PHYSICAL_ADDRESS address)
{
    auto va = MmMapIoSpace(address, 0x1000, MmNonCached);
    _mm_invlpg(va);
    return (UINT64)va;
}

void DTLB::HostedFreeVirtual(UINT64 address)
{
    return MmUnmapIoSpace((PVOID)address, 0x1000);
}

MMPTE_HARDWARE DTLB::HostedValidateTranslation(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
    DWORD idx[4]{
        (DWORD)rva.pml4e_index,
        (DWORD)rva.pdpte_index,
        (DWORD)rva.pde_index,
        (DWORD)rva.pte_index};

    auto page_map = (MMPTE_HARDWARE*)HostedGetVirtual(dtb);
    for (int i = 0; i < 4; i++)
    {
        auto entry = &page_map[idx[i]];
        if (!entry->Valid)
        {
            HostedFreeVirtual((UINT64)page_map);
            return {0};
        }
        if (i == 3)
        {
            auto result = *entry;
            HostedFreeVirtual((UINT64)page_map);
            return result;
        }
        auto next_page = entry->PageFrameNumber << 12;
        HostedFreeVirtual((UINT64)page_map);
        page_map = (MMPTE_HARDWARE*)HostedGetVirtual(next_page);
    }
    return {0};
}

BOOLEAN DTLB::HostedCommit4kbMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte)
{
    DWORD idx[4]{
        (DWORD)rva.pml4e_index,
        (DWORD)rva.pdpte_index,
        (DWORD)rva.pde_index,
        (DWORD)rva.pte_index};

    auto page_map = (MMPTE_HARDWARE*)HostedGetVirtual(dtb);

    for (int i = 0; i < 4; i++)
    {
        auto entry = &page_map[idx[i]];
        if (i == 3)
        {
            entry->AsUINT64 = pte.AsUINT64;
            HostedFreeVirtual((UINT64)page_map);
            break;
        }
        if (!entry->Valid)
        {
            auto new_page = FWA::ReservePages(1);
            entry->AsUINT64 = 0;
            entry->Valid = true;
            entry->Dirty1 = true;
            entry->PageFrameNumber = new_page >> 12;
            HostedFreeVirtual((UINT64)page_map);
            page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
        }
        else
        {
            auto new_page = entry->PageFrameNumber << 12;
            HostedFreeVirtual((UINT64)page_map);
            page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
        }
    }
    return TRUE;
}
