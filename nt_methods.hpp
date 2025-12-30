#pragma once

inline _ExAllocatePool fn_ExAllocatePool = nullptr;

PVOID FORCEINLINE ExAllocatePool(
	_In_ ULONG PoolType,
	_In_ SIZE_T NumberOfBytes
)
{
	return fn_ExAllocatePool(PoolType, NumberOfBytes);
}

inline _ExFreePool fn_ExFreePool = nullptr;

VOID FORCEINLINE ExFreePool(
	_In_ PVOID P
)
{
	return fn_ExFreePool(P);
}

inline _DbgPrintEx fn_DbgPrintEx = nullptr;
#define DbgPrintEx(ComponentId, Level, Format, ...) \
	fn_DbgPrintEx( \
		ComponentId, \
		Level, \
		Format, \
		__VA_ARGS__\
)

inline _IoAllocateMdl fn_IoAllocateMdl = nullptr;

PMDL FORCEINLINE IoAllocateMdl(
	_In_opt_ PVOID VirtualAddress,
	_In_ ULONG Length,
	_In_ BOOLEAN SecondaryBuffer,
	_In_ BOOLEAN ChargeQuota,
	_Inout_opt_ PIRP Irp
)
{
	return fn_IoAllocateMdl(
		VirtualAddress,
		Length,
		SecondaryBuffer,
		ChargeQuota,
		Irp
	);
}

inline _IoFreeMdl fn_IoFreeMdl = nullptr;
VOID FORCEINLINE IoFreeMdl(
	_In_ PMDL Mdl
)
{
	return fn_IoFreeMdl(Mdl);
}

inline _KeQueryActiveProcessorCount fn_KeQueryActiveProcessorCount = nullptr;

ULONG FORCEINLINE KeQueryActiveProcessorCount(
	ULONG GroupNumber
)
{
	return fn_KeQueryActiveProcessorCount(GroupNumber);
}

inline _MmGetPhysicalAddress fn_MmGetPhysicalAddress = nullptr;

PHYSICAL_ADDRESS FORCEINLINE MmGetPhysicalAddress(
	_In_ PVOID BaseAddress
)
{
	return fn_MmGetPhysicalAddress(BaseAddress);
}

inline _MmGetVirtualForPhysical fn_MmGetVirtualForPhysical = nullptr;

PVOID FORCEINLINE MmGetVirtualForPhysical(
	_In_ PHYSICAL_ADDRESS PhysicalAddress
)
{
	return fn_MmGetVirtualForPhysical(PhysicalAddress);
}

inline _MmCopyMemory fn_MmCopyMemory = nullptr;

NTSTATUS FORCEINLINE MmCopyMemory(
	_Out_ PVOID Target,
	_In_ UINT64 Source,
	_In_ SIZE_T Length,
	_In_ ULONG Flags,
	_Out_opt_ PSIZE_T NumberOfBytesCopied
)
{
	return fn_MmCopyMemory(
		Target,
		Source,
		Length,
		Flags,
		NumberOfBytesCopied
	);
}

inline _MmMapIoSpace fn_MmMapIoSpace = nullptr;

PVOID FORCEINLINE MmMapIoSpace(
	_In_ PHYSICAL_ADDRESS PhysicalAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ ULONG Protect
)
{
	return fn_MmMapIoSpace(
		PhysicalAddress,
		NumberOfBytes,
		Protect
	);
}

inline _MmUnmapIoSpace fn_MmUnmapIoSpace = nullptr;

VOID FORCEINLINE MmUnmapIoSpace(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes
)
{
	return fn_MmUnmapIoSpace(
		BaseAddress,
		NumberOfBytes
	);
}

inline _RtlCaptureContext fn_RtlCaptureContext = nullptr;

VOID FORCEINLINE RtlCaptureContext(
	_Inout_ PCONTEXT ContextRecord
)
{
	return fn_RtlCaptureContext(ContextRecord);
}

inline _KeStackAttachProcess fn_KeStackAttachProcess = nullptr;

VOID FORCEINLINE KeStackAttachProcess(
	_In_ PEPROCESS   PROCESS,
	_Inout_ PKAPC_STATE ApcState
)
{
	return fn_KeStackAttachProcess(
		PROCESS,
		ApcState
	);
}

inline _KeUnstackDetachProcess fn_KeUnstackDetachProcess = nullptr;

VOID FORCEINLINE KeUnstackDetachProcess(
	_Inout_ PKAPC_STATE ApcState
)
{
	return fn_KeUnstackDetachProcess(
		ApcState
	);
}

inline _KeDelayExecutionThread fn_KeDelayExecutionThread = nullptr;

NTSTATUS FORCEINLINE KeDelayExecutionThread(
	_In_ KPROCESSOR_MODE WaitMode,
	_In_ BOOLEAN         Alertable,
	_In_ PLARGE_INTEGER  Interval
)
{
	return fn_KeDelayExecutionThread(
		WaitMode,
		Alertable,
		Interval
	);
}

inline _PsCreateSystemThread fn_PsCreateSystemThread = nullptr;

