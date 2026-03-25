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
