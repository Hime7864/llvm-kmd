#include "bootstrap.hpp"

volatile void FreeAndExit()
{
    UINT64 host_driver_base = 0;
    UINT64 host_driver_size = 0;
    if (!NT_SUCCESS(Utils::LocateSelf(&host_driver_base, &host_driver_size)))
        return;

    UINT64 text_base, text_size;
    Utils::GetSectionInfo(Utils::GetKernelBase(), str_hash(".text"), &text_base, &text_size);

    auto GadgetPopRcx = Utils::SigScan(text_base, text_size, "59 C3");
    auto GadgetAddRsp = Utils::SigScan(text_base, text_size, "48 83 C4 28 C3");

    _mm_writecr8(0);

    UINT64 func_free = (UINT64)nt.fn_ExFreePool;
    UINT64 func_term = (UINT64)nt.fn_PsTerminateSystemThread;
    auto func3 = nt.fn_RtlFillMemory;

    auto func_base = (PVOID)FreeAndExit;
    auto range1 = (UINT64)func_base - host_driver_base;
    auto range2 = (host_driver_size - (range1 + 0x200));

    func3((PVOID)host_driver_base, (SIZE_T)range1, 0x00);
    func3((PVOID)(host_driver_base + (range1 + 0x200)), (SIZE_T)range2, 0x00);
    func3(func_base, (SIZE_T)0xA0, 0x00);



    __asm {
        call [func_term]
        mov     rcx, [host_driver_base]
        mov     rdx, [func_free]
        mov     r8, [func_term]
        mov     r9, [GadgetPopRcx]
        mov     r10, [GadgetAddRsp]

        sub     rsp, 50h
        mov     qword ptr[rsp + 40h], r8
        mov     qword ptr[rsp + 38h], 0
        mov     qword ptr[rsp + 30h], r9
        mov     qword ptr[rsp + 00h], r10
        jmp     rdx
    }
}

void CleanupDriver()
{
    HANDLE thread_handle = 0;
    _OBJECT_ATTRIBUTES object_attribues{};
    InitializeObjectAttributes(&object_attribues, nullptr, OBJ_KERNEL_HANDLE, 0, nullptr);
    PsCreateSystemThread(&thread_handle, 0, &object_attribues, 0, 0, (PKSTART_ROUTINE)&FreeAndExit, 0);
    return;
}

NTSTATUS volatile start()
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    if (NT_SUCCESS(resolve_imports()))
    {
        KAPC_STATE apc{0};
        KeStackAttachProcess(PsInitialSystemProcess(), &apc);
        status = resolve_sigged_imports();
        if (NT_SUCCESS(status))
            status = DriverEntry();
        CleanupDriver();
        KeUnstackDetachProcess(&apc);
    }
    return status;
}