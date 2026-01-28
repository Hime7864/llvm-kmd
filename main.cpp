#include <intrinsics.hpp>

#define printf(Format, ...) NtImports::fn_DbgPrintEx(0, 0, Format, __VA_ARGS__)

PEPROCESS FindEproc(UINT32 pid)
{
	auto flink = (UINT64)PsInitialSystemProcess();
	auto o_UniqueProcessId = *(UINT32*)((UINT64)NtImports::fn_PsGetProcessId + 0x3);// fix for 22H2 & 24H2
	do
	{
		if (PsGetProcessId((PEPROCESS)flink) == (UINT64)pid)
		{
			return (PEPROCESS)flink;
		}

		flink = *(UINT64*)((UINT64)flink + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
	} while (flink != (UINT64)PsInitialSystemProcess());
	return nullptr;
}

void LocateUsermodePteManipulationSessioned(PEPROCESS eproc)
{
	SIZE_T bytes_copied = 0;
	KAPC_STATE apc;

	KeStackAttachProcess(eproc, &apc);
	auto DTB = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;

	auto cpy_pml4 = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
	MmCopyMemory(cpy_pml4, DTB, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);

	auto pml4 = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(DTB);
	if (!pml4)
	{
		printf("PID 0x%08X PML4 Fucked\n");
		return;
	}
	for (int i = 0; i < 255; i++)
	{
		if (cpy_pml4[i].AsUINT64 != pml4[i].AsUINT64)
		{
			printf("PID 0x%08X [ Translation Level 0: %i ] -> pt tampering detected! %p vs %p\n", PsGetProcessId(eproc), i, cpy_pml4[i].AsUINT64, pml4[i].AsUINT64);
		}
		if (pml4[i].Valid)
		{
			auto cpy_pdpt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
			MmCopyMemory(cpy_pdpt, pml4[i].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);

			auto pdpt = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pml4[i].PageFrameNumber << PAGE_SHIFT);
			if (!pdpt)
			{
				printf("PID 0x%08X PDPT Fucked\n");
				break;
			}
			auto type = MiGetSystemRegionType(pdpt);
			if (type != 3)
			{
				printf("PID 0x%08X [ Translation Level 1: %i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, type);
			}
			for (int j = 0; j < 512; j++)
			{
				if(cpy_pdpt[j].AsUINT64 != pdpt[j].AsUINT64)
				{
					printf("PID 0x%08X [ Translation Level 1: %i,%i ] -> pt tampering detected! %p vs %p\n", PsGetProcessId(eproc), i, j, cpy_pdpt[j].AsUINT64, pdpt[j].AsUINT64);
				}
				if (pdpt[j].Valid && !pdpt[j].LargePage)
				{
					auto cpy_pdt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
					MmCopyMemory(cpy_pdt, pdpt[j].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);

					auto pdt = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pdpt[j].PageFrameNumber << PAGE_SHIFT);
					if (!pdt)
					{
						printf("PID 0x%08X PDT Fucked\n");
						break;
					}
					auto type = MiGetSystemRegionType(pdt);
					if (type != 3)
					{
						printf("PID 0x%08X [ Translation Level 2: %i,%i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, j, type);
					}
					for (int k = 0; k < 512; k++)
					{
						if(cpy_pdt[k].AsUINT64 != pdt[k].AsUINT64)
						{
							printf("PID 0x%08X [ Translation Level 2: %i,%i,%i ] -> pt tampering detected! %p vs %p\n", PsGetProcessId(eproc), i, j, k, cpy_pdt[k].AsUINT64, pdt[k].AsUINT64);
						}
						if (pdt[k].Valid && !pdt[k].LargePage)
						{
							auto cpy_pte = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
							MmCopyMemory(cpy_pte, pdt[k].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);

							auto pte = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pdt[k].PageFrameNumber << PAGE_SHIFT);
							if (!pte)
							{
								printf("PID 0x%08X PTE Fucked\n");
								break;
							}
							auto type = MiGetSystemRegionType(pte);
							if (type != 3)
							{
								printf("PID 0x%08X [ Translation Level 3: %i,%i,%i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, j, k, type);
							}
							for(int m = 0; m < 512; m++)
							{
								if(cpy_pte[m].AsUINT64 != pte[m].AsUINT64)
								{
									printf("PID 0x%08X [ Translation Level 3: %i,%i,%i,%i ] -> pt tampering detected! %p vs %p\n", PsGetProcessId(eproc), i, j, k, m, cpy_pte[m].AsUINT64, pte[m].AsUINT64);
								}
								if (pte[m].Valid)
								{
									auto leaf_pfn = MmGetVirtualForPhysical(pte[m].PageFrameNumber << PAGE_SHIFT);
									auto type = MiGetSystemRegionType(leaf_pfn);

									if (type)
									{
										LINEAR_ADDRESS rva;
										rva.AsUINT64 = 0;
										rva.pml4e_index = i;
										rva.pdpte_index = j;
										rva.pde_index = k;
										rva.pte_index = m;
										
										switch (type)
										{
											case 4:
											{
												printf("Sessioned PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> translation pte usage on leaf { type:%i }\n",
													PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
											}break;
											case 11:
											{
												printf("Sessioned PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> driver page usage on leaf { type:%i }\n",
													PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
											}break;
											case 8:
											{
												printf("Sessioned PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> mmio usage on leaf { type:%i }\n",
													PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
											}break;
											default:
											{
												printf("Sessioned PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> other non-normal page type usage on leaf { type:%i }\n",
													PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
												break;
											}
										}
										
									}
								}
							}
							ExFreePool(cpy_pte);
						}
					}
					ExFreePool(cpy_pdt);
				}
			}
			ExFreePool(cpy_pdpt);
		}
	}
	ExFreePool(cpy_pml4);
	KeUnstackDetachProcess(&apc);
	return;
}

void LocateUsermodePteManipulationNtoskrnl(PEPROCESS eproc)
{
	SIZE_T bytes_copied = 0;
	KAPC_STATE apc;

	KeStackAttachProcess(PsInitialSystemProcess(), &apc);
	auto DTB = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;

	auto pml4 = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
	MmCopyMemory(pml4, DTB, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);

	if (!pml4)
	{
		printf("PID 0x%08X PML4 Fucked\n");
		return;
	}
	for (int i = 0; i < 255; i++)
	{
		if (pml4[i].Valid)
		{
			auto pdpt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
			MmCopyMemory(pdpt, pml4[i].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);
			if (!pdpt)
			{
				printf("PID 0x%08X PDPT Fucked\n");
				break;
			}
			auto type = MiGetSystemRegionType(pdpt);
			if (type != 4)
			{
				printf("PID 0x%08X [ Translation Level 1: %i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, type);
			}
			for (int j = 0; j < 512; j++)
			{
				if (pdpt[j].Valid && !pdpt[j].LargePage)
				{
					auto pdt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
					MmCopyMemory(pdt, pdpt[j].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);
					if (!pdt)
					{
						printf("PID 0x%08X PDT Fucked\n");
						break;
					}
					auto type = MiGetSystemRegionType(pdt);
					if (type != 4)
					{
						printf("PID 0x%08X [ Translation Level 2: %i,%i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, j, type);
					}
					for (int k = 0; k < 512; k++)
					{
						if (pdt[k].Valid && !pdt[k].LargePage)
						{
							auto pte = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
							MmCopyMemory(pte, pdt[k].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);
							if (!pte)
							{
								printf("PID 0x%08X PTE Fucked\n");
								break;
							}
							auto type = MiGetSystemRegionType(pte);
							if (type != 4)
							{
								printf("PID 0x%08X [ Translation Level 3: %i,%i,%i ] -> non-system pte { type:%i }\n", PsGetProcessId(eproc), i, j, k, type);
							}
							for (int m = 0; m < 512; m++)
							{
								if (pte[m].Valid)
								{
									auto leaf_pfn = MmGetVirtualForPhysical(pte[m].PageFrameNumber << PAGE_SHIFT);
									auto type = MiGetSystemRegionType(leaf_pfn);

									if (type)
									{
										LINEAR_ADDRESS rva;
										rva.AsUINT64 = 0;
										rva.pml4e_index = i;
										rva.pdpte_index = j;
										rva.pde_index = k;
										rva.pte_index = m;

										switch (type)
										{
										case 4:
										{
											printf("Ntoskrnl  PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> translation pte usage on leaf { type:%i }\n",
												PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
										}break;
										case 11:
										{
											printf("Ntoskrnl  PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> driver page usage on leaf { type:%i }\n",
												PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
										}break;
										case 8:
										{
											printf("Ntoskrnl  PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> mmio usage on leaf { type:%i }\n",
												PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
										}break;
										default:
										{
											printf("Ntoskrnl  PID 0x%08X [ RVA: %p | PFN: %010LLX ] -> other non-normal page type usage on leaf { type:%i }\n",
												PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
											break;
										}
										}

									}
								}
							}
							ExFreePool(pte);
						}
					}
					ExFreePool(pdt);
				}
			}
			ExFreePool(pdpt);
		}
	}
	ExFreePool(pml4);
	KeUnstackDetachProcess(&apc);
	return;
}

void ScanEProcess()
{
	printf("=== Scanning usermode for Pte Manipulation ===\n");
	printf("  - Only scans for 4kb mappings\n");
	printf("  - Only 4 level translation supported\n");
	printf("  - dosn't scan for PFN xref ctx-2-ctx\n");
	auto MHz = MSR::PSTATE(0).get_frequency_mhz();
	auto tsc_start = MSR::TSC();
	auto flink = (UINT64)PsInitialSystemProcess();
	auto o_UniqueProcessId = *(UINT32*)((UINT64)NtImports::fn_PsGetProcessId + 0x3);// fix for 22H2 & 24H2
	do
	{
		if (PsGetProcessId((PEPROCESS)flink) != 4)
		{
			auto PEB = *(QWORD*)(flink + o_UniqueProcessId + 0x110);
			if (PEB)
			{
				LocateUsermodePteManipulationSessioned((PEPROCESS)flink);
				LocateUsermodePteManipulationNtoskrnl((PEPROCESS)flink);
			}
		}
		flink = *(UINT64*)((UINT64)flink + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
	} while (flink != (UINT64)PsInitialSystemProcess());
	double mutiplier = (double)(MSR::TSC() - tsc_start) / (double)MHz;
	auto Ms = (UINT64)(mutiplier * 1000.0);
	printf("Usermode pte scan completed in %llu ms\n", Ms);
	return;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	ScanEProcess();
	return STATUS_SUCCESS;
}
