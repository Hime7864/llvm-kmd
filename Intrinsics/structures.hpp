#pragma once

// --- kmd_types.hpp ---
// 128-bit integers
typedef unsigned __int128 UINT128;
typedef __int128 INT128;

// 64-bit integers
typedef unsigned long long UINT64;
typedef unsigned long long ULONGLONG;
typedef unsigned long long UINT3264;
typedef unsigned long long ULONG64;
typedef unsigned long long UINT64;
typedef unsigned long long SIZE_T;
typedef long long LONGLONG;
typedef long long INT64;
typedef long long LONG64;

// 32-bit integers
typedef unsigned long UINT32;
typedef unsigned long UINT32;
typedef unsigned long UINTN;
typedef unsigned long ULONG;
typedef long INT32;
typedef long LONG;
typedef int INT;
typedef unsigned int UINT;

// 16-bit integers
typedef unsigned short UINT16;
typedef unsigned short UINT16;
typedef unsigned short USHORT;
typedef short INT16;
typedef short SHORT;

// 8-bit integers
typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef char INT8;
typedef char CHAR;
typedef char CCHAR;

// Floating point
typedef double DOUBLE;
typedef float FLOAT;

// Boolean types
typedef bool BOOLEAN;
typedef int BOOL;

// Character types
typedef wchar_t WCHAR;

// Void type
typedef void VOID;

// Integer pointers (pointer-sized integers, not pointers to integers)
typedef long long INT_PTR;
typedef unsigned long long UINT_PTR;
typedef long long LONG_PTR;
typedef unsigned long long ULONG_PTR;

// Generic pointers
typedef void* PVOID;
typedef const void* PCVOID;
typedef void* LPVOID;

// Byte pointers
typedef unsigned char* PBYTE;
typedef unsigned char* LPBYTE;

// Character pointers
typedef char* PCHAR;
typedef char* LPSTR;
typedef const char* PCSTR;
typedef const char* LPCSTR;

// Wide character pointers
typedef wchar_t* LPWSTR;
typedef const wchar_t* PCWSTR;
typedef const wchar_t* LPCWSTR;

// Numeric type pointers
typedef short* PSHORT;
typedef unsigned short* PUSHORT;
typedef int* PINT;
typedef unsigned int* PUINT;
typedef long* PLONG;
typedef unsigned long* PULONG;
typedef UINT32* PUINT32;
typedef UINT3264* PUINT3264;
typedef UINT64* PUINT64;
typedef INT64* PINT64;

// Size and pointer-sized pointers
typedef SIZE_T* PSIZE_T;
typedef INT_PTR* PINT_PTR;
typedef UINT_PTR* PUINT_PTR;
typedef LONG_PTR* PLONG_PTR;
typedef ULONG_PTR* PULONG_PTR;

// Boolean pointers
typedef BOOLEAN* PBOOLEAN;
typedef BOOL* PBOOL;

typedef long HRESULT;
typedef long NTSTATUS;
typedef unsigned long long PHYSICAL_ADDRESS;
typedef PHYSICAL_ADDRESS* PPHYSICAL_ADDRESS;
typedef long long HANDLE;
typedef long long* PHANDLE;
typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY* PKAFFINITY;
typedef UINT8 KIRQL;
typedef KIRQL* PKIRQL;
typedef ULONG_PTR KSPIN_LOCK;
typedef KSPIN_LOCK* PKSPIN_LOCK;
typedef LONG KPRIORITY;
typedef UINT64 POOL_FLAGS;

// Access control
typedef UINT32 ACCESS_MASK;
typedef ACCESS_MASK* PACCESS_MASK;

#define DeclDataType(type, title)    \
    type _##title;                   \
    typedef type _##title title;     \
    typedef type _##title* P##title; \
    typedef type _##title const* PC##title

// Basic structures
DeclDataType(struct, UNICODE_STRING);
DeclDataType(struct, ANSI_STRING);
DeclDataType(struct, LARGE_INTEGER);
DeclDataType(struct, ULARGE_INTEGER);
DeclDataType(struct, LIST_ENTRY);
DeclDataType(struct, SINGLE_LIST_ENTRY);
DeclDataType(struct, RTL_BITMAP);
DeclDataType(struct, RTL_AVL_TREE);
DeclDataType(struct, RTL_BALANCED_NODE);
DeclDataType(struct, GUID);

// Memory management structures
DeclDataType(struct, MMPTE_HARDWARE);
DeclDataType(struct, MMPTE);
DeclDataType(struct, MMPFN);
DeclDataType(struct, MDL);
DeclDataType(struct, PHYSICAL_MEMORY_RANGE);

// Process and thread structures
DeclDataType(struct, EPROCESS);
DeclDataType(struct, ETHREAD);
DeclDataType(struct, KTHREAD);
DeclDataType(struct, KPROCESS);
DeclDataType(struct, KPRCB);
DeclDataType(struct, KPCR);

// Driver and device structures
DeclDataType(struct, DRIVER_OBJECT);
DeclDataType(struct, DEVICE_OBJECT);
DeclDataType(struct, FILE_OBJECT);

