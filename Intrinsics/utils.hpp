#pragma once

#include "structures.hpp"
#include "imports.hpp"
#include "assembly.hpp"
#include "msr.hpp"
#include "crt.hpp"

class Utils
{
public:
    static UINT64 ResolveRel32(
        UCHAR count,
        UINT64 address);

    template <size_t N, size_t Size>
    static UINT64 SigScan(
        UINT64 scan_start,
        UINT64 max_scan,
        Pattern<N, Size> pat);

    static UINT64 SigScan(
        UINT64 scan_start,
        UINT64 max_scan,
        PCSTR ida_sig);

    template <size_t N, size_t Size>
    static UINT64 SigScan_s(
        UINT64 scan_start,
        UINT64 max_scan,
        Pattern<N, Size> pat);

    static UINT64 SigScan_s(
        UINT64 scan_start,
        UINT64 max_scan,
        PCSTR ida_sig);

    static UINT64 GetKernelBase();

    static UINT64 GetProcAddress(
        UINT64 module,
        PCSTR export_name);

    static UINT64 GetProcAddressHash(
        UINT64 module,
        UINT32 export_hash);

    static VOID GetProcAddressBuffer(
        UINT64 module,
        FUNCTION_TABLE_ENTRY* table,
        UINT32 count);

    static NTSTATUS GetSectionInfo(
        UINT64 module_base,
        PCSTR section_name,
        UINT64* section_address,
        UINT64* section_size);

    // Hash-based overload (mirrors how other APIs are used with str_hash("...")).
    static NTSTATUS GetSectionInfo(
        UINT64 module_base,
        UINT32 section_hash,
        UINT64* section_address,
        UINT64* section_size);

    static UINT64 GetImageSize(
        UINT64 module_base);

    static BOOLEAN AddressInRange(
        UINT64 address,
        UINT64 range_base,
        UINT64 range_size);

    static BOOLEAN AddressInSection(
        UINT64 module_base,
        UINT64 address,
        UINT32 section_hash);

    static FORCEINLINE BOOLEAN IsCanonicalAddress(
        UINT64 address);

    static FORCEINLINE UINT64 AlignDown(
        UINT64 value,
        UINT64 alignment);

    static FORCEINLINE UINT64 AlignUp(
        UINT64 value,
        UINT64 alignment);

    static FORCEINLINE BOOLEAN IsAligned(
        UINT64 value,
        UINT64 alignment);

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
        UINT64* module_base,
        UINT64* module_size);
};

template <size_t N, size_t Size>
__forceinline UINT64 Utils::SigScan(UINT64 scan_start, UINT64 max_scan, Pattern<N, Size> pat)
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
            return (UINT64)current;
    }

    return 0;
}

template <size_t N, size_t Size>
__forceinline UINT64 Utils::SigScan_s(UINT64 scan_start, UINT64 max_scan, Pattern<N, Size> pat)
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
            auto status = MmCopyMemory(&_data, (UINT64)(current + i), sizeof(BYTE), MM_COPY_MEMORY_VIRTUAL, &bytesRead);
            if (!NT_SUCCESS(status) || _data != pat.bytes[i])
            {
                matched = false;
                break;
            }
        }
        if (matched)
            return (UINT64)current;
    }
    return 0;
}

FORCEINLINE BOOLEAN Utils::IsCanonicalAddress(UINT64 address)
{
    UINT64 sign_extension = address >> 48;
    return sign_extension == 0 || sign_extension == 0xFFFF;
}

FORCEINLINE UINT64 Utils::AlignDown(UINT64 value, UINT64 alignment)
{
    if (!alignment)
        return value;
    return value & ~(alignment - 1);
}

FORCEINLINE UINT64 Utils::AlignUp(UINT64 value, UINT64 alignment)
{
    if (!alignment)
        return value;
    return (value + alignment - 1) & ~(alignment - 1);
}

FORCEINLINE BOOLEAN Utils::IsAligned(UINT64 value, UINT64 alignment)
{
    if (!alignment)
        return FALSE;
    return (value & (alignment - 1)) == 0;
}
