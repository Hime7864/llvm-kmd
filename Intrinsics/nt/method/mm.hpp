#pragma once

PHYSICAL_ADDRESS FORCEINLINE MmGetPhysicalAddress(
    _In_ PVOID BaseAddress)
{
    return NtImports::fn_MmGetPhysicalAddress(
        BaseAddress);
}

PVOID FORCEINLINE MmGetVirtualForPhysical(
    _In_ PHYSICAL_ADDRESS PhysicalAddress)
{
    return NtImports::fn_MmGetVirtualForPhysical(
        PhysicalAddress);
}

NTSTATUS FORCEINLINE MmCopyMemory(
    _Out_ PVOID Target,
    _In_ UINT64 Source,
    _In_ SIZE_T Length,
    _In_ ULONG Flags,
    _Out_opt_ PSIZE_T NumberOfBytesCopied)
{
    return NtImports::fn_MmCopyMemory(
        Target,
        Source,
        Length,
        Flags,
        NumberOfBytesCopied);
}

PVOID FORCEINLINE MmMapIoSpace(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Protect)
{
    return NtImports::fn_MmMapIoSpace(
        PhysicalAddress,
        NumberOfBytes,
        Protect);
}

VOID FORCEINLINE MmUnmapIoSpace(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes)
{
    return NtImports::fn_MmUnmapIoSpace(
        BaseAddress,
        NumberOfBytes);
}

BOOLEAN FORCEINLINE MmIsAddressValid(
    _In_ PVOID VirtualAddress)
{
    return NtImports::fn_MmIsAddressValid(
        VirtualAddress);
}

PVOID FORCEINLINE MmGetSystemRoutineAddress(
    _In_ PUNICODE_STRING SystemRoutineName)
{
    return NtImports::fn_MmGetSystemRoutineAddress(
        SystemRoutineName);
}

VOID FORCEINLINE MmProbeAndLockPages(
    _Inout_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ LOCK_OPERATION Operation)
{
    return NtImports::fn_MmProbeAndLockPages(
        MemoryDescriptorList,
        AccessMode,
        Operation);
}

VOID FORCEINLINE MmUnlockPages(
    _Inout_ PMDL MemoryDescriptorList)
{
    return NtImports::fn_MmUnlockPages(
        MemoryDescriptorList);
}

PVOID FORCEINLINE MmMapLockedPagesSpecifyCache(
    _In_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_opt_ PVOID RequestedAddress,
    _In_ ULONG BugCheckOnFailure,
    _In_ ULONG Priority)
{
    return NtImports::fn_MmMapLockedPagesSpecifyCache(
        MemoryDescriptorList,
        AccessMode,
        CacheType,
        RequestedAddress,
        BugCheckOnFailure,
        Priority);
}

VOID FORCEINLINE MmUnmapLockedPages(
    _In_ PVOID BaseAddress,
    _In_ PMDL MemoryDescriptorList)
{
    return NtImports::fn_MmUnmapLockedPages(
        BaseAddress,
        MemoryDescriptorList);
}

NTSTATUS FORCEINLINE MmProtectMdlSystemAddress(
    _In_ PMDL MemoryDescriptorList,
    _In_ ULONG NewProtect)
{
    return NtImports::fn_MmProtectMdlSystemAddress(
        MemoryDescriptorList,
        NewProtect);
}

PPHYSICAL_MEMORY_RANGE FORCEINLINE MmGetPhysicalMemoryRanges()
{
    return NtImports::fn_MmGetPhysicalMemoryRanges();
}

PVOID FORCEINLINE MmAllocateContiguousMemorySpecifyCacheNode(
    _In_ SIZE_T NumberOfBytes,
    _In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_ ULONG PreferredNode)
{
    return NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode(
        NumberOfBytes,
        LowestAcceptableAddress,
        HighestAcceptableAddress,
        BoundaryAddressMultiple,
        CacheType,
        PreferredNode);
}

VOID FORCEINLINE MmFreeContiguousMemorySpecifyCache(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ MEMORY_CACHING_TYPE CacheType)
{
    return NtImports::fn_MmFreeContiguousMemorySpecifyCache(
        BaseAddress,
        NumberOfBytes,
        CacheType);
}

PMMPFN FORCEINLINE MmPfnDatabase()
{
    return *(PMMPFN*)NtImports::fn_MmPfnDatabase;
}

PBYTE FORCEINLINE MiSystemRegionTypeDatabase()
{
    return (PBYTE)NtImports::fn_MiSystemRegionTypeDatabase;
}

UINT32 FORCEINLINE MiGetSystemRegionType(
    _In_ PVOID PfnEntry)
{
    return NtImports::fn_MiGetSystemRegionType(
        PfnEntry);
}

BOOLEAN FORCEINLINE MmIsIoSpaceActive(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes)
{
    return NtImports::fn_MmIsIoSpaceActive(
        PhysicalAddress,
        NumberOfBytes);
}
