#include "imports.hpp"

NTSTATUS resolve_imports()
{
	auto kernel_base = Utils::GetKernelBase();
	if (!kernel_base)
		return STATUS_UNSUCCESSFUL;

	fn_DbgPrintEx = (_DbgPrintEx)Utils::GetProcAddress(
		kernel_base,
		"DbgPrintEx"
	);

	fn_sprintf = (_sprintf)Utils::GetProcAddress(
		kernel_base,
		"sprintf"
	);

	fn_swprintf = (_swprintf)Utils::GetProcAddress(
		kernel_base,
		"swprintf"
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

	fn_MmGetPhysicalMemoryRanges = (_MmGetPhysicalMemoryRanges)Utils::GetProcAddress(
		kernel_base,
		"MmGetPhysicalMemoryRanges"
	);

	fn_RtlCopyMemory = (_RtlCopyMemory)Utils::GetProcAddress(
		kernel_base,
		"RtlCopyMemory"
	);

	fn_RtlFillMemory = (_RtlFillMemory)Utils::GetProcAddress(
		kernel_base,
		"RtlFillMemory"
	);

	fn_MmAllocateContiguousMemorySpecifyCacheNode = (_MmAllocateContiguousMemorySpecifyCacheNode)Utils::GetProcAddress(
		kernel_base,
		"MmAllocateContiguousMemorySpecifyCacheNode"
	);

	fn_MmFreeContiguousMemorySpecifyCache = (_MmFreeContiguousMemorySpecifyCache)Utils::GetProcAddress(
		kernel_base,
		"MmFreeContiguousMemorySpecifyCache"
	);

	fn_PsInitialSystemProcess = (_PsInitialSystemProcess)Utils::GetProcAddress(
		kernel_base,
		"PsInitialSystemProcess"
	);

	fn_RtlInitUnicodeString = (_RtlInitUnicodeString)Utils::GetProcAddress(
		kernel_base,
		"RtlInitUnicodeString"
	);

	fn_ZwCreateFile = (_ZwCreateFile)Utils::GetProcAddress(
		kernel_base,
		"ZwCreateFile"
	);

	fn_ZwWriteFile = (_ZwWriteFile)Utils::GetProcAddress(
		kernel_base,
		"ZwWriteFile"
	);

	fn_ZwClose = (_ZwClose)Utils::GetProcAddress(
		kernel_base,
		"ZwClose"
	);

	fn_KeGetCurrentThread = (_KeGetCurrentThread)Utils::GetProcAddress(
		kernel_base,
		"KeGetCurrentThread"
	);

	return STATUS_SUCCESS;
}

NTSTATUS resolve_sigged_imports()
{
	auto kernel_base = Utils::GetKernelBase();
	if (!kernel_base)
		return STATUS_UNSUCCESSFUL;

	QWORD kernel_text_base, kernel_text_size;
	if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, ".text", &kernel_text_base, (DWORD*)&kernel_text_size)))
		return STATUS_UNSUCCESSFUL;

	fn_MmPfnDatabase = (_MmPfnDatabase)Utils::deref(3, Utils::sig_scan(kernel_text_base, kernel_text_size, "48 8B 3D ? ? ? ? 48 C1 EF 09"));

	return STATUS_SUCCESS;
}

void FreeAndExit(PVOID last_thread)
{
	QWORD host_driver_base = 0;
	QWORD host_driver_size = 0;
	if (!NT_SUCCESS(Utils::SelfModuleBase(&host_driver_base, &host_driver_size)))
		return;
	__writecr8(0);
	Sleep(1500);
	QWORD func1 = (QWORD)fn_ExFreePool;
	QWORD func2 = (QWORD)fn_PsTerminateSystemThread;
	auto func3 = fn_RtlFillMemory;
	auto func_base = (PVOID)FreeAndExit;
	auto range1 = ((QWORD)func_base - host_driver_base) - 1;
	auto range2 = (host_driver_size - (range1 + 0x120));

	func3(last_thread, (SIZE_T)0x400, 0x00);
	func3((PVOID)host_driver_base, (SIZE_T)range1, 0x00);
	func3((PVOID)(host_driver_base + (range1 + 0x120)), (SIZE_T)range2, 0x00);
	func3(func_base, (SIZE_T)0xB0, 0x00);
	__asm {
		mov rcx, [host_driver_base]
		mov rdx, [func1]
		mov r8, [func2]
		call self
	self:
		sub rsp, 8h
		mov rax, r8
		mov[rsp], rax
		mov rax, rdx
		jmp rax
	}
}

void CleanupDriver()
{
	HANDLE thread_handle = 0;
	_OBJECT_ATTRIBUTES object_attribues{ };
	InitializeObjectAttributes(&object_attribues, nullptr, OBJ_KERNEL_HANDLE, 0, nullptr);
	PsCreateSystemThread(&thread_handle, 0, &object_attribues, 0, 0, (PKSTART_ROUTINE)&FreeAndExit, KeGetCurrentThread());
	return;
}

NTSTATUS volatile start()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	if (NT_SUCCESS(resolve_imports()))
	{
		KAPC_STATE apc{ 0 };
		KeStackAttachProcess(PsInitialSystemProcess(), &apc);

		if(NT_SUCCESS(resolve_sigged_imports()))
		{
			status = DriverEntry(nullptr, nullptr);
		}
		CleanupDriver();
		KeUnstackDetachProcess(&apc);
	}
	return status;
}