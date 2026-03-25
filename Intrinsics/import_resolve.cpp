#include "intrinsics.hpp"

FUNCTION_TABLE_ENTRY function_table[]{
    {str_hash("ExAllocatePool"), &NtImports::fn_ExAllocatePool},
    {str_hash("ExAllocatePoolWithTag"), &NtImports::fn_ExAllocatePoolWithTag},
    {str_hash("ExAllocatePool2"), &NtImports::fn_ExAllocatePool2},
    {str_hash("ExFreePool"), &NtImports::fn_ExFreePool},
    {str_hash("ExFreePoolWithTag"), &NtImports::fn_ExFreePoolWithTag},
    {str_hash("ObReferenceObject"), &NtImports::fn_ObReferenceObject},
    {str_hash("ObDereferenceObject"), &NtImports::fn_ObDereferenceObject},
    {str_hash("DbgPrintEx"), &NtImports::fn_DbgPrintEx},
    {str_hash("vDbgPrintEx"), &NtImports::fn_vDbgPrintEx},
    {str_hash("sprintf"), &NtImports::fn_sprintf},
    {str_hash("vsprintf"), &NtImports::fn_vsprintf},
    {str_hash("swprintf"), &NtImports::fn_swprintf},
    {str_hash("vswprintf"), &NtImports::fn_vswprintf},
    {str_hash("IoAllocateIrp"), &NtImports::fn_IoAllocateIrp},
    {str_hash("IoFreeIrp"), &NtImports::fn_IoFreeIrp},
    {str_hash("IoAllocateWorkItem"), &NtImports::fn_IoAllocateWorkItem},
    {str_hash("IoQueueWorkItem"), &NtImports::fn_IoQueueWorkItem},
    {str_hash("IoFreeWorkItem"), &NtImports::fn_IoFreeWorkItem},
    {str_hash("IoAllocateMdl"), &NtImports::fn_IoAllocateMdl},
    {str_hash("IoFreeMdl"), &NtImports::fn_IoFreeMdl},
    {str_hash("KeQueryActiveProcessorCount"), &NtImports::fn_KeQueryActiveProcessorCount},
    {str_hash("KeInitializeEvent"), &NtImports::fn_KeInitializeEvent},
    {str_hash("KeSetEvent"), &NtImports::fn_KeSetEvent},
    {str_hash("KeResetEvent"), &NtImports::fn_KeResetEvent},
    {str_hash("KeClearEvent"), &NtImports::fn_KeClearEvent},
    {str_hash("KeWaitForSingleObject"), &NtImports::fn_KeWaitForSingleObject},
    {str_hash("MmGetPhysicalAddress"), &NtImports::fn_MmGetPhysicalAddress},
    {str_hash("MmGetVirtualForPhysical"), &NtImports::fn_MmGetVirtualForPhysical},
    {str_hash("MmCopyMemory"), &NtImports::fn_MmCopyMemory},
    {str_hash("MmMapIoSpace"), &NtImports::fn_MmMapIoSpace},
    {str_hash("MmUnmapIoSpace"), &NtImports::fn_MmUnmapIoSpace},
    {str_hash("MmIsAddressValid"), &NtImports::fn_MmIsAddressValid},
    {str_hash("MmGetSystemRoutineAddress"), &NtImports::fn_MmGetSystemRoutineAddress},
    {str_hash("MmProbeAndLockPages"), &NtImports::fn_MmProbeAndLockPages},
    {str_hash("MmUnlockPages"), &NtImports::fn_MmUnlockPages},
    {str_hash("MmMapLockedPagesSpecifyCache"), &NtImports::fn_MmMapLockedPagesSpecifyCache},
    {str_hash("MmUnmapLockedPages"), &NtImports::fn_MmUnmapLockedPages},
    {str_hash("MmProtectMdlSystemAddress"), &NtImports::fn_MmProtectMdlSystemAddress},
    {str_hash("RtlCaptureContext"), &NtImports::fn_RtlCaptureContext},
    {str_hash("KeStackAttachProcess"), &NtImports::fn_KeStackAttachProcess},
    {str_hash("KeUnstackDetachProcess"), &NtImports::fn_KeUnstackDetachProcess},
    {str_hash("KeDelayExecutionThread"), &NtImports::fn_KeDelayExecutionThread},
    {str_hash("KeSetSystemAffinityThread"), &NtImports::fn_KeSetSystemAffinityThread},
    {str_hash("KeAcquireSpinLock"), &NtImports::fn_KeAcquireSpinLock},
    {str_hash("KeReleaseSpinLock"), &NtImports::fn_KeReleaseSpinLock},
    {str_hash("KeRaiseIrqlToDpcLevel"), &NtImports::fn_KeRaiseIrqlToDpcLevel},
    {str_hash("KeLowerIrql"), &NtImports::fn_KeLowerIrql},
    {str_hash("PsCreateSystemThread"), &NtImports::fn_PsCreateSystemThread},
    {str_hash("PsTerminateSystemThread"), &NtImports::fn_PsTerminateSystemThread},
    {str_hash("PsLookupProcessByProcessId"), &NtImports::fn_PsLookupProcessByProcessId},
    {str_hash("KeIpiGenericCall"), &NtImports::fn_KeIpiGenericCall},
    {str_hash("MmGetPhysicalMemoryRanges"), &NtImports::fn_MmGetPhysicalMemoryRanges},
    {str_hash("RtlCopyMemory"), &NtImports::fn_RtlCopyMemory},
    {str_hash("RtlFillMemory"), &NtImports::fn_RtlFillMemory},
    {str_hash("RtlCompareUnicodeString"), &NtImports::fn_RtlCompareUnicodeString},
    {str_hash("RtlEqualUnicodeString"), &NtImports::fn_RtlEqualUnicodeString},
    {str_hash("RtlCopyUnicodeString"), &NtImports::fn_RtlCopyUnicodeString},
    {str_hash("RtlFreeUnicodeString"), &NtImports::fn_RtlFreeUnicodeString},
    {str_hash("MmAllocateContiguousMemorySpecifyCacheNode"), &NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode},
    {str_hash("MmFreeContiguousMemorySpecifyCache"), &NtImports::fn_MmFreeContiguousMemorySpecifyCache},
    {str_hash("PsInitialSystemProcess"), &NtImports::fn_PsInitialSystemProcess},
    {str_hash("PsGetCurrentProcess"), &NtImports::fn_PsGetCurrentProcess},
    {str_hash("PsGetCurrentProcessId"), &NtImports::fn_PsGetCurrentProcessId},
    {str_hash("RtlInitUnicodeString"), &NtImports::fn_RtlInitUnicodeString},
    {str_hash("ZwOpenFile"), &NtImports::fn_ZwOpenFile},
    {str_hash("ZwCreateFile"), &NtImports::fn_ZwCreateFile},
    {str_hash("ZwWriteFile"), &NtImports::fn_ZwWriteFile},
    {str_hash("ZwReadFile"), &NtImports::fn_ZwReadFile},
    {str_hash("ZwQueryInformationFile"), &NtImports::fn_ZwQueryInformationFile},
    {str_hash("ZwDeviceIoControlFile"), &NtImports::fn_ZwDeviceIoControlFile},
    {str_hash("ZwClose"), &NtImports::fn_ZwClose},
    {str_hash("KeGetCurrentThread"), &NtImports::fn_KeGetCurrentThread},
    {str_hash("PsGetProcessId"), &NtImports::fn_PsGetProcessId},
    {str_hash("MmIsIoSpaceActive"), &NtImports::fn_MmIsIoSpaceActive},
    {str_hash("IoCreateDevice"), &NtImports::fn_IoCreateDevice},
    {str_hash("IoCreateDriver"), &NtImports::fn_IoCreateDriver},
    {str_hash("IoCreateSymbolicLink"), &NtImports::fn_IoCreateSymbolicLink},
    {str_hash("IoDeleteSymbolicLink"), &NtImports::fn_IoDeleteSymbolicLink},
    {str_hash("IoDeleteDevice"), &NtImports::fn_IoDeleteDevice},
    {str_hash("IofCompleteRequest"), &NtImports::fn_IofCompleteRequest},
};

