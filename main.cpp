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

bool NAKED is_zero_page(PVOID page)
{
	__asm
	{
		vpxor ymm0, ymm0, ymm0
		mov eax, 4096 / 32
		loop:
		vmovdqu ymm1, [rcx]
			vpxor ymm0, ymm0, ymm1
			add rcx, 32
			vptest ymm0, ymm0
			jnz nonzero

			dec eax
			jnz loop

			mov al, 1
			vzeroupper
			ret

			nonzero :
		xor eax, eax
			vzeroupper
			ret
	}
}

void issue()
{
	auto pfnDatabase = MmPfnDatabase();
	auto flink = (UINT64)PsInitialSystemProcess();
	auto o_UniqueProcessId = *(UINT32*)((UINT64)NtImports::fn_PsGetProcessId + 0x3);// fix for 22H2 & 24H2
	do
	{
		if (PsGetProcessId((PEPROCESS)flink) != 4)
		{
			auto PEB = *(QWORD*)(flink + o_UniqueProcessId + 0x110);
			if (PEB)
			{
				auto dtb = *(UINT64*)((UINT64)flink + 0x28) & ~0xFFF;
				//printf("dtb: %p\n", dtb);

				//printf("RAPE: %p\n", pfnDatabase[dtb >> PAGE_SHIFT].PteAddress);
				printf("RAPE pa: %p\n", MmGetPhysicalAddress(pfnDatabase[dtb >> PAGE_SHIFT].PteAddress));
			}
		}

		flink = *(UINT64*)((UINT64)flink + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
	} while (flink != (UINT64)PsInitialSystemProcess());
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto eproc = FindEproc(16444);
	if(!eproc)
		return STATUS_UNSUCCESSFUL;

	auto pfnDatabase = MmPfnDatabase();

	auto dtb = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;

	auto dtb_PteAddress = (PVOID)&pfnDatabase[dtb >> PAGE_SHIFT].PteAddress;
	auto dtb_PteAddress_pa = MmGetPhysicalAddress(dtb_PteAddress);
	auto dtb_PteAddress_offset = (UINT64)dtb_PteAddress & 0xFFF;

	;
	printf("spoof1 %p\n", Utils::ReadPhysical<UINT64>(dtb + 0x7D0));
	printf("spoof2 %p\n", *(QWORD*)MmGetVirtualForPhysical(dtb + 0x7D0));
	printf("%p\n", pfnDatabase[dtb >> PAGE_SHIFT].PteAddress);
	printf("dtb %p\n", dtb);
	printf("%p\n", MmGetPhysicalAddress(MmGetVirtualForPhysical(dtb)));

	return STATUS_UNSUCCESSFUL;
	dTlbPoison.Initialize();

	LINEAR_ADDRESS rva = MmGetVirtualForPhysical(dtb);

	for (int i = 256; i < 512; i++)
	{
		rva.pte_index = i;
		auto type = MiGetSystemRegionType(rva.AsPointer);
		if (type != 3)
			continue;
		
		auto pa = MmGetPhysicalAddress(rva.AsPointer);
		if (!pa)
			continue;
		if (is_zero_page(rva.AsPointer))
		{
			//create page_map
			auto free_page_pa = FWA::ReservePages(1);
			auto free_page_va = (MMPTE_HARDWARE*)MmMapIoSpace(free_page_pa, 0x1000, MmNonCached);
			if (free_page_va)
			{
				for (int i = 0; i < 512; i++)// 1gb pages
				{
					free_page_va[i].Valid = 1;
					free_page_va[i].Dirty1 = 1;
					free_page_va[i].Owner = 1;
					free_page_va[i].Write = 1;
					free_page_va[i].LargePage = 1;
					free_page_va[i].PageFrameNumber = i * 0x40000;
				}
				MmUnmapIoSpace(free_page_va, 0x1000);
			}
			else
				printf("fucked up\n");
			

			//copy usermode pml4 to shadow pml4
			dTlbPoison.ReserveRvaPoison(0xC0FFEE1000ull, pa);
			auto source = MmGetVirtualForPhysical(dtb);
			dTlbPoison.CommitRvaPoison(0xC0FFEE1000ull);
			RtlCopyMemory((PVOID)0xC0FFEE1000ull, source, 0x1000);


			//create new pte and insert into usermode pml4
			MMPTE_HARDWARE tmp_pte{ 0 };
			tmp_pte.Valid = true;
			tmp_pte.Dirty1 = true;
			tmp_pte.Owner = true;
			tmp_pte.Write = true;
			tmp_pte.PageFrameNumber = free_page_pa >> PAGE_SHIFT;

			dTlbPoison.ReserveRvaPoison(0xC0FFEE2000ull, dtb);
			dTlbPoison.CommitRvaPoison(0xC0FFEE2000ull);
			(*(UINT64*)0xC0FFEE27D0ULL) = tmp_pte.AsUINT64;
			printf("New Pt %p\n", (*(UINT64*)0xC0FFEE27D0ULL));

			//update pteaddress to fake page
			dTlbPoison.ReserveRvaPoison(0xC0FFEE3000ull, pa);
			dTlbPoison.CommitRvaPoison(0xC0FFEE3000ull);
			*(UINT64*)(0xC0FFEE3000ull + dtb_PteAddress_offset) = rva.AsUINT64;
			
			printf("Abuseable Page %p %010X\n", rva.AsUINT64, pa);
			printf("MMpfnEntry ptr %p\n", dtb_PteAddress);

			auto pte = Utils::ReadPhysical<UINT64>(dtb + 0x7D0);
			printf("MmCopyMemory pt entry %p\n", pte);

			break;
		}
	}

	//dTlbPoison.Cleanup();

	return STATUS_SUCCESS;
}