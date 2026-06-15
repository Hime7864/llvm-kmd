# llvm-kmd

`llvm-kmd` is a small x64 Windows kernel-mode boilerplate built around Clang/LLVM-style development rather than the normal MSVC/WDK workflow.

The goal is to provide a reusable base for low-level kernel projects where importing undocumented functions, structures, CPU intrinsics, and utility code into a fresh WDK project would otherwise mean repeating the same setup work each time.

## Why This Exists

I created this project because ill equipped for the kind of kernel research projects I work on. When working with undocumented routines and structures, bringing everything into a new WDK project repeatedly becomes tedious. This repository is meant to be a personal boilerplate that I can fork when starting something new.

It also pulls in two smaller projects:

- `DTLB`: helpers for accessing physical-address without relying on Windows physical-memory APIs.
- `FWA`: allocator working with memory outside normal Windows commitments, using unused regions of EFI memory.

The CPU support is intentionally focused on what I currently need, mostly AMD-oriented MSR and CPUID helpers. The project does not try to cover every MSR or CPUID leaf up front; new definitions can be added as future projects need them.

## Project Layout

- `main.cpp`: current driver entry point.
- `Intrinsics/assembly.hpp`: low-level CPU instruction wrappers.
- `Intrinsics/cpuid.*`: CPUID feature helpers.
- `Intrinsics/msr.*`: MSR helpers and definitions.
- `Intrinsics/crt.*`: minimal CRT routines for freestanding kernel-style code.
- `Intrinsics/imports.hpp`: kernel import table and wrapper functions.
- `Intrinsics/import_resolve.cpp`: export and signature-based import resolution.
- `Intrinsics/utils_*`: PE parsing, signature scanning, address translation, and location helpers.
- `Intrinsics/fwa.*`: firmware/unused physical memory allocation helpers.
- `Intrinsics/dtlb_*`: DTLB/page-table helpers for accessing physical memory.
- `Intrinsics/bootstrap.hpp` and `Intrinsics/driver_boot.cpp`: custom startup, import resolution, and cleanup flow.

## Build Notes

The Visual Studio project is configured for x64 ClangCL builds and emits a `.sys` target for x64 configurations. This is not a standard WDK driver template, and it intentionally avoids relying on normal WDK imports in favor of its own import table and supporting definitions.

This repository is best treated as a research and boilerplate base. Kernel-mode code can crash or corrupt the system if used incorrectly, so test only in controlled environments such as VMs or dedicated test machines.

## Current State

The current `DriverEntry` is intentionally minimal and only prints a debug message. Most of the value in the repository is the supporting runtime and low-level helper code rather than a finished driver feature.

## Limitations

- x64-focused.
- AMD-focused in the current MSR/CPUID coverage.
- Not a complete WDK replacement.
- No broad Windows build compatibility matrix.
- Signature-based imports may need updates across Windows versions.
- No automated tests are currently included.
