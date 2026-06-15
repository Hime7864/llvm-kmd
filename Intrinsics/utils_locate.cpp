#include "utils.hpp"

UINT64 Utils::GetKernelBase()
{
    auto sig = Utils::SigScan(MSR::LSTAR() & ~0xFFFFFULL, 0xA00000ULL, pattern("66 89 05 ? ? ? ? 48 8D 05 ? ? ? ? 48 89"));
    if (!sig)
        return 0;
    return *(UINT64*)(*(UINT64*)ResolveRel32(3, sig + 0x07) + 0x30);
}

NTSTATUS Utils::LocateSelf(UINT64* module_base, UINT64* module_size)
{
    if (!module_base)
        return STATUS_INVALID_PARAMETER;
    *module_base = 0;

    auto first_page = ((UINT64)memcpy) & ~0xFFFULL;
    auto last_page = Utils::SigScan_s(first_page, 0x200000, pattern("52 65 74 70 6F 6C 69 6E 65 56 31"));
    if (!last_page)
        return STATUS_UNSUCCESSFUL;

    if (last_page & 0xFFF)
        last_page = (last_page & ~0xFFFULL) + 0x1000;

    *module_base = first_page;
    if (module_size)
        *module_size = last_page - first_page;

    return STATUS_SUCCESS;
}