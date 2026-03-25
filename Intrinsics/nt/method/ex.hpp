#pragma once

PVOID FORCEINLINE ExAllocatePool(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes)
{
    return NtImports::fn_ExAllocatePool(
        PoolType,
        NumberOfBytes);
}

PVOID FORCEINLINE ExAllocatePoolWithTag(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag)
{
    return NtImports::fn_ExAllocatePoolWithTag(
        PoolType,
        NumberOfBytes,
        Tag);
}

PVOID FORCEINLINE ExAllocatePool2(
    _In_ POOL_FLAGS Flags,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag)
{
    return NtImports::fn_ExAllocatePool2(
        Flags,
        NumberOfBytes,
        Tag);
}

VOID FORCEINLINE ExFreePool(
    _In_ PVOID P)
{
    return NtImports::fn_ExFreePool(P);
}

VOID FORCEINLINE ExFreePoolWithTag(
    _In_ PVOID P,
    _In_ ULONG Tag)
{
    return NtImports::fn_ExFreePoolWithTag(
        P,
        Tag);
}
