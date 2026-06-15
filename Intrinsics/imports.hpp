#pragma once

#include "structures.hpp"

struct NtImports
{
    PVOID(__stdcall* fn_ExAllocatePool)(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes) = nullptr;

    PVOID(__stdcall* fn_ExAllocatePoolWithTag)(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag) = nullptr;

    PVOID(__stdcall* fn_ExAllocatePool2)(
    _In_ POOL_FLAGS Flags,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag) = nullptr;

    VOID(__stdcall* fn_ExFreePool)(
    _In_ PVOID P) = nullptr;

    VOID(__stdcall* fn_ExFreePoolWithTag)(
    _In_ PVOID P,
    _In_ ULONG Tag) = nullptr;

    LONG(__stdcall* fn_ObReferenceObject)(
    _In_ PVOID Object) = nullptr;

    LONG(__stdcall* fn_ObDereferenceObject)(
    _In_ PVOID Object) = nullptr;

    ULONG(__stdcall* fn_DbgPrintEx)(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_opt_...) = nullptr;

    ULONG(__stdcall* fn_vDbgPrintEx)(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_ va_list ArgList) = nullptr;

    int(__stdcall* fn_sprintf)(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_opt_...) = nullptr;

    int(__stdcall* fn_vsprintf)(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_ va_list ArgList) = nullptr;

    int(__stdcall* fn_swprintf)(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_opt_...) = nullptr;

    int(__stdcall* fn_vswprintf)(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_ va_list ArgList) = nullptr;

    PMDL(__stdcall* fn_IoAllocateMdl)(
    _In_opt_ PVOID VirtualAddress,
    _In_ ULONG Length,
    _In_ BOOLEAN SecondaryBuffer,
    _In_ BOOLEAN ChargeQuota,
    _Inout_opt_ PIRP Irp) = nullptr;

    VOID(__stdcall* fn_IoFreeMdl)(
    _In_ PMDL Mdl) = nullptr;

    PHYSICAL_ADDRESS(__stdcall* fn_MmGetPhysicalAddress)(
    _In_ PVOID BaseAddress) = nullptr;

