#include "intrinsics.hpp"

NTSTATUS Utils::LocateSelf(QWORD* module_base, QWORD* module_size)
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
