#include <intrinsics.hpp>
struct NMI_CALLBACK_DATA
{
    UINT64 cnt;
    UINT64 delta;
    bool switch_flag;
};

/*
57930-A0-PUB_3.00.pdf
CPUID_Fn80000008_EBX [Extended Feature Extensions ID EBX] (Core::X86::Cpuid::FeatureExtIdEbx)
*/
struct CPUID_FeatureExtIdEbx
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 CLZERO : 1;
            UINT32 InstRetCntMsr : 1;
            /*
            InstRetCntMsr: instructions retired count support. Read-only. Reset: Fixed,1.
            1=Core::X86::Msr::IRPerfCount supported.
            */
            UINT32 RstrFpErrPtrs : 1;
            UINT32 INVLPGB : 1;
            UINT32 RDPRU : 1;
            UINT32 Reserved0 : 1;
            UINT32 MBE : 1;
            UINT32 Reserved1 : 1;
            UINT32 MCOMMIT : 1;
            UINT32 WBNOINVD : 1;
            UINT32 Reserved2 : 1;
            UINT32 IBPB : 1;
            UINT32 INT_WBINVD : 1;
            UINT32 IBRS : 1;
            UINT32 STIBP : 1;
            UINT32 Reserved3 : 1;
            UINT32 StibpAlwaysOn : 1;
            UINT32 IbrsPreferred : 1;
            UINT32 IbrsProvidesSameModeProtection : 1;
            UINT32 EferLmsleUnsupported : 1;
            UINT32 Reserved4 : 2;
            UINT32 PPIN : 1;
            UINT32 SSBD : 1;
            UINT32 Reserved5 : 2;
            UINT32 CPPC : 1;
            UINT32 PSFD : 1;
            UINT32 BTC_NO : 1;
            UINT32 IBPB_RET : 1;
            UINT32 BranchSample : 1;
        };
    };
};

/*
57930-A0-PUB_3.00.pdf
CPUID_Fn0000000B_EDX [Extended Topology Enumeration] (Core::X86::Cpuid::ExtTopEnumEdx)
*/
struct CPUID_ExtTopEnumEdx
{
    union
    {
        UINT32 AsUINT32;
        struct
        {
            UINT32 ExtendedLocalApicId : 32;
        };
    };
};


/*
57930-A0-PUB_3.00.pdf
MSRC000_00E9 [Instructions Retired Performance Count] (Core::X86::Msr::IRPerfCount)
*/
struct MSR_IPRerfCounter
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            UINT64 IrPerfCount : 48;
            /* 
            IRPerfCount: instructions retired counter. Reset: 0000_0000_0000h. Dedicated Instructions Retired register
            increments on once for every instruction retired. See Core::X86::Msr::HWCR[IRPerfEn].
            AccessType: Core::X86::Msr::HWCR[EffFreqReadOnlyLock] ? Read,Error-on-write,Volatile : Readwrite,Volatile
            */
            UINT64 Reserved : 16;
        };
    };
};


struct MSR2
{
    static constexpr UINT32 _MSR_IRPerfCount = 0xC00000E9UL;
    static constexpr UINT32 _MSR_HWCR = 0xC0010015UL;

    static MSR_IPRerfCounter NOINLINE IRPerfCount()
    {
        //return { .AsUINT64 = __readmsr(_MSR_IRPerfCount) }; mscv sucks
        MSR_IPRerfCounter result;
        result.AsUINT64 = __readmsr(_MSR_IRPerfCount);
        return result;
    }

    static MSR_HWCR NOINLINE HWCR()
    {
        //return { .AsUINT64 = __readmsr(_MSR_HWCR) }; mscv sucks
        MSR_HWCR result;
        result.AsUINT64 = __readmsr(_MSR_HWCR);
        return result;
    }

    static void NOINLINE HWCR(MSR_HWCR value)
    {
        __writemsr(_MSR_HWCR, value.AsUINT64);
    }
};


