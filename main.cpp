#include <intrinsics.hpp>

struct NMI_DATA
{
    UINT64 data;
};

PVOID g_NmiCallbackHandle;
_KAFFINITY_EX* g_NmiAffinity;
NMI_DATA* g_NmiContext;


bool NmiCallback(PVOID ctx, bool handled)
{
    auto coreid = CPUID::current_core_number();
    auto nmi_data = &((NMI_DATA*)ctx)[coreid];
    auto hwcr = MSR::HWCR();
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
        nmi_data->data += !((ircnt_1 - ircnt_0) - offset);
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

    KeInitializeAffinityEx(g_NmiAffinity);
    for (int i=0;i<numCores;i++)
        KeAddProcessorAffinityEx(g_NmiAffinity, i);
    
    printf("This test will take about 20 seconds to complete.\n starting in 3 seconds...\n");
    Sleep(3000);

    for (int i = 0; i < 10; i++)
    {
        printf("Test %i/10\n", i + 1);
        memset(g_NmiContext, 0, nmiContextLength);

        int nmi_count = 50;

        for (int i = 0; i < nmi_count; i++)
        {
            HalSendNMI(g_NmiAffinity);
            Sleep(25);
        }

        int score = nmi_count * numCores;
        int max_score = score;

        for (int i = 0; i < numCores; i++)
        {
            auto nmi_data = &((NMI_DATA*)g_NmiContext)[i];
            score -= nmi_count - nmi_data->data;
            printf("Core %02d: %i/%i\n", i, nmi_data->data, nmi_count);
        }

        printf("Score: %i/%i\n\n", score, max_score);
        Sleep(500);
    }

    


    KeDeregisterNmiCallback(g_NmiCallbackHandle);

    ExFreePoolWithTag(g_NmiAffinity, NMI_CB_POOL_TAG);
    ExFreePoolWithTag(g_NmiContext, NMI_CB_POOL_TAG);


    return STATUS_SUCCESS;
}
