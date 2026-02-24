#include <intrinsics.hpp>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto pstate = MSR::PSTATE(0);

	printf("CpuDfsId %i\n", pstate.CpuDfsId);
	printf("CpuFid %i\n", pstate.CpuFid);
	printf("CpuVid %i\n", pstate.CpuVid);
	printf("IddDiv %i\n", pstate.IddDiv);
	printf("IddValue %i\n", pstate.IddValue);
	printf("PstateEn %i\n", pstate.PstateEn);
	return STATUS_SUCCESS;
}