BOOLEAN NmiCallback(
    _In_ PVOID CallbackContext,
    _In_ BOOLEAN IsHandled)
{
    int delta_cnt = 15014;
    auto apicId = CPUID::current_core_number();
    auto callback_data = &((NMI_CALLBACK_DATA*)CallbackContext)[apicId];
    
    if (MSR::HWCR().IRPerfEn)
    {
        auto irperf_0 = MSR2::IRPerfCount().IrPerfCount;
    
        for (int i = 0; i < 12500; ++i)
            _mm_pause(); // #SMI will tend to hit in here
    
        // ensure all instructions have retired, although this msr read should sanitize anyhow
        _mm_mfence();
        _mm_lfence();
    
        auto irperf_1 = MSR2::IRPerfCount().IrPerfCount;
    
        auto score = ((irperf_1 - irperf_0) - delta_cnt);
        if (score) // if IRperf reports more than expected we caught a smi
        {
            /* 
            since each core enter smm there is a wait time 
            for each core to enter at about the same time 
            so some cores run more than others since they 
            are all spinning to syncing getting the lowest delta
            should be the around one of the last core to be serviced 
            */
            if (!callback_data->delta)
                callback_data->delta = score;
            else if (score < callback_data->delta)
                callback_data->delta = score;
        }
        else
            callback_data->cnt++;
    }
    else
    {
        /*
        to catch if smm is setting irperfen bit 
        there is no reason they should do this unless
		they want to hide smm entry/exit from software
        */
        callback_data->switch_flag = true;
    }

    return TRUE;
}

struct DISPATCH_RECORD
{
    _In_ UINT32 Iteration;
    _In_ UINT32 BatchCount;
    _In_ UINT32 Timeout;
    _In_ UINT64 IrperfDeltaAvg;
    _In_ UINT64 IrperfDeltaRange;
    _In_ UINT64 SmiStormingThreshold;
};

