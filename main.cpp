#include <intrinsics.hpp>

struct CORE_TSC
{
	UINT64 tsc;
	UINT64 init_latency;
	UINT64 avg_latency;
	UINT64 pstate;
	UINT64 MHz;
	double mutipler;
};

void ipi(CORE_TSC* tsc)
{
	auto aperf_init = MSR::APERF();
	auto mperf_init = MSR::MPERF();
	auto id = CPUID::current_core_number();
	tsc[id].tsc = __rdtsc();
	auto count = __rdtsc();
	auto efer = MSR::EFER();
	count = __rdtsc() - count;
	tsc[id].init_latency = count;

	UINT64 avg = 0;
	for (int i = 0; i < 10000; i++)
	{
		count = __rdtsc();
		MSR::EFER();
		count = __rdtsc() - count;
		avg += count;
	}
	avg /= 10000;
	auto perf = MSR::APERF() - aperf_init;
	auto mperf = MSR::MPERF() - mperf_init;

	tsc[id].mutipler = (double)perf / (double)mperf;
	tsc[id].avg_latency = avg;
	tsc[id].pstate = MSR::PSTATE_STATUS().CurPstate;
	tsc[id].MHz = MSR::PSTATE(tsc[id].pstate).get_frequency_mhz();
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	return STATUS_SUCCESS;
}