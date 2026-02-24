#include "intrinsics.hpp"

LPSTR CPUID::vendor_string()
{
    DWORD leaf = 0x0000000UL;
    static char results[13]{ 0 };
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10 + 4 * 0], ebx
        mov[r10 + 4 * 1], edx
        mov[r10 + 4 * 2], ecx
    }
    return results;
}

UINT32 NAKED CPUID::current_apic_id()
{
    __asm
    {
        push rbx
        push rcx
        push rdx

        xor eax, eax
        mov eax, 0x1UL
        cpuid
        mov eax, ebx
		ror eax, 24

        pop rdx
        pop rcx
        pop rbx
        ret
    }
}

UINT32 NAKED CPUID::current_core_number()
{
    __asm
    {
        push rbx
        push rcx
        push rdx

        xor eax, eax
        mov eax, 0xBUL
        cpuid
        mov eax, edx

		pop rdx
		pop rcx
        pop rbx
		ret
    }
}


CPUID_SVM_REVISION CPUID::svm_revision()
{
    DWORD leaf = 0x8000000AUL;
    static UINT32 results{ 0 };
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10], eax
    }
    return { .AsUINT32 = results };
}

CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION CPUID::svm_revision_and_feature_identification()
{
    DWORD leaf = 0x8000000AUL;
    static UINT32 results{ 0 };
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10], ebx
    }
    return { .NASID = results };
}

CPUID_SVM_FEATURE_IDENTIFICATION CPUID::svm_feature_identification()
{
    DWORD leaf = 0x8000000AUL;
    static UINT32 results{ 0 };
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10], edx
    }
    return { .AsUINT32 = results };
}

CPUID_APMI CPUID::apmi_features()
{
    DWORD leaf = 0x80000007UL;
    static UINT32 results{ 0 };
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10], edx
    }
    return { .AsUINT32 = results };
}