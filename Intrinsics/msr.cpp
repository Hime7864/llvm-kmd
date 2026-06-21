#include "msr.hpp"
#include "assembly.hpp"

MSR_TSC_RATIO MSR::TSC_RATIO()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_TSC_RATIO)};
}

MSR_PSTATE_CURRENT_LIMIT MSR::PSTATE_CURRENT_LIMIT()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_PSTATE_CURRENT_LIMIT)};
}

MSR_PSTATE_CONTROL MSR::PSTATE_CONTROL()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_PSTATE_CONTROL)};
}

VOID MSR::PSTATE_CONTROL(MSR_PSTATE_CONTROL data)
{
    return _mm_writemsr(_MSR_PSTATE_CONTROL, data.AsUINT64);
}

MSR_PSTATE_STATUS MSR::PSTATE_STATUS()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_PSTATE_STATUS)};
}

UINT64 NOINLINE MSR::APERF()
{
    return _mm_readmsr(_MSR_APERF);
}

UINT64 NOINLINE MSR::IRPerfCount()
{
    return _mm_readmsr(_MSR_IRPerfCount);
}


UINT64 NOINLINE MSR::MPERF()
{
    return _mm_readmsr(_MSR_MPERF);
}

VOID NOINLINE MSR::APERF(UINT64 data)
{
    return _mm_writemsr(_MSR_APERF, data);
}

VOID NOINLINE MSR::MPERF(UINT64 data)
{
    return _mm_writemsr(_MSR_MPERF, data);
}

UINT64 NOINLINE MSR::APERF_READ_ONLY()
{
    return _mm_readmsr(_MSR_APERF_READ_ONLY);
}

UINT64 NOINLINE MSR::MPERF_READ_ONLY()
{
    return _mm_readmsr(_MSR_MPERF_READ_ONLY);
}

MSR_CPPC_CAPABILITY_1 NOINLINE MSR::CPPC_CAPABILITY_1()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CPPC_CAPABILITY_1)};
}

MSR_CPPC_ENABLE NOINLINE MSR::CPPC_ENABLE()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CPPC_ENABLE)};
}

void NOINLINE MSR::CPPC_ENABLE(MSR_CPPC_ENABLE enable)
{
    _mm_writemsr(_MSR_CPPC_ENABLE, enable.AsUINT64);
}

MSR_CPPC_CAPABILITY_2 NOINLINE MSR::CPPC_CAPABILITY_2()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CPPC_CAPABILITY_2)};
}

MSR_CPPC_REQUEST NOINLINE MSR::CPPC_REQUEST()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CPPC_REQUEST)};
}

void NOINLINE MSR::CPPC_REQUEST(MSR_CPPC_REQUEST request)
{
	_mm_writemsr(_MSR_CPPC_REQUEST, request.AsUINT64);
}

MSR_CPPC_STATUS NOINLINE MSR::CPPC_STATUS()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CPPC_STATUS)};
}
//L3_QOS_ABMC_CFG

MSR_L3_QOS_ABMC_CFG NOINLINE MSR::L3_QOS_ABMC_CFG()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_L3_QOS_ABMC_CFG)};
}

MSR_L3_RAPL_POWER_UNIT_0 NOINLINE MSR::L3_RAPL_POWER_UNIT_0()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_L3_RAPL_POWER_UNIT_0)};
}

MSR_CORE_ENERGY_STAT NOINLINE MSR::CORE_ENERGY_STAT()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_CORE_ENERGY_STAT)};
}

MSR_L3_PACKAGE_ENERGY_STATUS NOINLINE MSR::L3_PACKAGE_ENERGY_STATUS()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_L3_PACKAGE_ENERGY_STATUS)};
}

MSR_PSTATE MSR::PSTATE(int level)
{
    switch (level)
    {
    case 0:
        return {.AsUINT64 = _mm_readmsr(_MSR_P0STATE)};
    case 1:
        return {.AsUINT64 = _mm_readmsr(_MSR_P1STATE)};
    case 2:
        return {.AsUINT64 = _mm_readmsr(_MSR_P2STATE)};
    default:
        return {.AsUINT64 = 0};
    }
}

MSR_PREFETCH_CONTROL MSR::PREFETCH_CONTROL()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_PREFETCH_CONTROL)};
}

