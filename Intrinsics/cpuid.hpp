#pragma once

#include "structures.hpp"

struct CPUID_RESULT
{
    UINT32 eax;
    UINT32 ebx;
    UINT32 ecx;
    UINT32 edx;
};

struct CPUID_EXTENDED_FEATURE_IDENTIFICATION_ECX
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 LahfSahf : 1;
            UINT32 CmpLegacy : 1;
            UINT32 SVM : 1;
            UINT32 ExtApicSpace : 1;
            UINT32 AltMovCr8 : 1;
            UINT32 ABM : 1;
            UINT32 SSE4A : 1;
            UINT32 MisAlignSse : 1;
            UINT32 ThreeDNowPrefetch : 1;
            UINT32 OSVW : 1;
            UINT32 IBS : 1;
            UINT32 XOP : 1;
            UINT32 SKINIT : 1;
            UINT32 WDT : 1;
            UINT32 Reserved0 : 1;
            UINT32 LWP : 1;
            UINT32 FMA4 : 1;
            UINT32 TCE : 1;
            UINT32 Reserved1 : 4;
            UINT32 TopologyExtensions : 1;
            UINT32 PerfCtrExtCore : 1;
            UINT32 PerfCtrExtNb : 1;
            UINT32 Reserved2 : 1;
            UINT32 DataBreakpointExtension : 1;
            UINT32 PerfTsc : 1;
            UINT32 PerfCtrExtL3 : 1;
            UINT32 MwaitExtended : 1;
            UINT32 AdMskExtn : 1;
        };
    };
};

struct CPUID_EXTENDED_FEATURE_IDENTIFICATION_EDX
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 FPU : 1;
            UINT32 VME : 1;
            UINT32 DE : 1;
            UINT32 PSE : 1;
            UINT32 TSC : 1;
            UINT32 MSR : 1;
            UINT32 PAE : 1;
            UINT32 MCE : 1;
            UINT32 CMPXCHG8B : 1;
            UINT32 APIC : 1;
            UINT32 Reserved0 : 1;
            UINT32 SysCallSysRet : 1;
            UINT32 MTRR : 1;
            UINT32 PGE : 1;
            UINT32 MCA : 1;
            UINT32 CMOV : 1;
            UINT32 PAT : 1;
            UINT32 PSE36 : 1;
            UINT32 Reserved1 : 2;
            UINT32 NX : 1;
            UINT32 Reserved2 : 1;
            UINT32 MmxExt : 1;
            UINT32 MMX : 1;
            UINT32 FXSR : 1;
            UINT32 FFXSR : 1;
            UINT32 Page1GB : 1;
            UINT32 RDTSCP : 1;
            UINT32 Reserved3 : 1;
            UINT32 LM : 1;
            UINT32 ThreeDNowExt : 1;
            UINT32 ThreeDNow : 1;
        };
    };
};

struct CPUID_ADDRESS_SIZE_INFORMATION
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 PhysicalAddressBits : 8;
            UINT32 LinearAddressBits : 8;
            UINT32 GuestPhysicalAddressBits : 8;
            UINT32 Reserved : 8;
        };
    };
};

struct CPUID_PROCESSOR_TOPOLOGY
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 ExtendedApicId;
        };
    } eax;

    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 CoreId : 8;
            UINT32 ThreadsPerCore : 8;
            UINT32 NodeId : 8;
            UINT32 NodesPerProcessor : 3;
            UINT32 Reserved : 5;
        };
    } ebx;
};

struct CPUID_STRUCTURED_EXTENDED_FEATURES_EBX
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 FSGSBASE : 1;
            UINT32 Reserved0 : 2;
            UINT32 BMI1 : 1;
            UINT32 Reserved1 : 1;
            UINT32 AVX2 : 1;
            UINT32 Reserved2 : 1;
            UINT32 SMEP : 1;
            UINT32 BMI2 : 1;
            UINT32 Reserved3 : 1;
            UINT32 INVPCID : 1;
            UINT32 Reserved4 : 1;
            UINT32 PQM : 1;
            UINT32 Reserved5 : 2;
            UINT32 PQE : 1;
            UINT32 Reserved6 : 1;
            UINT32 RDSEED : 1;
            UINT32 ADX : 1;
            UINT32 SMAP : 1;
            UINT32 Reserved7 : 2;
            UINT32 CLFLUSHOPT : 1;
            UINT32 CLWB : 1;
            UINT32 Reserved8 : 4;
            UINT32 SHA : 1;
            UINT32 Reserved9 : 2;
        };
    };
};

struct CPUID_SVM_REVISION // Virtual Machine Control Register
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 SvmRev : 8;
            UINT32 Reserved : 24;
        };
    };
};