// I/O structures
DeclDataType(struct, IRP);
DeclDataType(struct, IO_STATUS_BLOCK);
DeclDataType(struct, IO_STACK_LOCATION);
DeclDataType(struct, IO_WORKITEM);
DeclDataType(struct, FILE_BASIC_INFORMATION);
DeclDataType(struct, FILE_STANDARD_INFORMATION);
DeclDataType(struct, FILE_POSITION_INFORMATION);
DeclDataType(struct, FILE_NETWORK_OPEN_INFORMATION);
DeclDataType(struct, FILE_NAME_INFORMATION);
DeclDataType(struct, KDEVICE_QUEUE_ENTRY);

// Synchronization structures
DeclDataType(struct, KEVENT);
DeclDataType(struct, KMUTEX);
DeclDataType(struct, KSEMAPHORE);
DeclDataType(struct, FAST_MUTEX);
DeclDataType(struct, DISPATCHER_HEADER);

// APC structures
DeclDataType(struct, KAPC);
DeclDataType(struct, KAPC_STATE);

// Context and state
DeclDataType(struct, CONTEXT);
DeclDataType(struct, XSAVE_FORMAT);
DeclDataType(struct, XSAVE_AREA);
DeclDataType(struct, M128A);
DeclDataType(struct, MACHINE_FRAME);

// Object management
DeclDataType(struct, OBJECT_ATTRIBUTES);
DeclDataType(struct, OBJECT_TYPE);
DeclDataType(struct, OBJECT_HANDLE_INFORMATION);
DeclDataType(struct, CLIENT_ID);

// Descriptor tables
DeclDataType(struct, DESCRIPTOR_TABLE_REGISTER);
DeclDataType(struct, SEGMENT_SELECTOR);

DeclDataType(enum, KPROCESSOR_MODE);
DeclDataType(enum, POOL_TYPE);
DeclDataType(enum, MEMORY_CACHING_TYPE);
DeclDataType(enum, MODE);
DeclDataType(enum, KWAIT_REASON);
DeclDataType(enum, KAPC_ENVIRONMENT);
DeclDataType(enum, EVENT_TYPE);
DeclDataType(enum, LOCK_OPERATION);
DeclDataType(enum, WORK_QUEUE_TYPE);
DeclDataType(enum, FILE_INFORMATION_CLASS);
DeclDataType(enum, MM_PAGE_PRIORITY);
DeclDataType(enum, IO_PRIORITY_HINT);

struct SEGMENT_REGISTER;

#undef DeclDataType

typedef void(__stdcall* PIO_APC_ROUTINE)(
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG Reserved);

typedef VOID(__stdcall* PIO_WORKITEM_ROUTINE)(
    PDEVICE_OBJECT DeviceObject,
    PVOID Context);

typedef NTSTATUS(__stdcall* PDRIVER_INITIALIZE)(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath);

typedef void(__stdcall* PDRIVER_UNLOAD)(
    PDRIVER_OBJECT DriverObject);

typedef NTSTATUS(__stdcall* PDRIVER_DISPATCH)(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp);

typedef NTSTATUS (*PKSTART_ROUTINE)(PVOID StartContext);

typedef void(__stdcall* PKDEFERRED_ROUTINE)(
    PKAPC Apc,
    PVOID* NormalRoutine,
    PVOID* NormalContext,
    PVOID* SystemArgument1,
    PVOID* SystemArgument2);

typedef void(__stdcall* PKNORMAL_ROUTINE)(
    PVOID NormalContext,
    PVOID SystemArgument1,
    PVOID SystemArgument2);

typedef void(__stdcall* PKRUNDOWN_ROUTINE)(
    PKAPC Apc);

#define NULL nullptr
#define TRUE true
#define FALSE false

// Min/Max macros
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

// SAL annotations (Source Annotation Language)
#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_
#define _In_reads_(size)
#define _Out_writes_(size)
#define _In_reads_bytes_(size)
#define _In_reads_bytes_opt_(size)
#define _Out_writes_bytes_(size)
#define _Out_writes_bytes_opt_(size)

#define CONST const

#define DELETE (0x00010000L)
#define READ_CONTROL (0x00020000L)
#define WRITE_DAC (0x00040000L)
#define WRITE_OWNER (0x00080000L)
#define SYNCHRONIZE (0x00100000L)

#define STANDARD_RIGHTS_REQUIRED (0x000F0000L)
#define STANDARD_RIGHTS_READ (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE (READ_CONTROL)
#define STANDARD_RIGHTS_ALL (0x001F0000L)

#define SPECIFIC_RIGHTS_ALL (0x0000FFFFL)

#define GENERIC_READ (0x80000000L)
#define GENERIC_WRITE (0x40000000L)
#define GENERIC_EXECUTE (0x20000000L)
#define GENERIC_ALL (0x10000000L)

#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_DEVICE 0x00000040
#define FILE_ATTRIBUTE_TEMPORARY 0x00000100

#define FILE_SHARE_READ 0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define FILE_SHARE_DELETE 0x00000004

