#include "bootstrap.hpp"

FUNCTION_TABLE_ENTRY function_table[]{
    {str_hash("ExAllocatePool"), &nt.fn_ExAllocatePool},
    {str_hash("ExAllocatePoolWithTag"), &nt.fn_ExAllocatePoolWithTag},
    {str_hash("ExAllocatePool2"), &nt.fn_ExAllocatePool2},
    {str_hash("ExFreePool"), &nt.fn_ExFreePool},
    {str_hash("ExFreePoolWithTag"), &nt.fn_ExFreePoolWithTag},
    {str_hash("ObReferenceObject"), &nt.fn_ObReferenceObject},
    {str_hash("ObDereferenceObject"), &nt.fn_ObDereferenceObject},
    {str_hash("DbgPrintEx"), &nt.fn_DbgPrintEx},
    {str_hash("vDbgPrintEx"), &nt.fn_vDbgPrintEx},
    {str_hash("sprintf"), &nt.fn_sprintf},
    {str_hash("vsprintf"), &nt.fn_vsprintf},
    {str_hash("swprintf"), &nt.fn_swprintf},
    {str_hash("vswprintf"), &nt.fn_vswprintf},
    {str_hash("IoAllocateIrp"), &nt.fn_IoAllocateIrp},
    {str_hash("IoFreeIrp"), &nt.fn_IoFreeIrp},
    {str_hash("IoAllocateWorkItem"), &nt.fn_IoAllocateWorkItem},
    {str_hash("IoQueueWorkItem"), &nt.fn_IoQueueWorkItem},
    {str_hash("IoFreeWorkItem"), &nt.fn_IoFreeWorkItem},
    {str_hash("IoAllocateMdl"), &nt.fn_IoAllocateMdl},
    {str_hash("IoFreeMdl"), &nt.fn_IoFreeMdl},
    {str_hash("KeQueryActiveProcessorCount"), &nt.fn_KeQueryActiveProcessorCount},
    {str_hash("KeInitializeEvent"), &nt.fn_KeInitializeEvent},
    {str_hash("KeSetEvent"), &nt.fn_KeSetEvent},
    {str_hash("KeResetEvent"), &nt.fn_KeResetEvent},
    {str_hash("KeClearEvent"), &nt.fn_KeClearEvent},
    {str_hash("KeWaitForSingleObject"), &nt.fn_KeWaitForSingleObject},
    {str_hash("MmGetPhysicalAddress"), &nt.fn_MmGetPhysicalAddress},
    {str_hash("MmGetVirtualForPhysical"), &nt.fn_MmGetVirtualForPhysical},
    {str_hash("MmCopyMemory"), &nt.fn_MmCopyMemory},
    {str_hash("MmMapIoSpace"), &nt.fn_MmMapIoSpace},
    {str_hash("MmUnmapIoSpace"), &nt.fn_MmUnmapIoSpace},
    {str_hash("MmIsAddressValid"), &nt.fn_MmIsAddressValid},
    {str_hash("MmGetSystemRoutineAddress"), &nt.fn_MmGetSystemRoutineAddress},
    {str_hash("MmProbeAndLockPages"), &nt.fn_MmProbeAndLockPages},
    {str_hash("MmUnlockPages"), &nt.fn_MmUnlockPages},
    {str_hash("MmMapLockedPagesSpecifyCache"), &nt.fn_MmMapLockedPagesSpecifyCache},
    {str_hash("MmUnmapLockedPages"), &nt.fn_MmUnmapLockedPages},
    {str_hash("MmProtectMdlSystemAddress"), &nt.fn_MmProtectMdlSystemAddress},
    {str_hash("RtlCaptureContext"), &nt.fn_RtlCaptureContext},
    {str_hash("KeStackAttachProcess"), &nt.fn_KeStackAttachProcess},
    {str_hash("KeUnstackDetachProcess"), &nt.fn_KeUnstackDetachProcess},
    {str_hash("KeDelayExecutionThread"), &nt.fn_KeDelayExecutionThread},
    {str_hash("KeSetSystemAffinityThread"), &nt.fn_KeSetSystemAffinityThread},
    {str_hash("KeAcquireSpinLock"), &nt.fn_KeAcquireSpinLock},
    {str_hash("KeReleaseSpinLock"), &nt.fn_KeReleaseSpinLock},
    {str_hash("KeRaiseIrqlToDpcLevel"), &nt.fn_KeRaiseIrqlToDpcLevel},
    {str_hash("KeLowerIrql"), &nt.fn_KeLowerIrql},
    {str_hash("PsCreateSystemThread"), &nt.fn_PsCreateSystemThread},
    {str_hash("PsTerminateSystemThread"), &nt.fn_PsTerminateSystemThread},
    {str_hash("PsLookupProcessByProcessId"), &nt.fn_PsLookupProcessByProcessId},
    {str_hash("KeIpiGenericCall"), &nt.fn_KeIpiGenericCall},
    {str_hash("MmGetPhysicalMemoryRanges"), &nt.fn_MmGetPhysicalMemoryRanges},
    {str_hash("RtlCopyMemory"), &nt.fn_RtlCopyMemory},
    {str_hash("RtlFillMemory"), &nt.fn_RtlFillMemory},
    {str_hash("RtlCompareUnicodeString"), &nt.fn_RtlCompareUnicodeString},
    {str_hash("RtlEqualUnicodeString"), &nt.fn_RtlEqualUnicodeString},
    {str_hash("RtlCopyUnicodeString"), &nt.fn_RtlCopyUnicodeString},
    {str_hash("RtlFreeUnicodeString"), &nt.fn_RtlFreeUnicodeString},
    {str_hash("MmAllocateContiguousMemorySpecifyCacheNode"), &nt.fn_MmAllocateContiguousMemorySpecifyCacheNode},
    {str_hash("MmFreeContiguousMemorySpecifyCache"), &nt.fn_MmFreeContiguousMemorySpecifyCache},
    {str_hash("PsInitialSystemProcess"), &nt.fn_PsInitialSystemProcess},
    {str_hash("PsGetCurrentProcess"), &nt.fn_PsGetCurrentProcess},
    {str_hash("PsGetCurrentProcessId"), &nt.fn_PsGetCurrentProcessId},
    {str_hash("RtlInitUnicodeString"), &nt.fn_RtlInitUnicodeString},
    {str_hash("ZwOpenFile"), &nt.fn_ZwOpenFile},
    {str_hash("ZwCreateFile"), &nt.fn_ZwCreateFile},
    {str_hash("ZwWriteFile"), &nt.fn_ZwWriteFile},
    {str_hash("ZwReadFile"), &nt.fn_ZwReadFile},
    {str_hash("ZwQueryInformationFile"), &nt.fn_ZwQueryInformationFile},
    {str_hash("ZwDeviceIoControlFile"), &nt.fn_ZwDeviceIoControlFile},
    {str_hash("ZwClose"), &nt.fn_ZwClose},
    {str_hash("KeGetCurrentThread"), &nt.fn_KeGetCurrentThread},
    {str_hash("PsGetProcessId"), &nt.fn_PsGetProcessId},
    {str_hash("MmIsIoSpaceActive"), &nt.fn_MmIsIoSpaceActive},
    {str_hash("IoCreateDevice"), &nt.fn_IoCreateDevice},
    {str_hash("IoCreateDriver"), &nt.fn_IoCreateDriver},
    {str_hash("IoCreateSymbolicLink"), &nt.fn_IoCreateSymbolicLink},
    {str_hash("IoDeleteSymbolicLink"), &nt.fn_IoDeleteSymbolicLink},
    {str_hash("IoDeleteDevice"), &nt.fn_IoDeleteDevice},
    {str_hash("IofCompleteRequest"), &nt.fn_IofCompleteRequest},
    {str_hash("KeRegisterNmiCallback"), &nt.fn_KeRegisterNmiCallback},
    {str_hash("KeDeregisterNmiCallback"), &nt.fn_KeDeregisterNmiCallback},
    {str_hash("HalSendNMI"), &nt.fn_HalSendNMI},
    {str_hash("KeAddProcessorAffinityEx"), &nt.fn_KeAddProcessorAffinityEx},
    {str_hash("KeInitializeAffinityEx"), &nt.fn_KeInitializeAffinityEx},
    {str_hash("ObfDereferenceObject"), &nt.fn_ObfDereferenceObject},
    {str_hash("KeGetCurrentProcessorNumberEx"), &nt.fn_KeGetCurrentProcessorNumberEx},
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

    UINT64 kernel_text_base, kernel_text_size;

    if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, str_hash(".text"), &kernel_text_base, &kernel_text_size)))
        return STATUS_UNSUCCESSFUL;

    nt.fn_MmPfnDatabase = (decltype(nt.fn_MmPfnDatabase))Utils::ResolveRel32(3, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("48 8B 3D ? ? ? ? 48 C1 EF ? 49 23 ? ? 8B")));
    nt.fn_MiGetSystemRegionType = (decltype(nt.fn_MiGetSystemRegionType))Utils::ResolveRel32(1, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("E8 ? ? ? ? 8B C8 45 84 FE")));
    nt.fn_MiSystemRegionTypeDatabase = (decltype(nt.fn_MiSystemRegionTypeDatabase))Utils::ResolveRel32(3, Utils::SigScan(kernel_text_base, kernel_text_size, pattern("48 8D 0D ? ? ? ? 0F B6 04 08 C3")));

    return STATUS_SUCCESS;
}