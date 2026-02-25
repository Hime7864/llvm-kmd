#include <intrinsics.hpp>

struct CORE_TSC
{
	UINT64 tsc;
	UINT64 init_latency;
	UINT64 avg_latency;
	UINT64 min_latency;
	UINT64 pstate;
	UINT64 MHz;
	double mutipler;
};

void efer_ipi(CORE_TSC* tsc)
{
	auto aperf_init = MSR::APERF();
	auto mperf_init = MSR::MPERF();
	auto id = CPUID::current_core_number();
	tsc[id].tsc = __rdtsc();
	auto count = __rdtsc();
	auto efer = MSR::EFER();
	count = __rdtsc() - count;
	tsc[id].init_latency = count;
	tsc[id].min_latency = 0xFFFFFFFF;
	UINT64 avg = 0;
	for (int i = 0; i < 10000; i++)
	{
		count = __rdtsc();
		MSR::EFER();
		count = __rdtsc() - count;
		if(count < tsc[id].min_latency)
			tsc[id].min_latency = count;
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

void measure_efer()
{
	auto efer_tsc = ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(efer_ipi, efer_tsc);

	UINT64 delta = 0;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (((CORE_TSC*)efer_tsc)[i].pstate == 0 && ((CORE_TSC*)efer_tsc)[i].mutipler > 1.0)
		{
			printf("Core %d: %llu %llu %llu\n",
				i,
				((CORE_TSC*)efer_tsc)[i].init_latency,
				((CORE_TSC*)efer_tsc)[i].avg_latency,
				((CORE_TSC*)efer_tsc)[i].min_latency,
				((CORE_TSC*)efer_tsc)[i].pstate,
				((CORE_TSC*)efer_tsc)[i].MHz,
				((CORE_TSC*)efer_tsc)[i].mutipler
			);
			delta += ((CORE_TSC*)efer_tsc)[i].min_latency;
		}
	}

	ExFreePool(efer_tsc);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	measure_efer();

	return STATUS_SUCCESS;
}