#include "utils.hpp"

UINT64 Utils::ResolveRel32(BYTE count, UINT64 address)
{
    if (address)
        return address + *(int*)(address + count) + count + 4;
    return 0;
}

#define TO_BYTE(c) ((c) >= '0' && (c) <= '9' ? (c) - '0' : ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : ((c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : 0)))

UINT64 Utils::SigScan(UINT64 scan_start, UINT64 max_scan, const char* ida_sig)
{
    BYTE* start = (BYTE*)scan_start;
    BYTE* end = start + max_scan;
    if (start > end)
        return 0;

    for (BYTE* current = start; current < end; ++current)
    {
        const BYTE* data = current;
        const char* pattern = ida_sig;
        int matched = 1;
        while (*pattern)
        {
            if (*pattern == ' ')
            {
                ++pattern;
                continue;
            }
            if (*pattern == '?')
            {
                ++data;
                ++pattern;
                if (*pattern == '?')
                    ++pattern;
                continue;
            }
            BYTE byte = (TO_BYTE(*pattern) << 4) | TO_BYTE(*(pattern + 1));
            if (*data != byte)
            {
                matched = 0;
                break;
            }
            data++;
            pattern += 2;
        }
        if (matched)
            return (UINT64)current;
    }

    return 0;
}

UINT64 Utils::SigScan_s(UINT64 scan_start, UINT64 max_scan, const char* ida_sig)
{
    BYTE* start = (BYTE*)scan_start;
    BYTE* end = start + max_scan;
    if (start > end)
        return 0;

    for (BYTE* current = start; current < end; ++current)
    {
        const BYTE* data = current;
        const char* pattern = ida_sig;
        int matched = 1;
        while (*pattern)
        {
            if (*pattern == ' ')
            {
                ++pattern;
                continue;
            }
            if (*pattern == '?')
            {
                ++data;
                ++pattern;
                if (*pattern == '?')
                    ++pattern;
                continue;
            }
            BYTE byte = (TO_BYTE(*pattern) << 4) | TO_BYTE(*(pattern + 1));

            BYTE _data = 0;
            SIZE_T bytesRead;
            auto status = MmCopyMemory(&_data, (UINT64)data, sizeof(BYTE), MM_COPY_MEMORY_VIRTUAL, &bytesRead);

            if (!NT_SUCCESS(status) || _data != byte)
            {
                matched = 0;
                break;
            }
            data++;
            pattern += 2;
        }
        if (matched)
            return (UINT64)current;
    }

    return 0;
}

#undef TO_BYTE
