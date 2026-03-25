#pragma once

VOID FORCEINLINE DbgPrintEx(
    _In_ ULONG ComponentId,
    _In_ ULONG Level,
    _In_ PCSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    NtImports::fn_vDbgPrintEx(ComponentId, Level, Format, args);
    va_end(args);
    return;
}
