#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline LONG(__stdcall* fn_ObReferenceObject)(
    _In_ PVOID Object) = nullptr;

inline LONG(__stdcall* fn_ObDereferenceObject)(
    _In_ PVOID Object) = nullptr;
} // namespace NtImports
