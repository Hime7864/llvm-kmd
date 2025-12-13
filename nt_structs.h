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
    UINT64 Valid : 1;                                                      //0x0
    UINT64 Dirty1 : 1;                                                     //0x0
    UINT64 Owner : 1;                                                      //0x0
    UINT64 WriteThrough : 1;                                               //0x0
    UINT64 CacheDisable : 1;                                               //0x0
    UINT64 Accessed : 1;                                                   //0x0
    UINT64 Dirty : 1;                                                      //0x0
    UINT64 LargePage : 1;                                                  //0x0
    UINT64 Global : 1;                                                     //0x0
    UINT64 CopyOnWrite : 1;                                                //0x0
    UINT64 Unused : 1;                                                     //0x0
    UINT64 Write : 1;                                                      //0x0
    UINT64 PageFrameNumber : 40;                                           //0x0
    UINT64 ReservedForSoftware : 4;                                        //0x0
    UINT64 WsleAge : 4;                                                    //0x0
    UINT64 WsleProtection : 3;                                             //0x0
    UINT64 NoExecute : 1;                                                  //0x0
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
    KProcessorModeInvalid,
    KernelMode,
    UserMode,
    KProcessorModeMaximum
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