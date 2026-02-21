#pragma once

// 128-bit integers
typedef unsigned __int128 UINT128;
typedef __int128 INT128;

// 64-bit integers
typedef unsigned long long UINT64;
typedef unsigned long long ULONGLONG;
typedef unsigned long long DWORD64;
typedef unsigned long long ULONG64;
typedef unsigned long long QWORD;
typedef unsigned long long SIZE_T;
typedef long long LONGLONG;
typedef long long INT64;
typedef long long LONG64;

// 32-bit integers
typedef unsigned long UINT32;
typedef unsigned long DWORD;
typedef unsigned long UINTN;
typedef unsigned long ULONG;
typedef long INT32;
typedef long LONG;
typedef int INT;
typedef unsigned int UINT;

// 16-bit integers
typedef unsigned short UINT16;
typedef unsigned short WORD;
typedef unsigned short USHORT;
typedef short INT16;
typedef short SHORT;

// 8-bit integers
typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef char INT8;
typedef char CHAR;

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
typedef DWORD* PDWORD;
typedef DWORD64* PDWORD64;
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

// Access control
typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK* PACCESS_MASK;


#define DeclDataType(type, title) \
    type _##title; \
    typedef type _##title title; \
    typedef type _##title* P##title; \
    typedef type _##title const* PC##title

// Basic structures
DeclDataType(struct, UNICODE_STRING);
DeclDataType(struct, ANSI_STRING);
DeclDataType(struct, LARGE_INTEGER);
DeclDataType(struct, ULARGE_INTEGER);
DeclDataType(struct, LIST_ENTRY);
DeclDataType(struct, SINGLE_LIST_ENTRY);
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
DeclDataType(struct, KDEVICE_QUEUE_ENTRY);

// Synchronization structures
DeclDataType(struct, KEVENT);
DeclDataType(struct, KMUTEX);
DeclDataType(struct, KSEMAPHORE);
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

struct SEGMENT_REGISTER;

#undef DeclDataType

typedef void(__stdcall* PIO_APC_ROUTINE)(
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG Reserved
    );

typedef NTSTATUS(__stdcall* PDRIVER_INITIALIZE)(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
    );

typedef void(__stdcall* PDRIVER_UNLOAD)(
    PDRIVER_OBJECT DriverObject
    );

typedef NTSTATUS(__stdcall* PDRIVER_DISPATCH)(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
    );

typedef NTSTATUS(*PKSTART_ROUTINE)(PVOID StartContext);

typedef void(__stdcall* PKDEFERRED_ROUTINE)(
    PKAPC Apc,
    PVOID* NormalRoutine,
    PVOID* NormalContext,
    PVOID* SystemArgument1,
    PVOID* SystemArgument2
    );

typedef void(__stdcall* PKNORMAL_ROUTINE)(
    PVOID NormalContext,
    PVOID SystemArgument1,
    PVOID SystemArgument2
    );

typedef void(__stdcall* PKRUNDOWN_ROUTINE)(
    PKAPC Apc
    );

#define NULL nullptr
#define TRUE true
#define FALSE false

// Min/Max macros
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

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
#define _Out_writes_bytes_(size)

#define CONST const

#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)

#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)
#define STANDARD_RIGHTS_READ             (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE            (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE          (READ_CONTROL)
#define STANDARD_RIGHTS_ALL              (0x001F0000L)

#define SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define FILE_ATTRIBUTE_NORMAL            0x00000080
#define FILE_ATTRIBUTE_READONLY          0x00000001
#define FILE_ATTRIBUTE_HIDDEN            0x00000002
#define FILE_ATTRIBUTE_SYSTEM            0x00000004
#define FILE_ATTRIBUTE_DIRECTORY         0x00000010
#define FILE_ATTRIBUTE_ARCHIVE           0x00000020
#define FILE_ATTRIBUTE_DEVICE            0x00000040
#define FILE_ATTRIBUTE_TEMPORARY         0x00000100

#define FILE_SHARE_READ                  0x00000001
#define FILE_SHARE_WRITE                 0x00000002
#define FILE_SHARE_DELETE                0x00000004

#define FILE_SUPERSEDE                   0x00000000
#define FILE_OPEN                        0x00000001
#define FILE_CREATE                      0x00000002
#define FILE_OPEN_IF                     0x00000003
#define FILE_OVERWRITE                   0x00000004
#define FILE_OVERWRITE_IF                0x00000005
#define FILE_MAXIMUM_DISPOSITION         0x00000005

