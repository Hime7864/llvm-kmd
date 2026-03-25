#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline int(__stdcall* fn_sprintf)(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_opt_...) = nullptr;

inline int(__stdcall* fn_vsprintf)(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_ va_list ArgList) = nullptr;

inline int(__stdcall* fn_swprintf)(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_opt_...) = nullptr;

inline int(__stdcall* fn_vswprintf)(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_ va_list ArgList) = nullptr;
} // namespace NtImports