struct CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION
{
    UINT32 NASID;
};

struct CPUID_SVM_FEATURE_IDENTIFICATION
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 NP : 1;
            UINT32 LbrVirt : 1;
            UINT32 SVML : 1;
            UINT32 NRIPS : 1;
            UINT32 TscRateMsr : 1;
            UINT32 VMCBClean : 1;
            UINT32 FlushByAsid : 1;
            UINT32 DecodeAssists : 1;
            UINT32 Reserved0 : 2;
            UINT32 PauseFilter : 1;
            UINT32 Reserved1 : 1;
            UINT32 PauseFilterThreshold : 19;
        };
    };
};

struct CPUID_FEATURE_EXT_ID_EBX
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 CLZERO : 1;
            UINT32 InstRetCntMsr : 1;
            UINT32 RstrFpErrPtrs : 1;
            UINT32 INVLPGB : 1;
            UINT32 RDPRU : 1;
            UINT32 Reserved0 : 1;
            UINT32 MBE : 1;
            UINT32 Reserved1 : 1;
            UINT32 MCOMMIT : 1;
            UINT32 WBNOINVD : 1;
            UINT32 Reserved2 : 2;
            UINT32 IBPB : 1;
            UINT32 INT_WBINVD : 1;
            UINT32 IBRS : 1;
            UINT32 STIBP : 1;
            UINT32 Reserved3 : 1;
            UINT32 StibpAlwaysOn : 1;
            UINT32 IbrsPreferred : 1;
            UINT32 IbrsProvidesSameModeProtection : 1;
            UINT32 EferLmsleUnsupported : 1;
            UINT32 Reserved4 : 2;
            UINT32 PPIN : 1;
            UINT32 SSBD : 1;
            UINT32 Reserved5 : 2;
            UINT32 CPPC : 1;
            UINT32 PSFD : 1;
            UINT32 BTC_NO : 1;
            UINT32 IBPB_RET : 1;
            UINT32 BranchSample : 1;
        };
    };
};

struct CPUID_APMI
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 TS : 1;
            UINT32 FID : 1;
            UINT32 VID : 1;
            UINT32 TTP : 1;
            UINT32 TM : 1;
            UINT32 : 1;
            UINT32 MHzSteps : 1;
            UINT32 HwPstate : 1;
            /*
            MSRC001_0061 [P-state Current Limit], MSRC001_0062
            [P-state Control] and MSRC001_0063 [P-state Status] exist.
            */
            UINT32 TscInvariant : 1;
            /*
            The TSC rate is ensured to be invariant across all P-States, C-States,
            and stop grant transitions (such as STPCLK Throttling); therefore the TSC is suitable for use as a
            source of time. 0 = No such guarantee is made and software should avoid attempting to use the TSC
            as a source of time.
            */
            UINT32 CPB : 1;
            UINT32 EffFreqRO : 1;
            /*
            EffFreqRO: read-only effective frequency interface. 1=Indicates presence of MSRC000_00E7
            [Read-Only Max Performance Frequency Clock Count (MPerfReadOnly)] and MSRC000_00E8
            [Read-Only Actual Performance Frequency Clock Count (APerfReadOnly)].
            */
            UINT32 ProcFeedbackInterface : 1;
            UINT32 ProcPowerReporting : 1;
            UINT32 ConnectedStandby : 1;
            UINT32 RAPL : 1;
            UINT32 FastCPPC : 1;
            UINT32 Reserved : 16;
        };
    };
};

class CPUID
{
public:
    static CPUID_RESULT query(
        UINT32 leaf,
        UINT32 subleaf = 0);

    static UINT32 max_basic_leaf();

    static UINT32 max_extended_leaf();

    static UINT32 current_apic_id();

    static UINT32 current_core_number();

    static CPUID_SVM_REVISION svm_revision();

    static CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION svm_revision_and_feature_identification();

    static CPUID_SVM_FEATURE_IDENTIFICATION svm_feature_identification();

    static LPSTR vendor_string();

    static LPSTR brand_string();

    static CPUID_APMI apmi_features();

    static CPUID_EXTENDED_FEATURE_IDENTIFICATION_ECX extended_feature_identification_ecx();

    static CPUID_EXTENDED_FEATURE_IDENTIFICATION_EDX extended_feature_identification_edx();

    static CPUID_ADDRESS_SIZE_INFORMATION address_size_information();

    static CPUID_FEATURE_EXT_ID_EBX feature_ext_id_ebx();

    static CPUID_PROCESSOR_TOPOLOGY processor_topology();

    static CPUID_STRUCTURED_EXTENDED_FEATURES_EBX structured_extended_features_ebx();
};