NTSTATUS FORCEINLINE PsCreateSystemThread(
	_Out_ PHANDLE ThreadHandle,
	_In_ ULONG DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ HANDLE ProcessHandle,
	_Out_opt_ PCLIENT_ID ClientId,
	_In_ PKSTART_ROUTINE StartRoutine,
	_In_opt_ PVOID StartContext
)
{
	return fn_PsCreateSystemThread(
		ThreadHandle,
		DesiredAccess,
		ObjectAttributes,
		ProcessHandle,
		ClientId,
		StartRoutine,
		StartContext
	);
}

inline _PsTerminateSystemThread fn_PsTerminateSystemThread = nullptr;

NTSTATUS FORCEINLINE PsTerminateSystemThread(
	_In_ NTSTATUS ExitStatus
)
{
	return fn_PsTerminateSystemThread(ExitStatus);
}

inline _KeIpiGenericCall fn_KeIpiGenericCall = nullptr;

PVOID FORCEINLINE KeIpiGenericCall(
	_In_ PVOID BroadcastFunction,
	_In_ PVOID Context
)
{
	return fn_KeIpiGenericCall(
		BroadcastFunction,
		Context
	);
}

inline _MmGetPhysicalMemoryRanges fn_MmGetPhysicalMemoryRanges = nullptr;
PPHYSICAL_MEMORY_RANGE FORCEINLINE MmGetPhysicalMemoryRanges()
{
	return fn_MmGetPhysicalMemoryRanges();
}

inline _RtlCopyMemory fn_RtlCopyMemory = nullptr;
PVOID FORCEINLINE RtlCopyMemory(
	_In_ VOID* Destination,
	_In_ CONST VOID* Source,
	_In_ SIZE_T Length
)
{
	return fn_RtlCopyMemory(
		Destination,
		Source,
		Length
	);
}

inline _RtlFillMemory fn_RtlFillMemory = nullptr;
VOID FORCEINLINE RtlFillMemory(
	_Out_ VOID* Destination,
	_In_ SIZE_T Length,
	_In_ BYTE Fill
)
{
	return fn_RtlFillMemory(
		Destination,
		Length,
		Fill
	);
}

inline _MmAllocateContiguousMemorySpecifyCacheNode fn_MmAllocateContiguousMemorySpecifyCacheNode = nullptr;
PVOID FORCEINLINE MmAllocateContiguousMemorySpecifyCacheNode(
	_In_ SIZE_T NumberOfBytes,
	_In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
	_In_ MEMORY_CACHING_TYPE CacheType,
	_In_ ULONG PreferredNode
)
{
	return fn_MmAllocateContiguousMemorySpecifyCacheNode(
		NumberOfBytes,
		LowestAcceptableAddress,
		HighestAcceptableAddress,
		BoundaryAddressMultiple,
		CacheType,
		PreferredNode
	);
}

inline _MmFreeContiguousMemorySpecifyCache fn_MmFreeContiguousMemorySpecifyCache = nullptr;

VOID FORCEINLINE MmFreeContiguousMemorySpecifyCache(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ MEMORY_CACHING_TYPE CacheType
)
{
	return fn_MmFreeContiguousMemorySpecifyCache(
		BaseAddress,
		NumberOfBytes,
		CacheType
	);
}

inline _PsInitialSystemProcess fn_PsInitialSystemProcess = nullptr;
PEPROCESS FORCEINLINE PsInitialSystemProcess()
{
	return *(PEPROCESS*)fn_PsInitialSystemProcess;
}

inline _MmPfnDatabase fn_MmPfnDatabase = nullptr;
PMMPFN FORCEINLINE MmPfnDatabase()
{
	return *(PMMPFN*)fn_MmPfnDatabase;
}

inline _ZwCreateFile fn_ZwCreateFile = nullptr;
NTSTATUS FORCEINLINE ZwCreateFile(
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
)
{
	return fn_ZwCreateFile(
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		AllocationSize,
		FileAttributes,
		ShareAccess,
		CreateDisposition,
		CreateOptions,
		EaBuffer,
		EaLength
	);
}

inline _ZwWriteFile fn_ZwWriteFile = nullptr;
NTSTATUS FORCEINLINE ZwWriteFile(
	_Out_ HANDLE FileHandle,
	_In_opt_ HANDLE Event,
	_In_opt_ PIO_APC_ROUTINE ApcRoutine,
	_In_opt_ PVOID ApcContext,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_In_ PVOID Buffer,
	_In_ ULONG Length,
	_In_opt_ PLARGE_INTEGER ByteOffset,
	_In_opt_ PULONG Key
)
{
	return fn_ZwWriteFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		Buffer,
		Length,
		ByteOffset,
		Key
	);
}

inline _ZwClose fn_ZwClose = nullptr;
NTSTATUS FORCEINLINE ZwClose(
	_In_ HANDLE Handle
)
{
	return fn_ZwClose(
		Handle
	);
}

inline _RtlInitUnicodeString fn_RtlInitUnicodeString = nullptr;
VOID FORCEINLINE RtlInitUnicodeString(
	_Out_ PUNICODE_STRING DestinationString,
	_In_opt_ PCWSTR SourceString
)
{
	return fn_RtlInitUnicodeString(
		DestinationString,
		SourceString
	);
}
