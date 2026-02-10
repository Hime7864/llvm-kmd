#pragma once

PVOID FORCEINLINE ExAllocatePool(
	_In_ ULONG PoolType,
	_In_ SIZE_T NumberOfBytes
)
{
	return NtImports::fn_ExAllocatePool(
		PoolType, 
		NumberOfBytes
	);
}

VOID FORCEINLINE ExFreePool(
	_In_ PVOID P
)
{
	return NtImports::fn_ExFreePool(P);
}

VOID FORCEINLINE DbgPrintEx(
	_In_ ULONG ComponentId,
	_In_ ULONG Level,
	_In_ PCSTR Format,
	_In_opt_ ...
)
{
	va_list args;
	va_start(args, Format);
	NtImports::fn_vDbgPrintEx(ComponentId, Level, Format, args);
	va_end(args);
	return;
}

int FORCEINLINE sprintf(
	_Out_ char* Buffer,
	_In_ PCSTR Format,
	_In_opt_ ...
)
{
	va_list args;
	va_start(args, Format);
	auto ret = NtImports::fn_vsprintf
		? NtImports::fn_vsprintf(Buffer, Format, args)
		: 0;
	va_end(args);
	return ret;
}

int FORCEINLINE swprintf(
	_Out_ wchar_t* Buffer,
	_In_ PCWSTR Format,
	_In_opt_ ...
)
{
	va_list args;
	va_start(args, Format);
	auto ret = NtImports::fn_vswprintf
		? NtImports::fn_vswprintf(Buffer, Format, args)
		: 0;
	va_end(args);
	return ret;
}

PMDL FORCEINLINE IoAllocateMdl(
	_In_opt_ PVOID VirtualAddress,
	_In_ ULONG Length,
	_In_ BOOLEAN SecondaryBuffer,
	_In_ BOOLEAN ChargeQuota,
	_Inout_opt_ PIRP Irp
)
{
	return NtImports::fn_IoAllocateMdl(
		VirtualAddress,
		Length,
		SecondaryBuffer,
		ChargeQuota,
		Irp
	);
}


VOID FORCEINLINE IoFreeMdl(
	_In_ PMDL Mdl
)
{
	return NtImports::fn_IoFreeMdl(Mdl);
}

ULONG FORCEINLINE KeQueryActiveProcessorCount(
	ULONG GroupNumber
)
{
	return NtImports::fn_KeQueryActiveProcessorCount(
		GroupNumber
	);
}

PHYSICAL_ADDRESS FORCEINLINE MmGetPhysicalAddress(
	_In_ PVOID BaseAddress
)
{
	return NtImports::fn_MmGetPhysicalAddress(
		BaseAddress
	);
}

PVOID FORCEINLINE MmGetVirtualForPhysical(
	_In_ PHYSICAL_ADDRESS PhysicalAddress
)
{
	return NtImports::fn_MmGetVirtualForPhysical(
		PhysicalAddress
	);
}

NTSTATUS FORCEINLINE MmCopyMemory(
	_Out_ PVOID Target,
	_In_ UINT64 Source,
	_In_ SIZE_T Length,
	_In_ ULONG Flags,
	_Out_opt_ PSIZE_T NumberOfBytesCopied
)
{
	return NtImports::fn_MmCopyMemory(
		Target,
		Source,
		Length,
		Flags,
		NumberOfBytesCopied
	);
}

PVOID FORCEINLINE MmMapIoSpace(
	_In_ PHYSICAL_ADDRESS PhysicalAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ ULONG Protect
)
{
	return NtImports::fn_MmMapIoSpace(
		PhysicalAddress,
		NumberOfBytes,
		Protect
	);
}

VOID FORCEINLINE MmUnmapIoSpace(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes
)
{
	return NtImports::fn_MmUnmapIoSpace(
		BaseAddress,
		NumberOfBytes
	);
}

UINT64 FORCEINLINE RtlCaptureContext(
	_Inout_ PCONTEXT ContextRecord
)
{
	return NtImports::fn_RtlCaptureContext(
		ContextRecord
	);
}

VOID FORCEINLINE KeStackAttachProcess(
	_In_ PEPROCESS PROCESS,
	_Inout_ PKAPC_STATE ApcState
)
{
	return NtImports::fn_KeStackAttachProcess(
		PROCESS,
		ApcState
	);
}

VOID FORCEINLINE KeUnstackDetachProcess(
	_In_ PKAPC_STATE ApcState
)
{
	return NtImports::fn_KeUnstackDetachProcess(
		ApcState
	);
}

NTSTATUS FORCEINLINE KeDelayExecutionThread(
	_In_ KPROCESSOR_MODE WaitMode,
	_In_ BOOLEAN Alertable,
	_In_ PLARGE_INTEGER Interval
)
{
	return NtImports::fn_KeDelayExecutionThread(
		WaitMode,
		Alertable,
		Interval
	);
}

