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

QWORD pages = 0;

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


//0x4 bytes (sizeof)
enum _MI_ASSIGNED_REGION_TYPES
{
	AssignedRegionPfnDatabase = 0,
	AssignedRegionNonPagedPool = 1,
	AssignedRegionPagedPool = 2,
	AssignedRegionSystemCache = 3,
	AssignedRegionSystemPtes = 4,
	AssignedRegionKasan = 5,
	AssignedRegionUltraZero = 6,
	AssignedRegionCfg = 7,
	AssignedRegionHyperSpace = 8,
	AssignedRegionKernelStacks = 9,
	AssignedRegionNonCachedMappings = 10,
	AssignedRegionSoftWsles = 11,
	AssignedRegionPageTables = 12,
	AssignedRegionNotUsed = 13,
	AssignedRegionSecureNonPagedPool = 14,
	AssignedRegionKernelShadowStacks = 15,
	AssignedRegionSystemDataViews = 16,
	AssignedRegionSystemImages = 17,
	AssignedRegionMaximum = 18
};

const char* GetRegionName(UINT32 type)
{
	switch (type)
	{
	case AssignedRegionPfnDatabase:          return "PfnDatabase";
	case AssignedRegionNonPagedPool:         return "NonPagedPool";
	case AssignedRegionPagedPool:            return "PagedPool";
	case AssignedRegionSystemCache:          return "SystemCache";
	case AssignedRegionSystemPtes:           return "SystemPtes";
	case AssignedRegionKasan:                return "Kasan";
	case AssignedRegionUltraZero:            return "UltraZero";
	case AssignedRegionCfg:                  return "Cfg";
	case AssignedRegionHyperSpace:           return "HyperSpace";
	case AssignedRegionKernelStacks:         return "KernelStacks";
	case AssignedRegionNonCachedMappings:    return "NonCachedMappings";
	case AssignedRegionSoftWsles:            return "SoftWsles";
	case AssignedRegionPageTables:           return "PageTables";
	case AssignedRegionNotUsed:              return "NotUsed";
	case AssignedRegionSecureNonPagedPool:   return "SecureNonPagedPool";
	case AssignedRegionKernelShadowStacks:   return "KernelShadowStacks";
	case AssignedRegionSystemDataViews:      return "SystemDataViews";
	case AssignedRegionSystemImages:         return "SystemImages";
	case AssignedRegionMaximum:              return "Maximum";

	default:                                 return "Unknown";
	}
}


void ValidatePageMap(PHYSICAL_ADDRESS dtb, PHYSICAL_ADDRESS page_map)
{
	auto PfnDatabase = MmPfnDatabase();

	auto pPteVa = &PfnDatabase[page_map >> PAGE_SHIFT].PteAddress;
	printf("pPteVa %p\n", pPteVa);

	auto pPteSession = Utils::ReadLinear<LINEAR_ADDRESS>(dtb, pPteVa);
	auto pPteKernel = Utils::ReadLinear<LINEAR_ADDRESS>(pPteVa);

	if(pPteSession.AsUINT64 != pPteKernel.AsUINT64)
		printf("Session vs Kernel PfnDatabase Linear Mismatch\n");

	auto pfnSession = Utils::LinearTranslatePte(dtb, pPteSession).PageFrameNumber;
	auto pfnKernel = Utils::LinearTranslatePte(pPteKernel).PageFrameNumber;

	

	auto pPteSession2 = &PfnDatabase[pfnSession].PteAddress;
	auto pPteKernel2 = &PfnDatabase[pfnKernel].PteAddress;

	printf("PFN Session PTE Addr: %p\n", pPteSession2);
	printf("PFN Kernel  PTE Addr: %p\n", pPteKernel2);

	//MiGetSystemRegionType(pdpt);
}

