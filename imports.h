#pragma once
#include "basics.hpp"
#include "cpuid.hpp"
#include "msr.hpp"
#include "Utils.hpp"

#include "nt_exports.h"
#include "nt_structs.h"
#include "nt_methods.h"

#ifdef _DEBUG
#define printf(Format, ...) DbgPrintEx(0, 0, Format, __VA_ARGS__)
#else
#define printf(Format, ...)
#endif

inline void Sleep(_In_ UINT32 milliseconds)
{
	LARGE_INTEGER interval;
	interval.QuadPart = -(10 * 1000 * (__int64)milliseconds); // Negative value for relative time
	KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

#include "Entry.hpp"
#include "Main.hpp"