NTSTATUS BroadcastNmiAndRecord(DISPATCH_RECORD* record)
{
    auto numCores = KeQueryActiveProcessorCount(0);

    ULONG nmiCtxLen = numCores * sizeof(NMI_CALLBACK_DATA);

    auto affinity = (_KAFFINITY_EX*)ExAllocatePoolWithTag(NonPagedPool, sizeof(_KAFFINITY_EX), NMI_CB_POOL_TAG);
    if (!affinity)
    {
        DbgPrintEx(0, 0, "[smm-dtc] Failed to allocate memory for NMI affinity.\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    auto ctx = (NMI_CALLBACK_DATA*)ExAllocatePoolWithTag(NonPagedPool, nmiCtxLen, NMI_CB_POOL_TAG);
    if (!ctx)
    {
        ExFreePoolWithTag(affinity, NMI_CB_POOL_TAG);
        DbgPrintEx(0, 0, "[smm-dtc] Failed to allocate memory for NMI callback context.\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    auto handle = KeRegisterNmiCallback(NmiCallback, ctx);
    if (!handle)
    {
        ExFreePoolWithTag(affinity, NMI_CB_POOL_TAG);
        ExFreePoolWithTag(ctx, NMI_CB_POOL_TAG);
        DbgPrintEx(0, 0, "[smm-dtc] Failed to register NMI callback.\n");
        return STATUS_UNSUCCESSFUL;
    }

    memset(ctx, 0, nmiCtxLen);
    memset(affinity, 0, sizeof(_KAFFINITY_EX));

    KeInitializeAffinityEx(affinity);
    for (int i = 0; i < numCores; i++)
        KeAddProcessorAffinityEx(affinity, i);

    UINT64 expected_counter = numCores * record->Iteration * record->BatchCount;
    UINT64 resulted_counter = 0;
    
    for (int i = 0; i < record->Iteration; i++)
    {
        memset(ctx, 0, nmiCtxLen);
        DbgPrintEx(0, 0, "[smm-dtc] %i/%i\n", i + 1, record->Iteration);

        for (int i = 0; i < record->BatchCount; i++)
        {
            HalSendNMI(affinity);
            Sleep(1);
        }

        bool dtc_storming = false;

        UINT64 LowestIrperfDelta = 0;
        for (int l = 0; l < numCores; l++)
        {
            bool tripped = false;
            auto callback_data = &((NMI_CALLBACK_DATA*)ctx)[l];

            DbgPrintEx(0, 0, "   Core[%02i]\n", l);

            resulted_counter += callback_data->cnt;

            if (callback_data->switch_flag)
            {
                tripped = true;
                DbgPrintEx(0, 0, "      * IRPerfEn bit was switched off <- Very Suspicious\n");
            }

            if (callback_data->delta)
            {
                tripped = true;
                auto smi_captured = record->BatchCount - callback_data->cnt;
                DbgPrintEx(0, 0, "      * smi captured %i - reported delta %i\n", smi_captured, callback_data->delta);
                if (smi_captured >= record->SmiStormingThreshold)
                {
                    DbgPrintEx(0, 0, "         > SmiStormingThreshold Met");
                    dtc_storming = true;
                }

                // capture lowest of batch
                if (LowestIrperfDelta == 0)
                    LowestIrperfDelta = callback_data->delta;
                if (callback_data->delta < LowestIrperfDelta)
                    LowestIrperfDelta = callback_data->delta;
            }

            if (!tripped)
                DbgPrintEx(0, 0, "      * Normal\n", l);
        }

        if (LowestIrperfDelta)
        {
            if (LowestIrperfDelta < record->IrperfDeltaAvg + record->IrperfDeltaRange &&
                LowestIrperfDelta > record->IrperfDeltaAvg - record->IrperfDeltaRange)
            {
                DbgPrintEx(0, 0, "   * IrperfDelta within expected range\n");
                if (dtc_storming)
                {
                    DbgPrintEx(0, 0, "      > Smi Storming may be a result of faulty hardware or software\n");
                }
            }
            else
            {
                DbgPrintEx(0, 0, "   * IrperfDelta falls outside expected range <- Very Suspicious\n");
            }
        }
        
        Sleep(record->Timeout);
    }
    
    KeDeregisterNmiCallback(handle);

    ExFreePoolWithTag(affinity, NMI_CB_POOL_TAG);
    ExFreePoolWithTag(ctx, NMI_CB_POOL_TAG);
    return STATUS_SUCCESS;
}

NTSTATUS BroadcastIRPerfEnable()
{
    auto hwcr = MSR::HWCR();
    if (!hwcr.IRPerfEn)
    {
        hwcr.IRPerfEn = TRUE;
        MSR::HWCR(hwcr);
    }
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry()
{
    
    // Check if the processor has IRPerfCount enabled, if not enable it
    KeIpiGenericCall(BroadcastIRPerfEnable, 0);

    auto record = (DISPATCH_RECORD*)ExAllocatePoolWithTag(NonPagedPool, sizeof(DISPATCH_RECORD), NMI_CB_POOL_TAG);
    if (!record)
    {
        DbgPrintEx(0, 0, "[smm-dtc] Failed to allocate memory for DISPATCH_RECORD.\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    // Setting for detection
    record->BatchCount = 50;
    record->Iteration = 3;
    record->Timeout = 1000;           // ms
    record->IrperfDeltaAvg = 25000;   // limited on hardware but this is about what is expected
    record->IrperfDeltaRange = 20000; // examples i've tested with SmmInfect show delta more than 10x this variance
    record->SmiStormingThreshold = 3; // smi do happen however post boot, however they should be very rare

    if (!NT_SUCCESS(BroadcastNmiAndRecord(record)))
        DbgPrintEx(0, 0, "[smm-dtc] Failed to broadcast NMI and record results.\n");

    ExFreePoolWithTag(record, NMI_CB_POOL_TAG);


    return STATUS_SUCCESS;
}
