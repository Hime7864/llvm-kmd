#include <intrinsics.hpp>

struct PhysicalMemoryFlag
{
    UINT64 Valid : 1;
    UINT64 Commited : 1;
    UINT64 PageFrameNumber : 40;
    UINT64 InitCommit : 1;
    UINT64 Scattered : 1;
    UINT64 Contiguous : 1;
    UINT64 ContiguousStart : 1;
    UINT64 PatMapping : 1;
};

struct PhysicalMemoryDatabase
{
    MMPTE_HARDWARE* pPte;
    PVOID rva_alloc;
    SIZE_T rva_alloc_size;
    PhysicalMemoryFlag Flags;
};

struct MemoryManager
{
    PhysicalMemoryDatabase database[0x1000];

    static MemoryManager* Init()
    {
        constexpr auto buffer_pages = (sizeof(MemoryManager) >> 12) + ((sizeof(MemoryManager) & 0xFFF) ? 1 : 0);

        auto buffer = ExAllocatePool(NonPagedPoolNx, buffer_pages * 0x1000);

        RtlFillMemory(buffer, buffer_pages * 0x1000, 0);

        auto mm = (MemoryManager*)buffer;

        for (int i = 0; i < buffer_pages; i++)
        {
            PhysicalMemoryFlag flag{ 0 };
            flag.Valid = true;
            flag.Commited = true;
            flag.PageFrameNumber = Utils::LinearTranslate((UINT64)buffer + i * 0x1000) >> 12;
            flag.InitCommit = true;
            mm->InsertPage(flag);
        }

        for (int i = 0; i < 3800; i++)
        {
            auto buffer_scatter = ExAllocatePool(NonPagedPoolNx, 0x1000);
            RtlFillMemory(buffer_scatter, 0x1000, 0);
            PhysicalMemoryFlag flag{0};
            flag.Valid = true;
            flag.PageFrameNumber = Utils::LinearTranslate((UINT64)buffer_scatter) >> 12;
            flag.Scattered = true;
            mm->InsertPage(flag, buffer_scatter);
        }

        auto contiguous_buffer = MmAllocateContiguousMemorySpecifyCacheNode(
            (0x1000 - (3800 + buffer_pages)) * 0x1000, 
            0, 
            -1, 
            0, 
            MmNonCached, 
            0
        );
        RtlFillMemory(contiguous_buffer, (0x1000 - (3800 + buffer_pages)) * 0x1000, 0);

        for (int i = 0; i < (0x1000 - (3800 + buffer_pages)); i++)
        {
            PhysicalMemoryFlag flag{0};
            flag.Valid = true;
            flag.PageFrameNumber = Utils::LinearTranslate((UINT64)contiguous_buffer + i * 0x1000) >> 12;
            flag.Contiguous = true;
            if (i == 0)
            {
                flag.ContiguousStart = true;
                mm->InsertPage(flag, contiguous_buffer, (0x1000 - (3800 + buffer_pages)) * 0x1000);
            }
            else
            {
                mm->InsertPage(flag);
            }
        }

        return mm;
    }

