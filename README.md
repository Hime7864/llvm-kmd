I don't like MSCV nor do I like WDK, so this is my driver base for unsigned drivers using LLVM/Clang.

This isn't close to being a complete rewrite of WDK and has only exports and structs that I use in my projects.

Support for Windows builds 22H2-25H2.

## Helper functions / API (non-NT-import wrappers)

This section lists the **project-specific helpers** exposed in the `KMD/` headers (i.e. not the NT import wrapper functions in `KMD/nt_methods.hpp`).

### `KMD/entry.hpp`

- **`NTSTATUS resolve_imports()`**: Resolve NT imports via exports/signatures (project-specific loader glue).
- **`NTSTATUS resolve_sigged_imports()`**: Resolve the “sigged” imports path.
- **`NTSTATUS volatile start()`**: Driver entry/start routine used by this base.

### `KMD/utils.hpp` (`class Utils`)

- **`QWORD ResolveRel32(UCHAR count, QWORD address)`**: Resolve a RIP-relative `rel32` reference at `address`.
- **`QWORD SigScan(...)`**: Pattern scan memory ranges (supports `Pattern<>` and IDA-style signature strings).
  - `template<size_t N, size_t Size> QWORD SigScan(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)`
  - `QWORD SigScan(QWORD scan_start, QWORD max_scan, PCSTR ida_sig)`
- **`QWORD SigScan_s(...)`**: “Safe” signature scan using `MmCopyMemory` reads (supports `Pattern<>` and IDA-style strings).
  - `template<size_t N, size_t Size> QWORD SigScan_s(QWORD scan_start, QWORD max_scan, Pattern<N, Size> pat)`
  - `QWORD SigScan_s(QWORD scan_start, QWORD max_scan, PCSTR ida_sig)`
- **`QWORD GetKernelBase()`**: Locate the kernel base address.
- **`QWORD GetProcAddress(QWORD module, PCSTR export_name)`**: Resolve an exported symbol from a PE image in memory.
- **`VOID GetProcAddressBuffer(QWORD module, FUNCTION_TABLE_ENTRY* table, DWORD count)`**: Bulk resolve exports into a table using `str_hash` hashes.
- **`NTSTATUS GetSectionInfo(QWORD module_base, PCSTR section_name, QWORD* section_address, QWORD* section_size)`**: Get section VA + size from a PE image.
- **`NTSTATUS GetSectionInfo(QWORD module_base, DWORD section_hash, QWORD* section_address, QWORD* section_size)`**: Same as above, but matches sections by `str_hash()` of the section name (up to 8 bytes).
- **`NTSTATUS ReadPhysical(PHYSICAL_ADDRESS address, PVOID buffer, SIZE_T size)`**: Read physical memory into a buffer.
  - `template <typename T> T ReadPhysical(PHYSICAL_ADDRESS address)`
- **Page table translation helpers**:
  - `PHYSICAL_ADDRESS LinearTranslatePPte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)`
  - `PHYSICAL_ADDRESS LinearTranslatePPte(LINEAR_ADDRESS rva)`
  - `MMPTE_HARDWARE LinearTranslatePte(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)`
  - `MMPTE_HARDWARE LinearTranslatePte(LINEAR_ADDRESS rva)`
  - `PHYSICAL_ADDRESS LinearTranslate(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)`
  - `PHYSICAL_ADDRESS LinearTranslate(LINEAR_ADDRESS rva)`
- **Linear (virtual) reads**:
  - `NTSTATUS ReadLinear(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)`
  - `template <typename T> T ReadLinear(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)`
  - `NTSTATUS ReadLinear(LINEAR_ADDRESS rva, PVOID buffer, SIZE_T size)`
  - `template <typename T> T ReadLinear(LINEAR_ADDRESS rva)`
- **`BOOLEAN RvaValid(...)`**: Validate that a virtual address is translatable/present.
  - `BOOLEAN RvaValid(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)`
  - `BOOLEAN RvaValid(LINEAR_ADDRESS rva)`
- **`NTSTATUS LocateSelf(QWORD* module_base, QWORD* module_size)`**: Locate this module in memory (base + size).

### `KMD/fwAlloc.hpp` (`class FWA`)

Firmware “free” physical page allocator backed by scanning for large zero-page runs.

- **`void Initialize()`**: Scan physical memory ranges and build the internal free list.
- **`void Cleanup()`**: Zero out any pages that were reserved and reset internal state.
- **`UINT32 pages_free()`**: Pages remaining.
- **`UINT32 pages_total()`**: Total pages tracked.
- **`PHYSICAL_ADDRESS ReservePages(SIZE_T pages)`**: Reserve a contiguous run of `pages` and return its physical base.

### `KMD/cpuid.hpp` (`class CPUID`)

- **`UINT32 current_core_number()`**: Current core index as reported by CPUID leaf `0xB`.
- **`LPSTR vendor_string()`**: CPU vendor string.
- **SVM helpers** (AMD):
  - `CPUID_SVM_REVISION svm_revision()`
  - `CPUID_SVM_REVISION_AND_FEATURE_IDENTIFICATION svm_revision_and_feature_identification()`
  - `CPUID_SVM_FEATURE_IDENTIFICATION svm_feature_identification()`
- **`CPUID_APMI apmi_features()`**: APMI feature bits (e.g. invariant TSC).

### `KMD/msr.hpp` (`class MSR`)

Convenience wrappers around common MSRs (read/write where applicable):

- **VM/SVM & syscall**: `VM_CR()`, `VM_CR(MSR_VM_CR)`, `LSTAR()`, `LSTAR(UINT64)`
- **EFER**: `EFER()`, `EFER(MSR_EFER)`
- **PAT**: `PAT()`, `PAT(MSR_PAT)`
- **FS/GS base**: `FS_BASE()`, `FS_BASE(UINT64)`, `GS_BASE()`, `GS_BASE(UINT64)`
- **HSAVE**: `HSAVE_PA()`, `HSAVE_PA(UINT64)`
- **APIC**: `APIC_BASE()`, `APIC_BASE(MSR_APIC_BASE)`, `ICR()`, `ICR(MSR_ICR)`
- **SMM/TSC/P-states**: `SMBASE()`, `SMM_ADDR()`, `SMM_MASK()`, `TSC_RATIO()`, `PSTATE_*()`, `PSTATE(int level)`, `TSC()`, `TSC(UINT64)`

### `KMD/intrinsics.hpp`

- **`Sleep(UINT32 milliseconds)`**: Kernel sleep helper (uses `KeDelayExecutionThread`).
