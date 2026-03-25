#include "intrinsics.hpp"

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
