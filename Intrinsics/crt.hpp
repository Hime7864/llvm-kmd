#pragma once

#include "structures.hpp"

extern "C"
{
    int __cdecl memcmp(const void* dst, const void* src, size_t size);
    void* __cdecl memcpy(void* Destination, const void* Source, size_t Length);
    void* __cdecl memmove(void* Destination, const void* Source, size_t Length);
    void* __cdecl memset(void* Destination, int Value, size_t Length);
    void* __cdecl memzero(void* Destination, size_t Length);

    size_t __cdecl strlen(const char* str);
    size_t __cdecl wcslen(const wchar_t* str);
    size_t __cdecl strnlen(const char* str, size_t max_count);
    size_t __cdecl wcsnlen(const wchar_t* str, size_t max_count);
    char* __cdecl strcpy(char* dest, const char* src);
    wchar_t* __cdecl wcscpy(wchar_t* dest, const wchar_t* src);
    char* __cdecl strncpy(char* dest, const char* src, size_t count);
    wchar_t* __cdecl wcsncpy(wchar_t* dest, const wchar_t* src, size_t count);
    char* __cdecl strcat(char* dest, const char* src);
    wchar_t* __cdecl wcscat(wchar_t* dest, const wchar_t* src);

    char __cdecl tolower(char ch);
    wchar_t __cdecl towlower(wchar_t ch);
    char __cdecl toupper(char ch);
    wchar_t __cdecl towupper(wchar_t ch);

    int __cdecl strcmp(const char* str1, const char* str2);
    int __cdecl strncmp(const char* str1, const char* str2, size_t max);
    int __cdecl wcscmp(const wchar_t* str1, const wchar_t* str2);
    int __cdecl wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t max);
    int __cdecl _stricmp(const char* str1, const char* str2);
    int __cdecl _strnicmp(const char* str1, const char* str2, size_t count);
    int __cdecl _wcsicmp(const wchar_t* str1, const wchar_t* str2);
    int __cdecl _wcsnicmp(const wchar_t* str1, const wchar_t* str2, size_t count);
    char* __cdecl strchr(const char* str, int ch);
    wchar_t* __cdecl wcschr(const wchar_t* str, wchar_t ch);
    char* __cdecl strrchr(const char* str, int ch);
    char* __cdecl strstr(const char* str, const char* substr);
    wchar_t* __cdecl wcsstr(const wchar_t* str, const wchar_t* substr);
}

