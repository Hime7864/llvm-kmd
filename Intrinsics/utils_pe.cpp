#include "utils.hpp"

UINT64 Utils::GetProcAddress(UINT64 module, const char* export_name)
{
    if (!module)
        return 0;
    UINT64 e_lfanew = *(UINT32*)(module + 0x3C);
    UINT64 pe_header = module + e_lfanew;

    if (*(UINT32*)pe_header != 0x4550)
        return 0;

    UINT64 optional_header = pe_header + 0x18;
    UINT32 export_rva = *(UINT32*)(optional_header + 0x70);
    if (!export_rva)
        return 0;

    UINT64 export_table = module + export_rva;
    UINT32 number_of_names = *(UINT32*)(export_table + 0x18);
    UINT32 address_of_functions = *(UINT32*)(export_table + 0x1C);
    UINT32 address_of_names = *(UINT32*)(export_table + 0x20);
    UINT32 address_of_name_ordinals = *(UINT32*)(export_table + 0x24);

    for (UINT32 i = 0; i < number_of_names; i++)
    {
        UINT32 name_rva = *(UINT32*)(module + address_of_names + i * 4);
        const char* function_name = (const char*)(module + name_rva);
        if (strncmp(function_name, export_name, strlen(export_name)) == 0)
        {
            UINT16 ordinal = *(UINT16*)(module + address_of_name_ordinals + i * 2);
            UINT32 function_rva = *(UINT32*)(module + address_of_functions + ordinal * 4);
            return module + function_rva;
        }
    }
    return 0;
}

UINT64 Utils::GetProcAddressHash(UINT64 module, UINT32 export_hash)
{
    if (!module || !export_hash)
        return 0;

    UINT64 e_lfanew = *(UINT32*)(module + 0x3C);
    UINT64 pe_header = module + e_lfanew;

    if (*(UINT32*)pe_header != 0x4550)
        return 0;

    UINT64 optional_header = pe_header + 0x18;
    UINT32 export_rva = *(UINT32*)(optional_header + 0x70);
    if (!export_rva)
        return 0;

    UINT64 export_table = module + export_rva;
    UINT32 number_of_names = *(UINT32*)(export_table + 0x18);
    UINT32 address_of_functions = *(UINT32*)(export_table + 0x1C);
    UINT32 address_of_names = *(UINT32*)(export_table + 0x20);
    UINT32 address_of_name_ordinals = *(UINT32*)(export_table + 0x24);

    for (UINT32 i = 0; i < number_of_names; i++)
    {
        UINT32 name_rva = *(UINT32*)(module + address_of_names + i * 4);
        const char* function_name = (const char*)(module + name_rva);
        if (str_hash(function_name, strlen(function_name)) != export_hash)
            continue;

        UINT16 ordinal = *(UINT16*)(module + address_of_name_ordinals + i * 2);
        UINT32 function_rva = *(UINT32*)(module + address_of_functions + ordinal * 4);
        return module + function_rva;
    }

    return 0;
}

VOID Utils::GetProcAddressBuffer(UINT64 module_base, FUNCTION_TABLE_ENTRY* table, UINT32 count)
{
    if (!module_base)
        return;
    UINT64 e_lfanew = *(UINT32*)(module_base + 0x3C);
    UINT64 pe_header = module_base + e_lfanew;

    if (*(UINT32*)pe_header != 0x4550)
        return;

    UINT64 optional_header = pe_header + 0x18;
    UINT32 export_rva = *(UINT32*)(optional_header + 0x70);
    if (!export_rva)
        return;

    UINT64 export_table = module_base + export_rva;
    UINT32 number_of_names = *(UINT32*)(export_table + 0x18);
    UINT32 address_of_functions = *(UINT32*)(export_table + 0x1C);
    UINT32 address_of_names = *(UINT32*)(export_table + 0x20);
    UINT32 address_of_name_ordinals = *(UINT32*)(export_table + 0x24);

    for (UINT32 i = 0; i < number_of_names; i++)
    {
        UINT32 name_rva = *(UINT32*)(module_base + address_of_names + i * 4);
        const char* function_name = (const char*)(module_base + name_rva);

        for (int j = 0; j < count; j++)
        {
            if (str_hash(function_name, strlen(function_name)) == table[j].Hash)
            {
                UINT16 ordinal = *(UINT16*)(module_base + address_of_name_ordinals + i * 2);
                UINT32 function_rva = *(UINT32*)(module_base + address_of_functions + ordinal * 4);
                *(PVOID*)table[j].FunctionPtr = (PVOID)(module_base + function_rva);
            }
        }
    }
    return;
}

