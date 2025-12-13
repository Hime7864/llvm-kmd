#include "basics.hpp"

extern "C" {
    int __cdecl memcmp(void* dst, void* src, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            if (((BYTE*)dst)[i] != ((BYTE*)src)[i]) {
                return ((BYTE*)dst)[i] - ((BYTE*)src)[i];
            }
        }
        return 0;
    }

    void* __cdecl memcpy(void* Destination, const void* Source, size_t Length)
    {
        for (size_t i = 0; i < Length; ++i)
            ((BYTE*)Destination)[i] = ((BYTE*)Source)[i];
    }

    void* __cdecl memset(void* Destination, int Value, size_t Length)
    {
        char* q = (char*)Destination;
        char* end = q + Length;
        for (;;) {
            if (q >= end) break; *q++ = (char)Value;
            if (q >= end) break; *q++ = (char)Value;
            if (q >= end) break; *q++ = (char)Value;
            if (q >= end) break; *q++ = (char)Value;
        }
        return Destination;
    }

    size_t __cdecl strlen(const char* str)
    {
        size_t len = 0;
        while (str[len] != '\0') {
            len++;
        }
		return len;
    }

    size_t __cdecl wcslen(const wchar_t* str)
    {
        size_t len = 0;
        while (str[len] != L'\0') {
            len++;
        }
		return len;
    }

    char __cdecl tolower(char ch)
    {
        if (ch >= 'A' && ch <= 'Z')
			return ch + 32;
		return ch;
    }

    wchar_t __cdecl towlower(wchar_t ch)
    {
        if (ch >= L'A' && ch <= L'Z')
            return ch + 32;
        return ch;
    }

    char __cdecl toupper(char ch)
    {
        if (ch >= 'a' && ch <= 'z')
            return ch - 32;
        return ch;
    }

    wchar_t __cdecl towupper(wchar_t ch)
    {
        if (ch >= L'a' && ch <= L'z')
            return ch - 32;
        return ch;
    }

    int __cdecl strcmp(const char* str1, const char* str2, size_t max = 255)
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

    int __cdecl wcscmp(const wchar_t* str1, const wchar_t* str2, size_t max = 255)
    {
        while (*str1 && *str2 && towlower(*str1) == towlower(*str2))
        {
            str1++;
            str2++;
            if (--max == 0)
                return 0;
        }
		return towlower(*str1) - towlower(*str2);
    }

    UINT64 __readcr0()
    {
        UINT64 result = 0;
        __asm {
            mov rax, cr0
            mov [result], rax
        }
        return result;
    }

    UINT64 __readcr2()
    {
        UINT64 result = 0;
        __asm {
            mov rax, cr2
            mov[result], rax
        }
        return result;
    }

    UINT64 __readcr3()
    {
        UINT64 result = 0;
        __asm {
            mov rax, cr3
            mov[result], rax
        }
        return result;
    }

    UINT64 __readcr4()
    {
        UINT64 result = 0;
        __asm {
            mov rax, cr4
            mov[result], rax
        }
        return result;
    }

    UINT64 __readcr8()
    {
        UINT64 result = 0;
        __asm {
            mov rax, cr8
            mov[result], rax
        }
        return result;
    }

    VOID __writecr0(_In_ UINT64 value)
    {
        __asm {
            mov rax, value
            mov cr0, rax
        }return;
    }

    VOID __writecr2(_In_ UINT64 value)
    {
        __asm {
            mov rax, value
            mov cr2, rax
		}return;
    }

    VOID __writecr3(_In_ UINT64 value)
    {
        __asm {
            mov rax, value
            mov cr3, rax
		}return;
    }

    VOID __writecr4(_In_ UINT64 value)
    {
        __asm {
            mov rax, value
            mov cr4, rax
        }return;
    }

    VOID __writecr8(_In_ UINT64 value)
    {
        __asm {
            mov rax, value
            mov cr8, rax
		}return;
    }

    VOID __invlpg(_In_ PVOID address)
    {
        __asm {
            mov rax, address
            invlpg[rax]
		}return;
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
        UINT64 result = (UINT64)high << 32 | low;
		return result;
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
        }return;
    }

    VOID __sidt(_In_ PVOID idtr)
    {
        __asm {
            mov rax, idtr
            sidt[rax]
		}return;
    }

    VOID __sgdt(_In_ PVOID gdtr)
    {
        __asm {
            mov rax, gdtr
            sgdt[rax]
        }return;
    }

    VOID __vmrun(_Inout_ PHYSICAL_ADDRESS hsave)
    {
        __asm {
            mov rax, hsave
            vmrun rax
        }return;
    }

    VOID __vmsave(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, vmcb
            vmsave rax
		}return;
	}

    VOID __vmload(_Inout_ PHYSICAL_ADDRESS hsave)
    {
        __asm {
            mov rax, hsave
            vmload rax
        }return;
    }
}