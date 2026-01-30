#pragma once

//0x10 bytes (sizeof)
struct _UNICODE_STRING
{
    UINT16 Length;                                                          //0x0
    UINT16 MaximumLength;                                                   //0x2
    LPWSTR Buffer;                                                          //0x8
};

//0x8 bytes (sizeof)

struct _LARGE_INTEGER
{
    union
    {
        INT64 QuadPart;                                                  //0x0
        struct
        {
            UINT32 LowPart;                                                      //0x0
            INT32 HighPart;                                                      //0x4
        };
    };
};


//0x10 bytes (sizeof)
struct _LIST_ENTRY
{
    PLIST_ENTRY Flink;                                              //0x0
    PLIST_ENTRY Blink;                                              //0x8
};


//0x8 bytes (sizeof)
struct _MMPTE_HARDWARE
{
    union
    {
        UINT64 AsUINT64;
        struct {
            ULONGLONG Valid : 1;                                                      //0x0
            ULONGLONG Dirty1 : 1;                                                     //0x0
            ULONGLONG Owner : 1;                                                      //0x0
            ULONGLONG WriteThrough : 1;                                               //0x0
            ULONGLONG CacheDisable : 1;                                               //0x0
            ULONGLONG Accessed : 1;                                                   //0x0
            ULONGLONG Dirty : 1;                                                      //0x0
            ULONGLONG LargePage : 1;                                                  //0x0
            ULONGLONG Global : 1;                                                     //0x0
            ULONGLONG CopyOnWrite : 1;                                                //0x0
            ULONGLONG Unused : 1;                                                     //0x0
            ULONGLONG Write : 1;                                                      //0x0
            ULONGLONG PageFrameNumber : 40;                                           //0x0
            ULONGLONG ReservedForSoftware : 4;                                        //0x0
            ULONGLONG WsleAge : 4;                                                    //0x0
            ULONGLONG WsleProtection : 3;                                             //0x0
            ULONGLONG NoExecute : 1;                                                  //0x0
        };

    };
};


//0x18 bytes (sizeof)
struct _RTL_BALANCED_NODE
{
    struct
    {
        PRTL_BALANCED_NODE Left;
        PRTL_BALANCED_NODE Right;
    };
    union
    {
        UINT64 ParentValue;
        struct
        {
            UINT8 Red : 1;
            UINT8 Balance : 2;
        };
    };
};

//0x8 bytes (sizeof)
struct _RTL_AVL_TREE
{
    PRTL_BALANCED_NODE Root;
};

//0x10 bytes (sizeof)
struct _M128A
{
    UINT64 Low;                                                          //0x0
    INT64 High;                                                          //0x8
};

//0x200 bytes (sizeof)
struct _XSAVE_FORMAT
{
    UINT16 ControlWord;                                                     //0x0
    UINT16 StatusWord;                                                      //0x2
    UINT8 TagWord;                                                          //0x4
    UINT8 Reserved1;                                                        //0x5
    UINT16 ErrorOpcode;                                                     //0x6
    UINT32 ErrorOffset;                                                      //0x8
    UINT16 ErrorSelector;                                                   //0xc
    UINT16 Reserved2;                                                       //0xe
    UINT32 DataOffset;                                                       //0x10
    UINT16 DataSelector;                                                    //0x14
    UINT16 Reserved3;                                                       //0x16
    UINT32 MxCsr;                                                            //0x18
    UINT32 MxCsr_Mask;                                                       //0x1c
    M128A FloatRegisters[8];                                                //0x20
    M128A XmmRegisters[16];                                                 //0xa0
    UINT8 Reserved4[96];                                                    //0x1a0
};

