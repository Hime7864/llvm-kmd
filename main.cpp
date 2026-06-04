#include <intrinsics.hpp>

struct NMI_CALLBACK_DATA
{
    UINT64 cnt;
    UINT64 delta;
    bool switch_flag;
};

struct DISPATCH_RECORD
{
    _In_ UINT32 Iteration;
    _In_ UINT32 BatchCount;
    _In_ UINT32 Timeout;
    _In_ UINT64 IrperfDeltaAvg;
    _In_ UINT64 IrperfDeltaRange;
    _In_ UINT64 SmiStormingThreshold;
};

bool is_amd_cpu()
{
    //int cpu_info[4];
    //__cpuid(cpu_info, 0);
    //return cpu_info[0] >= 0x80000000 && cpu_info[1] == 'htuA';

    auto cpu_info = CPUID::query(0x00000000UL);
    return cpu_info.ebx == 'htuA';
}

namespace AMD
{
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
        MSRC001_0015 [Hardware Configuration] (Core::X86::Msr::HWCR)
        */
    struct MSR_HWCR
    {
        union
        {
            UINT64 AsUINT64;
            struct
            {
                UINT64 SmmLock : 1;
                UINT64 Reserved0 : 2;
                UINT64 TlbCacheDis : 1;
                UINT64 InvdWbinvd : 1;
                UINT64 Reserved1 : 2;
                UINT64 AllowFeerOnNe : 1;
                UINT64 ignneEm : 1;
                UINT64 MonMwaitDis : 1;
                UINT64 MonMwaitUserEn : 1;
                UINT64 Reserved2 : 2;
                UINT64 SmiSpCycDis : 1;
                UINT64 RsmSpCycDis : 1;
                UINT64 Reserved3 : 2;
                UINT64 Wrap32Dis : 1;
                UINT64 McStatusWrEn : 1;
                UINT64 Reserved4 : 1;
                UINT64 IoCfgGpFault : 1;
                UINT64 LockTscToCurrentP0 : 1;
                UINT64 Reserved5 : 2;
                UINT64 TscFreqSel : 1;
                UINT64 CpbDis : 1;
                UINT64 EffFreqCntMwait : 1;
                UINT64 EffFreqReadOnlyLock : 1;
                UINT64 Reserved6 : 2;
                UINT64 IRPerfEn : 1;
                /*
                    IRPerfEn: enable instructions retired counter. Read-write. Reset: 0. 1=Enable Core::X86::Msr::IRPerfCount.
                    */
                UINT64 Reserved7 : 2;
                UINT64 SmmPgCfgLock : 1;
                UINT64 DowGradeFp512ToPF256 : 1;
                UINT64 CpuidUserDis : 1;
                UINT64 Reserved8 : 28;
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

    struct CPUID2
    {
        static constexpr UINT32 _CPUID_FeatureExtIdEbx = 0x80000008UL;
        static constexpr UINT32 _CPUID_ExtTopEnumEdx = 0x0000000BUL;

        static CPUID_FeatureExtIdEbx NOINLINE FeatureExtIdEbx()
        {
            //CPUID_FeatureExtIdEbx result;
            //int cpu_info[4];
            //__cpuidex(cpu_info, _CPUID_FeatureExtIdEbx, 0);
            //result.AsUINT32 = cpu_info[1];
            //return result;

            auto cpu_info = CPUID::query(_CPUID_FeatureExtIdEbx, 0);
            CPUID_FeatureExtIdEbx result;
            result.AsUINT32 = cpu_info.ebx;
            return result;
        }

        static CPUID_ExtTopEnumEdx NOINLINE ExtTopEnumEdx()
        {
            //CPUID_ExtTopEnumEdx result;
            //int cpu_info[4];
            //__cpuidex(cpu_info, _CPUID_ExtTopEnumEdx, 0);
            //result.AsUINT32 = cpu_info[3];
            //return result;

            auto cpu_info = CPUID::query(_CPUID_ExtTopEnumEdx, 0);
            CPUID_ExtTopEnumEdx result;
            result.AsUINT32 = cpu_info.edx;
            return result;
        }
    };

    struct MSR
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
        int delta_cnt = 15014; // this number needs to be changed depending on how this is compiled
        auto apicId = CPUID2::ExtTopEnumEdx().ExtendedLocalApicId;
        auto callback_data = &((NMI_CALLBACK_DATA*)CallbackContext)[apicId];

