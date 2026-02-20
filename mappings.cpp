#include "imports.hpp"

void SVM::CreateMapping()
{
	hCr3 = FWA::ReservePages(1);

	vCoreCount = KeQueryActiveProcessorCount(0);
	auto vCpuSize = vCoreCount * sizeof(VCORE);
	auto vCpuPa = FWA::ReservePages(vCpuSize >> 12);
	vCpu = (VCORE*)MmMapIoSpace(vCpuPa, vCpuSize, MmNonCached); 

	if (!idtBase)
	{
		auto idtBasePa = FWA::ReservePages(1);
		idtBase = (UINT64)MmMapIoSpace(idtBasePa, 0x1000, MmNonCached);
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.NoExecute = true;
		pte.Global = true;
		pte.PageFrameNumber = idtBasePa >> 12;
		HostedCommitMapping(hCr3, idtBase, pte, PAGEMAP_4KB);
	}

	if (!MSR::APIC_BASE().extd)
	{
		auto apicBasePa = MSR::APIC_BASE().aba;
		vaApicBase = (xAPIC_REGISTERS*)MmMapIoSpace(apicBasePa << 12, 0x1000, MmNonCached);
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.CacheDisable = true;
		pte.NoExecute = true;
		pte.Global = true;
		pte.PageFrameNumber = apicBasePa;
		HostedCommitMapping(hCr3, vaApicBase, pte, PAGEMAP_4KB);
	}
	else
		vaApicBase = 0x0;

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
	return;
}