#pragma once

#include "structures.hpp"

extern "C"
{
    NAKED inline UINT64 _mm_readcr0()
    {
        __asm {
            mov rax, cr0
            ret
        }
    }

    NAKED inline UINT64 _mm_readcr2()
    {
        __asm {
            mov rax, cr2
            ret
        }
    }

    NAKED inline UINT64 _mm_readcr3()
    {
        __asm {
            mov rax, cr3
            ret
        }
    }

    NAKED inline UINT64 _mm_readcr4()
    {
        __asm {
            mov rax, cr4
            ret
        }
    }

    NAKED inline UINT64 _mm_readcr8()
    {
        __asm {
            mov rax, cr8
            ret
        }
    }

    NAKED inline VOID _mm_writecr0(_In_ UINT64 value)
    {
        __asm {
            mov cr0, rcx
            ret
        }
    }

    NAKED inline VOID _mm_writecr2(_In_ UINT64 value)
    {
        __asm {
            mov cr2, rcx
            ret
        }
    }

    NAKED inline VOID _mm_writecr3(_In_ UINT64 value)
    {
        __asm {
            mov cr3, rcx
            ret
        }
    }

    NAKED inline VOID _mm_writecr4(_In_ UINT64 value)
    {
        __asm {
            mov cr4, rcx
            ret
        }
    }

    NAKED inline VOID _mm_writecr8(_In_ UINT64 value)
    {
        __asm {
            mov cr8, rcx
            ret
        }
    }

    NAKED inline VOID _mm_clflushopt(PVOID address)
    {
        __asm {
            clflushopt [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_invlpg(_In_ PVOID address)
    {
        __asm {
            invlpg [rcx]
            ret
        }
    }

    NAKED inline UINT64 _mm_rdtsc()
    {
        __asm {
            rdtsc
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID _mm_swapgs()
    {
        __asm {
            swapgs
            ret
        }
    }

    NAKED inline UINT64 _mm_xgetbv(_In_ UINT32 xcr)
    {
        __asm {
            mov ecx, ecx
            xgetbv
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID _mm_xsetbv(_In_ UINT32 xcr, _In_ UINT64 value)
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

    NAKED inline VOID _mm_clwb(PVOID address)
    {
        __asm {
            clwb [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_prefetchw(PVOID address)
    {
        __asm {
            prefetchw [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_wbinvd()
    {
        __asm {
            wbinvd
            ret
        }
    }
    
    NAKED inline VOID _mm_invd()
    {
        __asm {
            invd
            ret
        }
    }

    inline VOID _mm_cpuidex(
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

    inline VOID _mm_cpuid(
        _In_ UINT32 leaf,
        _Out_ UINT64* rax,
        _Out_ UINT64* rbx,
        _Out_ UINT64* rcx,
        _Out_ UINT64* rdx)
    {
        _mm_cpuidex(
            leaf,
            0,
            rax,
            rbx,
            rcx,
            rdx);
    }

    NAKED inline UINT64 _mm_readmsr(_In_ UINT32 msr)
    {
        __asm {
            mov ecx, ecx
            rdmsr
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID _mm_writemsr(_In_ UINT32 msr, _In_ UINT64 value)
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

    NAKED inline UINT64 _mm_rdtscp(_Out_ UINT32* aux)
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

    NAKED inline UINT64 _mm_rdpmc(_In_ UINT32 counter)
    {
        __asm {
            mov ecx, ecx
            rdpmc
            shl rdx, 32
            or rax, rdx
            ret
        }
    }

    NAKED inline VOID _mm_sidt(_Out_ SEGMENT_REGISTER* idtr)
    {
        __asm {
            sidt [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_sgdt(_Out_ SEGMENT_REGISTER* gdtr)
    {
        __asm {
            sgdt [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_lidt(_In_ SEGMENT_REGISTER* idtr)
    {
        __asm {
            lidt [rcx]
            ret
        }
    }

    NAKED inline VOID _mm_lgdt(_In_ SEGMENT_REGISTER* gdtr)
    {
        __asm {
            lgdt [rcx]
            ret
        }
    }

    NAKED inline UINT16 _mm_sldt()
    {
        __asm {
            xor eax, eax
            sldt ax
            ret
        }
    }

    NAKED inline VOID _mm_lldt(_In_ UINT16 selector)
    {
        __asm {
            lldt cx
            ret
        }
    }

    NAKED inline UINT16 _mm_str()
    {
        __asm {
            xor eax, eax
            str ax
            ret
        }
    }

    NAKED inline VOID _mm_ltr(_In_ UINT16 selector)
    {
        __asm {
            ltr cx
            ret
        }
    }

    NAKED inline UINT16 _mm_readcs()
    {
        __asm {
            xor eax, eax
            mov ax, cs
            ret
        }
    }

    NAKED inline UINT16 _mm_readds()
    {
        __asm {
            xor eax, eax
            mov ax, ds
            ret
        }
    }

    NAKED inline UINT16 _mm_readss()
    {
        __asm {
            xor eax, eax
            mov ax, ss
            ret
        }
    }

    NAKED inline UINT16 _mm_reades()
    {
        __asm {
            xor eax, eax
            mov ax, es
            ret
        }
    }

    NAKED inline UINT16 _mm_readfs()
    {
        __asm {
            xor eax, eax
            mov ax, fs
            ret
        }
    }

    NAKED inline UINT16 _mm_readgs()
    {
        __asm {
            xor eax, eax
            mov ax, gs
            ret
        }
    }

    NAKED inline VOID _mm_writeds(_In_ UINT16 value)
    {
        __asm {
            mov ds, cx
            ret
        }
    }

    NAKED inline VOID _mm_writess(_In_ UINT16 value)
    {
        __asm {
            mov ss, cx
            ret
        }
    }

    NAKED inline VOID _mm_writees(_In_ UINT16 value)
    {
        __asm {
            mov es, cx
            ret
        }
    }

    NAKED inline VOID _mm_writefs(_In_ UINT16 value)
    {
        __asm {
            mov fs, cx
            ret
        }
    }

    NAKED inline VOID _mm_writegs(_In_ UINT16 value)
    {
        __asm {
            mov gs, cx
            ret
        }
    }

    NAKED inline PHYSICAL_ADDRESS _mm_vmrun(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmrun rax
            ret
        }
    }

    NAKED inline VOID _mm_vmsave(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmsave rax
            ret
        }
    }

    NAKED inline VOID _mm_vmload(_In_ PHYSICAL_ADDRESS vmcb)
    {
        __asm {
            mov rax, rcx
            vmload rax
            ret
        }
    }

    NAKED inline VOID _mm_stgi()
    {
        __asm {
            stgi
            ret
        }
    }

    NAKED inline VOID _mm_clgi()
    {
        __asm {
            clgi
            ret
        }
    }

    NAKED inline VOID _mm_vmmcall()
    {
        __asm {
            vmmcall
            ret
        }
    }

    NAKED inline VOID _mm_invlpga(
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

    NAKED inline VOID _mm_skinit(_In_ UINT32 slb_physical_address)
    {
        __asm {
            mov eax, ecx
            skinit
            ret
        }
    }
}
