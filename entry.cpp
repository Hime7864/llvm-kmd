#include "imports.hpp"

NTSTATUS resolve_imports()
{
    auto kernel_base = Utils::GetKernelBase();
    if (!kernel_base)
        return STATUS_UNSUCCESSFUL;

    NtImports::fn_ExAllocatePool = (decltype(NtImports::fn_ExAllocatePool))Utils::GetProcAddress(
        kernel_base,
        "ExAllocatePool"
    );

    NtImports::fn_ExFreePool = (decltype(NtImports::fn_ExFreePool))Utils::GetProcAddress(
        kernel_base,
        "ExFreePool"
    );

    NtImports::fn_DbgPrintEx = (decltype(NtImports::fn_DbgPrintEx))Utils::GetProcAddress(
        kernel_base,
        "DbgPrintEx"
    );

    NtImports::fn_sprintf = (decltype(NtImports::fn_sprintf))Utils::GetProcAddress(
        kernel_base,
        "sprintf"
    );

    NtImports::fn_swprintf = (decltype(NtImports::fn_swprintf))Utils::GetProcAddress(
        kernel_base,
        "swprintf"
    );

    NtImports::fn_IoAllocateMdl = (decltype(NtImports::fn_IoAllocateMdl))Utils::GetProcAddress(
        kernel_base,
        "IoAllocateMdl"
    );

    NtImports::fn_IoFreeMdl = (decltype(NtImports::fn_IoFreeMdl))Utils::GetProcAddress(
        kernel_base,
        "IoFreeMdl"
    );

    NtImports::fn_KeQueryActiveProcessorCount = (decltype(NtImports::fn_KeQueryActiveProcessorCount))Utils::GetProcAddress(
        kernel_base,
        "KeQueryActiveProcessorCount"
    );

    NtImports::fn_MmGetPhysicalAddress = (decltype(NtImports::fn_MmGetPhysicalAddress))Utils::GetProcAddress(
        kernel_base,
        "MmGetPhysicalAddress"
    );

    NtImports::fn_MmGetVirtualForPhysical = (decltype(NtImports::fn_MmGetVirtualForPhysical))Utils::GetProcAddress(
        kernel_base,
        "MmGetVirtualForPhysical"
    );

    NtImports::fn_MmCopyMemory = (decltype(NtImports::fn_MmCopyMemory))Utils::GetProcAddress(
        kernel_base,
        "MmCopyMemory"
    );

    NtImports::fn_MmMapIoSpace = (decltype(NtImports::fn_MmMapIoSpace))Utils::GetProcAddress(
        kernel_base,
        "MmMapIoSpace"
    );

    NtImports::fn_MmUnmapIoSpace = (decltype(NtImports::fn_MmUnmapIoSpace))Utils::GetProcAddress(
        kernel_base,
        "MmUnmapIoSpace"
    );

    NtImports::fn_RtlCaptureContext = (decltype(NtImports::fn_RtlCaptureContext))Utils::GetProcAddress(
        kernel_base,
        "RtlCaptureContext"
    );

    NtImports::fn_KeStackAttachProcess = (decltype(NtImports::fn_KeStackAttachProcess))Utils::GetProcAddress(
        kernel_base,
        "KeStackAttachProcess"
    );

    NtImports::fn_KeUnstackDetachProcess = (decltype(NtImports::fn_KeUnstackDetachProcess))Utils::GetProcAddress(
        kernel_base,
        "KeUnstackDetachProcess"
    );

    NtImports::fn_KeDelayExecutionThread = (decltype(NtImports::fn_KeDelayExecutionThread))Utils::GetProcAddress(
        kernel_base,
        "KeDelayExecutionThread"
    );

    NtImports::fn_PsCreateSystemThread = (decltype(NtImports::fn_PsCreateSystemThread))Utils::GetProcAddress(
        kernel_base,
        "PsCreateSystemThread"
    );

    NtImports::fn_PsTerminateSystemThread = (decltype(NtImports::fn_PsTerminateSystemThread))Utils::GetProcAddress(
        kernel_base,
        "PsTerminateSystemThread"
    );

    NtImports::fn_KeIpiGenericCall = (decltype(NtImports::fn_KeIpiGenericCall))Utils::GetProcAddress(
        kernel_base,
        "KeIpiGenericCall"
    );

    NtImports::fn_MmGetPhysicalMemoryRanges = (decltype(NtImports::fn_MmGetPhysicalMemoryRanges))Utils::GetProcAddress(
        kernel_base,
        "MmGetPhysicalMemoryRanges"
    );

    NtImports::fn_RtlCopyMemory = (decltype(NtImports::fn_RtlCopyMemory))Utils::GetProcAddress(
        kernel_base,
        "RtlCopyMemory"
    );

    NtImports::fn_RtlFillMemory = (decltype(NtImports::fn_RtlFillMemory))Utils::GetProcAddress(
        kernel_base,
        "RtlFillMemory"
    );

    NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode = (decltype(NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode))Utils::GetProcAddress(
        kernel_base,
        "MmAllocateContiguousMemorySpecifyCacheNode"
    );

    NtImports::fn_MmFreeContiguousMemorySpecifyCache = (decltype(NtImports::fn_MmFreeContiguousMemorySpecifyCache))Utils::GetProcAddress(
        kernel_base,
        "MmFreeContiguousMemorySpecifyCache"
    );

    NtImports::fn_PsInitialSystemProcess = (decltype(NtImports::fn_PsInitialSystemProcess))Utils::GetProcAddress(
        kernel_base,
        "PsInitialSystemProcess"
    );

    NtImports::fn_RtlInitUnicodeString = (decltype(NtImports::fn_RtlInitUnicodeString))Utils::GetProcAddress(
        kernel_base,
        "RtlInitUnicodeString"
    );

    NtImports::fn_ZwCreateFile = (decltype(NtImports::fn_ZwCreateFile))Utils::GetProcAddress(
        kernel_base,
        "ZwCreateFile"
    );

    NtImports::fn_ZwWriteFile = (decltype(NtImports::fn_ZwWriteFile))Utils::GetProcAddress(
        kernel_base,
        "ZwWriteFile"
    );

    NtImports::fn_ZwReadFile = (decltype(NtImports::fn_ZwReadFile))Utils::GetProcAddress(
        kernel_base,
        "ZwReadFile"
    );

    NtImports::fn_ZwClose = (decltype(NtImports::fn_ZwClose))Utils::GetProcAddress(
        kernel_base,
        "ZwClose"
    );

    NtImports::fn_KeGetCurrentThread = (decltype(NtImports::fn_KeGetCurrentThread))Utils::GetProcAddress(
        kernel_base,
        "KeGetCurrentThread"
    );

    NtImports::fn_GetUniqueProcessId = (decltype(NtImports::fn_GetUniqueProcessId))Utils::GetProcAddress(
        kernel_base,
        "PsGetProcessId"
    );

    return STATUS_SUCCESS;
}

