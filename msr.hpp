#pragma once

struct MSR_VM_CR
{
	union
	{
		UINT64 AsUINT64;
		struct
		{
			UINT64 dpd : 1;
			UINT64 r_init : 1;
			UINT64 dis_a20m : 1;
			UINT64 lock : 1;
			UINT64 svm_dis : 1;
			UINT64 reserved0 : 59;
		};
	};
};

struct MSR_EFER
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64 sce : 1;
			UINT64 reserved0 : 7;
			UINT64 lme : 1;
			UINT64 reserved1 : 1;
			UINT64 lma : 1;
			UINT64 nxe : 1;
			UINT64 svme : 1;
			UINT64 lmsle : 1;
			UINT64 ffxsr : 1;
			UINT64 tce : 1;
			UINT64 reserved : 42;
		};
	};
};

struct MSR_PAT
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64 P0 : 3;
			UINT64 P1 : 3;
			UINT64 P2 : 3;
			UINT64 P3 : 3;
			UINT64 P4 : 3;
			UINT64 P5 : 3;
			UINT64 P6 : 3;
			UINT64 P7 : 3;
			UINT64 reserved : 40;
		};
	};
};

class MSR
{
private:
	static constexpr DWORD _MSR_FS_BASE = 0xC0000100UL;
	static constexpr DWORD _MSR_GS_BASE = 0xC0000101UL;
	//VM_CR MSR (C001_0114h)
	static constexpr DWORD _MSR_VM_CR = 0xC0010114UL;
	//LSTAR MSR (C000_0082h)
	static constexpr DWORD _MSR_LSTAR = 0xC0000082UL;
	//3.1.7 Extended Feature Enable Register (EFER)
	static constexpr DWORD _MSR_EFER = 0xC0000080UL;
	static constexpr DWORD _MSR_PAT = 0x00000277UL;
	static constexpr DWORD _MSR_HSAVE_PA = 0xC0010117UL;

	static constexpr DWORD _MSR_TSC_RATIO = 0xC0000104UL;
public:
	static MSR_VM_CR read_vm_cr();
	static void write_vm_cr(MSR_VM_CR vm_cr);
	static MSR_EFER read_efer();
	static void write_efer(MSR_EFER efer);
	static MSR_PAT read_pat();
	static void write_pat(MSR_PAT pat);
	static UINT64 read_lstar();
	static void write_lstar(UINT64 lstar);
	static UINT64 read_fs_base();
	static void write_fs_base(UINT64 fs_base);
	static UINT64 read_gs_base();
	static void write_gs_base(UINT64 gs_base);
	static UINT64 read_hsave_pa();
	static void write_hsave_pa(UINT64 hsave_pa);
	static UINT64 read_tsc_ratio();
};