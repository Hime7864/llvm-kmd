#include "intrinsics.hpp"

MSR_VM_CR MSR::VM_CR()
{
    return {.AsUINT64 = __readmsr(_MSR_VM_CR)};
}

VOID MSR::VM_CR(MSR_VM_CR vm_cr)
{
    __writemsr(_MSR_VM_CR, vm_cr.AsUINT64);
}

MSR_SPEC_CTRL MSR::SPEC_CTRL()
{
    return {.AsUINT64 = __readmsr(_MSR_SPEC_CTRL)};
}

VOID MSR::SPEC_CTRL(MSR_SPEC_CTRL spec_ctrl)
{
    __writemsr(_MSR_SPEC_CTRL, spec_ctrl.AsUINT64);
}

VOID MSR::PRED_CMD(MSR_PRED_CMD pred_cmd)
{
    __writemsr(_MSR_PRED_CMD, pred_cmd.AsUINT64);
}

UINT32 MSR::PATCH_LEVEL()
{
    return (UINT32)__readmsr(_MSR_PATCH_LEVEL);
}

MSR_MTRR_CAP MSR::MTRR_CAP()
{
    return {.AsUINT64 = __readmsr(_MSR_MTRR_CAP)};
}

UINT64 MSR::SYSENTER_CS()
{
    return __readmsr(_MSR_SYSENTER_CS);
}

VOID MSR::SYSENTER_CS(UINT64 value)
{
    __writemsr(_MSR_SYSENTER_CS, value);
}

UINT64 MSR::SYSENTER_ESP()
{
    return __readmsr(_MSR_SYSENTER_ESP);
}

VOID MSR::SYSENTER_ESP(UINT64 value)
{
    __writemsr(_MSR_SYSENTER_ESP, value);
}

UINT64 MSR::SYSENTER_EIP()
{
    return __readmsr(_MSR_SYSENTER_EIP);
}

VOID MSR::SYSENTER_EIP(UINT64 value)
{
    __writemsr(_MSR_SYSENTER_EIP, value);
}

MSR_EFER MSR::EFER()
{
    return {.AsUINT64 = __readmsr(_MSR_EFER)};
}

VOID MSR::EFER(MSR_EFER efer)
{
    __writemsr(_MSR_EFER, efer.AsUINT64);
}

MSR_PAT MSR::PAT()
{
    return {.AsUINT64 = __readmsr(_MSR_PAT)};
}

VOID MSR::PAT(MSR_PAT pat)
{
    __writemsr(_MSR_PAT, pat.AsUINT64);
}

MSR_STAR MSR::STAR()
{
    return {.AsUINT64 = __readmsr(_MSR_STAR)};
}

VOID MSR::STAR(MSR_STAR star)
{
    __writemsr(_MSR_STAR, star.AsUINT64);
}

UINT64 MSR::LSTAR()
{
    return __readmsr(_MSR_LSTAR);
}

VOID MSR::LSTAR(UINT64 lstar)
{
    __writemsr(_MSR_LSTAR, lstar);
}

UINT64 MSR::CSTAR()
{
    return __readmsr(_MSR_CSTAR);
}

VOID MSR::CSTAR(UINT64 cstar)
{
    __writemsr(_MSR_CSTAR, cstar);
}

MSR_SYSCALL_FLAG_MASK MSR::SYSCALL_FLAG_MASK()
{
    return {.AsUINT64 = __readmsr(_MSR_SYSCALL_FLAG_MASK)};
}