NTSTATUS resolve_imports()
{
    auto kernel_base = Utils::GetKernelBase();
    if (!kernel_base)
        return STATUS_UNSUCCESSFUL;

    Utils::GetProcAddressBuffer(
        kernel_base,
        function_table,
        sizeof(function_table) / sizeof(FUNCTION_TABLE_ENTRY));

    return STATUS_SUCCESS;
}

NTSTATUS resolve_sigged_imports()
{
    auto kernel_base = Utils::GetKernelBase();
    if (!kernel_base)
        return STATUS_UNSUCCESSFUL;

    QWORD kernel_text_base, kernel_text_size;

    if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, str_hash(".text"), &kernel_text_base, &kernel_text_size)))
        return STATUS_UNSUCCESSFUL;

    NtImports::fn_MmPfnDatabase = (decltype(NtImports::fn_MmPfnDatabase))Utils::ResolveRel32(3, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("48 8B 3D ? ? ? ? 48 C1 EF ? 49 23 ? ? 8B")));
    NtImports::fn_MiGetSystemRegionType = (decltype(NtImports::fn_MiGetSystemRegionType))Utils::ResolveRel32(1, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("E8 ? ? ? ? 8B C8 45 84 FE")));
    NtImports::fn_MiSystemRegionTypeDatabase = (decltype(NtImports::fn_MiSystemRegionTypeDatabase))Utils::ResolveRel32(3, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("48 8D 0D ? ? ? ? 0F B6 04 08 C3")));

    return STATUS_SUCCESS;
}
