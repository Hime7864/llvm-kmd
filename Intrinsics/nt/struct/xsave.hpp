#pragma once

#include "../fwd.hpp"

// 0x10 bytes (sizeof)
struct _M128A
{
    UINT64 Low; // 0x0
    INT64 High; // 0x8
};

// 0x200 bytes (sizeof)
struct _XSAVE_FORMAT
{
    UINT16 ControlWord;      // 0x0
    UINT16 StatusWord;       // 0x2
    UINT8 TagWord;           // 0x4
    UINT8 Reserved1;         // 0x5
    UINT16 ErrorOpcode;      // 0x6
    UINT32 ErrorOffset;      // 0x8
    UINT16 ErrorSelector;    // 0xc
    UINT16 Reserved2;        // 0xe
    UINT32 DataOffset;       // 0x10
    UINT16 DataSelector;     // 0x14
    UINT16 Reserved3;        // 0x16
    UINT32 MxCsr;            // 0x18
    UINT32 MxCsr_Mask;       // 0x1c
    M128A FloatRegisters[8]; // 0x20
    M128A XmmRegisters[16];  // 0xa0
    UINT8 Reserved4[96];     // 0x1a0
};
