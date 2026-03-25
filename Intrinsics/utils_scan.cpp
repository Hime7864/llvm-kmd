#include "intrinsics.hpp"

QWORD Utils::ResolveRel32(BYTE count, QWORD address)
{
    if (address)
        return address + *(int*)(address + count) + count + 4;
    return 0;
}

#define TO_BYTE(c) ((c) >= '0' && (c) <= '9' ? (c) - '0' : ((c) >= 'A' && (c) <= 'F' ? (c) - 'A' + 10 : ((c) >= 'a' && (c) <= 'f' ? (c) - 'a' + 10 : 0)))

QWORD Utils::SigScan(QWORD scan_start, QWORD max_scan, const char* ida_sig)
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
            return (QWORD)current;
    }

    return 0;
}

QWORD Utils::SigScan_s(QWORD scan_start, QWORD max_scan, const char* ida_sig)
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
            auto status = MmCopyMemory(&_data, (QWORD)data, sizeof(BYTE), MM_COPY_MEMORY_VIRTUAL, &bytesRead);

            if (!NT_SUCCESS(status) || _data != byte)
            {
                matched = 0;
                break;
            }
            data++;
            pattern += 2;
        }
        if (matched)
            return (QWORD)current;
    }

    return 0;
}

#undef TO_BYTE

QWORD Utils::GetKernelBase()
{
    auto sig = Utils::SigScan(MSR::LSTAR() & ~0xFFFFFULL, 0xA00000ULL, pattern("66 89 05 ? ? ? ? 48 8D 05 ? ? ? ? 48 89"));
    if (!sig)
        return 0;
    return *(QWORD*)(*(QWORD*)ResolveRel32(3, sig + 0x07) + 0x30);
}
