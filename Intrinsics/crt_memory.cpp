#include "crt.hpp"

extern "C"
{
    int __cdecl memcmp(const void* buf1, const void* buf2, size_t size)
    {
        if (!buf1 || !buf2)
            return 0;

        const BYTE* ptr1 = static_cast<const BYTE*>(buf1);
        const BYTE* ptr2 = static_cast<const BYTE*>(buf2);

        for (size_t i = 0; i < size; ++i)
        {
            if (ptr1[i] != ptr2[i])
            {
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
        while (Length >= 16)
        {
            *reinterpret_cast<UINT128*>(dest) = *reinterpret_cast<const UINT128*>(src);
            dest += 16;
            src += 16;
            Length -= 16;
        }

        // Copy remaining chunks of 8 bytes (64-bit)
        if (Length >= 8)
        {
            *reinterpret_cast<UINT64*>(dest) = *reinterpret_cast<const UINT64*>(src);
            dest += 8;
            src += 8;
            Length -= 8;
        }

        // Copy remaining chunks of 4 bytes (32-bit)
        if (Length >= 4)
        {
            *reinterpret_cast<UINT32*>(dest) = *reinterpret_cast<const UINT32*>(src);
            dest += 4;
            src += 4;
            Length -= 4;
        }

        // Copy remaining chunks of 2 bytes (16-bit)
        if (Length >= 2)
        {
            *reinterpret_cast<UINT16*>(dest) = *reinterpret_cast<const UINT16*>(src);
            dest += 2;
            src += 2;
            Length -= 2;
        }

        // Copy the last byte (8-bit)
        if (Length == 1)
        {
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
        if (dest < src || dest >= (src + Length))
        {
            // No overlap or destination is before source - copy forward
            return memcpy(Destination, Source, Length);
        }
        else
        {
            // Overlap detected - copy backward
            dest += Length;
            src += Length;

            while (Length >= 8)
            {
                dest -= 8;
                src -= 8;
                *reinterpret_cast<UINT64*>(dest) = *reinterpret_cast<const UINT64*>(src);
                Length -= 8;
            }

            while (Length > 0)
            {
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

        for (;;)
        {
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
            if (q >= end)
                break;
            *q++ = val;
        }
        return Destination;
    }

    void* __cdecl memzero(void* Destination, size_t Length)
    {
        return memset(Destination, 0, Length);
    }
}