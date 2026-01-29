#pragma once

namespace NtImports
{
    inline PVOID(__stdcall* fn_ExAllocatePool)(
        _In_ ULONG PoolType,
        _In_ SIZE_T NumberOfBytes
        ) = nullptr;

    inline VOID(__stdcall* fn_ExFreePool)(
        _In_ PVOID P
        ) = nullptr;

    inline ULONG(__stdcall* fn_DbgPrintEx)(
        _In_ ULONG ComponentId,
        _In_ ULONG Level,
        _In_ PCSTR Format,
        _In_opt_ ...
        ) = nullptr;

    inline ULONG(__stdcall* fn_vDbgPrintEx)(
        _In_ ULONG ComponentId,
        _In_ ULONG Level,
        _In_ PCSTR Format,
        _In_ va_list ArgList
        ) = nullptr;

    inline int(__stdcall* fn_sprintf)(
        _Out_ char* Buffer,
        _In_ PCSTR Format,
        _In_opt_ ...
        ) = nullptr;

    inline int(__stdcall* fn_vsprintf)(
        _Out_ char* Buffer,
        _In_ PCSTR Format,
        _In_ va_list ArgList
        ) = nullptr;

    inline int(__stdcall* fn_swprintf)(
        _Out_ wchar_t* Buffer,
        _In_ PCWSTR Format,
        _In_opt_ ...
        ) = nullptr;

    inline int(__stdcall* fn_vswprintf)(
        _Out_ wchar_t* Buffer,
        _In_ PCWSTR Format,
        _In_ va_list ArgList
        ) = nullptr;

    inline PMDL(__stdcall* fn_IoAllocateMdl)(
        _In_opt_ PVOID VirtualAddress,
        _In_ ULONG Length,
        _In_ BOOLEAN SecondaryBuffer,
        _In_ BOOLEAN ChargeQuota,
        _Inout_opt_ PIRP Irp
        ) = nullptr;

    inline VOID(__stdcall* fn_IoFreeMdl)(
        _In_ PMDL Mdl
        ) = nullptr;

    inline ULONG(__stdcall* fn_KeQueryActiveProcessorCount)(
        ULONG GroupNumber
        ) = nullptr;

    inline PHYSICAL_ADDRESS(__stdcall* fn_MmGetPhysicalAddress)(
        _In_ PVOID BaseAddress
        ) = nullptr;

    inline PVOID(__stdcall* fn_MmGetVirtualForPhysical)(
        _In_ PHYSICAL_ADDRESS PhysicalAddress
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_MmCopyMemory)(
        _Out_ PVOID Target,
        _In_ UINT64 Source,
        _In_ SIZE_T Length,
        _In_ ULONG Flags,
        _Out_opt_ PSIZE_T NumberOfBytesCopied
        ) = nullptr;

    inline PVOID(__stdcall* fn_MmMapIoSpace)(
        _In_ PHYSICAL_ADDRESS PhysicalAddress,
        _In_ SIZE_T NumberOfBytes,
        _In_ ULONG Protect
        ) = nullptr;

    inline VOID(__stdcall* fn_MmUnmapIoSpace)(
        _In_ PVOID BaseAddress,
        _In_ SIZE_T NumberOfBytes
        ) = nullptr;

    inline VOID(__stdcall* fn_RtlCaptureContext)(
        _Inout_ PCONTEXT ContextRecord
        ) = nullptr;

    inline VOID(__stdcall* fn_KeStackAttachProcess)(
        _In_ PEPROCESS PROCESS,
        _Inout_ PKAPC_STATE ApcState
        ) = nullptr;

    inline VOID(__stdcall* fn_KeUnstackDetachProcess)(
        _In_ PKAPC_STATE ApcState
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_KeDelayExecutionThread)(
        _In_ KPROCESSOR_MODE WaitMode,
        _In_ BOOLEAN Alertable,
        _In_ PLARGE_INTEGER Interval
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_PsCreateSystemThread)(
        _Out_ PHANDLE ThreadHandle,
        _In_ ULONG DesiredAccess,
        _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
        _In_opt_ HANDLE ProcessHandle,
        _Out_opt_ PCLIENT_ID ClientId,
        _In_ PKSTART_ROUTINE StartRoutine,
        _In_opt_ PVOID StartContext
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_PsTerminateSystemThread)(
        _In_ NTSTATUS ExitStatus
        ) = nullptr;