//0x4d0 bytes (sizeof)
struct _CONTEXT
{
    UINT64 P1Home;                                                       //0x0
    UINT64 P2Home;                                                       //0x8
    UINT64 P3Home;                                                       //0x10
    UINT64 P4Home;                                                       //0x18
    UINT64 P5Home;                                                       //0x20
    UINT64 P6Home;                                                       //0x28
    UINT32 ContextFlags;                                                     //0x30
    UINT32 MxCsr;                                                            //0x34
    UINT16 SegCs;                                                           //0x38
    UINT16 SegDs;                                                           //0x3a
    UINT16 SegEs;                                                           //0x3c
    UINT16 SegFs;                                                           //0x3e
    UINT16 SegGs;                                                           //0x40
    UINT16 SegSs;                                                           //0x42
    UINT32 EFlags;                                                           //0x44
    UINT64 Dr0;                                                          //0x48
    UINT64 Dr1;                                                          //0x50
    UINT64 Dr2;                                                          //0x58
    UINT64 Dr3;                                                          //0x60
    UINT64 Dr6;                                                          //0x68
    UINT64 Dr7;                                                          //0x70
    UINT64 Rax;                                                          //0x78
    UINT64 Rcx;                                                          //0x80
    UINT64 Rdx;                                                          //0x88
    UINT64 Rbx;                                                          //0x90
    UINT64 Rsp;                                                          //0x98
    UINT64 Rbp;                                                          //0xa0
    UINT64 Rsi;                                                          //0xa8
    UINT64 Rdi;                                                          //0xb0
    UINT64 R8;                                                           //0xb8
    UINT64 R9;                                                           //0xc0
    UINT64 R10;                                                          //0xc8
    UINT64 R11;                                                          //0xd0
    UINT64 R12;                                                          //0xd8
    UINT64 R13;                                                          //0xe0
    UINT64 R14;                                                          //0xe8
    UINT64 R15;                                                          //0xf0
    UINT64 Rip;                                                          //0xf8
    union
    {
        XSAVE_FORMAT FltSave;                                               //0x100
        struct
        {
            M128A Header[2];                                                //0x100
            M128A Legacy[8];                                                //0x120
            M128A Xmm0;                                                     //0x1a0
            M128A Xmm1;                                                     //0x1b0
            M128A Xmm2;                                                     //0x1c0
            M128A Xmm3;                                                     //0x1d0
            M128A Xmm4;                                                     //0x1e0
            M128A Xmm5;                                                     //0x1f0
            M128A Xmm6;                                                     //0x200
            M128A Xmm7;                                                     //0x210
            M128A Xmm8;                                                     //0x220
            M128A Xmm9;                                                     //0x230
            M128A Xmm10;                                                    //0x240
            M128A Xmm11;                                                    //0x250
            M128A Xmm12;                                                    //0x260
            M128A Xmm13;                                                    //0x270
            M128A Xmm14;                                                    //0x280
            M128A Xmm15;                                                    //0x290
        };
    };
    M128A VectorRegister[26];                                               //0x300
    UINT64 VectorControl;                                                //0x4a0
    UINT64 DebugControl;                                                 //0x4a8
    UINT64 LastBranchToRip;                                              //0x4b0
    UINT64 LastBranchFromRip;                                            //0x4b8
    UINT64 LastExceptionToRip;                                           //0x4c0
    UINT64 LastExceptionFromRip;                                         //0x4c8
};

//0x30 bytes (sizeof)
struct _MDL
{
    PMDL Next;                                                              //0x0
    SHORT Size;                                                             //0x8
    SHORT MdlFlags;                                                         //0xa
    PEPROCESS Process;                                                      //0x10
    PVOID MappedSystemVa;                                                   //0x18
    PVOID StartVa;                                                          //0x20
    UINT32 ByteCount;                                                        //0x28
    UINT32 ByteOffset;                                                       //0x2c
};

//0x10 bytes (sizeof)
struct _IO_STATUS_BLOCK
{
    union
    {
        INT32 Status;                                                        //0x0
        PVOID Pointer;                                                      //0x0
    };
    UINT64 Information;                                                  //0x8
};