#define FILE_SUPERSEDE 0x00000000
#define FILE_OPEN 0x00000001
#define FILE_CREATE 0x00000002
#define FILE_OPEN_IF 0x00000003
#define FILE_OVERWRITE 0x00000004
#define FILE_OVERWRITE_IF 0x00000005
#define FILE_MAXIMUM_DISPOSITION 0x00000005

#define FILE_SYNCHRONOUS_IO_NONALERT 0x00000020
#define FILE_NON_DIRECTORY_FILE 0x00000040

#define OBJ_INHERIT 0x00000002L
#define OBJ_PERMANENT 0x00000010L
#define OBJ_EXCLUSIVE 0x00000020L
#define OBJ_CASE_INSENSITIVE 0x00000040L
#define OBJ_OPENIF 0x00000080L
#define OBJ_OPENLINK 0x00000100L
#define OBJ_KERNEL_HANDLE 0x00000200L
#define OBJ_FORCE_ACCESS_CHECK 0x00000400L
#define OBJ_VALID_ATTRIBUTES 0x000007F2L

#define InitializeObjectAttributes(p, n, a, r, s) \
    {                                             \
        (p)->Length = sizeof(_OBJECT_ATTRIBUTES); \
        (p)->RootDirectory = r;                   \
        (p)->Attributes = a;                      \
        (p)->ObjectName = n;                      \
        (p)->SecurityDescriptor = s;              \
        (p)->SecurityQualityOfService = NULL;     \
    }

#define PASSIVE_LEVEL 0
#define LOW_LEVEL 0
#define APC_LEVEL 1
#define DISPATCH_LEVEL 2
#define CMCI_LEVEL 5
#define CLOCK_LEVEL 13
#define IPI_LEVEL 14
#define DRS_LEVEL 14
#define POWER_LEVEL 14
#define PROFILE_LEVEL 15
#define HIGH_LEVEL 15

#define MM_COPY_MEMORY_PHYSICAL 0x1
#define MM_COPY_MEMORY_VIRTUAL 0x2

#define MM_ANY_NODE_OK 0x80000000UL
#define MdlMappingNoWrite 0x80000000UL
#define MdlMappingNoExecute 0x40000000UL

#define IO_NO_INCREMENT 0
#define IO_DISK_INCREMENT 1
#define IO_CD_ROM_INCREMENT 1
#define IO_NETWORK_INCREMENT 2
#define IO_KEYBOARD_INCREMENT 6
#define IO_MOUSE_INCREMENT 6
#define IO_SOUND_INCREMENT 8

#define FILE_DEVICE_BEEP 0x00000001
#define FILE_DEVICE_CD_ROM 0x00000002
#define FILE_DEVICE_DISK 0x00000007
#define FILE_DEVICE_KEYBOARD 0x0000000b
#define FILE_DEVICE_MOUSE 0x0000000f
#define FILE_DEVICE_NETWORK 0x00000012
#define FILE_DEVICE_UNKNOWN 0x00000022
#define FILE_DEVICE_VIDEO 0x00000023

#define PAGE_SIZE 0x1000
#define PAGE_SHIFT 12
#define LARGE_PAGE_SIZE 0x200000
#define LARGE_PAGE_SHIFT 21
#define HUGE_PAGE_SIZE 0x40000000
#define HUGE_PAGE_SHIFT 30

#define PAGE_LEVEL_4KB 3
#define PAGE_LEVEL_2MB 2
#define PAGE_LEVEL_1GB 1

#define PAGE_ALIGN(va) ((PVOID)((ULONG_PTR)(va) & ~(PAGE_SIZE - 1)))
#define PAGE_ALIGN_64K(va) ((PVOID)((ULONG_PTR)(va) & ~(0x10000 - 1)))
#define ROUND_TO_PAGES(size) (((ULONG_PTR)(size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define BYTES_TO_PAGES(size) (((size) >> PAGE_SHIFT) + (((size) & (PAGE_SIZE - 1)) != 0))
#define BYTE_OFFSET(va) ((ULONG)((LONG_PTR)(va) & (PAGE_SIZE - 1)))
#define ADDRESS_AND_SIZE_TO_SPAN_PAGES(va, size) \
    ((ULONG)((((ULONG_PTR)(va) & (PAGE_SIZE - 1)) + (size) + (PAGE_SIZE - 1)) >> PAGE_SHIFT))

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status) ((((ULONG)(Status)) >> 30) == 1)
#define NT_WARNING(Status) ((((ULONG)(Status)) >> 30) == 2)
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED ((NTSTATUS)0xC0000002L)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_VIOLATION ((NTSTATUS)0xC0000005L)
#define STATUS_INVALID_HANDLE ((NTSTATUS)0xC0000008L)
#define STATUS_INVALID_PARAMETER ((NTSTATUS)0xC000000DL)
#define STATUS_NO_SUCH_DEVICE ((NTSTATUS)0xC000000EL)
#define STATUS_NO_SUCH_FILE ((NTSTATUS)0xC000000FL)
#define STATUS_INVALID_DEVICE_REQUEST ((NTSTATUS)0xC0000010L)
#define STATUS_END_OF_FILE ((NTSTATUS)0xC0000011L)
#define STATUS_NO_MEMORY ((NTSTATUS)0xC0000017L)
#define STATUS_ACCESS_DENIED ((NTSTATUS)0xC0000022L)
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#define STATUS_OBJECT_TYPE_MISMATCH ((NTSTATUS)0xC0000024L)
#define STATUS_OBJECT_NAME_INVALID ((NTSTATUS)0xC0000033L)
#define STATUS_OBJECT_NAME_NOT_FOUND ((NTSTATUS)0xC0000034L)
#define STATUS_OBJECT_NAME_COLLISION ((NTSTATUS)0xC0000035L)
#define STATUS_PROCEDURE_NOT_FOUND ((NTSTATUS)0xC000007AL)
#define STATUS_INSUFFICIENT_RESOURCES ((NTSTATUS)0xC000009AL)
#define UNSUPPORTED_PROCESSOR ((NTSTATUS)0x0000005DL)
#define STATUS_NOT_SUPPORTED ((NTSTATUS)0xC00000BBL)
#define STATUS_INVALID_PARAMETER_1 ((NTSTATUS)0xC00000EFL)
#define STATUS_INVALID_PARAMETER_2 ((NTSTATUS)0xC00000F0L)
#define STATUS_INVALID_PARAMETER_3 ((NTSTATUS)0xC00000F1L)
#define STATUS_INVALID_PARAMETER_4 ((NTSTATUS)0xC00000F2L)
#define STATUS_INVALID_ADDRESS ((NTSTATUS)0xC0000141L)
#define STATUS_PFN_NOT_PRESENT ((NTSTATUS)0xC00001FFUL)
#define STATUS_PTE_NOT_VALID ((NTSTATUS)0xC00002FFUL)

