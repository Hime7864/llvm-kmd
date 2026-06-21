#pragma once

#include "structures.hpp"

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
    union
    {
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
    union
    {
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

struct MSR_SPEC_CTRL
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 IBRS : 1;
            UINT64 STIBP : 1;
            UINT64 SSBD : 1;
            UINT64 Reserved0 : 4;
            UINT64 PSFD : 1;
            UINT64 Reserved1 : 56;
        };
    };
};

struct MSR_PRED_CMD
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 IBPB : 1;
            UINT64 Reserved : 63;
        };
    };
};

struct MSR_MTRR_CAP
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 VariableRangeCount : 8;
            UINT64 FixedRangeSupported : 1;
            UINT64 Reserved0 : 1;
            UINT64 WriteCombiningSupported : 1;
            UINT64 Reserved1 : 53;
        };
    };
};

struct MSR_STAR
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Target : 32;
            UINT64 Reserved : 16;
            UINT64 SysRetSelector : 16;
        };
    };
};

struct MSR_SYSCALL_FLAG_MASK
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Mask : 32;
            UINT64 Reserved : 32;
        };
    };
};

struct MSR_ICR
{
    union
    {
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
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 8;
            UINT64 bsc : 1; // Boot Strap CPU Core
            UINT64 Reserved1 : 1;
            UINT64 extd : 1; // x2APIC Mode Enable
            UINT64 ae : 1;   // APIC Enable
            UINT64 aba : 40; // APIC Base Address
            UINT64 Reserved2 : 12;
        };
    };
};

struct MSR_SMM_MASK
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 : 12;
            UINT64 TMTypeDram : 3;
            UINT64 : 1;
            UINT64 TSegMask : 32;
            UINT64 : 16;
        };
    };
};

struct MSR_SMM_BASE
{
    union
    {
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
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 FRAC : 32;
            UINT64 INT : 8;
            UINT64 : 24;
        };
    };
};

struct MSR_TSC_AUX
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Aux : 32;
            UINT64 Reserved : 32;
        };
    };
};

struct MSR_MCG_CAP
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Count : 8;
            UINT64 MCG_CTL_Present : 1;
            UINT64 Reserved : 55;
        };
    };
};

struct MSR_MCG_STATUS
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 RIPV : 1;
            UINT64 EIPV : 1;
            UINT64 MCIP : 1;
            UINT64 Reserved : 61;
        };
    };
};

struct MSR_DBG_CTL
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 LBR : 1;
            UINT64 BTF : 1;
            UINT64 PB : 4;
            UINT64 Reserved : 58;
        };
    };
};

struct MSR_MTRR_DEF_TYPE
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 MemoryType : 8;
            UINT64 Reserved0 : 2;
            UINT64 FixedRangeEnable : 1;
            UINT64 Enable : 1;
            UINT64 Reserved1 : 52;
        };
    };
};

struct MSR_XSS
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 11;
            UINT64 CET_U : 1;
            UINT64 CET_S : 1;
            UINT64 Reserved1 : 51;
        };
    };
};

struct MSR_PREFETCH_CONTROL
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 L1StreamDisable : 1;
            UINT64 L1StrideDisable : 1;
            UINT64 L1RegionDisable : 1;
            UINT64 L2StreamDisable : 1;
            UINT64 Reserved0 : 1;
            UINT64 UpDownDisable : 1;
            UINT64 Reserved1 : 58;
        };
    };
};

