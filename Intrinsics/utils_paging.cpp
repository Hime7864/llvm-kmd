#include "utils.hpp"

NTSTATUS Utils::ReadPhysical(PHYSICAL_ADDRESS address, PVOID buffer, SIZE_T size)
{
    SIZE_T bytesRead = 0;
    auto irql = _mm_readcr8();
    _mm_writecr8(1);

    auto status = MmCopyMemory(
        buffer,
        address,
        size,
        MM_COPY_MEMORY_PHYSICAL,
        &bytesRead);

    if (status == STATUS_INVALID_ADDRESS)
    {
        auto va = MmMapIoSpace(
            address,
            size,
            MmNonCached);
        if (va)
        {
            RtlCopyMemory(
                buffer,
                va,
                size);
            status = STATUS_SUCCESS;
            MmUnmapIoSpace(va, size);
        }
    }

    _mm_writecr8(irql);
    return status;
}

PHYSICAL_ADDRESS Utils::LinearTranslatePPte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
    UINT64 idx[4]{
        8ull * rva.pml4e_index,
        8ull * rva.pdpte_index,
        8ull * rva.pde_index,
        8ull * rva.pte_index};

    PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
    for (int i = 0; i < 4; i++)
    {
        auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
        if (!pte.Valid || !pte.PageFrameNumber)
            return page_map + idx[i];
        switch (i)
        {
        case 1: // 1gb page
        {
            if (pte.LargePage)
            {
                return page_map + idx[i];
            }
        }
        break;
        case 2: // 2mb page
        {
            if (pte.LargePage)
            {
                return page_map + idx[i];
            }
        }
        break;
        case 3: // 4kb page
        {
            return page_map + idx[i];
        }
        break;
        }
        page_map = pte.PageFrameNumber << 12;
    }
    return {0};
}

PHYSICAL_ADDRESS Utils::LinearTranslatePPte(LINEAR_ADDRESS rva)
{
    return LinearTranslatePPte(_mm_readcr3(), rva);
}

MMPTE_HARDWARE Utils::LinearTranslatePte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
    UINT64 idx[4]{
        8ull * rva.pml4e_index,
        8ull * rva.pdpte_index,
        8ull * rva.pde_index,
        8ull * rva.pte_index};

    PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
    for (int i = 0; i < 4; i++)
    {
        auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
        if (!pte.Valid)
            return pte;
        switch (i)
        {
        case 1: // 1gb page
        {
            if (pte.LargePage)
            {
                return pte;
            }
        }
        break;
        case 2: // 2mb page
        {
            if (pte.LargePage)
            {
                return pte;
            }
        }
        break;
        case 3: // 4kb page
        {
            return pte;
        }
        break;
        }
        page_map = pte.PageFrameNumber << 12;
    }
    return {0};
}

MMPTE_HARDWARE Utils::LinearTranslatePte(LINEAR_ADDRESS rva)
{
    return LinearTranslatePte(_mm_readcr3(), rva);
}

PHYSICAL_ADDRESS Utils::LinearTranslate(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
    UINT64 idx[4]{
        8ull * rva.pml4e_index,
        8ull * rva.pdpte_index,
        8ull * rva.pde_index,
        8ull * rva.pte_index};

    PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
    for (int i = 0; i < 4; i++)
    {
        auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
        if (!pte.Valid)
            return 0;
        switch (i)
        {
        case 1: // 1gb page
        {
            if (pte.LargePage)
            {
                return (pte.PageFrameNumber << 12) + rva.huge_offset;
            }
        }
        break;
        case 2: // 2mb page
        {
            if (pte.LargePage)
            {
                return (pte.PageFrameNumber << 12) + rva.large_offset;
            }
        }
        break;
        case 3: // 4kb page
        {
            return (pte.PageFrameNumber << 12) + rva.offset;
        }
        break;
        }
        page_map = pte.PageFrameNumber << 12;
    }
    return 0;
}

PHYSICAL_ADDRESS Utils::LinearTranslate(LINEAR_ADDRESS rva)
{
    return LinearTranslate(_mm_readcr3(), rva);
}

NTSTATUS Utils::ReadLinear(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)
{
    UINT64 idx[4]{
        8ull * rva.pml4e_index,
        8ull * rva.pdpte_index,
        8ull * rva.pde_index,
        8ull * rva.pte_index};

    PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
    for (int i = 0; i < 4; i++)
    {
        auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
        if (!pte.Valid)
            return STATUS_PTE_NOT_VALID;
        if (!pte.PageFrameNumber)
            return STATUS_PFN_NOT_PRESENT;
        switch (i)
        {
        case 1: // 1gb page
        {
            if (pte.LargePage)
            {
                PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.huge_offset;
                return ReadPhysical(target_address, buffer, size);
            }
        }
        break;
        case 2: // 2mb page
        {
            if (pte.LargePage)
            {
                PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.large_offset;
                return ReadPhysical(target_address, buffer, size);
            }
        }
        break;
        case 3: // 4kb page
        {
            PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.offset;
            return ReadPhysical(target_address, buffer, size);
        }
        break;
        }
        page_map = pte.PageFrameNumber << 12;
    }
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS Utils::ReadLinearSafe(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size, SIZE_T* bytes_read)
{
    if (!buffer)
        return STATUS_INVALID_PARAMETER;

    if (bytes_read)
        *bytes_read = 0;

    BYTE* output = (BYTE*)buffer;
    UINT64 current = rva.AsUINT64;
    SIZE_T remaining = size;

    while (remaining)
    {
        LINEAR_ADDRESS current_rva(current);
        PHYSICAL_ADDRESS physical = LinearTranslate(dtb, current_rva);
        if (!physical)
            return STATUS_PTE_NOT_VALID;

        SIZE_T chunk = min((SIZE_T)(PAGE_SIZE - current_rva.offset), remaining);
        auto status = ReadPhysical(physical, output, chunk);
        if (!NT_SUCCESS(status))
            return status;

        output += chunk;
        current += chunk;
        remaining -= chunk;
        if (bytes_read)
            *bytes_read += chunk;
    }

    return STATUS_SUCCESS;
}

NTSTATUS Utils::ReadLinear(LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)
{
    return ReadLinear(_mm_readcr3(), rva, buffer, size);
}

NTSTATUS Utils::ReadLinearSafe(LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size, SIZE_T* bytes_read)
{
    return ReadLinearSafe(_mm_readcr3(), rva, buffer, size, bytes_read);
}

BOOLEAN Utils::RvaValid(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
    return LinearTranslate(dtb, rva) != 0;
}

BOOLEAN Utils::RvaValid(LINEAR_ADDRESS rva)
{
    return RvaValid(_mm_readcr3(), rva);
}