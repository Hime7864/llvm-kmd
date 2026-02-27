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

void ipi_read_efer_rdtsc(UINT64 buffer)
{
	HIGH_PERCISION_TIMER clock;
	auto cpu_id = CPUID::current_core_number();
	auto logs = (LOG_TIME*)(buffer + cpu_id * 0x1000);
	for (int i = 0; i < 512; i++)
	{
		clock.Start<tmRDTSC>();
		MSR::EFER();
		clock.Stop<tmRDTSC>();
		logs[i] = clock.Log();
	}
	return;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto core_count = CPUID::current_core_number();
	auto tsc_buffer = ExAllocatePool(NonPagedPool, 0x1000 * core_count);
	KeIpiGenericCall(ipi_read_efer_rdtsc, tsc_buffer);
	

	for (int j = 0; j < 512; j++)
	{
		for (int i = 0; i < core_count; i++)
		{
			auto logs = (LOG_TIME*)((UINT64)tsc_buffer + i * 0x1000);
			auto log = logs[j];
			printf("Core %i: Mode %i, Rate %iMHz, TSC %i\n", i, log.Mode, log.Rate, log.TSC);

		}
	}
	ExFreePool(tsc_buffer);
	return STATUS_SUCCESS;
}