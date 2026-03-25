#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline PMDL(__stdcall* fn_IoAllocateMdl)(
    _In_opt_ PVOID VirtualAddress,
    _In_ ULONG Length,
    _In_ BOOLEAN SecondaryBuffer,
    _In_ BOOLEAN ChargeQuota,
    _Inout_opt_ PIRP Irp) = nullptr;

inline VOID(__stdcall* fn_IoFreeMdl)(
    _In_ PMDL Mdl) = nullptr;
} // namespace NtImports
