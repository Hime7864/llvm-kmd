#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline PVOID(__stdcall* fn_ExAllocatePool)(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes) = nullptr;

inline PVOID(__stdcall* fn_ExAllocatePoolWithTag)(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag) = nullptr;

inline PVOID(__stdcall* fn_ExAllocatePool2)(
    _In_ POOL_FLAGS Flags,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag) = nullptr;

inline VOID(__stdcall* fn_ExFreePool)(
    _In_ PVOID P) = nullptr;

inline VOID(__stdcall* fn_ExFreePoolWithTag)(
    _In_ PVOID P,
    _In_ ULONG Tag) = nullptr;
} // namespace NtImports
