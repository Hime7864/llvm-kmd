#include "imports.hpp"

UINT64 SVM::HostedGetVirtual(PHYSICAL_ADDRESS address)
{
	auto va = MmMapIoSpace(address, 0x1000, MmNonCached);
	__invlpg(va);
	return (UINT64)va;
}

void SVM::HostedFreeVirtual(UINT64 address)
{
	return MmUnmapIoSpace((PVOID)address, 0x1000);
}

BOOLEAN SVM::HostedCommitMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte, PAGEMAP_SIZE pageSize)
{
	DWORD idx[4]{
		(DWORD)rva.pml4e_index,
		(DWORD)rva.pdpte_index,
		(DWORD)rva.pde_index,
		(DWORD)rva.pte_index
	};

	auto page_map = (MMPTE_HARDWARE*)HostedGetVirtual(dtb);

	for (int i = 0; i < 4; i++)
	{
		auto entry = &page_map[idx[i]];
		if (i == pageSize)
		{
			entry->AsUINT64 = pte.AsUINT64;
			HostedFreeVirtual((UINT64)page_map);
			break;
		}
		else if (!entry->Valid)
		{
			auto new_page = FWA::ReservePages(1);
			entry->AsUINT64 = 0;
			entry->Valid = true;
			if (pte.Owner)
				entry->Owner = true;
			if (pte.Dirty1)
				entry->Dirty1 = true;
			entry->PageFrameNumber = new_page >> 12;
			HostedFreeVirtual((UINT64)page_map);
			page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
		}
		else if (!entry->LargePage)
		{
			auto new_page = entry->PageFrameNumber << 12;
			HostedFreeVirtual((UINT64)page_map);
			page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
		}
		else
		{
			HostedFreeVirtual((UINT64)page_map);
			return FALSE;
		}
	}
	return TRUE;
}
