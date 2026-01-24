#pragma once

typedef unsigned __int128 UINT128;
typedef unsigned long long UINT64;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef unsigned long long DWORD64;
typedef unsigned long long ULONG64;
typedef __int128 INT128;
typedef long long INT64;
typedef long long LONG64;
typedef double DOUBLE;
typedef unsigned long long QWORD;

typedef int* INT_PTR;
typedef unsigned int* UINT_PTR;
typedef long* LONG_PTR;
typedef unsigned long* ULONG_PTR;
typedef unsigned long long SIZE_T;
typedef SIZE_T* PSIZE_T;

typedef unsigned long UINT32;
typedef unsigned long DWORD;
typedef unsigned long UINTN;
typedef unsigned long ULONG;
typedef long INT32;
typedef long LONG;
typedef float FLOAT;
typedef int INT;

typedef unsigned short UINT16;
typedef unsigned short WORD;
typedef short INT16;
typedef short SHORT;
typedef unsigned short USHORT;
typedef wchar_t WCHAR;

typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef char INT8;
typedef char CHAR;

typedef bool BOOLEAN;
typedef int BOOL;

typedef void VOID;

typedef const void* PCVOID;
typedef void* PVOID;
typedef const char* PCSTR;
typedef const wchar_t* PCWSTR;
typedef unsigned char* PBYTE;
typedef unsigned char* LPBYTE;
typedef char* PCHAR;
typedef unsigned short* PUSHORT;
typedef short* PSHORT;
typedef long* PLONG;
typedef unsigned long* PULONG;
typedef unsigned int* PUINT;
typedef const char* LPCSTR;
typedef const wchar_t* LPCWSTR;
typedef char* LPSTR;
typedef wchar_t* LPWSTR;
typedef void* LPVOID;

typedef INT_PTR* PINT_PTR;
typedef UINT_PTR* PUINT_PTR;
typedef ULONG_PTR* PULONG_PTR;

typedef DWORD* PDWORD;
typedef DWORD64* PDWORD64;
typedef BOOLEAN* PBOOLEAN;

typedef long HRESULT;
typedef long NTSTATUS;
typedef unsigned long long PHYSICAL_ADDRESS;
typedef long long HANDLE;
typedef long long* PHANDLE;
typedef ULONG_PTR  KAFFINITY;

typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK* PACCESS_MASK;

#define DeclDataType(type, title) \
    type _##title; \
    typedef type _##title title; \
    typedef type _##title* P##title

DeclDataType(struct, UNICODE_STRING);
DeclDataType(struct, LARGE_INTEGER);
DeclDataType(struct, LIST_ENTRY);
DeclDataType(struct, MMPTE_HARDWARE);
DeclDataType(struct, RTL_AVL_TREE);
DeclDataType(struct, RTL_BALANCED_NODE);
DeclDataType(struct, DRIVER_OBJECT);
DeclDataType(struct, EPROCESS);
DeclDataType(struct, CONTEXT);
DeclDataType(struct, XSAVE_FORMAT);
DeclDataType(struct, M128A);
DeclDataType(struct, IRP);
DeclDataType(struct, MDL);
DeclDataType(struct, IO_STATUS_BLOCK);
DeclDataType(struct, IO_STACK_LOCATION);
DeclDataType(struct, FILE_OBJECT);
DeclDataType(struct, KEVENT);
DeclDataType(struct, DISPATCHER_HEADER);
DeclDataType(struct, KDEVICE_QUEUE_ENTRY);
DeclDataType(struct, ETHREAD);
DeclDataType(struct, KTHREAD);
DeclDataType(struct, KAPC);
DeclDataType(struct, KAPC_STATE);
DeclDataType(enum, KPROCESSOR_MODE);
DeclDataType(struct, OBJECT_ATTRIBUTES);
DeclDataType(struct, CLIENT_ID);
DeclDataType(struct, DESCRIPTOR_TABLE_REGISTER);
DeclDataType(struct, SEGMENT_SELECTOR);
DeclDataType(struct, PHYSICAL_MEMORY_RANGE);
DeclDataType(enum, POOL_TYPE);
DeclDataType(struct, MACHINE_FRAME);
DeclDataType(enum, MEMORY_CACHING_TYPE);
DeclDataType(struct, MMPFN);
DeclDataType(struct, KPRCB);

typedef void(__stdcall* PIO_APC_ROUTINE)(PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, ULONG Reserved);

typedef NTSTATUS(*_KSTART_ROUTINE)(PVOID StartContext);
typedef _KSTART_ROUTINE KSTART_ROUTINE;
typedef _KSTART_ROUTINE* PKSTART_ROUTINE;

#undef DeclDataType

#define NULL 0
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define TRUE true
#define FALSE false

#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_

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

#define GENERIC_WRITE					(0x40000000L)
#define GENERIC_READ					(0x80000000L)
#define GENERIC_EXECUTE					(0x20000000L)
#define GENERIC_ALL						(0x10000000L)

