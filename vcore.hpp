#pragma once

struct ALIGN(4096) STORAGE
{
	PHYSICAL_ADDRESS vmcb; // 0x0000
	UINT64 switch_stack; // 0x0008
	UINT64 switch_cr3; // 0x0010


	UINT64 tsc; // 0x0018
	UINT64 mperf; // 0x0020
	UINT64 aperf; // 0x0028
	CONTEXT gCtx; // 0x0030
	CONTEXT hCtx; // 0x0500
};

struct ALIGN(4096) VCORE
{
	STORAGE storage; // 0x0000
	BYTE hstack[0x3000]; // 0x1000
	BYTE hsave[0x1000]; // 0x4000
	MSRPM msrpm; // 0x5000
	VMCB vmcb; // 0x7000
};