#pragma once

struct PACKED FUNCTION_TABLE_ENTRY
{
    UINT32 Hash;
    PVOID FunctionPtr;
};

template <size_t N>
volatile constexpr DWORD str_hash(const char (&str)[N])
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
