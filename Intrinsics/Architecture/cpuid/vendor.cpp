#include "intrinsics.hpp"

CPUID_RESULT CPUID::query(UINT32 leaf, UINT32 subleaf)
{
    UINT64 rax = 0;
    UINT64 rbx = 0;
    UINT64 rcx = 0;
    UINT64 rdx = 0;

    _cpuidex(
        leaf,
        subleaf,
        &rax,
        &rbx,
        &rcx,
        &rdx);

    return {
        .eax = (UINT32)rax,
        .ebx = (UINT32)rbx,
        .ecx = (UINT32)rcx,
        .edx = (UINT32)rdx};
}

UINT32 CPUID::max_basic_leaf()
{
    return query(0x00000000UL).eax;
}

UINT32 CPUID::max_extended_leaf()
{
    return query(0x80000000UL).eax;
}

LPSTR CPUID::vendor_string()
{
    static char results[13]{0};
    auto cpuid = query(0x00000000UL);

    *(UINT32*)(results + 0) = cpuid.ebx;
    *(UINT32*)(results + 4) = cpuid.edx;
    *(UINT32*)(results + 8) = cpuid.ecx;
    results[12] = '\0';

    return results;
}

LPSTR CPUID::brand_string()
{
    static char results[49]{0};

    auto leaf2 = query(0x80000002UL);
    auto leaf3 = query(0x80000003UL);
    auto leaf4 = query(0x80000004UL);

    UINT32* dst = (UINT32*)results;
    dst[0] = leaf2.eax;
    dst[1] = leaf2.ebx;
    dst[2] = leaf2.ecx;
    dst[3] = leaf2.edx;
    dst[4] = leaf3.eax;
    dst[5] = leaf3.ebx;
    dst[6] = leaf3.ecx;
    dst[7] = leaf3.edx;
    dst[8] = leaf4.eax;
    dst[9] = leaf4.ebx;
    dst[10] = leaf4.ecx;
    dst[11] = leaf4.edx;
    results[48] = '\0';

    return results;
}
