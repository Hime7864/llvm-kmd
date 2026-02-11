#pragma once

enum VMEXIT_CODE : UINT64
{
    VMEXIT_CR0_READ = 0x0000,
    VMEXIT_CR1_READ = 0x0001,
    VMEXIT_CR2_READ = 0x0002,
    VMEXIT_CR3_READ = 0x0003,
    VMEXIT_CR4_READ = 0x0004,
    VMEXIT_CR5_READ = 0x0005,
    VMEXIT_CR6_READ = 0x0006,
    VMEXIT_CR7_READ = 0x0007,
    VMEXIT_CR8_READ = 0x0008,
    VMEXIT_CR9_READ = 0x0009,
    VMEXIT_CR10_READ = 0x000A,
    VMEXIT_CR11_READ = 0x000B,
    VMEXIT_CR12_READ = 0x000C,
    VMEXIT_CR13_READ = 0x000D,
    VMEXIT_CR14_READ = 0x000E,
    VMEXIT_CR15_READ = 0x000F,

    VMEXIT_CR0_WRITE = 0x0010,
    VMEXIT_CR1_WRITE = 0x0011,
    VMEXIT_CR2_WRITE = 0x0012,
    VMEXIT_CR3_WRITE = 0x0013,
    VMEXIT_CR4_WRITE = 0x0014,
    VMEXIT_CR5_WRITE = 0x0015,
    VMEXIT_CR6_WRITE = 0x0016,
    VMEXIT_CR7_WRITE = 0x0017,
    VMEXIT_CR8_WRITE = 0x0018,
    VMEXIT_CR9_WRITE = 0x0019,
    VMEXIT_CR10_WRITE = 0x001A,
    VMEXIT_CR11_WRITE = 0x001B,
    VMEXIT_CR12_WRITE = 0x001C,
    VMEXIT_CR13_WRITE = 0x001D,
    VMEXIT_CR14_WRITE = 0x001E,
    VMEXIT_CR15_WRITE = 0x001F,

    VMEXIT_DR0_READ = 0x0020,
    VMEXIT_DR1_READ = 0x0021,
    VMEXIT_DR2_READ = 0x0022,
    VMEXIT_DR3_READ = 0x0023,
    VMEXIT_DR4_READ = 0x0024,
    VMEXIT_DR5_READ = 0x0025,
    VMEXIT_DR6_READ = 0x0026,
    VMEXIT_DR7_READ = 0x0027,
    VMEXIT_DR8_READ = 0x0028,
    VMEXIT_DR9_READ = 0x0029,
    VMEXIT_DR10_READ = 0x002A,
    VMEXIT_DR11_READ = 0x002B,
    VMEXIT_DR12_READ = 0x002C,
    VMEXIT_DR13_READ = 0x002D,
    VMEXIT_DR14_READ = 0x002E,
    VMEXIT_DR15_READ = 0x002F,

    VMEXIT_DR0_WRITE = 0x0030,
    VMEXIT_DR1_WRITE = 0x0031,
    VMEXIT_DR2_WRITE = 0x0032,
    VMEXIT_DR3_WRITE = 0x0033,
    VMEXIT_DR4_WRITE = 0x0034,
    VMEXIT_DR5_WRITE = 0x0035,
    VMEXIT_DR6_WRITE = 0x0036,
    VMEXIT_DR7_WRITE = 0x0037,
    VMEXIT_DR8_WRITE = 0x0038,
    VMEXIT_DR9_WRITE = 0x0039,
    VMEXIT_DR10_WRITE = 0x003A,
    VMEXIT_DR11_WRITE = 0x003B,
    VMEXIT_DR12_WRITE = 0x003C,
    VMEXIT_DR13_WRITE = 0x003D,
    VMEXIT_DR14_WRITE = 0x003E,
    VMEXIT_DR15_WRITE = 0x003F,

