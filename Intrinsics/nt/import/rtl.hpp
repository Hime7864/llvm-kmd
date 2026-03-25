#pragma once

#include "../fwd.hpp"

namespace NtImports
{
inline VOID(__stdcall* fn_RtlCopyMemory)(
    _Out_ VOID* Destination,
    _In_ CONST VOID* Source,
    _In_ SIZE_T Length) = nullptr;

inline VOID(__stdcall* fn_RtlFillMemory)(
    _Out_ VOID* Destination,
    _In_ SIZE_T Length,
    _In_ BYTE Fill) = nullptr;

inline VOID(__stdcall* fn_RtlInitUnicodeString)(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PCWSTR SourceString) = nullptr;

inline LONG(__stdcall* fn_RtlCompareUnicodeString)(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive) = nullptr;

inline BOOLEAN(__stdcall* fn_RtlEqualUnicodeString)(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive) = nullptr;

inline VOID(__stdcall* fn_RtlCopyUnicodeString)(
    _Inout_ PUNICODE_STRING DestinationString,
    _In_opt_ CONST UNICODE_STRING* SourceString) = nullptr;

inline VOID(__stdcall* fn_RtlFreeUnicodeString)(
    _Inout_ PUNICODE_STRING UnicodeString) = nullptr;
} // namespace NtImports
