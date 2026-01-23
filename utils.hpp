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
        DWORD* section_size
    );
    static MMPTE_HARDWARE GetPTE(
        QWORD virtual_address
    );
    static QWORD GetPPTE(
        QWORD virtual_address
    );
    static bool IsAddressValid(
        QWORD address
    );
    static MMPTE_HARDWARE GetPTE(
        QWORD ctx,
        QWORD virtual_address
    );
    static QWORD GetPPTE(
        QWORD ctx,
        QWORD virtual_address
    );
    static bool IsAddressValid(
        QWORD ctx,
        QWORD address
    );
    static NTSTATUS SelfModuleBase(
        QWORD* module_base,
        QWORD* module_size
    );
};