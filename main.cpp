#include <intrinsics.hpp>

#define ZYAN_NO_LIBC
#define ZYDIS_STATIC_BUILD
#define ZYCORE_STATIC_BUILD
#define ZYDIS_DISABLE_FORMATTER
#define ZYDIS_DISABLE_ENCODER
#include <Zydis/Zydis.h>

PHYSICAL_MEMORY_RANGE fw_range[31];
UINT64 fw_range_count = 0;
UINT64 page_idx = 0;

struct UefiPeRegion
{
    UINT64 phys;
    UINT64 size; // page-aligned SizeOfImage
};

struct UefiHole
{
    UINT64 phys;
    UINT64 size;
    UINT32 pe_begin; // index into uefi_pe[]
    UINT32 pe_count;
};

static UefiPeRegion uefi_pe[128];
static UINT64 uefi_pe_count = 0;
static UefiHole uefi_hole[16];
static UINT64 uefi_hole_count = 0;

static constexpr UINT64 kMaxPrefixHits = 2;

bool NAKED is_zero_page(PVOID page)
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

static bool IsCalleeSavedGpr64(ZydisRegister reg)
{
    switch (reg)
    {
    case ZYDIS_REGISTER_RBX:
    case ZYDIS_REGISTER_RBP:
    case ZYDIS_REGISTER_RDI:
    case ZYDIS_REGISTER_RSI:
    case ZYDIS_REGISTER_R12:
    case ZYDIS_REGISTER_R13:
    case ZYDIS_REGISTER_R14:
    case ZYDIS_REGISTER_R15:
        return true;
    default:
        return false;
    }
}

static bool MatchFunctionPrefix(ZydisDecoder* dec, const UINT8* p, ZyanUSize len)
{
    ZyanUSize ip = 0;
    int push_count = 0;
    bool saw_lea = false;
    bool saw_sub = false;

    while (push_count < 15)
    {
        ZydisDecodedInstruction insn;
        ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];
        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(dec, p + ip, len - ip, &insn, ops)))
            return false;

        if (insn.mnemonic != ZYDIS_MNEMONIC_PUSH)
            break;

        // Real prologues push 64-bit callee-saved regs only (not rdx/rsp/imm/pushw).
        if (insn.operand_count_visible < 1 ||
            ops[0].type != ZYDIS_OPERAND_TYPE_REGISTER ||
            !IsCalleeSavedGpr64(ops[0].reg.value))
            return false;

        push_count++;
        ip += insn.length;
    }

    if (push_count < 3)
        return false;

    while (ip < len && !(saw_lea && saw_sub))
    {
        ZydisDecodedInstruction insn;
        ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];
        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(dec, p + ip, len - ip, &insn, ops)))
            break;

        // lea rbp, [mem]
        if (insn.mnemonic == ZYDIS_MNEMONIC_LEA && !saw_sub &&
            insn.operand_count_visible >= 2 &&
            ops[0].type == ZYDIS_OPERAND_TYPE_REGISTER &&
            ops[0].reg.value == ZYDIS_REGISTER_RBP &&
            ops[1].type == ZYDIS_OPERAND_TYPE_MEMORY)
        {
            saw_lea = true;
            ip += insn.length;
            continue;
        }

        // sub rsp, imm/reg
        if (insn.mnemonic == ZYDIS_MNEMONIC_SUB &&
            ops[0].type == ZYDIS_OPERAND_TYPE_REGISTER &&
            ops[0].reg.value == ZYDIS_REGISTER_RSP)
        {
            saw_sub = true;
            ip += insn.length;
            break;
        }
        break;
    }

    // Require real frame setup: sub rsp, optionally preceded by lea rbp.
    if (!saw_sub)
        return false;

    // Look-ahead: next 10 must decode; first must not be another PUSH/POP.
    for (int i = 0; i < 10; ++i)
    {
        ZydisDecodedInstruction insn;
        ZydisDecodedOperand ops[ZYDIS_MAX_OPERAND_COUNT];
        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(dec, p + ip, len - ip, &insn, ops)))
            return false;

        if (i == 0 &&
            (insn.mnemonic == ZYDIS_MNEMONIC_PUSH || insn.mnemonic == ZYDIS_MNEMONIC_POP))
            return false;

        ip += insn.length;
    }

    return true;
}

// Returns SizeOfImage on success, 0 if page is not a page-aligned AMD64 PE.
static UINT32 TryGetPeSizeOfImage(UINT64 page)
{
    if (*(UINT16*)page != 0x5A4D)
        return 0;

    UINT32 e_lfanew = *(UINT32*)(page + 0x3C);
    if (e_lfanew >= 0x1000 - 0x58)
        return 0;

    if (*(UINT16*)(page + e_lfanew) != 0x4550)
        return 0;

    if (*(UINT16*)(page + e_lfanew + 4) != 0x8664)
        return 0;

    return *(UINT32*)(page + e_lfanew + 0x50);
}