VOID MSR::SYSCALL_FLAG_MASK(MSR_SYSCALL_FLAG_MASK mask)
{
    __writemsr(_MSR_SYSCALL_FLAG_MASK, mask.AsUINT64);
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

UINT64 MSR::KERNEL_GS_BASE()
{
    return __readmsr(_MSR_KERNEL_GS_BASE);
}

VOID MSR::KERNEL_GS_BASE(UINT64 kernel_gs_base)
{
    __writemsr(_MSR_KERNEL_GS_BASE, kernel_gs_base);
}

MSR_TSC_AUX MSR::TSC_AUX()
{
    return {.AsUINT64 = __readmsr(_MSR_TSC_AUX)};
}

VOID MSR::TSC_AUX(MSR_TSC_AUX aux)
{
    __writemsr(_MSR_TSC_AUX, aux.AsUINT64);
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
    return {.AsUINT64 = __readmsr(_MSR_APIC_BASE)};
}

VOID MSR::APIC_BASE(MSR_APIC_BASE apic_base)
{
    __writemsr(_MSR_APIC_BASE, apic_base.AsUINT64);
}

MSR_ICR MSR::ICR()
{
    return {.AsUINT64 = __readmsr(_MSR_ICR)};
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
    return {.AsUINT64 = __readmsr(_MSR_SMM_ADDR)};
}

MSR_SMM_MASK MSR::SMM_MASK()
{
    return {.AsUINT64 = __readmsr(_MSR_SMM_MASK)};
}

MSR_MCG_CAP MSR::MCG_CAP()
{
    return {.AsUINT64 = __readmsr(_MSR_MCG_CAP)};
}

MSR_MCG_STATUS MSR::MCG_STATUS()
{
    return {.AsUINT64 = __readmsr(_MSR_MCG_STATUS)};
}

VOID MSR::MCG_STATUS(MSR_MCG_STATUS status)
{
    __writemsr(_MSR_MCG_STATUS, status.AsUINT64);
}

UINT64 MSR::MCG_CTL()
{
    return __readmsr(_MSR_MCG_CTL);
}

VOID MSR::MCG_CTL(UINT64 control)
{
    __writemsr(_MSR_MCG_CTL, control);
}

MSR_DBG_CTL MSR::DBG_CTL()
{
    return {.AsUINT64 = __readmsr(_MSR_DBG_CTL)};
}

VOID MSR::DBG_CTL(MSR_DBG_CTL debug_control)
{
    __writemsr(_MSR_DBG_CTL, debug_control.AsUINT64);
}

UINT64 MSR::BR_FROM()
{
    return __readmsr(_MSR_BR_FROM);
}

UINT64 MSR::BR_TO()
{
    return __readmsr(_MSR_BR_TO);
}

UINT64 MSR::LAST_EXCP_FROM_IP()
{
    return __readmsr(_MSR_LAST_EXCP_FROM_IP);
}

UINT64 MSR::LAST_EXCP_TO_IP()
{
    return __readmsr(_MSR_LAST_EXCP_TO_IP);
}

MSR_MTRR_DEF_TYPE MSR::MTRR_DEF_TYPE()
{
    return {.AsUINT64 = __readmsr(_MSR_MTRR_DEF_TYPE)};
}

VOID MSR::MTRR_DEF_TYPE(MSR_MTRR_DEF_TYPE data)
{
    __writemsr(_MSR_MTRR_DEF_TYPE, data.AsUINT64);
}

UINT64 MSR::U_CET()
{
    return __readmsr(_MSR_U_CET);
}

VOID MSR::U_CET(UINT64 data)
{
    __writemsr(_MSR_U_CET, data);
}

UINT64 MSR::S_CET()
{
    return __readmsr(_MSR_S_CET);
}

VOID MSR::S_CET(UINT64 data)
{
    __writemsr(_MSR_S_CET, data);
}

MSR_XSS MSR::XSS()
{
    return {.AsUINT64 = __readmsr(_MSR_XSS)};
}

VOID MSR::XSS(MSR_XSS data)
{
    __writemsr(_MSR_XSS, data.AsUINT64);
}

UINT64 MSR::APIC_ID()
{
    return __readmsr(_MSR_APIC_ID);
}

UINT64 MSR::APIC_VERSION()
{
    return __readmsr(_MSR_APIC_VERSION);
}

UINT64 MSR::APIC_TPR()
{
    return __readmsr(_MSR_APIC_TPR);
}

VOID MSR::APIC_TPR(UINT64 value)
{
    __writemsr(_MSR_APIC_TPR, value);
}

UINT64 MSR::APIC_PPR()
{
    return __readmsr(_MSR_APIC_PPR);
}

VOID MSR::APIC_EOI()
{
    __writemsr(_MSR_APIC_EOI, 0);
}

UINT64 MSR::APIC_LDR()
{
    return __readmsr(_MSR_APIC_LDR);
}

UINT64 MSR::APIC_SVR()
{
    return __readmsr(_MSR_APIC_SVR);
}

VOID MSR::APIC_SVR(UINT64 value)
{
    __writemsr(_MSR_APIC_SVR, value);
}

UINT64 MSR::APIC_ESR()
{
    return __readmsr(_MSR_APIC_ESR);
}

VOID MSR::APIC_ESR(UINT64 value)
{
    __writemsr(_MSR_APIC_ESR, value);
}

VOID MSR::APIC_SELF_IPI(UINT64 value)
{
    __writemsr(_MSR_APIC_SELF_IPI, value);
}
