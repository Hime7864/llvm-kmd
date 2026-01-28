### Summary of what `main.cpp` does

`main.cpp` implements the driver’s entrypoint (`DriverEntry`) and a **page-table scan** that runs immediately when the driver is loaded. It outputs its findings using `DbgPrintEx` (aliased as `printf` in this file).

- **Enumerates processes**: Starts from `PsInitialSystemProcess()` and walks the active process list (using a calculated `EPROCESS` offset) to visit each process. It skips PID `4` and only scans processes that appear to have a user-mode PEB pointer.

- **Reads each process DTB / CR3**: For each target `EPROCESS`, it reads the Directory Table Base (`DTB`) from the process object (`eproc + 0x28`) and uses that as the base for page-table traversal.

- **Walks 4-level page tables (4KB pages only)**: It iterates PML4 → PDPT → PDT → PTE entries and only descends through **non–large page** mappings (`LargePage == 0`), i.e. it focuses on **4KB mappings**.

- **Two scan modes per process**:
  - **Sessioned scan** (`LocateUsermodePteManipulationSessioned`): Attaches to the target process (`KeStackAttachProcess`), copies page-table pages from physical memory, compares them against the live mapped page-table pages, and prints when it detects differences (“pt tampering detected”).
  - **Ntoskrnl/System-context scan** (`LocateUsermodePteManipulationNtoskrnl`): Attaches to the system process and reads page-table pages from physical memory without relying on the target’s address space mapping.

- **Flags “non-normal” page usage**: For valid leaf PTEs it maps the referenced PFN and uses `MiGetSystemRegionType()` to classify the backing page. It prints messages when the type indicates translation/page-table usage, driver-backed pages, MMIO, or other non-normal types, along with the reconstructed virtual address (from the PML4/PDPT/PDT/PTE indices) and PFN.

- **Times the scan**: `ScanEProcess()` measures execution time using the TSC and an MSR-derived frequency, then prints how long the scan took.

In short: **loading the driver triggers a system-wide sweep of user processes’ page tables to spot unexpected mappings and potential page-table/PTE manipulation, reporting results to the kernel debugger.**
