#include "imports.hpp"

QWORD Utils::ResolveRel32(BYTE count, QWORD address)
{
    if (address)
        return address + *(int*)(address + count) + count + 4;
    return 0;
}

#define TO_BYTE(c) ((c) >= '0' && (c) <= '9' ? (c) - '0' : ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : ((c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : 0)))

template<size_t N, size_t Size>
QWORD SigScan(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)
{
	UCHAR* start = (UCHAR*)scan_start;
	UCHAR* end = start + max_scan;
	if (start > end) return 0;

	for (UCHAR* current = start; current < end; ++current)
	{
		bool matched = true;
		for (size_t i = 0; i < pat.size; i++)
		{
			if (pat.mask[i] == 0x00)
				continue;

			if (current[i] != pat.bytes[i])
			{
				matched = false;
				break;
			}
		}

		if (matched)
			return (QWORD)current;
	}

	return 0;
}

QWORD Utils::SigScan(QWORD scan_start, QWORD max_scan, const char* ida_sig)
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

template<size_t N, size_t Size>
QWORD Utils::SigScan_s(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)
{
	UCHAR* start = (UCHAR*)scan_start;
	UCHAR* end = start + max_scan;
	if (start > end) return 0;
	for (UCHAR* current = start; current < end; ++current)
	{
		bool matched = true;
		for (size_t i = 0; i < pat.size; i++)
		{
			if (pat.mask[i] == 0x00)
				continue;
			BYTE _data = 0;
			SIZE_T bytesRead;
			auto status = MmCopyMemory(&_data, (QWORD)(current + i), sizeof(BYTE), MM_COPY_MEMORY_VIRTUAL, &bytesRead);
			if (!NT_SUCCESS(status) || _data != pat.bytes[i])
			{
				matched = false;
				break;
			}
		}
		if (matched)
			return (QWORD)current;
	}
	return 0;
}

QWORD Utils::SigScan_s(QWORD scan_start, QWORD max_scan, const char* ida_sig)
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

#undef TO_BYTE

QWORD Utils::GetKernelBase()
{
    auto sig = SigScan(MSR::LSTAR() & ~0xFFFFFULL, 0xA00000ULL, pattern("66 89 05 ? ? ? ? 48 8D 05 ? ? ? ? 48 89"));
    if (!sig) return 0;
    return *(QWORD*)(*(QWORD*)ResolveRel32(3, sig + 0x07) + 0x30);
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
        if (strncmp(function_name, export_name, strlen(export_name)) == 0)
        {
            WORD ordinal = *(WORD*)(module + address_of_name_ordinals + i * 2);
            DWORD function_rva = *(DWORD*)(module + address_of_functions + ordinal * 4);
            return module + function_rva;
        }
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

NTSTATUS Utils::ReadPhysical(PHYSICAL_ADDRESS address, PVOID buffer, SIZE_T size)
{
	SIZE_T bytesRead = 0;
	auto irql = __readcr8();
	__writecr8(1);
	
	auto status = MmCopyMemory(
		buffer,
		address,
		size,
		MM_COPY_MEMORY_PHYSICAL,
		&bytesRead
	);

	if (status == STATUS_INVALID_ADDRESS)
	{
		auto va = MmMapIoSpace(
			address, 
			size, 
			MmNonCached
		);
		if (va)
		{
			RtlCopyMemory(
				buffer,
				va,
				size
			);
			status = STATUS_SUCCESS;
			MmUnmapIoSpace(va, size);
		}
	}

	__writecr8(irql);
	return status;
}

template <typename type>
type Utils::ReadPhysical(PHYSICAL_ADDRESS address)
{
	type buffer = { 0 };
	Utils::ReadPhysical(address, &buffer, sizeof(type));
	return buffer;
}

PHYSICAL_ADDRESS Utils::LinearTranslatePPte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
	UINT64 idx[4]{
		8ull * rva.pml4e_index,
		8ull * rva.pdpte_index,
		8ull * rva.pde_index,
		8ull * rva.pte_index
	};

	PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
	for (int i = 0; i < 4; i++)
	{
		auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
		if (!pte.Valid || !pte.PageFrameNumber)
			return page_map + idx[i];
		switch (i)
		{
		case 1://1gb page
		{
			if (pte.LargePage)
			{
				return page_map + idx[i];
			}
		}break;
		case 2://2mb page
		{
			if (pte.LargePage)
			{
				return page_map + idx[i];
			}
		}break;
		case 3://4kb page
		{
			return page_map + idx[i];
		}break;
		}
		page_map = pte.PageFrameNumber << 12;
	}
	return { 0 };
}

