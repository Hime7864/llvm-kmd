#include "imports.hpp"

void SVM::Cleanup()
{
	if (vCpu)
		MmUnmapIoSpace(vCpu, KeQueryActiveProcessorCount(0) * sizeof(VCORE));

	FWA::Cleanup();
	return;
}
