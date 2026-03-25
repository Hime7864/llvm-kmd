#pragma once

int FORCEINLINE sprintf(
    _Out_ char* Buffer,
    _In_ PCSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    auto ret = NtImports::fn_vsprintf
                   ? NtImports::fn_vsprintf(Buffer, Format, args)
                   : 0;
    va_end(args);
    return ret;
}

int FORCEINLINE swprintf(
    _Out_ wchar_t* Buffer,
    _In_ PCWSTR Format,
    _In_opt_...)
{
    va_list args;
    va_start(args, Format);
    auto ret = NtImports::fn_vswprintf
                   ? NtImports::fn_vswprintf(Buffer, Format, args)
                   : 0;
    va_end(args);
    return ret;
}