    PVOID(__stdcall* fn_MmGetVirtualForPhysical)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress) = nullptr;

    NTSTATUS(__stdcall* fn_MmCopyMemory)(
    _Out_ PVOID Target,
    _In_ UINT64 Source,
    _In_ SIZE_T Length,
    _In_ ULONG Flags,
    _Out_opt_ PSIZE_T NumberOfBytesCopied) = nullptr;

    PVOID(__stdcall* fn_MmMapIoSpace)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Protect) = nullptr;

    VOID(__stdcall* fn_MmUnmapIoSpace)(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes) = nullptr;

    BOOLEAN(__stdcall* fn_MmIsAddressValid)(
    _In_ PVOID VirtualAddress) = nullptr;

    PVOID(__stdcall* fn_MmGetSystemRoutineAddress)(
    _In_ PUNICODE_STRING SystemRoutineName) = nullptr;

    VOID(__stdcall* fn_MmProbeAndLockPages)(
    _Inout_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ LOCK_OPERATION Operation) = nullptr;

    VOID(__stdcall* fn_MmUnlockPages)(
    _Inout_ PMDL MemoryDescriptorList) = nullptr;

    PVOID(__stdcall* fn_MmMapLockedPagesSpecifyCache)(
    _In_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_opt_ PVOID RequestedAddress,
    _In_ ULONG BugCheckOnFailure,
    _In_ ULONG Priority) = nullptr;

    VOID(__stdcall* fn_MmUnmapLockedPages)(
    _In_ PVOID BaseAddress,
    _In_ PMDL MemoryDescriptorList) = nullptr;

    NTSTATUS(__stdcall* fn_MmProtectMdlSystemAddress)(
    _In_ PMDL MemoryDescriptorList,
    _In_ ULONG NewProtect) = nullptr;

    PPHYSICAL_MEMORY_RANGE(__stdcall* fn_MmGetPhysicalMemoryRanges)() = nullptr;

    PVOID(__stdcall* fn_MmAllocateContiguousMemorySpecifyCacheNode)(
    _In_ SIZE_T NumberOfBytes,
    _In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_ ULONG PreferredNode) = nullptr;

    VOID(__stdcall* fn_MmFreeContiguousMemorySpecifyCache)(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes,
    _In_ MEMORY_CACHING_TYPE CacheType) = nullptr;

    PMMPFN(__stdcall* fn_MmPfnDatabase)() = nullptr;

    UINT32(__stdcall* fn_MiSystemRegionTypeDatabase)() = nullptr;

    UINT32(__stdcall* fn_MiGetSystemRegionType)(
    _In_ PVOID PfnEntry) = nullptr;

    BOOLEAN(__stdcall* fn_MmIsIoSpaceActive)(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes) = nullptr;

    ULONG(__stdcall* fn_KeQueryActiveProcessorCount)(
    ULONG GroupNumber) = nullptr;

    UINT64(__stdcall* fn_RtlCaptureContext)(
    _Inout_ PCONTEXT ContextRecord) = nullptr;

    VOID(__stdcall* fn_KeStackAttachProcess)(
    _In_ PEPROCESS PROCESS,
    _Inout_ PKAPC_STATE ApcState) = nullptr;

    VOID(__stdcall* fn_KeUnstackDetachProcess)(
    _In_ PKAPC_STATE ApcState) = nullptr;

    NTSTATUS(__stdcall* fn_KeDelayExecutionThread)(
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_ PLARGE_INTEGER Interval) = nullptr;

    VOID(__stdcall* fn_KeInitializeEvent)(
    _Out_ PKEVENT Event,
    _In_ EVENT_TYPE Type,
    _In_ BOOLEAN State) = nullptr;

    LONG(__stdcall* fn_KeSetEvent)(
    _Inout_ PKEVENT Event,
    _In_ KPRIORITY Increment,
    _In_ BOOLEAN Wait) = nullptr;

    LONG(__stdcall* fn_KeResetEvent)(
    _Inout_ PKEVENT Event) = nullptr;

    VOID(__stdcall* fn_KeClearEvent)(
    _Inout_ PKEVENT Event) = nullptr;

    NTSTATUS(__stdcall* fn_KeWaitForSingleObject)(
    _In_ PVOID Object,
    _In_ KWAIT_REASON WaitReason,
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout) = nullptr;

    KAFFINITY(__stdcall* fn_KeSetSystemAffinityThread)(
    _In_ KAFFINITY Affinity) = nullptr;

    PVOID(__stdcall* fn_KeIpiGenericCall)(
    _In_ PVOID BroadcastFunction,
    _In_ PVOID Context) = nullptr;

    VOID(__stdcall* fn_KeAcquireSpinLock)(
    _Inout_ PKSPIN_LOCK SpinLock,
    _Out_ PKIRQL OldIrql) = nullptr;

    VOID(__stdcall* fn_KeReleaseSpinLock)(
    _Inout_ PKSPIN_LOCK SpinLock,
    _In_ KIRQL NewIrql) = nullptr;

    KIRQL(__stdcall* fn_KeRaiseIrqlToDpcLevel)() = nullptr;

    VOID(__stdcall* fn_KeLowerIrql)(
    _In_ KIRQL NewIrql) = nullptr;

    PETHREAD(__stdcall* fn_KeGetCurrentThread)() = nullptr;

    NTSTATUS(__stdcall* fn_PsCreateSystemThread)(
    _Out_ PHANDLE ThreadHandle,
    _In_ ULONG DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ HANDLE ProcessHandle,
    _Out_opt_ PCLIENT_ID ClientId,
    _In_ PKSTART_ROUTINE StartRoutine,
    _In_opt_ PVOID StartContext) = nullptr;

    NTSTATUS(__stdcall* fn_PsTerminateSystemThread)(
    _In_ NTSTATUS ExitStatus) = nullptr;

    NTSTATUS(__stdcall* fn_PsLookupProcessByProcessId)(
    _In_ HANDLE ProcessId,
    _Out_ PEPROCESS* Process) = nullptr;

    PEPROCESS(__stdcall* fn_PsInitialSystemProcess)() = nullptr;

    PEPROCESS(__stdcall* fn_PsGetCurrentProcess)() = nullptr;

    HANDLE(__stdcall* fn_PsGetCurrentProcessId)() = nullptr;

    UINT64(__stdcall* fn_PsGetProcessId)(
    _In_ PEPROCESS Process) = nullptr;

    VOID(__stdcall* fn_RtlCopyMemory)(
    _Out_ VOID* Destination,
    _In_ CONST VOID* Source,
    _In_ SIZE_T Length) = nullptr;

    VOID(__stdcall* fn_RtlFillMemory)(
    _Out_ VOID* Destination,
    _In_ SIZE_T Length,
    _In_ BYTE Fill) = nullptr;

    VOID(__stdcall* fn_RtlInitUnicodeString)(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PCWSTR SourceString) = nullptr;

    LONG(__stdcall* fn_RtlCompareUnicodeString)(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive) = nullptr;

    BOOLEAN(__stdcall* fn_RtlEqualUnicodeString)(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive) = nullptr;

    VOID(__stdcall* fn_RtlCopyUnicodeString)(
    _Inout_ PUNICODE_STRING DestinationString,
    _In_opt_ CONST UNICODE_STRING* SourceString) = nullptr;

    VOID(__stdcall* fn_RtlFreeUnicodeString)(
    _Inout_ PUNICODE_STRING UnicodeString) = nullptr;

    NTSTATUS(__stdcall* fn_ZwOpenFile)(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions) = nullptr;

    NTSTATUS(__stdcall* fn_ZwCreateFile)(
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
    _In_ ULONG EaLength) = nullptr;

    NTSTATUS(__stdcall* fn_ZwWriteFile)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key) = nullptr;

    NTSTATUS(__stdcall* fn_ZwReadFile)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_ PVOID Buffer,
    _In_ ULONG Length,
    _In_opt_ PLARGE_INTEGER ByteOffset,
    _In_opt_ PULONG Key) = nullptr;

    NTSTATUS(__stdcall* fn_ZwQueryInformationFile)(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass) = nullptr;

    NTSTATUS(__stdcall* fn_ZwDeviceIoControlFile)(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG IoControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength) = nullptr;

    NTSTATUS(__stdcall* fn_ZwClose)(
    _In_ HANDLE Handle) = nullptr;

    NTSTATUS(__stdcall* fn_IoCreateDevice)(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ UINT32 DeviceExtensionSize,
    _In_opt_ PUNICODE_STRING DeviceName,
    _In_ UINT32 DeviceType,
    _In_ UINT32 DeviceCharacteristics,
    _In_ BOOLEAN Exclusive,
    _Out_ PDEVICE_OBJECT* DeviceObject) = nullptr;

    NTSTATUS(__stdcall* fn_IoCreateDriver)(
    _In_opt_ PUNICODE_STRING DriverName,
    _In_ PDRIVER_INITIALIZE InitializationFunction) = nullptr;

    PIRP(__stdcall* fn_IoAllocateIrp)(
    _In_ CCHAR StackSize,
    _In_ BOOLEAN ChargeQuota) = nullptr;

    VOID(__stdcall* fn_IoFreeIrp)(
    _In_ PIRP Irp) = nullptr;

    PIO_WORKITEM(__stdcall* fn_IoAllocateWorkItem)(
    _In_ PDEVICE_OBJECT DeviceObject) = nullptr;

    VOID(__stdcall* fn_IoQueueWorkItem)(
    _In_ PIO_WORKITEM IoWorkItem,
    _In_ PIO_WORKITEM_ROUTINE WorkerRoutine,
    _In_ WORK_QUEUE_TYPE QueueType,
    _In_opt_ PVOID Context) = nullptr;

    VOID(__stdcall* fn_IoFreeWorkItem)(
    _In_ PIO_WORKITEM IoWorkItem) = nullptr;

    NTSTATUS(__stdcall* fn_IoCreateSymbolicLink)(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ PUNICODE_STRING DeviceName) = nullptr;

    NTSTATUS(__stdcall* fn_IoDeleteSymbolicLink)(
    _In_ PUNICODE_STRING SymbolicLinkName) = nullptr;

    VOID(__stdcall* fn_IoDeleteDevice)(
    _In_ PDEVICE_OBJECT DeviceObject) = nullptr;

    VOID(__stdcall* fn_IofCompleteRequest)(
    _In_ PIRP Irp,
    _In_ CHAR PriorityBoost) = nullptr;

    PVOID(__stdcall* fn_KeRegisterNmiCallback)(
    _In_ PVOID Callback,
    _In_opt_ PVOID Context) = nullptr;

    NTSTATUS(__stdcall* fn_KeDeregisterNmiCallback)(
    _In_ PVOID Callback) = nullptr;

    VOID(__stdcall* fn_HalSendNMI)(
    _In_ _KAFFINITY_EX* Affinity) = nullptr;

    VOID(__stdcall* fn_KeAddProcessorAffinityEx)(
    _Inout_ _KAFFINITY_EX* Affinity,
    _In_ INT Number) = nullptr;

    VOID(__stdcall* fn_KeInitializeAffinityEx)(
    _Out_ _KAFFINITY_EX* Affinity) = nullptr;

    VOID(__stdcall* fn_ObfDereferenceObject)(
    _In_ PVOID Object) = nullptr;

    ULONG(__stdcall* fn_KeGetCurrentProcessorNumberEx)(
    _Out_ _PROCESSOR_NUMBER* GroupIndex) = nullptr;
};

