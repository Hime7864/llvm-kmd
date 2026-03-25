#pragma once

#include "../fwd.hpp"
#include "../enums.hpp"

namespace NtImports
{
inline ULONG(__stdcall* fn_KeQueryActiveProcessorCount)(
    ULONG GroupNumber) = nullptr;

inline UINT64(__stdcall* fn_RtlCaptureContext)(
    _Inout_ PCONTEXT ContextRecord) = nullptr;

inline VOID(__stdcall* fn_KeStackAttachProcess)(
    _In_ PEPROCESS PROCESS,
    _Inout_ PKAPC_STATE ApcState) = nullptr;

inline VOID(__stdcall* fn_KeUnstackDetachProcess)(
    _In_ PKAPC_STATE ApcState) = nullptr;

inline NTSTATUS(__stdcall* fn_KeDelayExecutionThread)(
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_ PLARGE_INTEGER Interval) = nullptr;

inline VOID(__stdcall* fn_KeInitializeEvent)(
    _Out_ PKEVENT Event,
    _In_ EVENT_TYPE Type,
    _In_ BOOLEAN State) = nullptr;

inline LONG(__stdcall* fn_KeSetEvent)(
    _Inout_ PKEVENT Event,
    _In_ KPRIORITY Increment,
    _In_ BOOLEAN Wait) = nullptr;

inline LONG(__stdcall* fn_KeResetEvent)(
    _Inout_ PKEVENT Event) = nullptr;

inline VOID(__stdcall* fn_KeClearEvent)(
    _Inout_ PKEVENT Event) = nullptr;

inline NTSTATUS(__stdcall* fn_KeWaitForSingleObject)(
    _In_ PVOID Object,
    _In_ KWAIT_REASON WaitReason,
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout) = nullptr;

inline KAFFINITY(__stdcall* fn_KeSetSystemAffinityThread)(
    _In_ KAFFINITY Affinity) = nullptr;

inline PVOID(__stdcall* fn_KeIpiGenericCall)(
    _In_ PVOID BroadcastFunction,
    _In_ PVOID Context) = nullptr;

inline VOID(__stdcall* fn_KeAcquireSpinLock)(
    _Inout_ PKSPIN_LOCK SpinLock,
    _Out_ PKIRQL OldIrql) = nullptr;

inline VOID(__stdcall* fn_KeReleaseSpinLock)(
    _Inout_ PKSPIN_LOCK SpinLock,
    _In_ KIRQL NewIrql) = nullptr;

inline KIRQL(__stdcall* fn_KeRaiseIrqlToDpcLevel)() = nullptr;

inline VOID(__stdcall* fn_KeLowerIrql)(
    _In_ KIRQL NewIrql) = nullptr;

inline PETHREAD(__stdcall* fn_KeGetCurrentThread)() = nullptr;
} // namespace NtImports