struct MSR_HWCR
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 SmmLock : 1;
            UINT64 Reserved0 : 2;
            UINT64 TlbCacheDis : 1;
            UINT64 InvdWbinvd : 1;
            UINT64 Reserved1 : 2;
            UINT64 AllowFeerOnNe : 1;
            UINT64 ignneEm : 1;
            UINT64 MonMwaitDis : 1;
            UINT64 MonMwaitUserEn : 1;
            UINT64 Reserved2 : 2;
            UINT64 SmiSpCycDis : 1;
            UINT64 RsmSpCycDis : 1;
            UINT64 Reserved3 : 2;
            UINT64 Wrap32Dis : 1;
            UINT64 McStatusWrEn : 1;
            UINT64 Reserved4 : 1;
            UINT64 IoCfgGpFault : 1;
            UINT64 LockTscToCurrentP0 : 1;
            UINT64 Reserved5 : 2;
            UINT64 TscFreqSel : 1;
            UINT64 CpbDis : 1;
            UINT64 EffFreqCntMwait : 1;
            UINT64 EffFreqReadOnlyLock : 1;
            UINT64 Reserved6 : 2;
            UINT64 IRPerfEn : 1;
            UINT64 Reserved7 : 2;
            UINT64 SmmPgCfgLock: 1;
            UINT64 DowGradeFp512ToPF256 : 1;
            UINT64 CpuidUserDis : 1;
            UINT64 Reserved8 : 28;
        };
    };
};

struct MSR_SYS_CFG
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 19;
            UINT64 MtrrFixDramModEn : 1;
            UINT64 MtrrVarDramEn : 1;
            UINT64 MtrrTom2En : 1;
            UINT64 Tom2ForceMemTypeWB : 1;
            UINT64 SMEE : 1;
            UINT64 SecureNestedPagingEn : 1;
            UINT64 VmplEn : 1;
            UINT64 Reserved1 : 38;
        };
    };
};

struct MSR_TOP_MEM
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 23;
            UINT64 TopOfMemory : 25;
            UINT64 Reserved1 : 16;
        };
    };
};

struct MSR_TOM2
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 23;
            UINT64 TopOfMemory2 : 25;
            UINT64 Reserved1 : 16;
        };
    };
};

struct MSR_PSTATE_CURRENT_LIMIT
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 CurPstateLimit : 4;
            UINT64 PstateMaxVal : 4;
            UINT64 : 56;
        };
    };
};

struct MSR_PSTATE_CONTROL
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 PstateCmd : 4;
            UINT64 : 60;
        };
    };
};

struct MSR_PSTATE_STATUS
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 CurPstate : 4;
            UINT64 : 60;
        };
    };
};

struct MSR_PSTATE
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 CpuFid : 12;
            UINT64 Reserved0 : 2;
            UINT64 CpuVid : 8;
            UINT64 IddValue : 8;
            UINT64 IddDiv : 2;
            UINT64 CpuVid_high : 1;
            UINT64 : 30;
            UINT64 PstateEn : 1;
        };
    };
    UINT64 get_frequency_mhz();

    UINT64 get_performance_mhz();
};

struct MSR_CPPC_CAPABILITY_1
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 LowestPerf : 8;
            UINT64 LowNonLinPerf : 8;
            UINT64 NominalPerf : 8;
            UINT64 HighestPerf : 8;
            UINT64 Reserved : 32;
        };
    };
};

struct MSR_CPPC_ENABLE
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 CPPC_En : 1;
            UINT64 : 63;
        };
    };
};

struct MSR_CPPC_CAPABILITY_2
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 MaxPerf : 8;
            UINT64 Reserved : 56;
        };
    };
};

struct MSR_CPPC_REQUEST
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 MaxPerf : 8;
            UINT64 MinPerf : 8;
            UINT64 DesPerf : 8;
            UINT64 EnergyPerfPref : 8;
            UINT64 Reserved : 32;
        };
    };
};

struct MSR_CPPC_STATUS
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 Reserved0 : 1;
            UINT64 MinEx : 1;
            UINT64 Reserved1 : 62;
        };
    };
};

struct MSR_L3_RAPL_POWER_UNIT_0
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 PowerUnits : 4;
            UINT64 Reserved0 : 4;
            UINT64 EnergyStatusUnits : 5;
            UINT64 Reserved1 : 3;
            UINT64 TimeUnits : 4;
            UINT64 Reserved2 : 44;
        };
    };
};

