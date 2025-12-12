#pragma once

// 64-bit
typedef unsigned long long UINT64;
typedef unsigned long long QWORD;
typedef unsigned long long ULONGLONG;
typedef unsigned long long SIZE_T;
typedef long long INT64;
typedef long long LONGLONG;
typedef double DOUBLE;
// 32-bit
typedef unsigned long UINT32;
typedef unsigned long DWORD;
typedef unsigned long UINTN;
typedef long INT32;
typedef long LONG;
typedef float FLOAT;
typedef int INT;
// 16-bit
typedef unsigned short UINT16;
typedef unsigned short WORD;
typedef short INT16;
typedef short SHORT;
typedef wchar_t WCHAR;
// 8-bit
typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef char INT8;
typedef char CHAR;
// boolean
typedef bool BOOLEAN;
// void
typedef void VOID;
// pointers
typedef const void* PCVOID;
typedef void* PVOID;
typedef const char* PCSTR;
typedef const wchar_t* PCWSTR;
typedef unsigned char* PBYTE;
//other
typedef long NTSTATUS;



#define DeclDataType(type, title) \
    type _##title; \
    typedef type _##title title; \
    typedef type _##title* P##title

DeclDataType(struct, UNICODE_STRING);
DeclDataType(struct, LARGE_INTEGER);
DeclDataType(struct, LIST_ENTRY);
DeclDataType(struct, MMPTE_HARDWARE);
DeclDataType(struct, RTL_AVL_TREE);
DeclDataType(struct, DRIVER_OBJECT);

#undef DeclDataType

#define NULL 0
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
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
    void* __cdecl malloc(size_t size);
	void __cdecl free(void* ptr);
}

class FnPtr {
public:
    template<typename Ret, typename ...Args>
    inline Ret invoke(Args ...args)
    {
        return ((Ret(__fastcall*)(Args...))this)(args...);
    }
};



static inline size_t strnlen_s(const char* str, size_t strsz)
{
    size_t len = 0;
    while (len < strsz && str[len] != '\0') {
        len++;
    }
    return len;
}

static inline size_t strnlen(const char* str)
{
    return strnlen_s(str, 128);
}

static inline size_t wcsnlen_s(const wchar_t* str, size_t strsz)
{
    size_t len = 0;
    while (len < strsz && str[len] != L'\0') {
        len++;
    }
    return len;
}

static inline size_t wcsnlen(const wchar_t* str)
{
    return wcsnlen_s(str, 128);
}

static inline char tolower(char str)
{
    if (str >= 'A' && str <= 'Z')
        return str + 32;
    return str;
}

static inline wchar_t tolower(wchar_t str)
{
    if (str >= L'A' && str <= L'Z')
        return str + 32;
    return str;
}

static inline int strncmp_s(const char* str1, const char* str2, size_t max = 255)
{
    while (*str1 && *str2 && tolower(*str1) == tolower(*str2))
    {
        str1++;
        str2++;
        if (--max == 0)
            return 0;
    }
    return tolower(*str1) - tolower(*str2);
}

static inline int strncmp(const char* str1, const char* str2)
{
    return strncmp_s(str1, str2, 255);
}

static inline int wcsncmp_s(const wchar_t* str1, const wchar_t* str2, size_t max = 255)
{
    while (*str1 && *str2 && tolower(*str1) == tolower(*str2))
    {
        str1++;
        str2++;
        if (--max == 0)
            return 0;
    }
    return tolower(*str1) - tolower(*str2);
}

static inline int wcsncmp(const wchar_t* str1, const wchar_t* str2)
{
    return wcsncmp_s(str1, str2, 255);
}

static QWORD __readcr0()
{
    QWORD result = 0;
    __asm {
        mov rax, cr0
        mov[result], rax
    }
    return result;
}

static void __writecr0(QWORD value)
{
    __asm {
        mov rax, value
        mov cr0, rax
    }
}

static QWORD __readcr2()
{
    QWORD result = 0;
    __asm {
        mov rax, cr2
        mov[result], rax
    }
    return result;
}

static void __writecr2(QWORD value)
{
    __asm {
        mov rax, value
        mov cr2, rax
    }
}

static QWORD __readcr3()
{
    QWORD result = 0;
    __asm {
        mov rax, cr3
        mov[result], rax
    }
    return result;
}

static void __writecr3(QWORD value)
{
    __asm {
        mov rax, value
        mov cr3, rax
    }
}

static QWORD __readcr4()
{
    QWORD result = 0;
    __asm {
        mov rax, cr4
        mov[result], rax
    }
    return result;
}

static void __writecr4(QWORD value)
{
    __asm {
        mov rax, value
        mov cr4, rax
    }
}

static QWORD __readcr8()
{
    QWORD result = 0;
    __asm {
        mov rax, cr8
        mov[result], rax
    }
    return result;
}

static void __writecr8(QWORD value)
{
    __asm {
        mov rax, value
        mov cr8, rax
    }
}

static inline void __invlpg(void* address)
{
    __asm {
        mov rax, address
        invlpg[rax]
    }
}

static inline QWORD __readmsr(DWORD msr)
{
    DWORD low, high;
    __asm {
        mov ecx, msr
        rdmsr
        mov low, eax
        mov high, edx
    }
    QWORD result = (QWORD)high << 32 | low;
    return result;
}

static inline void __writemsr(DWORD msr, QWORD value)
{
    DWORD low = (DWORD)(value & 0xFFFFFFFF);
    DWORD high = (DWORD)(value >> 32);
    __asm {
        mov ecx, msr
        mov eax, low
        mov edx, high
        wrmsr
    }
    return;
}

//__sidt
//__sgdt

static inline void __sidt(void* idtr)
{
    __asm {
        mov rax, idtr
        sidt[rax]
    }
}

static inline void __sgdt(void* gdtr)
{
    __asm {
        mov rax, gdtr
        sgdt[rax]
    }
}

//__vmrun
static void __vmrun(QWORD vmcb_pa)
{
    __asm {
        mov rax, vmcb_pa
        vmrun rax
    }
    return;
}

//__vmsave
static inline void __vmsave(QWORD vmcb_pa)
{
    __asm {
        mov rax, vmcb_pa
        vmsave rax
    }
    return;
}

//__vmload

static inline void __vmload(QWORD vmcb_pa)
{
    __asm {
        mov rax, vmcb_pa
        vmload rax
    }
    return;
}

//__vmlaunch

static void __vmlaunch(QWORD vmcb_pa)
{
    __vmload(vmcb_pa);
    __vmrun(vmcb_pa);
    __vmsave(vmcb_pa);
}