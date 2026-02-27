#include <intrinsics.hpp>

enum TIMER_MODE
{
	tmNone,
	tmRDTSC,
	tmRDTSCP,
	tmTSC,
	tmAPERF,
	tmMPERF
};

struct LOG_TIME
{
	WORD Mode;
	WORD Rate;
	DWORD TSC;
};

class HIGH_PERCISION_TIMER
{
private:
	INT64 aperf_init;
	INT64 tsc_init;

	INT64 sync_time;
	INT64 start_time;
	double ratio;
	UINT32 MHz;
	INT32 pstate;
	TIMER_MODE mode;
public:
	template<TIMER_MODE Mode>
	void Start();

	template<TIMER_MODE Mode>
	void Stop();

	LOG_TIME Log(bool dbg = false)
	{
		LOG_TIME log_time;
		log_time.Mode = (WORD)mode;
		log_time.Rate = (WORD)((double)MHz * ratio);
		log_time.TSC = (DWORD)start_time;
		if (dbg)
		{
			switch (mode)
			{
			case tmRDTSC:
				printf("RDTSC [%lli] @ %iMHz\n", start_time, (INT32)((double)MHz * ratio));
				break;
			case tmRDTSCP:
				printf("RDTSCP [%lli] @ %iMHz\n", start_time, (INT32)((double)MHz * ratio));
				break;
			case tmAPERF:
				printf("APERF [%lli] @ %iMHz\n", start_time, (INT32)((double)MHz * ratio));
				break;
			case tmMPERF:
				printf("MPERF [%lli] @ %iMHz\n", start_time, (INT32)((double)MHz * ratio));
				break;
			default:
				break;
			}
		}
		return log_time;
	}
};

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Start<tmRDTSC>()
{
	mode = tmNone;
	aperf_init = MSR::APERF();
	_mm_pause();
	aperf_init = MSR::APERF() - aperf_init;
	tsc_init = MSR::TSC();
	_mm_pause();
	tsc_init = MSR::TSC() - tsc_init;
	sync_time = __rdtsc();
	_mm_pause();
	sync_time = __rdtsc() - sync_time;
	_mm_pause();
	start_time = __rdtsc() + sync_time;
	_mm_pause();
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Stop<tmRDTSC>()
{
	start_time = __rdtsc() - start_time;
	ratio = (double)aperf_init / (double)tsc_init;
	pstate = MSR::PSTATE_STATUS().CurPstate;
	MHz = (UINT32)MSR::PSTATE(0).get_frequency_mhz();
	mode = tmRDTSC;
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Start<tmRDTSCP>()
{
	mode = tmNone;
	aperf_init = MSR::APERF();
	_mm_pause();
	aperf_init = MSR::APERF() - aperf_init;
	tsc_init = MSR::TSC();
	_mm_pause();
	tsc_init = MSR::TSC() - tsc_init;
	DWORD aux;
	sync_time = __rdtscp(&aux);
	_mm_pause();
	sync_time = __rdtscp(&aux) - sync_time;
	_mm_pause();
	start_time = __rdtscp(&aux) + sync_time;
	_mm_pause();
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Stop<tmRDTSCP>()
{
	DWORD aux;
	start_time = __rdtscp(&aux) - start_time;
	ratio = (double)aperf_init / (double)tsc_init;
	pstate = MSR::PSTATE_STATUS().CurPstate;
	MHz = (UINT32)MSR::PSTATE(0).get_frequency_mhz();
	mode = tmRDTSCP;
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Start<tmAPERF>()
{
	mode = tmNone;
	aperf_init = MSR::APERF();
	_mm_pause();
	aperf_init = MSR::APERF() - aperf_init;
	tsc_init = MSR::TSC();
	_mm_pause();
	tsc_init = MSR::TSC() - tsc_init;
	sync_time = MSR::APERF();
	_mm_pause();
	sync_time = MSR::APERF() - sync_time;
	_mm_pause();
	start_time = MSR::APERF() + sync_time;
	_mm_pause();
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Stop<tmAPERF>()
{
	start_time = MSR::APERF() - start_time;
	ratio = (double)aperf_init / (double)tsc_init;
	pstate = MSR::PSTATE_STATUS().CurPstate;
	MHz = (UINT32)MSR::PSTATE(0).get_frequency_mhz();
	mode = tmAPERF;
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Start<tmMPERF>()
{
	mode = tmNone;
	aperf_init = MSR::APERF();
	_mm_pause();
	aperf_init = MSR::APERF() - aperf_init;
	tsc_init = MSR::TSC();
	_mm_pause();
	tsc_init = MSR::TSC() - tsc_init;
	sync_time = MSR::MPERF();
	_mm_pause();
	sync_time = MSR::MPERF() - sync_time;
	_mm_pause();
	start_time = MSR::MPERF() + sync_time;
	_mm_pause();
	return;
}

template<>
void FORCEINLINE HIGH_PERCISION_TIMER::Stop<tmMPERF>()
{
	start_time = MSR::MPERF() - start_time;
	ratio = (double)aperf_init / (double)tsc_init;
	pstate = MSR::PSTATE_STATUS().CurPstate;
	MHz = (UINT32)MSR::PSTATE(0).get_frequency_mhz();
	mode = tmMPERF;
	return;
}

void ipi_read_efer_rdtsc(LOG_TIME* buffer)
{
	HIGH_PERCISION_TIMER clock;
	auto core_count = KeQueryActiveProcessorCount(0);
	auto cpu_id = CPUID::current_core_number();
	int idx = 3333;
	for (int i = 0; i < 10; i++)
	{
		clock.Start<tmRDTSC>();
		for (int x = 0; x < idx; x++)
			MSR::EFER();
		clock.Stop<tmRDTSC>();
		buffer[cpu_id + (core_count * i)] = clock.Log();
		buffer[cpu_id + (core_count * i)].TSC /= idx;
	}
	for (int i = 10; i < 20; i++)
	{
		clock.Start<tmRDTSCP>();
		for (int x = 0; x < idx; x++)
			MSR::EFER();
		clock.Stop<tmRDTSCP>();
		buffer[cpu_id + (core_count * i)] = clock.Log();
		buffer[cpu_id + (core_count * i)].TSC /= idx;
	}
	for (int i = 20; i < 30; i++)
	{
		clock.Start<tmMPERF>();
		for (int x = 0; x < idx; x++)
			MSR::EFER();
		clock.Stop<tmMPERF>();
		buffer[cpu_id + (core_count * i)] = clock.Log();
		buffer[cpu_id + (core_count * i)].TSC /= idx;
	}
	for (int i = 30; i < 40; i++)
	{
		clock.Start<tmAPERF>();
		for (int x = 0; x < idx; x++)
			MSR::EFER();
		clock.Stop<tmAPERF>();
		buffer[cpu_id + (core_count * i)] = clock.Log();
		buffer[cpu_id + (core_count * i)].TSC /= idx;
	}
	return;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto core_count = CPUID::current_core_number();
	auto buffer = (LOG_TIME*)ExAllocatePool(NonPagedPool, 0x20000);
	RtlFillMemory(buffer, 0x20000, 0);
	for (int x = 0; x < 10; x++)
	{
		KeIpiGenericCall(ipi_read_efer_rdtsc, buffer);

		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < core_count; j++)
			{
				auto log = buffer[j + (core_count * i)];
				printf("(%i, %i)", log.Rate / 100, log.TSC);
				//printf("Core %i: Mode %i, Rate %iMHz, TSC %i\n", j, log.Mode, log.Rate, log.TSC);
			}
		}
	}
	

	ExFreePool(buffer);
	return STATUS_SUCCESS;
}