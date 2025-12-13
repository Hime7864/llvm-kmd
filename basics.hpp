#pragma once

typedef unsigned long long UINT64;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef unsigned long long DWORD64;
typedef unsigned long long ULONG64;
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
typedef long HANDLE;
typedef long* PHANDLE;



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

typedef NTSTATUS(*_KSTART_ROUTINE)(PVOID StartContext);
typedef _KSTART_ROUTINE KSTART_ROUTINE;
typedef _KSTART_ROUTINE* PKSTART_ROUTINE;

#undef DeclDataType

#define NULL 0
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg(args, type) __builtin_va_arg(args, type)
#define va_end __builtin_va_end

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define FORCEINLINE __attribute__((always_inline)) inline
#define ALIGN(alignment) __attribute__((aligned(alignment)))

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define STATUS_SUCCESS 0x00000000UL
#define STATUS_UNSUCCESSFUL 0xC0000001UL
#define STATUS_NOT_IMPLEMENTED 0xC0000002UL
#define STATUS_INVALID_PARAMETER 0xC000000DL

#define PASSIVE_LEVEL 0
#define APC_LEVEL 1
#define DISPATCH_LEVEL 2


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
	UINT64 __readmsr(_In_ UINT32 msr);
	VOID __writemsr(_In_ UINT32 msr, _In_ UINT64 value);
	VOID __sidt(_In_ PVOID idtr);
	VOID __sgdt(_In_ PVOID gdtr);
	VOID __vmrun(_Inout_ PHYSICAL_ADDRESS hsave);
	VOID __vmsave(_In_ PHYSICAL_ADDRESS vmcb);
	VOID __vmload(_Inout_ PHYSICAL_ADDRESS hsave);
}

class FnPtr {
public:
    template<typename Ret, typename ...Args>
    inline Ret invoke(Args ...args)
    {
        return ((Ret(__fastcall*)(Args...))this)(args...);
    }
};