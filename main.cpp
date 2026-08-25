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
                            if (lea_address == string)
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

                                            for (int i = 0; i < 20; i++)
                                            {
                                                printf("%02x", ((unsigned char*)target)[i]);
                                            }


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
            return 0;
            UINT64 start = win32k_base;
            UINT64 size = win32k_size;
            
            for (UINT64 i = 0; i < 2000; i++)
            {
                auto sig_0 = Utils::SigScan_s(start, size, "48 8D 0D ? ? ? ? 48");
                if (sig_0)
                {
                    start = sig_0 + 1;
                    size = win32k_size - (start - win32k_base);
                    auto rel = string - sig_0;
                    auto rel_offset = *(int*)(sig_0 + 3) + 7;
                    if (rel == rel_offset)
                    {
                        auto sig_1 = Utils::SigScan_s((UINT64)sig_0, 0x200, "48 8B 05 ? ? ? ? 48 8B 4C 24 20");
                        if (sig_1)
                        {
                            if (Utils::RvaValid(sig_1 + 3) && Utils::RvaValid((sig_1 + *(int*)(sig_1 + 3) + 7)))
                            {
                                auto rel2 = *(UINT64*)(sig_1 + *(int*)(sig_1 + 3) + 7);
                                auto sig_2 = Utils::SigScan_s((UINT64)rel2, 0x200, "48 8B 05 ? ? ? ? 48 85 C0");
                                if (sig_2)
                                {
                                    if (Utils::RvaValid(sig_2 + 3) && Utils::RvaValid((sig_2 + *(int*)(sig_2 + 3) + 7)))
                                    {
                                        //DbgPrintEx(0, 0, "{ Detected Windows 10 (%i) }\n", i);
                                        //auto pssdt = (sig_2 + *(int*)(sig_2 + 3) + 7);
                                        //DbgPrintEx(0, 0, "%p: mov rax, cs:qword_%p\n", (PVOID)sig_2, pssdt);
                                        //o_NtUserCompositionInputSinkViewInstanceIdFromPoint = (QWORD)(pssdt);
                                        //_NtUserCompositionInputSinkViewInstanceIdFromPoint = (FnPtr*)*(QWORD*)(pssdt);
                                        //*(QWORD*)(pssdt) = (QWORD)NtUserCompositionInputSinkViewInstanceIdFromPoint;
                                        //DbgPrintEx(0, 0, "Swapped %p for %p\n", (PVOID)_NtUserCompositionInputSinkViewInstanceIdFromPoint, (PVOID)NtUserCompositionInputSinkViewInstanceIdFromPoint);
                                        //DbgPrintEx(0, 0, "Successful\n");
    									return 1;
                                    }
                                }
                            }
    
                            if (Utils::RvaValid(sig_1 + 3) && Utils::RvaValid((sig_1 + *(int*)(sig_1 + 3) + 7)))
                            {
                                auto rel2 = *(UINT64*)(sig_1 + *(int*)(sig_1 + 3) + 7);
                                auto sig_2 = Utils::SigScan_s((UINT64)rel2, 0x200, "48 8B 80 ? ? ? ? 48 85 C0");
                                if (sig_2)
                                {
                                    if (Utils::RvaValid(sig_2 + 3))
                                    {
                                        //DbgPrintEx(0, 0, "{ Detected Windows 11 (%i) }\n", i);
                                        //auto offset = *(int*)(sig_2 + 3);
                                        //DbgPrintEx(0, 0, "%p: mov rax, [rax+%xh]\n", (PVOID)sig_1, offset);
                                        //auto s_gSessionGlobalSlots = *(UINT64*)Utils::ResolveRel32(3, Utils::SigScan_s(win32k_base, win32k_size, "48 8B 05 ? ? ? ? FF C9"));
                                        //
                                        //auto pssdt = *(QWORD*)(*(QWORD*)(*(QWORD*)s_gSessionGlobalSlots + 0x88ULL) + 0x150ULL) + (UINT64)offset;
                                        //
                                        //o_NtUserCompositionInputSinkViewInstanceIdFromPoint = (QWORD)(pssdt);
                                        //_NtUserCompositionInputSinkViewInstanceIdFromPoint = (FnPtr*)*(QWORD*)(pssdt);
                                        //*(QWORD*)(pssdt) = (QWORD)NtUserCompositionInputSinkViewInstanceIdFromPoint;
                                        //DbgPrintEx(0, 0, "Swapped %p for %p\n", (PVOID)_NtUserCompositionInputSinkViewInstanceIdFromPoint, (PVOID)NtUserCompositionInputSinkViewInstanceIdFromPoint);
                                        //DbgPrintEx(0, 0, "Successful\n");
                                        return 2;
                                    }
                                }
                            }
                        }
                    }
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


UINT64 pCallback_0 = 0;
FnPtr* oCallback_0 = nullptr;

UINT64 pCallback_1 = 0;
FnPtr* oCallback_1 = nullptr;

int count = 0;

UINT64 data = 0;

UINT64 CallbackFunction_1();

UINT64 CallbackFunction_0()
{
    count++;
    //data = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x10);

    printf("test");
    //*(UINT64*)pCallback_1 = (UINT64)CallbackFunction_1;


    return oCallback_0->invoke<UINT64>();
}

UINT64 CallbackFunction_1()
{
    count++;

    //*(UINT64*)pCallback_0 = (UINT64)CallbackFunction_0;

    return oCallback_1->invoke<UINT64>();
}

/*
UINT64 pCallback = 0;
FnPtr* oCallback = nullptr;
int count = 0;

UINT64 CallbackFunction()
{
    count++;
    return oCallback->invoke<UINT64>();
}*/


//HalpInterruptController  48 8B 1D ? ? ? ? 48 8B 83

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
                                if (MmGetPhysicalAddress((PVOID)rsp))
                                {
                                    LINEAR_ADDRESS rva = rsp;
                                    if (rva.pml4e_index > 128)
                                    {
                                        //printf("Thread: %p, ctx: %p, test: %p\n", (PVOID)thread, (PVOID)ctx, (PVOID)test);
                                        for (int i = 0; i < 70; i++)
                                        {
                                            if (((UINT64*)rsp)[i - 70] > text_base && ((UINT64*)rsp)[i - 70] < (text_base + text_size))
                                            {
                                                //printf("%i rsp: %p\n", i - 70, ((UINT64*)test)[i - 70]);

                                                //oCallback = (FnPtr*)((UINT64*)rsp)[i - 70];
                                                auto idx = i - 70;

                                                //if(!pCallback_1)
                                                //    pCallback_1 = (UINT64) & ((UINT64*)rsp)[idx];
                                                //else if(!pCallback_0)
                                                //pCallback_0 = (UINT64) & ((UINT64*)rsp)[idx];

                                                printf("ret [%p]%p %s\n", &((UINT64*)rsp)[idx], ((UINT64*)rsp)[idx], (process_current + 0x338));



                                                //printf("%p\n", &((UINT64*)rsp)[idx]);
                                                //((UINT64*)rsp)[idx] = (UINT64)CallbackFunction;
                                                //if (MmGetPhysicalAddress((PVOID)((UINT64*)test)[idx - 1]))
                                                //    printf("valid [%p]%p", (PVOID)((UINT64*)test)[idx - 1], *(PVOID*)((UINT64*)test)[idx - 1]);
                                                //printf("pop arg %p\n", ((UINT64*)test)[idx - 1]);
                                                //return;
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


NTSTATUS DriverEntry()
{

    printf("Count: %i\n", count);
    Test_Kthread();

	printf("pCallback_0: %p\n", (PVOID)pCallback_0);
	printf("pCallback_1: %p\n", (PVOID)pCallback_1);

    if (pCallback_0)
    {
        oCallback_0 = (FnPtr*)*(UINT64*)pCallback_0;
        *(UINT64*)pCallback_0 = (UINT64)CallbackFunction_0;
    }
	
    if (pCallback_1)
    {
        oCallback_1 = (FnPtr*)*(UINT64*)pCallback_1;
        //*(UINT64*)pCallback_1 = (UINT64)CallbackFunction_1;
    }

	Sleep(10);
    printf("Count: %i\n", count);
    printf("data: %p\n", data);
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