#define NMI_CB_POOL_TAG 'BCmN'

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg(args, type) __builtin_va_arg(args, type)
#define va_end __builtin_va_end

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define FORCEINLINE __attribute__((always_inline)) inline
#define NOINLINE __attribute__((noinline))
#define ALIGN(alignment) __attribute__((aligned(alignment)))
#define NORETURN __attribute__((noreturn))
#define DEPRECATED __attribute__((deprecated))
#define PRESERVE __attribute__((preserve_most))

#define CONTAINING_RECORD(address, type, field) \
    ((type*)((PCHAR)(address) - (ULONG_PTR)(&((type*)0)->field)))

#define FIELD_OFFSET(type, field) ((LONG)(LONG_PTR) & (((type*)0)->field))

#define RTL_FIELD_SIZE(type, field) (sizeof(((type*)0)->field))

#define RTL_SIZEOF_THROUGH_FIELD(type, field) \
    (FIELD_OFFSET(type, field) + RTL_FIELD_SIZE(type, field))

#define ARRAYSIZE(A) (sizeof(A) / sizeof((A)[0]))

// --- kmd_pattern.hpp ---
struct PACKED FUNCTION_TABLE_ENTRY
{
    UINT32 Hash;
    PVOID FunctionPtr;
};

template <size_t N>
volatile constexpr UINT32 str_hash(const char (&str)[N])
{
    UINT32 results = 0;
    auto count = N / 4 + ((N % 4) ? 1 : 0);
    for (size_t i = 0; i < count; i++)
        results += ((UINT32*)str)[i];
    return results;
}

constexpr UINT32 str_hash(const char* str, size_t N)
{
    UINT32 results = 0;
    auto count = N / 4;
    for (size_t i = 0; i < count; i++)
        results += ((UINT32*)str)[i];
    switch (N % 4)
    {
    case 3:
        results += ((UINT32*)str)[count] & 0x00FFFFFF;
        break;
    case 2:
        results += ((UINT32*)str)[count] & 0x0000FFFF;
        break;
    case 1:
        results += ((UINT32*)str)[count] & 0x000000FF;
    default:
        break;
    }
    return results;
}

constexpr const UCHAR to_nibble(char c)
{
    if (c >= '0' && c <= '9')
        return (UCHAR)(c - '0');
    if (c >= 'A' && c <= 'F')
        return (UCHAR)(c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return (UCHAR)(c - 'a' + 10);
    return 0;
}

template <size_t N>
constexpr size_t count_pattern_bytes(const char (&str)[N])
{
    size_t count = 0;
    for (size_t i = 0; i < N - 1;)
    {
        if (str[i] == '?')
            i += 2;
        else
            i += 3;
        count++;
    }
    return count;
}

template <size_t N, size_t Size>
struct Pattern
{
    UCHAR bytes[Size]{};
    UCHAR mask[Size]{};
    size_t size = Size;

    __forceinline constexpr Pattern(const char (&str)[N])
    {
        int idx = 0;
        for (int i = 0; i < N - 1 && idx < Size;)
        {
            if (str[i] == '?')
            {
                bytes[idx] = 0x00;
                mask[idx] = 0x00;
                idx++;
                i += 2;
            }
            else
            {
                bytes[idx] = ((UCHAR)to_nibble(str[i]) << 4) | (UCHAR)to_nibble(str[i + 1]);
                mask[idx] = 0xFF;
                idx++;
                i += 3;
            }
        }
    }
};

#define pattern(str) []() { \
    constexpr static auto pat = Pattern<sizeof(str) / sizeof(str[0]), count_pattern_bytes(str)>(str); \
    return pat; }()

