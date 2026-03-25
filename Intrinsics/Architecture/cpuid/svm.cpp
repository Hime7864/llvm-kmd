#include "intrinsics.hpp"

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
