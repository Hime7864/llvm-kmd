#include "imports.h"

LPSTR CPUID::VendorString()
{
    DWORD leaf = CPUID::_CPUID_PROCESSOR_VENDOR;
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

UINT32 CPUID::CurrentCoreNumber()
{
    DWORD leaf = CPUID::_CPUID_EXTENDED_TOPOLOGY;
    static UINT32 results = 0;
    __asm
    {
        mov eax, leaf
        CPUID
        lea r10, results
        mov[r10], edx
    }
    return results;
}


CPUID_SVM_REVISION CPUID::SvmRevision()
{
    DWORD leaf = CPUID::_CPUID_SVM_REVISION_IDENTIFICATION;
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

CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION CPUID::SvmRevisionAndFeatureIdentification()
{
    DWORD leaf = CPUID::_CPUID_SVM_REVISION_IDENTIFICATION;
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

CPUID_SVM_FEATURE_IDENTIFICATION CPUID::SvmFeatureIdentification()
{
    DWORD leaf = _CPUID_SVM_REVISION_IDENTIFICATION;
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