    VMEXIT_EXCEPTION_DE = 0x0040,  // #DE
    VMEXIT_EXCEPTION_DB = 0x0041,  // #DB
    VMEXIT_EXCEPTION_NMI = 0x0042, // NMI (internal)
    VMEXIT_EXCEPTION_BP = 0x0043,  // #BP
    VMEXIT_EXCEPTION_OF = 0x0044,  // #OF
    VMEXIT_EXCEPTION_BR = 0x0045,  // #BR
    VMEXIT_EXCEPTION_UD = 0x0046,  // #UD
    VMEXIT_EXCEPTION_NM = 0x0047,  // #NM
    VMEXIT_EXCEPTION_DF = 0x0048,  // #DF
    VMEXIT_EXCEPTION_TS = 0x004A,  // #TS
    VMEXIT_EXCEPTION_NP = 0x004B,  // #NP
    VMEXIT_EXCEPTION_SS = 0x004C,  // #SS
    VMEXIT_EXCEPTION_GP = 0x004D,  // #GP
    VMEXIT_EXCEPTION_PF = 0x004E,  // #PF
    VMEXIT_EXCEPTION_MF = 0x0050,  // #MF
    VMEXIT_EXCEPTION_AC = 0x0051,  // #AC
    VMEXIT_EXCEPTION_MC = 0x0052,  // #MC
    VMEXIT_EXCEPTION_XF = 0x0053,  // #XF

    VMEXIT_INTR = 0x0060,
    VMEXIT_NMI = 0x0061,
    VMEXIT_SMI = 0x0062,
    VMEXIT_INIT = 0x0063,
    VMEXIT_VINTR = 0x0064,
    VMEXIT_CR0_SEL_WRITE = 0x0065, 
    VMEXIT_IDTR_READ = 0x0066,
    VMEXIT_GDTR_READ = 0x0067,
    VMEXIT_LDTR_READ = 0x0068,
    VMEXIT_TR_READ = 0x0069,
    VMEXIT_IDTR_WRITE = 0x006A,
    VMEXIT_GDTR_WRITE = 0x006B,
    VMEXIT_LDTR_WRITE = 0x006C,
    VMEXIT_TR_WRITE = 0x006D,

    VMEXIT_RDTSC = 0x006E,
    VMEXIT_RDPMC = 0x006F,
    VMEXIT_PUSHF = 0x0070,
    VMEXIT_POPF = 0x0071,
    VMEXIT_CPUID = 0x0072,
    VMEXIT_RSM = 0x0073,
    VMEXIT_IRET = 0x0074,
    VMEXIT_SWINT = 0x0075,
    VMEXIT_INVD = 0x0076,
    VMEXIT_PAUSE = 0x0077,
    VMEXIT_HLT = 0x0078,
    VMEXIT_INVLPG = 0x0079,
    VMEXIT_INVLPGA = 0x007A,
    VMEXIT_IOIO = 0x007B,
    VMEXIT_MSR = 0x007C,
    VMEXIT_TASK_SWITCH = 0x007D,
    VMEXIT_FERR_FREEZE = 0x007E,
    VMEXIT_SHUTDOWN = 0x007F,

    VMEXIT_VMRUN = 0x0080,
    VMEXIT_VMMCALL = 0x0081,
    VMEXIT_VMLOAD = 0x0082,
    VMEXIT_VMSAVE = 0x0083,
    VMEXIT_STGI = 0x0084,
    VMEXIT_CLGI = 0x0085,
    VMEXIT_SKINIT = 0x0086,
    VMEXIT_RDTSCP = 0x0087,
    VMEXIT_ICEBP = 0x0088,
    VMEXIT_WBINVD = 0x0089,
    VMEXIT_MONITOR = 0x008A,
    VMEXIT_MWAIT = 0x008B,
    VMEXIT_MWAIT_CONDITIONAL = 0x008C,
    VMEXIT_XSETBV = 0x008D,
    VMEXIT_RDPRU = 0x008E,
    VMEXIT_EFER_WRITE_TRAP = 0x008F,

