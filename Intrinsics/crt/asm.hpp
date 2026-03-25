#pragma once

#include "kmd_types.hpp"
#include "Architecture/layout.hpp"

extern "C"
{
    NAKED inline UINT64 __readcr0()
    {
        __asm {
            mov rax, cr0
            ret
        }
    }

    NAKED inline UINT64 __readcr2()
    {
        __asm {
            mov rax, cr2
            ret
        }
    }

    NAKED inline UINT64 __readcr3()
    {
        __asm {
            mov rax, cr3
            ret
        }
    }

    NAKED inline UINT64 __readcr4()
    {
        __asm {
            mov rax, cr4
            ret
        }
    }

    NAKED inline UINT64 __readcr8()
    {
        __asm {
            mov rax, cr8
            ret
        }
    }

    NAKED inline VOID __writecr0(_In_ UINT64 value)
    {
        __asm {
            mov cr0, rcx
            ret
        }
    }

    NAKED inline VOID __writecr2(_In_ UINT64 value)
    {
        __asm {
            mov cr2, rcx
            ret
        }
    }

    NAKED inline VOID __writecr3(_In_ UINT64 value)
    {
        __asm {
            mov cr3, rcx
            ret
        }
    }

    NAKED inline VOID __writecr4(_In_ UINT64 value)
    {
        __asm {
            mov cr4, rcx
            ret
        }
    }

    NAKED inline VOID __writecr8(_In_ UINT64 value)
    {
        __asm {
            mov cr8, rcx
            ret
        }
    }

    NAKED inline VOID __clflush(PVOID address)
    {
        __asm {
            clflush [rcx]
            ret
        }
    }

    NAKED inline VOID __clflushopt(PVOID address)
    {
        __asm {
            clflushopt [rcx]
            ret
        }
    }

    NAKED inline VOID __invlpg(_In_ PVOID address)
    {
        __asm {
            invlpg [rcx]
            ret
        }
    }

    NAKED inline VOID __kmd_pause()
    {
        __asm {
            pause
            ret
        }
    }

    NAKED inline VOID __kmd_lfence()
    {
        __asm {
            lfence
            ret
        }
    }

    NAKED inline VOID __kmd_sfence()
    {
        __asm {
            sfence
            ret
        }
    }

    NAKED inline VOID __kmd_mfence()
    {
        __asm {
            mfence
            ret
        }
    }

    NAKED inline UINT64 __kmd_rdtsc()
    {
        __asm {
            rdtsc
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID __kmd_swapgs()
    {
        __asm {
            swapgs
            ret
        }
    }

    NAKED inline UINT64 __kmd_xgetbv(_In_ UINT32 xcr)
    {
        __asm {
            mov ecx, ecx
            xgetbv
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID __kmd_xsetbv(_In_ UINT32 xcr, _In_ UINT64 value)
    {
        __asm {
            mov r8, rdx
            mov eax, r8d
            shr r8, 32
            mov edx, r8d
            xsetbv
            ret
        }
    }

    NAKED inline VOID __kmd_clwb(PVOID address)
    {
        __asm {
            clwb [rcx]
            ret
        }
    }

    NAKED inline VOID __kmd_prefetchw(PVOID address)
    {
        __asm {
            prefetchw [rcx]
            ret
        }
    }

    NAKED inline VOID __kmd_wbinvd()
    {
        __asm {
            wbinvd
            ret
        }
    }

    NAKED inline VOID __kmd_invd()
    {
        __asm {
            invd
            ret
        }
    }

    inline VOID _cpuidex(
        _In_ UINT32 leaf,
        _In_ UINT32 subleaf,
        _Out_ UINT64* rax,
        _Out_ UINT64* rbx,
        _Out_ UINT64* rcx,
        _Out_ UINT64* rdx)
    {
        UINT64 _rax, _rbx, _rcx, _rdx;
        __asm {
            mov eax, leaf
            mov ecx, subleaf
            cpuid
            mov [_rax], rax
            mov [_rbx], rbx
            mov [_rcx], rcx
            mov [_rdx], rdx
        }
        *rax = _rax;
        *rbx = _rbx;
        *rcx = _rcx;
        *rdx = _rdx;
    }

    inline VOID _cpuid(
        _In_ UINT32 leaf,
        _Out_ UINT64* rax,
        _Out_ UINT64* rbx,
        _Out_ UINT64* rcx,
        _Out_ UINT64* rdx)
    {
        _cpuidex(
            leaf,
            0,
            rax,
            rbx,
            rcx,
            rdx);
    }

    NAKED inline UINT64 __readmsr(_In_ UINT32 msr)
    {
        __asm {
            mov ecx, ecx
            rdmsr
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID __writemsr(_In_ UINT32 msr, _In_ UINT64 value)
    {
        __asm {
            mov r8, rdx
            mov ecx, ecx
            mov eax, r8d
            shr r8, 32
            mov edx, r8d
            wrmsr
            ret
        }
    }

    NAKED inline UINT64 __rdtscp(_Out_ UINT32* aux)
    {
        __asm {
            mov r8, rcx
            rdtscp
            test r8, r8
            jz rdtscp_done
            mov [r8], ecx
        rdtscp_done:
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline UINT64 __rdpmc(_In_ UINT32 counter)
    {
        __asm {
            mov ecx, ecx
            rdpmc
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID __sidt(_Out_ SEGMENT_REGISTER* idtr)
    {
        __asm {
            sidt [rcx]
            ret
        }
    }

    NAKED inline VOID __sgdt(_Out_ SEGMENT_REGISTER* gdtr)
    {
        __asm {
            sgdt [rcx]
            ret
        }
    }

    NAKED inline VOID __lidt(_In_ SEGMENT_REGISTER* idtr)
    {
        __asm {
            lidt [rcx]
            ret
        }
    }

    NAKED inline VOID __lgdt(_In_ SEGMENT_REGISTER* gdtr)
    {
        __asm {
            lgdt [rcx]
            ret
        }
    }

    NAKED inline UINT16 __sldt()
    {
        __asm {
            xor eax, eax
            sldt ax
            ret
        }
    }

    NAKED inline VOID __lldt(_In_ UINT16 selector)
    {
        __asm {
            lldt cx
            ret
        }
    }

    NAKED inline UINT16 __str()
    {
        __asm {
            xor eax, eax
            str ax
            ret
        }
    }

    NAKED inline VOID __ltr(_In_ UINT16 selector)
    {
        __asm {
            ltr cx
            ret
        }
    }

    NAKED inline UINT16 __readcs()
    {
        __asm {
            xor eax, eax
            mov ax, cs
            ret
        }
    }

    NAKED inline UINT16 __readds()
    {
        __asm {
            xor eax, eax
            mov ax, ds
            ret
        }
    }

    NAKED inline UINT16 __readss()
    {
        __asm {
            xor eax, eax
            mov ax, ss
            ret
        }
    }

    NAKED inline UINT16 __reades()
    {
        __asm {
            xor eax, eax
            mov ax, es
            ret
        }
    }

    NAKED inline UINT16 __readfs()
    {
        __asm {
            xor eax, eax
            mov ax, fs
            ret
        }
    }

    NAKED inline UINT16 __readgs()
    {
        __asm {
            xor eax, eax
            mov ax, gs
            ret
        }
    }

    NAKED inline VOID __writeds(_In_ UINT16 value)
    {
        __asm {
            mov ds, cx
            ret
        }
    }

    NAKED inline VOID __writess(_In_ UINT16 value)
    {
        __asm {
            mov ss, cx
            ret
        }
    }

    NAKED inline VOID __writees(_In_ UINT16 value)
    {
        __asm {
            mov es, cx
            ret
        }
    }

    NAKED inline VOID __writefs(_In_ UINT16 value)
    {
        __asm {
            mov fs, cx
            ret
        }
    }

    NAKED inline VOID __writegs(_In_ UINT16 value)
    {
        __asm {
            mov gs, cx
            ret
        }
    }

    NAKED inline PHYSICAL_ADDRESS __vmrun(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmrun rax
            ret
        }
    }

    NAKED inline VOID __vmsave(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmsave rax
            ret
        }
    }

    NAKED inline VOID __vmload(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmload rax
            ret
        }
    }

    NAKED inline VOID __stgi()
    {
        __asm {
            stgi
            ret
        }
    }

    NAKED inline VOID __clgi()
    {
        __asm {
            clgi
            ret
        }
    }

    NAKED inline VOID __kmd_vmmcall()
    {
        __asm {
            vmmcall
            ret
        }
    }

    NAKED inline VOID __kmd_invlpga(
        _In_ UINT64 address,
        _In_ UINT32 asid)
    {
        __asm {
            mov rax, rcx
            mov ecx, edx
            invlpga
            ret
        }
    }

    NAKED inline VOID __skinit(_In_ UINT32 slb_physical_address)
    {
        __asm {
            mov eax, ecx
            skinit
            ret
        }
    }
}
