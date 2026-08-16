#include <intrinsics.hpp>

NAKED KPRCB* KeGetCurrentPrcb()
{
	__asm {
		mov rax, gs:20h
		ret
	}
}

void log_thing(UINT64 target_pa)
{

	auto base_rva = Utils::GetKernelBase();
	auto base_pa = MmGetPhysicalAddress((PVOID)base_rva);
	auto switch_pfn = base_pa >> 12;
	MMPTE_HARDWARE switch_pte;
	switch_pte.AsUINT64 = 0x8A000001A5D68121;
	switch_pte.PageFrameNumber = switch_pfn;

	auto pfn_database = MmPfnDatabase();
	pfn_database[target_pa >> 12].u3.e4.CacheAttribute = 2;

	auto prcb = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
	auto pStruct = prcb + 0x68;

	*(UINT64*)pStruct &= ~0x1FFFFF;

	auto funnypte = ((*(UINT64*)(pStruct) >> 9) & 0x7FFFFFFFF8ULL) + MmPteBase();
	*(UINT64*)funnypte = switch_pte.AsUINT64;
	printf("[DBG] funnypte: %p ------ PTE %p\n", funnypte, *(UINT64*)funnypte);


	SIZE_T bytesCopied = 0;
	UINT64 data = 0;
	MmCopyMemory(&data, target_pa, 8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	printf("[DBG] MmCopyMemory - [PA:%p] ->  %p\n", target_pa, data);

	auto a1 = pStruct;

	
	

	

	//auto target = *(UINT64*)pStruct;
	//printf("before: %p -> %p\n", pStruct, target);

	//auto v13 = target & 0x1FFFFF;
	//printf("v13: %p\n", v13);

	//*(UINT64*)a1 = *(UINT64*)a1 - 0x1000;
	
	//auto base = *(UINT64*)a1 & ~0x1FFFFF;
	//printf("change %p %p\n", *(UINT64*)a1, base);
	//*(UINT64*)a1 = base;
	//*(UINT64*)a1 -= 0x1000;

	//*(UINT64*)a1 &= ~0x1FFFFF;
	//UINT64 v4 = *(UINT64*)a1;
	////*(UINT64*)a1 += 0x200000ull;
	//UINT64 v13 = v4 & (0x200000ull - 1);
	//UINT64 v12 = 1 << 12;
	//UINT64 a2 = (1 << 12) + v13;

	//{
		//auto next = v12 + v4;
		//auto funnypte = ((next >> 9) & 0x7FFFFFFFF8ULL) + MmPteBase();
		////*(UINT64*)funnypte = switch_pte.AsUINT64;
		////*(UINT64*)a1 += 0x200000ull;
		//UINT64 data2 = 0;
		//SIZE_T bytesCopied2 = 0;
		//MmCopyMemory(&data2, target_pa, 8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied2);
		//printf("[DBG] MmCopyMemory - [PA:%p] ->  %p\n", target_pa, data2);
		//
		//printf("[DBG] funnypte: %p ------ PTE %p\n", funnypte, *(UINT64*)funnypte);
		//printf("[DBG] next: %p", next);
		//printf("[DBG] ret v4:%p a1:%p", v4, a1);



	//}
	//else
	//{
		//printf("[DBG] false");
	//}


	////printf("a1 %p->%p\n", *(UINT64*)a1, *(UINT64*)a1 & 0x1FFFFF);
	////printf("%p <= 0x200000 &&  %p != 0\n", (*(UINT64*)a1 & 0x1FFFFF) + 0x10000, (*(UINT64*)a1 & 0x1FFFFF));
	//if ((*(UINT64*)a1 & 0x1FFFFF) + 0x10000 <= 0x200000 && (*(UINT64*)a1 & 0x1FFFFF) != 0)
	//{
	//
	//
	//	target += (target_pa & 0xFFF);
	//	//printf("PhysicalAddress: %p -> is valid %i\n", (PVOID)target, Utils::RvaValid(target));
	//	printf("true ret -> %p : %p\n", a1, target);
	//	//UINT64 data = 0;
	//	//SIZE_T bytesCopied = 0;
	//	//MmCopyMemory(&data, pa, 8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	//	//printf("MmCopyMemory: [%p] -> %p\n", &data, data);
	//
	//	//auto test = *(UINT64*)((a1 + (UINT64)pa));
	//	//printf("test: %p -> %p\n", (*(UINT64*)a1 + (UINT64)pa), test);
	//
	//	//printf("return %p -> %p or %p;\n", (*(UINT64*)a1 + (UINT64)pa), MmGetPhysicalAddress((PVOID)((*(UINT64*)a1 + (UINT64)pa))), MmGetPhysicalAddress((PVOID)((a1 + (UINT64)pa))));
	//}
	//else
	//	printf("false\n");
	return;
}

NTSTATUS DriverEntry()
{
	auto buffer = ExAllocatePool(NonPagedPool, 0x1000);
	(*(UINT64*)buffer) = 0xDEADBEEF;
	auto pa = MmGetPhysicalAddress(buffer);
	printf("[DBG] Target PA - [%p]\n", pa);


	log_thing((UINT64)pa);
	//log_thing(pa2);

	//UINT64 data2 = 0;
	//SIZE_T bytesCopied2 = 0;
	//MmCopyMemory(&data2, pa2, 8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied2);
	//printf("[DBG] MmCopyMemory - [PA:%p] ->  %p\n", pa2, data2);
	
	//UINT64 data = 0;
	//SIZE_T bytesCopied = 0;
	//MmCopyMemory(&data, pa, 8, MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	//printf("[DBG] MmCopyMemory - [PA:%p] ->  %p\n", pa, data);
	//log_thing(pa);



	ExFreePool(buffer);
    return STATUS_SUCCESS;
}