        if (MSR::HWCR().IRPerfEn)
        {
            auto irperf_0 = MSR::IRPerfCount().IrPerfCount;

            for (int i = 0; i < 12500; ++i)
                _mm_pause(); // #SMI will tend to hit in here

            // ensure all instructions have retired, although this msr read should sanitize anyhow
            _mm_mfence();
            _mm_lfence();

            auto irperf_1 = MSR::IRPerfCount().IrPerfCount;

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

    NTSTATUS BroadcastNmiAndRecord(
        DISPATCH_RECORD* record)
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

    NTSTATUS Service()
    {
        // Check if the processor supports instructions retired MSR
        if (!CPUID2::FeatureExtIdEbx().InstRetCntMsr)
        {
            DbgPrintEx(0, 0, "[smm-dtc] Instructions Retired MSR not supported on this processor.\n");
            return UNSUPPORTED_PROCESSOR;
        }

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
}; // namespace AMD

bool is_intel_cpu()
{
    //int cpu_info[4];
    //__cpuid(cpu_info, 0);
    //return cpu_info[0] >= 1 && cpu_info[1] == 'uneG';

    auto cpu_info = CPUID::query(0x00000000UL);
    return cpu_info.ebx == 'uneG';
}

namespace Intel
{
    struct MSR_FIXED_CTR0
    {
        union
        {
            UINT64 AsUINT64;
            struct
            {
                UINT64 INST_RETIRED : 64;
            };
        };
    };

    struct MSR_PERF_GLOBAL_CTRL
    {
        union
        {
            UINT64 AsUINT64;
            struct
            {
                UINT64 IA32_PMC0 : 1;
                UINT64 IA32_PMC1 : 1;
                UINT64 Reserved0 : 30;
                UINT64 IA32_FIXED_CTR0 : 1;
                UINT64 IA32_FIXED_CTR1 : 1;
                UINT64 IA32_FIXED_CTR2 : 1;
                UINT64 Reserved1 : 29;
            };
        };
    };

    struct MSR_FIXED_CTR_CTRL
    {
        union
        {
            UINT64 AsUINT64;
            struct
            {
                UINT64 Cntr0_enable : 1;
                UINT64 Cntr0_ring : 2;
                UINT64 Cntr0_pmi : 1;
                UINT64 Cntr1_enable : 1;
                UINT64 Cntr1_ring : 2;
                UINT64 Cntr1_pmi : 1;
                UINT64 Cntr2_enable : 1;
                UINT64 Cntr2_ring : 2;
                UINT64 Cntr2_pmi : 1;
                UINT64 Reserved0 : 56;
            };
        };
    };

    struct MSR
    {
        static constexpr UINT32 IA32_FIXED_CTR_CTRL = 0x0000038DUL;
        static constexpr UINT32 IA32_PERF_GLOBAL_CTRL = 0x0000038FUL;
        static constexpr UINT32 IA32_FIXED_CTR0 = 0x00000309UL;

        static MSR_FIXED_CTR_CTRL NOINLINE FIXED_CTR_CTRL()
        {
            MSR_FIXED_CTR_CTRL result;
            result.AsUINT64 = __readmsr(IA32_FIXED_CTR_CTRL);
            return result;
        }

        static void NOINLINE FIXED_CTR_CTRL(MSR_FIXED_CTR_CTRL value)
        {
            __writemsr(IA32_FIXED_CTR_CTRL, value.AsUINT64);
        }

        static MSR_PERF_GLOBAL_CTRL NOINLINE PERF_GLOBAL_CTRL()
        {
            MSR_PERF_GLOBAL_CTRL result;
            result.AsUINT64 = __readmsr(IA32_PERF_GLOBAL_CTRL);
            return result;
        }

        static void NOINLINE PERF_GLOBAL_CTRL(MSR_PERF_GLOBAL_CTRL value)
        {
            __writemsr(IA32_PERF_GLOBAL_CTRL, value.AsUINT64);
        }

        static MSR_FIXED_CTR0 NOINLINE FIXED_CTR0()
        {
            MSR_FIXED_CTR0 result;
            result.AsUINT64 = __readmsr(IA32_FIXED_CTR0);
            return result;
        }
    };

    struct CPUID2
    {
        static bool is_pmu_supported()
        {
            //CPUID.0AH.EAX[7:0] > 0
            //I hate how intel does their documentation couldn't
            //be bothered to put this in a bitfield
            //int cpu_info[4];
            //__cpuid(cpu_info, 0xA);
            //return (cpu_info[0] & 0xFF) > 0;

            auto cpu_info = CPUID::query(0xA);
            return (cpu_info.eax & 0xFF) > 0;

        }
    };

    BOOLEAN NmiCallback(
        _In_ PVOID CallbackContext,
        _In_ BOOLEAN IsHandled)
    {
        _PROCESSOR_NUMBER proc_number;
        KeGetCurrentProcessorNumberEx(&proc_number);
        
        int delta_cnt = 15012; // this number needs to be changed depending on how this is compiled
        auto callback_data = &((NMI_CALLBACK_DATA*)CallbackContext)[proc_number.Number];

        if (MSR::PERF_GLOBAL_CTRL().IA32_FIXED_CTR0)
        {
            _mm_mfence();
            _mm_lfence();
            auto irperf_0 = MSR::FIXED_CTR0().INST_RETIRED;

            for (int i = 0; i < 12500; ++i)
                _mm_pause(); // #SMI will tend to hit in here

            // ensure all instructions have retired
            _mm_mfence();
            _mm_lfence();
            auto irperf_1 = MSR::FIXED_CTR0().INST_RETIRED;

            auto score = ((irperf_1 - irperf_0) - delta_cnt);

            if ((int)score < 10 && (int)score > -10) // intel isn't as clean on AMD 
                score = 0; 

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
    
    NTSTATUS BroadcastInstRetiredEnable()
    {
        auto fixed_ctrl = MSR::FIXED_CTR_CTRL();
        fixed_ctrl.Cntr0_enable = TRUE;
        fixed_ctrl.Cntr0_ring = 3;
        fixed_ctrl.Cntr0_pmi = FALSE;
        MSR::FIXED_CTR_CTRL(fixed_ctrl);

        auto global_ctrl = MSR::PERF_GLOBAL_CTRL();
        global_ctrl.IA32_FIXED_CTR0 = TRUE;
        MSR::PERF_GLOBAL_CTRL(global_ctrl);

       // DbgPrintEx(0, 0, "%i %i %i | %i %i %i | %i %i %i\n",
       //     fixed_ctrl.Cntr0_enable,
       //     fixed_ctrl.Cntr0_ring,
       //     fixed_ctrl.Cntr0_pmi,
       //     fixed_ctrl.Cntr1_enable,
       //     fixed_ctrl.Cntr1_ring,
       //     fixed_ctrl.Cntr1_pmi,
       //     fixed_ctrl.Cntr2_enable,
       //     fixed_ctrl.Cntr2_ring,
       //     fixed_ctrl.Cntr2_pmi);


         //if (!global_ctrl.IA32_FIXED_CTR0)
         //{
         //    
         //    global_ctrl.IA32_FIXED_CTR0 = TRUE;
         //    MSR::PERF_GLOBAL_CTRL(global_ctrl);
         //}
        return STATUS_SUCCESS;
    }

    NTSTATUS BroadcastNmiAndRecord(
        DISPATCH_RECORD* record)
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

    NTSTATUS Service()
    {
        // Check if the processor supports architectural PMU features
        if (!CPUID2::is_pmu_supported())
        {
            DbgPrintEx(0, 0, "[smm-dtc] Architectural PMU features not supported on this processor.\n");
            return UNSUPPORTED_PROCESSOR;
        }

         // Check if the processor has INST_RETIRED enabled, if not enable it
        KeIpiGenericCall(BroadcastInstRetiredEnable, 0);

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
};


NTSTATUS DriverEntry()
{
    if (is_amd_cpu())
    {
        DbgPrintEx(0, 0, "[smm-dtc] AMD processor detected.\n");
        AMD::Service();
    }
    else if (is_intel_cpu())
    {
        DbgPrintEx(0, 0, "[smm-dtc] Intel processor detected.\n");
        Intel::Service();
    }
    else
    {
        DbgPrintEx(0, 0, "[smm-dtc] Unsupported processor vendor.\n");
    }

    return STATUS_SUCCESS;
}