NTSTATUS resolve_sigged_imports()
{
	auto kernel_base = Utils::GetKernelBase();
	if (!kernel_base)
		return STATUS_UNSUCCESSFUL;

	QWORD kernel_text_base, kernel_text_size;
	if (!NT_SUCCESS(Utils::GetSectionInfo(kernel_base, ".text", &kernel_text_base, &kernel_text_size)))
		return STATUS_UNSUCCESSFUL;

	NtImports::fn_MmPfnDatabase = (decltype(NtImports::fn_MmPfnDatabase))Utils::ResolveRel32(3, Utils::SigScan(kernel_text_base, kernel_text_size, "48 8B 3D ? ? ? ? 48 C1 EF 09"));
	
	return STATUS_SUCCESS;
}

void FreeAndExit(PVOID last_thread)
{
	QWORD host_driver_base = 0;
	QWORD host_driver_size = 0;
	if (!NT_SUCCESS(Utils::LocateSelf(&host_driver_base, &host_driver_size)))
		return;
	__writecr8(0);
	Sleep(250);
	QWORD func1 = (QWORD)NtImports::fn_ExFreePool;
	QWORD func2 = (QWORD)NtImports::fn_PsTerminateSystemThread;
	auto func3 = NtImports::fn_RtlFillMemory;
	auto func_base = (PVOID)FreeAndExit;
	auto range1 = ((QWORD)func_base - host_driver_base) - 8;
	auto range2 = (host_driver_size - (range1 + 0x110));

	//func3(last_thread, (SIZE_T)0x800, 0x00);
	func3((PVOID)host_driver_base, (SIZE_T)range1, 0x00);
	func3((PVOID)(host_driver_base + (range1 + 0x110)), (SIZE_T)range2, 0x00);
	func3(func_base, (SIZE_T)0x90, 0x00);
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
		status = resolve_sigged_imports();
		if (NT_SUCCESS(status))
			status = DriverEntry(nullptr, nullptr);
		CleanupDriver();
		KeUnstackDetachProcess(&apc);
	}
	return status;
}