#pragma once

#include "../fwd.hpp"

#include "wait.hpp"

// 0x10 bytes (sizeof)
struct _IO_STATUS_BLOCK
{
    union
    {
        INT32 Status;  // 0x0
        PVOID Pointer; // 0x0
    };
    UINT64 Information; // 0x8
};

// 0x28 bytes (sizeof)
struct _FILE_BASIC_INFORMATION
{
    LARGE_INTEGER CreationTime;   // 0x0
    LARGE_INTEGER LastAccessTime; // 0x8
    LARGE_INTEGER LastWriteTime;  // 0x10
    LARGE_INTEGER ChangeTime;     // 0x18
    ULONG FileAttributes;         // 0x20
};

// 0x18 bytes (sizeof)
struct _FILE_STANDARD_INFORMATION
{
    LARGE_INTEGER AllocationSize; // 0x0
    LARGE_INTEGER EndOfFile;      // 0x8
    ULONG NumberOfLinks;          // 0x10
    BOOLEAN DeletePending;        // 0x14
    BOOLEAN Directory;            // 0x15
};

// 0x8 bytes (sizeof)
struct _FILE_POSITION_INFORMATION
{
    LARGE_INTEGER CurrentByteOffset; // 0x0
};

// 0x38 bytes (sizeof)
struct _FILE_NETWORK_OPEN_INFORMATION
{
    LARGE_INTEGER CreationTime;   // 0x0
    LARGE_INTEGER LastAccessTime; // 0x8
    LARGE_INTEGER LastWriteTime;  // 0x10
    LARGE_INTEGER ChangeTime;     // 0x18
    LARGE_INTEGER AllocationSize; // 0x20
    LARGE_INTEGER EndOfFile;      // 0x28
    ULONG FileAttributes;         // 0x30
};

// Variable-sized structure.
struct _FILE_NAME_INFORMATION
{
    ULONG FileNameLength; // 0x0
    WCHAR FileName[1];    // 0x4
};

// 0x30 bytes (sizeof)
struct _MDL
{
    PMDL Next;            // 0x0
    SHORT Size;           // 0x8
    SHORT MdlFlags;       // 0xa
    PEPROCESS Process;    // 0x10
    PVOID MappedSystemVa; // 0x18
    PVOID StartVa;        // 0x20
    UINT32 ByteCount;     // 0x28
    UINT32 ByteOffset;    // 0x2c
};

// 0x58 bytes (sizeof)
struct _KAPC
{
    UINT8 Type;              // 0x0
    UINT8 AllFlags;          // 0x1
    UINT8 Size;              // 0x2
    UINT8 SpareByte1;        // 0x3
    UINT32 SpareLong0;       // 0x4
    PKTHREAD Thread;         // 0x8
    LIST_ENTRY ApcListEntry; // 0x10
    PVOID Reserved[3];       // 0x20
    PVOID NormalContext;     // 0x38
    PVOID SystemArgument1;   // 0x40
    PVOID SystemArgument2;   // 0x48
    INT8 ApcStateIndex;      // 0x50
    INT8 ApcMode;            // 0x51
    UINT8 Inserted;          // 0x52
};

// 0xd0 bytes (sizeof)
struct _IRP
{
    SHORT Type;      // 0x0
    UINT16 Size;     // 0x2
    PMDL MdlAddress; // 0x8
    UINT32 Flags;    // 0x10
    union
    {
        PIRP MasterIrp;         // 0x18
        INT32 IrpCount;         // 0x18
        PVOID SystemBuffer;     // 0x18
    } AssociatedIrp;            // 0x18
    LIST_ENTRY ThreadListEntry; // 0x20
    IO_STATUS_BLOCK IoStatus;   // 0x30
    INT8 RequestorMode;         // 0x40
    UINT8 PendingReturned;      // 0x41
    INT8 StackCount;            // 0x42
    INT8 CurrentLocation;       // 0x43
    UINT8 Cancel;               // 0x44
    UINT8 CancelIrql;           // 0x45
    INT8 ApcEnvironment;        // 0x46
    UINT8 AllocationFlags;      // 0x47
    PIO_STATUS_BLOCK UserIosb;  // 0x48
    PKEVENT UserEvent;          // 0x50
    union
    {
        struct
        {
            union
            {
                VOID (*UserApcRoutine)(PVOID arg1, struct _IO_STATUS_BLOCK* arg2, UINT32 arg3); // 0x58
                PVOID IssuingProcess;                                                           // 0x58
            };
            PVOID UserApcContext;                                          // 0x60
        } AsynchronousParameters;                                          // 0x58
        LARGE_INTEGER AllocationSize;                                      // 0x58
    } Overlay;                                                             // 0x58
    VOID (*CancelRoutine)(struct _DEVICE_OBJECT* arg1, struct _IRP* arg2); // 0x68
    PVOID UserBuffer;                                                      // 0x70
    union
    {
        struct
        {
            union
            {
                KDEVICE_QUEUE_ENTRY DeviceQueueEntry; // 0x78
                PVOID DriverContext[4];               // 0x78
            };
            PETHREAD Thread;       // 0x98
            INT8* AuxiliaryBuffer; // 0xa0
            LIST_ENTRY ListEntry;  // 0xa8
            union
            {
                PIO_STACK_LOCATION CurrentStackLocation; // 0xb8
                UINT32 PacketType;                       // 0xb8
            };
            PFILE_OBJECT OriginalFileObject; // 0xc0
        } Overlay;                           // 0x78
        KAPC Apc;                            // 0x78
        PVOID CompletionKey;                 // 0x78
    } Tail;                                  // 0x78
};

// 0x30 bytes (sizeof)
struct _KAPC_STATE
{
    LIST_ENTRY ApcListHead[2]; // 0x0
    PEPROCESS Process;         // 0x20
    union
    {
        UINT8 InProgressFlags; // 0x28
        struct
        {
            UINT8 KernelApcInProgress : 1;  // 0x28
            UINT8 SpecialApcInProgress : 1; // 0x28
        };
    };
    UINT8 KernelApcPending; // 0x29
    union
    {
        UINT8 UserApcPendingAll; // 0x2a
        struct
        {
            UINT8 SpecialUserApcPending : 1; // 0x2a
            UINT8 UserApcPending : 1;        // 0x2a
        };
    };
};
