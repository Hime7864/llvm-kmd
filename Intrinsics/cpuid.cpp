#include "cpuid.hpp"
#include "assembly.hpp"

CPUID_RESULT CPUID::query(UINT32 leaf, UINT32 subleaf)
{
    UINT64 rax = 0;
    UINT64 rbx = 0;
    UINT64 rcx = 0;
    UINT64 rdx = 0;

    int cpuInfo[4] = { 0 };

    _mm_cpuidex(
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

UINT32 CPUID::current_apic_id()
{
    return query(0x00000001UL).ebx >> 24;
}

UINT32 CPUID::current_core_number()
{
    return query(0x0000000BUL, 0).edx;
}

CPUID_ADDRESS_SIZE_INFORMATION CPUID::address_size_information()
{
    return {
        .AsUINT32 = query(0x80000008UL).eax};
}

CPUID_FEATURE_EXT_ID_EBX CPUID::feature_ext_id_ebx()
{
    return {
        .AsUINT32 = query(0x80000008UL).ebx};
}

CPUID_PROCESSOR_TOPOLOGY CPUID::processor_topology()
{
    auto cpuid = query(0x8000001EUL);

    return {
        .eax = {.AsUINT32 = cpuid.eax},
        .ebx = {.AsUINT32 = cpuid.ebx}};
}

CPUID_STRUCTURED_EXTENDED_FEATURES_EBX CPUID::structured_extended_features_ebx()
{
    return {
        .AsUINT32 = query(0x00000007UL, 0).ebx};
}

CPUID_SVM_REVISION CPUID::svm_revision()
{
    return {
        .AsUINT32 = query(0x8000000AUL).eax};
}

CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION CPUID::svm_revision_and_feature_identification()
{
    return {
        .NASID = query(0x8000000AUL).ebx};
}

CPUID_SVM_FEATURE_IDENTIFICATION CPUID::svm_feature_identification()
{
    return {
        .AsUINT32 = query(0x8000000AUL).edx};
}

CPUID_APMI CPUID::apmi_features()
{
    return {
        .AsUINT32 = query(0x80000007UL).edx};
}

CPUID_EXTENDED_FEATURE_IDENTIFICATION_ECX CPUID::extended_feature_identification_ecx()
{
    return {
        .AsUINT32 = query(0x80000001UL).ecx};
}

CPUID_EXTENDED_FEATURE_IDENTIFICATION_EDX CPUID::extended_feature_identification_edx()
{
    return {
        .AsUINT32 = query(0x80000001UL).edx};
}
