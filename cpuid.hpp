#pragma once


struct CPUID_SVM_REVISION//Virtual Machine Control Register
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

struct CPUID_APMI
{
	union {
		UINT32 AsUINT32;
		struct {
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
		};
	};
};

class CPUID
{
public:
	//CPUID Fn8000_000A_EAX SVM Revision & Feature Identification
	static constexpr DWORD _CPUID_SVM_REVISION_IDENTIFICATION = 0x8000000AUL;

	static constexpr DWORD _CPUID_PROCESSOR_VENDOR = 0x0000000UL;

	static constexpr DWORD _CPUID_EXTENDED_TOPOLOGY = 0x0000000BU;

	static UINT32 current_core_number();

	static CPUID_SVM_REVISION svm_revision();

	static CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION svm_revision_and_feature_identification();

	static CPUID_SVM_FEATURE_IDENTIFICATION svm_feature_identification();

	static LPSTR vendor_string();

	static CPUID_APMI apmi_features();
};