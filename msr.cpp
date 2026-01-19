#include "imports.hpp"

MSR_VM_CR MSR::read_vm_cr()
{
	return { .AsUINT64 = __readmsr(_MSR_VM_CR) };
}

void MSR::write_vm_cr(MSR_VM_CR vm_cr)
{
	return __writemsr(_MSR_VM_CR, vm_cr.AsUINT64);
}

MSR_EFER MSR::read_efer()
{
	return { .AsUINT64 = __readmsr(_MSR_EFER) };
}

void MSR::write_efer(MSR_EFER efer)
{
	return __writemsr(_MSR_EFER, efer.AsUINT64);
}

MSR_PAT MSR::read_pat()
{
	return { .AsUINT64 = __readmsr(_MSR_PAT) };
}

void MSR::write_pat(MSR_PAT pat)
{
	return __writemsr(_MSR_PAT, pat.AsUINT64);
}

UINT64 MSR::read_lstar()
{
	return __readmsr(_MSR_LSTAR);
}

void MSR::write_lstar(UINT64 lstar)
{
	__writemsr(_MSR_LSTAR, lstar);
}

UINT64 MSR::read_fs_base()
{
	return __readmsr(_MSR_FS_BASE);
}

void MSR::write_fs_base(UINT64 fs_base)
{
	__writemsr(_MSR_FS_BASE, fs_base);
}

UINT64 MSR::read_gs_base()
{
	return __readmsr(_MSR_GS_BASE);
}

void MSR::write_gs_base(UINT64 gs_base)
{
	__writemsr(_MSR_GS_BASE, gs_base);
}

PHYSICAL_ADDRESS MSR::read_hsave_pa()
{
	return __readmsr(_MSR_HSAVE_PA);
}

void MSR::write_hsave_pa(PHYSICAL_ADDRESS hsave_pa)
{
	__writemsr(_MSR_HSAVE_PA, hsave_pa);
}

UINT64 MSR::read_tsc_ratio()
{
	return __readmsr(_MSR_TSC_RATIO);
}

MSR_APIC_BASE MSR::read_apic_base()
{
	return { .AsUINT64 = __readmsr(_MSR_APIC_BASE) };
}

void MSR::write_apic_base(MSR_APIC_BASE apic_base)
{
	__writemsr(_MSR_APIC_BASE, apic_base.AsUINT64);
}

MSR_ICR MSR::read_icr()
{
	return { .AsUINT64 = __readmsr(_MSR_ICR) };
}

void MSR::write_icr(MSR_ICR icr)
{
	__writemsr(_MSR_ICR, icr.AsUINT64);
}

UINT64 MSR::read_smbase()
{
	return __readmsr(_MSR_SMBASE);
}

MSR_SMM_BASE MSR::read_smm_addr()
{
	return { .AsUINT64 = __readmsr(_MSR_SMM_ADDR) };
}

MSR_SMM_MASK MSR::read_smm_mask()
{
	return { .AsUINT64 = __readmsr(_MSR_SMM_MASK) };
}