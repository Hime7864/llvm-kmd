#include "intrinsics.hpp"

MSR_TSC_RATIO MSR::TSC_RATIO()
{
    return {.AsUINT64 = __readmsr(_MSR_TSC_RATIO)};
}

MSR_PSTATE_CURRENT_LIMIT MSR::PSTATE_CURRENT_LIMIT()
{
    return {.AsUINT64 = __readmsr(_MSR_PSTATE_CURRENT_LIMIT)};
}

MSR_PSTATE_CONTROL MSR::PSTATE_CONTROL()
{
    return {.AsUINT64 = __readmsr(_MSR_PSTATE_CONTROL)};
}

VOID MSR::PSTATE_CONTROL(MSR_PSTATE_CONTROL data)
{
    return __writemsr(_MSR_PSTATE_CONTROL, data.AsUINT64);
}

MSR_PSTATE_STATUS MSR::PSTATE_STATUS()
{
    return {.AsUINT64 = __readmsr(_MSR_PSTATE_STATUS)};
}

UINT64 NOINLINE MSR::APERF()
{
    return __readmsr(_MSR_APERF);
}

UINT64 NOINLINE MSR::MPERF()
{
    return __readmsr(_MSR_MPERF);
}

VOID NOINLINE MSR::APERF(UINT64 data)
{
    return __writemsr(_MSR_APERF, data);
}

VOID NOINLINE MSR::MPERF(UINT64 data)
{
    return __writemsr(_MSR_MPERF, data);
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
        return {.AsUINT64 = __readmsr(_MSR_P0STATE)};
    case 1:
        return {.AsUINT64 = __readmsr(_MSR_P1STATE)};
    case 2:
        return {.AsUINT64 = __readmsr(_MSR_P2STATE)};
    default:
        return {.AsUINT64 = 0};
    }
}

MSR_PREFETCH_CONTROL MSR::PREFETCH_CONTROL()
{
    return {.AsUINT64 = __readmsr(_MSR_PREFETCH_CONTROL)};
}

VOID MSR::PREFETCH_CONTROL(MSR_PREFETCH_CONTROL control)
{
    __writemsr(_MSR_PREFETCH_CONTROL, control.AsUINT64);
}

MSR_SYS_CFG MSR::SYS_CFG()
{
    return {.AsUINT64 = __readmsr(_MSR_SYS_CFG)};
}

VOID MSR::SYS_CFG(MSR_SYS_CFG sys_cfg)
{
    __writemsr(_MSR_SYS_CFG, sys_cfg.AsUINT64);
}

MSR_HWCR MSR::HWCR()
{
    return {.AsUINT64 = __readmsr(_MSR_HWCR)};
}

VOID MSR::HWCR(MSR_HWCR hwcr)
{
    __writemsr(_MSR_HWCR, hwcr.AsUINT64);
}

MSR_TOP_MEM MSR::TOP_MEM()
{
    return {.AsUINT64 = __readmsr(_MSR_TOP_MEM)};
}

VOID MSR::TOP_MEM(MSR_TOP_MEM top_mem)
{
    __writemsr(_MSR_TOP_MEM, top_mem.AsUINT64);
}

MSR_TOM2 MSR::TOM2()
{
    return {.AsUINT64 = __readmsr(_MSR_TOM2)};
}

VOID MSR::TOM2(MSR_TOM2 tom2)
{
    __writemsr(_MSR_TOM2, tom2.AsUINT64);
}
