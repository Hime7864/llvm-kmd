#pragma once

#include "structures.hpp"
#include "imports.hpp"
#include "assembly.hpp"

class FWA
{
private:
    static BOOLEAN init;
    static PHYSICAL_MEMORY_RANGE gRanges[128];
    static UINT32 gInBlockIdx[128];
    static UINT32 gRangeCnt;
    static UINT32 gTotalPages;
    static UINT32 gTotalUseage;

    volatile static bool NAKED IsPageZero(UINT64 page);

    volatile static UINT32 GetDriverSize(UINT64 base);

    volatile static void Setup();
public:

    volatile static void ZeroAndExit();

    static UINT32 TotalUsed();

    static UINT32 TotalPages();

    volatile static PHYSICAL_ADDRESS GetPages(UINT32 pages);
};