struct MSR_CORE_ENERGY_STAT
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 TotalEnergyConsumed : 64;
        };
    };
};

struct MSR_L3_PACKAGE_ENERGY_STATUS
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 TotalEnergyConsumed : 64;
        };
    };
};

struct MSR_L3_QOS_ABMC_CFG
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 L3CacheLclBwFillMon : 1;
            UINT64 L3CacheRmtBwFillMon : 1;
            UINT64 L3CacheLclBwNtWrMon : 1;
            UINT64 L3CacheRmtBwNtWrMon : 1;
            UINT64 L3CacheLclSlowBwFillMon : 1;
            UINT64 L3CacheRmtSlowBwFillMon : 1;
            UINT64 L3CacheVicBwMon : 1;
            UINT64 Reserved0 : 25;
            UINT64 BwSrc : 12;
            UINT64 Reserved1 : 3;
            UINT64 BwSrcIsClos : 1;
            UINT64 CounterId : 5;
            UINT64 Reserved2 : 9;
            UINT64 EnableCounter : 1;
            UINT64 ConfigureCounter : 1;
        };
    };
};

class MSR
{
public:
    static constexpr UINT32 _MSR_IRPerfCount = 0xC00000E9UL;
    static constexpr UINT32 _MSR_SPEC_CTRL = 0x00000048UL;
    static constexpr UINT32 _MSR_PRED_CMD = 0x00000049UL;
    static constexpr UINT32 _MSR_PATCH_LEVEL = 0x0000008BUL;
    static constexpr UINT32 _MSR_MTRR_CAP = 0x000000FEUL;
    static constexpr UINT32 _MSR_SYSENTER_CS = 0x00000174UL;
    static constexpr UINT32 _MSR_SYSENTER_ESP = 0x00000175UL;
    static constexpr UINT32 _MSR_SYSENTER_EIP = 0x00000176UL;
    static constexpr UINT32 _MSR_MCG_CAP = 0x00000179UL;
    static constexpr UINT32 _MSR_MCG_STATUS = 0x0000017AUL;
    static constexpr UINT32 _MSR_MCG_CTL = 0x0000017BUL;
    static constexpr UINT32 _MSR_DBG_CTL = 0x000001D9UL;
    static constexpr UINT32 _MSR_BR_FROM = 0x000001DBUL;
    static constexpr UINT32 _MSR_BR_TO = 0x000001DCUL;
    static constexpr UINT32 _MSR_LAST_EXCP_FROM_IP = 0x000001DDUL;
    static constexpr UINT32 _MSR_LAST_EXCP_TO_IP = 0x000001DEUL;
    static constexpr UINT32 _MSR_MTRR_DEF_TYPE = 0x000002FFUL;
    static constexpr UINT32 _MSR_U_CET = 0x000006A0UL;
    static constexpr UINT32 _MSR_S_CET = 0x000006A2UL;
    static constexpr UINT32 _MSR_XSS = 0x00000DA0UL;
    static constexpr UINT32 _MSR_APIC_ID = 0x00000802UL;
    static constexpr UINT32 _MSR_APIC_VERSION = 0x00000803UL;
    static constexpr UINT32 _MSR_APIC_TPR = 0x00000808UL;
    static constexpr UINT32 _MSR_APIC_PPR = 0x0000080AUL;
    static constexpr UINT32 _MSR_APIC_EOI = 0x0000080BUL;
    static constexpr UINT32 _MSR_APIC_LDR = 0x0000080DUL;
    static constexpr UINT32 _MSR_APIC_SVR = 0x0000080FUL;
    static constexpr UINT32 _MSR_APIC_ESR = 0x00000828UL;
    static constexpr UINT32 _MSR_APIC_SELF_IPI = 0x0000083FUL;
    static constexpr UINT32 _MSR_FS_BASE = 0xC0000100UL;
    static constexpr UINT32 _MSR_GS_BASE = 0xC0000101UL;
    static constexpr UINT32 _MSR_KERNEL_GS_BASE = 0xC0000102UL;
    static constexpr UINT32 _MSR_TSC_AUX = 0xC0000103UL;
    static constexpr UINT32 _MSR_STAR = 0xC0000081UL;
    static constexpr UINT32 _MSR_VM_CR = 0xC0010114UL;
    static constexpr UINT32 _MSR_LSTAR = 0xC0000082UL;
    static constexpr UINT32 _MSR_CSTAR = 0xC0000083UL;
    static constexpr UINT32 _MSR_SYSCALL_FLAG_MASK = 0xC0000084UL;
    static constexpr UINT32 _MSR_EFER = 0xC0000080UL;
    static constexpr UINT32 _MSR_PAT = 0x00000277UL;
    static constexpr UINT32 _MSR_HSAVE_PA = 0xC0010117UL;
    static constexpr UINT32 _MSR_APIC_BASE = 0x0000001BUL;
    static constexpr UINT32 _MSR_ICR = 0x00000830UL;
    static constexpr UINT32 _MSR_SMBASE = 0xC0010111UL;
    static constexpr UINT32 _MSR_SMM_ADDR = 0xC0010112UL;
    static constexpr UINT32 _MSR_SMM_MASK = 0xC0010113UL;
    static constexpr UINT32 _MSR_TSC_RATIO = 0xC0000104UL;
    static constexpr UINT32 _MSR_PSTATE_CURRENT_LIMIT = 0xC0010061UL;
    static constexpr UINT32 _MSR_PSTATE_CONTROL = 0xC0010062UL;
    static constexpr UINT32 _MSR_PSTATE_STATUS = 0xC0010063UL;
    static constexpr UINT32 _MSR_P0STATE = 0xC0010064UL;
    static constexpr UINT32 _MSR_P1STATE = 0xC0010065UL;
    static constexpr UINT32 _MSR_P2STATE = 0xC0010066UL;
    static constexpr UINT32 _MSR_APERF = 0x000000E8UL;
    static constexpr UINT32 _MSR_MPERF = 0x000000E7UL;
    static constexpr UINT32 _MSR_APERF_READ_ONLY = 0xC00000E8UL;
    static constexpr UINT32 _MSR_MPERF_READ_ONLY = 0xC00000E7UL;
    static constexpr UINT32 _MSR_CPPC_CAPABILITY_1 = 0xC00102B0UL;
    static constexpr UINT32 _MSR_CPPC_ENABLE = 0xC00102B1UL;
    static constexpr UINT32 _MSR_CPPC_CAPABILITY_2 = 0xC00102B2UL;
    static constexpr UINT32 _MSR_CPPC_REQUEST = 0xC00102B3UL;
    static constexpr UINT32 _MSR_CPPC_STATUS = 0xC00102B4UL;
    static constexpr UINT32 _MSR_L3_RAPL_POWER_UNIT_0 = 0xC0010299UL;
    static constexpr UINT32 _MSR_CORE_ENERGY_STAT = 0xC001029AUL;
    static constexpr UINT32 _MSR_L3_PACKAGE_ENERGY_STATUS = 0xC001029BUL;
    static constexpr UINT32 _MSR_L3_QOS_ABMC_CFG = 0xC00003FDUL;
    static constexpr UINT32 _MSR_TSC = 0x00000010UL;
    static constexpr UINT32 _MSR_TSC_DEADLINE = 0x000006E0UL;
    static constexpr UINT32 _MSR_TSC_ADJUST = 0x0000003BUL;
    static constexpr UINT32 _MSR_PREFETCH_CONTROL = 0xC0000108UL;
    static constexpr UINT32 _MSR_SYS_CFG = 0xC0010010UL;
    static constexpr UINT32 _MSR_HWCR = 0xC0010015UL;
    static constexpr UINT32 _MSR_TOP_MEM = 0xC001001AUL;
    static constexpr UINT32 _MSR_TOM2 = 0xC001001DUL;
    static constexpr UINT32 _MSR_2XAPIC_LVT_TIMER = 0x00000832UL;
    static constexpr UINT32 _MSR_2XAPIC_TIMER_INIT_COUNT = 0x00000838UL;
    static constexpr UINT32 _MSR_2XAPIC_TIMER_CUR_COUNT = 0x00000839UL;
    static constexpr UINT32 _MSR_2XAPIC_TIMER_DIV_CONF = 0x0000083EUL;

