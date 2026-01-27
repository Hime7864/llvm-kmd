#pragma once
// Table 15-8. MSR Ranges Covered by MSRPM (40332-r4.00.pdf)
struct MSRPM
{
    UCHAR Legacy[0x800];      //0x0000–0x07FF | [0x0000_0000 – 0x0000_1FFF]
    UCHAR Amd_6th_Gen[0x800]; //0x0800–0x0FFF | [0xC000_0000 – 0xC000_1FFF]
    UCHAR Amd_Modern[0x800];  //0x1000–0x17FF | [0xC001_0000 – 0xC001_1FFF]
    UCHAR Reserved[0x800];    //0x1800–0x1FFF | Reserved

private:
    void set_bit(DWORD msr, BOOLEAN read, BOOLEAN set)
    {
        DWORD range = (msr >> 16) & 0xFFFF;
        DWORD offset = msr & 0x1FFF;
        DWORD bitpos = offset << 1;
        DWORD byte_index = bitpos >> 3;
        UCHAR bit_mask = (UCHAR)(1 << (bitpos & 7));

        UCHAR* array;

        switch (range)
        {
        case 0x0000: array = Legacy;        break;
        case 0xC000: array = Amd_6th_Gen;   break;
        case 0xC001: array = Amd_Modern;    break;
        default:     return;
        }

        if (byte_index >= 0x800) return;

        if (read)
        {
            if (set)
                array[byte_index] |= bit_mask;
            else
                array[byte_index] &= ~bit_mask;
        }
        else
        {
            bit_mask <<= 1;
            if (bit_mask == 0)
            {
                bit_mask = 1;
                byte_index++;
                if (byte_index >= 0x800) return;
            }

            if (set)
                array[byte_index] |= bit_mask;
            else
                array[byte_index] &= ~bit_mask;
        }
    }
public:
    void read(DWORD msr, BOOLEAN intercept) { set_bit(msr, TRUE, intercept); }

    void write(DWORD msr, BOOLEAN intercept) { set_bit(msr, FALSE, intercept); }
};