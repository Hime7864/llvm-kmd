#include "imports.h"

NTSTATUS resolve_imports()
{
	auto kernel_base = Utils::GetKernelBase();
	if (!kernel_base)
		return STATUS_UNSUCCESSFUL;

	fn_DbgPrintEx = (_DbgPrintEx)Utils::GetProcAddress(
		kernel_base,
		"DbgPrintEx"
	);

	fn_MmGetPhysicalAddress = (_MmGetPhysicalAddress)Utils::GetProcAddress(
		kernel_base,
		"MmGetPhysicalAddress"
	);

	fn_ExAllocatePool = (_ExAllocatePool)Utils::GetProcAddress(
		kernel_base,
		"ExAllocatePool"
	);

	fn_ExFreePool = (_ExFreePool)Utils::GetProcAddress(
		kernel_base,
		"ExFreePool"
	);

	fn_DbgPrintEx = (_DbgPrintEx)Utils::GetProcAddress(
		kernel_base,
		"DbgPrintEx"
	);

	fn_IoAllocateMdl = (_IoAllocateMdl)Utils::GetProcAddress(
		kernel_base,
		"IoAllocateMdl"
	);

	fn_IoFreeMdl = (_IoFreeMdl)Utils::GetProcAddress(
		kernel_base,
		"IoFreeMdl"
	);

	fn_KeQueryActiveProcessorCount = (_KeQueryActiveProcessorCount)Utils::GetProcAddress(
		kernel_base,
		"KeQueryActiveProcessorCount"
	);

	fn_MmGetPhysicalAddress = (_MmGetPhysicalAddress)Utils::GetProcAddress(
		kernel_base,
		"MmGetPhysicalAddress"
	);

	fn_MmGetVirtualForPhysical = (_MmGetVirtualForPhysical)Utils::GetProcAddress(
		kernel_base,
		"MmGetVirtualForPhysical"
	);

	fn_MmCopyMemory = (_MmCopyMemory)Utils::GetProcAddress(
		kernel_base,
		"MmCopyMemory"
	);

	fn_MmMapIoSpace = (_MmMapIoSpace)Utils::GetProcAddress(
		kernel_base,
		"MmMapIoSpace"
	);

	fn_MmUnmapIoSpace = (_MmUnmapIoSpace)Utils::GetProcAddress(
		kernel_base,
		"MmUnmapIoSpace"
	);

	fn_RtlCaptureContext = (_RtlCaptureContext)Utils::GetProcAddress(
		kernel_base,
		"RtlCaptureContext"
	);

	fn_KeStackAttachProcess = (_KeStackAttachProcess)Utils::GetProcAddress(
		kernel_base,
		"KeStackAttachProcess"
	);

	fn_KeUnstackDetachProcess = (_KeUnstackDetachProcess)Utils::GetProcAddress(
		kernel_base,
		"KeUnstackDetachProcess"
	);

	fn_KeDelayExecutionThread = (_KeDelayExecutionThread)Utils::GetProcAddress(
		kernel_base,
		"KeDelayExecutionThread"
	);

	fn_PsCreateSystemThread = (_PsCreateSystemThread)Utils::GetProcAddress(
		kernel_base,
		"PsCreateSystemThread"
	);

	fn_PsTerminateSystemThread = (_PsTerminateSystemThread)Utils::GetProcAddress(
		kernel_base,
		"PsTerminateSystemThread"
	);

	fn_KeIpiGenericCall = (_KeIpiGenericCall)Utils::GetProcAddress(
		kernel_base,
		"KeIpiGenericCall"
	);

	return STATUS_SUCCESS;
}

NTSTATUS start()
{
	if(NT_SUCCESS(resolve_imports()))
		return DriverEntry(nullptr, nullptr);
	return STATUS_UNSUCCESSFUL;
}