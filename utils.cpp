#include "imports.hpp"

QWORD Utils::deref(BYTE count, QWORD address)
{
    if (address)
        return address + *(int*)(address + count) + count + 4;
    return 0;
}

#define TO_BYTE(c) ((c) >= '0' && (c) <= '9' ? (c) - '0' : ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : ((c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : 0)))

QWORD Utils::sig_scan(QWORD scan_start, QWORD max_scan, const char* ida_sig)
{
    BYTE* start = (BYTE*)scan_start;
    BYTE* end = start + max_scan;
    if (start > end) return 0;

    for (BYTE* current = start; current < end; ++current)
    {
        const BYTE* data = current;
        const char* pattern = ida_sig;
        int matched = 1;
        while (*pattern)
        {
            if (*pattern == ' ') { ++pattern; continue; }
            if (*pattern == '?') { ++data; ++pattern; if (*pattern == '?') ++pattern; continue; }
            BYTE byte = (TO_BYTE(*pattern) << 4) | TO_BYTE(*(pattern + 1));
            if (*data != byte) { matched = 0; break; }
            data++; pattern += 2;
        }
        if (matched) return (QWORD)current;
    }

    return 0;
}

QWORD Utils::sig_scan_safe(QWORD scan_start, QWORD max_scan, const char* ida_sig)
{
    BYTE* start = (BYTE*)scan_start;
    BYTE* end = start + max_scan;
    if (start > end) return 0;

    for (BYTE* current = start; current < end; ++current)
    {
        const BYTE* data = current;
        const char* pattern = ida_sig;
        int matched = 1;
        while (*pattern)
        {
            if (*pattern == ' ') { ++pattern; continue; }
            if (*pattern == '?') { ++data; ++pattern; if (*pattern == '?') ++pattern; continue; }
            BYTE byte = (TO_BYTE(*pattern) << 4) | TO_BYTE(*(pattern + 1));

            BYTE _data = 0;
			SIZE_T bytesRead;
			auto status = MmCopyMemory(&_data, (QWORD)data, sizeof(BYTE), MM_COPY_MEMORY_VIRTUAL, &bytesRead);

            if (!NT_SUCCESS(status) || _data != byte) { matched = 0; break; }
            data++; pattern += 2;
        }
        if (matched) return (QWORD)current;
    }

    return 0;
}

QWORD Utils::sig_scan_reverse(QWORD scan_start, QWORD max_scan, const char* ida_sig)
{
    BYTE* start = (BYTE*)scan_start;
    BYTE* end = (BYTE*)(scan_start - max_scan);
    if (start < end) return 0;

    for (BYTE* current = start; current >= end; --current)
    {
        const BYTE* data = current;
        const char* pattern = ida_sig;
        int matched = 1;
        while (*pattern)
        {
            if (*pattern == ' ') { ++pattern; continue; }
            if (*pattern == '?') { ++data; ++pattern; if (*pattern == '?') ++pattern; continue; }
            BYTE byte = (TO_BYTE(*pattern) << 4) | TO_BYTE(*(pattern + 1));
            if (*data != byte) { matched = 0; break; }
            data++; pattern += 2;
        }
        if (matched) return (QWORD)current;
    }
    return 0;
}

#undef TO_BYTE

QWORD Utils::GetKernelBase()
{
    auto sig = sig_scan(MSR::read_lstar() & ~0xFFFFFULL, 0xA00000ULL, "66 89 05 ? ? ? ? 48 8D 05 ? ? ? ? 48 89");
    if (!sig) return 0;
    return *(QWORD*)(*(QWORD*)deref(3, sig + 0x07) + 0x30);
}

QWORD Utils::GetProcAddress(QWORD module, const char* export_name)
{
    if (!module)
        return 0;
    QWORD e_lfanew = *(DWORD*)(module + 0x3C);
    QWORD pe_header = module + e_lfanew;

    if (*(DWORD*)pe_header != 0x4550)
        return 0;

    QWORD optional_header = pe_header + 0x18;
    DWORD export_rva = *(DWORD*)(optional_header + 0x70);
    if (!export_rva)
        return 0;

    QWORD export_table = module + export_rva;
    DWORD number_of_names = *(DWORD*)(export_table + 0x18);
    DWORD address_of_functions = *(DWORD*)(export_table + 0x1C);
    DWORD address_of_names = *(DWORD*)(export_table + 0x20);
    DWORD address_of_name_ordinals = *(DWORD*)(export_table + 0x24);

    for (DWORD i = 0; i < number_of_names; i++)
    {
        DWORD name_rva = *(DWORD*)(module + address_of_names + i * 4);
        const char* function_name = (const char*)(module + name_rva);
        if (strcmp(function_name, export_name, strlen(export_name)) == 0)
        {
            WORD ordinal = *(WORD*)(module + address_of_name_ordinals + i * 2);
            DWORD function_rva = *(DWORD*)(module + address_of_functions + ordinal * 4);
            return module + function_rva;
        }
    }
    return 0;
}