VOID MSR::PREFETCH_CONTROL(MSR_PREFETCH_CONTROL control)
{
    _mm_writemsr(_MSR_PREFETCH_CONTROL, control.AsUINT64);
}

MSR_SYS_CFG MSR::SYS_CFG()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_SYS_CFG)};
}

VOID MSR::SYS_CFG(MSR_SYS_CFG sys_cfg)
{
    _mm_writemsr(_MSR_SYS_CFG, sys_cfg.AsUINT64);
}

MSR_HWCR MSR::HWCR()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_HWCR)};
}

VOID MSR::HWCR(MSR_HWCR hwcr)
{
    _mm_writemsr(_MSR_HWCR, hwcr.AsUINT64);
}

MSR_TOP_MEM MSR::TOP_MEM()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_TOP_MEM)};
}

VOID MSR::TOP_MEM(MSR_TOP_MEM top_mem)
{
    _mm_writemsr(_MSR_TOP_MEM, top_mem.AsUINT64);
}

MSR_TOM2 MSR::TOM2()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_TOM2)};
}

VOID MSR::TOM2(MSR_TOM2 tom2)
{
    _mm_writemsr(_MSR_TOM2, tom2.AsUINT64);
}

UINT64 MSR::TSC()
{
    return _mm_readmsr(_MSR_TSC);
}

VOID MSR::TSC(UINT64 tsc)
{
    _mm_writemsr(_MSR_TSC, tsc);
}

UINT64 MSR::TSC_DEADLINE()
{
    return _mm_readmsr(_MSR_TSC_DEADLINE);
}

VOID MSR::TSC_DEADLINE(UINT64 deadline)
{
    _mm_writemsr(_MSR_TSC_DEADLINE, deadline);
}

UINT64 MSR::APIC_LVT_TIMER()
{
    return _mm_readmsr(_MSR_2XAPIC_LVT_TIMER);
}

VOID MSR::APIC_LVT_TIMER(UINT64 lvt_timer)
{
    _mm_writemsr(_MSR_2XAPIC_LVT_TIMER, lvt_timer);
}

UINT64 MSR::APIC_TIMER_INIT_COUNT()
{
    return _mm_readmsr(_MSR_2XAPIC_TIMER_INIT_COUNT);
}

VOID MSR::APIC_TIMER_INIT_COUNT(UINT64 init_count)
{
    _mm_writemsr(_MSR_2XAPIC_TIMER_INIT_COUNT, init_count);
}

UINT64 MSR::APIC_TIMER_CUR_COUNT()
{
    return _mm_readmsr(_MSR_2XAPIC_TIMER_CUR_COUNT);
}

UINT64 MSR::APIC_TIMER_DIV_CONF()
{
    return _mm_readmsr(_MSR_2XAPIC_TIMER_DIV_CONF);
}

VOID MSR::APIC_TIMER_DIV_CONF(UINT64 div_conf)
{
    _mm_writemsr(_MSR_2XAPIC_TIMER_DIV_CONF, div_conf);
}

UINT64 MSR::TSC_ADJUST()
{
    return _mm_readmsr(_MSR_TSC_ADJUST);
}

VOID MSR::TSC_ADJUST(UINT64 adjust)
{
    _mm_writemsr(_MSR_TSC_ADJUST, adjust);
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
        _mm_pause();
    auto perf = (UINT64)((double)(MSR::APERF() - aperf_init) /
                         ((double)(MSR::MPERF() - mperf_init)) * (double)get_frequency_mhz());
    return perf;
}

MSR_VM_CR MSR::VM_CR()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_VM_CR)};
}

VOID MSR::VM_CR(MSR_VM_CR vm_cr)
{
    _mm_writemsr(_MSR_VM_CR, vm_cr.AsUINT64);
}

MSR_SPEC_CTRL MSR::SPEC_CTRL()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_SPEC_CTRL)};
}

VOID MSR::SPEC_CTRL(MSR_SPEC_CTRL spec_ctrl)
{
    _mm_writemsr(_MSR_SPEC_CTRL, spec_ctrl.AsUINT64);
}

VOID MSR::PRED_CMD(MSR_PRED_CMD pred_cmd)
{
    _mm_writemsr(_MSR_PRED_CMD, pred_cmd.AsUINT64);
}

UINT32 MSR::PATCH_LEVEL()
{
    return (UINT32)_mm_readmsr(_MSR_PATCH_LEVEL);
}

