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

PVOID black_magic()
{
	auto alloc = ExAllocatePool(NonPagedPool, 0x1000);
	(*(UINT64*)alloc) = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];
	(*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13] = (UINT64)alloc + 0x8;
	RtlCopyMemory((PVOID)((UINT64)alloc + 0x8), "Get fucked lol", 0x10);
	return alloc;
}

void prevent_bsod(PVOID alloc)
{
	(*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13] = (*(UINT64*)alloc);
	ExFreePool(alloc);
	return;
}

UCHAR b2c(UCHAR val)
{
	if (val == 0x20)
		return val;
	if (val >= '0' && val <= '9')
		return val;
	if (val >= 'A' && val <= 'Z')
		return val;
	if (val >= 'a' && val <= 'z')
		return val;
	return '.';
}

void dmp_bytes(PVOID buffer, int bytes)
{
	for (int i = 0; i < bytes / 16; i++)
	{
		auto buf = (BYTE*)buffer + i * 16;
		printf("%04X : %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x : %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
			i * 16,
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
			buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15],
			b2c(buf[0]), b2c(buf[1]), b2c(buf[2]), b2c(buf[3]), b2c(buf[4]), b2c(buf[5]), b2c(buf[6]), b2c(buf[7]),
			b2c(buf[8]), b2c(buf[9]), b2c(buf[10]), b2c(buf[11]), b2c(buf[12]), b2c(buf[13]), b2c(buf[14]), b2c(buf[15])
		);
	}
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto eproc = FindEproc(100);
	if (eproc)
	{
		auto dtb = *(UINT64*)((UINT64)eproc + 0x28) & ~0xFFF;
		auto pfnDatabase = MmPfnDatabase();
		pfnDatabase[dtb >> PAGE_SHIFT].u2.LockBit = 1;
		pfnDatabase[dtb >> PAGE_SHIFT].u3.e1.CacheAttribute = 2;

	}

	auto kBase = Utils::GetKernelBase();
	auto alloc = ExAllocatePool(NonPagedPool, 0x1000);
	RtlCopyMemory(alloc, "My Little Cannary", 18);
	auto target_pa = MmGetPhysicalAddress(alloc);

	//(*(UINT64*)(kBase + 0xE35A00)) = target_pa >> PAGE_SHIFT;

	

	//E35A00
	auto pfnDatabase = MmPfnDatabase();
	//pfnDatabase[target_pa >> PAGE_SHIFT].u2.LockBit = 1;
	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute = 2;



	//auto a1 = &pfnDatabase[target_pa >> PAGE_SHIFT];
	//auto v16 = (UINT64)a1 + 0x30 * 1;

	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e4.EntireField = 0xFFFFFFFF;

	//printf("PageSizeIndex %i\n", pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute);
	
	//pfnDatabase[target_pa >> PAGE_SHIFT].u2.LockBit = 1;

	//pfnDatabase[target_pa >> PAGE_SHIFT].u4.PfnExists = 0;

	//auto flag = (*(QWORD*)(a1 + 0x28) & 0x10000000000LL) == 0;
	//pfnDatabase[target_pa >> PAGE_SHIFT].u4.EntireField = 0;
	//auto result2 = (*(DWORD*)(a1 + 0x24) >> 27);// 3 - ((*(DWORD*)(a1 + 0x24) >> 27) & 3u);
	//pfnDatabase[target_pa >> PAGE_SHIFT].u5.Active.AnchorLargePageSize = 0;

	//pfnDatabase[target_pa >> PAGE_SHIFT].u5.Active.PageTableLinked
	//printf("result2 %i\n", result2);
	//printf("test %08X\n", pfnDatabase[target_pa >> PAGE_SHIFT].u5.Active.AnchorLargePageSize);

	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.ReadInProgress = 0;
	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute = 2;
	
	SIZE_T bytesCopied = 0;
	//*(DWORD*)((UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8EB8) + 0x14) = 0xFFFF;
	//auto flag = *(DWORD*)((UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8EB8) + 0x14);
	//printf("Current CPU flags: %X\n", flag);



	BYTE buffer[0x21]{0};
	auto stat = MmCopyMemory(&buffer, target_pa, 0x20, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	printf("  Normal > MmCopyMemory(%08X) PFN[%X] -> %s", stat, target_pa >> PAGE_SHIFT, buffer);
	printf("counter %i\n", *(int*)(kBase + 0xE2DA54));
	//flag = *(DWORD*)((UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8EB8) + 0x14);
	//printf("Current CPU flags: %X\n", flag);

	//printf("CacheAttribute %i\n", pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute);

	auto status = black_magic();
	MmCopyMemory(&buffer, target_pa, 0x20, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	printf(" Spoofed > MmCopyMemory PFN[%X] -> %s", target_pa >> PAGE_SHIFT, buffer);
	prevent_bsod(status);
	
	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute = 2;
	//MmCopyMemory(alloc, target_pa, 0x1000, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	//pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute = 0;
	//printf("Force Page Miss\n");
	//dmp_bytes(alloc, 0x1000);
	//printf("  Normal > MmCopyMemory PFN[%X] -> %s", target_pa >> PAGE_SHIFT, buffer);
	
	//printf("CacheAttribute %i\n", pfnDatabase[target_pa >> PAGE_SHIFT].u3.e1.CacheAttribute);
	//auto alloc = ExAllocatePool(NonPagedPool, 0xA000);
	//RtlFillMemory(alloc, 0x1000, 0x67);
	//auto alloc_va = MmMapIoSpace(MmGetPhysicalAddress(alloc), 0xA000, MmNonCached);
	//
	//auto old = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];
	//auto bits = (PBYTE) & (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];
	//bits[25] = 0;
	//(*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13] = (UINT64)alloc_va;
	//
	//buffer = 0;
	//MmCopyMemory(&buffer, target_pa, 0x8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	//printf("  Spoofed > MmCopyMemory PFN[%X] -> %p", target_pa >> PAGE_SHIFT, buffer);
	//
	//(*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13] = old;
	//MmUnmapIoSpace(alloc_va, 0xA000);
	//ExFreePool(alloc);
	//
	//MmCopyMemory(&buffer, target_pa, 0x8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	//printf("  Real > MmCopyMemory PFN[%X] -> %p", target_pa >> PAGE_SHIFT, buffer);


	//auto v10 = 0x40000000ull;
	//if ((DWORD)3 != 1)
	//	v10 = 0x200000ull;                           // always this
	//auto v11 = (unsigned __int64)(unsigned int)v10 << 9;// 0x200000 << 9 = 0x10000000
	//if ((DWORD)3)
	//	v11 = v10;                                // v11 = 1MiB
	//UINT64 v12 = 1 << 12;
	//auto true_2 = *MmInternal & (v11 - 1);      // MmInternal & 0x1FFFFF = true
	//auto int_3 = (1 << 12) + true_2;
	//if (int_3 <= v11 && true_2 && (4 & 2) == 0)
	//{
	//	printf("[+] Condition met\n");
	//}
	//else
	//{
	//	printf("[!] Condition not met\n");
	//}

	return STATUS_SUCCESS;

	//auto MmInternal = (*(UINT64**)((UINT64)KeGetCurrentPrcb() + 0x8838))[13];

	/*
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
	
	
	

	
	*/
	

	return STATUS_SUCCESS;




	auto eproc = FindEproc(11464);
	if(!eproc)
		return STATUS_UNSUCCESSFUL;

	//auto pfnDatabase = MmPfnDatabase();

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