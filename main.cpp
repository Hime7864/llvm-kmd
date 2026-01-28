#include <intrinsics.hpp>


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

void LocateUsermodePteManipulation(PEPROCESS eproc)
{
	KAPC_STATE apc;
	KeStackAttachProcess(eproc, &apc);
	auto DTB = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;

	auto pml4 = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(DTB);
	for (int i = 0; i < 255; i++)
	{
		if (pml4[i].Valid)
		{
			auto pdpt = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pml4[i].PageFrameNumber << PAGE_SHIFT);
			for (int j = 0; j < 512; j++)
			{
				if (pdpt[j].Valid && !pdpt[j].LargePage)
				{
					auto pdt = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pdpt[j].PageFrameNumber << PAGE_SHIFT);
					for (int k = 0; k < 512; k++)
					{
						if (pdt[k].Valid && !pdt[k].LargePage)
						{
							auto cpy_pte = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
							SIZE_T bytes_copied = 0;
							MmCopyMemory(cpy_pte, pdt[k].PageFrameNumber << PAGE_SHIFT, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied);
							auto pte = (MMPTE_HARDWARE*)MmGetVirtualForPhysical(pdt[k].PageFrameNumber << PAGE_SHIFT);
							
							for(int m = 0; m < 512; m++)
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
											printf("PID 0x%08X RVA %p:%010LLX | leaf pte points to system pte\n", PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber);
										}break;
										case 11:
										{
											printf("PID 0x%08X RVA %p:%010LLX | leaf pte points to copy-on-write(or fixup) pte\n", PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber);
										}break;
										case 8:
										{
											printf("PID 0x%08X RVA %p:%010LLX | leaf pte points to mmio pte\n", PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber);
										}break;
										default:
										{
											printf("PID 0x%08X RVA %p:%010LLX | some other non-normal pte(%i)\n", PsGetProcessId(eproc), rva.AsUINT64, pte[m].PageFrameNumber, type);
											break;
										}
										}
										
									}
								}
								if (cpy_pte[m].AsUINT64 != pte[m].AsUINT64)
								{
									LINEAR_ADDRESS rva;
									rva.AsUINT64 = 0;
									rva.pml4e_index = i;
									rva.pdpte_index = j;
									rva.pde_index = k;
									rva.pte_index = m;
									printf("PID 0x%08X RVA %p:%010LLX | mmpfndatabase maniulation\n", PsGetProcessId(eproc), rva.AsUINT64, cpy_pte[m].PageFrameNumber);
								}
							}
							ExFreePool(cpy_pte);
						}
					}
				}
			}
		}
	}

	KeUnstackDetachProcess(&apc);
	return;
}

void ScanEProcess()
{
	auto flink = (UINT64)PsInitialSystemProcess();
	auto o_UniqueProcessId = *(UINT32*)((UINT64)NtImports::fn_PsGetProcessId + 0x3);// fix for 22H2 & 24H2
	do
	{
		if (PsGetProcessId((PEPROCESS)flink) != 4)
		{
			auto PEB = *(QWORD*)(flink + o_UniqueProcessId + 0x110);
			if (PEB)
			{
				LocateUsermodePteManipulation((PEPROCESS)flink);
			}
		}
		flink = *(UINT64*)((UINT64)flink + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
	} while (flink != (UINT64)PsInitialSystemProcess());
	return;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	ScanEProcess();
	return STATUS_SUCCESS;
}