static UINT64 PageAlignImageSize(UINT32 sizeofimage)
{
    return (sizeofimage & ~0xFFF) + (sizeofimage & 0xFFF ? 0x1000 : 0);
}

// Scan one page top-down (low→high offsets). Returns true when hit cap is reached.
static bool ScanPageForPrefixes(ZydisDecoder* dec, UINT64 page_rva, UINT64 page_phys,
    UINT64* hits, UINT64 max_hits)
{
    _mm_invlpg((PVOID)page_rva);

    if (is_zero_page((PVOID)page_rva))
        return false;

    for (UINT64 off = 0; off < 0x1000; ++off)
    {
        const UINT8* p = (const UINT8*)page_rva + off;
        const ZyanUSize avail = 0x1000 - off;

        if (!MatchFunctionPrefix(dec, p, avail))
            continue;

        DbgPrintEx(0, 0, "[llvm-kmd]: out-of-module UEFI fn @ phys 0x%llx\n",
            page_phys + off);

        if (++(*hits) >= max_hits)
            return true;
    }

    return false;
}

// Test helper: map [phys, phys+size) and byte-scan for function prefixes.
// Returns number of hits found (capped at max_hits).
UINT64 TestScanPhysicalForPrefixes(UINT64 phys, UINT64 size, UINT64 max_hits)
{
    if (!size || !max_hits)
        return 0;

    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    PHYSICAL_ADDRESS pa;
    pa = phys;

    auto rva = (UINT64)MmMapIoSpace(pa, size, MmNonCached);
    if (!rva)
    {
        DbgPrintEx(0, 0, "[llvm-kmd]: TestScan map failed phys=0x%llx size=0x%llx\n",
            phys, size);
        return 0;
    }

    DbgPrintEx(0, 0, "[llvm-kmd]: TestScan phys=0x%llx size=0x%llx max_hits=%llu\n",
        phys, size, max_hits);

    UINT64 hits = 0;
    for (UINT64 off = 0; off < size && hits < max_hits; ++off)
    {
        const UINT8* p = (const UINT8*)rva + off;
        const ZyanUSize avail = (ZyanUSize)(size - off);

        if (!MatchFunctionPrefix(&decoder, p, avail))
            continue;

        DbgPrintEx(0, 0, "[llvm-kmd]: TEST hit @ phys 0x%llx\n", phys + off);
        hits++;
    }

    MmUnmapIoSpace((PVOID)rva, size);

    DbgPrintEx(0, 0, "[llvm-kmd]: TestScan done, hits=%llu\n", hits);
    return hits;
}

// Walk pages from high→low (bottom→top), but each page is scanned top-down.
static bool ScanGapBackwards(ZydisDecoder* dec, UINT64 gap_lo, UINT64 gap_hi, UINT64* hits)
{
    if (gap_hi <= gap_lo || (gap_hi - gap_lo) < 0x1000)
        return false;

    // Expect page-aligned bounds (PE images are page-aligned).
    gap_lo = (gap_lo + 0xFFF) & ~0xFFFULL;
    gap_hi &= ~0xFFFULL;
    if (gap_hi <= gap_lo)
        return false;

    const UINT64 gap_size = gap_hi - gap_lo;
    PHYSICAL_ADDRESS pa;
    pa = gap_lo;

    auto rva = (UINT64)MmMapIoSpace(pa, gap_size, MmNonCached);
    if (!rva)
        return false;

    bool done = false;
    for (UINT64 page_phys = gap_hi - 0x1000; ; page_phys -= 0x1000)
    {
        if (page_phys < gap_lo)
            break;

        UINT64 page_rva = rva + (page_phys - gap_lo);
        if (ScanPageForPrefixes(dec, page_rva, page_phys, hits, kMaxPrefixHits))
        {
            done = true;
            break;
        }

        if (page_phys == gap_lo)
            break;
    }

    MmUnmapIoSpace((PVOID)rva, gap_size);
    return done;
}

// Trailing run of adjacent PEs (each starts where the previous page-aligned image ends).
// Returns index of the first PE in that foot block, or (UINT64)-1 if none.
static UINT64 FindFootBlockFirstIndex()
{
    if (uefi_pe_count < 3)
        return (UINT64)-1;

    UINT64 i = uefi_pe_count - 1;
    while (i > 0)
    {
        const UefiPeRegion* prev = &uefi_pe[i - 1];
        const UefiPeRegion* curr = &uefi_pe[i];
        if (curr->phys != prev->phys + prev->size)
            break;
        i--;
    }

    // Foot must be a real block (2+ modules) and sit after the first two head PEs.
    if ((uefi_pe_count - i) < 2 || i < 2)
        return (UINT64)-1;

    return i;
}

