#pragma once

class Utils
{
public:
    static QWORD ResolveRel32(
        UCHAR count,
        QWORD address
    );

    template<size_t N, size_t Size>
    static QWORD SigScan(
        QWORD scan_start,
        QWORD max_scan,
        Pattern<N, Size> pat
    );

    static QWORD SigScan(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig
    );

    template<size_t N, size_t Size>
    static QWORD SigScan_s(
        QWORD scan_start,
        QWORD max_scan,
        Pattern<N, Size> pat
    );

    static QWORD SigScan_s(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig
    );

    static QWORD GetKernelBase();

    static QWORD GetProcAddress(
        QWORD module,
        PCSTR export_name
    );

    static VOID GetProcAddressBuffer(
        QWORD module,
        FUNCTION_TABLE_ENTRY* table,
        DWORD count
    );

    static NTSTATUS GetSectionInfo(
        QWORD module_base,
        PCSTR section_name,
        QWORD* section_address,
        QWORD* section_size
    );

    static NTSTATUS ReadPhysical(
        PHYSICAL_ADDRESS address, 
        PVOID buffer, 
        SIZE_T size
    );

	template <typename type>
    static type ReadPhysical(
        PHYSICAL_ADDRESS address
    );

    static PHYSICAL_ADDRESS LinearTranslatePPte(
        PHYSICAL_ADDRESS dtb, 
        LINEAR_ADDRESS rva
    );

    static PHYSICAL_ADDRESS LinearTranslatePPte(
        LINEAR_ADDRESS rva
    );

    static MMPTE_HARDWARE LinearTranslatePte(
        PHYSICAL_ADDRESS dtb, 
        LINEAR_ADDRESS rva
    );

    static MMPTE_HARDWARE LinearTranslatePte(
        LINEAR_ADDRESS rva
    );

    static PHYSICAL_ADDRESS LinearTranslate(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva
    );

    static PHYSICAL_ADDRESS LinearTranslate(
        LINEAR_ADDRESS rva
    );
    
    static NTSTATUS ReadLinear(
        PHYSICAL_ADDRESS dtb, 
        LINEAR_ADDRESS rva, 
        PVOID buffer, 
        SIZE_T size
    );

    template <typename type>
    FORCEINLINE static type ReadLinear(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva
    )
    {
        type buffer = { 0 };
        ReadLinear(dtb, rva, &buffer, sizeof(type));
        return buffer;
    }

    static NTSTATUS ReadLinear(
        LINEAR_ADDRESS rva, 
        PVOID buffer, 
        SIZE_T size
    );

    template <typename type>
    FORCEINLINE static type ReadLinear(
        LINEAR_ADDRESS rva
    )
    {
        type buffer = { 0 };
        ReadLinear(rva, &buffer, sizeof(type));
        return buffer;
    }

    static BOOLEAN RvaValid(
        PHYSICAL_ADDRESS dtb, 
        LINEAR_ADDRESS rva
    );

    static BOOLEAN RvaValid(
        LINEAR_ADDRESS rva
    );

    static NTSTATUS LocateSelf(
        QWORD* module_base,
        QWORD* module_size
    );
};

// ... existing code ...

// Define the template in the header so it can be instantiated by any .cpp that uses it.
template<size_t N, size_t Size>
__forceinline QWORD Utils::SigScan(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)
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