MSR_MTRR_CAP MSR::MTRR_CAP()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_MTRR_CAP)};
}

UINT64 MSR::SYSENTER_CS()
{
    return _mm_readmsr(_MSR_SYSENTER_CS);
}

VOID MSR::SYSENTER_CS(UINT64 value)
{
    _mm_writemsr(_MSR_SYSENTER_CS, value);
}

UINT64 MSR::SYSENTER_ESP()
{
    return _mm_readmsr(_MSR_SYSENTER_ESP);
}

VOID MSR::SYSENTER_ESP(UINT64 value)
{
    _mm_writemsr(_MSR_SYSENTER_ESP, value);
}

UINT64 MSR::SYSENTER_EIP()
{
    return _mm_readmsr(_MSR_SYSENTER_EIP);
}

VOID MSR::SYSENTER_EIP(UINT64 value)
{
    _mm_writemsr(_MSR_SYSENTER_EIP, value);
}

MSR_EFER MSR::EFER()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_EFER)};
}

VOID MSR::EFER(MSR_EFER efer)
{
    _mm_writemsr(_MSR_EFER, efer.AsUINT64);
}

MSR_PAT MSR::PAT()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_PAT)};
}

VOID MSR::PAT(MSR_PAT pat)
{
    _mm_writemsr(_MSR_PAT, pat.AsUINT64);
}

MSR_STAR MSR::STAR()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_STAR)};
}

VOID MSR::STAR(MSR_STAR star)
{
    _mm_writemsr(_MSR_STAR, star.AsUINT64);
}

UINT64 MSR::LSTAR()
{
    return _mm_readmsr(_MSR_LSTAR);
}

VOID MSR::LSTAR(UINT64 lstar)
{
    _mm_writemsr(_MSR_LSTAR, lstar);
}

UINT64 MSR::CSTAR()
{
    return _mm_readmsr(_MSR_CSTAR);
}

VOID MSR::CSTAR(UINT64 cstar)
{
    _mm_writemsr(_MSR_CSTAR, cstar);
}

MSR_SYSCALL_FLAG_MASK MSR::SYSCALL_FLAG_MASK()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_SYSCALL_FLAG_MASK)};
}

VOID MSR::SYSCALL_FLAG_MASK(MSR_SYSCALL_FLAG_MASK mask)
{
    _mm_writemsr(_MSR_SYSCALL_FLAG_MASK, mask.AsUINT64);
}

UINT64 MSR::FS_BASE()
{
    return _mm_readmsr(_MSR_FS_BASE);
}

VOID MSR::FS_BASE(UINT64 fs_base)
{
    _mm_writemsr(_MSR_FS_BASE, fs_base);
}

UINT64 MSR::GS_BASE()
{
    return _mm_readmsr(_MSR_GS_BASE);
}

VOID MSR::GS_BASE(UINT64 gs_base)
{
    _mm_writemsr(_MSR_GS_BASE, gs_base);
}

UINT64 MSR::KERNEL_GS_BASE()
{
    return _mm_readmsr(_MSR_KERNEL_GS_BASE);
}

VOID MSR::KERNEL_GS_BASE(UINT64 kernel_gs_base)
{
    _mm_writemsr(_MSR_KERNEL_GS_BASE, kernel_gs_base);
}

MSR_TSC_AUX MSR::TSC_AUX()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_TSC_AUX)};
}

VOID MSR::TSC_AUX(MSR_TSC_AUX aux)
{
    _mm_writemsr(_MSR_TSC_AUX, aux.AsUINT64);
}

UINT64 MSR::HSAVE_PA()
{
    return _mm_readmsr(_MSR_HSAVE_PA);
}

VOID MSR::HSAVE_PA(UINT64 hsave_pa)
{
    _mm_writemsr(_MSR_HSAVE_PA, hsave_pa);
}

MSR_APIC_BASE MSR::APIC_BASE()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_APIC_BASE)};
}

VOID MSR::APIC_BASE(MSR_APIC_BASE apic_base)
{
    _mm_writemsr(_MSR_APIC_BASE, apic_base.AsUINT64);
}

MSR_ICR MSR::ICR()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_ICR)};
}

VOID MSR::ICR(MSR_ICR icr)
{
    _mm_writemsr(_MSR_ICR, icr.AsUINT64);
}

