#pragma once

struct ALIGN(4096) STORAGE
{
	CONTEXT gCtx; // 0x0000
	CONTEXT hCtx; // 0x04D0
	PHYSICAL_ADDRESS vmcb; // 0x09A0
	UINT64 tsc_exit;
	MSR_EFER efer;
	PHYSICAL_ADDRESS hsave;
	UINT64 tsc;
	UINT64 aperf;
	UINT64 mperf;
};

struct ALIGN(4096) VCORE
{
	STORAGE storage; // 0x0000
	BYTE hstack[0x3000]; // 0x1000
	BYTE hsave[0x1000]; // 0x4000
	MSRPM msrpm; // 0x5000
	VMCB vmcb; // 0x6000
};