inline NtImports nt;

PVOID FORCEINLINE ExAllocatePool(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes)
{
    return nt.fn_ExAllocatePool(
        PoolType,
        NumberOfBytes);
}

PVOID FORCEINLINE ExAllocatePoolWithTag(
    _In_ ULONG PoolType,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag)
{
    return nt.fn_ExAllocatePoolWithTag(
        PoolType,
        NumberOfBytes,
        Tag);
}

PVOID FORCEINLINE ExAllocatePool2(
    _In_ POOL_FLAGS Flags,
    _In_ SIZE_T NumberOfBytes,
    _In_ ULONG Tag)
{
    return nt.fn_ExAllocatePool2(
        Flags,
        NumberOfBytes,
        Tag);
}

VOID FORCEINLINE ExFreePool(
    _In_ PVOID P)
{
    return nt.fn_ExFreePool(P);
}

VOID FORCEINLINE ExFreePoolWithTag(
    _In_ PVOID P,
    _In_ ULONG Tag)
{
    return nt.fn_ExFreePoolWithTag(
        P,
        Tag);
}

LONG FORCEINLINE ObReferenceObject(
    _In_ PVOID Object)
{
    return nt.fn_ObReferenceObject(
        Object);
}

LONG FORCEINLINE ObDereferenceObject(
    _In_ PVOID Object)
{
    return nt.fn_ObDereferenceObject(
        Object);
}