class FnPtr
{
public:
    template <typename Ret, typename... Args>
    FORCEINLINE Ret invoke(Args... args)
    {
        return ((Ret(__fastcall*)(Args...))this)(args...);
    }

    template <typename Ret, typename... Args>
    FORCEINLINE Ret operator()(Args... args)
    {
        return invoke<Ret>(args...);
    }
};


// --- Architecture/layout.hpp ---
struct PACKED LINEAR_ADDRESS
{
    union
    {
        UINT64 AsUINT64;
        PVOID AsPointer;

        struct
        {
            UINT64 offset : 12;
            UINT64 pte_index : 9;
            UINT64 pde_index : 9;
            UINT64 pdpte_index : 9;
            UINT64 pml4e_index : 9;
            UINT64 sign_extend : 16;
        };

        struct
        {
            UINT64 large_offset : 21;
            UINT64 large_pde_index : 9;
            UINT64 large_pdpte_index : 9;
            UINT64 large_pml4e_index : 9;
            UINT64 large_sign_extend : 16;
        };

        struct
        {
            UINT64 huge_offset : 30;
            UINT64 huge_pdpte_index : 9;
            UINT64 huge_pml4e_index : 9;
            UINT64 huge_sign_extend : 16;
        };
    };

    LINEAR_ADDRESS()
        : AsUINT64(0)
    {
    }

    template <typename T>
    LINEAR_ADDRESS(T data)
        : AsUINT64((UINT64)data)
    {
    }

    template <typename T>
    FORCEINLINE T as() const
    {
        return reinterpret_cast<T>(AsUINT64);
    }

    FORCEINLINE bool is_valid() const
    {
        return AsUINT64 != 0 && sign_extend == 0xFFFF;
    }

    FORCEINLINE bool is_canonical() const
    {
        return (sign_extend == 0x0000) || (sign_extend == 0xFFFF);
    }
};

union IDT_GATE
{
    struct
    {
        UINT128 offset_low : 16;
        UINT128 selector : 16;
        UINT128 ist : 3;
        UINT128 : 5;
        UINT128 type : 4;
        UINT128 : 1;
        UINT128 dpl : 2;
        UINT128 p : 1;
        UINT128 offset_mid : 16;
        UINT128 offset_high : 32;
        UINT128 : 32;
    };
    UINT128 AsUINT128;

    FORCEINLINE UINT64 offset()
    {
        return ((UINT64)offset_high << 32) | ((UINT64)offset_mid << 16) | offset_low;
    }

    FORCEINLINE VOID offset(UINT64 data)
    {
        offset_low = (UINT16)(data & 0xFFFF);
        offset_mid = (UINT16)((data >> 16) & 0xFFFF);
        offset_high = (UINT32)((data >> 32) & 0xFFFFFFFF);
        return;
    }
};

union SEGMENT_DESCRIPTOR_ACCESS
{
    struct
    {
        UINT8 A : 1;
        UINT8 RW : 1;
        UINT8 DC : 1;
        UINT8 E : 1;
        UINT8 S : 1;
        UINT8 DPL : 2;
        UINT8 P : 1;
    };
    UINT8 AsUINT8;
};

union SEGMENT_DESCRIPTOR_FLAGS
{
    struct
    {
        UINT8 : 1;
        UINT8 L : 1;
        UINT8 DB : 1;
        UINT8 G : 1;
    };
    UINT8 AsUINT8;
};

union SEGMENT_DESCRIPTOR
{
    struct
    {
        UINT128 limitLow : 16;
        UINT128 baseLow : 16;
        UINT128 baseMid : 8;
        UINT128 access : 8;
        UINT128 limitHigh : 4;
        UINT128 flags : 4;
        UINT128 baseHigh : 8;
        UINT128 baseUpper : 32;
    };
    UINT128 AsUINT128;

    FORCEINLINE UINT64 base()
    {
        return ((UINT64)baseUpper << 32) | ((UINT64)baseHigh << 24) | ((UINT64)baseMid << 16) | ((UINT64)baseLow);
    }

    FORCEINLINE VOID base(UINT64 base)
    {
        baseLow = (UINT16)(base & 0xFFFF);
        baseMid = (UINT8)((base >> 16) & 0xFF);
        baseHigh = (UINT8)((base >> 24) & 0xFF);
        baseUpper = (UINT32)((base >> 32) & 0xFFFFFFFF);
        return;
    }

    FORCEINLINE UINT32 limit()
    {
        return ((UINT64)limitHigh << 16) | ((UINT64)limitLow);
    }

    FORCEINLINE VOID limit(UINT32 limit)
    {
        limitLow = (UINT16)(limit & 0xFFFF);
        limitHigh = (UINT8)((limit >> 16) & 0x0F);
        return;
    }
};

struct PACKED INTERUPT_STACK_TABLE
{
    struct
    {
        unsigned : 32;
    };
    UINT64 RSP[3];
    UINT64 IST[8];
    struct
    {
        unsigned : 32;
    };
    struct
    {
        unsigned : 32;
    };
    struct
    {
        UINT16 : 16;
    };
    UINT16 IOPB;
};

