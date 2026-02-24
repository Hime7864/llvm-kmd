#pragma once

struct VCORE;

template <typename TypeDelc>
struct VMEXIT_SHADOW
{
	TypeDelc data;
	UINT64 tsc_write;
	UINT64 tsc_read;

	void set_tsc(VCORE* vCore, bool is_write)
	{
		auto vmcb = &vCore->vmcb;
		auto ca = &vmcb->ControlArea;
		auto storage = &vCore->storage;
		auto gCtx = &storage->gCtx;
		auto ssa = &vmcb->SaveStateArea;
		if (is_write)
			ca->TscOffset -= tsc_write;
		else
			ca->TscOffset -= tsc_read;

		if (gCtx->Rdx == 0xDEAD01)
			tsc_read = ssa->Rax;

		if (gCtx->Rdx == 0xDEAD02)
			tsc_write = ssa->Rax;
		return;
	}
};

struct ALIGN(4096) STORAGE
{
	CONTEXT gCtx; // 0x0000
	CONTEXT hCtx; // 0x04D0
	PHYSICAL_ADDRESS vmcb; // 0x09A0
	UINT64 tsc_first_sight; // 0x09A8
	VMEXIT_SHADOW<MSR_EFER> efer;
	VMEXIT_SHADOW<PHYSICAL_ADDRESS> hsave;
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