    VMEXIT_CR0_WRITE_TRAP = 0x0090,
    VMEXIT_CR1_WRITE_TRAP = 0x0091,
    VMEXIT_CR2_WRITE_TRAP = 0x0092,
    VMEXIT_CR3_WRITE_TRAP = 0x0093,
    VMEXIT_CR4_WRITE_TRAP = 0x0094,
    VMEXIT_CR5_WRITE_TRAP = 0x0095,
    VMEXIT_CR6_WRITE_TRAP = 0x0096,
    VMEXIT_CR7_WRITE_TRAP = 0x0097,
    VMEXIT_CR8_WRITE_TRAP = 0x0098,
    VMEXIT_CR9_WRITE_TRAP = 0x0099,
    VMEXIT_CR10_WRITE_TRAP = 0x009A,
    VMEXIT_CR11_WRITE_TRAP = 0x009B,
    VMEXIT_CR12_WRITE_TRAP = 0x009C,
    VMEXIT_CR13_WRITE_TRAP = 0x009D,
    VMEXIT_CR14_WRITE_TRAP = 0x009E,
    VMEXIT_CR15_WRITE_TRAP = 0x009F,

    VMEXIT_MCOMMIT = 0x00A3,

    VMEXIT_NPF = 0x0400,
    VMEXIT_AVIC_INCOMPLETE_IPI = 0x0401,
    VMEXIT_AVIC_NOACCEL = 0x0402,
    VMEXIT_VMGEXIT = 0x0403,

    VMEXIT_INVALID_GUEST_STATE = -1ULL,
    VMEXIT_INVALID = -2ULL,
};

enum EXITINTINFO_TYPE : UINT64 {
    SVM_EXITINTINFO_TYPE_HW_EXCEPTION = 0,  // #DE, #DB, #GP, #PF, etc.
    SVM_EXITINTINFO_TYPE_SW_INTERRUPT = 1,  // INTn, INT3, INTO
    SVM_EXITINTINFO_TYPE_NMI = 2,  // NMI
    SVM_EXITINTINFO_TYPE_SW_EXCEPTION = 3   // INT3, ICEBP, BOUND, INTO
};

struct SEGMENT
{
    USHORT selector; // 0x00
    USHORT attrib; // 0x02
    UINT32 limit; // 0x04
    UINT64 base; // 0x08
};