NTSTATUS Utils::GetSectionInfo(QWORD module_base, const char* section_name, QWORD* section_address, DWORD* section_size)
{
    if (!module_base || !section_name || !section_address || !section_size)
        return -1;
    QWORD e_lfanew = *(DWORD*)(module_base + 0x3C);
    QWORD pe_header = module_base + e_lfanew;
    if (*(DWORD*)pe_header != 0x4550)
        return STATUS_UNSUCCESSFUL;
    WORD number_of_sections = *(WORD*)(pe_header + 0x6);
    WORD size_of_optional_header = *(WORD*)(pe_header + 0x14);
    QWORD section_header = pe_header + 0x18 + size_of_optional_header;
    for (WORD i = 0; i < number_of_sections; ++i)
    {
        QWORD current_section = section_header + (i * 0x28);
        if (memcmp((void*)current_section, (void*)section_name, strlen(section_name)) == 0)
        {
            DWORD virtual_address = *(DWORD*)(current_section + 0xC);
            DWORD virtual_size = *(DWORD*)(current_section + 0x8);
            *section_address = module_base + virtual_address;
            *section_size = virtual_size;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS ReadPhysicalAddress(QWORD TargetAddress, PVOID lpBuffer, QWORD Size, QWORD* BytesRead = nullptr)
{
    if (BytesRead)
        return MmCopyMemory(lpBuffer, TargetAddress, Size, MM_COPY_MEMORY_PHYSICAL, BytesRead);
    QWORD _BytesRead = 0;
    return !MmCopyMemory(lpBuffer, TargetAddress, Size, MM_COPY_MEMORY_PHYSICAL, &_BytesRead);
}

template <typename T>
T PhysRead(QWORD pa)
{
    T buffer;
    ReadPhysicalAddress(pa, &buffer, sizeof(T));
    return buffer;
}

QWORD Utils::GetPPTE(QWORD virtual_address)
{
    QWORD cr3 = __readcr3();
    if (cr3 & 0xFFF)
        cr3 &= ~0xFFF; // Ensure CR3 is page-aligned

    auto va = virtual_address;

    // Validate canonical address (48-bit virtual address space)
    if (((va >> 47) & 0x1FFFF) != 0 && ((va >> 47) & 0x1FFFF) != 0x1FFFF)
        return  { 0 };

    MMPTE_HARDWARE PTE;
    PTE = PhysRead<MMPTE_HARDWARE>(cr3 + (((va >> 39) & 0x1ff) * 8));
    if (!PTE.Valid)
        return { 0 };

    // PDPT
    QWORD next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 30) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // Check for 1GB page
    if (PTE.LargePage)
        return next_addr + (((va >> 30) & 0x1ff) * 8);

    // PDT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 21) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // Check for 2MB page
    if (PTE.LargePage)
        return next_addr + (((va >> 21) & 0x1ff) * 8);

    // PT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 12) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // 4KB page
    return next_addr + (((va >> 12) & 0x1ff) * 8);
}

MMPTE_HARDWARE Utils::GetPTE(QWORD va)
{
    QWORD cr3 = __readcr3();
    if (cr3 & 0xFFF)
        cr3 &= ~0xFFF; // Ensure CR3 is page-aligned

    // Validate canonical address (48-bit virtual address space)
    if (((va >> 47) & 0x1FFFF) != 0 && ((va >> 47) & 0x1FFFF) != 0x1FFFF)
        return  { 0 };

    MMPTE_HARDWARE PTE;
    PTE = PhysRead<MMPTE_HARDWARE>(cr3 + (((va >> 39) & 0x1ff) * 8));
    if (!PTE.Valid)
        return { 0 };

    // PDPT
    QWORD next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 30) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // Check for 1GB page
    if (PTE.LargePage)
        return PTE;

    // PDT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 21) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // Check for 2MB page
    if (PTE.LargePage)
        return PTE;

    // PT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 12) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };

    // 4KB page
    return PTE;
}

NTSTATUS Utils::SelfModuleBase(QWORD* module_base, QWORD* module_size)
{
    if (!module_base)
        return STATUS_INVALID_PARAMETER;
    *module_base = 0;


	auto first_page = ((UINT64)memcpy) & ~0xFFFULL;
    auto last_page = sig_scan_safe(first_page, 0x200000, "52 65 74 70 6F 6C 69 6E 65 56 31");
	if(!last_page)
		return STATUS_UNSUCCESSFUL;

    if(last_page & 0xFFF)
        last_page = (last_page & ~0xFFFULL) + 0x1000;

	*module_base = first_page;
    if(module_size)
		*module_size = last_page - first_page;
    return STATUS_SUCCESS;
}

bool Utils::IsAddressValid(QWORD address)
{
    MMPTE_HARDWARE pte = GetPTE(address);
    return pte.Valid;
}

MMPTE_HARDWARE Utils::GetPTE(QWORD ctx, QWORD va)
{
    QWORD cr3 = ctx;
    if (cr3 & 0xFFF)
        cr3 &= ~0xFFF; // Ensure CR3 is page-aligned
    // Validate canonical address (48-bit virtual address space)
    if (((va >> 47) & 0x1FFFF) != 0 && ((va >> 47) & 0x1FFFF) != 0x1FFFF)
        return  { 0 };
    MMPTE_HARDWARE PTE;
    PTE = PhysRead<MMPTE_HARDWARE>(cr3 + (((va >> 39) & 0x1ff) * 8));
    if (!PTE.Valid)
        return { 0 };
    // PDPT
    QWORD next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 30) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };
    // Check for 1GB page
    if (PTE.LargePage)
        return PTE;
    // PDT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 21) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };
    // Check for 2MB page
    if (PTE.LargePage)
        return PTE;
    // PT
    next_addr = (QWORD)PTE.PageFrameNumber << 12;
    PTE = PhysRead<MMPTE_HARDWARE>(next_addr + (((va >> 12) & 0x1ff) * 8));
    if (!PTE.Valid)
        return  { 0 };
    // 4KB page
    return PTE;
}

bool Utils::IsAddressValid(QWORD ctx, QWORD address)
{
    MMPTE_HARDWARE pte = GetPTE(ctx, address);
    return pte.Valid;
}

