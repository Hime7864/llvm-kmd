# llvm-kmd

x64 Windows kernel boilerplate for ClangCL, not the usual MSVC/WDK template.

Fork this when starting a new kernel research project. Put the actual work in `DriverEntry`, keep the import table, CRT, CPU helpers, FWA, and DTLB instead of rebuilding that setup each time.

> Lab and VM use only. This is not a signed production driver. Incorrect kernel-mode code can crash or corrupt the machine.

## Boot flow

The linker entry is `start`, not `DriverEntry`.

```
start → resolve_imports → resolve_sigged_imports → DriverEntry → CleanupDriver
```

Export-based NT APIs are filled first, then a few version-fragile internals are located by signature. `DriverEntry` runs attached to `PsInitialSystemProcess`. After it returns, `CleanupDriver` wipes the image and exits.

## Requirements

- Visual Studio 2022
- ClangCL toolset
- **x64 only.** The Win32 configurations in the project file are unused leftovers.

This is not a WDK driver template. It does not use the WDK import table; it resolves its own.

## Build

Open `llvm-kmd.sln`, select **x64** Debug or Release, and build. The output is `llvm-kmd.sys`.

## Using it

Include the umbrella header and implement `DriverEntry`:

```cpp
#include <intrinsics.hpp>

NTSTATUS DriverEntry()
{
    return STATUS_SUCCESS;
}
```

`printf` maps to `DbgPrintEx` once imports have resolved. The stub in `main.cpp` is intentionally empty; the value of the repo is the runtime around it.

## Layout

**Boot**

- `Intrinsics/bootstrap.hpp`, `Intrinsics/driver_boot.cpp` — custom startup, import resolve, and wipe/exit
- `main.cpp` — `DriverEntry`

**Imports**

- `Intrinsics/imports.hpp` — `NtImports` function pointers
- `Intrinsics/import_resolve.cpp` — export table plus signature-based internals
- `Intrinsics/structures.hpp` — NT types used without WDK headers

**CPU**

- `Intrinsics/assembly.hpp` — instruction wrappers
- `Intrinsics/cpuid.*`, `Intrinsics/msr.*` — AMD-oriented leaves and MSRs, added as needed

**Memory**

- `Intrinsics/fwa.*` — allocator over unused EFI/firmware physical ranges
- `Intrinsics/dtlb_*` — PTE poison / hosted translation for physical access without Windows mapping APIs

**CRT and helpers**

- `Intrinsics/crt.*` — freestanding string/memory routines
- `Intrinsics/utils_*` — PE parse, signature scan, paging, self-location
- `Intrinsics/intrinsics.hpp` — single include for driver code

## Extending

- **Exported NT API:** add a pointer to `NtImports` and a `{str_hash("Name"), &nt.fn_Name}` row in `function_table`.
- **Version-fragile internal:** resolve it in `resolve_sigged_imports()` with a `.text` signature. Retune when the build changes.
- **MSR / CPUID:** add the leaf or register you actually need. The set is not meant to be complete.

## Limitations

- AMD-focused; Intel coverage is whatever happened to be useful.
- Not a WDK replacement and not tested across a Windows build matrix.
- Signature-based imports break across versions and need retuning.
- x64 ClangCL only.