NTSTATUS Utils::GetSectionInfo(UINT64 module_base, const char* section_name, UINT64* section_address, UINT64* section_size)
{
    if (!module_base || !section_name || !section_address || !section_size)
        return -1;
    UINT64 e_lfanew = *(UINT32*)(module_base + 0x3C);
    UINT64 pe_header = module_base + e_lfanew;
    if (*(UINT32*)pe_header != 0x4550)
        return STATUS_UNSUCCESSFUL;
    UINT16 number_of_sections = *(UINT16*)(pe_header + 0x6);
    UINT16 size_of_optional_header = *(UINT16*)(pe_header + 0x14);
    UINT64 section_header = pe_header + 0x18 + size_of_optional_header;
    for (UINT16 i = 0; i < number_of_sections; ++i)
    {
        UINT64 current_section = section_header + (i * 0x28);
        if (memcmp((void*)current_section, (void*)section_name, strlen(section_name)) == 0)
        {
            UINT32 virtual_address = *(UINT32*)(current_section + 0xC);
            UINT64 virtual_size = *(UINT32*)(current_section + 0x8);
            *section_address = module_base + virtual_address;
            *section_size = virtual_size & 0xFFFFFFFF;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_UNSUCCESSFUL;
}

NTSTATUS Utils::GetSectionInfo(UINT64 module_base, UINT32 section_hash, UINT64* section_address, UINT64* section_size)
{
    if (!module_base || !section_hash || !section_address || !section_size)
        return -1;

    UINT64 e_lfanew = *(UINT32*)(module_base + 0x3C);
    UINT64 pe_header = module_base + e_lfanew;
    if (*(UINT32*)pe_header != 0x4550)
        return STATUS_UNSUCCESSFUL;

    UINT16 number_of_sections = *(UINT16*)(pe_header + 0x6);
    UINT16 size_of_optional_header = *(UINT16*)(pe_header + 0x14);
    UINT64 section_header = pe_header + 0x18 + size_of_optional_header;

    for (UINT16 i = 0; i < number_of_sections; ++i)
    {
        UINT64 current_section = section_header + (i * 0x28);

        const char* name = (const char*)current_section;
        size_t name_len = 0;
        for (; name_len < 8; ++name_len)
        {
            if (name[name_len] == '\0')
                break;
        }

        if (name_len && str_hash(name, name_len) == section_hash)
        {
            UINT32 virtual_address = *(UINT32*)(current_section + 0xC);
            UINT64 virtual_size = *(UINT32*)(current_section + 0x8);
            *section_address = module_base + virtual_address;
            *section_size = virtual_size & 0xFFFFFFFF;
            return STATUS_SUCCESS;
        }
    }

    return STATUS_UNSUCCESSFUL;
}

UINT64 Utils::GetImageSize(UINT64 module_base)
{
    if (!module_base)
        return 0;

    UINT64 e_lfanew = *(UINT32*)(module_base + 0x3C);
    UINT64 pe_header = module_base + e_lfanew;
    if (*(UINT32*)pe_header != 0x4550)
        return 0;

    UINT64 optional_header = pe_header + 0x18;
    return *(UINT32*)(optional_header + 0x38);
}

BOOLEAN Utils::AddressInRange(UINT64 address, UINT64 range_base, UINT64 range_size)
{
    if (!range_base || !range_size)
        return FALSE;
    return address >= range_base && address < (range_base + range_size);
}

BOOLEAN Utils::AddressInSection(UINT64 module_base, UINT64 address, UINT32 section_hash)
{
    UINT64 section_base = 0;
    UINT64 section_size = 0;

    if (!NT_SUCCESS(GetSectionInfo(module_base, section_hash, &section_base, &section_size)))
        return FALSE;

    return AddressInRange(address, section_base, section_size);
}