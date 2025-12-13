#include "imports.h"

QWORD Utils::deref(BYTE count, QWORD address)
{
    if (address)
        return address + *(int*)(address + count) + count + 4;
    return 0;
}
#define TO_BYTE(x) ((x >= '0' && x <= '9') ? (x - '0') : ((x >= 'A' && x <= 'F') ? (x - 'A' + 10) : ((x >= 'a' && x <= 'f') ? (x - 'a' + 10) : 0)))

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
    if(!sig) return 0;
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

    for (DWORD i = 0; i < number_of_names; ++i)
    {
        DWORD name_rva = *(DWORD*)(module + address_of_names + i * 4);
        const char* function_name = (const char*)(module + name_rva);
        const char* p1 = export_name;
        const char* p2 = function_name;
        while (*p1 && *p2 && *p1 == *p2) { ++p1; ++p2; }
        if (*p1 == '\0' && *p2 == '\0')
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
