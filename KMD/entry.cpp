#include "intrinsics.hpp"

FUNCTION_TABLE_ENTRY function_table[]
{
	{ str_hash("ExAllocatePool"), &NtImports::fn_ExAllocatePool},
	{ str_hash("ExFreePool"), &NtImports::fn_ExFreePool},
	{ str_hash("DbgPrintEx"), &NtImports::fn_DbgPrintEx},
	{ str_hash("vDbgPrintEx"), &NtImports::fn_vDbgPrintEx},
	{ str_hash("sprintf"), &NtImports::fn_sprintf},
	{ str_hash("vsprintf"), &NtImports::fn_vsprintf},
	{ str_hash("swprintf"), &NtImports::fn_swprintf},
	{ str_hash("vswprintf"), &NtImports::fn_vswprintf},
	{ str_hash("IoAllocateMdl"), &NtImports::fn_IoAllocateMdl},
	{ str_hash("IoFreeMdl"), &NtImports::fn_IoFreeMdl},
	{ str_hash("KeQueryActiveProcessorCount"), &NtImports::fn_KeQueryActiveProcessorCount},
	{ str_hash("MmGetPhysicalAddress"), &NtImports::fn_MmGetPhysicalAddress},
	{ str_hash("MmGetVirtualForPhysical"), &NtImports::fn_MmGetVirtualForPhysical},
	{ str_hash("MmCopyMemory"), &NtImports::fn_MmCopyMemory},
	{ str_hash("MmMapIoSpace"), &NtImports::fn_MmMapIoSpace},
	{ str_hash("MmUnmapIoSpace"), &NtImports::fn_MmUnmapIoSpace},
	{ str_hash("RtlCaptureContext"), &NtImports::fn_RtlCaptureContext},
	{ str_hash("KeStackAttachProcess"), &NtImports::fn_KeStackAttachProcess},
	{ str_hash("KeUnstackDetachProcess"), &NtImports::fn_KeUnstackDetachProcess},
	{ str_hash("KeDelayExecutionThread"), &NtImports::fn_KeDelayExecutionThread},
	{ str_hash("KeSetSystemAffinityThread"), &NtImports::fn_KeSetSystemAffinityThread},
	{ str_hash("PsCreateSystemThread"), &NtImports::fn_PsCreateSystemThread},
	{ str_hash("PsTerminateSystemThread"), &NtImports::fn_PsTerminateSystemThread},
	{ str_hash("KeIpiGenericCall"), &NtImports::fn_KeIpiGenericCall},
	{ str_hash("MmGetPhysicalMemoryRanges"), &NtImports::fn_MmGetPhysicalMemoryRanges},
	{ str_hash("RtlCopyMemory"), &NtImports::fn_RtlCopyMemory},
	{ str_hash("RtlFillMemory"), &NtImports::fn_RtlFillMemory},
	{ str_hash("MmAllocateContiguousMemorySpecifyCacheNode"), &NtImports::fn_MmAllocateContiguousMemorySpecifyCacheNode},
	{ str_hash("MmFreeContiguousMemorySpecifyCache"), &NtImports::fn_MmFreeContiguousMemorySpecifyCache},
	{ str_hash("PsInitialSystemProcess"), &NtImports::fn_PsInitialSystemProcess},
	{ str_hash("RtlInitUnicodeString"), &NtImports::fn_RtlInitUnicodeString},
	{ str_hash("ZwCreateFile"), &NtImports::fn_ZwCreateFile},
	{ str_hash("ZwWriteFile"), &NtImports::fn_ZwWriteFile},
	{ str_hash("ZwReadFile"), &NtImports::fn_ZwReadFile},
	{ str_hash("ZwClose"), &NtImports::fn_ZwClose},
	{ str_hash("KeGetCurrentThread"), &NtImports::fn_KeGetCurrentThread},
	{ str_hash("PsGetProcessId"), &NtImports::fn_PsGetProcessId},
	{ str_hash("MmIsIoSpaceActive"), &NtImports::fn_MmIsIoSpaceActive},
	{ str_hash("IoCreateDevice"), &NtImports::fn_IoCreateDevice},
	{ str_hash("IoCreateDriver"), &NtImports::fn_IoCreateDriver},
	{ str_hash("IoCreateSymbolicLink"), &NtImports::fn_IoCreateSymbolicLink},
	{ str_hash("IoDeleteSymbolicLink"), &NtImports::fn_IoDeleteSymbolicLink},
	{ str_hash("IoDeleteDevice"), &NtImports::fn_IoDeleteDevice},
	{ str_hash("IofCompleteRequest"), &NtImports::fn_IofCompleteRequest},
};

NTSTATUS resolve_imports()
{
    auto kernel_base = Utils::GetKernelBase();
    if (!kernel_base)
        return STATUS_UNSUCCESSFUL;

    Utils::GetProcAddressBuffer(
        kernel_base,
        function_table,
        sizeof(function_table) / sizeof(FUNCTION_TABLE_ENTRY)
    );

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
volatile void FreeAndExit()
{
	QWORD host_driver_base = 0;
	QWORD host_driver_size = 0;
	if (!NT_SUCCESS(Utils::LocateSelf(&host_driver_base, &host_driver_size)))
		return;
	__writecr8(0);
	Sleep(200);
	QWORD func1 = (QWORD)NtImports::fn_ExFreePool;
	QWORD func2 = (QWORD)NtImports::fn_PsTerminateSystemThread;
	auto func3 = NtImports::fn_RtlFillMemory;
	auto func_base = (PVOID)FreeAndExit;
	auto range1 = ((QWORD)func_base - host_driver_base) - 8;
	auto range2 = (host_driver_size - (range1 + 0x110));
	auto cr3 = __readcr3();


	func3((PVOID)host_driver_base, (SIZE_T)range1, 0x00);
	func3((PVOID)(host_driver_base + (range1 + 0x110)), (SIZE_T)range2, 0x00);
	func3(func_base, (SIZE_T)0xB0, 0x00);

	if (cr3 == 0x1AD000ull)
	{
		__asm {
			xor ecx, ecx
			mov rax, [func2]
			call rax
		}
	}
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
	PsCreateSystemThread(&thread_handle, 0, &object_attribues, 0, 0, (PKSTART_ROUTINE)&FreeAndExit,0);
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
			status = DriverEntry();
		CleanupDriver();
		KeUnstackDetachProcess(&apc);
	}
	return status;
}