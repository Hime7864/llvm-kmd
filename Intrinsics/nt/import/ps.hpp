#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline NTSTATUS(__stdcall* fn_PsCreateSystemThread)(
    _Out_ PHANDLE ThreadHandle,
    _In_ ULONG DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ HANDLE ProcessHandle,
    _Out_opt_ PCLIENT_ID ClientId,
    _In_ PKSTART_ROUTINE StartRoutine,
    _In_opt_ PVOID StartContext) = nullptr;

inline NTSTATUS(__stdcall* fn_PsTerminateSystemThread)(
    _In_ NTSTATUS ExitStatus) = nullptr;

inline NTSTATUS(__stdcall* fn_PsLookupProcessByProcessId)(
    _In_ HANDLE ProcessId,
    _Out_ PEPROCESS* Process) = nullptr;

inline PEPROCESS(__stdcall* fn_PsInitialSystemProcess)() = nullptr;

inline PEPROCESS(__stdcall* fn_PsGetCurrentProcess)() = nullptr;

inline HANDLE(__stdcall* fn_PsGetCurrentProcessId)() = nullptr;

inline UINT64(__stdcall* fn_PsGetProcessId)(
    _In_ PEPROCESS Process) = nullptr;
} // namespace NtImports
