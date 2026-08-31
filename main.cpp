#include <intrinsics.hpp>

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

int stackPtrCnt = 0;
int stackPtridx = 0;
int counter = 0;
UINT64 StackPtrBank[64];
FnPtr* OldRet = nullptr;

volatile UINT64 CallbackFunction(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    stackPtridx++;
    int idx = stackPtridx;
    auto ptr = (StackPtrBank[idx % stackPtrCnt] - 0x98);
    for (;;)
    {
        *(UINT64*)ptr = (UINT64)CallbackFunction;
        _mm_clflushopt((PVOID)ptr);
        _mm_pause();
        _mm_lfence();
        _mm_mfence();
        if (stackPtridx != idx)
            break;
    }
	printf("Callback %i\n", stackPtridx);
    return OldRet->invoke<UINT64>(a1, a2, a3, a4);
}

void InitCallback()
{
    stackPtrCnt = KeQueryActiveProcessorCount(0);
    GetIdleThreadStack(StackPtrBank);

    stackPtridx = 0;
    int idx = stackPtridx;
    auto ptr = (StackPtrBank[idx % stackPtrCnt] - 0x98);
    OldRet = (FnPtr*)*(UINT64*)ptr;
    for (;;)
    {
        *(UINT64*)ptr = (UINT64)CallbackFunction;
        _mm_clflushopt((PVOID)ptr);
        _mm_pause();
        _mm_lfence();
        _mm_mfence();
        if (stackPtridx != idx)
            break;
    }
    return;
}

NTSTATUS DriverEntry()
{
	printf("DriverEntry\n");
    
    InitCallback();

    Sleep(3000);

    return STATUS_SUCCESS;
}