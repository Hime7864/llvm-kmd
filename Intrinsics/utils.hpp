#pragma once

#include "structures.hpp"
#include "imports.hpp"
#include "assembly.hpp"
#include "msr.hpp"
#include "crt.hpp"

class Utils
{
public:
    static QWORD ResolveRel32(
        UCHAR count,
        QWORD address);

    template <size_t N, size_t Size>
    static QWORD SigScan(
        QWORD scan_start,
        QWORD max_scan,
        Pattern<N, Size> pat);

    static QWORD SigScan(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig);

    template <size_t N, size_t Size>
    static QWORD SigScan_s(
        QWORD scan_start,
        QWORD max_scan,
        Pattern<N, Size> pat);

    static QWORD SigScan_s(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig);

    static QWORD GetKernelBase();

    static QWORD GetProcAddress(
        QWORD module,
        PCSTR export_name);

    static QWORD GetProcAddressHash(
        QWORD module,
        DWORD export_hash);

    static VOID GetProcAddressBuffer(
        QWORD module,
        FUNCTION_TABLE_ENTRY* table,
        DWORD count);

    static NTSTATUS GetSectionInfo(
        QWORD module_base,
        PCSTR section_name,
        QWORD* section_address,
        QWORD* section_size);

    // Hash-based overload (mirrors how other APIs are used with str_hash("...")).
    static NTSTATUS GetSectionInfo(
        QWORD module_base,
        DWORD section_hash,
        QWORD* section_address,
        QWORD* section_size);

    static QWORD GetImageSize(
        QWORD module_base);

    static BOOLEAN AddressInRange(
        QWORD address,
        QWORD range_base,
        QWORD range_size);

    static BOOLEAN AddressInSection(
        QWORD module_base,
        QWORD address,
        DWORD section_hash);

    static FORCEINLINE BOOLEAN IsCanonicalAddress(
        QWORD address);

    static FORCEINLINE QWORD AlignDown(
        QWORD value,
        QWORD alignment);

    static FORCEINLINE QWORD AlignUp(
        QWORD value,
        QWORD alignment);

    static FORCEINLINE BOOLEAN IsAligned(
        QWORD value,
        QWORD alignment);

    static NTSTATUS ReadPhysical(
        PHYSICAL_ADDRESS address,
        PVOID buffer,
        SIZE_T size);

    template <typename type>
    static type ReadPhysical(
        PHYSICAL_ADDRESS address)
    {
        type buffer = {0};
        Utils::ReadPhysical(address, &buffer, sizeof(type));
        return buffer;
    }

    static PHYSICAL_ADDRESS LinearTranslatePPte(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva);

    static PHYSICAL_ADDRESS LinearTranslatePPte(
        LINEAR_ADDRESS rva);

    static MMPTE_HARDWARE LinearTranslatePte(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva);

    static MMPTE_HARDWARE LinearTranslatePte(
        LINEAR_ADDRESS rva);

    static PHYSICAL_ADDRESS LinearTranslate(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva);

    static PHYSICAL_ADDRESS LinearTranslate(
        LINEAR_ADDRESS rva);

    static NTSTATUS ReadLinear(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva,
        PVOID buffer,
        SIZE_T size);

    static NTSTATUS ReadLinearSafe(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva,
        PVOID buffer,
        SIZE_T size,
        SIZE_T* bytes_read);

    template <typename type>
    FORCEINLINE static type ReadLinear(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva)
    {
        type buffer = {0};
        ReadLinear(dtb, rva, &buffer, sizeof(type));
        return buffer;
    }

    static NTSTATUS ReadLinear(
        LINEAR_ADDRESS rva,
        PVOID buffer,
        SIZE_T size);

    static NTSTATUS ReadLinearSafe(
        LINEAR_ADDRESS rva,
        PVOID buffer,
        SIZE_T size,
        SIZE_T* bytes_read);

    template <typename type>
    FORCEINLINE static type ReadLinear(
        LINEAR_ADDRESS rva)
    {
        type buffer = {0};
        ReadLinear(rva, &buffer, sizeof(type));
        return buffer;
    }

    static BOOLEAN RvaValid(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva);

    static BOOLEAN RvaValid(
        LINEAR_ADDRESS rva);

    static NTSTATUS LocateSelf(
        QWORD* module_base,
        QWORD* module_size);
};

template <size_t N, size_t Size>
__forceinline QWORD Utils::SigScan(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)
{
    UCHAR* start = (UCHAR*)scan_start;
    UCHAR* end = start + max_scan;
    if (start > end)
        return 0;

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

template <size_t N, size_t Size>
__forceinline QWORD Utils::SigScan_s(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)
{
    UCHAR* start = (UCHAR*)scan_start;
    UCHAR* end = start + max_scan;
    if (start > end)
        return 0;
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

FORCEINLINE BOOLEAN Utils::IsCanonicalAddress(QWORD address)
{
    QWORD sign_extension = address >> 48;
    return sign_extension == 0 || sign_extension == 0xFFFF;
}

FORCEINLINE QWORD Utils::AlignDown(QWORD value, QWORD alignment)
{
    if (!alignment)
        return value;
    return value & ~(alignment - 1);
}

FORCEINLINE QWORD Utils::AlignUp(QWORD value, QWORD alignment)
{
    if (!alignment)
        return value;
    return (value + alignment - 1) & ~(alignment - 1);
}

FORCEINLINE BOOLEAN Utils::IsAligned(QWORD value, QWORD alignment)
{
    if (!alignment)
        return FALSE;
    return (value & (alignment - 1)) == 0;
}