PHYSICAL_ADDRESS Utils::LinearTranslatePPte(LINEAR_ADDRESS rva)
{
	return LinearTranslatePPte(__readcr3(), rva);
}

MMPTE_HARDWARE Utils::LinearTranslatePte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
	UINT64 idx[4]{
		8ull * rva.pml4e_index,
		8ull * rva.pdpte_index,
		8ull * rva.pde_index,
		8ull * rva.pte_index
	};

	PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
	for (int i = 0; i < 4; i++)
	{
		auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
		if (!pte.Valid)
			return pte;
		switch (i)
		{
		case 1://1gb page
		{
			if (pte.LargePage)
			{
				return pte;
			}
		}break;
		case 2://2mb page
		{
			if (pte.LargePage)
			{
				return pte;
			}
		}break;
		case 3://4kb page
		{
			return pte;
		}break;
		}
		page_map = pte.PageFrameNumber << 12;
	}
	return { 0 };
}

MMPTE_HARDWARE Utils::LinearTranslatePte(LINEAR_ADDRESS rva)
{
	return LinearTranslatePte(__readcr3(), rva);
}

PHYSICAL_ADDRESS Utils::LinearTranslate(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
	UINT64 idx[4]{
		8ull * rva.pml4e_index,
		8ull * rva.pdpte_index,
		8ull * rva.pde_index,
		8ull * rva.pte_index
	};

	PHYSICAL_ADDRESS page_map = dtb & ~0xFFF;
	for (int i = 0; i < 4; i++)
	{
		auto pte = ReadPhysical<MMPTE_HARDWARE>(page_map + idx[i]);
		if (!pte.Valid || !pte.PageFrameNumber)
			return 0;
		switch (i)
		{
		case 1://1gb page
		{
			if (pte.LargePage)
			{
				return (pte.PageFrameNumber << 12) + rva.huge_offset;
			}
		}break;
		case 2://2mb page
		{
			if (pte.LargePage)
			{
				return (pte.PageFrameNumber << 12) + rva.large_offset;
			}
		}break;
		case 3://4kb page
		{
			return (pte.PageFrameNumber << 12) + rva.offset;
		}break;
		}
		page_map = pte.PageFrameNumber << 12;
	}
	return 0;
}

PHYSICAL_ADDRESS Utils::LinearTranslate(LINEAR_ADDRESS rva)
{
	return LinearTranslate(__readcr3(), rva);
}

NTSTATUS Utils::ReadLinear(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)
{
	UINT64 idx[4]{
		8ull * rva.pml4e_index,
		8ull * rva.pdpte_index,
		8ull * rva.pde_index,
		8ull * rva.pte_index
	};

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
		case 1://1gb page
		{
			if (pte.LargePage)
			{
				PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.huge_offset;
				return ReadPhysical(target_address, buffer, size);
			}
		}break;
		case 2://2mb page
		{
			if (pte.LargePage)
			{
				PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.large_offset;
				return ReadPhysical(target_address, buffer, size);
			}
		}break;
		case 3://4kb page
		{
			PHYSICAL_ADDRESS target_address = (pte.PageFrameNumber << 12) + rva.offset;
			return ReadPhysical(target_address, buffer, size);
		}break;
		}
		page_map = pte.PageFrameNumber << 12;
	}
	return STATUS_UNSUCCESSFUL;
}

template <typename type>
type Utils::ReadLinear(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
	type buffer = { 0 };
	ReadLinear(dtb, rva, &buffer, sizeof(type));
	return buffer;
}

NTSTATUS Utils::ReadLinear(LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)
{
	return ReadLinear(__readcr3(), rva, buffer, size);
}

template <typename type>
type Utils::ReadLinear(LINEAR_ADDRESS rva)
{
	return ReadLinear<type>(__readcr3(), rva);
}

BOOLEAN Utils::RvaValid(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
{
	return LinearTranslate(dtb, rva) != 0;
}

BOOLEAN Utils::RvaValid(LINEAR_ADDRESS rva)
{
	return RvaValid(__readcr3(), rva);
}

NTSTATUS Utils::LocateSelf(QWORD* module_base, QWORD* module_size)
{
	if (!module_base)
		return STATUS_INVALID_PARAMETER;
	*module_base = 0;


	auto first_page = ((UINT64)memcpy) & ~0xFFFULL;
	auto last_page = Utils::SigScan_s(first_page, 0x200000, pattern("52 65 74 70 6F 6C 69 6E 65 56 31"));
	if (!last_page)
		return STATUS_UNSUCCESSFUL;

	if (last_page & 0xFFF)
		last_page = (last_page & ~0xFFFULL) + 0x1000;

	*module_base = first_page;
	if (module_size)
		*module_size = last_page - first_page;

	return STATUS_SUCCESS;
}