KAFFINITY FORCEINLINE KeSetSystemAffinityThread(
	_In_ KAFFINITY Affinity
)
{
	return NtImports::fn_KeSetSystemAffinityThread(
		Affinity
	);
}

NTSTATUS FORCEINLINE PsCreateSystemThread(
	_Out_ PHANDLE ThreadHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ HANDLE ProcessHandle,
	_Out_opt_ PCLIENT_ID ClientId,
	_In_ PKSTART_ROUTINE StartRoutine,
	_In_opt_ PVOID StartContext
)
{
	return NtImports::fn_PsCreateSystemThread(
		ThreadHandle,
		DesiredAccess,
		ObjectAttributes,
		ProcessHandle,
		ClientId,
		StartRoutine,
		StartContext
	);
}

NTSTATUS FORCEINLINE PsTerminateSystemThread(
	_In_ NTSTATUS ExitStatus
)
{
	return NtImports::fn_PsTerminateSystemThread(
		ExitStatus
	);
}

PVOID FORCEINLINE KeIpiGenericCall(
	_In_ PVOID BroadcastFunction,
	_In_ PVOID Context
)
{
	return NtImports::fn_KeIpiGenericCall(
		BroadcastFunction,
		Context
	);
}

PPHYSICAL_MEMORY_RANGE FORCEINLINE MmGetPhysicalMemoryRanges()
{
	return NtImports::fn_MmGetPhysicalMemoryRanges();
}

VOID FORCEINLINE RtlCopyMemory(
	_Out_ VOID* Destination,
	_In_ CONST VOID* Source,
	_In_ SIZE_T Length
)
{
	return NtImports::fn_RtlCopyMemory(
		Destination,
		Source,
		Length
	);
}

VOID FORCEINLINE RtlFillMemory(
	_Out_ VOID* Destination,
	_In_ SIZE_T Length,
	_In_ BYTE Fill
)
{
	return NtImports::fn_RtlFillMemory(
		Destination,
		Length,
		Fill
	);
}

PVOID FORCEINLINE MmAllocateContiguousMemorySpecifyCacheNode(
	_In_ SIZE_T NumberOfBytes,
	_In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
	_In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
	_In_ MEMORY_CACHING_TYPE CacheType,
	_In_ ULONG PreferredNode
)
{
	return NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode(
		NumberOfBytes,
		LowestAcceptableAddress,
		HighestAcceptableAddress,
		BoundaryAddressMultiple,
		CacheType,
		PreferredNode
	);
}

VOID FORCEINLINE MmFreeContiguousMemorySpecifyCache(
	_In_ PVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ MEMORY_CACHING_TYPE CacheType
)
{
	return NtImports::fn_MmFreeContiguousMemorySpecifyCache(
		BaseAddress,
		NumberOfBytes,
		CacheType
	);
}

PEPROCESS FORCEINLINE PsInitialSystemProcess()
{
	return *(PEPROCESS*)NtImports::fn_PsInitialSystemProcess;
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
	_In_ PVOID PfnEntry
)
{
	return NtImports::fn_MiGetSystemRegionType(
		PfnEntry
	);
}


VOID FORCEINLINE RtlInitUnicodeString(
	_Out_ PUNICODE_STRING DestinationString,
	_In_opt_ PCWSTR SourceString
)
{
	return NtImports::fn_RtlInitUnicodeString(
		DestinationString,
		SourceString
	);
}

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
	return NtImports::fn_ZwCreateFile(
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

NTSTATUS FORCEINLINE ZwWriteFile(
	_In_ HANDLE FileHandle,
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
	return NtImports::fn_ZwWriteFile(
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

NTSTATUS FORCEINLINE ZwReadFile(
	_In_ HANDLE FileHandle,
	_In_opt_ HANDLE Event,
	_In_opt_ PIO_APC_ROUTINE ApcRoutine,
	_In_opt_ PVOID ApcContext,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_Out_ PVOID Buffer,
	_In_ ULONG Length,
	_In_opt_ PLARGE_INTEGER ByteOffset,
	_In_opt_ PULONG Key
)
{
	return NtImports::fn_ZwReadFile(
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

NTSTATUS FORCEINLINE ZwClose(
	_In_ HANDLE Handle
)
{
	return NtImports::fn_ZwClose(
		Handle
	);
}

PETHREAD FORCEINLINE KeGetCurrentThread()
{
	return NtImports::fn_KeGetCurrentThread();
}

UINT64 FORCEINLINE PsGetProcessId(
	_In_ PEPROCESS Process
)
{
	return NtImports::fn_PsGetProcessId(
		Process
	);
}

BOOLEAN FORCEINLINE MmIsIoSpaceActive(
	_In_ PHYSICAL_ADDRESS PhysicalAddress,
	_In_ SIZE_T NumberOfBytes
)
{
	return NtImports::fn_MmIsIoSpaceActive(
		PhysicalAddress,
		NumberOfBytes
	);
}
