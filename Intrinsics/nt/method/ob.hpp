#pragma once

LONG FORCEINLINE ObReferenceObject(
    _In_ PVOID Object)
{
    return NtImports::fn_ObReferenceObject(
        Object);
}

LONG FORCEINLINE ObDereferenceObject(
    _In_ PVOID Object)
{
    return NtImports::fn_ObDereferenceObject(
        Object);
}
