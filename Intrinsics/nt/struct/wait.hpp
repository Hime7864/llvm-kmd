#pragma once

#include "../fwd.hpp"

#include "base.hpp"

struct _DISPATCHER_HEADER
{
    union
    {
        INT32 Lock; // 0x0
        struct
        {
            UINT8 Type;       // 0x0
            UINT8 Signalling; // 0x1
            UINT8 Size;       // 0x2
            UINT8 Reserved1;  // 0x3
        };
        struct
        {
            UINT8 TimerType; // 0x0
            union
            {
                UINT8 TimerControlFlags; // 0x1
                struct
                {
                    UINT8 Absolute : 1;              // 0x1
                    UINT8 Wake : 1;                  // 0x1
                    UINT8 EncodedTolerableDelay : 6; // 0x1
                };
            };
            UINT8 Hand; // 0x2
            union
            {
                UINT8 TimerMiscFlags; // 0x3
                struct
                {
                    UINT8 Index : 6;            // 0x3
                    UINT8 Inserted : 1;         // 0x3
                    volatile UINT8 Expired : 1; // 0x3
                };
            };
        };
        struct
        {
            UINT8 Timer2Type; // 0x0
            union
            {
                UINT8 Timer2Flags; // 0x1
                struct
                {
                    UINT8 Timer2Inserted : 1;      // 0x1
                    UINT8 Timer2Expiring : 1;      // 0x1
                    UINT8 Timer2CancelPending : 1; // 0x1
                    UINT8 Timer2SetPending : 1;    // 0x1
                    UINT8 Timer2Running : 1;       // 0x1
                    UINT8 Timer2Disabled : 1;      // 0x1
                    UINT8 Timer2ReservedFlags : 2; // 0x1
                };
            };
            UINT8 Timer2ComponentId; // 0x2
            UINT8 Timer2RelativeId;  // 0x3
        };
        struct
        {
            UINT8 QueueType; // 0x0
            union
            {
                UINT8 QueueControlFlags; // 0x1
                struct
                {
                    UINT8 Abandoned : 1;                 // 0x1
                    UINT8 DisableIncrement : 1;          // 0x1
                    UINT8 QueueReservedControlFlags : 6; // 0x1
                };
            };
            UINT8 QueueSize;     // 0x2
            UINT8 QueueReserved; // 0x3
        };
        struct
        {
            UINT8 ThreadType;     // 0x0
            UINT8 ThreadReserved; // 0x1
            union
            {
                UINT8 ThreadControlFlags; // 0x2
                struct
                {
                    UINT8 CycleProfiling : 1;             // 0x2
                    UINT8 CounterProfiling : 1;           // 0x2
                    UINT8 GroupScheduling : 1;            // 0x2
                    UINT8 AffinitySet : 1;                // 0x2
                    UINT8 Tagged : 1;                     // 0x2
                    UINT8 EnergyProfiling : 1;            // 0x2
                    UINT8 SchedulerAssist : 1;            // 0x2
                    UINT8 ThreadReservedControlFlags : 1; // 0x2
                };
            };
            union
            {
                UINT8 DebugActive; // 0x3
                struct
                {
                    UINT8 ActiveDR7 : 1;    // 0x3
                    UINT8 Instrumented : 1; // 0x3
                    UINT8 Minimal : 1;      // 0x3
                    UINT8 Reserved4 : 2;    // 0x3
                    UINT8 AltSyscall : 1;   // 0x3
                    UINT8 Emulation : 1;    // 0x3
                    UINT8 Reserved5 : 1;    // 0x3
                };
            };
        };
        struct
        {
            UINT8 MutantType;     // 0x0
            UINT8 MutantSize;     // 0x1
            UINT8 DpcActive;      // 0x2
            UINT8 MutantReserved; // 0x3
        };
    };
    INT32 SignalState;       // 0x4
    LIST_ENTRY WaitListHead; // 0x8
};

// 0x18 bytes (sizeof)
struct _KEVENT
{
    DISPATCHER_HEADER Header; // 0x0
};

// 0x38 bytes (sizeof)
struct _KMUTANT
{
    DISPATCHER_HEADER Header;   // 0x0
    LIST_ENTRY MutantListEntry; // 0x18
    PKTHREAD OwnerThread;       // 0x28
    BOOLEAN Abandoned;          // 0x30
    UCHAR ApcDisable;           // 0x31
};

// 0x20 bytes (sizeof)
struct _KSEMAPHORE
{
    DISPATCHER_HEADER Header; // 0x0
    LONG Limit;               // 0x18
};

// 0x38 bytes (sizeof)
struct _FAST_MUTEX
{
    LONG Count;       // 0x0
    PKTHREAD Owner;   // 0x8
    ULONG Contention; // 0x10
    KEVENT Event;     // 0x18
    ULONG OldIrql;    // 0x30
};

// 0x18 bytes (sizeof)
struct _KDEVICE_QUEUE_ENTRY
{
    LIST_ENTRY DeviceListEntry; // 0x0
    UINT32 SortKey;             // 0x10
    UINT8 Inserted;             // 0x14
};
