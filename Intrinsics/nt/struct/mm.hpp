#pragma once

#include "../fwd.hpp"

#include "base.hpp"

struct _MMPTE_HARDWARE
{
    union
    {
        UINT64 AsUINT64;
        struct
        {
            ULONGLONG Valid : 1;               // 0x0
            ULONGLONG Dirty1 : 1;              // 0x0
            ULONGLONG Owner : 1;               // 0x0
            ULONGLONG WriteThrough : 1;        // 0x0
            ULONGLONG CacheDisable : 1;        // 0x0
            ULONGLONG Accessed : 1;            // 0x0
            ULONGLONG Dirty : 1;               // 0x0
            ULONGLONG LargePage : 1;           // 0x0
            ULONGLONG Global : 1;              // 0x0
            ULONGLONG CopyOnWrite : 1;         // 0x0
            ULONGLONG Unused : 1;              // 0x0
            ULONGLONG Write : 1;               // 0x0
            ULONGLONG PageFrameNumber : 40;    // 0x0
            ULONGLONG ReservedForSoftware : 4; // 0x0
            ULONGLONG WsleAge : 4;             // 0x0
            ULONGLONG WsleProtection : 3;      // 0x0
            ULONGLONG NoExecute : 1;           // 0x0
        };
    };
};

//0x108 bytes (sizeof)
struct _KAFFINITY_EX
{
    USHORT Count;   //0x0
    USHORT Size;    //0x2
    ULONG Reserved; //0x4
    union
    {
        ULONGLONG Bitmap[1];        //0x8
        ULONGLONG StaticBitmap[32]; //0x8
    };
}; 

// 0x18 bytes (sizeof)
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

// 0x8 bytes (sizeof)
struct _RTL_AVL_TREE
{
    PRTL_BALANCED_NODE Root;
};

struct _MI_ACTIVE_PFN
{
    union
    {
        struct
        {
            ULONGLONG Tradable : 1;       // 0x0
            ULONGLONG NonPagedBuddy : 43; // 0x0
            ULONGLONG Spare : 20;         // 0x0
        } Leaf;                           // 0x0
        struct
        {
            ULONGLONG Tradable : 1;               // 0x0
            ULONGLONG NonPagedBuddy : 31;         // 0x0
            ULONGLONG UsedPageTableEntries : 10;  // 0x0
            ULONGLONG WsleAge : 3;                // 0x0
            ULONGLONG OldestWsleLeafEntries : 10; // 0x0
            ULONGLONG OldestWsleLeafAge : 3;      // 0x0
            ULONGLONG Spare : 6;                  // 0x0
        } PageTable;                              // 0x0
        ULONGLONG EntireActiveField;              // 0x0
    };
};