void ScanForOutOfModuleUEFI()
{
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    uefi_pe_count = 0;
    uefi_hole_count = 0;

    PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
    pmr += 2;

    // ---- Pass 1: mark PE modules (firmware regions) and remember each hole ----
    do
    {
        if (&pmr[0] && !pmr[0].NumberOfBytes.QuadPart)
            break;
        if (&pmr[1] && !pmr[1].NumberOfBytes.QuadPart)
            break;

        auto high = pmr[1].BaseAddress.QuadPart - 0x1000;
        auto low = pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart + 0x1000;
        auto size = high - low;

        if (size > 0x40000000)
            size = 0x40000000;

        auto rva = (UINT64)MmMapIoSpace(low, size, MmNonCached);
        if (rva)
        {
            UINT64 zero_start = 0;
            UINT32 pe_begin = (UINT32)uefi_pe_count;

            for (UINT64 current = rva; current < rva + size; )
            {
                _mm_invlpg((PVOID)current);

                if (is_zero_page((PVOID)current))
                {
                    if (!zero_start)
                        zero_start = current;
                    current += 4096;
                    continue;
                }

                if (fw_range_count < 31 && zero_start && (current - zero_start) > 0x200000)
                {
                    fw_range[fw_range_count].BaseAddress.QuadPart = (zero_start - rva) + low;
                    fw_range[fw_range_count].NumberOfBytes.QuadPart = current - zero_start;
                    fw_range_count++;
                }
                zero_start = 0;

                UINT32 sizeofimage = TryGetPeSizeOfImage(current);
                if (sizeofimage && uefi_pe_count < 128)
                {
                    UINT64 phys = (current - rva) + low;
                    UINT64 aligned = PageAlignImageSize(sizeofimage);

                    uefi_pe[uefi_pe_count].phys = phys;
                    uefi_pe[uefi_pe_count].size = aligned ? aligned : 4096;
                    uefi_pe_count++;

                    DbgPrintEx(0, 0, "[smm-dtc] Found firmware region at 0x%llx-0x%llx with size 0x%x\n",
                        phys, phys + sizeofimage, sizeofimage);

                    current += aligned ? aligned : 4096;
                    continue;
                }

                current += 4096;
            }

            if (uefi_hole_count < 16)
            {
                uefi_hole[uefi_hole_count].phys = low;
                uefi_hole[uefi_hole_count].size = size;
                uefi_hole[uefi_hole_count].pe_begin = pe_begin;
                uefi_hole[uefi_hole_count].pe_count = (UINT32)(uefi_pe_count - pe_begin);
                uefi_hole_count++;
            }

            MmUnmapIoSpace((PVOID)rva, size);
        }
        pmr++;
    } while (true);

    // ---- Pass 2: gap after 2nd module → first foot module, pages high→low ----
    UINT64 hits = 0;
    if (uefi_pe_count < 3)
    {
        DbgPrintEx(0, 0, "[llvm-kmd]: prefix scan done, hits=0 pes=%llu (need head+foot)\n",
            uefi_pe_count);
        return;
    }

    // Lower bound: end of second (larger) head module — last address of the skip zone.
    const UINT64 gap_lo = uefi_pe[1].phys + uefi_pe[1].size;

    const UINT64 foot_idx = FindFootBlockFirstIndex();
    if (foot_idx == (UINT64)-1)
    {
        DbgPrintEx(0, 0, "[llvm-kmd]: no adjacent foot PE block found\n");
        return;
    }

    // Upper bound: first module of the packed foot cluster (exclusive).
    const UINT64 gap_hi = uefi_pe[foot_idx].phys;

    DbgPrintEx(0, 0,
        "[llvm-kmd]: scanning gap 0x%llx-0x%llx backwards (after pe[1], before foot pe[%llu] @ 0x%llx)\n",
        gap_lo, gap_hi, foot_idx, gap_hi);

    ScanGapBackwards(&decoder, gap_lo, gap_hi, &hits);

    DbgPrintEx(0, 0, "[llvm-kmd]: prefix scan done, hits=%llu pes=%llu foot_first=%llu\n",
        hits, uefi_pe_count, foot_idx);
}

NTSTATUS DriverEntry()
{
    //auto pa = 0x747fdf81;
    //auto rva = (UINT64)MmMapIoSpace(pa, 0xA0, MmNonCached);
    //if (rva)
    //{
    //   
    //    for (int i = 0; i < 0xA0; i+=8)
    //    {
    //        DbgPrintEx(0, 0, "[llvm-kmd] %p : %02x %02x %02x %02x %02x %02x %02x %02x", rva + i, *(UINT8*)(rva + i + 0), *(UINT8*)(rva + i + 1), *(UINT8*)(rva + i + 2), *(UINT8*)(rva + i + 3), *(UINT8*)(rva + i + 4), *(UINT8*)(rva + i + 5), *(UINT8*)(rva + i + 6), *(UINT8*)(rva + i + 7));
    //    }
    //    MmUnmapIoSpace((PVOID)rva, 0xA0);
    //}

    //ScanForOutOfModuleUEFI();

    //TestScanPhysicalForPrefixes(0x79f6c000, 0x8000, 10);

    FWA::Initialize();

	DbgPrintEx(0, 0, "[llvm-kmd]: Hello, World\n");
    return STATUS_SUCCESS;
}