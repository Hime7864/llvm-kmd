#include "crt.hpp"

extern "C"
{
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
        while (*s && count < max_count)
        {
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
        while (*s && count < max_count)
        {
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
        while ((*dest++ = *src++) != '\0')
            ;
        return original_dest;
    }

    wchar_t* __cdecl wcscpy(wchar_t* dest, const wchar_t* src)
    {
        if (!dest || !src)
            return dest;

        wchar_t* original_dest = dest;
        while ((*dest++ = *src++) != L'\0')
            ;
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
        while ((*dest++ = *src++) != '\0')
            ;
        return original_dest;
    }

    wchar_t* __cdecl wcscat(wchar_t* dest, const wchar_t* src)
    {
        if (!dest || !src)
            return dest;

        wchar_t* original_dest = dest;
        while (*dest)
            ++dest;
        while ((*dest++ = *src++) != L'\0')
            ;
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

        while (*str1 && (*str1 == *str2))
        {
            ++str1;
            ++str2;
        }
        return (unsigned char)(*str1) - (unsigned char)(*str2);
    }

    int __cdecl strncmp(const char* str1, const char* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (*str1 == *str2))
        {
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

        while (*str1 && (*str1 == *str2))
        {
            ++str1;
            ++str2;
        }
        return *str1 - *str2;
    }

    int __cdecl wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (*str1 == *str2))
        {
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

        while (*str1 && (tolower(*str1) == tolower(*str2)))
        {
            ++str1;
            ++str2;
        }
        return tolower(*str1) - tolower(*str2);
    }

    int __cdecl _strnicmp(const char* str1, const char* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (tolower(*str1) == tolower(*str2)))
        {
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

        while (*str1 && (towlower(*str1) == towlower(*str2)))
        {
            ++str1;
            ++str2;
        }
        return towlower(*str1) - towlower(*str2);
    }

    int __cdecl _wcsnicmp(const wchar_t* str1, const wchar_t* str2, size_t count)
    {
        if (!str1 || !str2 || count == 0)
            return 0;

        while (count && *str1 && (towlower(*str1) == towlower(*str2)))
        {
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

        while (*str)
        {
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

        while (*str)
        {
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
        while (*str)
        {
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

        for (const char* s = str; *s; ++s)
        {
            const char* s1 = s;
            const char* s2 = substr;

            while (*s1 && *s2 && (*s1 == *s2))
            {
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

        for (const wchar_t* s = str; *s; ++s)
        {
            const wchar_t* s1 = s;
            const wchar_t* s2 = substr;

            while (*s1 && *s2 && (*s1 == *s2))
            {
                ++s1;
                ++s2;
            }

            if (*s2 == L'\0')
                return (wchar_t*)s;
        }

        return NULL;
    }
}