//0x18 bytes (sizeof)
struct _DISPATCHER_HEADER
{
    union
    {
        INT32 Lock;                                                 //0x0
        struct
        {
            UINT8 Type;                                                     //0x0
            UINT8 Signalling;                                               //0x1
            UINT8 Size;                                                     //0x2
            UINT8 Reserved1;                                                //0x3
        };
        struct
        {
            UINT8 TimerType;                                                //0x0
            union
            {
                UINT8 TimerControlFlags;                                    //0x1
                struct
                {
                    UINT8 Absolute : 1;                                       //0x1
                    UINT8 Wake : 1;                                           //0x1
                    UINT8 EncodedTolerableDelay : 6;                          //0x1
                };
            };
            UINT8 Hand;                                                     //0x2
            union
            {
                UINT8 TimerMiscFlags;                                       //0x3
                struct
                {
                    UINT8 Index : 6;                                          //0x3
                    UINT8 Inserted : 1;                                       //0x3
                    volatile UINT8 Expired : 1;                               //0x3
                };
            };
        };
        struct
        {
            UINT8 Timer2Type;                                               //0x0
            union
            {
                UINT8 Timer2Flags;                                          //0x1
                struct
                {
                    UINT8 Timer2Inserted : 1;                                 //0x1
                    UINT8 Timer2Expiring : 1;                                 //0x1
                    UINT8 Timer2CancelPending : 1;                            //0x1
                    UINT8 Timer2SetPending : 1;                               //0x1
                    UINT8 Timer2Running : 1;                                  //0x1
                    UINT8 Timer2Disabled : 1;                                 //0x1
                    UINT8 Timer2ReservedFlags : 2;                            //0x1
                };
            };
            UINT8 Timer2ComponentId;                                        //0x2
            UINT8 Timer2RelativeId;                                         //0x3
        };
        struct
        {
            UINT8 QueueType;                                                //0x0
            union
            {
                UINT8 QueueControlFlags;                                    //0x1
                struct
                {
                    UINT8 Abandoned : 1;                                      //0x1
                    UINT8 DisableIncrement : 1;                               //0x1
                    UINT8 QueueReservedControlFlags : 6;                      //0x1
                };
            };
            UINT8 QueueSize;                                                //0x2
            UINT8 QueueReserved;                                            //0x3
        };
        struct
        {
            UINT8 ThreadType;                                               //0x0
            UINT8 ThreadReserved;                                           //0x1
            union
            {
                UINT8 ThreadControlFlags;                                   //0x2
                struct
                {
                    UINT8 CycleProfiling : 1;                                 //0x2
                    UINT8 CounterProfiling : 1;                               //0x2
                    UINT8 GroupScheduling : 1;                                //0x2
                    UINT8 AffinitySet : 1;                                    //0x2
                    UINT8 Tagged : 1;                                         //0x2
                    UINT8 EnergyProfiling : 1;                                //0x2
                    UINT8 SchedulerAssist : 1;                                //0x2
                    UINT8 ThreadReservedControlFlags : 1;                     //0x2
                };
            };
            union
            {
                UINT8 DebugActive;                                          //0x3
                struct
                {
                    UINT8 ActiveDR7 : 1;                                      //0x3
                    UINT8 Instrumented : 1;                                   //0x3
                    UINT8 Minimal : 1;                                        //0x3
                    UINT8 Reserved4 : 2;                                      //0x3
                    UINT8 AltSyscall : 1;                                     //0x3
                    UINT8 Emulation : 1;                                      //0x3
                    UINT8 Reserved5 : 1;                                      //0x3
                };
            };
        };
        struct
        {
            UINT8 MutantType;                                               //0x0
            UINT8 MutantSize;                                               //0x1
            UINT8 DpcActive;                                                //0x2
            UINT8 MutantReserved;                                           //0x3
        };
    };
    INT32 SignalState;                                                       //0x4
    LIST_ENTRY WaitListHead;                                        //0x8
};

//0x18 bytes (sizeof)
struct _KEVENT
{
    DISPATCHER_HEADER Header;                                       //0x0
};

//0x18 bytes (sizeof)
struct _KDEVICE_QUEUE_ENTRY
{
    LIST_ENTRY DeviceListEntry;                                     //0x0
    UINT32 SortKey;                                                          //0x10
    UINT8 Inserted;                                                         //0x14
};

