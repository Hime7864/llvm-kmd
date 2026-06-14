#pragma once

NTSTATUS FORCEINLINE IoCreateDevice(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ UINT32 DeviceExtensionSize,
    _In_opt_ PUNICODE_STRING DeviceName,
    _In_ UINT32 DeviceType,
    _In_ UINT32 DeviceCharacteristics,
    _In_ BOOLEAN Exclusive,
    _Out_ PDEVICE_OBJECT* DeviceObject)
{
    return NtImports::fn_IoCreateDevice(
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
    return NtImports::fn_IoCreateDriver(
        DriverName,
        InitializationFunction);
}

PIRP FORCEINLINE IoAllocateIrp(
    _In_ CCHAR StackSize,
    _In_ BOOLEAN ChargeQuota)
{
    return NtImports::fn_IoAllocateIrp(
        StackSize,
        ChargeQuota);
}

VOID FORCEINLINE IoFreeIrp(
    _In_ PIRP Irp)
{
    return NtImports::fn_IoFreeIrp(
        Irp);
}

PIO_WORKITEM FORCEINLINE IoAllocateWorkItem(
    _In_ PDEVICE_OBJECT DeviceObject)
{
    return NtImports::fn_IoAllocateWorkItem(
        DeviceObject);
}

VOID FORCEINLINE IoQueueWorkItem(
    _In_ PIO_WORKITEM IoWorkItem,
    _In_ PIO_WORKITEM_ROUTINE WorkerRoutine,
    _In_ WORK_QUEUE_TYPE QueueType,
    _In_opt_ PVOID Context)
{
    return NtImports::fn_IoQueueWorkItem(
        IoWorkItem,
        WorkerRoutine,
        QueueType,
        Context);
}

VOID FORCEINLINE IoFreeWorkItem(
    _In_ PIO_WORKITEM IoWorkItem)
{
    return NtImports::fn_IoFreeWorkItem(
        IoWorkItem);
}

NTSTATUS FORCEINLINE IoCreateSymbolicLink(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ PUNICODE_STRING DeviceName)
{
    return NtImports::fn_IoCreateSymbolicLink(
        SymbolicLinkName,
        DeviceName);
}

NTSTATUS FORCEINLINE IoDeleteSymbolicLink(
    _In_ PUNICODE_STRING SymbolicLinkName)
{
    return NtImports::fn_IoDeleteSymbolicLink(
        SymbolicLinkName);
}

VOID FORCEINLINE IoDeleteDevice(
    _In_ PDEVICE_OBJECT DeviceObject)
{
    return NtImports::fn_IoDeleteDevice(
        DeviceObject);
}

VOID FORCEINLINE IofCompleteRequest(
    _In_ PIRP Irp,
    _In_ CHAR PriorityBoost)
{
    return NtImports::fn_IofCompleteRequest(
        Irp,
        PriorityBoost);
}

PVOID FORCEINLINE KeRegisterNmiCallback(
    _In_ PVOID Callback,
    _In_opt_ PVOID Context)
{
    return NtImports::fn_KeRegisterNmiCallback(
        Callback,
        Context);
}

NTSTATUS FORCEINLINE KeDeregisterNmiCallback(
    _In_ PVOID Callback)
{
    return NtImports::fn_KeDeregisterNmiCallback(
        Callback);
}

VOID FORCEINLINE HalSendNMI(
    _In_ _KAFFINITY_EX* Affinity)
{
    return NtImports::fn_HalSendNMI(
        Affinity);
}

VOID FORCEINLINE KeInitializeAffinityEx(
    _Out_ _KAFFINITY_EX* Affinity)
{
    return NtImports::fn_KeInitializeAffinityEx(
        Affinity);
}

VOID FORCEINLINE KeAddProcessorAffinityEx(
    _Inout_ _KAFFINITY_EX* Affinity,
    _In_ INT Number)
{
    return NtImports::fn_KeAddProcessorAffinityEx(
        Affinity,
        Number);
}

VOID FORCEINLINE ObfDereferenceObject(
    _In_ PVOID Object)
{
    return NtImports::fn_ObfDereferenceObject(
        Object);
}

ULONG FORCEINLINE KeGetCurrentProcessorNumberEx(
    _Out_ _PROCESSOR_NUMBER* GroupIndex)
{
    return NtImports::fn_KeGetCurrentProcessorNumberEx(
        GroupIndex);
}

NTSTATUS FORCEINLINE HalSetEnvironmentVariableEx(
    const wchar_t* lpName,
    const wchar_t* lpGuid,
    PVOID pBuffer,
    DWORD nSize)
{
    return NtImports::fn_HalSetEnvironmentVariableEx(
        lpName,
        lpGuid,
        pBuffer,
        nSize);
}