VOID FORCEINLINE DbgPrintEx(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    nt.fn_vDbgPrintEx(ComponentId, Level, Format, args);
    va_end(args);
    return;
}

int FORCEINLINE sprintf(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    auto ret = nt.fn_vsprintf
                   ? nt.fn_vsprintf(Buffer, Format, args)
                   : 0;
    va_end(args);
    return ret;
}

int FORCEINLINE swprintf(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    auto ret = nt.fn_vswprintf
                   ? nt.fn_vswprintf(Buffer, Format, args)
                   : 0;
    va_end(args);
    return ret;
}

PMDL FORCEINLINE IoAllocateMdl(
    _In_opt_ PVOID VirtualAddress,
    _In_ ULONG Length,
    _In_ BOOLEAN SecondaryBuffer,
    _In_ BOOLEAN ChargeQuota,
    _Inout_opt_ PIRP Irp)
{
    return nt.fn_IoAllocateMdl(
        VirtualAddress,
        Length,
        SecondaryBuffer,
        ChargeQuota,
        Irp);
}

VOID FORCEINLINE IoFreeMdl(
    _In_ PMDL Mdl)
{
    return nt.fn_IoFreeMdl(Mdl);
}

PHYSICAL_ADDRESS FORCEINLINE MmGetPhysicalAddress(
    _In_ PVOID BaseAddress)
{
    return nt.fn_MmGetPhysicalAddress(
        BaseAddress);
}

PVOID FORCEINLINE MmGetVirtualForPhysical(
    _In_ PHYSICAL_ADDRESS PhysicalAddress)
{
    return nt.fn_MmGetVirtualForPhysical(
        PhysicalAddress);
}