//0x58 bytes (sizeof)
struct _KAPC
{
    UINT8 Type;                                                             //0x0
    UINT8 AllFlags;                                                         //0x1
    UINT8 Size;                                                             //0x2
    UINT8 SpareByte1;                                                       //0x3
    UINT32 SpareLong0;                                                       //0x4
    PKTHREAD Thread;                                                //0x8
    LIST_ENTRY ApcListEntry;                                        //0x10
    PVOID Reserved[3];                                                      //0x20
    PVOID NormalContext;                                                    //0x38
    PVOID SystemArgument1;                                                  //0x40
    PVOID SystemArgument2;                                                  //0x48
    INT8 ApcStateIndex;                                                     //0x50
    INT8 ApcMode;                                                           //0x51
    UINT8 Inserted;                                                         //0x52
};

//0xd0 bytes (sizeof)
struct _IRP
{
    SHORT Type;                                                             //0x0
    UINT16 Size;                                                            //0x2
    PMDL MdlAddress;                                                //0x8
    UINT32 Flags;                                                            //0x10
    union
    {
        PIRP MasterIrp;                                             //0x18
        INT32 IrpCount;                                                      //0x18
        PVOID SystemBuffer;                                                 //0x18
    } AssociatedIrp;                                                        //0x18
    LIST_ENTRY ThreadListEntry;                                     //0x20
    IO_STATUS_BLOCK IoStatus;                                       //0x30
    INT8 RequestorMode;                                                     //0x40
    UINT8 PendingReturned;                                                  //0x41
    INT8 StackCount;                                                        //0x42
    INT8 CurrentLocation;                                                   //0x43
    UINT8 Cancel;                                                           //0x44
    UINT8 CancelIrql;                                                       //0x45
    INT8 ApcEnvironment;                                                    //0x46
    UINT8 AllocationFlags;                                                  //0x47
    PIO_STATUS_BLOCK UserIosb;                                      //0x48
    PKEVENT UserEvent;                                              //0x50
    union
    {
        struct
        {
            union
            {
                VOID(*UserApcRoutine)(PVOID arg1, struct _IO_STATUS_BLOCK* arg2, UINT32 arg3); //0x58
                PVOID IssuingProcess;                                       //0x58
            };
            PVOID UserApcContext;                                           //0x60
        } AsynchronousParameters;                                           //0x58
        LARGE_INTEGER AllocationSize;                                //0x58
    } Overlay;                                                              //0x58
    VOID(*CancelRoutine)(struct _DEVICE_OBJECT* arg1, struct _IRP* arg2);  //0x68
    PVOID UserBuffer;                                                       //0x70
    union
    {
        struct
        {
            union
            {
                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;               //0x78
                PVOID DriverContext[4];                                     //0x78
            };
            PETHREAD Thread;                                        //0x98
            INT8* AuxiliaryBuffer;                                          //0xa0
            LIST_ENTRY ListEntry;                                   //0xa8
            union
            {
                PIO_STACK_LOCATION CurrentStackLocation;            //0xb8
                UINT32 PacketType;                                           //0xb8
            };
            PFILE_OBJECT OriginalFileObject;                        //0xc0
        } Overlay;                                                          //0x78
        KAPC Apc;                                                   //0x78
        PVOID CompletionKey;                                                //0x78
    } Tail;                                                                 //0x78
};

//0x30 bytes (sizeof)
struct _KAPC_STATE
{
    LIST_ENTRY ApcListHead[2];                                      //0x0
    PEPROCESS Process;                                              //0x20
    union
    {
        UINT8 InProgressFlags;                                              //0x28
        struct
        {
            UINT8 KernelApcInProgress : 1;                                    //0x28
            UINT8 SpecialApcInProgress : 1;                                   //0x28
        };
    };
    UINT8 KernelApcPending;                                                 //0x29
    union
    {
        UINT8 UserApcPendingAll;                                            //0x2a
        struct
        {
            UINT8 SpecialUserApcPending : 1;                                  //0x2a
            UINT8 UserApcPending : 1;                                         //0x2a
        };
    };
};

