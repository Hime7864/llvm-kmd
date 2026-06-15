#include "fwa.hpp"

bool NAKED FWA::is_zero_page(PVOID page)
{
    __asm
    {
        vpxor ymm0, ymm0, ymm0
        mov eax, 4096 / 32
        loop:
        vmovdqu ymm1, [rcx]
            vpxor ymm0, ymm0, ymm1
            add rcx, 32
            vptest ymm0, ymm0
            jnz nonzero

            dec eax
            jnz loop

            mov al, 1
            vzeroupper
            ret

            nonzero :
        xor eax, eax
            vzeroupper
            ret
    }
}