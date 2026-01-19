#include "imports.hpp"

enum FW_STATUS : UINT32
{
	FW_DRIVER_RANGE_EXHAUSTED = 0x80000003,
	FW_MEMORY_MAPPING_FAILED = 0x80000002,
	FW_MEMORY_RANGES_FAILED = 0x80000001,
	FW_GENERIC_FAILURE = 0xFFFFFFFF,
	FW_SUCCESS = 0,
};

class ALIGN(512) FW_MEMORY_MAP
{
private:
	PHYSICAL_MEMORY_RANGE fw_range{ 0 };
	PHYSICAL_MEMORY_RANGE fw_drivers[30];
	UINT32 fw_driver_count;
	FW_STATUS init()
	{
		if (!this)
			return FW_GENERIC_FAILURE;

		RtlFillMemory(this, sizeof(FW_MEMORY_MAP), 0);

		PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
		if (!pmr)
			return FW_MEMORY_RANGES_FAILED;
		pmr += 3;
		do
		{
			if (&pmr[0] && !pmr[0].NumberOfBytes.QuadPart)
				break;
			printf("Searching range: %p - %p\n", 
				pmr[0].BaseAddress.QuadPart, 
				pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart);
			if (&pmr[1] && !pmr[1].NumberOfBytes.QuadPart)
				break;

			auto high = pmr[1].BaseAddress.QuadPart;
			auto low = pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart;
			auto size = high - low;
			printf("%p - %p\n", low, high);
			//auto vad = (UINT64)MmMapIoSpace(low, size, MmNonCached);
			//if (vad)
			//{
			//	for (auto current = vad; current < vad + size; current += 4096)
			//	{
			//		UINT64 hdr_offset = *(BYTE*)(current + 0x3C);
			//		if (*(UINT16*)current == 0x5A4D && // 'MZ' Hdr
			//			*(UINT16*)(current + hdr_offset + 4) == 0x8664)// AMD64
			//		{
			//			auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
			//			sizeofimage = (sizeofimage & ~0xFFF) + (sizeofimage & 0xFFF ? 0x1000 : 0);
			//
			//			fw_range.BaseAddress.QuadPart = low;
			//			fw_range.NumberOfBytes.QuadPart = size;
			//
			//			printf("%i\n", current - vad + low);
			//
			//			fw_drivers[fw_driver_count].BaseAddress.QuadPart = current - vad + low;
			//			fw_drivers[fw_driver_count].NumberOfBytes.QuadPart = sizeofimage;
			//			fw_driver_count++;
			//
			//			if (fw_driver_count == sizeof(fw_drivers) / sizeof(fw_drivers[0]))
			//				break;
			//		}
			//	}
			//	MmUnmapIoSpace((PVOID)vad, size);
			//}
			pmr++;
		} while (!fw_range.BaseAddress.QuadPart);

		return fw_range.BaseAddress.QuadPart ? FW_SUCCESS : FW_GENERIC_FAILURE;
	}

	bool NAKED is_zero_page(PVOID page)
	{
		__asm 
		{
			vpxord zmm0, zmm0, zmm0
			mov eax, 4096 / 64
		loop:
			vporq zmm0, zmm0, [rcx]
			add rcx, 64
			vptestmq k1, zmm0, zmm0
			kortestw k1, k1
			jnz nonzero
			dec eax 
			jnz loop
			mov al, 1
			ret
		nonzero:
			xor eax, eax
			ret
		}
	}
public:
	UINT32 DriverCount()
	{
		if (!this)
			return FW_GENERIC_FAILURE;

		if (!fw_range.BaseAddress.QuadPart)
			init();

		return fw_driver_count;
	}

	PHYSICAL_MEMORY_RANGE* GetDriverRange(UINT32 idx)
	{
		if (!this)
			return nullptr;

		if (!fw_range.BaseAddress.QuadPart)
			init();

		if (idx >= fw_driver_count)
			return nullptr;

		return &fw_drivers[idx];
	}

	PHYSICAL_MEMORY_RANGE* GetFirmwareRange()
	{
		if (!this)
			return nullptr;

		if (!fw_range.BaseAddress.QuadPart)
			init();

		return &fw_range;
	}

	void test()
	{
		if (!this)
			return;

		auto smm_addr = MSR::read_smm_addr();
		auto smm_mask = MSR::read_smm_mask();

		PHYSICAL_MEMORY_RANGE smm_range;
		smm_range.BaseAddress.QuadPart = (QWORD)(smm_addr.TSegBase << 16);
		smm_range.NumberOfBytes.QuadPart = (~smm_mask.TSegMask + 1) << 12;

		PHYSICAL_MEMORY_RANGE* pmr = MmGetPhysicalMemoryRanges();
		DWORD idx = 0;
		do
		{
			if (&pmr[0] && !pmr[0].NumberOfBytes.QuadPart)
				break;
			if (&pmr[1] && !pmr[1].NumberOfBytes.QuadPart)
				break;

			auto high = pmr[1].BaseAddress.QuadPart;
			auto low = pmr[0].BaseAddress.QuadPart + pmr[0].NumberOfBytes.QuadPart;
			auto size = high - low;

			if (low == smm_range.BaseAddress.QuadPart)
				low += smm_range.NumberOfBytes.QuadPart;// skip smram range

			printf("%p - %p\n", low, high);

			auto rva = (UINT64)MmMapIoSpace(low, size, MmNonCached);
			if (rva)
			{
				printf("pages %i\n", size / 4096);
				for (UINT64 current = rva; current < rva + size; current += 4096)
				{
					__invlpg((PVOID)current);
					if (is_zero_page((PVOID)current))
					{
						idx++;
					}
					else
					{
						UINT64 hdr_offset = *(BYTE*)(current + 0x3C);
						if (*(UINT16*)current == 0x5A4D && // 'MZ' Hdr
							*(UINT16*)(current + hdr_offset + 4) == 0x8664)// AMD64
						{
							auto sizeofimage = *(UINT32*)(current + hdr_offset + 0x50);
							sizeofimage = (sizeofimage & ~0xFFF) + (sizeofimage & 0xFFF ? 0x1000 : 0);
							printf("found driver at %p - %p\n", 
								(current - rva) + low, 
								(current - rva) + low + sizeofimage);
						}
					}

				}
				MmUnmapIoSpace((PVOID)rva, size);
			}
			pmr++;
		} while (true);

		printf("zero pages: %i\n", idx);

		return;
	}
};

UCHAR NAKED is_zero_page2(PVOID page)
{
	__asm 
	{
		vpxord zmm0, zmm0, zmm0
		mov eax, 4096 / 64
	loop:
		vporq zmm0, zmm0, [rcx]
		add rcx, 64
		vptestmq k1, zmm0, zmm0
		kortestw k1, k1
		jnz nonzero
		dec eax 
		jnz loop
		mov al, 1
		ret
	nonzero:
		xor eax, eax
		ret
	}
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	FW_MEMORY_MAP fwMm;
	
	fwMm.test();

	return STATUS_SUCCESS;
}