struct PACKED SEGMENT_REGISTER
{
    UINT16 Limit;
    UINT64 Base;

    FORCEINLINE UINT32 limit(USHORT selector) const
    {
        auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);

        UINT32 limitLow = *(reinterpret_cast<UINT16*>(&DescBytes[0]));
        UCHAR granFlags = DescBytes[6];
        UINT32 limitHigh = granFlags & 0x0F;
        UINT32 segLimit = (limitHigh << 16) | limitLow;

        if ((granFlags & 0x80) != 0)
            segLimit = (segLimit << 12) | 0xFFF;
        return segLimit;
    }

    FORCEINLINE USHORT attribute(USHORT selector) const
    {
        const auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);
        const USHORT access = static_cast<USHORT>(DescBytes[5]);
        const USHORT flags = static_cast<USHORT>((DescBytes[6] & 0xF0) >> 4);

        return static_cast<USHORT>((flags << 8) | access);
    }

    FORCEINLINE UINT64 base(USHORT selector) const
    {
        const auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);
        UINT32 baseLow = *(reinterpret_cast<UINT16*>(&DescBytes[2]));
        UINT32 baseMid = DescBytes[4];
        UINT32 baseHigh = DescBytes[7];
        UINT64 segBase = (static_cast<UINT64>(baseHigh) << 24) | (static_cast<UINT64>(baseMid) << 16) | baseLow;

        const UCHAR access = DescBytes[5];
        const bool system_desc = ((access & 0x10) == 0);
        if (system_desc && (offset + 15 <= sr->Limit))
        {
            const UINT32 baseUpper =
                (static_cast<UINT32>(DescBytes[8])) |
                (static_cast<UINT32>(DescBytes[9]) << 8) |
                (static_cast<UINT32>(DescBytes[10]) << 16) |
                (static_cast<UINT32>(DescBytes[11]) << 24);
            segBase |= (static_cast<UINT64>(baseUpper) << 32);
        }
        return segBase;
    }
};

enum ICR_MT : UINT32
{
    Fixed = 0,
    LowestPriority = 1,
    Smi = 2,
    RemoteRead = 3,
    Nmi = 4,
    Init = 5,
    StartUp = 6,
    ExtInt = 7
};

enum ICR_DM : UINT32
{
    Physical = 0,
    Logical = 1
};

enum ICR_DS : UINT32
{
    Idle = 0,
    SendPending = 1,
};

enum ICR_L : UINT32
{
    deassert = 0,
    assert = 1
};

enum ICR_TGM : UINT32
{
    edge = 0,
    level = 1
};

enum ICR_RRS : UINT32
{
    ReadInvalid = 0,
    DeliveryPending = 1,
    DeliveryDone = 2
};

enum ICR_DSH : UINT32
{
    Destination = 0,
    Self = 1,
    AllIncludingSelf = 2,
    AllExcludingSelf = 3
};

struct xAPIC_REGISTERS
{
    struct ICR_LOW
    {
        union
        {
            struct
            {
                UINT32 VEC : 8;
                ICR_MT MT : 3;
                ICR_DM DM : 1;
                ICR_DS DS : 1;
                UINT32 : 1;
                ICR_L L : 1;
                ICR_TGM TGM : 1;
                ICR_RRS RRS : 2;
                ICR_DSH DSH : 2;
                UINT32 : 12;
            };
            UINT32 AsUINT32;
        };
    };
    struct ICR_HIGH
    {
        union
        {
            struct
            {
                UINT32 : 24;
                UINT32 DES : 8;
            };
            UINT32 AsUINT32;
        };
    };

    bool isPending()
    {
        return ((ICR_LOW*)((UINT64)this + 0x300))->DS == ICR_DS::SendPending;
    }

    void WriteICR(ICR_LOW icr, ICR_HIGH dest)
    {
        if (icr.DSH == ICR_DSH::Destination)
            *(UINT32*)((UINT64)this + 0x314) = dest.AsUINT32;
        *(UINT32*)((UINT64)this + 0x300) = icr.AsUINT32;
        return;
    }
    void WriteICR(ICR_LOW icr)
    {
        if (icr.DSH == ICR_DSH::Destination)
            return;
        *(UINT32*)((UINT64)this + 0x300) = icr.AsUINT32;
        return;
    }
    void AddApicTimer(int delta)
    {
        UINT32 current = *(UINT32*)((UINT64)this + 0x390);
        *(UINT32*)((UINT64)this + 0x380) = current + delta;
        return;
    }
    UINT64 ApicTimerCurrent()
    {
        return *(UINT32*)((UINT64)this + 0x390);
    }
};


// --- nt/enums.hpp ---
enum _KPROCESSOR_MODE
{
    KernelMode,
    UserMode,
    MaximumMode
};

enum _MODE
{
    KernelModeEnum = 0,
    UserModeEnum = 1,
    MaximumModeEnum = 2
};

