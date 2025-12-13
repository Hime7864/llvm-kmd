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

class CPUID
{
public:
	//CPUID Fn8000_000A_EAX SVM Revision & Feature Identification
	static constexpr DWORD _CPUID_SVM_REVISION_IDENTIFICATION = 0x8000000AUL;

	static constexpr DWORD _CPUID_PROCESSOR_VENDOR = 0x0000000UL;

	static constexpr DWORD _CPUID_EXTENDED_TOPOLOGY = 0x0000000BU;
	
	static UINT32 CurrentCoreNumber();

	static CPUID_SVM_REVISION SvmRevision();

	static CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION SvmRevisionAndFeatureIdentification();

	static CPUID_SVM_FEATURE_IDENTIFICATION SvmFeatureIdentification();

	static LPSTR VendorString();
};