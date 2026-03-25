#pragma once

#include "../fwd.hpp"

// 0x10 bytes (sizeof)
struct _ANSI_STRING
{
    UINT16 Length;        // 0x0
    UINT16 MaximumLength; // 0x2
    PCHAR Buffer;         // 0x8
};

// 0x10 bytes (sizeof)
struct _UNICODE_STRING
{
    UINT16 Length;        // 0x0
    UINT16 MaximumLength; // 0x2
    LPWSTR Buffer;        // 0x8
};

// 0x8 bytes (sizeof)

struct _LARGE_INTEGER
{
    union
    {
        INT64 QuadPart; // 0x0
        struct
        {
            UINT32 LowPart; // 0x0
            INT32 HighPart; // 0x4
        };
    };
};

// 0x8 bytes (sizeof)
struct _SINGLE_LIST_ENTRY
{
    struct _SINGLE_LIST_ENTRY* Next; // 0x0
};

// 0x10 bytes (sizeof)
struct _LIST_ENTRY
{
    PLIST_ENTRY Flink; // 0x0
    PLIST_ENTRY Blink; // 0x8
};

struct _OBJECT_ATTRIBUTES
{
    UINT32 Length;                  // 0x0
    PVOID RootDirectory;            // 0x8
    PUNICODE_STRING ObjectName;     // 0x10
    UINT32 Attributes;              // 0x18
    PVOID SecurityDescriptor;       // 0x20
    PVOID SecurityQualityOfService; // 0x28
};

// 0x10 bytes (sizeof)
struct _CLIENT_ID
{
    PVOID UniqueProcess; // 0x0
    PVOID UniqueThread;  // 0x8
};

// 0x10 bytes (sizeof)
struct _GUID
{
    ULONG Data1;    // 0x0
    USHORT Data2;   // 0x4
    USHORT Data3;   // 0x6
    UCHAR Data4[8]; // 0x8
};

// 0x10 bytes (sizeof)
struct _RTL_BITMAP
{
    ULONG SizeOfBitMap; // 0x0
    PULONG Buffer;      // 0x8
};

// 0x8 bytes (sizeof)
struct _OBJECT_HANDLE_INFORMATION
{
    UINT32 HandleAttributes;   // 0x0
    ACCESS_MASK GrantedAccess; // 0x4
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

struct _PHYSICAL_MEMORY_RANGE
{
    LARGE_INTEGER BaseAddress;   // 0x0
    LARGE_INTEGER NumberOfBytes; // 0x8
};

struct _MACHINE_FRAME
{
    ULONGLONG Rip;   // 0x0
    USHORT SegCs;    // 0x8
    USHORT Fill1[3]; // 0xa
    ULONG EFlags;    // 0x10
    ULONG Fill2;     // 0x14
    ULONGLONG Rsp;   // 0x18
    USHORT SegSs;    // 0x20
    USHORT Fill3[3]; // 0x22
};
