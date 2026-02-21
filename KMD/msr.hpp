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
			UINT64 VEC : 8;
			ICR_MT MT : 3;
			ICR_DM DM : 1;
			UINT64 : 2;
			ICR_L L : 1;
			ICR_TGM TGM : 1;
			UINT64 : 2;
			ICR_DSH DSH : 2;
			UINT64 : 12;
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
	static constexpr DWORD _MSR_VM_CR = 0xC0010114UL;
	static constexpr DWORD _MSR_LSTAR = 0xC0000082UL;
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
	static constexpr DWORD _MSR_TSC = 0x00000010UL;
	static constexpr DWORD _MSR_TSC_DEADLINE = 0x000006E0UL;
	static constexpr DWORD _MSR_TSC_ADJUST = 0x0000003BUL;
	static constexpr DWORD _MSR_2XAPIC_LVT_TIMER = 0x00000832UL;
	static constexpr DWORD _MSR_2XAPIC_TIMER_INIT_COUNT = 0x00000838UL;
	static constexpr DWORD _MSR_2XAPIC_TIMER_CUR_COUNT = 0x00000839UL;
	static constexpr DWORD _MSR_2XAPIC_TIMER_DIV_CONF = 0x0000083EUL;

	static MSR_VM_CR VM_CR();
	static VOID VM_CR(MSR_VM_CR vm_cr);

	static MSR_EFER EFER();
	static VOID EFER(MSR_EFER efer);

	static MSR_PAT PAT();
	static VOID PAT(MSR_PAT pat);

	static UINT64 LSTAR();
	static VOID LSTAR(UINT64 lstar);

	static UINT64 FS_BASE();
	static VOID FS_BASE(UINT64 fs_base);

	static UINT64 GS_BASE();
	static VOID GS_BASE(UINT64 gs_base);

	static UINT64 HSAVE_PA();
	static VOID HSAVE_PA(UINT64 hsave_pa);

	static MSR_APIC_BASE APIC_BASE();
	static VOID APIC_BASE(MSR_APIC_BASE apic_base);

	static MSR_ICR ICR();
	static VOID ICR(MSR_ICR icr);

	static UINT64 SMBASE();

	static MSR_SMM_BASE SMM_ADDR();

	static MSR_SMM_MASK SMM_MASK();

	static MSR_TSC_RATIO TSC_RATIO();

	static MSR_PSTATE_CURRENT_LIMIT PSTATE_CURRENT_LIMIT();

	static MSR_PSTATE_CONTROL PSTATE_CONTROL();

	static MSR_PSTATE_STATUS PSTATE_STATUS();

	static UINT64 APERF();

	static UINT64 MPERF();

	static UINT64 APERF_READ_ONLY();

	static UINT64 MPERF_READ_ONLY();

	static MSR_PSTATE PSTATE(int level);

	static UINT64 TSC();
	static VOID TSC(UINT64 tsc);

	static UINT64 TSC_DEADLINE();
	static VOID TSC_DEADLINE(UINT64 deadline);

	static UINT64 TSC_ADJUST();
	static VOID TSC_ADJUST(UINT64 adjust);

	static UINT64 APIC_LVT_TIMER();
	static VOID APIC_LVT_TIMER(UINT64 lvt_timer);

	static UINT64 APIC_TIMER_INIT_COUNT();
	static VOID APIC_TIMER_INIT_COUNT(UINT64 init_count);

	static UINT64 APIC_TIMER_CUR_COUNT();

	static UINT64 APIC_TIMER_DIV_CONF();
	static VOID APIC_TIMER_DIV_CONF(UINT64 div_conf);
};