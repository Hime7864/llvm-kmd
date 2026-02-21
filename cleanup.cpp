#include "imports.hpp"

void SVM::Cleanup()
{
	if (vCpu)
		MmUnmapIoSpace(vCpu, vCoreCount * sizeof(VCORE));
	if (vaApicBase)
		MmUnmapIoSpace((PVOID)vaApicBase, 0x1000);
	FWA::Cleanup();
	return;
}