UINT64 MSR::SMBASE()
{
    return _mm_readmsr(_MSR_SMBASE);
}

MSR_SMM_BASE MSR::SMM_ADDR()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_SMM_ADDR)};
}

MSR_SMM_MASK MSR::SMM_MASK()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_SMM_MASK)};
}

MSR_MCG_CAP MSR::MCG_CAP()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_MCG_CAP)};
}

MSR_MCG_STATUS MSR::MCG_STATUS()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_MCG_STATUS)};
}

VOID MSR::MCG_STATUS(MSR_MCG_STATUS status)
{
    _mm_writemsr(_MSR_MCG_STATUS, status.AsUINT64);
}

UINT64 MSR::MCG_CTL()
{
    return _mm_readmsr(_MSR_MCG_CTL);
}

VOID MSR::MCG_CTL(UINT64 control)
{
    _mm_writemsr(_MSR_MCG_CTL, control);
}

MSR_DBG_CTL MSR::DBG_CTL()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_DBG_CTL)};
}

VOID MSR::DBG_CTL(MSR_DBG_CTL debug_control)
{
    _mm_writemsr(_MSR_DBG_CTL, debug_control.AsUINT64);
}

UINT64 MSR::BR_FROM()
{
    return _mm_readmsr(_MSR_BR_FROM);
}

UINT64 MSR::BR_TO()
{
    return _mm_readmsr(_MSR_BR_TO);
}

UINT64 MSR::LAST_EXCP_FROM_IP()
{
    return _mm_readmsr(_MSR_LAST_EXCP_FROM_IP);
}

UINT64 MSR::LAST_EXCP_TO_IP()
{
    return _mm_readmsr(_MSR_LAST_EXCP_TO_IP);
}

MSR_MTRR_DEF_TYPE MSR::MTRR_DEF_TYPE()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_MTRR_DEF_TYPE)};
}

VOID MSR::MTRR_DEF_TYPE(MSR_MTRR_DEF_TYPE data)
{
    _mm_writemsr(_MSR_MTRR_DEF_TYPE, data.AsUINT64);
}

UINT64 MSR::U_CET()
{
    return _mm_readmsr(_MSR_U_CET);
}

VOID MSR::U_CET(UINT64 data)
{
    _mm_writemsr(_MSR_U_CET, data);
}

UINT64 MSR::S_CET()
{
    return _mm_readmsr(_MSR_S_CET);
}

VOID MSR::S_CET(UINT64 data)
{
    _mm_writemsr(_MSR_S_CET, data);
}

MSR_XSS MSR::XSS()
{
    return {.AsUINT64 = _mm_readmsr(_MSR_XSS)};
}

VOID MSR::XSS(MSR_XSS data)
{
    _mm_writemsr(_MSR_XSS, data.AsUINT64);
}

UINT64 MSR::APIC_ID()
{
    return _mm_readmsr(_MSR_APIC_ID);
}

UINT64 MSR::APIC_VERSION()
{
    return _mm_readmsr(_MSR_APIC_VERSION);
}

UINT64 MSR::APIC_TPR()
{
    return _mm_readmsr(_MSR_APIC_TPR);
}

VOID MSR::APIC_TPR(UINT64 value)
{
    _mm_writemsr(_MSR_APIC_TPR, value);
}

UINT64 MSR::APIC_PPR()
{
    return _mm_readmsr(_MSR_APIC_PPR);
}

VOID MSR::APIC_EOI()
{
    _mm_writemsr(_MSR_APIC_EOI, 0);
}

UINT64 MSR::APIC_LDR()
{
    return _mm_readmsr(_MSR_APIC_LDR);
}

UINT64 MSR::APIC_SVR()
{
    return _mm_readmsr(_MSR_APIC_SVR);
}

VOID MSR::APIC_SVR(UINT64 value)
{
    _mm_writemsr(_MSR_APIC_SVR, value);
}

UINT64 MSR::APIC_ESR()
{
    return _mm_readmsr(_MSR_APIC_ESR);
}

VOID MSR::APIC_ESR(UINT64 value)
{
    _mm_writemsr(_MSR_APIC_ESR, value);
}

VOID MSR::APIC_SELF_IPI(UINT64 value)
{
    _mm_writemsr(_MSR_APIC_SELF_IPI, value);
}