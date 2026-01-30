#pragma once

class PACKED DTLB_POISON
{
private:
	UINT64 mmStackBase;//0x0
	UINT64 oldStackBase;//0x8
	PHYSICAL_MEMORY_RANGE stack_frame;//0x10
	PHYSICAL_ADDRESS mmCr3;//0x20
	PHYSICAL_ADDRESS oldCr3;//0x28

	UINT64 HostedGetVirtual(PHYSICAL_ADDRESS address);
	void HostedFreeVirtual(UINT64 address);
	MMPTE_HARDWARE HostedValidateTranslation(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva);
	BOOLEAN HostedCommit4kbMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte);
	UINT64 NAKED UpdatePoison(PVOID self);

public:
	BOOLEAN Initialize();

	void ReserveRvaPoison(LINEAR_ADDRESS rva, PHYSICAL_ADDRESS PoisonedPage);

	void CommitRvaPoison(LINEAR_ADDRESS rva);

	void Cleanup();
};