    inline PVOID(__stdcall* fn_KeIpiGenericCall)(
        _In_ PVOID BroadcastFunction,
        _In_ PVOID Context
        ) = nullptr;

    inline PPHYSICAL_MEMORY_RANGE(__stdcall* fn_MmGetPhysicalMemoryRanges)() = nullptr;

    inline VOID(__stdcall* fn_RtlCopyMemory)(
        _Out_ VOID* Destination,
        _In_ CONST VOID* Source,
        _In_ SIZE_T Length
        ) = nullptr;

    inline VOID(__stdcall* fn_RtlFillMemory)(
        _Out_ VOID* Destination,
        _In_ SIZE_T Length,
        _In_ BYTE Fill
        ) = nullptr;

    inline PVOID(__stdcall* fn_MmAllocateContiguousMemorySpecifyCacheNode)(
        _In_ SIZE_T NumberOfBytes,
        _In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
        _In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
        _In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
        _In_ MEMORY_CACHING_TYPE CacheType,
        _In_ ULONG PreferredNode
        ) = nullptr;

    inline VOID(__stdcall* fn_MmFreeContiguousMemorySpecifyCache)(
        _In_ PVOID BaseAddress,
        _In_ SIZE_T NumberOfBytes,
        _In_ MEMORY_CACHING_TYPE CacheType
        ) = nullptr;

    inline PEPROCESS(__stdcall* fn_PsInitialSystemProcess)() = nullptr;

    inline PMMPFN(__stdcall* fn_MmPfnDatabase)() = nullptr;

	inline UINT32(__stdcall* fn_MiSystemRegionTypeDatabase)() = nullptr;

    inline UINT32(__stdcall* fn_MiGetSystemRegionType)(
        _In_ PVOID PfnEntry
		) = nullptr;

    inline VOID(__stdcall* fn_RtlInitUnicodeString)(
        _Out_ PUNICODE_STRING DestinationString,
        _In_opt_ PCWSTR SourceString
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_ZwCreateFile)(
        _Out_ PHANDLE FileHandle,
        _In_ ACCESS_MASK DesiredAccess,
        _In_ POBJECT_ATTRIBUTES ObjectAttributes,
        _Out_ PIO_STATUS_BLOCK IoStatusBlock,
        _In_opt_ PLARGE_INTEGER AllocationSize,
        _In_ ULONG FileAttributes,
        _In_ ULONG ShareAccess,
        _In_ ULONG CreateDisposition,
        _In_ ULONG CreateOptions,
        _In_opt_ PVOID EaBuffer,
        _In_ ULONG EaLength
        ) = nullptr;

    inline NTSTATUS(__stdcall* fn_ZwWriteFile)(
        _In_ HANDLE FileHandle,
        _In_opt_ HANDLE Event,
        _In_opt_ PIO_APC_ROUTINE ApcRoutine,
        _In_opt_ PVOID ApcContext,
        _Out_ PIO_STATUS_BLOCK IoStatusBlock,
        _In_ PVOID Buffer,
        _In_ ULONG Length,
        _In_opt_ PLARGE_INTEGER ByteOffset,
        _In_opt_ PULONG Key
        ) = nullptr;

	inline NTSTATUS(__stdcall* fn_ZwReadFile)(
		_In_ HANDLE FileHandle,
		_In_opt_ HANDLE Event,
		_In_opt_ PIO_APC_ROUTINE ApcRoutine,
		_In_opt_ PVOID ApcContext,
		_Out_ PIO_STATUS_BLOCK IoStatusBlock,
		_Out_ PVOID Buffer,
		_In_ ULONG Length,
		_In_opt_ PLARGE_INTEGER ByteOffset,
		_In_opt_ PULONG Key
		) = nullptr;

    inline NTSTATUS(__stdcall* fn_ZwClose)(
        _In_ HANDLE Handle
        ) = nullptr;

    inline PETHREAD(__stdcall* fn_KeGetCurrentThread)() = nullptr;

    inline UINT64(__stdcall* fn_PsGetProcessId)(
        _In_ PEPROCESS Process
		) = nullptr;

    inline BOOLEAN(__stdcall* fn_MmIsIoSpaceActive)(
        _In_ PHYSICAL_ADDRESS PhysicalAddress,
        _In_ SIZE_T NumberOfBytes
		) = nullptr;
};