void ValidateUsermodePageMapping(PEPROCESS eproc)
{
	SIZE_T bytes_copied = 0;
	KAPC_STATE apc;
	auto PfnDatabase = MmPfnDatabase();
	
	auto pml4 = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
	auto pml4_pa = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;
	if(!NT_SUCCESS(MmCopyMemory(pml4, pml4_pa, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied)))
	{
		printf("Failed to read PML4\n");
		return;
	}

	for (int l4 = 0; l4 < 256; l4++)
	{
		if (!pml4[l4].Valid)
			continue;

		auto pdpt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
		auto pdpt_pa = pml4[l4].PageFrameNumber << PAGE_SHIFT;
		if (!NT_SUCCESS(MmCopyMemory(pdpt, pdpt_pa, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied)))
		{
			printf("Failed to read PDPT\n");
			return;
		}

		for (int l3 = 0; l3 < 512; l3++)
		{
			if (!pdpt[l3].Valid)
				continue;

			auto pdt = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
			auto pdt_pa = pdpt[l3].PageFrameNumber << PAGE_SHIFT;
			if (!NT_SUCCESS(MmCopyMemory(pdt, pdt_pa, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied)))
			{
				printf("Failed to read PDT\n");
				return;
			}

			for (int l2 = 0; l2 < 512; l2++)
			{
				if (!pdt[l2].Valid)
					continue;

				auto pte = (MMPTE_HARDWARE*)ExAllocatePool(NonPagedPool, 0x1000);
				auto pte_pa = pdt[l2].PageFrameNumber << PAGE_SHIFT;
				if (!NT_SUCCESS(MmCopyMemory(pte, pte_pa, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytes_copied)))
				{
					printf("Failed to read PTE\n");
					return;
				}

				for (int l1 = 0; l1 < 512; l1++)
				{
					if (!pte[l1].Valid)
						continue;

					auto pPteVa = PfnDatabase[pte[l1].PageFrameNumber].PteAddress;

					auto pa = Utils::LinearTranslate(pml4_pa, pPteVa);

					auto pfnEntry = &PfnDatabase[pa >> PAGE_SHIFT].PteAddress;

					auto PtSession = Utils::ReadLinear<MMPTE_HARDWARE>(pml4_pa, pPteVa);
					auto PtKernel = Utils::ReadLinear<MMPTE_HARDWARE>(pPteVa);

					if (PtSession.AsUINT64 != PtKernel.AsUINT64 && PtKernel.AsUINT64)
					{
						printf("Session PTE Addr: %p\n", PtSession.AsUINT64);
						printf("Kernel  PTE Addr: %p\n", PtKernel.AsUINT64);
					}
					else if (PtSession.AsUINT64 == PtKernel.AsUINT64)
					{
						LINEAR_ADDRESS rva;
						rva.AsUINT64 = 0;
						rva.pml4e_index = l4;
						rva.pdpte_index = l3;
						rva.pde_index = l2;
						rva.pte_index = l1;
						//printf("Matching PTEs at RVA: %p | PTE: %p\n", rva.AsUINT64, PtKernel.AsUINT64);
						auto type = MiGetSystemRegionType(PfnDatabase[PtKernel.PageFrameNumber].PteAddress);
						if (type != AssignedRegionKasan && type != AssignedRegionPfnDatabase)
						{
							printf("K&S PID 0x%08X [ RVA: %p | PFN: %010LLX ] type { %s }\n",
								PsGetProcessId(eproc), rva.AsUINT64, PtKernel.PageFrameNumber, GetRegionName(type));
						}
						//printf("Type: %s\n", GetRegionName(type));
					}
					else
					{
						LINEAR_ADDRESS rva;
						rva.AsUINT64 = 0;
						rva.pml4e_index = l4;
						rva.pdpte_index = l3;
						rva.pde_index = l2;
						rva.pte_index = l1;

						auto type = MiGetSystemRegionType(PfnDatabase[PtSession.PageFrameNumber].PteAddress);
						if (type != AssignedRegionSystemCache)
						{
							printf("S   PID 0x%08X [ RVA: %p | PFN: %010LLX ] type { %s }\n",
								PsGetProcessId(eproc), rva.AsUINT64, PtSession.PageFrameNumber, GetRegionName(type));
						}
					}

					//printf("Session PTE Addr: %p\n", pPteSession);


					//auto pPteKernel = PfnDatabase[pa >> PAGE_SHIFT].PteAddress;

					
					

				}
				ExFreePool(pte);
			}
			ExFreePool(pdt);
		}
		ExFreePool(pdpt);
	}
	ExFreePool(pml4);
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
				printf("Scanning PID 0x%08X\n", PsGetProcessId((PEPROCESS)flink));
				ValidateUsermodePageMapping((PEPROCESS)flink);

				//LocateUsermodePteManipulationSessioned((PEPROCESS)flink);
				//LocateUsermodePteManipulationNtoskrnl((PEPROCESS)flink);
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