#define FILE_SYNCHRONOUS_IO_NONALERT     0x00000020
#define FILE_NON_DIRECTORY_FILE          0x00000040

#define OBJ_INHERIT                      0x00000002L
#define OBJ_PERMANENT                    0x00000010L
#define OBJ_EXCLUSIVE                    0x00000020L
#define OBJ_CASE_INSENSITIVE             0x00000040L
#define OBJ_OPENIF                       0x00000080L
#define OBJ_OPENLINK                     0x00000100L
#define OBJ_KERNEL_HANDLE                0x00000200L
#define OBJ_FORCE_ACCESS_CHECK           0x00000400L
#define OBJ_VALID_ATTRIBUTES             0x000007F2L

#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->Length = sizeof(_OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
    (p)->SecurityQualityOfService = NULL; \
}

#define PASSIVE_LEVEL                    0
#define LOW_LEVEL                        0
#define APC_LEVEL                        1
#define DISPATCH_LEVEL                   2
#define CMCI_LEVEL                       5
#define CLOCK_LEVEL                      13
#define IPI_LEVEL                        14
#define DRS_LEVEL                        14
#define POWER_LEVEL                      14
#define PROFILE_LEVEL                    15
#define HIGH_LEVEL                       15

#define MM_COPY_MEMORY_PHYSICAL          0x1
#define MM_COPY_MEMORY_VIRTUAL           0x2

#define MM_ANY_NODE_OK                   0x80000000UL

#define PAGE_SIZE                        0x1000
#define PAGE_SHIFT                       12
#define LARGE_PAGE_SIZE                  0x200000
#define LARGE_PAGE_SHIFT                 21
#define HUGE_PAGE_SIZE                   0x40000000
#define HUGE_PAGE_SHIFT                  30

#define PAGE_ALIGN(va)                   ((PVOID)((ULONG_PTR)(va) & ~(PAGE_SIZE - 1)))
#define PAGE_ALIGN_64K(va)               ((PVOID)((ULONG_PTR)(va) & ~(0x10000 - 1)))
#define ROUND_TO_PAGES(size)             (((ULONG_PTR)(size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define BYTES_TO_PAGES(size)             (((size) >> PAGE_SHIFT) + (((size) & (PAGE_SIZE - 1)) != 0))
#define BYTE_OFFSET(va)                  ((ULONG)((LONG_PTR)(va) & (PAGE_SIZE - 1)))
#define ADDRESS_AND_SIZE_TO_SPAN_PAGES(va, size) \
    ((ULONG)((((ULONG_PTR)(va) & (PAGE_SIZE - 1)) + (size) + (PAGE_SIZE - 1)) >> PAGE_SHIFT))

#define NT_SUCCESS(Status)               (((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status)           ((((ULONG)(Status)) >> 30) == 1)
#define NT_WARNING(Status)               ((((ULONG)(Status)) >> 30) == 2)
#define NT_ERROR(Status)                 ((((ULONG)(Status)) >> 30) == 3)

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)
#define STATUS_NOT_IMPLEMENTED           ((NTSTATUS)0xC0000002L)
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_VIOLATION          ((NTSTATUS)0xC0000005L)
#define STATUS_INVALID_HANDLE            ((NTSTATUS)0xC0000008L)
#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)
#define STATUS_NO_SUCH_DEVICE            ((NTSTATUS)0xC000000EL)
#define STATUS_NO_SUCH_FILE              ((NTSTATUS)0xC000000FL)
#define STATUS_INVALID_DEVICE_REQUEST    ((NTSTATUS)0xC0000010L)
#define STATUS_END_OF_FILE               ((NTSTATUS)0xC0000011L)
#define STATUS_NO_MEMORY                 ((NTSTATUS)0xC0000017L)
#define STATUS_ACCESS_DENIED             ((NTSTATUS)0xC0000022L)
#define STATUS_BUFFER_TOO_SMALL          ((NTSTATUS)0xC0000023L)
#define STATUS_OBJECT_TYPE_MISMATCH      ((NTSTATUS)0xC0000024L)
#define STATUS_OBJECT_NAME_INVALID       ((NTSTATUS)0xC0000033L)
#define STATUS_OBJECT_NAME_NOT_FOUND     ((NTSTATUS)0xC0000034L)
#define STATUS_OBJECT_NAME_COLLISION     ((NTSTATUS)0xC0000035L)
#define STATUS_PROCEDURE_NOT_FOUND       ((NTSTATUS)0xC000007AL)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)
#define STATUS_NOT_SUPPORTED             ((NTSTATUS)0xC00000BBL)
#define STATUS_INVALID_PARAMETER_1       ((NTSTATUS)0xC00000EFL)
#define STATUS_INVALID_PARAMETER_2       ((NTSTATUS)0xC00000F0L)
#define STATUS_INVALID_PARAMETER_3       ((NTSTATUS)0xC00000F1L)
#define STATUS_INVALID_PARAMETER_4       ((NTSTATUS)0xC00000F2L)
#define STATUS_INVALID_ADDRESS           ((NTSTATUS)0xC0000141L)
#define STATUS_PFN_NOT_PRESENT           ((NTSTATUS)0xC00001FFUL)
#define STATUS_PTE_NOT_VALID             ((NTSTATUS)0xC00002FFUL)

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
    ((type *)((PCHAR)(address) - (ULONG_PTR)(&((type *)0)->field)))

