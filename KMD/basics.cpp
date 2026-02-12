#include "basics.hpp"

extern "C"
{
    int __cdecl memcmp(const void* buf1, const void* buf2, size_t size)
    {
        if (!buf1 || !buf2)
            return 0;

        const BYTE* ptr1 = static_cast<const BYTE*>(buf1);
        const BYTE* ptr2 = static_cast<const BYTE*>(buf2);

        for (size_t i = 0; i < size; ++i) {
            if (ptr1[i] != ptr2[i]) {
                return ptr1[i] - ptr2[i];
            }
        }
        return 0;
    }

    void* __cdecl memcpy(void* Destination, const void* Source, size_t Length)
    {
        if (!Destination || !Source || Length == 0)
            return Destination;

        auto* dest = static_cast<UINT8*>(Destination);
        auto* src = static_cast<const UINT8*>(Source);

        // Copy in chunks of 16 bytes (128-bit)
        while (Length >= 16) {
            *reinterpret_cast<UINT128*>(dest) = *reinterpret_cast<const UINT128*>(src);
            dest += 16;
            src += 16;
            Length -= 16;
        }

        // Copy remaining chunks of 8 bytes (64-bit)
        if (Length >= 8) {
            *reinterpret_cast<UINT64*>(dest) = *reinterpret_cast<const UINT64*>(src);
            dest += 8;
            src += 8;
            Length -= 8;
        }

        // Copy remaining chunks of 4 bytes (32-bit)
        if (Length >= 4) {
            *reinterpret_cast<UINT32*>(dest) = *reinterpret_cast<const UINT32*>(src);
            dest += 4;
            src += 4;
            Length -= 4;
        }

        // Copy remaining chunks of 2 bytes (16-bit)
        if (Length >= 2) {
            *reinterpret_cast<UINT16*>(dest) = *reinterpret_cast<const UINT16*>(src);
            dest += 2;
            src += 2;
            Length -= 2;
        }

        // Copy the last byte (8-bit)
        if (Length == 1) {
            *dest = *src;
        }

        return Destination;
    }

    void* __cdecl memmove(void* Destination, const void* Source, size_t Length)
    {
        if (!Destination || !Source || Length == 0)
            return Destination;

        auto* dest = static_cast<UINT8*>(Destination);
        auto* src = static_cast<const UINT8*>(Source);

        // Check for overlap and copy in appropriate direction
        if (dest < src || dest >= (src + Length)) {
            // No overlap or destination is before source - copy forward
            return memcpy(Destination, Source, Length);
        }
        else {
            // Overlap detected - copy backward
            dest += Length;
            src += Length;

            while (Length >= 8) {
                dest -= 8;
                src -= 8;
                *reinterpret_cast<UINT64*>(dest) = *reinterpret_cast<const UINT64*>(src);
                Length -= 8;
            }

            while (Length > 0) {
                dest--;
                src--;
                *dest = *src;
                Length--;
            }
        }

        return Destination;
    }

    void* __cdecl memset(void* Destination, int Value, size_t Length)
    {
        if (!Destination || Length == 0)
            return Destination;

        char* q = (char*)Destination;
        char* end = q + Length;
        char val = (char)Value;

        for (;;) {
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
            if (q >= end) break; *q++ = val;
        }
        return Destination;
    }

    void* __cdecl memzero(void* Destination, size_t Length)
    {
        return memset(Destination, 0, Length);
    }

    size_t __cdecl strlen(const char* str)
    {
        if (!str)
            return 0;

        const char* s = str;
        while (*s)
            ++s;
        return s - str;
    }

    size_t __cdecl wcslen(const wchar_t* str)
    {
        if (!str)
            return 0;

        const wchar_t* s = str;
        while (*s)
            ++s;
        return s - str;
    }

    size_t __cdecl strnlen(const char* str, size_t max_count)
    {
        if (!str)
            return 0;

        const char* s = str;
        size_t count = 0;
        while (*s && count < max_count) {
            ++s;
            ++count;
        }
        return count;
    }

    size_t __cdecl wcsnlen(const wchar_t* str, size_t max_count)
    {
        if (!str)
            return 0;

        const wchar_t* s = str;
        size_t count = 0;
        while (*s && count < max_count) {
            ++s;
            ++count;
        }
        return count;
    }

    char* __cdecl strcpy(char* dest, const char* src)
    {
        if (!dest || !src)
            return dest;

        char* original_dest = dest;
        while ((*dest++ = *src++) != '\0');
        return original_dest;
    }

    wchar_t* __cdecl wcscpy(wchar_t* dest, const wchar_t* src)
    {
        if (!dest || !src)
            return dest;

        wchar_t* original_dest = dest;
        while ((*dest++ = *src++) != L'\0');
        return original_dest;
    }

    char* __cdecl strncpy(char* dest, const char* src, size_t count)
    {
        if (!dest || !src)
            return dest;

        char* original_dest = dest;
        while (count && (*dest++ = *src++) != '\0')
            --count;

        while (count--)
            *dest++ = '\0';

        return original_dest;
    }

    wchar_t* __cdecl wcsncpy(wchar_t* dest, const wchar_t* src, size_t count)
    {
        if (!dest || !src)
            return dest;

        wchar_t* original_dest = dest;
        while (count && (*dest++ = *src++) != L'\0')
            --count;

        while (count--)
            *dest++ = L'\0';

        return original_dest;
    }

    char* __cdecl strcat(char* dest, const char* src)
    {
        if (!dest || !src)
            return dest;

        char* original_dest = dest;
        while (*dest)
            ++dest;
        while ((*dest++ = *src++) != '\0');
        return original_dest;
    }

    wchar_t* __cdecl wcscat(wchar_t* dest, const wchar_t* src)
    {
        if (!dest || !src)
            return dest;

        wchar_t* original_dest = dest;
        while (*dest)
            ++dest;
        while ((*dest++ = *src++) != L'\0');
        return original_dest;
    }

    char __cdecl tolower(char ch)
    {
        if (ch >= 'A' && ch <= 'Z')
            return ch + ('a' - 'A');
        return ch;
    }

    wchar_t __cdecl towlower(wchar_t ch)
    {
        if (ch >= L'A' && ch <= L'Z')
            return ch + (L'a' - L'A');
        return ch;
    }

    char __cdecl toupper(char ch)
    {
        if (ch >= 'a' && ch <= 'z')
            return ch - ('a' - 'A');
        return ch;
    }

    wchar_t __cdecl towupper(wchar_t ch)
    {
        if (ch >= L'a' && ch <= L'z')
            return ch - (L'a' - L'A');
        return ch;
    }

    int __cdecl strcmp(const char* str1, const char* str2)
    {
        if (!str1 || !str2)
            return 0;

        while (*str1 && (*str1 == *str2)) {
            ++str1;
            ++str2;
        }
        return (unsigned char)(*str1) - (unsigned char)(*str2);
    }

    int __cdecl strncmp(const char* str1, const char* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (*str1 == *str2)) {
            ++str1;
            ++str2;
            --count;
        }

        if (count == 0)
            return 0;

        return (unsigned char)(*str1) - (unsigned char)(*str2);
    }

    int __cdecl wcscmp(const wchar_t* str1, const wchar_t* str2)
    {
        if (!str1 || !str2)
            return 0;

        while (*str1 && (*str1 == *str2)) {
            ++str1;
            ++str2;
        }
        return *str1 - *str2;
    }

    int __cdecl wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (*str1 == *str2)) {
            ++str1;
            ++str2;
            --count;
        }

        if (count == 0)
            return 0;

        return *str1 - *str2;
    }

    int __cdecl _stricmp(const char* str1, const char* str2)
    {
        if (!str1 || !str2)
            return 0;

        while (*str1 && (tolower(*str1) == tolower(*str2))) {
            ++str1;
            ++str2;
        }
        return tolower(*str1) - tolower(*str2);
    }

    int __cdecl _strnicmp(const char* str1, const char* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (tolower(*str1) == tolower(*str2))) {
            ++str1;
            ++str2;
            --count;
        }

        if (count == 0)
            return 0;

        return tolower(*str1) - tolower(*str2);
    }

    int __cdecl _wcsicmp(const wchar_t* str1, const wchar_t* str2)
    {
        if (!str1 || !str2)
            return 0;

        while (*str1 && (towlower(*str1) == towlower(*str2))) {
            ++str1;
            ++str2;
        }
        return towlower(*str1) - towlower(*str2);
    }

    int __cdecl _wcsnicmp(const wchar_t* str1, const wchar_t* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (towlower(*str1) == towlower(*str2))) {
            ++str1;
            ++str2;
            --count;
        }

        if (count == 0)
            return 0;

        return towlower(*str1) - towlower(*str2);
    }

    char* __cdecl strchr(const char* str, int ch)
    {
        if (!str)
            return NULL;

        while (*str) {
            if (*str == (char)ch)
                return (char*)str;
            ++str;
        }
        return NULL;
    }

    wchar_t* __cdecl wcschr(const wchar_t* str, wchar_t ch)
    {
        if (!str)
            return NULL;

        while (*str) {
            if (*str == ch)
                return (wchar_t*)str;
            ++str;
        }
        return NULL;
    }

    char* __cdecl strrchr(const char* str, int ch)
    {
        if (!str)
            return NULL;

        const char* last = NULL;
        while (*str) {
            if (*str == (char)ch)
                last = str;
            ++str;
        }
        return (char*)last;
    }

    char* __cdecl strstr(const char* str, const char* substr)
    {
        if (!str || !substr)
            return NULL;

        if (*substr == '\0')
            return (char*)str;

        for (const char* s = str; *s; ++s) {
            const char* s1 = s;
            const char* s2 = substr;

            while (*s1 && *s2 && (*s1 == *s2)) {
                ++s1;
                ++s2;
            }

            if (*s2 == '\0')
                return (char*)s;
        }

        return NULL;
    }

    wchar_t* __cdecl wcsstr(const wchar_t* str, const wchar_t* substr)
    {
        if (!str || !substr)
            return NULL;

        if (*substr == L'\0')
            return (wchar_t*)str;

        for (const wchar_t* s = str; *s; ++s) {
            const wchar_t* s1 = s;
            const wchar_t* s2 = substr;

            while (*s1 && *s2 && (*s1 == *s2)) {
                ++s1;
                ++s2;
            }

            if (*s2 == L'\0')
                return (wchar_t*)s;
        }

        return NULL;
    }

    UINT64 NAKED __readcr0()
    {
        __asm {
            mov rax, cr0
            ret
        }
    }

    UINT64 NAKED __readcr2()
    {
        __asm {
            mov rax, cr2
            ret
        }
    }

    UINT64 NAKED __readcr3()
    {
        __asm {
            mov rax, cr3
            ret
        }
    }

    UINT64 NAKED __readcr4()
    {
        __asm {
            mov rax, cr4
            ret
        }
    }

    UINT64 NAKED __readcr8()
    {
        __asm {
            mov rax, cr8
            ret
        }
    }

    VOID NAKED __writecr0(_In_ UINT64 value)
    {
        __asm {
            mov cr0, rcx
            ret
        }
    }

    VOID NAKED __writecr2(_In_ UINT64 value)
    {
        __asm {
            mov cr2, rcx
            ret
        }
    }

    VOID NAKED __writecr3(_In_ UINT64 value)
    {
        __asm {
            mov cr3, rcx
            ret
        }
    }

    VOID NAKED __writecr4(_In_ UINT64 value)
    {
        __asm {
            mov cr4, rcx
            ret
        }
    }

    VOID NAKED __writecr8(_In_ UINT64 value)
    {
        __asm {
            mov cr8, rcx
            ret
        }
    }

    VOID NAKED __clflush(PVOID address)
    {
        __asm {
			clflush[rcx]
            ret
        }
    }

    VOID NAKED __clflushopt(PVOID address)
    {
        __asm {
            clflushopt[rcx]
			ret
		}
    }

    VOID __invlpg(_In_ PVOID address)
    {
        __asm {
            mov rax, address
            invlpg[rax]
        }
    }

    VOID _cpuid(
        _In_ UINT32 leaf,
        _Out_ UINT64* rax,
        _Out_ UINT64* rbx,
        _Out_ UINT64* rcx,
        _Out_ UINT64* rdx
    )
    {
        UINT64 _rax, _rbx, _rcx, _rdx;
        __asm {
            mov eax, leaf
            cpuid
            mov [_rax], rax
            mov [_rbx], rbx
            mov [_rcx], rcx
            mov [_rdx], rdx
        }
        *rax = _rax;
        *rbx = _rbx;
        *rcx = _rcx;
        *rdx = _rdx;
    }

    UINT64 __readmsr(_In_ UINT32 msr)
    {
        UINT32 low, high;
        __asm {
            mov ecx, msr
            rdmsr
            mov low, eax
            mov high, edx
        }
        return ((UINT64)high << 32) | low;
    }

    VOID __writemsr(_In_ UINT32 msr, _In_ UINT64 value)
    {
        UINT32 low = (UINT32)(value & 0xFFFFFFFF);
        UINT32 high = (UINT32)(value >> 32);
        __asm {
            mov ecx, msr
            mov eax, low
            mov edx, high
            wrmsr
        }
    }

    UINT64 __rdtscp(_Out_ UINT32* aux)
    {
        UINT32 low, high, ecx;
        __asm {
            rdtscp
            mov low, eax
            mov high, edx
            mov ecx, ecx
        }
        if (aux) *aux = ecx;
        return ((UINT64)high << 32) | low;
    }

    UINT64 __rdpmc(_In_ UINT32 counter)
    {
        UINT32 low, high;
        __asm {
            mov ecx, counter
            rdpmc
            mov low, eax
            mov high, edx
        }
        return ((UINT64)high << 32) | low;
    }

    VOID NAKED __sidt(_Out_ SEGMENT_REGISTER* idtr)
    {
        __asm {
            mov rax, rcx
            sidt[rax]
            ret
        }
    }

    VOID NAKED __sgdt(_Out_ SEGMENT_REGISTER* gdtr)
    {
        __asm {
            mov rax, rcx
            sgdt[rax]
            ret
        }
    }

    UINT16 __sldt()
    {
        UINT16 ldtr;
        __asm {
            sldt ax
            mov ldtr, ax
        }
        return ldtr;
    }

    VOID __lldt(_In_ UINT16 selector)
    {
        __asm {
            mov ax, selector
            lldt ax
        }
    }

    UINT16 __str()
    {
        UINT16 tr;
        __asm {
            str ax
            mov tr, ax
        }
        return tr;
    }

    VOID __ltr(_In_ UINT16 selector)
    {
        __asm {
            mov ax, selector
            ltr ax
        }
    }

    UINT16 __readcs()
    {
        UINT16 cs;
        __asm {
            mov ax, cs
            mov cs, ax
        }
        return cs;
    }

    UINT16 __readds()
    {
        UINT16 ds;
        __asm {
            mov ax, ds
            mov ds, ax
        }
        return ds;
    }

    UINT16 __readss()
    {
        UINT16 ss;
        __asm {
            mov ax, ss
            mov ss, ax
        }
        return ss;
    }

    UINT16 __reades()
    {
        UINT16 es;
        __asm {
            mov ax, es
            mov es, ax
        }
        return es;
    }

    UINT16 __readfs()
    {
        UINT16 fs;
        __asm {
            mov ax, fs
            mov fs, ax
        }
        return fs;
    }

    UINT16 __readgs()
    {
        UINT16 gs;
        __asm {
            mov ax, gs
            mov gs, ax
        }
        return gs;
    }

    VOID __writeds(_In_ UINT16 value)
    {
        __asm {
            mov ax, value
            mov ds, ax
        }
    }

    VOID __writess(_In_ UINT16 value)
    {
        __asm {
            mov ax, value
            mov ss, ax
        }
    }

    VOID __writees(_In_ UINT16 value)
    {
        __asm {
            mov ax, value
            mov es, ax
        }
    }

    VOID __writefs(_In_ UINT16 value)
    {
        __asm {
            mov ax, value
            mov fs, ax
        }
    }

    VOID __writegs(_In_ UINT16 value)
    {
        __asm {
            mov ax, value
            mov gs, ax
        }
    }

    PHYSICAL_ADDRESS NAKED __vmrun(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmrun rax
            ret
        }
    }

    VOID NAKED __vmsave(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmsave rax
            ret
        }
    }

    VOID NAKED __vmload(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmload rax
            ret
        }
    }

    VOID NAKED __stgi()
    {
        __asm {
            stgi
            ret
        }
    }

    VOID NAKED __clgi()
    {
        __asm {
            clgi
            ret
        }
    }

    VOID NAKED __skinit(_In_ UINT32 slb_physical_address)
    {
        __asm {
            mov eax, ecx
            skinit
            ret
        }
    }
}