    static MSR_VM_CR VM_CR();
    static VOID VM_CR(MSR_VM_CR vm_cr);

    static MSR_SPEC_CTRL SPEC_CTRL();
    static VOID SPEC_CTRL(MSR_SPEC_CTRL spec_ctrl);

    static VOID PRED_CMD(MSR_PRED_CMD pred_cmd);

    static UINT32 PATCH_LEVEL();

    static MSR_MTRR_CAP MTRR_CAP();

    static UINT64 SYSENTER_CS();
    static VOID SYSENTER_CS(UINT64 value);

    static UINT64 SYSENTER_ESP();
    static VOID SYSENTER_ESP(UINT64 value);

    static UINT64 SYSENTER_EIP();
    static VOID SYSENTER_EIP(UINT64 value);

    static MSR_EFER EFER();
    static VOID EFER(MSR_EFER efer);

    static MSR_PAT PAT();
    static VOID PAT(MSR_PAT pat);

    static MSR_STAR STAR();
    static VOID STAR(MSR_STAR star);

    static UINT64 LSTAR();
    static VOID LSTAR(UINT64 lstar);

    static UINT64 CSTAR();
    static VOID CSTAR(UINT64 cstar);

    static MSR_SYSCALL_FLAG_MASK SYSCALL_FLAG_MASK();
    static VOID SYSCALL_FLAG_MASK(MSR_SYSCALL_FLAG_MASK mask);

