#pragma once

ULONG FORCEINLINE KeQueryActiveProcessorCount(
    ULONG GroupNumber)
{
    return NtImports::fn_KeQueryActiveProcessorCount(
        GroupNumber);
}

UINT64 FORCEINLINE RtlCaptureContext(
    _Inout_ PCONTEXT ContextRecord)
{
    return NtImports::fn_RtlCaptureContext(
        ContextRecord);
}

VOID FORCEINLINE KeStackAttachProcess(
    _In_ PEPROCESS PROCESS,
    _Inout_ PKAPC_STATE ApcState)
{
    return NtImports::fn_KeStackAttachProcess(
        PROCESS,
        ApcState);
}

VOID FORCEINLINE KeUnstackDetachProcess(
    _In_ PKAPC_STATE ApcState)
{
    return NtImports::fn_KeUnstackDetachProcess(
        ApcState);
}

NTSTATUS FORCEINLINE KeDelayExecutionThread(
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_ PLARGE_INTEGER Interval)
{
    return NtImports::fn_KeDelayExecutionThread(
        WaitMode,
        Alertable,
        Interval);
}

VOID FORCEINLINE KeInitializeEvent(
    _Out_ PKEVENT Event,
    _In_ EVENT_TYPE Type,
    _In_ BOOLEAN State)
{
    return NtImports::fn_KeInitializeEvent(
        Event,
        Type,
        State);
}

LONG FORCEINLINE KeSetEvent(
    _Inout_ PKEVENT Event,
    _In_ KPRIORITY Increment,
    _In_ BOOLEAN Wait)
{
    return NtImports::fn_KeSetEvent(
        Event,
        Increment,
        Wait);
}

LONG FORCEINLINE KeResetEvent(
    _Inout_ PKEVENT Event)
{
    return NtImports::fn_KeResetEvent(
        Event);
}

VOID FORCEINLINE KeClearEvent(
    _Inout_ PKEVENT Event)
{
    return NtImports::fn_KeClearEvent(
        Event);
}

NTSTATUS FORCEINLINE KeWaitForSingleObject(
    _In_ PVOID Object,
    _In_ KWAIT_REASON WaitReason,
    _In_ KPROCESSOR_MODE WaitMode,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout)
{
    return NtImports::fn_KeWaitForSingleObject(
        Object,
        WaitReason,
        WaitMode,
        Alertable,
        Timeout);
}

KAFFINITY FORCEINLINE KeSetSystemAffinityThread(
    _In_ KAFFINITY Affinity)
{
    return NtImports::fn_KeSetSystemAffinityThread(
        Affinity);
}

PVOID FORCEINLINE KeIpiGenericCall(
    _In_ PVOID BroadcastFunction,
    _In_ PVOID Context)
{
    return NtImports::fn_KeIpiGenericCall(
        BroadcastFunction,
        Context);
}

VOID FORCEINLINE KeAcquireSpinLock(
    _Inout_ PKSPIN_LOCK SpinLock,
    _Out_ PKIRQL OldIrql)
{
    return NtImports::fn_KeAcquireSpinLock(
        SpinLock,
        OldIrql);
}

VOID FORCEINLINE KeReleaseSpinLock(
    _Inout_ PKSPIN_LOCK SpinLock,
    _In_ KIRQL NewIrql)
{
    return NtImports::fn_KeReleaseSpinLock(
        SpinLock,
        NewIrql);
}

KIRQL FORCEINLINE KeRaiseIrqlToDpcLevel()
{
    return NtImports::fn_KeRaiseIrqlToDpcLevel();
}

VOID FORCEINLINE KeLowerIrql(
    _In_ KIRQL NewIrql)
{
    return NtImports::fn_KeLowerIrql(
        NewIrql);
}

PETHREAD FORCEINLINE KeGetCurrentThread()
{
    return NtImports::fn_KeGetCurrentThread();
}