enum _KPROCESSOR_MODE {
    KernelMode,
    UserMode,
    MaximumMode
};

//0x30 bytes (sizeof)
struct _OBJECT_ATTRIBUTES
{
    UINT32 Length;                                                           //0x0
    PVOID RootDirectory;                                                    //0x8
    PUNICODE_STRING ObjectName;                                     //0x10
    UINT32 Attributes;                                                       //0x18
    PVOID SecurityDescriptor;                                               //0x20
    PVOID SecurityQualityOfService;                                         //0x28
};

//0x10 bytes (sizeof)
struct _CLIENT_ID
{
    PVOID UniqueProcess;                                                    //0x0
    PVOID UniqueThread;                                                     //0x8
};

struct _DESCRIPTOR_TABLE_REGISTER
{
    UINT16 limit;
    UINT64 base;
};

struct _SEGMENT_SELECTOR
{
    union
    {
        UINT16 value;
        struct
        {
            UINT16 rpl : 2;
            UINT16 table : 1;
            UINT16 index : 13;
        };
    };
};

enum _POOL_TYPE {
    NonPagedPool,
    NonPagedPoolExecute = NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed = NonPagedPool + 2,
    DontUseThisType,
    NonPagedPoolCacheAligned = NonPagedPool + 4,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS = NonPagedPool + 6,
    MaxPoolType,
    NonPagedPoolBase = 0,
    NonPagedPoolBaseMustSucceed = NonPagedPoolBase + 2,
    NonPagedPoolBaseCacheAligned = NonPagedPoolBase + 4,
    NonPagedPoolBaseCacheAlignedMustS = NonPagedPoolBase + 6,
    NonPagedPoolSession = 32,
    PagedPoolSession = NonPagedPoolSession + 1,
    NonPagedPoolMustSucceedSession = PagedPoolSession + 1,
    DontUseThisTypeSession = NonPagedPoolMustSucceedSession + 1,
    NonPagedPoolCacheAlignedSession = DontUseThisTypeSession + 1,
    PagedPoolCacheAlignedSession = NonPagedPoolCacheAlignedSession + 1,
    NonPagedPoolCacheAlignedMustSSession = PagedPoolCacheAlignedSession + 1,
    NonPagedPoolNx = 512,
    NonPagedPoolNxCacheAligned = NonPagedPoolNx + 4,
    NonPagedPoolSessionNx = NonPagedPoolNx + 32,

};

struct _PHYSICAL_MEMORY_RANGE
{
    LARGE_INTEGER BaseAddress;                                       //0x0
    LARGE_INTEGER NumberOfBytes;                                     //0x8
};

struct _MACHINE_FRAME
{
    ULONGLONG Rip;                                                          //0x0
    USHORT SegCs;                                                           //0x8
    USHORT Fill1[3];                                                        //0xa
    ULONG EFlags;                                                           //0x10
    ULONG Fill2;                                                            //0x14
    ULONGLONG Rsp;                                                          //0x18
    USHORT SegSs;                                                           //0x20
    USHORT Fill3[3];                                                        //0x22
};

//0x4 bytes (sizeof)
enum _MEMORY_CACHING_TYPE
{
    MmNonCached = 0,
    MmCached = 1,
    MmWriteCombined = 2,
    MmHardwareCoherentCached = 3,
    MmNonCachedUnordered = 4,
    MmUSWCCached = 5,
    MmMaximumCacheType = 6,
    MmNotMapped = -1
};

