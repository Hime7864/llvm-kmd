#include "imports.hpp"

void SVM::CreateMapping()
{
	hCr3 = FWA::ReservePages(1);

	coreCount = KeQueryActiveProcessorCount(0);
	auto vCpuSize = coreCount * sizeof(VCORE);
	auto vCpuPa = FWA::ReservePages(vCpuSize >> 12);
	vCpu = (VCORE*)MmMapIoSpace(vCpuPa, vCpuSize, MmNonCached);

	for (UINT64 current = (UINT64)vCpu; current < (UINT64)vCpu + vCpuSize; current += 0x1000)
	{
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.NoExecute = true;
		pte.Global = true;
		pte.PageFrameNumber = MmGetPhysicalAddress((PVOID)current) >> 12;
		HostedCommitMapping(hCr3, current, pte, PAGEMAP_4KB);
	}

	gCr3 = FWA::ReservePages(1);
	for (UINT64 current = 0x0; current < 0x800000000000ULL; current += 0x40000000ULL)
	{
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.Owner = true;
		pte.LargePage = true;
		pte.PageFrameNumber = current >> 12;
		HostedCommitMapping(gCr3, current, pte, PAGEMAP_1GB);
	}

	UINT64 driver_base = 0;
	UINT64 driver_size = 0;
	if (!NT_SUCCESS(Utils::LocateSelf(&driver_base, &driver_size)))
		return;

	for (UINT64 current = driver_base; current < driver_base + driver_size; current += 0x1000)
	{
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.Global = true;
		pte.PageFrameNumber = FWA::ReservePages(1) >> 12;
		HostedCommitMapping(hCr3, current, pte, PAGEMAP_4KB);
		auto rva = MmMapIoSpace(pte.PageFrameNumber << 12, 0x1000, MmNonCached);
		RtlCopyMemory(rva, (PVOID)current, 0x1000);
		MmUnmapIoSpace(rva, 0x1000);
	}

	//PHYSICAL_ADDRESS k_pml4 = __readcr3();
	//auto h_pml4 = (MMPTE_HARDWARE*)HostedGetVirtual(hCr3);
	//
	//for (int l4 = 256; l4 < 512; l4++)
	//{
	//	MMPTE_HARDWARE k_pml4e = Utils::ReadPhysical<MMPTE_HARDWARE>(k_pml4 + l4 * sizeof(MMPTE_HARDWARE));
	//	if (!k_pml4e.Valid)
	//		continue;
	//	MMPTE_HARDWARE h_pml4e = h_pml4[l4];
	//
	//	if (k_pml4e.AsUINT64 && !h_pml4e.AsUINT64)
	//	{
	//		h_pml4[l4] = k_pml4e;
	//		continue;
	//	}
	//	else
	//	{
	//		PHYSICAL_ADDRESS k_pml3 = k_pml4e.PageFrameNumber << 12;
	//		auto h_pml3 = (MMPTE_HARDWARE*)HostedGetVirtual(h_pml4e.PageFrameNumber << 12);
	//
	//		for (int l3 = 0; l3 < 512; l3++)
	//		{
	//			MMPTE_HARDWARE k_pml3e = Utils::ReadPhysical<MMPTE_HARDWARE>(k_pml3 + l3 * sizeof(MMPTE_HARDWARE));
	//			if (!k_pml3e.Valid)
	//				continue;
	//			MMPTE_HARDWARE h_pml3e = h_pml3[l3];
	//
	//			if (k_pml3e.AsUINT64 && !h_pml3e.AsUINT64)
	//			{
	//				h_pml3[l3] = k_pml3e;
	//				continue;
	//			}
	//			else
	//			{
	//				PHYSICAL_ADDRESS k_pml2 = k_pml3e.PageFrameNumber << 12;
	//				auto h_pml2 = (MMPTE_HARDWARE*)HostedGetVirtual(h_pml3e.PageFrameNumber << 12);
	//
	//				for (int l2 = 0; l2 < 512; l2++)
	//				{
	//					MMPTE_HARDWARE k_pml2e = Utils::ReadPhysical<MMPTE_HARDWARE>(k_pml2 + l2 * sizeof(MMPTE_HARDWARE));
	//					if (!k_pml2e.Valid)
	//						continue;
	//					MMPTE_HARDWARE h_pml2e = h_pml2[l3];
	//
	//					if (k_pml2e.AsUINT64 && !h_pml2e.AsUINT64)
	//					{
	//						h_pml2[l2] = k_pml2e;
	//						continue;
	//					}
	//					else
	//					{
	//						PHYSICAL_ADDRESS k_pml1 = k_pml2e.PageFrameNumber << 12;
	//						auto h_pml1 = (MMPTE_HARDWARE*)HostedGetVirtual(h_pml2e.PageFrameNumber << 12);
	//
	//						for (int l1 = 0; l1 < 512; l1++)
	//						{
	//							MMPTE_HARDWARE k_pml1e = Utils::ReadPhysical<MMPTE_HARDWARE>(k_pml1 + l1 * sizeof(MMPTE_HARDWARE));
	//							if (!k_pml1e.Valid)
	//								continue;
	//							MMPTE_HARDWARE h_pml1e = h_pml1[l1];
	//
	//							if (k_pml1e.AsUINT64 && !h_pml1e.AsUINT64)
	//							{
	//								h_pml1[l1] = k_pml1e;
	//								continue;
	//							}
	//						}
	//						HostedFreeVirtual((UINT64)h_pml1);
	//					}
	//				}
	//				HostedFreeVirtual((UINT64)h_pml2);
	//			}
	//		}
	//		HostedFreeVirtual((UINT64)h_pml3);
	//	}
	//}
	//HostedFreeVirtual((UINT64)h_pml4);
	return;
}