    static void Free(MemoryManager* mm)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (mm->database[i].Flags.Valid && mm->database[i].Flags.Scattered && !mm->database[i].Flags.InitCommit)
            {
                RtlFillMemory(mm->database[i].rva_alloc, 0x1000, 0);
                ExFreePool(mm->database[i].rva_alloc);
            }
        }

        for (int i = 0; i < 0x1000; i++)
        {
            if (mm->database[i].Flags.Valid && mm->database[i].Flags.ContiguousStart)
            {
                RtlFillMemory(mm->database[i].rva_alloc, mm->database[i].rva_alloc_size, 0);
                MmFreeContiguousMemorySpecifyCache(mm->database[i].rva_alloc, mm->database[i].rva_alloc_size, MmNonCached);
            }
        }

        ExFreePool(mm);
    }

    void InsertPage(PhysicalMemoryFlag Flag, PVOID rva_alloc, SIZE_T rva_alloc_size)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (!database[i].Flags.PageFrameNumber)
            {
                database[i].Flags = Flag;
                database[i].rva_alloc = rva_alloc;
                database[i].rva_alloc_size = rva_alloc_size;
                break;
            }
            else if (database[i].Flags.PageFrameNumber > Flag.PageFrameNumber)
            {
                PhysicalMemoryDatabase current_tmp, next_tmp = database[i];
                current_tmp.Flags = Flag;
                current_tmp.rva_alloc = rva_alloc;
                current_tmp.rva_alloc_size = rva_alloc_size;

                for (int j = i; j < 0x1000; j += 2)
                {
                    database[j] = current_tmp;
                    current_tmp = database[j + 1];
                    database[j + 1] = next_tmp;
                    if (!current_tmp.Flags.PageFrameNumber)
                        break;
                    next_tmp = current_tmp;
                    current_tmp = database[j + 2];
                    auto switch_tmp = current_tmp;
                    current_tmp = next_tmp;
                    next_tmp = switch_tmp;
                    if (!current_tmp.Flags.PageFrameNumber)
                        break;
                }
                break;
            }
        }
    }

    void InsertPage(PhysicalMemoryFlag Flag, PVOID rva_alloc)
    {
        InsertPage(Flag, rva_alloc, 0);
    }

    void InsertPage(PhysicalMemoryFlag Flag)
    {
        InsertPage(Flag, nullptr, 0);
    }

    PhysicalMemoryDatabase* GetMemoryDescriptor(PHYSICAL_ADDRESS page)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (database[i].Flags.Valid && database[i].Flags.PageFrameNumber == (page >> 12))
                return &database[i];
        }
        return nullptr;
    }

    PHYSICAL_ADDRESS GetSingleScatterPage()
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (database[i].Flags.Valid && !database[i].Flags.Commited && database[i].Flags.Scattered)
            {
                database[i].Flags.Commited = true;
                return database[i].Flags.PageFrameNumber << 12;
            }
        }
        return 0;
    }

    PHYSICAL_ADDRESS GetContiguousPages(SIZE_T pages)
    {
        UINT64 idx = 1;
        auto last_pfn = (int)database[0].Flags.PageFrameNumber;
        for (int i = 1; i < 0x1000; i++)
        {
            auto current_pfn = (int)database[i].Flags.PageFrameNumber;

            if (current_pfn - last_pfn == 1)
                idx++;
            else
                idx = 0;

            if (idx == pages)
            {
                for (int j = i - (int)pages; j <= i; j++)
                    database[j].Flags.Commited = true;
                return ((UINT64)current_pfn - pages) << 12;
            }
            last_pfn = current_pfn;
        }
        return 0;
    }

    void FreeSinglePage(PHYSICAL_ADDRESS page)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (database[i].Flags.Valid && database[i].Flags.Commited && database[i].Flags.PageFrameNumber == (page >> 12))
                database[i].Flags.Commited = false;
        }
        return;
    }

    MMPTE_HARDWARE OverwriteMapping(PHYSICAL_ADDRESS ctx, LINEAR_ADDRESS rva, MMPTE_HARDWARE pt)
    {
        UINT64 idx[4] {
            rva.pml4e_index,
            rva.pdpte_index,
            rva.pde_index,
            rva.pte_index
        };

        PHYSICAL_ADDRESS PmPa = ctx;
        PhysicalMemoryDatabase* pPmd;
        MMPTE_HARDWARE* pPm;
        MMPTE_HARDWARE* pPt;

        for (int i = 0; i < 4; i++)
        {
            pPmd = GetMemoryDescriptor(PmPa);
            if (!pPmd)
            {
                printf("Failed to get memory descriptor for PA: %llx\n", PmPa);
                return MMPTE_HARDWARE{0};
            }
            pPm = (MMPTE_HARDWARE*)pPmd->rva_alloc;
            pPt = &pPm[idx[i]];
            if (i == 3)
            {
                auto old_pt = *pPt;
                *pPt = pt;
                return old_pt;
            }
            else
            {
                if (!pPt->Valid)
                {
                    printf("Entry is not valid for PA: %llx\n", PmPa);
                    return MMPTE_HARDWARE{0};
                }
                PmPa = pPt->PageFrameNumber << 12;
            }
        }
    }

    BOOLEAN CreateMapping(PHYSICAL_ADDRESS ctx, LINEAR_ADDRESS rva, MMPTE_HARDWARE pt, int page_level = PAGE_LEVEL_4KB)
    {
        UINT64 idx[4]{
            rva.pml4e_index,
            rva.pdpte_index,
            rva.pde_index,
            rva.pte_index};

        PHYSICAL_ADDRESS PmPa = ctx;
        PhysicalMemoryDatabase* pPmd;
        MMPTE_HARDWARE* pPm;
        MMPTE_HARDWARE* pPt;

        for (int i = 0; i < 4; i++)
        {
            // Check is if Physical Address is under our commitment
            pPmd = GetMemoryDescriptor(PmPa);
            if (!pPmd)
            {
                printf("Failed to get memory descriptor for PA: %llx\n", PmPa);
                return false;
            }
            pPmd->Flags.PatMapping = true;
            // get the virtual address of the PFN
            pPm = (MMPTE_HARDWARE*)pPmd->rva_alloc;

            // Create Pointer to the current level entry
            pPt = &pPm[idx[i]];

            // /---------------------------------------------
            // | Check 2 conditions
            // | 1) check if we are on leaf or not
            // |   a) if we are on leaf page, check if
            // |      valid is already set, if so fail
            // |      the function
            // |   b) if leaf isn't valid, copy the
            // |      source pt into entry
            // | 2) check if we are on a translation page
            // |   a) if valid check large bit, if so
            // |      split the page
            // |   b) if valid is set and not large
            // |      just reuse it
            // |   c) if not valid, create a new entry
            // \---------------------------------------------

            if (i == page_level)
            {
                if (page_level == PAGE_LEVEL_4KB)
                {
                    if (pPt->Valid)
                    {
                        printf("PAGE_LEVEL_4KB Leaf entry is already valid for PA: %llx\n", PmPa);
                        return false;
                    }
                    else
                    {
                        *pPt = pt;
                        return true;
                    }
                }
                else if (page_level == PAGE_LEVEL_2MB)
                {
                    if (pPt->Valid)
                    {
                        printf("PAGE_LEVEL_2MB Leaf entry is already valid for PA: %llx\n", PmPa);
                        return false;
                    }
                    else
                    {
                        *pPt = pt;
                        return true;
                    }
                }
                else if (page_level == PAGE_LEVEL_1GB)
                {
                    if (pPt->Valid)
                    {
                        printf("PAGE_LEVEL_1GB Leaf entry is already valid for PA: %llx\n", PmPa);
                        return false;
                    }
                    else
                    {
                        *pPt = pt;
                        return true;
                    }
                }
                return false;
            }
            else
            {
                if (pPt->Valid)
                {
                    if (pPt->LargePage)
                    {
                        if (i == PAGE_LEVEL_1GB) // 1gb page
                        {
                            printf("Splitting 1gb page for PA: %llx\n", PmPa);
                            auto old_pfn = pPt->PageFrameNumber;
                            pPt->LargePage = false;

                            auto new_pm_pa = GetSingleScatterPage();
                            if (!new_pm_pa)
                            {
                                printf("Failed to get single scatter page for splitting 1gb page at PA: %llx\n", PmPa);
                                return false;
                            }

                            auto new_pPmd = GetMemoryDescriptor(new_pm_pa);
                            if (!new_pPmd)
                            {
                                printf("Failed to get memory descriptor for new PM page at PA: %llx\n", new_pm_pa);
                                return false;
                            }

                            new_pPmd->Flags.PatMapping = true;

                            pPt->PageFrameNumber = new_pm_pa >> 12;
                            pPt->Write = true;

                            auto new_pm = (MMPTE_HARDWARE*)new_pPmd->rva_alloc;

                            for (int j = 0; j < 512; j++)
                            {
                                if (j == idx[i + 1])
                                {
                                    new_pm[j].AsUINT64 = 0x0;
                                    new_pm[j].PageFrameNumber = 0x0;
                                    i--;
                                    continue;
                                }
                                else
                                {
                                    new_pm[j].Valid = true;
                                    new_pm[j].LargePage = true;
                                    new_pm[j].Write = true;
                                    new_pm[j].PageFrameNumber = old_pfn + j * 512;
                                }
                            }
                        }
                        else if (i == PAGE_LEVEL_2MB) // 2mb page
                        {
                            printf("Splitting 2mb page for PA: %llx\n", PmPa);
                            auto old_pfn = pPt->PageFrameNumber;
                            pPt->LargePage = false;

                            auto new_pm_pa = GetSingleScatterPage();
                            if (!new_pm_pa)
                            {
                                printf("Failed to get single scatter page for splitting 2mb page at PA: %llx\n", PmPa);
                                return false;
                            }

                            auto new_pPmd = GetMemoryDescriptor(new_pm_pa);
                            if (!new_pPmd)
                            {
                                printf("Failed to get memory descriptor for new PM page at PA: %llx\n", new_pm_pa);
                                return false;
                            }

                            new_pPmd->Flags.PatMapping = true;

                            pPt->PageFrameNumber = new_pm_pa >> 12;
                            pPt->Write = true;

                            auto new_pm = (MMPTE_HARDWARE*)new_pPmd->rva_alloc;

                            for (int j = 0; j < 512; j++)
                            {
                                if (j == idx[i + 1])
                                {
                                    new_pm[j].AsUINT64 = 0x0;
                                    i--;
                                    continue;
                                }
                                else
                                {
                                    new_pm[j].Valid = true;
                                    new_pm[j].Write = true;
                                    new_pm[j].PageFrameNumber = old_pfn + j;
                                }
                            }
                        }
                    }
                    else
                    {
                        PmPa = pPt->PageFrameNumber << 12;
                    }
                }
                else
                {
                    pPt->Valid = true;
                    pPt->Write = true;

                    auto new_page = GetSingleScatterPage();
                    if (!new_page)
                    {
                        printf("Failed to get single scatter page for new entry at PA: %llx\n", PmPa);
                        return false;
                    }

                    auto new_pPmd = GetMemoryDescriptor(new_page);
                    if (!new_pPmd)
                    {
                        printf("Failed to get memory descriptor for new PM page at PA: %llx\n", new_page);
                        return false;
                    }
                    new_pPmd->Flags.PatMapping = true;

                    pPt->PageFrameNumber = new_page >> 12;

                    PmPa = new_page;
                }
            }
        }
        printf("Failed to create mapping for PA: %llx\n", PmPa);
        return false;
    }

    UINT64 MapIoSpace(PHYSICAL_ADDRESS physical_address, SIZE_T size)
    {

    }

    void log()
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (database[i].Flags.Valid && database[i].Flags.Commited)
            {
                if (database[i].Flags.PatMapping)
                    printf("%08X -> PatMapping\n", database[i].Flags.PageFrameNumber);
                
            }
        }
    }
};

