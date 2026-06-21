#pragma once
#include "structures.hpp"

// --- crt.hpp ---
#include "crt.hpp"

// --- assembly.hpp ---
#include "assembly.hpp"

// --- cpuid.hpp ---
#include "cpuid.hpp"

// --- msr.hpp ---
#include "msr.hpp"
// --- imports.hpp ---
#include "imports.hpp"

// --- utils.hpp ---
#include "utils.hpp"

// --- fwa.hpp ---
#include "fwa.hpp"

// --- dtlb.hpp ---
#include "dtlb.hpp"

// --- entry.hpp ---
NTSTATUS resolve_imports();

NTSTATUS resolve_sigged_imports();

NTSTATUS volatile start();

NTSTATUS DriverEntry();

inline void Sleep(_In_ UINT32 milliseconds)
{
    LARGE_INTEGER interval;
    interval.QuadPart = -(10 * 1000 * (__int64)milliseconds);
    KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

extern FUNCTION_TABLE_ENTRY function_table[];

#define printf(Format, ...) nt.fn_DbgPrintEx(0, 0, Format, __VA_ARGS__)

