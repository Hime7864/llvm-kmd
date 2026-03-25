#pragma once

NTSTATUS FORCEINLINE PsCreateSystemThread(
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ HANDLE ProcessHandle,
    _Out_opt_ PCLIENT_ID ClientId,
    _In_ PKSTART_ROUTINE StartRoutine,
    _In_opt_ PVOID StartContext)
{
    return NtImports::fn_PsCreateSystemThread(
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
    return NtImports::fn_PsTerminateSystemThread(
        ExitStatus);
}

NTSTATUS FORCEINLINE PsLookupProcessByProcessId(
    _In_ HANDLE ProcessId,
    _Out_ PEPROCESS* Process)
{
    return NtImports::fn_PsLookupProcessByProcessId(
        ProcessId,
        Process);
}

PEPROCESS FORCEINLINE PsInitialSystemProcess()
{
    return *(PEPROCESS*)NtImports::fn_PsInitialSystemProcess;
}

PEPROCESS FORCEINLINE PsGetCurrentProcess()
{
    return NtImports::fn_PsGetCurrentProcess();
}

HANDLE FORCEINLINE PsGetCurrentProcessId()
{
    return NtImports::fn_PsGetCurrentProcessId();
}

UINT64 FORCEINLINE PsGetProcessId(
    _In_ PEPROCESS Process)
{
    return NtImports::fn_PsGetProcessId(
        Process);
}