    static UINT64 FS_BASE();
    static VOID FS_BASE(UINT64 fs_base);

    static UINT64 GS_BASE();
    static VOID GS_BASE(UINT64 gs_base);

    static UINT64 KERNEL_GS_BASE();
    static VOID KERNEL_GS_BASE(UINT64 kernel_gs_base);

    static MSR_TSC_AUX TSC_AUX();
    static VOID TSC_AUX(MSR_TSC_AUX aux);

    static UINT64 HSAVE_PA();
    static VOID HSAVE_PA(UINT64 hsave_pa);

    static MSR_APIC_BASE APIC_BASE();
    static VOID APIC_BASE(MSR_APIC_BASE apic_base);

    static MSR_ICR ICR();
    static VOID ICR(MSR_ICR icr);

    static UINT64 SMBASE();

    static MSR_SMM_BASE SMM_ADDR();

    static MSR_SMM_MASK SMM_MASK();

    static MSR_MCG_CAP MCG_CAP();

    static MSR_MCG_STATUS MCG_STATUS();
    static VOID MCG_STATUS(MSR_MCG_STATUS status);

    static UINT64 MCG_CTL();
    static VOID MCG_CTL(UINT64 control);

    static MSR_DBG_CTL DBG_CTL();
    static VOID DBG_CTL(MSR_DBG_CTL debug_control);

    static UINT64 BR_FROM();
    static UINT64 BR_TO();
    static UINT64 LAST_EXCP_FROM_IP();
    static UINT64 LAST_EXCP_TO_IP();

    static MSR_MTRR_DEF_TYPE MTRR_DEF_TYPE();
    static VOID MTRR_DEF_TYPE(MSR_MTRR_DEF_TYPE data);

    static UINT64 U_CET();
    static VOID U_CET(UINT64 data);

