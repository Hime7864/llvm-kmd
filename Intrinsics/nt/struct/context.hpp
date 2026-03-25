#pragma once

#include "../fwd.hpp"

#include "xsave.hpp"

struct _CONTEXT
{
    UINT64 P1Home;       // 0x0
    UINT64 P2Home;       // 0x8
    UINT64 P3Home;       // 0x10
    UINT64 P4Home;       // 0x18
    UINT64 P5Home;       // 0x20
    UINT64 P6Home;       // 0x28
    UINT32 ContextFlags; // 0x30
    UINT32 MxCsr;        // 0x34
    UINT16 SegCs;        // 0x38
    UINT16 SegDs;        // 0x3a
    UINT16 SegEs;        // 0x3c
    UINT16 SegFs;        // 0x3e
    UINT16 SegGs;        // 0x40
    UINT16 SegSs;        // 0x42
    UINT32 EFlags;       // 0x44
    UINT64 Dr0;          // 0x48
    UINT64 Dr1;          // 0x50
    UINT64 Dr2;          // 0x58
    UINT64 Dr3;          // 0x60
    UINT64 Dr6;          // 0x68
    UINT64 Dr7;          // 0x70
    UINT64 Rax;          // 0x78
    UINT64 Rcx;          // 0x80
    UINT64 Rdx;          // 0x88
    UINT64 Rbx;          // 0x90
    UINT64 Rsp;          // 0x98
    UINT64 Rbp;          // 0xa0
    UINT64 Rsi;          // 0xa8
    UINT64 Rdi;          // 0xb0
    UINT64 R8;           // 0xb8
    UINT64 R9;           // 0xc0
    UINT64 R10;          // 0xc8
    UINT64 R11;          // 0xd0
    UINT64 R12;          // 0xd8
    UINT64 R13;          // 0xe0
    UINT64 R14;          // 0xe8
    UINT64 R15;          // 0xf0
    UINT64 Rip;          // 0xf8
    union
    {
        XSAVE_FORMAT FltSave; // 0x100
        struct
        {
            M128A Header[2]; // 0x100
            M128A Legacy[8]; // 0x120
            M128A Xmm0;      // 0x1a0
            M128A Xmm1;      // 0x1b0
            M128A Xmm2;      // 0x1c0
            M128A Xmm3;      // 0x1d0
            M128A Xmm4;      // 0x1e0
            M128A Xmm5;      // 0x1f0
            M128A Xmm6;      // 0x200
            M128A Xmm7;      // 0x210
            M128A Xmm8;      // 0x220
            M128A Xmm9;      // 0x230
            M128A Xmm10;     // 0x240
            M128A Xmm11;     // 0x250
            M128A Xmm12;     // 0x260
            M128A Xmm13;     // 0x270
            M128A Xmm14;     // 0x280
            M128A Xmm15;     // 0x290
        };
    };
    M128A VectorRegister[26];    // 0x300
    UINT64 VectorControl;        // 0x4a0
    UINT64 DebugControl;         // 0x4a8
    UINT64 LastBranchToRip;      // 0x4b0
    UINT64 LastBranchFromRip;    // 0x4b8
    UINT64 LastExceptionToRip;   // 0x4c0
    UINT64 LastExceptionFromRip; // 0x4c8
};
