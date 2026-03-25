#include "intrinsics.hpp"

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
        if (strncmp(function_name, export_name, strlen(export_name)) == 0)
        {
            WORD ordinal = *(WORD*)(module + address_of_name_ordinals + i * 2);
            DWORD function_rva = *(DWORD*)(module + address_of_functions + ordinal * 4);
            return module + function_rva;
        }
    }
    return 0;
}

QWORD Utils::GetProcAddressHash(QWORD module, DWORD export_hash)
{
    if (!module || !export_hash)
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
        if (str_hash(function_name, strlen(function_name)) != export_hash)
            continue;

        WORD ordinal = *(WORD*)(module + address_of_name_ordinals + i * 2);
        DWORD function_rva = *(DWORD*)(module + address_of_functions + ordinal * 4);
        return module + function_rva;
    }

    return 0;
}

VOID Utils::GetProcAddressBuffer(QWORD module_base, FUNCTION_TABLE_ENTRY* table, DWORD count)
{
    if (!module_base)
        return;
    QWORD e_lfanew = *(DWORD*)(module_base + 0x3C);
    QWORD pe_header = module_base + e_lfanew;

    if (*(DWORD*)pe_header != 0x4550)
        return;

    QWORD optional_header = pe_header + 0x18;
    DWORD export_rva = *(DWORD*)(optional_header + 0x70);
    if (!export_rva)
        return;

    QWORD export_table = module_base + export_rva;
    DWORD number_of_names = *(DWORD*)(export_table + 0x18);
    DWORD address_of_functions = *(DWORD*)(export_table + 0x1C);
    DWORD address_of_names = *(DWORD*)(export_table + 0x20);
    DWORD address_of_name_ordinals = *(DWORD*)(export_table + 0x24);

    for (DWORD i = 0; i < number_of_names; i++)
    {
        DWORD name_rva = *(DWORD*)(module_base + address_of_names + i * 4);
        const char* function_name = (const char*)(module_base + name_rva);

        for (int j = 0; j < count; j++)
        {
            if (str_hash(function_name, strlen(function_name)) == table[j].Hash)
            {
                WORD ordinal = *(WORD*)(module_base + address_of_name_ordinals + i * 2);
                DWORD function_rva = *(DWORD*)(module_base + address_of_functions + ordinal * 4);
                *(PVOID*)table[j].FunctionPtr = (PVOID)(module_base + function_rva);
            }
        }
    }
    return;
}

NTSTATUS Utils::GetSectionInfo(QWORD module_base, const char* section_name, QWORD* section_address, QWORD* section_size)
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
            QWORD virtual_size = *(DWORD*)(current_section + 0x8);
            *section_address = module_base + virtual_address;
            *section_size = virtual_size & 0xFFFFFFFF;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS Utils::GetSectionInfo(QWORD module_base, DWORD section_hash, QWORD* section_address, QWORD* section_size)
{
    if (!module_base || !section_hash || !section_address || !section_size)
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

        const char* name = (const char*)current_section;
        size_t name_len = 0;
        for (; name_len < 8; ++name_len)
        {
            if (name[name_len] == '\0')
                break;
        }

        if (name_len && str_hash(name, name_len) == section_hash)
        {
            DWORD virtual_address = *(DWORD*)(current_section + 0xC);
            QWORD virtual_size = *(DWORD*)(current_section + 0x8);
            *section_address = module_base + virtual_address;
            *section_size = virtual_size & 0xFFFFFFFF;
            return STATUS_SUCCESS;
        }
    }

    return STATUS_UNSUCCESSFUL;
}

QWORD Utils::GetImageSize(QWORD module_base)
{
    if (!module_base)
        return 0;

    QWORD e_lfanew = *(DWORD*)(module_base + 0x3C);
    QWORD pe_header = module_base + e_lfanew;
    if (*(DWORD*)pe_header != 0x4550)
        return 0;

    QWORD optional_header = pe_header + 0x18;
    return *(DWORD*)(optional_header + 0x38);
}

BOOLEAN Utils::AddressInRange(QWORD address, QWORD range_base, QWORD range_size)
{
    if (!range_base || !range_size)
        return FALSE;
    return address >= range_base && address < (range_base + range_size);
}

BOOLEAN Utils::AddressInSection(QWORD module_base, QWORD address, DWORD section_hash)
{
    QWORD section_base = 0;
    QWORD section_size = 0;

    if (!NT_SUCCESS(GetSectionInfo(module_base, section_hash, &section_base, &section_size)))
        return FALSE;

    return AddressInRange(address, section_base, section_size);
}