//0x8 bytes (sizeof)
struct _MI_ACTIVE_PFN
{
    union
    {
        struct
        {
            ULONGLONG Tradable : 1;                                           //0x0
            ULONGLONG NonPagedBuddy : 43;                                     //0x0
            ULONGLONG Spare : 20;                                             //0x0
        } Leaf;                                                             //0x0
        struct
        {
            ULONGLONG Tradable : 1;                                           //0x0
            ULONGLONG NonPagedBuddy : 31;                                     //0x0
            ULONGLONG UsedPageTableEntries : 10;                              //0x0
            ULONGLONG WsleAge : 3;                                            //0x0
            ULONGLONG OldestWsleLeafEntries : 10;                             //0x0
            ULONGLONG OldestWsleLeafAge : 3;                                  //0x0
            ULONGLONG Spare : 6;                                              //0x0
        } PageTable;                                                        //0x0
        ULONGLONG EntireActiveField;                                        //0x0
    };
};

//0x10 bytes (sizeof)
struct _SLIST_ENTRY
{
    struct _SLIST_ENTRY* Next;                                              //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_PROTOTYPE
{
    ULONGLONG Valid : 1;                                                      //0x0
    ULONGLONG DemandFillProto : 1;                                            //0x0
    ULONGLONG HiberVerifyConverted : 1;                                       //0x0
    ULONGLONG ReadOnly : 1;                                                   //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG Combined : 1;                                                   //0x0
    ULONGLONG Unused1 : 4;                                                    //0x0
    LONGLONG ProtoAddress : 48;                                               //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_SOFTWARE
{
    ULONGLONG Valid : 1;                                                      //0x0
    ULONGLONG PageFileReserved : 1;                                           //0x0
    ULONGLONG PageFileAllocated : 1;                                          //0x0
    ULONGLONG ColdPage : 1;                                                   //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG Transition : 1;                                                 //0x0
    ULONGLONG PageFileLow : 4;                                                //0x0
    ULONGLONG UsedPageTableEntries : 10;                                      //0x0
    ULONGLONG ShadowStack : 1;                                                //0x0
    ULONGLONG OnStandbyLookaside : 1;                                         //0x0
    ULONGLONG Unused : 4;                                                     //0x0
    ULONGLONG PageFileHigh : 32;                                              //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_TIMESTAMP
{
    ULONGLONG MustBeZero : 1;                                                 //0x0
    ULONGLONG Unused : 3;                                                     //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG Transition : 1;                                                 //0x0
    ULONGLONG PageFileLow : 4;                                                //0x0
    ULONGLONG Reserved : 16;                                                  //0x0
    ULONGLONG GlobalTimeStamp : 32;                                           //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_TRANSITION
{
    ULONGLONG Valid : 1;                                                      //0x0
    ULONGLONG Write : 1;                                                      //0x0
    ULONGLONG OnStandbyLookaside : 1;                                         //0x0
    ULONGLONG IoTracker : 1;                                                  //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG Transition : 1;                                                 //0x0
    ULONGLONG PageFrameNumber : 40;                                           //0x0
    ULONGLONG Unused : 12;                                                    //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_SUBSECTION
{
    ULONGLONG Valid : 1;                                                      //0x0
    ULONGLONG Unused0 : 2;                                                    //0x0
    ULONGLONG OnStandbyLookaside : 1;                                         //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG ColdPage : 1;                                                   //0x0
    ULONGLONG Unused2 : 3;                                                    //0x0
    ULONGLONG ExecutePrivilege : 1;                                           //0x0
    LONGLONG SubsectionAddress : 48;                                          //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE_LIST
{
    ULONGLONG Valid : 1;                                                      //0x0
    ULONGLONG OneEntry : 1;                                                   //0x0
    ULONGLONG filler0 : 2;                                                    //0x0
    ULONGLONG SwizzleBit : 1;                                                 //0x0
    ULONGLONG Protection : 5;                                                 //0x0
    ULONGLONG Prototype : 1;                                                  //0x0
    ULONGLONG Transition : 1;                                                 //0x0
    ULONGLONG filler1 : 13;                                                   //0x0
    ULONGLONG NextEntry : 39;                                                 //0x0
};


//0x8 bytes (sizeof)
struct _MMPTE_ACTIVE_PAGE_TABLE_LINKS
{
    ULONGLONG Flink : 40;                                                     //0x0
    ULONGLONG BlinkHigh : 24;                                                 //0x0
};

//0x8 bytes (sizeof)
struct _MMPTE
{
    union
    {
        ULONGLONG Long;                                                     //0x0
        struct _MMPTE_HARDWARE Hard;                                        //0x0
        struct _MMPTE_PROTOTYPE Proto;                                      //0x0
        struct _MMPTE_SOFTWARE Soft;                                        //0x0
        struct _MMPTE_TIMESTAMP TimeStamp;                                  //0x0
        struct _MMPTE_TRANSITION Trans;                                     //0x0
        struct _MMPTE_SUBSECTION Subsect;                                   //0x0
        struct _MMPTE_LIST List;                                            //0x0
        struct _MMPTE_ACTIVE_PAGE_TABLE_LINKS PageTableLinks;               //0x0
    } u;                                                                    //0x0
};


//0x8 bytes (sizeof)
struct _MIPFNFLINK
{
    union
    {
        _SLIST_ENTRY* NextSlistPfn;                                  //0x0
        VOID* Next;                                                         //0x0
        struct
        {
            ULONGLONG Flink : 40;                                             //0x0
            ULONGLONG NodeFlinkLow : 24;                                      //0x0
        };
        ULONGLONG EntireField;                                              //0x0
        _MI_ACTIVE_PFN Active;                                       //0x0
    };
};

//0x1 bytes (sizeof)
struct _MMPFNENTRY1
{
    UCHAR PageLocation : 3;                                                   //0x0
    UCHAR WriteInProgress : 1;                                                //0x0
    UCHAR Modified : 1;                                                       //0x0
    UCHAR ReadInProgress : 1;                                                 //0x0
    UCHAR CacheAttribute : 2;                                                 //0x0
};

//0x8 bytes (sizeof)
struct _MIPFNBLINK
{
    union
    {
        //struct
        //{
        //    ULONGLONG Blink : 40;                                             //0x0
        //    ULONGLONG NodeBlinkLow : 19;                                      //0x0
        //    ULONGLONG TbFlushStamp : 3;                                       //0x0
        //    ULONGLONG PageBlinkDeleteBit : 1;                                 //0x0
        //    ULONGLONG PageBlinkLockBit : 1;                                   //0x0
        //    ULONGLONG ShareCount : 62;                                        //0x0
        //    ULONGLONG PageShareCountDeleteBit : 1;                            //0x0
        //    ULONGLONG PageShareCountLockBit : 1;                              //0x0
        //};
        LONGLONG EntireField;                                               //0x0
        struct
        {
            ULONGLONG LockNotUsed : 62;                                       //0x0
            ULONGLONG DeleteBit : 1;                                          //0x0
            ULONGLONG LockBit : 1;                                            //0x0
        };
    };
};

//0x1 bytes (sizeof)
struct _MMPFNENTRY3
{
    UCHAR Priority : 3;                                                       //0x0
    UCHAR OnProtectedStandby : 1;                                             //0x0
    UCHAR InPageError : 1;                                                    //0x0
    UCHAR SystemChargedPage : 1;                                              //0x0
    UCHAR RemovalRequested : 1;                                               //0x0
    UCHAR ParityError : 1;                                                    //0x0
};

//0x4 bytes (sizeof)
struct _MI_PFN_FLAGS
{
    union
    {
        struct
        {
            USHORT ReferenceCount;                                          //0x0
            UCHAR PageLocation : 3;                                           //0x2
            UCHAR WriteInProgress : 1;                                        //0x2
            UCHAR Modified : 1;                                               //0x2
            UCHAR ReadInProgress : 1;                                         //0x2
            UCHAR CacheAttribute : 2;                                         //0x2
            UCHAR Priority : 3;                                               //0x3
            UCHAR OnProtectedStandby : 1;                                     //0x3
            UCHAR InPageError : 1;                                            //0x3
            UCHAR SystemChargedPage : 1;                                      //0x3
            UCHAR RemovalRequested : 1;                                       //0x3
            UCHAR ParityError : 1;                                            //0x3
        };
        ULONG EntireField;                                                  //0x0
    };
};

//0x4 bytes (sizeof)
struct _MI_PFN_FLAGS5
{
    union
    {
        ULONG EntireField;                                                  //0x0
        struct
        {
            ULONG NodeBlinkHigh : 21;                                         //0x0
            ULONG NodeFlinkMiddle : 11;                                       //0x0
        } StandbyList;                                                      //0x0
        struct
        {
            UCHAR ModifiedListBucketIndex : 4;                                //0x0
        } MappedPageList;                                                   //0x0
        struct
        {
            ULONG PageTableBlinkLow : 16;                                     //0x0
            ULONG PageTableBuddyHigh : 10;                                    //0x0
            ULONG PageTableLinked : 1;                                        //0x0
            ULONG AnchorLargePageSize : 2;                                    //0x0
            ULONG Spare1 : 3;                                                 //0x0
        } Active;                                                           //0x0
    };
};

//0x8 bytes (sizeof)
struct _MI_PFN_FLAGS4
{
    union
    {
        struct
        {
            ULONGLONG PteFrame : 40;                                          //0x0
            ULONGLONG ResidentPage : 1;                                       //0x0
            ULONGLONG ResidentPageContainsBadPages : 1;                       //0x0
            ULONGLONG Unused1 : 1;                                            //0x0
            ULONGLONG Partition : 10;                                         //0x0
            ULONGLONG FileOnly : 1;                                           //0x0
            ULONGLONG PfnExists : 1;                                          //0x0
            ULONGLONG NodeFlinkHigh : 5;                                      //0x0
            ULONGLONG PageIdentity : 3;                                       //0x0
            ULONGLONG PrototypePte : 1;                                       //0x0
        };
        ULONGLONG EntireField;                                              //0x0
    };
};

//0x30 bytes (sizeof)
struct _MMPFN
{
    union
    {
        _LIST_ENTRY ListEntry;                                       //0x0
        _RTL_BALANCED_NODE TreeNode;                                 //0x0
        struct
        {
            _MIPFNFLINK u1;                                          //0x0
            union
            {
                _MMPTE* PteAddress;                                  //0x8
                ULONGLONG PteLong;                                          //0x8
            };
            _MMPTE OriginalPte;                                      //0x10
        };
    };
    _MIPFNBLINK u2;                                         //0x18
    union
    {
        struct
        {
            USHORT ReferenceCount;                                          //0x20
            _MMPFNENTRY1 e1;                                         //0x22
        };
        struct
        {
            _MMPFNENTRY3 e3;                                         //0x23
            struct
            {
                USHORT ReferenceCount;                                          //0x20
            } e2;                                                               //0x20
        };
        _MI_PFN_FLAGS e4;                                   //0x20
    } u3;                                                                   //0x20
    _MI_PFN_FLAGS5 u5;                                               //0x24
    _MI_PFN_FLAGS4 u4;                                               //0x28
};

enum _MI_ASSIGNED_REGION_TYPES
{
    AssignedRegionPfnDatabase = 0,
    AssignedRegionNonPagedPool = 1,
    AssignedRegionPagedPool = 2,
    AssignedRegionSystemCache = 3,
    AssignedRegionSystemPtes = 4,
    AssignedRegionKasan = 5,
    AssignedRegionUltraZero = 6,
    AssignedRegionCfg = 7,
    AssignedRegionHyperSpace = 8,
    AssignedRegionKernelStacks = 9,
    AssignedRegionNonCachedMappings = 10,
    AssignedRegionSoftWsles = 11,
    AssignedRegionPageTables = 12,
    AssignedRegionNotUsed = 13,
    AssignedRegionSecureNonPagedPool = 14,
    AssignedRegionKernelShadowStacks = 15,
    AssignedRegionSystemDataViews = 16,
    AssignedRegionSystemImages = 17,
    AssignedRegionMaximum = 18
};
