#pragma once

typedef PVOID(__stdcall* _ExAllocatePool)(
	_In_ ULONG PoolType,
	_In_ SIZE_T NumberOfBytes
	);

typedef VOID(__stdcall* _ExFreePool)(
	_In_ PVOID P
	);

typedef ULONG(__stdcall* _DbgPrintEx)(
	_In_ ULONG ComponentId,
	_In_ ULONG Level,
	_In_ PCSTR Format,
	_In_opt_...
	);

typedef int(__stdcall* _sprintf)(
	_Out_ char* Buffer,
	_In_ PCSTR Format,
	_In_opt_...
	);

typedef int(__stdcall* _swprintf)(
	_Out_ wchar_t* Buffer,
	_In_ PCWSTR Format,
	_In_opt_...
	);

typedef PMDL(__stdcall* _IoAllocateMdl)(
	_In_opt_ PVOID VirtualAddress,
	_In_ ULONG Length,
	_In_ BOOLEAN SecondaryBuffer,
	_In_ BOOLEAN ChargeQuota,
	_Inout_opt_ PIRP Irp
	);

typedef VOID(__stdcall* _IoFreeMdl)(
	_In_ PMDL Mdl
	);

typedef ULONG(__stdcall* _KeQueryActiveProcessorCount)(
	ULONG GroupNumber
	);

typedef PHYSICAL_ADDRESS(__stdcall* _MmGetPhysicalAddress)(
	_In_ PVOID BaseAddress
	);

typedef PVOID(__stdcall* _MmGetVirtualForPhysical)(
	_In_ PHYSICAL_ADDRESS PhysicalAddress
	);

typedef NTSTATUS(__stdcall* _MmCopyMemory)(
	_Out_ PVOID Target,
	_In_ UINT64 Source,
	_In_ SIZE_T Length,
	_In_ ULONG Flags,
	_Out_opt_ PSIZE_T NumberOfBytesCopied
	);

typedef PVOID(__stdcall* _MmMapIoSpace)(
	_In_ PHYSICAL_ADDRESS PhysicalAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ ULONG Protect
	);

typedef VOID(__stdcall* _MmUnmapIoSpace)(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes
	);

typedef VOID(__stdcall* _RtlCaptureContext)(
	_Inout_ PCONTEXT ContextRecord
	);

typedef VOID(__stdcall* _KeStackAttachProcess)(
	_In_ PEPROCESS   PROCESS,
	_Inout_ PKAPC_STATE ApcState
	);

typedef VOID(__stdcall* _KeUnstackDetachProcess)(
	_In_ PKAPC_STATE ApcState
	);

typedef NTSTATUS(__stdcall* _KeDelayExecutionThread)(
	_In_ KPROCESSOR_MODE WaitMode,
	_In_ BOOLEAN         Alertable,
	_In_ PLARGE_INTEGER  Interval
	);

typedef NTSTATUS(__stdcall* _PsCreateSystemThread)(
	_Out_ PHANDLE ThreadHandle,
	_In_ ULONG DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ HANDLE ProcessHandle,
	_Out_opt_ PCLIENT_ID ClientId,
	_In_ PKSTART_ROUTINE StartRoutine,
	_In_opt_ PVOID StartContext
	);

typedef NTSTATUS(__stdcall* _PsTerminateSystemThread)(
	_In_ NTSTATUS ExitStatus
	);

typedef PVOID(__stdcall* _KeIpiGenericCall)(
	_In_ PVOID BroadcastFunction,
	_In_ PVOID Context
	);

typedef PPHYSICAL_MEMORY_RANGE(__stdcall* _MmGetPhysicalMemoryRanges)();

typedef PVOID(__stdcall* _RtlCopyMemory)(
	_Out_ VOID* Destination,
	_In_ CONST VOID* Source,
	_In_ SIZE_T Length
	);

typedef VOID(__stdcall* _RtlFillMemory)(
	_Out_ VOID* Destination,
	_In_ SIZE_T Length,
	_In_ BYTE Fill
	);

typedef PVOID(__stdcall* _MmAllocateContiguousMemorySpecifyCacheNode)(
	_In_ SIZE_T NumberOfBytes,
	_In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
	_In_ MEMORY_CACHING_TYPE CacheType,
	_In_ ULONG PreferredNode
	);

typedef VOID(__stdcall* _MmFreeContiguousMemorySpecifyCache)(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ MEMORY_CACHING_TYPE CacheType
	);

typedef PEPROCESS(__stdcall* _PsInitialSystemProcess)();

typedef PMMPFN(__stdcall* _MmPfnDatabase)();

typedef VOID(__stdcall* _RtlInitUnicodeString)(
	_Out_ PUNICODE_STRING DestinationString,
	_In_opt_ PCWSTR SourceString
	);

typedef NTSTATUS(__stdcall* _ZwCreateFile)(
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
	);

typedef NTSTATUS(__stdcall* _ZwWriteFile)(
	_In_ HANDLE FileHandle,
	_In_opt_ HANDLE Event,
	_In_opt_ PIO_APC_ROUTINE ApcRoutine,
	_In_opt_ PVOID ApcContext,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_In_ PVOID Buffer,
	_In_ ULONG Length,
	_In_opt_ PLARGE_INTEGER ByteOffset,
	_In_opt_ PULONG Key
	);

typedef NTSTATUS(__stdcall* _ZwClose)(
	_In_ HANDLE Handle
	);

typedef PETHREAD(__stdcall* _KeGetCurrentThread)();