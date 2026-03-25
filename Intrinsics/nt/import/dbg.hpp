#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline ULONG(__stdcall* fn_DbgPrintEx)(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_opt_...) = nullptr;

inline ULONG(__stdcall* fn_vDbgPrintEx)(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_ va_list ArgList) = nullptr;
} // namespace NtImports