// Table B-2. VMCB Layout, Control Area (40332-r4.00.pdf)
struct ALIGN(1024) CONTROL_AREA
{
    struct {
        unsigned rCR0 : 1;
        unsigned rCR1 : 1;
        unsigned rCR2 : 1;
        unsigned rCR3 : 1;
        unsigned rCR4 : 1;
        unsigned rCR5 : 1;
        unsigned rCR6 : 1;
        unsigned rCR7 : 1;
        unsigned rCR8 : 1;
        unsigned rCR9 : 1;
        unsigned rCR10 : 1;
        unsigned rCR11 : 1;
        unsigned rCR12 : 1;
        unsigned rCR13 : 1;
        unsigned rCR14 : 1;
        unsigned rCR15 : 1;
        unsigned wCR0 : 1;
        unsigned wCR1 : 1;
        unsigned wCR2 : 1;
        unsigned wCR3 : 1;
        unsigned wCR4 : 1;
        unsigned wCR5 : 1;
        unsigned wCR6 : 1;
        unsigned wCR7 : 1;
        unsigned wCR8 : 1;
        unsigned wCR9 : 1;
        unsigned wCR10 : 1;
        unsigned wCR11 : 1;
        unsigned wCR12 : 1;
        unsigned wCR13 : 1;
        unsigned wCR14 : 1;
        unsigned wCR15 : 1;
        unsigned rDR0 : 1;
        unsigned rDR1 : 1;
        unsigned rDR2 : 1;
        unsigned rDR3 : 1;
        unsigned rDR4 : 1;
        unsigned rDR5 : 1;
        unsigned rDR6 : 1;
        unsigned rDR7 : 1;
        unsigned rDR8 : 1;
        unsigned rDR9 : 1;
        unsigned rDR10 : 1;
        unsigned rDR11 : 1;
        unsigned rDR12 : 1;
        unsigned rDR13 : 1;
        unsigned rDR14 : 1;
        unsigned rDR15 : 1;
        unsigned wDR0 : 1;
        unsigned wDR1 : 1;
        unsigned wDR2 : 1;
        unsigned wDR3 : 1;
        unsigned wDR4 : 1;
        unsigned wDR5 : 1;
        unsigned wDR6 : 1;
        unsigned wDR7 : 1;
        unsigned wDR8 : 1;
        unsigned wDR9 : 1;
        unsigned wDR10 : 1;
        unsigned wDR11 : 1;
        unsigned wDR12 : 1;
        unsigned wDR13 : 1;
        unsigned wDR14 : 1;
        unsigned wDR15 : 1;
        unsigned DivideError : 1;  // #DE   vector  0
        unsigned Debug : 1;  // #DB   vector  1
        unsigned : 1;  // #NMI  vector  2 (not implemented)
        unsigned Breakpoint : 1;  // #BP   vector  3
        unsigned Overflow : 1;  // #OF   vector  4
        unsigned BoundRange : 1;  // #BR   vector  5
        unsigned InvalidOpcode : 1;  // #UD   vector  6
        unsigned DeviceNotAvailable : 1;  // #NM   vector  7
        unsigned DoubleFault : 1;  // #DF   vector  8
        unsigned : 1;  //       vector  9 (reserved)
        unsigned InvalidTSS : 1;  // #TS   vector 10
        unsigned SegmentNotPresent : 1;  // #NP   vector 11
        unsigned Stack : 1;  // #SS   vector 12
        unsigned GeneralProtection : 1;  // #GP   vector 13
        unsigned PageFault : 1;  // #PF   vector 14
        unsigned : 1;  //       vector 15 (reserved)
        unsigned FloatingPointError : 1;  // #MF   vector 16
        unsigned AlignmentCheck : 1;  // #AC   vector 17
        unsigned MachineCheck : 1;  // #MC   vector 18
        unsigned SimdException : 1;  // #XF   vector 19
        unsigned : 12; //       vectors 20–31 (reserved)
        unsigned INTR : 1;
        unsigned NMI : 1;
        unsigned SMI : 1;
        unsigned INIT : 1;
        unsigned VINTR : 1;
        unsigned CR0_Selective : 1;
        unsigned rIDTR : 1;
        unsigned rGDTR : 1;
        unsigned rLDTR : 1;
        unsigned rTR : 1;
        unsigned wIDTR : 1;
        unsigned wGDTR : 1;
        unsigned wLDTR : 1;
        unsigned wTR : 1;
        unsigned RDTSC : 1;
        unsigned RDPMC : 1;
        unsigned PUSHF : 1;
        unsigned POPF : 1;
        unsigned CPUID : 1;
        unsigned RSM : 1;
        unsigned IRET : 1;
        unsigned INTn : 1;
        unsigned INVD : 1;
        unsigned PAUSE : 1;
        unsigned HLT : 1;
        unsigned INVLPG : 1;
        unsigned INVLPGA : 1;
        unsigned IOIO_Prot : 1;
        unsigned MSR_Prot : 1;
        unsigned TaskSwitch : 1;
        unsigned FerrFreeze : 1;
        unsigned Shutdown : 1;
        unsigned VMRUN : 1;
        unsigned VMMCALL : 1;
        unsigned VMLOAD : 1;
        unsigned VMSAVE : 1;
        unsigned STGI : 1;
        unsigned CLGI : 1;
        unsigned SKINIT : 1;
        unsigned RDTSCP : 1;
        unsigned ICEBP : 1;
        unsigned WBINVD : 1;
        unsigned MONITOR : 1;
        unsigned MWAIT_Unconditional : 1;
        unsigned MWAIT_Conditional : 1;
        unsigned XSETBV : 1;
        unsigned RDPRU : 1;
        unsigned wEFER : 1;
        unsigned wCR0_Post : 1;
        unsigned wCR1_Post : 1;
        unsigned wCR2_Post : 1;
        unsigned wCR3_Post : 1;
        unsigned wCR4_Post : 1;
        unsigned wCR5_Post : 1;
        unsigned wCR6_Post : 1;
        unsigned wCR7_Post : 1;
        unsigned wCR8_Post : 1;
        unsigned wCR9_Post : 1;
        unsigned wCR10_Post : 1;
        unsigned wCR11_Post : 1;
        unsigned wCR12_Post : 1;
        unsigned wCR13_Post : 1;
        unsigned wCR14_Post : 1;
        unsigned wCR15_Post : 1;
        unsigned : 3;
        unsigned MCOMMIT : 1;
        unsigned : 28;
    }Intercept; // 0x0
	struct 
    { 
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
    }; // 0x018
    UINT16 PauseFilterThreshold; // 0x03C
    UINT16 PauseFilterCount; // 0x03E
    PHYSICAL_ADDRESS IoPmBasePa; // 0x040
    PHYSICAL_ADDRESS MsrPmBasePa; // 0x048
    UINT64 TscOffset; // 0x050
    union {
        UINT64 GuestASID : 32;
        UINT64 DoNothing : 1;
        UINT64 FlushEntireTLB : 1;
        UINT64 FlushGuestTLB : 3;
        UINT64 FlushGuestNonGlobalTLB : 5;
        UINT64 : 22;
    } TlbControl; // 0x058
    union {
        UINT64 V_TPR : 8;
        UINT64 V_IRQ : 1;
        UINT64 V_GIF : 1;
        UINT64 : 6;
        UINT64 V_INTR_PRIO : 4;
        UINT64 V_IGN_TPR : 1;
        UINT64 : 3;
        UINT64 V_INTR_MASKING : 1;
        UINT64 V_GIF_ENABLE : 1;
        UINT64 : 5;
        UINT64 AVIC_ENABLED : 1;
        UINT64 V_INTR_VECTOR : 8;
        UINT64 : 24;
    } VirtualApic; // 0x060
    union {
        UINT64 InterruptShadow : 1;
        UINT64 GuestInterruptMask : 1;
        UINT64 : 62;
    } GuestInterruptState; // 0x068
    VMEXIT_CODE ExitCode; // 0x070
    union {
        // Figure 15-2. EXITINFO1 for IOIO Intercep
        struct {
            UINT64 TYPE : 1;  // Access Type (0 = OUT instruction, 1 = IN instruction
            UINT64 : 1;  // Reserved
            UINT64 STR : 1; // String based port access (INS, OUTS)
            UINT64 REP : 1; // Repeated port access
            UINT64 SZ8 : 1; // 8-bit operand size
            UINT64 SZ16 : 1; // 16-bit operand size
            UINT64 SZ32 : 1; // 32-bit operand size
            UINT64 A16 : 1; // 16-bit address
            UINT64 A32 : 1; // 32-bit address
            UINT64 A64 : 1; // 64-bit address
            UINT64 SEG : 3; // Effective segment number
            UINT64 : 3; // Reserved
            UINT64 PORT : 16; // I/O port number
        } IOIO;
    
