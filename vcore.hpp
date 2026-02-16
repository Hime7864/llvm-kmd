#pragma once

struct ALIGN(4096) STORAGE
{
	CONTEXT gCtx; // 0x0000
	CONTEXT hCtx; // 0x04D0
	PHYSICAL_ADDRESS vmcb; // 0x09A0
	MSR_EFER efer; // 0x09A8
	PHYSICAL_ADDRESS hsave; // 0x09B0
	UINT64 tsc; // 0x09B8
};

struct ALIGN(4096) VCORE
{
	STORAGE storage; // 0x0000
	BYTE hstack[0x3000]; // 0x1000
	BYTE hsave[0x1000]; // 0x4000
	MSRPM msrpm; // 0x5000
	VMCB vmcb; // 0x7000
	BYTE idt[0x1000]; // 0x8000
};