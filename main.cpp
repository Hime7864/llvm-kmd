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

void LocateTextSource(UINT64 source)
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
                return;
            }
        }
        current_entry = current_entry->Flink;
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

int count = 0;

FnPtr* callback = nullptr;
UINT64 CallbackFunction()
{
    count++;
    return callback->invoke<UINT64>();
}


NTSTATUS DriverEntry()
{

    auto k_base = Utils::GetKernelBase();
	printf("Kernel base: %p\n", (PVOID)k_base);
    UINT64 text_base, text_size;
    if (NT_SUCCESS(Utils::GetSectionInfo(k_base, str_hash(".text"), &text_base, &text_size)))
    {
        auto sig_3 = Utils::SigScan(text_base, text_size, "48 8B 0D ? ? ? ? 44 8B 81");
        if (sig_3)
        {
            sig_3 = *(UINT64*)Utils::ResolveRel32(3, sig_3);//*(UINT64*)(Utils::ResolveRel32(3, sig_3) + 0x70);

            auto test = *(UINT64*)(sig_3 + 0x70);
			printf("test: %p\n", (PVOID)test);
            for (int i = 0; i < 20; i++)
            {
				printf("%02x", ((unsigned char*)test)[i]);
            }

            printf("count: %i\n", count);
            callback = (FnPtr*)*(UINT64*)(sig_3 + 0x70);
			*(UINT64*)(sig_3 + 0x70) = (UINT64)CallbackFunction;
			Sleep(1000);
            *(UINT64*)(sig_3 + 0x70) = (UINT64)callback;
			printf("count: %i\n", count);


			printf("thing that gets called %p\n", *(UINT64*)test);
            LocateTextSource(sig_3 + 0x70);
        }
    }

    

    auto id = KeGetCurrentProcessorNumberEx(nullptr);
	auto core_count = KeQueryActiveProcessorCount(0);
    id = (id + 1) % core_count;

    auto test = *(UINT64*)((UINT64)KeGetCurrentPrcb() + 0x10);
	printf("Prcb: %p\n", (PVOID)test);

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