// 0x10 bytes (sizeof)
struct _SLIST_ENTRY
{
    struct _SLIST_ENTRY* Next; // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_PROTOTYPE
{
    ULONGLONG Valid : 1;                // 0x0
    ULONGLONG DemandFillProto : 1;      // 0x0
    ULONGLONG HiberVerifyConverted : 1; // 0x0
    ULONGLONG ReadOnly : 1;             // 0x0
    ULONGLONG SwizzleBit : 1;           // 0x0
    ULONGLONG Protection : 5;           // 0x0
    ULONGLONG Prototype : 1;            // 0x0
    ULONGLONG Combined : 1;             // 0x0
    ULONGLONG Unused1 : 4;              // 0x0
    LONGLONG ProtoAddress : 48;         // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_SOFTWARE
{
    ULONGLONG Valid : 1;                 // 0x0
    ULONGLONG PageFileReserved : 1;      // 0x0
    ULONGLONG PageFileAllocated : 1;     // 0x0
    ULONGLONG ColdPage : 1;              // 0x0
    ULONGLONG SwizzleBit : 1;            // 0x0
    ULONGLONG Protection : 5;            // 0x0
    ULONGLONG Prototype : 1;             // 0x0
    ULONGLONG Transition : 1;            // 0x0
    ULONGLONG PageFileLow : 4;           // 0x0
    ULONGLONG UsedPageTableEntries : 10; // 0x0
    ULONGLONG ShadowStack : 1;           // 0x0
    ULONGLONG OnStandbyLookaside : 1;    // 0x0
    ULONGLONG Unused : 4;                // 0x0
    ULONGLONG PageFileHigh : 32;         // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_TIMESTAMP
{
    ULONGLONG MustBeZero : 1;       // 0x0
    ULONGLONG Unused : 3;           // 0x0
    ULONGLONG SwizzleBit : 1;       // 0x0
    ULONGLONG Protection : 5;       // 0x0
    ULONGLONG Prototype : 1;        // 0x0
    ULONGLONG Transition : 1;       // 0x0
    ULONGLONG PageFileLow : 4;      // 0x0
    ULONGLONG Reserved : 16;        // 0x0
    ULONGLONG GlobalTimeStamp : 32; // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_TRANSITION
{
    ULONGLONG Valid : 1;              // 0x0
    ULONGLONG Write : 1;              // 0x0
    ULONGLONG OnStandbyLookaside : 1; // 0x0
    ULONGLONG IoTracker : 1;          // 0x0
    ULONGLONG SwizzleBit : 1;         // 0x0
    ULONGLONG Protection : 5;         // 0x0
    ULONGLONG Prototype : 1;          // 0x0
    ULONGLONG Transition : 1;         // 0x0
    ULONGLONG PageFrameNumber : 40;   // 0x0
    ULONGLONG Unused : 12;            // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_SUBSECTION
{
    ULONGLONG Valid : 1;              // 0x0
    ULONGLONG Unused0 : 2;            // 0x0
    ULONGLONG OnStandbyLookaside : 1; // 0x0
    ULONGLONG SwizzleBit : 1;         // 0x0
    ULONGLONG Protection : 5;         // 0x0
    ULONGLONG Prototype : 1;          // 0x0
    ULONGLONG ColdPage : 1;           // 0x0
    ULONGLONG Unused2 : 3;            // 0x0
    ULONGLONG ExecutePrivilege : 1;   // 0x0
    LONGLONG SubsectionAddress : 48;  // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_LIST
{
    ULONGLONG Valid : 1;      // 0x0
    ULONGLONG OneEntry : 1;   // 0x0
    ULONGLONG filler0 : 2;    // 0x0
    ULONGLONG SwizzleBit : 1; // 0x0
    ULONGLONG Protection : 5; // 0x0
    ULONGLONG Prototype : 1;  // 0x0
    ULONGLONG Transition : 1; // 0x0
    ULONGLONG filler1 : 13;   // 0x0
    ULONGLONG NextEntry : 39; // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE_ACTIVE_PAGE_TABLE_LINKS
{
    ULONGLONG Flink : 40;     // 0x0
    ULONGLONG BlinkHigh : 24; // 0x0
};

// 0x8 bytes (sizeof)
struct _MMPTE
{
    union
    {
        ULONGLONG Long;                                       // 0x0
        struct _MMPTE_HARDWARE Hard;                          // 0x0
        struct _MMPTE_PROTOTYPE Proto;                        // 0x0
        struct _MMPTE_SOFTWARE Soft;                          // 0x0
        struct _MMPTE_TIMESTAMP TimeStamp;                    // 0x0
        struct _MMPTE_TRANSITION Trans;                       // 0x0
        struct _MMPTE_SUBSECTION Subsect;                     // 0x0
        struct _MMPTE_LIST List;                              // 0x0
        struct _MMPTE_ACTIVE_PAGE_TABLE_LINKS PageTableLinks; // 0x0
    } u;                                                      // 0x0
};

// 0x8 bytes (sizeof)
struct _MIPFNFLINK
{
    union
    {
        _SLIST_ENTRY* NextSlistPfn; // 0x0
        VOID* Next;                 // 0x0
        struct
        {
            ULONGLONG Flink : 40;        // 0x0
            ULONGLONG NodeFlinkLow : 24; // 0x0
        };
        ULONGLONG EntireField; // 0x0
        _MI_ACTIVE_PFN Active; // 0x0
    };
};

// 0x1 bytes (sizeof)
struct _MMPFNENTRY1
{
    UCHAR PageLocation : 3;    // 0x0
    UCHAR WriteInProgress : 1; // 0x0
    UCHAR Modified : 1;        // 0x0
    UCHAR ReadInProgress : 1;  // 0x0
    UCHAR CacheAttribute : 2;  // 0x0
};

// 0x8 bytes (sizeof)
struct _MIPFNBLINK
{
    union
    {
        // struct
        // {
        //    ULONGLONG Blink : 40;                                             // 0x0
        //    ULONGLONG NodeBlinkLow : 19;                                      // 0x0
        //    ULONGLONG TbFlushStamp : 3;                                       // 0x0
        //    ULONGLONG PageBlinkDeleteBit : 1;                                 // 0x0
        //    ULONGLONG PageBlinkLockBit : 1;                                   // 0x0
        //    ULONGLONG ShareCount : 62;                                        // 0x0
        //    ULONGLONG PageShareCountDeleteBit : 1;                            // 0x0
        //    ULONGLONG PageShareCountLockBit : 1;                              // 0x0
        // };
        LONGLONG EntireField; // 0x0
        struct
        {
            ULONGLONG LockNotUsed : 62; // 0x0
            ULONGLONG DeleteBit : 1;    // 0x0
            ULONGLONG LockBit : 1;      // 0x0
        };
    };
};

// 0x1 bytes (sizeof)
struct _MMPFNENTRY3
{
    UCHAR Priority : 3;           // 0x0
    UCHAR OnProtectedStandby : 1; // 0x0
    UCHAR InPageError : 1;        // 0x0
    UCHAR SystemChargedPage : 1;  // 0x0
    UCHAR RemovalRequested : 1;   // 0x0
    UCHAR ParityError : 1;        // 0x0
};

// 0x4 bytes (sizeof)
struct _MI_PFN_FLAGS
{
    union
    {
        struct
        {
            USHORT ReferenceCount;        // 0x0
            UCHAR PageLocation : 3;       // 0x2
            UCHAR WriteInProgress : 1;    // 0x2
            UCHAR Modified : 1;           // 0x2
            UCHAR ReadInProgress : 1;     // 0x2
            UCHAR CacheAttribute : 2;     // 0x2
            UCHAR Priority : 3;           // 0x3
            UCHAR OnProtectedStandby : 1; // 0x3
            UCHAR InPageError : 1;        // 0x3
            UCHAR SystemChargedPage : 1;  // 0x3
            UCHAR RemovalRequested : 1;   // 0x3
            UCHAR ParityError : 1;        // 0x3
        };
        ULONG EntireField; // 0x0
    };
};

// 0x4 bytes (sizeof)
struct _MI_PFN_FLAGS5
{
    union
    {
        ULONG EntireField; // 0x0
        struct
        {
            ULONG NodeBlinkHigh : 21;   // 0x0
            ULONG NodeFlinkMiddle : 11; // 0x0
        } StandbyList;                  // 0x0
        struct
        {
            UCHAR ModifiedListBucketIndex : 4; // 0x0
        } MappedPageList;                      // 0x0
        struct
        {
            ULONG PageTableBlinkLow : 16;  // 0x0
            ULONG PageTableBuddyHigh : 10; // 0x0
            ULONG PageTableLinked : 1;     // 0x0
            ULONG AnchorLargePageSize : 2; // 0x0
            ULONG Spare1 : 3;              // 0x0
        } Active;                          // 0x0
    };
};

// 0x8 bytes (sizeof)
struct _MI_PFN_FLAGS4
{
    union
    {
        struct
        {
            ULONGLONG PteFrame : 40;                    // 0x0
            ULONGLONG ResidentPage : 1;                 // 0x0
            ULONGLONG ResidentPageContainsBadPages : 1; // 0x0
            ULONGLONG Unused1 : 1;                      // 0x0
            ULONGLONG Partition : 10;                   // 0x0
            ULONGLONG FileOnly : 1;                     // 0x0
            ULONGLONG PfnExists : 1;                    // 0x0
            ULONGLONG NodeFlinkHigh : 5;                // 0x0
            ULONGLONG PageIdentity : 3;                 // 0x0
            ULONGLONG PrototypePte : 1;                 // 0x0
        };
        ULONGLONG EntireField; // 0x0
    };
};

// 0x30 bytes (sizeof)
struct _MMPFN
{
    union
    {
        _LIST_ENTRY ListEntry;       // 0x0
        _RTL_BALANCED_NODE TreeNode; // 0x0
        struct
        {
            _MIPFNFLINK u1; // 0x0
            union
            {
                _MMPTE* PteAddress; // 0x8
                ULONGLONG PteLong;  // 0x8
            };
            _MMPTE OriginalPte; // 0x10
        };
    };
    _MIPFNBLINK u2; // 0x18
    union
    {
        struct
        {
            USHORT ReferenceCount; // 0x20
            _MMPFNENTRY1 e1;       // 0x22
        };
        struct
        {
            _MMPFNENTRY3 e3; // 0x23
            struct
            {
                USHORT ReferenceCount; // 0x20
            } e2;                      // 0x20
        };
        _MI_PFN_FLAGS e4; // 0x20
    } u3;                 // 0x20
    _MI_PFN_FLAGS5 u5;    // 0x24
    _MI_PFN_FLAGS4 u4;    // 0x28
};
