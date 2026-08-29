#include <intrinsics.hpp>


BOOLEAN GetKernelModuleBase(wchar_t* module_name, UINT64* base, UINT64* size)
{

    LIST_ENTRY* current_entry = nt.fn_PsLoadedModuleList->Flink;
    LIST_ENTRY* head = nt.fn_PsLoadedModuleList;

    while (current_entry != head)
    {
        KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(current_entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        // Check if BaseDllName is valid and matches the target module name (case-insensitive)
        if (entry->BaseDllName.Buffer && _wcsicmp(entry->BaseDllName.Buffer, module_name) == 0)
        {
            if (base)
                *base = (UINT64)entry->DllBase;
            if (size)
                *size = entry->SizeOfImage;
            return TRUE;
        }

        current_entry = current_entry->Flink;
    }
    return FALSE;
}

UINT64 LocateTextSource(UINT64 source)
{
    LIST_ENTRY* current_entry = nt.fn_PsLoadedModuleList->Flink;
    LIST_ENTRY* head = nt.fn_PsLoadedModuleList;

    while (current_entry != head)
    {
        KLDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(current_entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (entry->BaseDllName.Buffer)
        {
            if ((UINT64)entry->DllBase < source && ((UINT64)entry->DllBase + (UINT64)entry->SizeOfImage) > source)
            {
				DbgPrintEx(0, 0, "Found source in module: %wZ:%p -> %p\n", &entry->BaseDllName, entry->DllBase, source - (UINT64)entry->DllBase);
                return (UINT64)entry->DllBase;
            }
        }
        current_entry = current_entry->Flink;
    }
    return 0;
}

bool find_export(UINT64 module, UINT64 export_address, const char** out_name)
{
    if (!module)
        return 0;
    UINT64 e_lfanew = *(UINT32*)(module + 0x3C);
    UINT64 pe_header = module + e_lfanew;

    if (*(UINT32*)pe_header != 0x4550)
        return 0;

    UINT64 optional_header = pe_header + 0x18;
    UINT32 export_rva = *(UINT32*)(optional_header + 0x70);
    if (!export_rva)
        return 0;

    UINT64 export_table = module + export_rva;
    UINT32 number_of_names = *(UINT32*)(export_table + 0x18);
    UINT32 address_of_functions = *(UINT32*)(export_table + 0x1C);
    UINT32 address_of_names = *(UINT32*)(export_table + 0x20);
    UINT32 address_of_name_ordinals = *(UINT32*)(export_table + 0x24);

    for (UINT32 i = 0; i < number_of_names; i++)
    {
        UINT32 name_rva = *(UINT32*)(module + address_of_names + i * 4);
        if(out_name)
            *out_name = (const char*)(module + name_rva);
        UINT16 ordinal = *(UINT16*)(module + address_of_name_ordinals + i * 2);
        UINT32 function_rva = *(UINT32*)(module + address_of_functions + ordinal * 4);
        if (module + function_rva == export_address)
        {
            return true;
        }
            
    }
    return false;
}

void dtc_win32k_hook()
{
    UINT64 win32kfull_base, win32kfull_size;
    if (!GetKernelModuleBase(L"win32kfull.sys", &win32kfull_base, &win32kfull_size))
        return;

    UINT64 win32k_base, win32k_size;
    if (!GetKernelModuleBase(L"win32k.sys", &win32k_base, &win32k_size))
        return;

    UINT64 win32k_text_base, win32k_text_size;
    UINT64 init_win32k_text_base, init_win32k_text_size;
    if (NT_SUCCESS(Utils::GetSectionInfo(win32k_base, str_hash(".text"), &win32k_text_base, &win32k_text_size)))
    {
        init_win32k_text_base = win32k_text_base;
        init_win32k_text_size = win32k_text_size;
        for (;;)
        {
            auto sig_0 = Utils::SigScan(win32k_text_base, win32k_text_size, "48 8D 0D ? ? ? ? 48");
            if (sig_0)
            {
                auto diff = sig_0 - win32k_text_base;
                if (diff < win32k_text_size)
                {
                    win32k_text_base = sig_0 + 1;
                    win32k_text_size -= diff - 1;

                    auto lea_address = Utils::ResolveRel32(3, sig_0);

                    auto sig_1 = Utils::SigScan((UINT64)win32k_text_base - 1, win32k_text_size + 1, "48 8B 05 ? ? ? ? 48 8B 4C 24 20");
                    if (sig_1)
                    {
                        sig_1 = *(UINT64*)Utils::ResolveRel32(3, sig_1);
                        auto sig_2 = Utils::SigScan((UINT64)sig_1, (win32k_text_base + win32k_text_size) - (UINT64)sig_1, "48 8B ? ? ? ? ? 48 85 C0");
                        if (sig_2)
                        {

                            auto sig_3 = Utils::SigScan(init_win32k_text_base, init_win32k_text_size, "48 8B 05 ? ? ? ? FF C9");
                            if (sig_3)
                            {
                                auto offset = *(int*)(sig_2 + 3);
                                sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
                                sig_3 = *(UINT64*)sig_3;
                                sig_3 = *(UINT64*)(sig_3 + 0x88ULL);
                                sig_3 = *(UINT64*)(sig_3 + 0x150ULL);

                                auto pssdt = sig_3 + offset;
                                auto target = *(UINT64*)pssdt;

                                if (!find_export(win32kfull_base, target, nullptr) && target)
                                {
                                    printf("%s", lea_address);
                                    printf("Win32k Hooked -> [%p]%p\n", (PVOID)pssdt, (PVOID)target);
                                }
                            }
                        }
                    }
                }
                else
                    break;
            }
            else
                break;
        }
    }
    
    return;
}

UINT64 LocateSSDT()
{
    UINT64 win32k_base, win32k_size;
    if (GetKernelModuleBase(L"win32k.sys", &win32k_base, &win32k_size))
    {
		auto string = Utils::SigScan_s(win32k_base, win32k_size, "4e 74 55 73 65 72 43 6f 6d 70 6f 73 69 74 69 6f 6e 49 6e 70 75 74 53 69 6e 6b 56 69 65 77 49 6e 73 74 61 6e 63 65 49 64 46 72 6f 6d 50 6f 69 6e 74");
        if (string)
        {
            UINT64 win32k_text_base, win32k_text_size;
            UINT64 init_win32k_text_base, init_win32k_text_size;
            if (NT_SUCCESS(Utils::GetSectionInfo(win32k_base, str_hash(".text"), &win32k_text_base, &win32k_text_size)))
            {
				init_win32k_text_base = win32k_text_base;
				init_win32k_text_size = win32k_text_size;
                for(;;)
                {
                    auto sig_0 = Utils::SigScan(win32k_text_base, win32k_text_size, "48 8D 0D ? ? ? ? 48");
                    if (sig_0)
                    {
                        auto diff = sig_0 - win32k_text_base;
                        if (diff < win32k_text_size)
                        {
                            win32k_text_base = sig_0 + 1;
                            win32k_text_size -= diff - 1;

							auto lea_address = Utils::ResolveRel32(3, sig_0);
                            printf("%s\n", lea_address);
                            //if (lea_address == string)
                            {
                                auto sig_1 = Utils::SigScan((UINT64)win32k_text_base - 1, win32k_text_size + 1, "48 8B 05 ? ? ? ? 48 8B 4C 24 20");
                                if (sig_1)
                                {
									sig_1 = *(UINT64*)Utils::ResolveRel32(3, sig_1);
                                    auto sig_2 = Utils::SigScan((UINT64)sig_1, (win32k_text_base + win32k_text_size) - (UINT64)sig_1, "48 8B ? ? ? ? ? 48 85 C0");
                                    if (sig_2)
                                    {

										auto sig_3 = Utils::SigScan(init_win32k_text_base, init_win32k_text_size, "48 8B 05 ? ? ? ? FF C9");
                                        if (sig_3)
                                        {
											auto offset = *(int*)(sig_2 + 3);
											sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
											sig_3 = *(UINT64*)sig_3;
                                            sig_3 = *(UINT64*)(sig_3 + 0x88ULL);
                                            sig_3 = *(UINT64*)(sig_3 + 0x150ULL);
                                            auto pssdt = sig_3 + offset; 

											auto target = *(UINT64*)pssdt;
											printf("Target: %p\n", (PVOID)target);
											LocateTextSource(target);

											//\
                                            //if (win32k_base < target && win32k_base + win32k_size > target)
                                            //    printf("in\n");
                                            //else
											//	printf("out\n");
                                        }
                                        else
                                        {

                                        }
                                    }
                                }
                            }
                        }
                        else
                            break;
                    }
                    else
                        break;
                }
            }
        }
    }
    return 0;
}

NAKED KPRCB* KeGetCurrentPrcb()
{
    __asm {
        mov rax, gs:20h
        ret
    }
}

UINT64 GetUltraMapping()
{
    auto pte_base = MmPteBase();
    auto MmInternal = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
    auto pte_ptr = MmInternal + 0x68;
    return pte_base + ((*(UINT64*)pte_ptr >> 9) & 0x7FFFFFFFF8ULL);
}

UINT64 ReadPhysicalMemory(UINT64 physicalAddress)
{
    UINT64 data = 0;
    SIZE_T bytesCopied = 0;
    MmCopyMemory(&data, physicalAddress, sizeof(data), MM_COPY_MEMORY_PHYSICAL, &bytesCopied);
    return data;
}

BOOLEAN NmiCallback(PVOID context, BOOLEAN handled)
{
    return TRUE;
}

/*
void SVM::CreateInterruptHandler()
{
    SEGMENT_REGISTER k_gdtr{ 0 };
    __sgdt(&k_gdtr);

    SEGMENT_REGISTER k_idtr{ 0 };
    __sidt(&k_idtr);


    auto core_idx = CPUID::current_core_number();
    auto vCore = &vCpu[core_idx];

    //copy kernel GDT into host GDT
    RtlCopyMemory(vCore->hGdt, (PVOID)k_gdtr.Base, sizeof(vCore->hGdt));

    //copy kernel IDT into host IDT
    RtlCopyMemory(vCore->hIdt, (PVOID)k_idtr.Base, sizeof(vCore->hIdt));


    //4 is TSS idx
    auto tr = __str();
    vCore->hGdt[(tr >> 3) / 2].base((UINT64)&vCore->hIst);

    //2 is NMI idx
    vCore->hIdt[2].offset((UINT64)&SVM::NmiStub);

    vCore->hIst.IOPB = (UINT16)sizeof(vCore->hIst);
    vCore->hIst.IST[vCore->hIdt[2].ist] = (UINT64)&vCore->hstackIntr[0x1000];


    SEGMENT_REGISTER idtr{ 0 };
    idtr.Base = (UINT64)vCore->hIdt;
    idtr.Limit = k_idtr.Limit;
    __lidt(&idtr);

    SEGMENT_REGISTER gdtr{ 0 };
    gdtr.Base = (UINT64)vCore->hGdt;
    gdtr.Limit = k_gdtr.Limit;
    __lgdt(&gdtr);

    __sgdt(&k_gdtr);
    tr = __str();
    vCore->hGdt[(tr >> 3) / 2].access = 0x89;
    __ltr(tr);
    return;
}
*/



//HalpInterruptController  48 8B 1D ? ? ? ? 48 8B 83

UINT64 pCallback_scheduler = 0;
FnPtr* oCallback_scheduler = nullptr;
UINT64 idle_eproc = 0;
UINT64 ssa_bnk[100];


UINT64 CallbackFunction_scheduler(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    auto next_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x10);
    if (next_thread)
    {
        auto eproc = *(UINT64*)(next_thread + 0x220);
        if (*(UINT64*)(eproc + 0x338) == 0x656C6449)
        {
            idle_eproc = eproc;
            *(UINT64*)pCallback_scheduler = (UINT64)oCallback_scheduler;
        }
    }
    return oCallback_scheduler->invoke<UINT64>(a1, a2, a3, a4);
}

UINT64 get_scheduler_eproc()
{
    idle_eproc = 0;
    auto k_base = Utils::GetKernelBase();
    UINT64 text_base, text_size;
    if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    {
        auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 1D ? ? ? ? 48 8B 83");
        if (sig_3)
        {
            sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
            pCallback_scheduler = (sig_3 + 0x78);
            oCallback_scheduler = (FnPtr*)*(UINT64*)pCallback_scheduler;
            *(UINT64*)pCallback_scheduler = (UINT64)CallbackFunction_scheduler;
        }
    }
    while(!idle_eproc)
		Sleep(1);
    return idle_eproc;
}

UINT64 get_dwm_eproc()
{
    int thread_count = 0;
    auto process_current = (UINT64)PsInitialSystemProcess();
    auto o_UniqueProcessId = *(UINT32*)((UINT64)nt.fn_PsGetProcessId + 0x3);

    auto kernel_base = Utils::GetKernelBase();
    UINT64 text_base, text_size;
    Utils::GetSectionInfo(kernel_base, str_hash(".text"), &text_base, &text_size);

    do {
        if (*(UINT32*)(process_current + 0x338) == 0x2E6D7764)
        {
			return process_current;
        }
        process_current = *(UINT64*)(process_current + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
    } while ((process_current != (UINT64)PsInitialSystemProcess()));

    return 0;
}  


UINT64 FindExecution_text_base, FindExecution_text_size;
UINT64 FindExecution_idle_eproc = 0;
UINT64 FindExecution_dwm_eproc = 0;

UINT64 pCallback_0 = 0;
FnPtr* oCallback_0 = nullptr;

UINT64 pCallback_1 = 0;
FnPtr* oCallback_1 = nullptr;

int count = 0;

void FindExecution_Head();
void FindExecution_Tail();

bool lock_0, lock_1;

UINT64 CallbackFunction_0(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{

    for (int l = 0; l < KeQueryActiveProcessorCount(0); l++)
    {
        auto rsp = *(UINT64*)(ssa_bnk[l] - 0x68);
		printf("Core %i: %p\n", l, (PVOID) *(UINT64*)(ssa_bnk[l] - 0x8));
        *(UINT64*)(ssa_bnk[l] - 0x8) = 0x0;
        if (MmGetPhysicalAddress((PVOID)rsp))
        {
            int level = 500;
            for (int i = 0; i < level; i++)
            {
                if (MmGetPhysicalAddress((PVOID) & ((UINT64*)rsp)[i - level]))
                {
                    if (((UINT64*)rsp)[i - level] > FindExecution_text_base && ((UINT64*)rsp)[i - level] < (FindExecution_text_base + FindExecution_text_size))
                    {
                        printf("%03i found: %p\n", i, (PVOID)((UINT64*)rsp)[i - level]);
                        break;
                    }
                }
            }
        }
    }

    printf("S-Head :)");
    //_mm_writecr8(15);
    //
    //for (int i = 0; i < 100000; i++)
    //    _mm_pause();
    //
    //_mm_writecr8(0);
    //printf("E-Head :)");
    return oCallback_0->invoke<UINT64>(a1, a2, a3, a4);
}

UINT64 CallbackFunction_1(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    printf("S-Tail :)");
    _mm_writecr8(15);

    lock_0 = true;
    while (lock_0)
    {
        for(int i=0;i<10000;i++)
			_mm_pause();
        _mm_lfence();
        _mm_mfence();
        FindExecution_Head();
    }
    lock_1 = false;

    _mm_writecr8(0);
    printf("E-Tail :)");
    return oCallback_1->invoke<UINT64>(a1, a2, a3, a4);
}

void FindExecution_Head() 
{

    auto current_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8);

    auto thread_head = *(UINT64*)(FindExecution_idle_eproc + 0x370);
    auto thread_current = (LIST_ENTRY*)thread_head;;
    do {
        if (!MmGetPhysicalAddress(thread_current))
            break;

        if (current_thread != (UINT64)thread_current)
        {
            auto thread = (UINT64)thread_current - 0x578;

            if (MmGetPhysicalAddress((PVOID)(thread + 0x58)))
            {
                auto rsp = *(UINT64*)(thread + 0x58);
                if (MmGetPhysicalAddress((PVOID)rsp))
                {
                    LINEAR_ADDRESS rva = rsp;
                    if (rva.pml4e_index > 128)
                    {
                        int level = 150;
                        for (int i = 0; i < level; i++)
                        {
                            if (MmGetPhysicalAddress((PVOID)((UINT64*)rsp)[i - level]))
                            {
                                if (((UINT64*)rsp)[i - level] > FindExecution_text_base && ((UINT64*)rsp)[i - level] < (FindExecution_text_base + FindExecution_text_size))
                                {
                                    pCallback_0 = (UINT64) & ((UINT64*)rsp)[i - level];
                                    oCallback_0 = (FnPtr*)*(UINT64*)pCallback_0;
                                    *(UINT64*)pCallback_0 = (UINT64)CallbackFunction_0;
                                    printf("Found head: %p\n", (PVOID)pCallback_0);
                                    //break;
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            
        }
        thread_current = thread_current->Blink;
    } while ((UINT64)thread_current != thread_head);
    return;
}

void FindExecution_Tail()
{
    auto current_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8);

    auto thread_head = *(UINT64*)(FindExecution_dwm_eproc + 0x370);
    auto thread_current = (LIST_ENTRY*)thread_head;
    do {
        if (!MmGetPhysicalAddress(thread_current))
            break;
        if (current_thread != (UINT64)thread_current)
        {
            auto thread = (UINT64)thread_current - 0x578;

            if (MmGetPhysicalAddress((PVOID)(thread + 0x90)))
            {
                auto ctx = *(UINT64*)(thread + 0x90);
                if (ctx)
                {
                    if (MmGetPhysicalAddress((PVOID)(ctx + 0x180)))
                    {
                        auto rsp = *(UINT64*)(ctx + 0x180);
                        //auto rsp = *(UINT64*)(thread + 0x58);
                        if (MmGetPhysicalAddress((PVOID)rsp))
                        {
                            LINEAR_ADDRESS rva = rsp;
                            if (rva.pml4e_index > 128)
                            {
                                int level = 80;
                                for (int i = 0; i < level; i++)
                                {
                                    if (MmGetPhysicalAddress((PVOID)((UINT64*)rsp)[i - level]))
                                    {
                                        if (((UINT64*)rsp)[i - level] > FindExecution_text_base && ((UINT64*)rsp)[i - level] < (FindExecution_text_base + FindExecution_text_size))
                                        {
                                            pCallback_1 = (UINT64) & ((UINT64*)rsp)[i - level];
                                            oCallback_1 = (FnPtr*)*(UINT64*)pCallback_1;
                                            *(UINT64*)pCallback_1 = (UINT64)CallbackFunction_1;
                                            //printf("Found tail: %p\n", (PVOID)pCallback_1);
                                            return;
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
            }
            
        }
        thread_current = thread_current->Flink;
    } while ((UINT64)thread_current != thread_head);
    return;
}

void Test_Kthread()
{
    int thread_count = 0;
    auto process_current = (UINT64)PsInitialSystemProcess();
    auto o_UniqueProcessId = *(UINT32*)((UINT64)nt.fn_PsGetProcessId + 0x3);

    auto kernel_base = Utils::GetKernelBase();
    UINT64 text_base, text_size;
    Utils::GetSectionInfo(kernel_base, str_hash(".text"), &text_base, &text_size);


    auto self_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8);
    do {
        auto pid = *(UINT64*)(process_current + o_UniqueProcessId);

        auto thread_head = *(UINT64*)(process_current + 0x370);
        auto thread_current = (LIST_ENTRY*)thread_head;

        auto active_thread_count = *(UINT32*)(process_current + 0x380);
        if (active_thread_count)// && *(UINT32*)(process_current + 0x338) == 0x2E6D7764)
        {
            do {
                if (!MmGetPhysicalAddress(thread_current))
                    break;

                thread_count++;

                auto thread = (UINT64)thread_current - 0x578;

                if (thread != self_thread)
                {
                    auto apc_eproc = *(UINT64*)(thread + 0xB8);
                    if (apc_eproc == process_current)
                    {
                        auto ctx = *(UINT64*)(thread + 0x90);
                        if (ctx)
                        {
                            if (MmGetPhysicalAddress((PVOID)(ctx + 0x180)))
                            {
                                auto rsp = *(UINT64*)(ctx + 0x180);
                                //auto rsp = *(UINT64*)(ctx + 0x58);
                                if (MmGetPhysicalAddress((PVOID)rsp))
                                {
                                    LINEAR_ADDRESS rva = rsp;
                                    if (rva.pml4e_index > 128)
                                    {
                                        //printf("Thread: %p, ctx: %p, test: %p\n", (PVOID)thread, (PVOID)ctx, (PVOID)test);
                                        for (int i = 0; i < 200; i++)
                                        {
                                            if (((UINT64*)rsp)[i - 200] > text_base && ((UINT64*)rsp)[i - 200] < (text_base + text_size))
                                            {
                                                //printf("%i rsp: %p\n", i - 70, ((UINT64*)test)[i - 70]);

                                                //oCallback = (FnPtr*)((UINT64*)rsp)[i - 70];
                                                auto idx = i - 200;

                                                //if(!pCallback_1)
                                                //    pCallback_1 = (UINT64) & ((UINT64*)rsp)[idx];
                                                //else if(!pCallback_0)

                                                pCallback_0 = (UINT64) & ((UINT64*)rsp)[idx];
                                                oCallback_0 = (FnPtr*)*(UINT64*)pCallback_0;
                                                *(UINT64*)pCallback_0 = (UINT64)CallbackFunction_0;

                                                printf("ret [%p]%p %s\n", &((UINT64*)rsp)[idx], ((UINT64*)rsp)[idx], (process_current + 0x338));



                                                //printf("%p\n", &((UINT64*)rsp)[idx]);
                                                //((UINT64*)rsp)[idx] = (UINT64)CallbackFunction;
                                                //if (MmGetPhysicalAddress((PVOID)((UINT64*)test)[idx - 1]))
                                                //    printf("valid [%p]%p", (PVOID)((UINT64*)test)[idx - 1], *(PVOID*)((UINT64*)test)[idx - 1]);
                                                //printf("pop arg %p\n", ((UINT64*)test)[idx - 1]);
                                                return;
                                                break;
                                                //((UINT64*)test)[0 - i] = 0x0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                thread_current = thread_current->Flink;
            } while ((UINT64)thread_current != thread_head);
        }
        process_current = *(UINT64*)(process_current + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
    } while ((process_current != (UINT64)PsInitialSystemProcess()));
    return;
}



UINT64 pCallback_3 = 0;
FnPtr* oCallback_3 = nullptr;

UINT64 CallbackFunction_3(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    auto next_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x10);
    if (next_thread)
    {
        auto eproc = *(UINT64*)(next_thread + 0x220);
        if (*(UINT64*)(eproc + 0x338) == 0x656C6449)
        {
            printf("thread %p : %d\n", next_thread, CPUID::current_core_number()); 
        }
    }
    return oCallback_3->invoke<UINT64>(a1, a2, a3, a4);
}

UINT64 nigger()
{
    idle_eproc = 0;
    auto k_base = Utils::GetKernelBase();
    UINT64 text_base, text_size;
    if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    {
        auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 1D ? ? ? ? 48 8B 83");
        if (sig_3)
        {
            sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
            pCallback_3 = (sig_3 + 0x78);
            oCallback_3 = (FnPtr*)*(UINT64*)pCallback_3;
            *(UINT64*)pCallback_3    = (UINT64)CallbackFunction_3;
            Sleep(1000);
            *(UINT64*)pCallback_3 = (UINT64)oCallback_3;
        }
    }
    while (!idle_eproc)
        Sleep(1);
    return idle_eproc;
}


UINT64 pCallback_scheduler_log = 0;
FnPtr* oCallback_scheduler_log = nullptr;

UINT64 CallbackFunction_scheduler_log(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    auto next_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x10);
    if (next_thread)
    {
        auto eproc = *(UINT64*)(next_thread + 0x220);
        if (*(UINT64*)(eproc + 0x338) == 0x656C6449)
        {
			printf("thread %p : %d\n", next_thread, CPUID::current_core_number());
        }
    }
    return oCallback_scheduler_log->invoke<UINT64>(a1, a2, a3, a4);
}

void scheduler_log()
{
    auto k_base = Utils::GetKernelBase();
    UINT64 text_base, text_size;
    if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    {
        auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 1D ? ? ? ? 48 8B 83");
        if (sig_3)
        {
            sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
            pCallback_scheduler_log = (sig_3 + 0x78);
            oCallback_scheduler_log = (FnPtr*)*(UINT64*)pCallback_scheduler_log;
            *(UINT64*)pCallback_scheduler_log = (UINT64)CallbackFunction_scheduler_log;
            Sleep(1000);
            *(UINT64*)pCallback_scheduler_log = (UINT64)oCallback_scheduler_log;
        }
    }
    return;
}


void ipi_GetIdleThreadStack(UINT64* buffer)
{
    auto next_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x18);
    auto idx = KeGetCurrentProcessorNumberEx(nullptr);
    buffer[idx] = *(UINT64*)(next_thread + 0x58);
    return;
}

void GetIdleThreadStack(UINT64* buffer)
{
    KeIpiGenericCall(ipi_GetIdleThreadStack, buffer);
    return;
}

UINT64 pCallback = 0;
FnPtr* oCallback = nullptr;

UINT64 CallbackFunction(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    printf("%p %p %p %p", a1, a2, a3, a4);
    return oCallback->invoke<UINT64>(a1, a2, a3, a4);
}

void LocateCallback(UINT64* ptr, int cnt)
{
    //auto funny = Utils::SigScan(FindExecution_text_base, FindExecution_text_size, "48 8B C4 48 89 58 ? 56 57 41 54 41 56 41 57 48 83 EC ? 48 8B D9");
    //auto funny = Utils::SigScan(FindExecution_text_base, FindExecution_text_size, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 C0 0F 57 C0 0F 11 44 24 ? 33 F6");
    auto funny = Utils::SigScan(
        FindExecution_text_base,
        FindExecution_text_size, 
        "48 83 C4 ? C3 CC 83 F8 ? 74 ? 4C 8B 44 24"
    );

    auto funny2 = Utils::SigScan(
        FindExecution_text_base,
        FindExecution_text_size,
        "48 83 C4 ? C3 CC 45 33 C9 48 C7 44 24 ? ? ? ? ? 45 33 C0"
    );

    auto funny3 = Utils::SigScan(
        FindExecution_text_base,
        FindExecution_text_size,
        "48 81 C4 ? ? ? ? 41 5F 41 5E 41 5D 41 5C 5F 5E 5B 5D C3 CC 38 96"
	);

    auto funny4 = Utils::SigScan(
        FindExecution_text_base,
        FindExecution_text_size,
        "44 38 6D ? 74 ? 41 B7"
	);

    auto funny5 = Utils::SigScan(
        FindExecution_text_base,
        FindExecution_text_size,
        "8B 84 24 ? ? ? ? 4C 8B 6C 24 ? 4C 8B 64 24"
    );


    //48 83 C4 ? C3 CC 45 33 C9 48 C7 44 24 ? ? ? ? ? 45 33 C0
    
    printf("funny: %p\n", (PVOID)funny);

	auto k_base = Utils::GetKernelBase();



    for (int l = 0; l < cnt; l++)
    {
        auto rsp = ptr[l];
        if (MmGetPhysicalAddress((PVOID)rsp))
        {

			auto entry = (UINT64*)rsp;
			int depth = 1000;

            for (int i = 0; i < depth; i++)
            {
				auto ptr = &entry[i - depth];
                if (MmGetPhysicalAddress((PVOID)ptr))
                {
                    if (*ptr > FindExecution_text_base && *ptr < (FindExecution_text_base + FindExecution_text_size))
                    {


                        if (*ptr != funny && *ptr != funny2 && *ptr != funny3 && *ptr != funny4 && *ptr != funny5)
                        {
                            printf("Core %i: lvl %i | %p %p\n", l, i, (PVOID)ptr, (PVOID)((*ptr - k_base) + 0x140000000));
                            *ptr = (UINT64)ptr;
                        }
                        else
                        {
                            if (*ptr == funny)
                                printf("Log 1\n");
							if (*ptr == funny2)
								printf("Log 2\n");
                            if (*ptr == funny3)
								printf("Log 3\n");
							if (*ptr == funny4)
								printf("Log 4\n");
                            if (*ptr == funny5)
								printf("Log 5\n");
                        }
						
                    }
				}
            }



            //int level = 0x30000;
            //for (int i = 0; i < level; i++)
            //{
            //    auto idx = i - level;
            //    if(idx > 0x8000)
            //        break;
            //    if (MmGetPhysicalAddress((PVOID) & ((UINT64*)rsp)[idx]))
            //    {
            //        if (((UINT64*)rsp)[idx] && ((UINT64*)rsp)[idx] > FindExecution_text_base && ((UINT64*)rsp)[idx] < (FindExecution_text_base + FindExecution_text_size))
            //        {
            //            if ((UINT64)((UINT64*)rsp)[idx] == funny)
            //            {
            //                pCallback = (UINT64) & ((UINT64*)rsp)[idx];
            //                oCallback = (FnPtr*)*(UINT64*)pCallback;
            //                printf("Found callback: %p oCallback %p\n", (PVOID)pCallback, (PVOID)oCallback);
            //                return;
            //            }
            //        }   //
            //    }
            //}
		}
    }



    return;




    oCallback = (FnPtr*)0x0;
    //for (int x = 0; x < 10; x++)
    {
        //printf("GetIdleThreadStack %i\n", x);
        bool found = false;
        for (int l = 0; l < cnt; l++)
        {
            if(l == KeGetCurrentProcessorNumberEx(nullptr))
				continue;
            auto rsp = ptr[l];
            //printf("   base stack core %i: %p\n", l, (PVOID)rsp);
            if (MmGetPhysicalAddress((PVOID)rsp))
            {
                rsp = *(UINT64*)rsp;
                int level = 0x30000;
                for (int i = 0; i < level; i++)
                {
                    auto idx = i - level;
                    if(idx > 0x8000)
                        break;
                    if (MmGetPhysicalAddress((PVOID) & ((UINT64*)rsp)[idx]))
                    {

                        //if(funny == (((UINT64*)rsp)[idx]))
							//printf("Found funny: %p\n", (PVOID)((UINT64*)rsp)[idx]);
                        if (((UINT64*)rsp)[idx] && ((UINT64*)rsp)[idx] > FindExecution_text_base && ((UINT64*)rsp)[idx] < (FindExecution_text_base + FindExecution_text_size))
                        {
                            //pCallback = (UINT64) & ((UINT64*)rsp)[idx];
                            //oCallback = (FnPtr*)*(UINT64*)pCallback;
                        
                            
                            if ((UINT64)oCallback == funny)
                            {
                                found = true;
                                printf("!!! %i Found callback: %p oCallback %p\n", i, (PVOID)pCallback, (PVOID)oCallback);
                                //*(UINT64*)pCallback = (UINT64)CallbackFunction;
                                //Sleep(100);
                                //*(UINT64*)pCallback = (UINT64)oCallback;
                                //printf("!!! %i Found callback: %p oCallback %p\n", i, (PVOID)pCallback, (PVOID)oCallback);
                                //return;
                            }
								
                            //*(UINT64*)pCallback = (UINT32)pCallback;
                            //else
							//    printf("%i callback: %p oCallback %p\n", i, (PVOID)pCallback, (PVOID)oCallback);
                        }   //
                    }
                }
                if(found)
                    break;
            }
        }
       // Sleep(10);
    }
    return;
}

NTSTATUS DriverEntry()
{
    auto cnt = KeQueryActiveProcessorCount(0);
	auto ptr = (UINT64*)ExAllocatePool(NonPagedPoolNx, cnt * sizeof(UINT64));
    GetIdleThreadStack(ptr);

    Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &FindExecution_text_base, &FindExecution_text_size);
    
    printf("base %p\n", Utils::GetKernelBase());
    printf("text_base: %p, text_size: %p\n", (PVOID)FindExecution_text_base, (PVOID)FindExecution_text_size);
    LocateCallback(ptr, cnt);
    
    ExFreePool(ptr);

    return STATUS_SUCCESS;


    //Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &FindExecution_text_base, &FindExecution_text_size);
    //printf("text_base: %p, text_size: %p\n", (PVOID)FindExecution_text_base, (PVOID)FindExecution_text_size);
    //FindExecution_idle_eproc = get_scheduler_eproc();
    //
    //populate_ssa();
    //
    //for (int i = 0; i < 10; i++)
    //{
    //    FindExecution_Head();
    //    Sleep(10);
    //}
    

   

    
    


    //return STATUS_SUCCESS;
    //scheduler_log();
   // return STATUS_SUCCESS;
    //scheduler_log();
    //Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &FindExecution_text_base, &FindExecution_text_size);
	//printf("text_base: %p, text_size: %p\n", (PVOID)FindExecution_text_base, (PVOID)FindExecution_text_size);
    //FindExecution_idle_eproc = get_scheduler_eproc();

    FindExecution_Head();

    return STATUS_SUCCESS;
    //scheduler_log();

    //(0xFFFFA60D5EBBDC20);

    //Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &FindExecution_text_base, &FindExecution_text_size);
    //FindExecution_idle_eproc = get_scheduler_eproc();
	//FindExecution_dwm_eproc = get_dwm_eproc();
    //FindExecution_Head();
    ////FindExecution_Tail();


    //if(eproc)
	//    printf("Idle eproc: %p %s\n", eproc, (eproc + 0x338));
    //return STATUS_SUCCESS;
    //auto process_current = (UINT64)PsInitialSystemProcess();
    //auto o_UniqueProcessId = *(UINT32*)((UINT64)nt.fn_PsGetProcessId + 0x3);
    //
    //auto kernel_base = Utils::GetKernelBase();
    //UINT64 text_base, text_size;
    //Utils::GetSectionInfo(kernel_base, str_hash(".text"), &text_base, &text_size);
    //
    //auto self_thread = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8);
    //do {
    //    auto thread_head = *(UINT64*)(process_current + 0x370);
    //    auto thread_current = (LIST_ENTRY*)thread_head;
    //    auto DTB = *(UINT64*)((UINT64)process_current + 0x28);
    //    printf("%p %s\n", *(UINT64*)(process_current + 0x338), (process_current + 0x338));
    //    
    //    process_current = *(UINT64*)(process_current + o_UniqueProcessId + 0x8) - (o_UniqueProcessId + 0x8);
    //} while ((process_current != (UINT64)PsInitialSystemProcess()));





    //return STATUS_SUCCESS;


    //auto kernel_base = Utils::GetKernelBase();
    //Utils::GetSectionInfo(kernel_base, str_hash(".text"), &text_base_ntoskrl, &text_size_ntoskrl);
    //
    ////printf("before Count: %i\n", count);
    ////
    ////Test_Kthread();
    ////
    ////Sleep(10);
	////printf("Count: %i\n", count);
    ////0x8768
    //
	//auto rsp = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8768);
    //int deep = 2000;
    //for (int i = 0; i < deep; i++)
    //{
    //    if (((UINT64*)rsp)[i - deep] > text_base_ntoskrl && ((UINT64*)rsp)[i - deep] < (text_base_ntoskrl + text_size_ntoskrl))
    //    {
	//		printf("found: %p\n", (PVOID)((UINT64*)rsp)[i - deep]);
    //        //count = 0;
	//		//pCallback = (UINT64) & ((UINT64*)rsp)[i - deep];
    //        //oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        //*(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        break;
    //    }
    //}

	//printf("stack: %p\n", (PVOID)stack);



    //auto k_base = Utils::GetKernelBase();
    //printf("Kernel base: %p\n", (PVOID)k_base);
    //UINT64 text_base, text_size;
    //if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    //{
    //    auto sig_3 = Utils::SigScan(text_base, text_size, "E8 ? ? ? ? 8A D8 83 C9");
    //    if (sig_3)
    //    {
    //        sig_3 = *(UINT64*)Utils::ResolveRel32(1, sig_3);
    //        pCallback = sig_3;
    //        oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        count = 0;
    //        printf("count: %i\n", (PVOID)count);
    //        *(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        Sleep(10);
    //        *(UINT64*)pCallback = (UINT64)oCallback;
    //        printf("count: %i\n", (PVOID)count);
    //
    //    }
    //}
    
    //upper_half();
    //dtc_win32k_hook();

    //auto k_base = Utils::GetKernelBase();
    //printf("Kernel base: %p\n", (PVOID)k_base);
    //UINT64 text_base, text_size;
    //if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    //{
    //    auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 1D ? ? ? ? 48 8B 83");
    //    if (sig_3)
    //    {
    //        sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
    //        pCallback = (sig_3 + 0x78);
    //        oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        *(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        Sleep(10);
    //        *(UINT64*)pCallback = (UINT64)oCallback;
    //
    //        for (int i = 0; i < 500; i++)
    //        {
    //            if(data[i])
	//				printf("%p %s\n", data[i], (UINT64)(*(UINT64*)(data[i] + 0x220) + 0x338));
    //        }
    //
    //    }
    //}

    //printf("Count: %i\n", count);
    //Test_Kthread();

	//printf("pCallback_0: %p\n", (PVOID)pCallback_0);
	//printf("pCallback_1: %p\n", (PVOID)pCallback_1);
    ////
    //if (pCallback_0)
    //{
    //    oCallback_0 = (FnPtr*)*(UINT64*)pCallback_0;
    //    *(UINT64*)pCallback_0 = (UINT64)CallbackFunction_0;
    //}
	////
    ////if (pCallback_1)
    ////{
    ////    oCallback_1 = (FnPtr*)*(UINT64*)pCallback_1;
    ////    //*(UINT64*)pCallback_1 = (UINT64)CallbackFunction_1;
    ////}
    //
	//Sleep(1000);
    //printf("Count: %i\n", count);
    //printf("data: %p\n", data);
    return STATUS_SUCCESS;



   // _MMPTE_HARDWARE pte;
   // pte.AsUINT64 = 0xd27c4dee7326fbf1ull;
	//pte.PageFrameNumber = 0x1D5E;
   // pte.Valid = 1;
   // pte.NoExecute = 0;
   // pte.Dirty1 = 1;
   //
	//printf("pte: %p\n", (PVOID)pte.AsUINT64);
   //
   //
    ////00000001D5E5D000
    ////000000015D5E5000
    //auto kernel_base = Utils::GetKernelBase();
	//auto va = ExAllocatePool(NonPagedPoolNx, 0x1000);
	//auto pa_alloc = MmGetPhysicalAddress(va);
	//ExFreePool(va);
    //UINT64 pa = pa_alloc & ~0xFFF + 0x10000;
	//printf("pa: %p\n", (PVOID)pa);
	//auto ioSpace = MmMapIoSpace(pa, 0x1000, MmNonCached);
    //if (ioSpace)
    //{
    //    for (int i = 0; i < 511; i++)
    //    {
    //        auto pte = ((UINT64*)ioSpace)[i];
    //        printf("worked pte: %p\n", (PVOID)pte);
	//	}
	//	MmUnmapIoSpace(ioSpace, 0x1000);
    //}
    //else
    //{
    //    for (int i = 0; i < 511; i++)
    //    {
    //        printf("failed : pte: %p\n", (PVOID)ReadPhysicalMemory(pa + i * 8));
    //    }
    //}
    return STATUS_SUCCESS;

    //auto kernel_base = Utils::GetKernelBase();
    ////UINT64 text_base, text_size;
    ////if (NT_SUCCESS(Utils::GetSectionInfo(kernel_base, str_hash(".text"), &text_base, &text_size)))
    //{
	//	UINT64 section_base, section_size;
    //    if (NT_SUCCESS(Utils::GetSectionInfo(kernel_base, str_hash(".data"), &section_base, &section_size)))
    //    {
    //        int count2 = 0;
    //        for (int i = 0; i < section_size; i += 8)
    //        {
	//			auto ptr = *(UINT64*)(section_base + i);
    //
    //            auto imgBase = LocateTextSource(ptr);
    //            if (imgBase)
    //            {
    //                UINT64 text_base, text_size;
    //                if (NT_SUCCESS(Utils::GetSectionInfo(imgBase, str_hash(".text"), &text_base, &text_size)))
    //                {
    //                    if (ptr > text_base && ptr < (text_base + text_size))
    //                    {
    //                        hooks[count2].address = section_base + i;
    //                        hooks[count2].original = ptr;
    //                        count2++;
    //                        if (count2 > 1800)
    //                            break;
    //
    //                        count = 0;
    //                        pCallback = hooks[count2].address;
    //                        oCallback = (FnPtr*)*(UINT64*)pCallback;
    //                        *(UINT64*)pCallback = (UINT64)CallbackFunction;
    //                        Sleep(10);
    //                        *(UINT64*)pCallback = (UINT64)oCallback;
    //                        if(count)
    //                            printf("%04X [%p:%p] -> %p:%p\n", i, (UINT64)(section_base + i), 0x140000000 + (UINT64)(section_base + i) - imgBase, (UINT64)ptr, 0x140000000 + ptr - imgBase);
    //                    }
    //                }
    //            }
    //        }
    //
    //
    //        
	//		printf("count2: %i\n", count2);
    //        /*
    //        count = 0;
    //        pCallback = hooks[1274].address;
    //        oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        *(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        Sleep(100);
    //        *(UINT64*)pCallback = (UINT64)oCallback;
    //
	//		printf("count: %i\n", count);
    //
    //        */
    //
    //        //for (int i = 0; i < count2; i++)
    //        //{
    //        //    count = 0;
    //        //    pCallback = hooks[i].address;
    //        //    oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        //    *(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        //    Sleep(10);
    //        //    *(UINT64*)pCallback = (UINT64)oCallback;
    //        //
    //        //    if (count)
    //        //    {
	//		//		printf("idx:%i Hooked %p:%p cnt %i\n", i, hooks[i].address, 0x140000000 + hooks[i].address - kernel_base, count);
    //        //    }
    //        //}
    //
    //        //count = 0;
    //        //pCallback = 0xFFFFF80486FC0DE8;
    //        //oCallback = (FnPtr*)*(UINT64*)pCallback;
    //        //*(UINT64*)pCallback = (UINT64)CallbackFunction;
    //        //Sleep(10);
    //        //*(UINT64*)pCallback = (UINT64)oCallback;
    //        //
    //        //for (int i = 0; i < 20; i++)
    //        //{
	//		//	printf("Rax %p Rbx %p Rcx %p Rdx %p Rsi %p\n", ctx[i].Rax, ctx[i].Rbx, ctx[i].Rcx, ctx[i].Rdx, ctx[i].Rsi);
    //        //}
    //
    //    }
    //}
    //
    //
    //return STATUS_SUCCESS;




    //UINT64 win32k_base, win32k_size;
    //GetKernelModuleBase(L"win32k.sys", &win32k_base, &win32k_size);
    //UINT64 win32k_text_base, win32k_text_size;
    //Utils::GetSectionInfo(win32k_base, str_hash(".text"), &win32k_text_base, &win32k_text_size);
    //
    //auto sig_3 = Utils::SigScan(win32k_text_base, win32k_text_size, "48 8B 05 ? ? ? ? FF C9");



    //sig_78: FFFFF801B01D9F20 00000001403D9F20

	//auto ioSpace = MmMapIoSpace(0xFFFFF000, 0x1000, MmNonCached);
    //
	//
    //
    //if (ioSpace)
    //{
    //    //size_t bytesCopied = 0;
    //    //
    //    LINEAR_ADDRESS la;
    //    la.AsUINT64 = (UINT64)ioSpace;
	//	printf("l4e %i\n", la.pml4e_index);
    //
	//	auto tmp = MmMapIoSpace(ReadPhysicalMemory(_mm_readcr3() + la.pml4e_index * 8), 0x1000, MmNonCached);
    //    if (tmp)
    //    {
    //        printf("worked");
	//		MmUnmapIoSpace(tmp, 0x1000);
    //    }
    //    else
	//		printf("failed");
    //    
    //
	//	auto pa = Utils::LinearTranslate(ioSpace);
	//	printf("pa: %p\n", (PVOID)pa);
	//	//printf("pa: %p\n", (PVOID)pa);
    //
    //    //ReadPhysicalMemory(_mm_readcr3() + la.pml4e_index * 8);
    //
    //   //for (int i = 0; i < 511; i++)
    //   //    ((UINT64*)ioSpace)[i] = i; //ReadPhysicalMemory(_mm_readcr3() + i * 8);
    //    
    //    auto pte = ((UINT64*)ioSpace)[510];
    //    printf("pte: %p\n", (PVOID)pte);
    //
	//	//printf("status: %i\n", status);
	//	MmUnmapIoSpace(ioSpace, 0x1000);
    //}

    //auto k_base = Utils::GetKernelBase();
	////printf("Kernel base: %p\n", (PVOID)k_base);
    //UINT64 text_base, text_size;
    //if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    //{
    //    //auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 1D ? ? ? ? 48 8B 83");
    //    //if (sig_3)
    //    //{
    //    //    sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
    //    //    pCallback = (sig_3 + 0x78);
    //    //    oCallback = (FnPtr*)*(UINT64*)pCallback;
    //    //    //*(UINT64*)pCallback = (UINT64)CallbackFunction;
    //    //    Sleep(10);
    //    //    *(UINT64*)pCallback = (UINT64)oCallback;
    //    //    for (int i = 0; i < 200; i++)
    //    //    {
    //    //        if (batch_list[i])
    //    //        {
    //    //            printf("%p\n", batch_list[i]);
    //    //            //auto ptr = *(UINT64*)(batch_list[i] + 0x220);
    //    //            //if(ptr)
    //    //            //    printf("%p %i %s:%i\n", batch_list[i], *(BYTE*)(batch_list[i] + 0x71), (ptr + 0x338), *(UINT64*)(ptr + 0x1D0));
    //    //        }
    //    //            
    //    //    }
    //    //    //
    //        //printf("Rax: %p\n", ctx.Rax);
    //        //printf("Rbx: %p\n", ctx.Rbx);
    //        //printf("Rcx: %p\n", ctx.Rcx);
    //        //printf("Rdx: %p\n", ctx.Rdx);
    //        //printf("Rsi: %p\n", ctx.Rsi);
    //        //for (UINT64 i = 0x0; i < 0x300; i += 0x8)
    //        //            if(*(UINT64*)(sig_3 + i) - k_base < 0x140000000)
    //        //    	        printf("sig_%02X: %p %p\n", i, *(UINT64*)(sig_3 + i), 0x140000000 + *(UINT64*)(sig_3 + i) - k_base);
    //    //}
    //    auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 0D ? ? ? ? 44 8B 81");
    //    if (sig_3)
    //    {
    //        sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);//*(UINT64*)(Utils::ResolveRel32(3, sig_3) + 0x70);
    //    //
    //    //
    //    //    for(UINT64 i = 0x0;i<0x300;i+=0x8)
    //    //        if(*(UINT64*)(sig_3 + i) - k_base < 0x140000000)
	//	//	        printf("sig_%02X: %p %p\n", i, *(UINT64*)(sig_3 + i), 0x140000000 + *(UINT64*)(sig_3 + i) - k_base);
    //    //
    //    //    printf("%p\n", *(UINT64*)0xFFFFF801B0DC3720);
    //    //
    //    //    //printf("%p\n", *(UINT64*)0xFFFFF800EFF0F150);
    //    //
	//		pCallback = (sig_3 + 0x70);
	//		oCallback = (FnPtr*)*(UINT64*)pCallback;
	//		*(UINT64*)pCallback = (UINT64)CallbackFunction;
    //
	//		Sleep(10);
    //        for (int i = 0; i < count; i++)
    //        {
    //            if (batch_list[i])
    //            {
    //                auto ptr = *(UINT64*)(batch_list[i] + 0x220);
    //                if(ptr)
    //                    printf("%p %i %s:%i\n", batch_list[i], *(BYTE*)(batch_list[i] + 0x71), (ptr + 0x338), *(UINT64*)(ptr + 0x1D0));
    //            }
    //                
    //        }
    //    //    
    //    //
    //    //    //printf("count: %i\n", count);
    //    //    //callback = (FnPtr*)*(UINT64*)(sig_3 + 0x70);
	//	//	//*(UINT64*)(sig_3 + 0x70) = (UINT64)CallbackFunction;
	//	//	////Sleep(1000);
    //    //    //*(UINT64*)(sig_3 + 0x70) = (UINT64)callback;
	//	//	//printf("count: %i\n", count);
    //    //    //
    //    //    //auto sig_0 = Utils::ResolveRel32(3, Utils::SigScan(text_base, text_size, "48 8D 15 ? ? ? ? 49 8B 45"));
	//	//	//printf("old callback: %p\n", (PVOID)callback);
    //    //    //printf("sig_0: %p\n", (PVOID)sig_0);
    //    //    //
    //    //    //
    //    //    //
	//	//	//printf("thing that gets called %p\n", *(UINT64*)test);
    //    //    //LocateTextSource(sig_3 + 0x70);
    //    }
    //}

    
	//auto callback = KeRegisterNmiCallback(NmiCallback, nullptr);



	//_KAFFINITY_EX affinity;
    //KeInitializeAffinityEx(&affinity);
	//KeAddProcessorAffinityEx(&affinity, id);
    //HalSendNMI(&affinity);//fire next core

	//KeDeregisterNmiCallback(callback);

    //auto pte_base = MmPteBase();
    //auto MmInternal = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x8838);
	//printf("thing %p\n", *(UINT64*)(MmInternal + 0x144));


    //UINT64 win32k_base, win32k_size;
    //GetKernelModuleBase(L"win32k.sys", &win32k_base, &win32k_size);
    //UINT64 win32k_text_base, win32k_text_size;
    //Utils::GetSectionInfo(win32k_base, str_hash(".text"), &win32k_text_base, &win32k_text_size);
    //
    //auto sig_3 = Utils::SigScan(win32k_text_base, win32k_text_size, "48 8B 05 ? ? ? ? FF C9");
    //if (sig_3)
    //{
    //    sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);
    //    sig_3 = *(UINT64*)(sig_3);
    //    sig_3 = *(UINT64*)(sig_3 + 0x98);
    //    sig_3 = *(UINT64*)(sig_3 + 0x30);
    //    auto v0 = *(UINT64*)(sig_3 + 0xF70);
    //    //auto v1 = *(UINT64*)(sig_3 + 0x8F0);
    //    LocateTextSource(v0);//IsMulDestroyBrushInternalSupported
    //    printf("v0: %p\n", (PVOID)v0);
    //   // LocateTextSource(v1);//GreFlush
    //    //printf("v1: %p\n", (PVOID)v1);
    //
    //
    //    //v0 = *(int (**)(void))(*(_QWORD*)(W32GetWin32kBaseApiSetTable() + 0x18) + 0x8E8LL);
    //}


    //LocateSSDT();

    //FWA::Initialize();


    //UINT64 kernel_text_base, kernel_text_size;
    //Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &kernel_text_base, &kernel_text_size);
    //auto address = Utils::SigScan(kernel_text_base, kernel_text_size, pattern("0F 22 D9 F6 05"));
	
    //FWA::Initialize();


	//Found pattern at: 00000000006B47F0

    return STATUS_SUCCESS;
}