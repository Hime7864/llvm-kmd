#include "intrinsics.hpp"

MSR_VM_CR MSR::VM_CR()
{
	return { .AsUINT64 = __readmsr(_MSR_VM_CR) };
}

VOID MSR::VM_CR(MSR_VM_CR vm_cr)
{
	__writemsr(_MSR_VM_CR, vm_cr.AsUINT64);
}

MSR_EFER MSR::EFER()
{
	return { .AsUINT64 = __readmsr(_MSR_EFER) };
}

VOID MSR::EFER(MSR_EFER efer)
{
	__writemsr(_MSR_EFER, efer.AsUINT64);
}

MSR_PAT MSR::PAT()
{
	return { .AsUINT64 = __readmsr(_MSR_PAT) };
}

VOID MSR::PAT(MSR_PAT pat)
{
	__writemsr(_MSR_PAT, pat.AsUINT64);
}

UINT64 MSR::LSTAR()
{
	return __readmsr(_MSR_LSTAR);
}

VOID MSR::LSTAR(UINT64 lstar)
{
	__writemsr(_MSR_LSTAR, lstar);
}

UINT64 MSR::FS_BASE()
{
	return __readmsr(_MSR_FS_BASE);
}

VOID MSR::FS_BASE(UINT64 fs_base)
{
	__writemsr(_MSR_FS_BASE, fs_base);
}

UINT64 MSR::GS_BASE()
{
	return __readmsr(_MSR_GS_BASE);
}

VOID MSR::GS_BASE(UINT64 gs_base)
{
	__writemsr(_MSR_GS_BASE, gs_base);
}

UINT64 MSR::HSAVE_PA()
{
	return __readmsr(_MSR_HSAVE_PA);
}

VOID MSR::HSAVE_PA(UINT64 hsave_pa)
{
	__writemsr(_MSR_HSAVE_PA, hsave_pa);
}

MSR_APIC_BASE MSR::APIC_BASE()
{
	return { .AsUINT64 = __readmsr(_MSR_APIC_BASE) };
}

VOID MSR::APIC_BASE(MSR_APIC_BASE apic_base)
{
	__writemsr(_MSR_APIC_BASE, apic_base.AsUINT64);
}

MSR_ICR MSR::ICR()
{
	return { .AsUINT64 = __readmsr(_MSR_ICR) };
}

VOID MSR::ICR(MSR_ICR icr)
{
	__writemsr(_MSR_ICR, icr.AsUINT64);
}

UINT64 MSR::SMBASE()
{
	return __readmsr(_MSR_SMBASE);
}

MSR_SMM_BASE MSR::SMM_ADDR()
{
	return { .AsUINT64 = __readmsr(_MSR_SMM_ADDR) };
}

MSR_SMM_MASK MSR::SMM_MASK()
{
	return { .AsUINT64 = __readmsr(_MSR_SMM_MASK) };
}

MSR_TSC_RATIO MSR::TSC_RATIO()
{
	return { .AsUINT64 = __readmsr(_MSR_TSC_RATIO) };
}

MSR_PSTATE_CURRENT_LIMIT MSR::PSTATE_CURRENT_LIMIT()
{
	return { .AsUINT64 = __readmsr(_MSR_PSTATE_CURRENT_LIMIT) };
}

MSR_PSTATE_CONTROL MSR::PSTATE_CONTROL()
{
	return { .AsUINT64 = __readmsr(_MSR_PSTATE_CONTROL) };
}

MSR_PSTATE_STATUS MSR::PSTATE_STATUS()
{
	return { .AsUINT64 = __readmsr(_MSR_PSTATE_STATUS) };
}

UINT64 MSR::APERF()
{
	return __readmsr(_MSR_APERF);
}

UINT64 MSR::MPERF()
{
	return __readmsr(_MSR_MPERF);
}

UINT64 MSR::APERF_READ_ONLY()
{
	return __readmsr(_MSR_APERF_READ_ONLY);
}

UINT64 MSR::MPERF_READ_ONLY()
{
	return __readmsr(_MSR_MPERF_READ_ONLY);
}

MSR_PSTATE MSR::PSTATE(int level)
{
	switch (level)
	{
	case 0:
		return { .AsUINT64 = __readmsr(_MSR_P0STATE) };
	case 1:
		return { .AsUINT64 = __readmsr(_MSR_P1STATE) };
	case 2:
		return { .AsUINT64 = __readmsr(_MSR_P2STATE) };
	default:
		return { .AsUINT64 = 0 };
	}
}

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
	return ((CpuFid + 16ll) * 100ll) / (1ll << CpuDfsId) * 2000000ll;
}

UINT64 MSR_PSTATE::get_performance_mhz()
{
	auto mperf_init = MSR::MPERF();
	auto aperf_init = MSR::APERF();
	for (int i = 0; i < 100000; i++)
		__asm { pause };
	auto perf = (UINT64)(
		(double)(MSR::APERF() - aperf_init) /
		((double)(MSR::MPERF() - mperf_init)
			) * (double)get_frequency_mhz());
	return perf;
}