        //Table 15-4. EXITINFO1 for INTn
        struct {
            UINT64 Vector : 8;   // Interrupt vector (0–255)
            UINT64 Reserved : 56;   // Must be zero
        } INTn;
    
        //Table 15-5. EXITINFO1 for INVLPG
        struct {
            UINT64 LinearAddress;
        } INVLPG;
    
        //Table 15-2. EXITINFO1 for MOV CRx
        struct {
            UINT64 GPRn : 4;   // General-purpose register number (0–15)
            UINT64 : 59; // Must be zero
            UINT64 toOrFrom : 1;// MOV-to-CR == 0 || MOV-from-CR == 1
        } MOVCRX;
    
        //Table 15-3. EXITINFO1 for MOV DRx
        struct {
            UINT64 GPRn : 4;   // General-purpose register number (0–15)
            UINT64 : 60; // Must be zero
        } MOVDRX;
    
        // AVIC-related #VMEXIT events
        struct {
            UINT64 ICRL : 32;
            UINT64 ICRH : 32;
        } AVIC;
    
        //15.12.17 #AC (Alignment Check)
        struct {
            UINT64 ErrorCode;
        } AC;
    
        // 15.11 MSR Intercepts
        struct {
            UINT64 isWrite;
        } MSR;
    
        //15.25.6 Nested versus Guest Page Faults, Fault Ordering
        struct {
            //Bit 0 (P)—cleared to 0 if the nested page was not present, 1 otherwise
            UINT64 P : 1;
            //Bit 1 (RW)—set to 1 if the nested page table level access was a write. Note that host table walks for
            //guest page tables are always treated as data writes
            UINT64 RW : 1;
            //Bit 2 (US)—set to 1 if the nested page table level access was a user access. Note that nested page
            //table accesses performed by the MMU are treated as user accesses unless there are features
            //enabled that override this.
            UINT64 US : 1;
            //Bit 3 (RSV)—set to 1 if reserved bits were set in the corresponding nested page table entry
            UINT64 RSV : 1;
            //Bit 4 (ID)—set to 1 if the nested page table level access was a code read. Note that nested table
            //walks for guest page tables are always treated as data writes, even if the access itself is a code read
            UINT64 ID : 1;
            UINT64 : 27;
            //Bit 32—set to 1 if nested page fault occurred while translating the guest’s final physical address
            UINT64 NPF : 1;
            //Bit 33—set to 1 if nested page fault occurred while translating the guest page tables
            UINT64 GPF : 1;
            //Bit 34 (ENC): Set to 1 if the guest’s effective C-bit was 1, 0 otherwise.
            UINT64 ENC : 1;
            //Bit 35 (SIZEM): Set to 1 if the fault was caused by a size mismatch between PVALIDATE or
            //RMPADJUST and the RMP, 0 otherwise.
            UINT64 SIZEM : 1;
            // Bit 36 (VMPL): Set to 1 if the fault was caused by a VMPL permission check failure, 0 otherwise.
            UINT64 VMPL : 1;
            UINT64 : 27;
        } PF;
    } ExitInfo1; // 0x78
    union {
        // Nested Page Fault error code (SVM_VMEXIT_NPF)
        struct {
            UINT64 ErrorCodeLow : 32;  // Standard PF bits (P, W, U, RSVD, I)
            UINT64 ErrorCodeHigh : 32;  // Extended (PK, SGX, RMP for SEV-SNP)
        } Npf;
    
