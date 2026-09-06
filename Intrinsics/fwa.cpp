#include "fwa.hpp"

BOOLEAN FWA::init = false;
PHYSICAL_MEMORY_RANGE FWA::gRanges[128];
UINT32 FWA::gInBlockIdx[128];
UINT32 FWA::gRangeCnt;
UINT32 FWA::gTotalPages;
UINT32 FWA::gTotalUseage;

volatile bool NAKED FWA::IsPageZero(UINT64 page)
{
    __asm
    {
        vpxor ymm0, ymm0, ymm0
        mov eax, 4096 / 32
        loop:
        vmovdqu ymm1, [rcx]
            vpxor ymm0, ymm0, ymm1
            add rcx, 32
            vptest ymm0, ymm0
            jnz nonzero

            dec eax
            jnz loop

            mov al, 1
            vzeroupper
            ret

            nonzero :
        xor eax, eax
            vzeroupper
            ret
    }
}

volatile UINT32 NOINLINE FWA::GetDriverSize(UINT64 base)
{
    UINT64 hdr_offset = *(BYTE*)(base + 0x3C);
    if (*(UINT16*)base == 0x5A4D &&                  // 'MZ' Hdr
        *(UINT16*)(base + hdr_offset + 4) == 0x8664) // AMD64
    {
        auto sizeofimage = *(UINT32*)(base + hdr_offset + 0x50);
        
        if (sizeofimage & 0xFFF)
            sizeofimage = (sizeofimage & ~0xFFF) + 0x1000;
        return sizeofimage;
    }
    return 0;
}

volatile void FWA::Setup()
{
    if (init)
        return;

    gRangeCnt = 0;
    gTotalPages = 0;
    gTotalUseage = 0;

    bool found_efi = false;

    auto range = MmGetPhysicalMemoryRanges();
    range += 2;
    do
    {
        if (&range[0] && !range[0].NumberOfBytes.QuadPart)
            break;
        if (&range[1] && !range[1].NumberOfBytes.QuadPart)
            break;

        auto high = range[1].BaseAddress.QuadPart - 0x1000;
        auto low = range[0].BaseAddress.QuadPart + range[0].NumberOfBytes.QuadPart + 0x1000;
        auto size = high - low;


        UINT64 pa_head = 0;
        for (UINT64 current = low; current < high; current += 0x1000)
        {
            auto io = (UINT64)MmMapIoSpace(current, 0x1000, MmNonCached);
            if (io)
            {
                _mm_invlpg((PVOID)io);
                auto driver_size = GetDriverSize(io);
                if (driver_size)
                    found_efi = true;
                if (found_efi)
                {
                    if (!IsPageZero(io))
                    {
                        if (pa_head != 0)
                        {
                            if (gRangeCnt < 128)
                            {
                                gRanges[gRangeCnt].BaseAddress.QuadPart = pa_head;
                                gRanges[gRangeCnt].NumberOfBytes.QuadPart = current - pa_head;
                                gTotalPages += (current - pa_head) >> 12;
                                gInBlockIdx[gRangeCnt] = 0;
                                gRangeCnt++;
                            }
                        }

                        if(driver_size)
                            current += driver_size - 0x1000;

                        pa_head = 0;
                        continue;
                    }

                    if (!pa_head)
                        pa_head = current;
                }
                MmUnmapIoSpace((PVOID)io, 0x1000);
            }
        }

        if (found_efi)
        {
            init = true;
            return;
        }

        range++;
    } while (true);
    return;
}

volatile void FWA::ZeroAndExit()
{
    if (!init)
        return;
    init = false;
    for (int i = 0; i < gRangeCnt; i++)
    {
        auto io = MmMapIoSpace(gRanges[i].BaseAddress.QuadPart, gInBlockIdx[i] << 12, MmNonCached);
        if (io)
        {
            RtlFillMemory(io, gInBlockIdx[i] << 12, 0);
            MmUnmapIoSpace(io, gInBlockIdx[i] << 12);
        }
    }
    return;
}

UINT32 FWA::TotalUsed()
{
    FWA::Setup();
    if (!init)
        return 0;
    return gTotalUseage;
}

UINT32 FWA::TotalPages()
{
    FWA::Setup();
    if (!init)
        return 0;
    return gTotalPages;
}

volatile PHYSICAL_ADDRESS FWA::GetPages(UINT32 pages)
{
    FWA::Setup();
    if (!init)
        return 0;
    for (int i = 0; i < gRangeCnt; i++)
    {
        auto block_head = gRanges[i].BaseAddress.QuadPart + (gInBlockIdx[i] << 12);
        auto block_pages = (gRanges[i].NumberOfBytes.QuadPart - (gInBlockIdx[i] << 12)) >> 12;
        if (block_pages >= pages)
        {
            gInBlockIdx[i] += pages;
            gTotalUseage += pages;
            return (PHYSICAL_ADDRESS)block_head;
        }
    }
    return 0;
}