#define FIELD_OFFSET(type, field) ((LONG)(LONG_PTR)&(((type *)0)->field))

#define RTL_FIELD_SIZE(type, field) (sizeof(((type *)0)->field))

#define RTL_SIZEOF_THROUGH_FIELD(type, field) \
    (FIELD_OFFSET(type, field) + RTL_FIELD_SIZE(type, field))

#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))

extern "C"
{
    // Memory functions
    int __cdecl memcmp(const void* dst, const void* src, size_t size);
    void* __cdecl memcpy(void* Destination, const void* Source, size_t Length);
    void* __cdecl memmove(void* Destination, const void* Source, size_t Length);
    void* __cdecl memset(void* Destination, int Value, size_t Length);

    // String functions
    size_t __cdecl strlen(const char* str);
    size_t __cdecl wcslen(const wchar_t* str);
    char* __cdecl strcpy(char* dest, const char* src);
    wchar_t* __cdecl wcscpy(wchar_t* dest, const wchar_t* src);
    char* __cdecl strcat(char* dest, const char* src);
    wchar_t* __cdecl wcscat(wchar_t* dest, const wchar_t* src);

    // Character conversion
    char __cdecl tolower(char ch);
    wchar_t __cdecl towlower(wchar_t ch);
    char __cdecl toupper(char ch);
    wchar_t __cdecl towupper(wchar_t ch);

    // String comparison
    int __cdecl strcmp(const char* str1, const char* str2);
    int __cdecl strncmp(const char* str1, const char* str2, size_t max);
    int __cdecl wcscmp(const wchar_t* str1, const wchar_t* str2);
    int __cdecl wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t max);
    int __cdecl _stricmp(const char* str1, const char* str2);
    int __cdecl _wcsicmp(const wchar_t* str1, const wchar_t* str2);

    // Control register access
    UINT64 __readcr0();
    UINT64 __readcr2();
    UINT64 __readcr3();
    UINT64 __readcr4();
    UINT64 __readcr8();
    VOID __writecr0(_In_ UINT64 value);
    VOID __writecr2(_In_ UINT64 value);
    VOID __writecr3(_In_ UINT64 value);
    VOID __writecr4(_In_ UINT64 value);
    VOID __writecr8(_In_ UINT64 value);

    // Segment register access
    UINT16 __readcs();
    UINT16 __readds();
    UINT16 __readss();
    UINT16 __readgs();
    UINT16 __readfs();
    UINT16 __reades();
    VOID __writeds(_In_ UINT16 value);
    VOID __writess(_In_ UINT16 value);
    VOID __writegs(_In_ UINT16 value);
    VOID __writefs(_In_ UINT16 value);
    VOID __writees(_In_ UINT16 value);

	VOID __clflush(PVOID address);
	VOID __clflushopt(PVOID address);

    // TLB management
    VOID __invlpg(_In_ PVOID address);

	// CPUID instruction
    VOID _cpuid(_In_ UINT32 leaf, _Out_ UINT64* eax, _Out_ UINT64* ebx,  _Out_ UINT64* ecx, _Out_ UINT64* edx);

    // MSR access
    UINT64 __readmsr(_In_ UINT32 msr);
    VOID __writemsr(_In_ UINT32 msr, _In_ UINT64 value);

    // Timestamp counter
    UINT64 __rdtscp(_Out_ UINT32* aux);

    // Performance counter
    UINT64 __rdpmc(_In_ UINT32 counter);

    // Descriptor tables
    VOID __sidt(_Out_ SEGMENT_REGISTER* idtr);
    VOID __sgdt(_Out_ SEGMENT_REGISTER* gdtr);
    VOID __lidt(_In_ SEGMENT_REGISTER* idtr);
    VOID __lgdt(_In_ SEGMENT_REGISTER* gdtr);
    UINT16 __sldt();
    VOID __lldt(_In_ UINT16 selector);
    UINT16 __str();
    VOID __ltr(_In_ UINT16 selector);

    // SVM instructions
    PHYSICAL_ADDRESS __vmrun(_Inout_ PHYSICAL_ADDRESS vmcb);
    VOID __vmsave(_In_ PHYSICAL_ADDRESS vmcb);
    VOID __vmload(_In_ PHYSICAL_ADDRESS vmcb);
    VOID __stgi();
    VOID __clgi();
    VOID __skinit(_In_ UINT32 slb_physical_address);
}

