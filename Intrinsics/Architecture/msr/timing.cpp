#include "intrinsics.hpp"

UINT64 MSR::TSC()
{
    return __readmsr(_MSR_TSC);
}

VOID MSR::TSC(UINT64 tsc)
{
    __writemsr(_MSR_TSC, tsc);
}

UINT64 MSR::TSC_DEADLINE()
{
    return __readmsr(_MSR_TSC_DEADLINE);
}

VOID MSR::TSC_DEADLINE(UINT64 deadline)
{
    __writemsr(_MSR_TSC_DEADLINE, deadline);
}

UINT64 MSR::APIC_LVT_TIMER()
{
    return __readmsr(_MSR_2XAPIC_LVT_TIMER);
}

VOID MSR::APIC_LVT_TIMER(UINT64 lvt_timer)
{
    __writemsr(_MSR_2XAPIC_LVT_TIMER, lvt_timer);
}

UINT64 MSR::APIC_TIMER_INIT_COUNT()
{
    return __readmsr(_MSR_2XAPIC_TIMER_INIT_COUNT);
}

VOID MSR::APIC_TIMER_INIT_COUNT(UINT64 init_count)
{
    __writemsr(_MSR_2XAPIC_TIMER_INIT_COUNT, init_count);
}

UINT64 MSR::APIC_TIMER_CUR_COUNT()
{
    return __readmsr(_MSR_2XAPIC_TIMER_CUR_COUNT);
}

UINT64 MSR::APIC_TIMER_DIV_CONF()
{
    return __readmsr(_MSR_2XAPIC_TIMER_DIV_CONF);
}

VOID MSR::APIC_TIMER_DIV_CONF(UINT64 div_conf)
{
    __writemsr(_MSR_2XAPIC_TIMER_DIV_CONF, div_conf);
}

UINT64 MSR::TSC_ADJUST()
{
    return __readmsr(_MSR_TSC_ADJUST);
}

VOID MSR::TSC_ADJUST(UINT64 adjust)
{
    __writemsr(_MSR_TSC_ADJUST, adjust);
}

UINT64 MSR_PSTATE::get_frequency_mhz()
{
    return IddValue * 100ll;
}

UINT64 MSR_PSTATE::get_performance_mhz()
{
    auto mperf_init = MSR::MPERF();
    auto aperf_init = MSR::APERF();
    for (int i = 0; i < 100000; i++)
        __kmd_pause();
    auto perf = (UINT64)((double)(MSR::APERF() - aperf_init) /
                         ((double)(MSR::MPERF() - mperf_init)) * (double)get_frequency_mhz());
    return perf;
}