enum _POOL_TYPE
{
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

enum _EVENT_TYPE
{
    NotificationEvent = 0,
    SynchronizationEvent = 1
};

enum _LOCK_OPERATION
{
    IoReadAccess = 0,
    IoWriteAccess = 1,
    IoModifyAccess = 2
};

enum _WORK_QUEUE_TYPE
{
    CriticalWorkQueue = 0,
    DelayedWorkQueue = 1,
    HyperCriticalWorkQueue = 2,
    NormalWorkQueue = 3,
    BackgroundWorkQueue = 4,
    RealTimeWorkQueue = 5,
    SuperCriticalWorkQueue = 6,
    MaximumWorkQueue = 7
};

enum _MM_PAGE_PRIORITY
{
    LowPagePriority = 0,
    NormalPagePriority = 16,
    HighPagePriority = 32
};

enum _KAPC_ENVIRONMENT
{
    OriginalApcEnvironment = 0,
    AttachedApcEnvironment = 1,
    CurrentApcEnvironment = 2,
    InsertApcEnvironment = 3
};

enum _IO_PRIORITY_HINT
{
    IoPriorityVeryLow = 0,
    IoPriorityLow = 1,
    IoPriorityNormal = 2,
    IoPriorityHigh = 3,
    IoPriorityCritical = 4,
    MaxIoPriorityTypes = 5
};

enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation = 2,
    FileBothDirectoryInformation = 3,
    FileBasicInformation = 4,
    FileStandardInformation = 5,
    FileInternalInformation = 6,
    FileEaInformation = 7,
    FileAccessInformation = 8,
    FileNameInformation = 9,
    FileRenameInformation = 10,
    FileLinkInformation = 11,
    FileNamesInformation = 12,
    FileDispositionInformation = 13,
    FilePositionInformation = 14,
    FileFullEaInformation = 15,
    FileModeInformation = 16,
    FileAlignmentInformation = 17,
    FileAllInformation = 18,
    FileAllocationInformation = 19,
    FileEndOfFileInformation = 20,
    FileAlternateNameInformation = 21,
    FileStreamInformation = 22,
    FilePipeInformation = 23,
    FilePipeLocalInformation = 24,
    FilePipeRemoteInformation = 25,
    FileMailslotQueryInformation = 26,
    FileMailslotSetInformation = 27,
    FileCompressionInformation = 28,
    FileObjectIdInformation = 29,
    FileCompletionInformation = 30,
    FileMoveClusterInformation = 31,
    FileQuotaInformation = 32,
    FileReparsePointInformation = 33,
    FileNetworkOpenInformation = 34,
    FileAttributeTagInformation = 35,
    FileTrackingInformation = 36,
    FileIdBothDirectoryInformation = 37,
    FileIdFullDirectoryInformation = 38,
    FileValidDataLengthInformation = 39,
    FileShortNameInformation = 40,
    FileIoCompletionNotificationInformation = 41,
    FileIoStatusBlockRangeInformation = 42,
    FileIoPriorityHintInformation = 43,
    FileSfioReserveInformation = 44,
    FileSfioVolumeInformation = 45,
    FileHardLinkInformation = 46,
    FileProcessIdsUsingFileInformation = 47,
    FileNormalizedNameInformation = 48,
    FileNetworkPhysicalNameInformation = 49,
    FileIdGlobalTxDirectoryInformation = 50,
    FileIsRemoteDeviceInformation = 51,
    FileUnusedInformation = 52,
    FileNumaNodeInformation = 53,
    FileStandardLinkInformation = 54,
    FileRemoteProtocolInformation = 55,
    FileRenameInformationBypassAccessCheck = 56,
    FileLinkInformationBypassAccessCheck = 57,
    FileVolumeNameInformation = 58,
    FileIdInformation = 59,
    FileIdExtdDirectoryInformation = 60,
    FileReplaceCompletionInformation = 61,
    FileHardLinkFullIdInformation = 62,
    FileIdExtdBothDirectoryInformation = 63,
    FileDispositionInformationEx = 64,
    FileRenameInformationEx = 65,
    FileRenameInformationExBypassAccessCheck = 66,
    FileDesiredStorageClassInformation = 67,
    FileStatInformation = 68,
    FileMemoryPartitionInformation = 69,
    FileStatLxInformation = 70,
    FileCaseSensitiveInformation = 71,
    FileLinkInformationEx = 72,
    FileLinkInformationExBypassAccessCheck = 73,
    FileStorageReserveIdInformation = 74,
    FileCaseSensitiveInformationForceAccessCheck = 75,
    FileKnownFolderInformation = 76,
    FileMaximumInformation = 77
};

enum _KWAIT_REASON
{
    Executive = 0,
    FreePage = 1,
    PageIn = 2,
    PoolAllocation = 3,
    DelayExecution = 4,
    Suspended = 5,
    UserRequest = 6,
    WrExecutive = 7,
    WrFreePage = 8,
    WrPageIn = 9,
    WrPoolAllocation = 10,
    WrDelayExecution = 11,
    WrSuspended = 12,
    WrUserRequest = 13,
    WrEventPair = 14,
    WrQueue = 15,
    WrLpcReceive = 16,
    WrLpcReply = 17,
    WrVirtualMemory = 18,
    WrPageOut = 19,
    WrRendezvous = 20,
    WrKeyedEvent = 21,
    WrTerminated = 22,
    WrProcessInSwap = 23,
    WrCpuRateControl = 24,
    WrCalloutStack = 25,
    WrKernel = 26,
    WrResource = 27,
    WrPushLock = 28,
    WrMutex = 29,
    WrQuantumEnd = 30,
    WrDispatchInt = 31,
    WrPreempted = 32,
    WrYieldExecution = 33,
    WrFastMutex = 34,
    WrGuardedMutex = 35,
    WrRundown = 36,
    WrAlertByThreadId = 37,
    WrDeferredPreempt = 38,
    MaximumWaitReason = 39
};

