#pragma once

#include "../fwd.hpp"
#include "../enums.hpp"

namespace NtImports
{
inline PHYSICAL_ADDRESS(__stdcall* fn_MmGetPhysicalAddress)(
    _In_ PVOID BaseAddress) = nullptr;

inline PVOID(__stdcall* fn_MmGetVirtualForPhysical)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress) = nullptr;

inline NTSTATUS(__stdcall* fn_MmCopyMemory)(
    _Out_ PVOID Target,
    _In_ UINT64 Source,
    _In_ SIZE_T Length,
    _In_ ULONG Flags,
    _Out_opt_ PSIZE_T NumberOfBytesCopied) = nullptr;

inline PVOID(__stdcall* fn_MmMapIoSpace)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Protect) = nullptr;

inline VOID(__stdcall* fn_MmUnmapIoSpace)(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes) = nullptr;

inline BOOLEAN(__stdcall* fn_MmIsAddressValid)(
    _In_ PVOID VirtualAddress) = nullptr;

inline PVOID(__stdcall* fn_MmGetSystemRoutineAddress)(
    _In_ PUNICODE_STRING SystemRoutineName) = nullptr;

inline VOID(__stdcall* fn_MmProbeAndLockPages)(
    _Inout_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ LOCK_OPERATION Operation) = nullptr;

inline VOID(__stdcall* fn_MmUnlockPages)(
    _Inout_ PMDL MemoryDescriptorList) = nullptr;

inline PVOID(__stdcall* fn_MmMapLockedPagesSpecifyCache)(
    _In_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_opt_ PVOID RequestedAddress,
    _In_ ULONG BugCheckOnFailure,
    _In_ ULONG Priority) = nullptr;

inline VOID(__stdcall* fn_MmUnmapLockedPages)(
    _In_ PVOID BaseAddress,
    _In_ PMDL MemoryDescriptorList) = nullptr;

inline NTSTATUS(__stdcall* fn_MmProtectMdlSystemAddress)(
    _In_ PMDL MemoryDescriptorList,
    _In_ ULONG NewProtect) = nullptr;

inline PPHYSICAL_MEMORY_RANGE(__stdcall* fn_MmGetPhysicalMemoryRanges)() = nullptr;

inline PVOID(__stdcall* fn_MmAllocateContiguousMemorySpecifyCacheNode)(
    _In_ SIZE_T NumberOfBytes,
    _In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_ ULONG PreferredNode) = nullptr;

inline VOID(__stdcall* fn_MmFreeContiguousMemorySpecifyCache)(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ MEMORY_CACHING_TYPE CacheType) = nullptr;

inline PMMPFN(__stdcall* fn_MmPfnDatabase)() = nullptr;

inline UINT32(__stdcall* fn_MiSystemRegionTypeDatabase)() = nullptr;

inline UINT32(__stdcall* fn_MiGetSystemRegionType)(
    _In_ PVOID PfnEntry) = nullptr;

inline BOOLEAN(__stdcall* fn_MmIsIoSpaceActive)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes) = nullptr;
} // namespace NtImports
