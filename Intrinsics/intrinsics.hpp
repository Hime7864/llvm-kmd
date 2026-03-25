#pragma once

#include "basics.hpp"
#include "cpuid.hpp"
#include "msr.hpp"

#include "nt/nt.hpp"

#include "utils.hpp"

#include "fwAlloc.hpp"
#include "dTlbPoison.hpp"
#include "entry.hpp"

#ifdef _DEBUG
#define printf(Format, ...) NtImports::fn_DbgPrintEx(0, 0, Format, __VA_ARGS__)
#else
#define printf(Format, ...)
#endif

inline DTLB_POISON dTlbPoison;

inline void Sleep(_In_ UINT32 milliseconds)
{
    LARGE_INTEGER interval;
    interval.QuadPart = -(10 * 1000 * (__int64)milliseconds); // Negative value for relative time
    KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

NTSTATUS DriverEntry();