struct PACKED FUNCTION_TABLE_ENTRY
{
    UINT32 Hash;
    PVOID FunctionPtr;
};

template<size_t N>
volatile constexpr DWORD str_hash(const char(&str)[N])
{
    DWORD results = 0;
    auto count = N / 4 + ((N % 4) ? 1 : 0);
    for (size_t i = 0; i < count; i++)
        results += ((DWORD*)str)[i];
    return results;
}

constexpr DWORD str_hash(const char* str, size_t N)
{
    DWORD results = 0;
    auto count = N / 4;
    for (size_t i = 0; i < count; i++)
        results += ((DWORD*)str)[i];
    switch (N % 4)
    {
    case 3:
        results += ((DWORD*)str)[count] & 0x00FFFFFF;
        break;
    case 2:
        results += ((DWORD*)str)[count] & 0x0000FFFF;
        break;
    case 1:
        results += ((DWORD*)str)[count] & 0x000000FF;
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

template<size_t N>
constexpr size_t count_pattern_bytes(const char(&str)[N])
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

template<size_t N, size_t Size>
struct Pattern
{
    UCHAR bytes[Size]{};
    UCHAR mask[Size]{};
    size_t size = Size;

    __forceinline constexpr Pattern(const char(&str)[N])
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
    template<typename Ret, typename ...Args>
    FORCEINLINE Ret invoke(Args ...args)
    {
        return ((Ret(__fastcall*)(Args...))this)(args...);
    }

    template<typename Ret, typename ...Args>
    FORCEINLINE Ret operator()(Args ...args)
    {
        return invoke<Ret>(args...);
    }
};

struct PACKED LINEAR_ADDRESS
{
    union
    {
        UINT64 AsUINT64;
        PVOID AsPointer;

        // 4KB page (standard)
        struct
        {
            UINT64 offset : 12;
            UINT64 pte_index : 9;
            UINT64 pde_index : 9;
            UINT64 pdpte_index : 9;
            UINT64 pml4e_index : 9;
            UINT64 sign_extend : 16;
        };

        // 2MB large page
        struct
        {
            UINT64 large_offset : 21;
            UINT64 large_pde_index : 9;
            UINT64 large_pdpte_index : 9;
            UINT64 large_pml4e_index : 9;
            UINT64 large_sign_extend : 16;
        };

        // 1GB huge page
        struct
        {
            UINT64 huge_offset : 30;
            UINT64 huge_pdpte_index : 9;
            UINT64 huge_pml4e_index : 9;
            UINT64 huge_sign_extend : 16;
        };
    };

    LINEAR_ADDRESS() : AsUINT64(0) {}

    template<typename T>
    LINEAR_ADDRESS(T data) : AsUINT64((UINT64)data) {}

    template<typename T>
    FORCEINLINE T as() const { return reinterpret_cast<T>(AsUINT64); }

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
};

struct PACKED INTERUPT_STACK_TABLE
{
    struct { unsigned : 32; };
    UINT32 RSP0_low;
    UINT32 RSP0_high;
    UINT32 RSP1_low;
    UINT32 RSP1_high;
    UINT32 RSP2_low;
    UINT32 RSP2_high;
    struct { unsigned : 32; };
    struct { unsigned : 32; };
	UINT32 IST1_low;
	UINT32 IST1_high;
	UINT32 IST2_low;
	UINT32 IST2_high;
	UINT32 IST3_low;
	UINT32 IST3_high;
	UINT32 IST4_low;
	UINT32 IST4_high;
	UINT32 IST5_low;
	UINT32 IST5_high;
	UINT32 IST6_low;
	UINT32 IST6_high;
	UINT32 IST7_low;
	UINT32 IST7_high;
    struct { unsigned : 32; };
    struct { unsigned : 32; };
    struct { UINT16 : 16; };
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
        const bool system_desc = ((access & 0x10) == 0); // S bit == 0
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
        if(icr.DSH == ICR_DSH::Destination)
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
};