        // Exception error codes (GP, PF, SS, etc.)
        struct {
            UINT64 ErrorCode : 32;
            UINT64 Reserved : 32;
        } Exception;
    
        // MSR data (SVM_VMEXIT_MSR)
        struct {
            UINT64 MsrData : 64;  // Full 64-bit MSR value
        } Msr;
    
        // CR/DR access value
        struct {
            UINT64 RegisterValue : 64;  // Value read/written
        } RegisterAccess;
    
        // AVIC vector (SVM_VMEXIT_AVIC_*)
        struct {
            UINT64 Vector : 8;
            UINT64 Reserved : 56;
        } Avic;
    
        // IOIO data (rarely used)
        struct {
            UINT64 IoData : 64;  // Transferred data
        } Io;
    
    } ExitInfo2; // 0x80
    union {
        UINT64 Vector : 8;   // Exception/interrupt vector (0–255)
        EXITINTINFO_TYPE Type : 3;   // Delivery type
        UINT64 ErrorCodeValid : 1;   // 1 = Error code pushed (and valid)
        UINT64 Reserved1 : 19;  // Must be zero
        UINT64 Valid : 1;   // 1 = This EXITINTINFO is valid
        UINT64 Reserved2 : 32;  // Must be zero
    } ExitInfoIntercept; // 0x88
    union {
        UINT64 NP_Enable : 1;
        UINT64 SEV_Enable : 1;
        UINT64 SEV_ES_Enable : 1;
        UINT64 GME_Trap : 1;
        UINT64 : 1;
        UINT64 VTE : 1;
        UINT64 : 58;
    } NestedPagingControl; // 0x090
    PHYSICAL_ADDRESS AvicApicBar; // 0x098
    PHYSICAL_ADDRESS GhcbGpa; // 0x0A0
    union {
        UINT64 VECTOR : 8;
        UINT64 TYPE : 3;
        UINT64 EV : 1;
        UINT64 : 19;
        UINT64 V : 1;
        UINT64 ERRORCODE : 32;
    } EventInjection; // 0x0A8;
    PHYSICAL_ADDRESS NestedCr3; // 0x0B0
    union {
        UINT64 LBR_VirtEnable : 1;
        UINT64 VMSAVE_VMLOAD_Virt : 1;
        UINT64 : 62;
    } VirtualizationFeatures; // 0x0B8
    UINT64 VmcbCleanBits; // 0x0C0
    UINT64 NextRip; // 0x0C8
    union {
        INT128 BytesFetched : 8;
        INT128 InstructionBytes : 120;
    }GuestInstructionBytes; // 0x0D0
    PHYSICAL_ADDRESS AvicBackingPage; // 0x0E0
	struct 
    { 
        unsigned : 32;
        unsigned : 32;
    }; // 0x0E8
    PHYSICAL_ADDRESS AvicLogicalTable; // 0x0F0
    PHYSICAL_ADDRESS AvicPhysicalTable; // 0x0F8
    struct
    {
        unsigned : 32;
        unsigned : 32;
    };// 0x100
    PHYSICAL_ADDRESS VmsaPointer; // 0x108
};

