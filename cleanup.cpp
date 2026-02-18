#include "imports.hpp"

void SVM::Cleanup()
{
	if (vCpu)
		MmUnmapIoSpace(vCpu, KeQueryActiveProcessorCount(0) * sizeof(VCORE));
	if (vaApicBase)
		MmUnmapIoSpace((PVOID)vaApicBase, 0x1000);
	if (idtBase)
		MmUnmapIoSpace((PVOID)idtBase, 0x1000);
	FWA::Cleanup();
	return;
}
