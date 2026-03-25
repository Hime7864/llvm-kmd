#pragma once

VOID FORCEINLINE RtlCopyMemory(
    _Out_ VOID* Destination,
    _In_ CONST VOID* Source,
    _In_ SIZE_T Length)
{
    return NtImports::fn_RtlCopyMemory(
        Destination,
        Source,
        Length);
}

VOID FORCEINLINE RtlFillMemory(
    _Out_ VOID* Destination,
    _In_ SIZE_T Length,
    _In_ BYTE Fill)
{
    return NtImports::fn_RtlFillMemory(
        Destination,
        Length,
        Fill);
}

VOID FORCEINLINE RtlInitUnicodeString(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PCWSTR SourceString)
{
    return NtImports::fn_RtlInitUnicodeString(
        DestinationString,
        SourceString);
}

LONG FORCEINLINE RtlCompareUnicodeString(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive)
{
    return NtImports::fn_RtlCompareUnicodeString(
        String1,
        String2,
        CaseInSensitive);
}

BOOLEAN FORCEINLINE RtlEqualUnicodeString(
    _In_ PUNICODE_STRING String1,
    _In_ PUNICODE_STRING String2,
    _In_ BOOLEAN CaseInSensitive)
{
    return NtImports::fn_RtlEqualUnicodeString(
        String1,
        String2,
        CaseInSensitive);
}

VOID FORCEINLINE RtlCopyUnicodeString(
    _Inout_ PUNICODE_STRING DestinationString,
    _In_opt_ CONST UNICODE_STRING* SourceString)
{
    return NtImports::fn_RtlCopyUnicodeString(
        DestinationString,
        SourceString);
}

VOID FORCEINLINE RtlFreeUnicodeString(
    _Inout_ PUNICODE_STRING UnicodeString)
{
    return NtImports::fn_RtlFreeUnicodeString(
        UnicodeString);
}