#define FILE_ATTRIBUTE_NORMAL			 0x00000080
#define FILE_SHARE_READ                  0x00000001
#define FILE_SHARE_WRITE                 0x00000002

#define FILE_OVERWRITE_IF                0x00000005
#define OBJ_CASE_INSENSITIVE             0x00000040L
#define FILE_SYNCHRONOUS_IO_NONALERT     0x00000020L

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg(args, type) __builtin_va_arg(args, type)
#define va_end __builtin_va_end

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define FORCEINLINE __attribute__((always_inline)) inline
#define NOINLINE __attribute__((noinline))
#define ALIGN(alignment) __attribute__((aligned(alignment)))

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define STATUS_SUCCESS 0x00000000UL
#define STATUS_UNSUCCESSFUL 0xC0000001UL
#define STATUS_NOT_IMPLEMENTED 0xC0000002UL
#define STATUS_INVALID_PARAMETER 0xC000000DL
#define STATUS_INSUFFICIENT_RESOURCES 0xC000009AL

#define MM_COPY_MEMORY_PHYSICAL             0x1
#define MM_COPY_MEMORY_VIRTUAL              0x2

#define MM_ANY_NODE_OK 0x80000000UL

#define PASSIVE_LEVEL 0
#define APC_LEVEL 1
#define DISPATCH_LEVEL 2
#define HIGH_LEVEL 15

#define 	OBJ_KERNEL_HANDLE   0x00000200L
#define InitializeObjectAttributes(p, n, a, r, s) { \
       (p)->Length = sizeof(_OBJECT_ATTRIBUTES); \
       (p)->RootDirectory = r; \
       (p)->Attributes = a; \
       (p)->ObjectName = n; \
       (p)->SecurityDescriptor = s; \
       (p)->SecurityQualityOfService = NULL; \
       }

struct PACKED SEGMENT_REGISTER
{
	UINT16 Limit;
	UINT64 Base;

	inline UINT32 limit(USHORT selector)
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

	inline USHORT attribute(USHORT selector)
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

	inline UINT64 base(USHORT selector)
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


extern "C"
{
	int __cdecl memcmp(void* dst, void* src, size_t size);
	void* __cdecl memcpy(void* Destination, const void* Source, size_t Length);
	void* __cdecl memset(void* Destination, int Value, size_t Length);

	size_t __cdecl strlen(const char* str);
	size_t __cdecl wcslen(const wchar_t* str);
	char __cdecl tolower(char ch);
	wchar_t __cdecl towlower(wchar_t ch);
	char __cdecl toupper(char ch);
	wchar_t __cdecl towupper(wchar_t ch);
	int __cdecl strcmp(const char* str1, const char* str2, size_t max);
	int __cdecl wcscmp(const wchar_t* str1, const wchar_t* str2, size_t max);

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
	VOID __invlpg(_In_ PVOID address);
	VOID __cpuid2(_In_ UINT32 _ecx, _Out_ UINT32* _eax, _Out_ UINT32* _ebx, _Out_ UINT32* __ecx, _Out_ UINT32* _edx);
	VOID __readmsr2(_In_ UINT32 _ecx, _Out_ UINT32* _eax, _Out_ UINT32* _edx);
	UINT64 __readmsr(_In_ UINT32 msr);
	VOID __writemsr2(_In_ UINT32 _ecx, _In_ UINT32 _eax, _In_ UINT32 _edx);
	VOID __writemsr(_In_ UINT32 msr, _In_ UINT64 value);
	VOID __rdtsc2(_Out_ UINT32* _eax, _Out_ UINT32* _edx);
	VOID __sidt(_In_ SEGMENT_REGISTER* idtr);
	VOID __sgdt(_In_ SEGMENT_REGISTER* gdtr);
	PHYSICAL_ADDRESS __vmrun(_Inout_ PHYSICAL_ADDRESS vmcb);
	VOID __vmsave(_In_ PHYSICAL_ADDRESS vmcb);
	VOID __vmload(_Inout_ PHYSICAL_ADDRESS vmcb);
	UINT64 __rdpmc(_In_ UINT32 counter);
}

class FnPtr {
public:
	template<typename Ret, typename ...Args>
	inline Ret invoke(Args ...args)
	{
		return ((Ret(__fastcall*)(Args...))this)(args...);
	}
};

struct LINEAR_ADDRESS
{
	union
	{
		UINT64 AsUINT64;
		struct {
			UINT64 offset : 12;
			UINT64 pte_index : 9;
			UINT64 pdte_index : 9;
			UINT64 pdpte_index : 9;
			UINT64 pml4e_index : 9;
			UINT64 sign_extend : 16;
		};

		struct
		{
			QWORD large_offset : 21;
		};

		struct
		{
			QWORD huge_offset : 30;
		};
	};
	LINEAR_ADDRESS() : AsUINT64(0) {}
	template<typename T>
	LINEAR_ADDRESS(T data) : AsUINT64(reinterpret_cast<UINT64>((QWORD)data)) {}
};
