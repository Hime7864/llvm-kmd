#pragma once

PMDL FORCEINLINE IoAllocateMdl(
    _In_opt_ PVOID VirtualAddress,
    _In_ ULONG Length,
    _In_ BOOLEAN SecondaryBuffer,
    _In_ BOOLEAN ChargeQuota,
    _Inout_opt_ PIRP Irp)
{
    return NtImports::fn_IoAllocateMdl(
        VirtualAddress,
        Length,
        SecondaryBuffer,
        ChargeQuota,
        Irp);
}

VOID FORCEINLINE IoFreeMdl(
    _In_ PMDL Mdl)
{
    return NtImports::fn_IoFreeMdl(Mdl);
}
