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

#include "Entry.hpp"
#include "Main.hpp"