    static UINT64 S_CET();
    static VOID S_CET(UINT64 data);

    static MSR_XSS XSS();
    static VOID XSS(MSR_XSS data);

    static MSR_TSC_RATIO TSC_RATIO();

    static MSR_PSTATE_CURRENT_LIMIT PSTATE_CURRENT_LIMIT();

    static MSR_PSTATE_CONTROL PSTATE_CONTROL();

    static VOID PSTATE_CONTROL(MSR_PSTATE_CONTROL data);

    static MSR_PSTATE_STATUS PSTATE_STATUS();

    static UINT64 IRPerfCount();

    static UINT64 APERF();

    static UINT64 MPERF();

    static VOID APERF(UINT64 data);

    static VOID MPERF(UINT64 data);

    static UINT64 APERF_READ_ONLY();

    static UINT64 MPERF_READ_ONLY();

    static MSR_CPPC_CAPABILITY_1 CPPC_CAPABILITY_1();

    static MSR_CPPC_ENABLE CPPC_ENABLE();

	static void CPPC_ENABLE(MSR_CPPC_ENABLE enable);

    static MSR_CPPC_CAPABILITY_2 CPPC_CAPABILITY_2();

    static MSR_CPPC_REQUEST CPPC_REQUEST();

    static void CPPC_REQUEST(MSR_CPPC_REQUEST request);

    static MSR_CPPC_STATUS CPPC_STATUS();

    static MSR_L3_QOS_ABMC_CFG L3_QOS_ABMC_CFG();

    static MSR_L3_RAPL_POWER_UNIT_0 L3_RAPL_POWER_UNIT_0();

    static MSR_CORE_ENERGY_STAT CORE_ENERGY_STAT();

    static MSR_L3_PACKAGE_ENERGY_STATUS L3_PACKAGE_ENERGY_STATUS();

    static MSR_PSTATE PSTATE(int level);

    static UINT64 TSC();
    static VOID TSC(UINT64 tsc);

    static UINT64 TSC_DEADLINE();
    static VOID TSC_DEADLINE(UINT64 deadline);

    static UINT64 TSC_ADJUST();
    static VOID TSC_ADJUST(UINT64 adjust);

    static UINT64 APIC_ID();
    static UINT64 APIC_VERSION();
    static UINT64 APIC_TPR();
    static VOID APIC_TPR(UINT64 value);
    static UINT64 APIC_PPR();
    static VOID APIC_EOI();
    static UINT64 APIC_LDR();
    static UINT64 APIC_SVR();
    static VOID APIC_SVR(UINT64 value);
    static UINT64 APIC_ESR();
    static VOID APIC_ESR(UINT64 value);
    static VOID APIC_SELF_IPI(UINT64 value);

    static UINT64 APIC_LVT_TIMER();
    static VOID APIC_LVT_TIMER(UINT64 lvt_timer);

    static UINT64 APIC_TIMER_INIT_COUNT();
    static VOID APIC_TIMER_INIT_COUNT(UINT64 init_count);

    static UINT64 APIC_TIMER_CUR_COUNT();

    static UINT64 APIC_TIMER_DIV_CONF();
    static VOID APIC_TIMER_DIV_CONF(UINT64 div_conf);

    static MSR_PREFETCH_CONTROL PREFETCH_CONTROL();
    static VOID PREFETCH_CONTROL(MSR_PREFETCH_CONTROL control);

    static MSR_SYS_CFG SYS_CFG();
    static VOID SYS_CFG(MSR_SYS_CFG sys_cfg);

    static MSR_HWCR HWCR();
    static VOID HWCR(MSR_HWCR hwcr);

    static MSR_TOP_MEM TOP_MEM();
    static VOID TOP_MEM(MSR_TOP_MEM top_mem);

    static MSR_TOM2 TOM2();
    static VOID TOM2(MSR_TOM2 tom2);
};
