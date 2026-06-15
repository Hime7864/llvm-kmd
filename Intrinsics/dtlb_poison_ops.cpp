#include "dtlb.hpp"

VOID NAKED DTLB::UpdatePoison(PVOID self)
{
    __asm {
        push rbx
        push rcx
        push rdx

        mov rcx, [rdx]
        mov rdx, [rdx + 8]

        mov rax, [rcx + 0x20]
        mov cr3, rax

        invlpg[rcx]
        invlpg[rdx]

        mov rbx, [rdx]
        xor eax, eax
        mov[rdx], rax
        mov[rdx], rbx

        mov rax, [rcx + 0x28]
        mov cr3, rax

        pop rdx
        pop rcx
        pop rbx
        ret
    }
}

BOOLEAN DTLB::Initialize()
{
    FWA::Initialize();

    UINT64 module_base, module_size;
    Utils::LocateSelf(&module_base, &module_size);

    mmCr3 = FWA::ReservePages(1);

    stack_frame.BaseAddress.QuadPart = FWA::ReservePages(10);
    stack_frame.NumberOfBytes.QuadPart = 0xA000;

    mmStackBase = module_base + module_size;
    oldCr3 = _mm_readcr3();

    for (int i = 0; i < stack_frame.NumberOfBytes.QuadPart >> 12; i++)
    {
        MMPTE_HARDWARE pte;
        pte.AsUINT64 = 0;
        pte.Valid = true;
        pte.Dirty1 = true;
        pte.Write = true;
        pte.Dirty = true;
        pte.NoExecute = true;
        pte.PageFrameNumber = (stack_frame.BaseAddress.QuadPart >> 12) + i;
        HostedCommit4kbMapping(mmCr3, mmStackBase + (i << 12), pte);
    }

    for (int i = 0; i < module_size >> 12; i++)
    {
        auto new_page = FWA::ReservePages(1);
        MMPTE_HARDWARE pte;
        pte.AsUINT64 = 0;
        pte.Valid = true;
        pte.Dirty1 = true;
        pte.Write = true;
        pte.Dirty = true;
        pte.PageFrameNumber = new_page >> 12;
        HostedCommit4kbMapping(mmCr3, module_base + (i << 12), pte);
        auto page_va = HostedGetVirtual(new_page);
        RtlCopyMemory((PVOID)page_va, (PVOID)(module_base + (i << 12)), 4096);
        HostedFreeVirtual(page_va);
    }

    return TRUE;
}

void DTLB::ReserveRvaPoison(LINEAR_ADDRESS rva, PHYSICAL_ADDRESS PoisonedPage)
{
    MMPTE_HARDWARE pte;
    pte.AsUINT64 = 0;
    pte.Valid = true;
    pte.Owner = true;
    pte.Global = true;
    pte.Dirty1 = true;
    pte.Write = true;
    pte.PageFrameNumber = PoisonedPage >> 12;

    HostedCommit4kbMapping(mmCr3, rva, pte);
    return;
}

void DTLB::CommitRvaPoison(LINEAR_ADDRESS rva)
{
    struct IPI_DATA
    {
        PVOID self;
        UINT64 target_address;
    };
    IPI_DATA data;

    data.self = this;
    data.target_address = rva.AsUINT64;
    auto irql = _mm_readcr8();
    _mm_writecr8(15);
    UpdatePoison(&data);
    _mm_writecr8(irql);
    return;
}

void DTLB::Cleanup()
{
    FWA::Cleanup();
    return;
}

DTLB dTlbPoison;