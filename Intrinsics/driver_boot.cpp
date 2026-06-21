#include "bootstrap.hpp"

volatile void FreeAndExit()
{
    UINT64 host_driver_base = 0;
    UINT64 host_driver_size = 0;
    if (!NT_SUCCESS(Utils::LocateSelf(&host_driver_base, &host_driver_size)))
        return;
    _mm_writecr8(0);
    Sleep(200);
    UINT64 func1 = (UINT64)nt.fn_ExFreePool;
    UINT64 func2 = (UINT64)nt.fn_PsTerminateSystemThread;
    auto func3 = nt.fn_RtlFillMemory;
    auto func_base = (PVOID)FreeAndExit;
    auto range1 = (UINT64)func_base - host_driver_base;
    auto range2 = (host_driver_size - (range1 + 0x200));
    auto cr3 = _mm_readcr3();

    func3((PVOID)host_driver_base, (SIZE_T)range1, 0x00);
    func3((PVOID)(host_driver_base + (range1 + 0x200)), (SIZE_T)range2, 0x00);
    func3(func_base, (SIZE_T)0xA0, 0x00);

    //if (cr3 == 0x1AD000ull)
    //{
    //    __asm {
    //        xor ecx, ecx
    //        mov rax, [func2]
    //        call rax
    //    }
    //}
    __asm {
        mov r10, [func3]
        sub rsp, 0x200h
        mov rax, rsp
        mov rcx, rax
        mov rdx, 0x200
		xor r8d, r8d
		call r10
		add rsp, 0x200h
        mov rcx, [host_driver_base]
        mov rdx, [func1]
        mov r8, [func2]
        xor r9d, r9d
		sub rsp, 100h
        call self
    self:
        sub rsp, 8h
        mov rax, r8
        mov[rsp], rax
        mov rax, rdx
        jmp rax
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