NTSTATUS FORCEINLINE MmCopyMemory(
    _Out_ PVOID Target,
    _In_ UINT64 Source,
    _In_ SIZE_T Length,
    _In_ ULONG Flags,
    _Out_opt_ PSIZE_T NumberOfBytesCopied)
{
    return nt.fn_MmCopyMemory(
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
    return nt.fn_MmMapIoSpace(
        PhysicalAddress,
        NumberOfBytes,
        Protect);
}

VOID FORCEINLINE MmUnmapIoSpace(
    _In_ PVOID BaseAddress,
    _In_ SIZE_T NumberOfBytes)
{
    return nt.fn_MmUnmapIoSpace(
        BaseAddress,
        NumberOfBytes);
}

BOOLEAN FORCEINLINE MmIsAddressValid(
    _In_ PVOID VirtualAddress)
{
    return nt.fn_MmIsAddressValid(
        VirtualAddress);
}

PVOID FORCEINLINE MmGetSystemRoutineAddress(
    _In_ PUNICODE_STRING SystemRoutineName)
{
    return nt.fn_MmGetSystemRoutineAddress(
        SystemRoutineName);
}

VOID FORCEINLINE MmProbeAndLockPages(
    _Inout_ PMDL MemoryDescriptorList,
    _In_ KPROCESSOR_MODE AccessMode,
    _In_ LOCK_OPERATION Operation)
{
    return nt.fn_MmProbeAndLockPages(
        MemoryDescriptorList,
        AccessMode,
        Operation);
}

VOID FORCEINLINE MmUnlockPages(
    _Inout_ PMDL MemoryDescriptorList)
{
    return nt.fn_MmUnlockPages(
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
    return nt.fn_MmMapLockedPagesSpecifyCache(
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
    return nt.fn_MmUnmapLockedPages(
        BaseAddress,
        MemoryDescriptorList);
}

NTSTATUS FORCEINLINE MmProtectMdlSystemAddress(
    _In_ PMDL MemoryDescriptorList,
    _In_ ULONG NewProtect)
{
    return nt.fn_MmProtectMdlSystemAddress(
        MemoryDescriptorList,
        NewProtect);
}

PPHYSICAL_MEMORY_RANGE FORCEINLINE MmGetPhysicalMemoryRanges()
{
    return nt.fn_MmGetPhysicalMemoryRanges();
}

PVOID FORCEINLINE MmAllocateContiguousMemorySpecifyCacheNode(
    _In_ SIZE_T NumberOfBytes,
    _In_ PHYSICAL_ADDRESS LowestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS HighestAcceptableAddress,
    _In_ PHYSICAL_ADDRESS BoundaryAddressMultiple,
    _In_ MEMORY_CACHING_TYPE CacheType,
    _In_ ULONG PreferredNode)
{
    return nt.fn_MmAllocateContiguousMemorySpecifyCacheNode(
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
    return nt.fn_MmFreeContiguousMemorySpecifyCache(
        BaseAddress,
        NumberOfBytes,
        CacheType);
}

PMMPFN FORCEINLINE MmPfnDatabase()
{
    return *(PMMPFN*)nt.fn_MmPfnDatabase;
}

PBYTE FORCEINLINE MiSystemRegionTypeDatabase()
{
    return (PBYTE)nt.fn_MiSystemRegionTypeDatabase;
}

UINT32 FORCEINLINE MiGetSystemRegionType(
    _In_ PVOID PfnEntry)
{
    return nt.fn_MiGetSystemRegionType(
        PfnEntry);
}

BOOLEAN FORCEINLINE MmIsIoSpaceActive(
    _In_ PHYSICAL_ADDRESS PhysicalAddress,
    _In_ SIZE_T NumberOfBytes)
{
    return nt.fn_MmIsIoSpaceActive(
        PhysicalAddress,
        NumberOfBytes);
}

ULONG FORCEINLINE KeQueryActiveProcessorCount(
    ULONG GroupNumber)
{
    return nt.fn_KeQueryActiveProcessorCount(
        GroupNumber);
}

UINT64 FORCEINLINE RtlCaptureContext(
    _Inout_ PCONTEXT ContextRecord)
{
    return nt.fn_RtlCaptureContext(
        ContextRecord);
}

VOID FORCEINLINE KeStackAttachProcess(
    _In_ PEPROCESS PROCESS,
    _Inout_ PKAPC_STATE ApcState)
{
    return nt.fn_KeStackAttachProcess(
        PROCESS,
        ApcState);
}

VOID FORCEINLINE KeUnstackDetachProcess(
    _In_ PKAPC_STATE ApcState)
{
    return nt.fn_KeUnstackDetachProcess(
        ApcState);
}

NTSTATUS FORCEINLINE KeDelayExecutionThread(
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_ PLARGE_INTEGER Interval)
{
    return nt.fn_KeDelayExecutionThread(
        WaitMode,
        Alertable,
        Interval);
}

VOID FORCEINLINE KeInitializeEvent(
    _Out_ PKEVENT Event,
    _In_ EVENT_TYPE Type,
    _In_ BOOLEAN State)
{
    return nt.fn_KeInitializeEvent(
        Event,
        Type,
        State);
}

LONG FORCEINLINE KeSetEvent(
    _Inout_ PKEVENT Event,
    _In_ KPRIORITY Increment,
    _In_ BOOLEAN Wait)
{
    return nt.fn_KeSetEvent(
        Event,
        Increment,
        Wait);
}

LONG FORCEINLINE KeResetEvent(
    _Inout_ PKEVENT Event)
{
    return nt.fn_KeResetEvent(
        Event);
}

VOID FORCEINLINE KeClearEvent(
    _Inout_ PKEVENT Event)
{
    return nt.fn_KeClearEvent(
        Event);
}

NTSTATUS FORCEINLINE KeWaitForSingleObject(
    _In_ PVOID Object,
    _In_ KWAIT_REASON WaitReason,
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout)
{
    return nt.fn_KeWaitForSingleObject(
        Object,
        WaitReason,
        WaitMode,
        Alertable,
        Timeout);
}

KAFFINITY FORCEINLINE KeSetSystemAffinityThread(
    _In_ KAFFINITY Affinity)
{
    return nt.fn_KeSetSystemAffinityThread(
        Affinity);
}

PVOID FORCEINLINE KeIpiGenericCall(
    _In_ PVOID BroadcastFunction,
    _In_ PVOID Context)
{
    return nt.fn_KeIpiGenericCall(
        BroadcastFunction,
        Context);
}

VOID FORCEINLINE KeAcquireSpinLock(
    _Inout_ PKSPIN_LOCK SpinLock,
    _Out_ PKIRQL OldIrql)
{
    return nt.fn_KeAcquireSpinLock(
        SpinLock,
        OldIrql);
}

VOID FORCEINLINE KeReleaseSpinLock(
    _Inout_ PKSPIN_LOCK SpinLock,
    _In_ KIRQL NewIrql)
{
    return nt.fn_KeReleaseSpinLock(
        SpinLock,
        NewIrql);
}

KIRQL FORCEINLINE KeRaiseIrqlToDpcLevel()
{
    return nt.fn_KeRaiseIrqlToDpcLevel();
}

VOID FORCEINLINE KeLowerIrql(
    _In_ KIRQL NewIrql)
{
    return nt.fn_KeLowerIrql(
        NewIrql);
}

PETHREAD FORCEINLINE KeGetCurrentThread()
{
    return nt.fn_KeGetCurrentThread();
}

NTSTATUS FORCEINLINE PsCreateSystemThread(
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ HANDLE ProcessHandle,
    _Out_opt_ PCLIENT_ID ClientId,
    _In_ PKSTART_ROUTINE StartRoutine,
    _In_opt_ PVOID StartContext)
{
    return nt.fn_PsCreateSystemThread(
        ThreadHandle,
        DesiredAccess,
        ObjectAttributes,
        ProcessHandle,
        ClientId,
        StartRoutine,
        StartContext);
}

NTSTATUS FORCEINLINE PsTerminateSystemThread(
    _In_ NTSTATUS ExitStatus)
{
    return nt.fn_PsTerminateSystemThread(
        ExitStatus);
}

NTSTATUS FORCEINLINE PsLookupProcessByProcessId(
    _In_ HANDLE ProcessId,
    _Out_ PEPROCESS* Process)
{
    return nt.fn_PsLookupProcessByProcessId(
        ProcessId,
        Process);
}

PEPROCESS FORCEINLINE PsInitialSystemProcess()
{
    return *(PEPROCESS*)nt.fn_PsInitialSystemProcess;
}

PEPROCESS FORCEINLINE PsGetCurrentProcess()
{
    return nt.fn_PsGetCurrentProcess();
}

HANDLE FORCEINLINE PsGetCurrentProcessId()
{
    return nt.fn_PsGetCurrentProcessId();
}

UINT64 FORCEINLINE PsGetProcessId(
    _In_ PEPROCESS Process)
{
    return nt.fn_PsGetProcessId(
        Process);
}

VOID FORCEINLINE RtlCopyMemory(
    _Out_ VOID* Destination,
    _In_ CONST VOID* Source,
    _In_ SIZE_T Length)
{
    return nt.fn_RtlCopyMemory(
        Destination,
        Source,
        Length);
}

VOID FORCEINLINE RtlFillMemory(
    _Out_ VOID* Destination,
    _In_ SIZE_T Length,
    _In_ BYTE Fill)
{
    return nt.fn_RtlFillMemory(
        Destination,
        Length,
        Fill);
}

VOID FORCEINLINE RtlInitUnicodeString(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PCWSTR SourceString)
{
    return nt.fn_RtlInitUnicodeString(
        DestinationString,
        SourceString);
}

LONG FORCEINLINE RtlCompareUnicodeString(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive)
{
    return nt.fn_RtlCompareUnicodeString(
        String1,
        String2,
        CaseInSensitive);
}

BOOLEAN FORCEINLINE RtlEqualUnicodeString(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive)
{
    return nt.fn_RtlEqualUnicodeString(
        String1,
        String2,
        CaseInSensitive);
}

VOID FORCEINLINE RtlCopyUnicodeString(
    _Inout_ PUNICODE_STRING DestinationString,
    _In_opt_ CONST UNICODE_STRING* SourceString)
{
    return nt.fn_RtlCopyUnicodeString(
        DestinationString,
        SourceString);
}

VOID FORCEINLINE RtlFreeUnicodeString(
    _Inout_ PUNICODE_STRING UnicodeString)
{
    return nt.fn_RtlFreeUnicodeString(
        UnicodeString);
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
    _In_ ULONG EaLength)
{
    return nt.fn_ZwCreateFile(
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
        EaLength);
}

NTSTATUS FORCEINLINE ZwOpenFile(
    _Out_ PHANDLE FileHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG ShareAccess,
    _In_ ULONG OpenOptions)
{
    return nt.fn_ZwOpenFile(
        FileHandle,
        DesiredAccess,
        ObjectAttributes,
        IoStatusBlock,
        ShareAccess,
        OpenOptions);
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
    _In_opt_ PULONG Key)
{
    return nt.fn_ZwWriteFile(
        FileHandle,
        Event,
        ApcRoutine,
        ApcContext,
        IoStatusBlock,
        Buffer,
        Length,
        ByteOffset,
        Key);
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
    _In_opt_ PULONG Key)
{
    return nt.fn_ZwReadFile(
        FileHandle,
        Event,
        ApcRoutine,
        ApcContext,
        IoStatusBlock,
        Buffer,
        Length,
        ByteOffset,
        Key);
}

NTSTATUS FORCEINLINE ZwQueryInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass)
{
    return nt.fn_ZwQueryInformationFile(
        FileHandle,
        IoStatusBlock,
        FileInformation,
        Length,
        FileInformationClass);
}

NTSTATUS FORCEINLINE ZwDeviceIoControlFile(
    _In_ HANDLE FileHandle,
    _In_opt_ HANDLE Event,
    _In_opt_ PIO_APC_ROUTINE ApcRoutine,
    _In_opt_ PVOID ApcContext,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _In_ ULONG IoControlCode,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength)
{
    return nt.fn_ZwDeviceIoControlFile(
        FileHandle,
        Event,
        ApcRoutine,
        ApcContext,
        IoStatusBlock,
        IoControlCode,
        InputBuffer,
        InputBufferLength,
        OutputBuffer,
        OutputBufferLength);
}

NTSTATUS FORCEINLINE ZwClose(
    _In_ HANDLE Handle)
{
    return nt.fn_ZwClose(
        Handle);
}

NTSTATUS FORCEINLINE IoCreateDevice(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ UINT32 DeviceExtensionSize,
    _In_opt_ PUNICODE_STRING DeviceName,
    _In_ UINT32 DeviceType,
    _In_ UINT32 DeviceCharacteristics,
    _In_ BOOLEAN Exclusive,
    _Out_ PDEVICE_OBJECT* DeviceObject)
{
    return nt.fn_IoCreateDevice(
        DriverObject,
        DeviceExtensionSize,
        DeviceName,
        DeviceType,
        DeviceCharacteristics,
        Exclusive,
        DeviceObject);
}

NTSTATUS FORCEINLINE IoCreateDriver(
    _In_opt_ PUNICODE_STRING DriverName,
    _In_ PDRIVER_INITIALIZE InitializationFunction)
{
    return nt.fn_IoCreateDriver(
        DriverName,
        InitializationFunction);
}

PIRP FORCEINLINE IoAllocateIrp(
    _In_ CCHAR StackSize,
    _In_ BOOLEAN ChargeQuota)
{
    return nt.fn_IoAllocateIrp(
        StackSize,
        ChargeQuota);
}

VOID FORCEINLINE IoFreeIrp(
    _In_ PIRP Irp)
{
    return nt.fn_IoFreeIrp(
        Irp);
}

PIO_WORKITEM FORCEINLINE IoAllocateWorkItem(
    _In_ PDEVICE_OBJECT DeviceObject)
{
    return nt.fn_IoAllocateWorkItem(
        DeviceObject);
}

VOID FORCEINLINE IoQueueWorkItem(
    _In_ PIO_WORKITEM IoWorkItem,
    _In_ PIO_WORKITEM_ROUTINE WorkerRoutine,
    _In_ WORK_QUEUE_TYPE QueueType,
    _In_opt_ PVOID Context)
{
    return nt.fn_IoQueueWorkItem(
        IoWorkItem,
        WorkerRoutine,
        QueueType,
        Context);
}

VOID FORCEINLINE IoFreeWorkItem(
    _In_ PIO_WORKITEM IoWorkItem)
{
    return nt.fn_IoFreeWorkItem(
        IoWorkItem);
}

NTSTATUS FORCEINLINE IoCreateSymbolicLink(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ PUNICODE_STRING DeviceName)
{
    return nt.fn_IoCreateSymbolicLink(
        SymbolicLinkName,
        DeviceName);
}

NTSTATUS FORCEINLINE IoDeleteSymbolicLink(
    _In_ PUNICODE_STRING SymbolicLinkName)
{
    return nt.fn_IoDeleteSymbolicLink(
        SymbolicLinkName);
}

VOID FORCEINLINE IoDeleteDevice(
    _In_ PDEVICE_OBJECT DeviceObject)
{
    return nt.fn_IoDeleteDevice(
        DeviceObject);
}

VOID FORCEINLINE IofCompleteRequest(
    _In_ PIRP Irp,
    _In_ CHAR PriorityBoost)
{
    return nt.fn_IofCompleteRequest(
        Irp,
        PriorityBoost);
}

PVOID FORCEINLINE KeRegisterNmiCallback(
    _In_ PVOID Callback,
    _In_opt_ PVOID Context)
{
    return nt.fn_KeRegisterNmiCallback(
        Callback,
        Context);
}

NTSTATUS FORCEINLINE KeDeregisterNmiCallback(
    _In_ PVOID Callback)
{
    return nt.fn_KeDeregisterNmiCallback(
        Callback);
}

VOID FORCEINLINE HalSendNMI(
    _In_ _KAFFINITY_EX* Affinity)
{
    return nt.fn_HalSendNMI(
        Affinity);
}

VOID FORCEINLINE KeInitializeAffinityEx(
    _Out_ _KAFFINITY_EX* Affinity)
{
    return nt.fn_KeInitializeAffinityEx(
        Affinity);
}

VOID FORCEINLINE KeAddProcessorAffinityEx(
    _Inout_ _KAFFINITY_EX* Affinity,
    _In_ INT Number)
{
    return nt.fn_KeAddProcessorAffinityEx(
        Affinity,
        Number);
}

VOID FORCEINLINE ObfDereferenceObject(
    _In_ PVOID Object)
{
    return nt.fn_ObfDereferenceObject(
        Object);
}

ULONG FORCEINLINE KeGetCurrentProcessorNumberEx(
    _Out_ _PROCESSOR_NUMBER* GroupIndex)
{
    return nt.fn_KeGetCurrentProcessorNumberEx(
        GroupIndex);
}
