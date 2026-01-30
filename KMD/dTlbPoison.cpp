#include "intrinsics.hpp"

UINT64 DTLB_POISON::HostedGetVirtual(PHYSICAL_ADDRESS address)
{
	auto va = MmMapIoSpace(address, 0x1000, MmNonCached);
	__invlpg(va);
	return (UINT64)va;
}

void DTLB_POISON::HostedFreeVirtual(UINT64 address)
{
	return MmUnmapIoSpace((PVOID)address, 0x1000);
}

MMPTE_HARDWARE DTLB_POISON::HostedValidateTranslation(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
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
		if (!entry->Valid)
		{
			HostedFreeVirtual((UINT64)page_map);
			return { 0 };
		}
		if (i == 3)
		{
			auto result = *entry;
			HostedFreeVirtual((UINT64)page_map);
			return result;
		}
		auto next_page = entry->PageFrameNumber << 12;
		HostedFreeVirtual((UINT64)page_map);
		page_map = (MMPTE_HARDWARE*)HostedGetVirtual(next_page);
	}
	return { 0 };
}

BOOLEAN DTLB_POISON::HostedCommit4kbMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte)
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
		if (i == 3)
		{
			entry->AsUINT64 = pte.AsUINT64;
			HostedFreeVirtual((UINT64)page_map);
			break;
		}
		if (!entry->Valid)
		{
			auto new_page = FWA::ReservePages(1);
			entry->AsUINT64 = 0;
			entry->Valid = true;
			entry->Dirty1 = true;
			entry->PageFrameNumber = new_page >> 12;
			HostedFreeVirtual((UINT64)page_map);
			page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
		}
		else
		{
			auto new_page = entry->PageFrameNumber << 12;
			HostedFreeVirtual((UINT64)page_map);
			page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
		}
	}
	return TRUE;
}

VOID NAKED DTLB_POISON::UpdatePoison(PVOID self)
{
	__asm {
		push rbx
		push rcx
		push rdx

		mov rcx, [rdx]
		mov rdx, [rdx + 8]

		mov rax, [rcx + 0x20]
		mov cr3, rax
		
		invlpg[rcx]
		invlpg[rdx]
		
		mov rbx, [rdx]
		xor eax, eax
		mov[rdx], rax
		mov[rdx], rbx
		
		mov rax, [rcx + 0x28]
		mov cr3, rax

		pop rdx
		pop rcx
		pop rbx
		ret
	}
}

BOOLEAN DTLB_POISON::Initialize()
{
	FWA::Initialize();

	UINT64 module_base, module_size;
	Utils::LocateSelf(&module_base, &module_size);

	mmCr3 = FWA::ReservePages(1);

	stack_frame.BaseAddress.QuadPart = FWA::ReservePages(10);
	stack_frame.NumberOfBytes.QuadPart = 0xA000;

	mmStackBase = module_base + module_size;
	oldCr3 = __readcr3();

	for (int i = 0; i < stack_frame.NumberOfBytes.QuadPart >> 12; i++)
	{
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.Dirty = true;
		pte.NoExecute = true;
		pte.PageFrameNumber = (stack_frame.BaseAddress.QuadPart >> 12) + i;
		HostedCommit4kbMapping(mmCr3, mmStackBase + (i << 12), pte);
	}

	for (int i = 0; i < module_size >> 12; i++)
	{
		auto new_page = FWA::ReservePages(1);
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.Dirty = true;
		pte.PageFrameNumber = new_page >> 12;
		HostedCommit4kbMapping(mmCr3, module_base + (i << 12), pte);
		auto page_va = HostedGetVirtual(new_page);
		RtlCopyMemory((PVOID)page_va, (PVOID)(module_base + (i << 12)), 4096);
		HostedFreeVirtual(page_va);
	}

	return TRUE;
}

void DTLB_POISON::ReserveRvaPoison(LINEAR_ADDRESS rva, PHYSICAL_ADDRESS PoisonedPage)
{
	MMPTE_HARDWARE pte;
	pte.AsUINT64 = 0;
	pte.Valid = true;
	pte.Owner = true;
	pte.Global = true;
	pte.Dirty1 = true;
	pte.Write = true;
	pte.PageFrameNumber = PoisonedPage >> 12;

	HostedCommit4kbMapping(mmCr3, rva, pte);
	return;
}

void DTLB_POISON::CommitRvaPoison(LINEAR_ADDRESS rva)
{
	struct IPI_DATA
	{
		PVOID self;
		UINT64 target_address;
	};
	IPI_DATA data;

	data.self = this;
	data.target_address = rva.AsUINT64;
	auto irql = __readcr8();
	__writecr8(15);
	UpdatePoison(&data);
	__writecr8(irql);
	return;
}

void DTLB_POISON::Cleanup()
{
	FWA::Cleanup();
	return;
}
