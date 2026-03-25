#pragma once

#include "kmd_types.hpp"

struct PACKED LINEAR_ADDRESS
{
    union
    {
        UINT64 AsUINT64;
        PVOID AsPointer;

        struct
        {
            UINT64 offset : 12;
            UINT64 pte_index : 9;
            UINT64 pde_index : 9;
            UINT64 pdpte_index : 9;
            UINT64 pml4e_index : 9;
            UINT64 sign_extend : 16;
        };

        struct
        {
            UINT64 large_offset : 21;
            UINT64 large_pde_index : 9;
            UINT64 large_pdpte_index : 9;
            UINT64 large_pml4e_index : 9;
            UINT64 large_sign_extend : 16;
        };

        struct
        {
            UINT64 huge_offset : 30;
            UINT64 huge_pdpte_index : 9;
            UINT64 huge_pml4e_index : 9;
            UINT64 huge_sign_extend : 16;
        };
    };

    LINEAR_ADDRESS()
        : AsUINT64(0)
    {
    }

    template <typename T>
    LINEAR_ADDRESS(T data)
        : AsUINT64((UINT64)data)
    {
    }

    template <typename T>
    FORCEINLINE T as() const
    {
        return reinterpret_cast<T>(AsUINT64);
    }

    FORCEINLINE bool is_valid() const
    {
        return AsUINT64 != 0 && sign_extend == 0xFFFF;
    }

    FORCEINLINE bool is_canonical() const
    {
        return (sign_extend == 0x0000) || (sign_extend == 0xFFFF);
    }
};

union IDT_GATE
{
    struct
    {
        UINT128 offset_low : 16;
        UINT128 selector : 16;
        UINT128 ist : 3;
        UINT128 : 5;
        UINT128 type : 4;
        UINT128 : 1;
        UINT128 dpl : 2;
        UINT128 p : 1;
        UINT128 offset_mid : 16;
        UINT128 offset_high : 32;
        UINT128 : 32;
    };
    UINT128 AsUINT128;

    FORCEINLINE UINT64 offset()
    {
        return ((UINT64)offset_high << 32) | ((UINT64)offset_mid << 16) | offset_low;
    }

    FORCEINLINE VOID offset(UINT64 data)
    {
        offset_low = (UINT16)(data & 0xFFFF);
        offset_mid = (UINT16)((data >> 16) & 0xFFFF);
        offset_high = (UINT32)((data >> 32) & 0xFFFFFFFF);
        return;
    }
};

union SEGMENT_DESCRIPTOR_ACCESS
{
    struct
    {
        UINT8 A : 1;
        UINT8 RW : 1;
        UINT8 DC : 1;
        UINT8 E : 1;
        UINT8 S : 1;
        UINT8 DPL : 2;
        UINT8 P : 1;
    };
    UINT8 AsUINT8;
};

union SEGMENT_DESCRIPTOR_FLAGS
{
    struct
    {
        UINT8 : 1;
        UINT8 L : 1;
        UINT8 DB : 1;
        UINT8 G : 1;
    };
    UINT8 AsUINT8;
};

union SEGMENT_DESCRIPTOR
{
    struct
    {
        UINT128 limitLow : 16;
        UINT128 baseLow : 16;
        UINT128 baseMid : 8;
        UINT128 access : 8;
        UINT128 limitHigh : 4;
        UINT128 flags : 4;
        UINT128 baseHigh : 8;
        UINT128 baseUpper : 32;
    };
    UINT128 AsUINT128;

    FORCEINLINE UINT64 base()
    {
        return ((UINT64)baseUpper << 32) | ((UINT64)baseHigh << 24) | ((UINT64)baseMid << 16) | ((UINT64)baseLow);
    }

    FORCEINLINE VOID base(UINT64 base)
    {
        baseLow = (UINT16)(base & 0xFFFF);
        baseMid = (UINT8)((base >> 16) & 0xFF);
        baseHigh = (UINT8)((base >> 24) & 0xFF);
        baseUpper = (UINT32)((base >> 32) & 0xFFFFFFFF);
        return;
    }

    FORCEINLINE UINT32 limit()
    {
        return ((UINT64)limitHigh << 16) | ((UINT64)limitLow);
    }

    FORCEINLINE VOID limit(UINT32 limit)
    {
        limitLow = (UINT16)(limit & 0xFFFF);
        limitHigh = (UINT8)((limit >> 16) & 0x0F);
        return;
    }
};

struct PACKED INTERUPT_STACK_TABLE
{
    struct
    {
        unsigned : 32;
    };
    UINT64 RSP[3];
    UINT64 IST[8];
    struct
    {
        unsigned : 32;
    };
    struct
    {
        unsigned : 32;
    };
    struct
    {
        UINT16 : 16;
    };
    UINT16 IOPB;
};