// --- nt/struct/base.hpp ---
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

// --- nt/struct/xsave.hpp ---
// 0x10 bytes (sizeof)
struct _M128A
{
    UINT64 Low; // 0x0
    INT64 High; // 0x8
};

// 0x200 bytes (sizeof)
struct _XSAVE_FORMAT
{
    UINT16 ControlUINT16;      // 0x0
    UINT16 StatusUINT16;       // 0x2
    UINT8 TagUINT16;           // 0x4
    UINT8 Reserved1;         // 0x5
    UINT16 ErrorOpcode;      // 0x6
    UINT32 ErrorOffset;      // 0x8
    UINT16 ErrorSelector;    // 0xc
    UINT16 Reserved2;        // 0xe
    UINT32 DataOffset;       // 0x10
    UINT16 DataSelector;     // 0x14
    UINT16 Reserved3;        // 0x16
    UINT32 MxCsr;            // 0x18
    UINT32 MxCsr_Mask;       // 0x1c
    M128A FloatRegisters[8]; // 0x20
    M128A XmmRegisters[16];  // 0xa0
    UINT8 Reserved4[96];     // 0x1a0
};

// --- nt/struct/context.hpp ---
struct _CONTEXT
{
    UINT64 P1Home;       // 0x0
    UINT64 P2Home;       // 0x8
    UINT64 P3Home;       // 0x10
    UINT64 P4Home;       // 0x18
    UINT64 P5Home;       // 0x20
    UINT64 P6Home;       // 0x28
    UINT32 ContextFlags; // 0x30
    UINT32 MxCsr;        // 0x34
    UINT16 SegCs;        // 0x38
    UINT16 SegDs;        // 0x3a
    UINT16 SegEs;        // 0x3c
    UINT16 SegFs;        // 0x3e
    UINT16 SegGs;        // 0x40
    UINT16 SegSs;        // 0x42
    UINT32 EFlags;       // 0x44
    UINT64 Dr0;          // 0x48
    UINT64 Dr1;          // 0x50
    UINT64 Dr2;          // 0x58
    UINT64 Dr3;          // 0x60
    UINT64 Dr6;          // 0x68
    UINT64 Dr7;          // 0x70
    UINT64 Rax;          // 0x78
    UINT64 Rcx;          // 0x80
    UINT64 Rdx;          // 0x88
    UINT64 Rbx;          // 0x90
    UINT64 Rsp;          // 0x98
    UINT64 Rbp;          // 0xa0
    UINT64 Rsi;          // 0xa8
    UINT64 Rdi;          // 0xb0
    UINT64 R8;           // 0xb8
    UINT64 R9;           // 0xc0
    UINT64 R10;          // 0xc8
    UINT64 R11;          // 0xd0
    UINT64 R12;          // 0xd8
    UINT64 R13;          // 0xe0
    UINT64 R14;          // 0xe8
    UINT64 R15;          // 0xf0
    UINT64 Rip;          // 0xf8
    union
    {
        XSAVE_FORMAT FltSave; // 0x100
        struct
        {
            M128A Header[2]; // 0x100
            M128A Legacy[8]; // 0x120
            M128A Xmm0;      // 0x1a0
            M128A Xmm1;      // 0x1b0
            M128A Xmm2;      // 0x1c0
            M128A Xmm3;      // 0x1d0
            M128A Xmm4;      // 0x1e0
            M128A Xmm5;      // 0x1f0
            M128A Xmm6;      // 0x200
            M128A Xmm7;      // 0x210
            M128A Xmm8;      // 0x220
            M128A Xmm9;      // 0x230
            M128A Xmm10;     // 0x240
            M128A Xmm11;     // 0x250
            M128A Xmm12;     // 0x260
            M128A Xmm13;     // 0x270
            M128A Xmm14;     // 0x280
            M128A Xmm15;     // 0x290
        };
    };
    M128A VectorRegister[26];    // 0x300
    UINT64 VectorControl;        // 0x4a0
    UINT64 DebugControl;         // 0x4a8
    UINT64 LastBranchToRip;      // 0x4b0
    UINT64 LastBranchFromRip;    // 0x4b8
    UINT64 LastExceptionToRip;   // 0x4c0
    UINT64 LastExceptionFromRip; // 0x4c8
};

// --- nt/struct/mm.hpp ---
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

//0x4 bytes (sizeof)
struct _PROCESSOR_NUMBER
{
    USHORT Group;   //0x0
    UCHAR Number;   //0x2
    UCHAR Reserved; //0x3
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

// --- nt/struct/wait.hpp ---
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

// --- nt/struct/io.hpp ---
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