MemoryManager* g_mm;

PHYSICAL_ADDRESS host_cr3;

struct NPT
{
    static PHYSICAL_ADDRESS Initialize(MemoryManager* mm)
    {
        auto ctx = g_mm->GetSingleScatterPage();
        for (int i = 0; i < 1024; i++)
        {
            MMPTE_HARDWARE pt { 0 };
            pt.Valid = true;
            pt.Write = true;
            pt.PageFrameNumber = i * 0x40000;
            pt.LargePage = true;

            auto result = mm->CreateMapping(ctx, pt.PageFrameNumber << 12, pt, PAGE_LEVEL_1GB);
        }

        return ctx;
    }
};

NTSTATUS DriverEntry()
{
    g_mm = MemoryManager::Init();

    auto npt_cr3 = NPT::Initialize(g_mm);
   
    LINEAR_ADDRESS npt = 0x0;

    MMPTE_HARDWARE pt{0};
    pt.Valid = true;
    pt.Write = true;
    pt.PageFrameNumber = 0xDEAD;
    pt.LargePage = true;

    npt.pde_index = 12;
    npt.pte_index = 24;

    printf("=== after ===\n");
    printf("-1) NPT addr %p -> %08X\n", npt.AsUINT64 - 0x1000, Utils::LinearTranslate(npt_cr3, npt.AsUINT64 - 0x1000));
    printf("0 ) NPT addr %p -> %08X\n", npt, Utils::LinearTranslate(npt_cr3, npt));
    printf("+1) NPT addr %p -> %08X\n", npt.AsUINT64 + 0x1000, Utils::LinearTranslate(npt_cr3, npt.AsUINT64 + 0x1000));

    g_mm->CreateMapping(npt_cr3, npt, pt);

    printf("=== before ===\n");
    printf("-1) NPT addr %p -> %08X\n", npt.AsUINT64 - 0x1000, Utils::LinearTranslate(npt_cr3, npt.AsUINT64 - 0x1000));
    printf("0 ) NPT addr %p -> %08X\n", npt, Utils::LinearTranslate(npt_cr3, npt));
    printf("+1) NPT addr %p -> %08X\n", npt.AsUINT64 + 0x1000, Utils::LinearTranslate(npt_cr3, npt.AsUINT64 + 0x1000));
    


    g_mm->log();

    MemoryManager::Free(g_mm);
    return STATUS_SUCCESS;
}