struct PACKED SEGMENT_REGISTER
{
    UINT16 Limit;
    UINT64 Base;

    FORCEINLINE UINT32 limit(USHORT selector) const
    {
        auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);

        UINT32 limitLow = *(reinterpret_cast<UINT16*>(&DescBytes[0]));
        UCHAR granFlags = DescBytes[6];
        UINT32 limitHigh = granFlags & 0x0F;
        UINT32 segLimit = (limitHigh << 16) | limitLow;

        if ((granFlags & 0x80) != 0)
            segLimit = (segLimit << 12) | 0xFFF;
        return segLimit;
    }

    FORCEINLINE USHORT attribute(USHORT selector) const
    {
        const auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);
        const USHORT access = static_cast<USHORT>(DescBytes[5]);
        const USHORT flags = static_cast<USHORT>((DescBytes[6] & 0xF0) >> 4);

        return static_cast<USHORT>((flags << 8) | access);
    }

    FORCEINLINE UINT64 base(USHORT selector) const
    {
        const auto sr = this;
        if (!sr)
            return 0;
        UINT16 idx = selector >> 3;
        UINT64 offset = idx * 8;
        if (offset + 7 > sr->Limit)
            return 0;
        auto DescBytes = reinterpret_cast<UCHAR*>(sr->Base + offset);
        UINT32 baseLow = *(reinterpret_cast<UINT16*>(&DescBytes[2]));
        UINT32 baseMid = DescBytes[4];
        UINT32 baseHigh = DescBytes[7];
        UINT64 segBase = (static_cast<UINT64>(baseHigh) << 24) | (static_cast<UINT64>(baseMid) << 16) | baseLow;

        const UCHAR access = DescBytes[5];
        const bool system_desc = ((access & 0x10) == 0);
        if (system_desc && (offset + 15 <= sr->Limit))
        {
            const UINT32 baseUpper =
                (static_cast<UINT32>(DescBytes[8])) |
                (static_cast<UINT32>(DescBytes[9]) << 8) |
                (static_cast<UINT32>(DescBytes[10]) << 16) |
                (static_cast<UINT32>(DescBytes[11]) << 24);
            segBase |= (static_cast<UINT64>(baseUpper) << 32);
        }
        return segBase;
    }
};

enum ICR_MT : UINT32
{
    Fixed = 0,
    LowestPriority = 1,
    Smi = 2,
    RemoteRead = 3,
    Nmi = 4,
    Init = 5,
    StartUp = 6,
    ExtInt = 7
};

enum ICR_DM : UINT32
{
    Physical = 0,
    Logical = 1
};

enum ICR_DS : UINT32
{
    Idle = 0,
    SendPending = 1,
};

enum ICR_L : UINT32
{
    deassert = 0,
    assert = 1
};

enum ICR_TGM : UINT32
{
    edge = 0,
    level = 1
};

enum ICR_RRS : UINT32
{
    ReadInvalid = 0,
    DeliveryPending = 1,
    DeliveryDone = 2
};

enum ICR_DSH : UINT32
{
    Destination = 0,
    Self = 1,
    AllIncludingSelf = 2,
    AllExcludingSelf = 3
};

struct xAPIC_REGISTERS
{
    struct ICR_LOW
    {
        union
        {
            struct
            {
                UINT32 VEC : 8;
                ICR_MT MT : 3;
                ICR_DM DM : 1;
                ICR_DS DS : 1;
                UINT32 : 1;
                ICR_L L : 1;
                ICR_TGM TGM : 1;
                ICR_RRS RRS : 2;
                ICR_DSH DSH : 2;
                UINT32 : 12;
            };
            UINT32 AsUINT32;
        };
    };
    struct ICR_HIGH
    {
        union
        {
            struct
            {
                UINT32 : 24;
                UINT32 DES : 8;
            };
            UINT32 AsUINT32;
        };
    };

    bool isPending()
    {
        return ((ICR_LOW*)((UINT64)this + 0x300))->DS == ICR_DS::SendPending;
    }

    void WriteICR(ICR_LOW icr, ICR_HIGH dest)
    {
        if (icr.DSH == ICR_DSH::Destination)
            *(UINT32*)((UINT64)this + 0x314) = dest.AsUINT32;
        *(UINT32*)((UINT64)this + 0x300) = icr.AsUINT32;
        return;
    }
    void WriteICR(ICR_LOW icr)
    {
        if (icr.DSH == ICR_DSH::Destination)
            return;
        *(UINT32*)((UINT64)this + 0x300) = icr.AsUINT32;
        return;
    }
    void AddApicTimer(int delta)
    {
        UINT32 current = *(UINT32*)((UINT64)this + 0x390);
        *(UINT32*)((UINT64)this + 0x380) = current + delta;
        return;
    }
    UINT64 ApicTimerCurrent()
    {
        return *(UINT32*)((UINT64)this + 0x390);
    }
};
