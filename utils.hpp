#pragma once

class Utils
{
public:
    static QWORD deref(
        UCHAR count,
        QWORD address
    );

    static QWORD sig_scan(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig
    );

    static QWORD sig_scan_safe(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig
    );

    static QWORD sig_scan_reverse(
        QWORD scan_start,
        QWORD max_scan,
        PCSTR ida_sig
    );

    static QWORD GetKernelBase();

    static QWORD GetProcAddress(
        QWORD module,
        PCSTR export_name
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
    static type ReadLinear(
        PHYSICAL_ADDRESS dtb,
        LINEAR_ADDRESS rva
    );

    static NTSTATUS ReadLinear(
        LINEAR_ADDRESS rva, 
        PVOID buffer, 
        SIZE_T size
    );

    template <typename type>
    static type ReadLinear(
        LINEAR_ADDRESS rva
    );

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