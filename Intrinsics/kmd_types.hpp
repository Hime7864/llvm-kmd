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
typedef UINT8 KIRQL;
typedef KIRQL* PKIRQL;
typedef ULONG_PTR KSPIN_LOCK;
typedef KSPIN_LOCK* PKSPIN_LOCK;
typedef LONG KPRIORITY;
typedef UINT64 POOL_FLAGS;

// Access control
typedef DWORD ACCESS_MASK;
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
