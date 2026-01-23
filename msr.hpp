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

struct MSR_ICR
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64 vec : 8;
			UINT64 mt : 3;
			UINT64 dm : 1;
			UINT64 Reserved0 : 2;
			UINT64 l : 1;
			UINT64 tmg : 1;
			UINT64 Reserved1 : 2;
			UINT64 dsh : 2;
			UINT64 Reserved2 : 12;
			UINT64 dest : 32;
		};
	};
};

struct MSR_APIC_BASE
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64 Reserved0 : 8;
			UINT64 bsc : 1;//Boot Strap CPU Core
			UINT64 Reserved1 : 1;
			UINT64 extd : 1;// x2APIC Mode Enable
			UINT64 ae : 1;//APIC Enable
			UINT64 aba : 40;// APIC Base Address
			UINT64 Reserved2 : 12;
		};
	};
};

struct MSR_SMM_MASK
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64  : 12;
			UINT64 TMTypeDram : 3;
			UINT64 : 1;
			UINT64 TSegMask : 32;
			UINT64 : 16;

		};
	};
};

struct MSR_SMM_BASE
{
	union {
		UINT64 AsUINT64;
		struct
		{
			UINT64 : 16;
			UINT64 TSegBase : 32;
			UINT64 : 16;
		};
	};
};

struct MSR_TSC_RATIO
{
	union {
		UINT64 AsUINT64;
		struct {
			UINT64 FRAC : 32;
			UINT64 INT : 8;
			UINT64 : 24;
		};
	};
};


struct MSR_PSTATE_CURRENT_LIMIT
{
	union {
		UINT64 AsUINT64;
		struct {
			UINT64 CurPstateLimit : 4;
			UINT64 PstateMaxVal : 4;
			UINT64 : 56;
		};
	};
};

struct MSR_PSTATE_CONTROL
{
	union {
		UINT64 AsUINT64;
		struct {
			UINT64 PstateCmd : 4;
			UINT64 : 60;
		};
	};
};

struct MSR_PSTATE_STATUS
{
	union {
		UINT64 AsUINT64;
		struct {
			UINT64 CurPstate : 4;
			UINT64 : 60;
		};
	};
};

struct MSR_PSTATE
{
	union {
		UINT64 AsUINT64;
		struct {
			UINT64 CpuFid : 8;
			UINT64 CpuDfsId : 6;
			UINT64 CpuVid : 8;
			UINT64 IddValue : 8;
			UINT64 IddDiv : 2;
			UINT64 : 30;
			UINT64 PstateEn : 2;
		};
	};
	UINT64 get_frequency_mhz();

	UINT64 get_performance_mhz();
};

class MSR
{
public:
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

	static constexpr DWORD _MSR_APIC_BASE = 0x0000001BUL;
	static constexpr DWORD _MSR_ICR = 0x00000830UL;

	static constexpr DWORD _MSR_SMBASE = 0xC0010111UL;
	static constexpr DWORD _MSR_SMM_ADDR = 0xC0010112UL;
	static constexpr DWORD _MSR_SMM_MASK = 0xC0010113UL;

	static constexpr DWORD _MSR_TSC_RATIO = 0xC0000104UL;

	static constexpr DWORD _MSR_PSTATE_CURRENT_LIMIT = 0xC0010061UL;
	static constexpr DWORD _MSR_PSTATE_CONTROL = 0xC0010062UL;
	static constexpr DWORD _MSR_PSTATE_STATUS = 0xC0010063UL;
	static constexpr DWORD _MSR_P0STATE = 0xC0010064UL;
	static constexpr DWORD _MSR_P1STATE = 0xC0010065UL;
	static constexpr DWORD _MSR_P2STATE = 0xC0010066UL;

	static constexpr DWORD _MSR_APERF = 0x000000E8UL;
	static constexpr DWORD _MSR_MPERF = 0x000000E7UL;

	static constexpr DWORD _MSR_APERF_READ_ONLY = 0xC00000E8UL;
	static constexpr DWORD _MSR_MPERF_READ_ONLY = 0xC00000E7UL;

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
	static MSR_APIC_BASE read_apic_base();
	static void write_apic_base(MSR_APIC_BASE apic_base);
	static MSR_ICR read_icr();
	static void write_icr(MSR_ICR icr);
	static UINT64 read_smbase();
	static MSR_SMM_BASE read_smm_addr();
	static MSR_SMM_MASK read_smm_mask();
	static MSR_TSC_RATIO read_tsc_ratio();
	static MSR_PSTATE_CURRENT_LIMIT read_pstate_current_limit();
	static MSR_PSTATE_CONTROL read_pstate_control();
	static MSR_PSTATE_STATUS read_pstate_status();
	static UINT64 read_aperf();
	static UINT64 read_mperf();
	static UINT64 read_aperf_read_only();
	static UINT64 read_mperf_read_only();
	static MSR_PSTATE read_pstate(int level);
};