// Table B-2. VMCB Layout, State Save Area (40332-r4.00.pdf)
struct PACKED SAVE_STATE_AREA// Table B-2. VMCB Layout, State Save Area (40332-r4.00.pdf)
{
    SEGMENT ES; // 0x000
    SEGMENT CS; // 0x010
    SEGMENT SS; // 0x020
    SEGMENT DS; // 0x030
    SEGMENT FS; // 0x040
    SEGMENT GS; // 0x050
    SEGMENT GDTR; // 0x060
    SEGMENT LDTR; // 0x070
    SEGMENT IDTR; // 0x080
    SEGMENT TR; // 0x090
    struct
    { 
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
    };
    UCHAR Reserved[3];
    UCHAR CPL; // 0x0CB
    struct { unsigned : 32; };
    UINT64 EFER; // 0x0D0
    struct 
    { 
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
    };
    UINT64 CR4;       // 0x148
    UINT64 CR3;       // 0x150
    UINT64 CR0;       // 0x158
    UINT64 DR7;       // 0x160
    UINT64 DR6;       // 0x168
    UINT64 RFLAGS;    // 0x170
    UINT64 Rip;       // 0x178
    struct
    {
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
		unsigned : 32;
        unsigned : 32;
    };
    UINT64 RSP;       // 0x1D8
    struct 
    { 
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
    };
    UINT64 Rax;       // 0x1F8
    UINT64 STAR;      // 0x200
    UINT64 LSTAR;     // 0x208
    UINT64 CSTAR;     // 0x210
    UINT64 SFMASK;    // 0x218
    UINT64 KernelGsBase; // 0x220
    UINT64 SYSENTER_CS; // 0x228
    UINT64 SYSENTER_ESP; // 0x230
    UINT64 SYSENTER_EIP; // 0x238
    UINT64 CR2;       // 0x240
    struct 
    { 
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
        unsigned : 32;
    };
    UINT64 G_PAT;      // 0x268
    UINT64 DBGCTL;    // 0x270
    UINT64 BR_FROM;   // 0x278
    UINT64 BR_TO;     // 0x280
    UINT64 LASTEXCPFROM; // 0x288
    UINT64 LASTEXCPTO;   // 0x290
};

struct ALIGN(4096) VMCB
{
    CONTROL_AREA ControlArea; // 0x000
    SAVE_STATE_AREA SaveStateArea; // 0x400
};