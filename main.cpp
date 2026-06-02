#include <intrinsics.hpp>

struct NMI_DATA
{
    UINT64 counter;
    UINT64 score;
};

PVOID g_NmiCallbackHandle;
_KAFFINITY_EX* g_NmiAffinity;
NMI_DATA* g_NmiContext;


bool NmiCallback(PVOID ctx, bool handled)
{
    auto coreid = CPUID::current_core_number();
    auto nmi_data = &((NMI_DATA*)ctx)[coreid];
    auto hwcr = MSR::HWCR();
    if (!hwcr.IRPerfEn)
    {
        hwcr.IRPerfEn = 1;
        MSR::HWCR(hwcr);
    }
    hwcr = MSR::HWCR();
    if (hwcr.IRPerfEn)
    {
        int offset = 15012;
        auto ircnt_0 = MSR::IRPerfCount();
        for (int i = 0; i < 12500; ++i)
        {
            _mm_pause();
        }
        _mm_mfence();
        _mm_lfence();
        auto ircnt_1 = MSR::IRPerfCount();
        auto score = ((ircnt_1 - ircnt_0) - offset);
        if (score)
        {
            if (!nmi_data->score)
                nmi_data->score = score;
            else if (score < nmi_data->score)
                nmi_data->score = score;
        }
        else
            nmi_data->counter++;
        
    }
    return TRUE;
}


NTSTATUS DriverEntry()
{
    auto numCores = KeQueryActiveProcessorCount(0);
    
    ULONG nmiContextLength = numCores * sizeof(NMI_DATA);

    g_NmiAffinity = (_KAFFINITY_EX*)ExAllocatePoolWithTag(NonPagedPool, sizeof(_KAFFINITY_EX), NMI_CB_POOL_TAG);
    g_NmiContext = (NMI_DATA*)ExAllocatePoolWithTag(NonPagedPool, nmiContextLength, NMI_CB_POOL_TAG);
    g_NmiCallbackHandle = KeRegisterNmiCallback(NmiCallback, g_NmiContext);

    memset(g_NmiAffinity, 0, sizeof(_KAFFINITY_EX));
    memset(g_NmiContext, 0, nmiContextLength);

    KeInitializeAffinityEx(g_NmiAffinity);
    for (int i=0;i<numCores;i++)
        KeAddProcessorAffinityEx(g_NmiAffinity, i);
    
    int iterations = 10;
    int nmi_count = 50;

    auto expected_score = nmi_count * iterations * numCores;
    auto tested_score = 0;
    
    for (int i = 0; i < iterations; i++)
    {
        printf("%i/%i\n", i + 1, iterations);
        memset(g_NmiContext, 0, nmiContextLength);

        for (int i = 0; i < nmi_count; i++)
        {
            HalSendNMI(g_NmiAffinity);
            Sleep(1);
        }

        for (int i = 0; i < numCores; i++)
        {
            auto nmi_data = &((NMI_DATA*)g_NmiContext)[i];
            tested_score += nmi_data->counter;
        }

        auto lower_bound = 0;
        for (int l = 0; l < numCores; l++)
        {
            auto nmi_data = &((NMI_DATA*)g_NmiContext)[l];
            if (nmi_data->score)
            {
                printf("Core %i: %i\n", l, nmi_data->score);
                if (lower_bound == 0)
                    lower_bound = nmi_data->score;
                if (nmi_data->score < lower_bound)
                    lower_bound = nmi_data->score;
            }
        }
        if (lower_bound)
        printf("Lower bound: %i\n", lower_bound);
        //Sleep(500);
        
    }

    printf("Expected: %d\n", expected_score);
    printf("Tested: %d\n", tested_score);
    auto percentage = (double)tested_score / expected_score * 100.0;
    printf("Percentage dropped: %i\n", 100 - (int)percentage);


    


    KeDeregisterNmiCallback(g_NmiCallbackHandle);

    ExFreePoolWithTag(g_NmiAffinity, NMI_CB_POOL_TAG);
    ExFreePoolWithTag(g_NmiContext, NMI_CB_POOL_TAG);


    return STATUS_SUCCESS;
}
