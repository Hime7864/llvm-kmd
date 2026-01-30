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

PVOID NAKED KeGetCurrentPrcb()
{
	__asm
	{
		mov rax, gs:[0x20]
		ret
	}
}

void IpiCallback()
{
	auto MmInternal = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];
	printf("%p\n", MmInternal);
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	auto MmInternal = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];


	auto v10 = 0x40000000ull;
	if ((DWORD)3 != 1)
		v10 = 0x200000ull;                           // always this
	auto v11 = (unsigned __int64)(unsigned int)v10 << 9;// 0x200000 << 9 = 0x10000000
	if ((DWORD)3)
		v11 = v10;                                // v11 = 1MiB
	UINT64 v12 = 1 << 12;
	auto true_2 = MmInternal & (v11 - 1);      // MmInternal & 0x1FFFFF = true
	auto int_3 = (1 << 12) + true_2;
	if (int_3 <= v11 && true_2 && (4 & 2) == 0)
	{
		printf("ret %p\n", MmInternal);
		printf("Condition met\n");
	}

	printf("%p\n", MmInternal & 0x1FFFFF);
	return STATUS_SUCCESS;
	//auto prbc = KeGetCurrentPrcb();
	auto kBase = Utils::GetKernelBase();
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	QWORD buffer = Utils::ReadPhysical<QWORD>(0x12340000);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);

	//auto pMmInternal = (PBYTE)&(*(UINT64**)((UINT64)prbc + 0x8838))[13];
	//auto MmInternal = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];
	//if (pMmInternal[0x19])
	//{
	//	MmInternal -= 0x1000ull;
	//	printf("MmInternal %p %i\n", MmInternal, Utils::RvaValid(MmInternal));
	//}
	//(*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	//printf("%p", (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13]);
	auto ptr = 0x323722 + kBase;
	auto ptr2 = 0x32372F + kBase;
	//auto MmInternal = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13] - 0x1000;
	auto anotherPteAddress = (((MmInternal - 1) >> 18) & 0x3FFFFFF8) + *(UINT64*)ptr;//ptr = 48 B8 ? ? ? ? ? ? ? ? 48 03 D8 49 BC
	//anotherPteAddress = ((anotherPteAddress >> 9) & 0x7FFFFFFFF8LL) + *(UINT64*)ptr2;
	auto PteAddress = anotherPteAddress - 8ull;
	auto v26 = (__int64)(PteAddress << 25) >> 16;
	printf("2v26: %p %p\n", *(QWORD*)PteAddress, MmGetPhysicalAddress((PVOID)PteAddress));
	printf("2v26: %p %p\n", *(QWORD*)anotherPteAddress, MmGetPhysicalAddress((PVOID)anotherPteAddress));
	
	
	

	

	

	return STATUS_SUCCESS;




	auto eproc = FindEproc(11464);
	if(!eproc)
		return STATUS_UNSUCCESSFUL;

	auto pfnDatabase = MmPfnDatabase();

	auto dtb = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;

	auto dtb_PteAddress = (PVOID)&pfnDatabase[dtb >> PAGE_SHIFT].PteAddress;
	auto dtb_PteAddress_pa = MmGetPhysicalAddress(dtb_PteAddress);
	auto dtb_PteAddress_offset = (UINT64)dtb_PteAddress & 0xFFF;

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


			printf("spoof1 %p\n", Utils::ReadPhysical<UINT64>(dtb + 0x7D0));
			printf("spoof2 %p\n", *(QWORD*)MmGetVirtualForPhysical(dtb + 0x7D0));

			break;
		}
	}

	//dTlbPoison.Cleanup();

	return STATUS_SUCCESS;
}