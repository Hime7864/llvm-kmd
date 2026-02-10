#pragma once

enum PAGEMAP_SIZE
{
	PAGEMAP_1GB = 1,
	PAGEMAP_2KB = 2,
	PAGEMAP_4KB = 3,
};

class SVM
{
private:
	static UINT64 dCr3;
	static UINT64 hCr3;
	static UINT64 gCr3;
	static VCORE* vCpu;

	static UINT64 HostedGetVirtual(PHYSICAL_ADDRESS address);

	static void HostedFreeVirtual(UINT64 address);

	static BOOLEAN HostedCommitMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte, PAGEMAP_SIZE pageSize);

	static void CreateMapping();

	static void ControlArea();

	static void NAKED VmLoop(VCORE* vCore, PHYSICAL_ADDRESS vmcb);

	static void __attribute__((preserve_most)) VmExit(VCORE* vCore);

	static void NAKED SaveCtx(VCORE* vCore);

	static void NAKED LoadCtx(VCORE* vCore);

	static void LaunchCore(int affinity);

public:
	static void Initialize();

	static void LaunchVm();

	static void Cleanup();
};