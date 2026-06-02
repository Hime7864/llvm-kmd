#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline NTSTATUS(__stdcall* fn_IoCreateDevice)(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ UINT32 DeviceExtensionSize,
    _In_opt_ PUNICODE_STRING DeviceName,
    _In_ UINT32 DeviceType,
    _In_ UINT32 DeviceCharacteristics,
    _In_ BOOLEAN Exclusive,
    _Out_ PDEVICE_OBJECT* DeviceObject) = nullptr;

inline NTSTATUS(__stdcall* fn_IoCreateDriver)(
    _In_opt_ PUNICODE_STRING DriverName,
    _In_ PDRIVER_INITIALIZE InitializationFunction) = nullptr;

inline PIRP(__stdcall* fn_IoAllocateIrp)(
    _In_ CCHAR StackSize,
    _In_ BOOLEAN ChargeQuota) = nullptr;

inline VOID(__stdcall* fn_IoFreeIrp)(
    _In_ PIRP Irp) = nullptr;

inline PIO_WORKITEM(__stdcall* fn_IoAllocateWorkItem)(
    _In_ PDEVICE_OBJECT DeviceObject) = nullptr;

inline VOID(__stdcall* fn_IoQueueWorkItem)(
    _In_ PIO_WORKITEM IoWorkItem,
    _In_ PIO_WORKITEM_ROUTINE WorkerRoutine,
    _In_ WORK_QUEUE_TYPE QueueType,
    _In_opt_ PVOID Context) = nullptr;

inline VOID(__stdcall* fn_IoFreeWorkItem)(
    _In_ PIO_WORKITEM IoWorkItem) = nullptr;

inline NTSTATUS(__stdcall* fn_IoCreateSymbolicLink)(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ PUNICODE_STRING DeviceName) = nullptr;

inline NTSTATUS(__stdcall* fn_IoDeleteSymbolicLink)(
    _In_ PUNICODE_STRING SymbolicLinkName) = nullptr;

inline VOID(__stdcall* fn_IoDeleteDevice)(
    _In_ PDEVICE_OBJECT DeviceObject) = nullptr;

inline VOID(__stdcall* fn_IofCompleteRequest)(
    _In_ PIRP Irp,
    _In_ CHAR PriorityBoost) = nullptr;

inline PVOID(__stdcall* fn_KeRegisterNmiCallback)(
    _In_ PVOID Callback,
    _In_opt_ PVOID Context) = nullptr;

inline NTSTATUS(__stdcall* fn_KeDeregisterNmiCallback)(
    _In_ PVOID Callback) = nullptr;
inline VOID(__stdcall* fn_HalSendNMI)(
    _In_ _KAFFINITY_EX* Affinity) = nullptr;

inline VOID(__stdcall* fn_KeAddProcessorAffinityEx)(
    _Inout_ _KAFFINITY_EX* Affinity,
    _In_ INT Number) = nullptr;

inline VOID(__stdcall* fn_KeInitializeAffinityEx)(
    _Out_ _KAFFINITY_EX* Affinity) = nullptr;

inline VOID(__stdcall* fn_ObfDereferenceObject)(
    _In_ PVOID Object) = nullptr;
} // namespace NtImports
