#pragma once

struct ALIGN(4096) STORAGE
{
	CONTEXT gCtx; // 0x0000
	CONTEXT hCtx; // 0x04D0
	PHYSICAL_ADDRESS vmcb; // 0x09A0
	MSR_EFER efer; // 0x09A8
	PHYSICAL_ADDRESS hsave; // 0x09B0
	UINT64 tsc; // 0x09B8
	UINT64 resync; // 0x09C0
};

struct ALIGN(4096) VCORE
{
	STORAGE storage; // 0x0000
	BYTE hstack[0x2000]; // 0x1000
	BYTE hstackIntr[0x2000]; // 0x3000
	BYTE hsave[0x1000]; // 0x5000
	MSRPM msrpm; // 0x6000
	VMCB vmcb; // 0x8000
	IDT_GATE hIdt[256];// 0x9000
	SEGMENT_DESCRIPTOR hGdt[18];// 0xA000
	INTERUPT_STACK_TABLE hIst;// 0xA120
};