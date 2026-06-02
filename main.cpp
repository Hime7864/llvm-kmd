#include <intrinsics.hpp>

struct NMI_DATA
{
    UINT64 counter;
    UINT64 score;
    UINT64 hits;
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
            nmi_data->hits++;
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
    
    int iterations = 3;
    int nmi_count = 50;

    auto expected_score = nmi_count * iterations * numCores;
    auto tested_score = 0;

    UINT64 expected_smi_base = 25000;
    UINT64 expected_smi_range = 20000;
    
    bool report_smi_storm = false;
    bool report_unexpected = false;

    for (int i = 0; i < iterations; i++)
    {
        DbgPrintEx(0,0,"%i/%i\n", i + 1, iterations);
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

        UINT64 lower_bound = 0;
        for (int l = 0; l < numCores; l++)
        {
            auto nmi_data = &((NMI_DATA*)g_NmiContext)[l];
            if (nmi_data->score)
            {
                if (nmi_data->hits <= 3)
                    DbgPrintEx(0, 0, "Core %i: %i -> core hit %i times\n", l, nmi_data->score, nmi_data->hits);
                else
                {
                    DbgPrintEx(0, 0, "Core %i: %i -> core hit %i times <- Smi Storming\n", l, nmi_data->score, nmi_data->hits);
                    report_smi_storm = true;
                }
                    
                if (lower_bound == 0)
                    lower_bound = nmi_data->score;
                if (nmi_data->score < lower_bound)
                    lower_bound = nmi_data->score;
            }
        }
        if (lower_bound)
        {
            if (lower_bound < expected_smi_base + expected_smi_range && 
                lower_bound > expected_smi_base - expected_smi_range)
            {
                DbgPrintEx(0, 0, "Expected (Normal)\n");
            }
            else
            {
                DbgPrintEx(0, 0, "Unexpected (Out of Range) <- Very likely SMM is hyjacked\n");
                report_unexpected = true;
            }
        }
        Sleep(500);
    }
    auto percentage = (double)tested_score / expected_score * 100.0;
    if (100 - (int)percentage > 10)
        DbgPrintEx(0, 0, "Nmi's effected: %i%% :`(\n", 100 - (int)percentage);
    else if (100 - (int)percentage)
        DbgPrintEx(0, 0, "Nmi's effected: %i%% :(\n", 100 - (int)percentage);
    else
        DbgPrintEx(0, 0, "Nmi's effected: %i%% :)\n", 100 - (int)percentage);

    DbgPrintEx(0, 0, "Test finished.\n");
    if (report_unexpected && report_smi_storm)
    {
        DbgPrintEx(0, 0, "100% Smm is actively being abused\n");
    }
    else if (report_unexpected)
    {
        DbgPrintEx(0, 0, "Smm very likely is hyjacked\n");
    }
    else if (report_smi_storm)
    {
        DbgPrintEx(0, 0, "Could be faulty hardware/software but #smi is being stormed\n");
    }
    else
    {
        DbgPrintEx(0, 0, "Smm doesn't seem to be hyjacked\n");
    }
    


    KeDeregisterNmiCallback(g_NmiCallbackHandle);

    ExFreePoolWithTag(g_NmiAffinity, NMI_CB_POOL_TAG);
    ExFreePoolWithTag(g_NmiContext, NMI_CB_POOL_TAG);


    return STATUS_SUCCESS;
}
