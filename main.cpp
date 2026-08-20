#include <intrinsics.hpp>

NAKED KPRCB* KeGetCurrentPrcb()
{
	__asm {
		mov rax, gs:20h
		ret
	}
}

UINT64 GetUltraMapping()
{
	auto pte_base = MmPteBase();
	auto MmInternal = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
	auto pte_ptr = MmInternal + 0x68;
	return pte_base + ((*(UINT64*)pte_ptr >> 9) & 0x7FFFFFFFF8ULL);
}

UINT64 ReadPhysicalMemory(UINT64 physicalAddress)
{
	UINT64 data = 0;
	SIZE_T bytesCopied = 0;
	MmCopyMemory(&data, physicalAddress, sizeof(data), MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
	return data;
}

void log_thing(UINT64 target_pa)
{

	//ReadPhysicalMemory(target_pa);

	auto MmInternal = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
	*(UINT64*)MmInternal = 0xDEADBEEF;
	//printf("[KMD] thing %p\n", *(UINT64*)MmInternal);
	//auto ultra_mapping = GetUltraMapping();
	//printf("[KMD] UltraMapping %p\n", ultra_mapping);
	//
	//ReadPhysicalMemory(target_pa);
	//
	//printf("[KMD] thing %p\n", *(UINT64*)MmInternal);
	//ultra_mapping = GetUltraMapping();
	//printf("[KMD] UltraMapping %p\n", ultra_mapping);
	

	
	printf("[KMD] MmInternal [%p] %p -> %p\n", ((UINT64)KeGetCurrentPrcb() + 0x8838), *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838), GetUltraMapping());
	auto ret = ReadPhysicalMemory(target_pa);
	printf("[KMD] ReadPhysicalMemory [%p]->%p\n", target_pa, ret);
	printf("[KMD] MmInternal [%p] %p -> %p\n", ((UINT64)KeGetCurrentPrcb() + 0x8838), *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838), GetUltraMapping());
	return;
	//auto kernel_base_rva = Utils::GetKernelBase();
	//printf("[KMD] GetKernelBase va-%p\n", kernel_base_rva);
	//auto kernel_base_pa = MmGetPhysicalAddress((PVOID)kernel_base_rva);
	//printf("[KMD] GetKernelBase pa-%p\n", kernel_base_pa);
	//auto pte_database = MmPfnDatabase();
	//auto pte_base = MmPteBase();
	//
	//MMPTE_HARDWARE switch_pte;
	//switch_pte.AsUINT64 = 0x8A000001A5D68121;
	//switch_pte.PageFrameNumber = kernel_base_pa >> 12;
	//
	//
	//auto MmInternal = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
	//printf("[KMD] MmInternal %p\n", MmInternal);
	//auto pUltraMapping = MmInternal + 0x68;
	//printf("[KMD] pUltraMapping %p->%p\n", pUltraMapping, pUltraMapping);
	//auto UltraMapping = pte_base + (((*(UINT64*)pUltraMapping) >> 9) & 0x7FFFFFFFF8ULL);
	//printf("[KMD] UltraMapping %p\n", UltraMapping);
	//
	//
	////ReferenceCount
	//
	//printf("ReferenceCount %i\n", pte_database[target_pa >> 12].u3.ReferenceCount);
	//
	//auto old_ReferenceCount = pte_database[target_pa >> 12].u3.ReferenceCount;
	//pte_database[target_pa >> 12].u3.ReferenceCount = 3;
	//auto original_pte = pte_database[target_pa >> 12].OriginalPte.u.Hard.AsUINT64;
	//pte_database[target_pa >> 12].OriginalPte.u.Hard.AsUINT64 = switch_pte.AsUINT64;
	//printf("[KMD] OriginalPte %p\n", original_pte);
	//printf("[KMD] new pte %p\n", pte_database[target_pa >> 12].OriginalPte.u.Hard.AsUINT64);
	//pte_database[target_pa >> 12].u3.e4.CacheAttribute = 3;
	//printf("[KMD] ReadPhysicalMemory [%p]->%p\n", target_pa, ReadPhysicalMemory(target_pa));
	//
	//printf("[KMD] after pte %p\n", pte_database[target_pa >> 12].OriginalPte.u.Hard.AsUINT64);
	//pte_database[target_pa >> 12].u3.e4.CacheAttribute = 0;
	//pte_database[target_pa >> 12].u3.ReferenceCount = old_ReferenceCount;
	//pte_database[target_pa >> 12].OriginalPte.u.Hard.AsUINT64 = original_pte;
	//return;
}

NTSTATUS DriverEntry()
{
	auto buffer = ExAllocatePool(NonPagedPool, 0x1000);
	(*(UINT64*)buffer) = 0xDEADBEEF;
	auto pa = MmGetPhysicalAddress(buffer);
	printf("[KMD] Requested PFN - [%p]\n", pa >> 12);


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