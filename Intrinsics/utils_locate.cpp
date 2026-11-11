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

    if (!g_self_base || !g_self_size)
    {
        *module_base = 0;
        if (module_size)
            *module_size = 0;
        return STATUS_UNSUCCESSFUL;
    }

    *module_base = g_self_base;
    if (module_size)
        *module_size = g_self_size;
    return STATUS_SUCCESS;
}
