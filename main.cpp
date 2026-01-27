#include <intrinsics.hpp>

class PACKED MEMORY_MANAGER
{
private:
	UINT64 mmStackBase;//0x0
	UINT64 oldStackBase;//0x8
	PHYSICAL_MEMORY_RANGE stack_frame;//0x10
	PHYSICAL_ADDRESS mmCr3;//0x20
	PHYSICAL_ADDRESS oldCr3;//0x28

	UINT64 HostedGetVirtual(PHYSICAL_ADDRESS address)
	{
		auto va = MmMapIoSpace(address, 0x1000, MmNonCached);
		__invlpg(va);
		return (UINT64)va;
	}

	void HostedFreeVirtual(UINT64 address)
	{
		return MmUnmapIoSpace((PVOID)address, 0x1000);
	}

	MMPTE_HARDWARE HostedValidateTranslation(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva)
	{
		DWORD idx[4]{
			(DWORD)rva.pml4e_index,
			(DWORD)rva.pdpte_index,
			(DWORD)rva.pde_index,
			(DWORD)rva.pte_index
		};

		auto page_map = (MMPTE_HARDWARE*)HostedGetVirtual(dtb);
		for (int i = 0; i < 4; i++)
		{
			auto entry = &page_map[idx[i]];
			if (!entry->Valid)
			{
				HostedFreeVirtual((UINT64)page_map);
				return { 0 };
			}
			if (i == 3)
			{
				auto result = *entry;
				HostedFreeVirtual((UINT64)page_map);
				return result;
			}
			auto next_page = entry->PageFrameNumber << 12;
			HostedFreeVirtual((UINT64)page_map);
			page_map = (MMPTE_HARDWARE*)HostedGetVirtual(next_page);
		}
		return { 0 };
	}

	BOOLEAN HostedCommit4kbMapping(PHYSICAL_ADDRESS dtb, LINEAR_ADDRESS rva, MMPTE_HARDWARE pte)
	{
		DWORD idx[4]{
			(DWORD)rva.pml4e_index,
			(DWORD)rva.pdpte_index,
			(DWORD)rva.pde_index,
			(DWORD)rva.pte_index
		};

		auto page_map = (MMPTE_HARDWARE*)HostedGetVirtual(dtb);

		for (int i = 0; i < 4; i++)
		{
			auto entry = &page_map[idx[i]];
			if (i == 3)
			{
				entry->AsUINT64 = pte.AsUINT64;
				HostedFreeVirtual((UINT64)page_map);
				break;
			}
			if (!entry->Valid)
			{
				auto new_page = FWA::ReservePages(1);
				entry->AsUINT64 = 0;
				entry->Valid = true;
				entry->Dirty1 = true;
				entry->PageFrameNumber = new_page >> 12;
				HostedFreeVirtual((UINT64)page_map);
				page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
			}
			else
			{
				auto new_page = entry->PageFrameNumber << 12;
				HostedFreeVirtual((UINT64)page_map);
				page_map = (MMPTE_HARDWARE*)HostedGetVirtual(new_page);
			}
		}
		return TRUE;
	}

	static void NAKED UpdatePoison(PVOID self)
	{
		__asm {
			push rbx
			push rdx

			mov rdx, [rcx + 8]
			mov rcx, [rcx]

			mov rax, [rcx + 0x20]
			mov cr3, rax

			invlpg[rcx]
			invlpg[rdx]

			mov rbx, [rdx]
			xor eax, eax
			mov[rdx], rax
			mov[rdx], rbx

			mov rax, [rcx + 0x28]
			mov cr3, rax

			pop rdx
			pop rbx
			ret
		}
	}

public:
	BOOLEAN Initialize()
	{
		FWA::Initialize();

		UINT64 module_base, module_size;
		Utils::LocateSelf(&module_base, &module_size);

		mmCr3 = FWA::ReservePages(1);

		stack_frame.BaseAddress.QuadPart = FWA::ReservePages(10);
		stack_frame.NumberOfBytes.QuadPart = 0xA000;

		mmStackBase = module_base + module_size;
		oldCr3 = __readcr3();

		for (int i = 0; i < stack_frame.NumberOfBytes.QuadPart >> 12; i++)
		{
			MMPTE_HARDWARE pte;
			pte.AsUINT64 = 0;
			pte.Valid = true;
			pte.Dirty1 = true;
			pte.Write = true;
			pte.Dirty = true;
			pte.NoExecute = true;
			pte.PageFrameNumber = (stack_frame.BaseAddress.QuadPart >> 12) + i;
			HostedCommit4kbMapping(mmCr3, mmStackBase + (i << 12), pte);
		}

		for (int i = 0; i < module_size >> 12; i++)
		{
			auto new_page = FWA::ReservePages(1);
			MMPTE_HARDWARE pte;
			pte.AsUINT64 = 0;
			pte.Valid = true;
			pte.Dirty1 = true;
			pte.Write = true;
			pte.Dirty = true;
			pte.PageFrameNumber = new_page >> 12;
			HostedCommit4kbMapping(mmCr3, module_base + (i << 12), pte);
			auto page_va = HostedGetVirtual(new_page);
			RtlCopyMemory((PVOID)page_va, (PVOID)(module_base + (i << 12)), 4096);
			HostedFreeVirtual(page_va);
		}

		return TRUE;
	}

	PHYSICAL_ADDRESS GetFirmwarePage()
	{

		return FWA::ReservePages(1);
	}

	void ReserveRvaPoison(LINEAR_ADDRESS rva, PHYSICAL_ADDRESS PoisonedPage)
	{
		MMPTE_HARDWARE pte;
		pte.AsUINT64 = 0;
		pte.Valid = true;
		pte.Owner = true;
		pte.Global = true;
		pte.Dirty1 = true;
		pte.Write = true;
		pte.PageFrameNumber = PoisonedPage >> 12;

		HostedCommit4kbMapping(mmCr3, rva, pte);
		return;
	}

	void CommitRvaPoison(LINEAR_ADDRESS rva)
	{
		struct IPI_DATA
		{
			PVOID self;
			UINT64 target_address;
		};
		IPI_DATA data;

		data.self = this;
		data.target_address = rva.AsUINT64;
		auto irql = __readcr8();
		__writecr8(15);
		UpdatePoison(&data);
		__writecr8(irql);
		return;
	}

	void Cleanup()
	{
		FWA::Cleanup();
	}
}fwMm;

void NAKED cpy_page(PVOID dest, PVOID source)
{
	__asm
	{
		mov eax, 4096 / 64
		loop:
		vmovdqu64 zmm0, [rdx]
			vmovdqu64[rcx], zmm0
			add rdx, 64
			add rcx, 64
			dec eax
			jnz loop
			ret
	}
}

UCHAR page_buffer[4096]{ 0 };

UCHAR b2c(UCHAR val)
{
	if (val == 0x20)
		return val;
	if (val >= '0' && val <= '9')
		return val;
	if (val >= 'A' && val <= 'Z')
		return val;
	if (val >= 'a' && val <= 'z')
		return val;
	return '.';
}

void dmp_bytes(PVOID buffer, int bytes)
{
	for (int i = 0; i < bytes / 16; i++)
	{
		auto buf = (BYTE*)buffer + i * 16;
		printf("%04X : %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x : %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
			i * 16,
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
			buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15],
			b2c(buf[0]), b2c(buf[1]), b2c(buf[2]), b2c(buf[3]), b2c(buf[4]), b2c(buf[5]), b2c(buf[6]), b2c(buf[7]),
			b2c(buf[8]), b2c(buf[9]), b2c(buf[10]), b2c(buf[11]), b2c(buf[12]), b2c(buf[13]), b2c(buf[14]), b2c(buf[15])
		);
	}
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	fwMm.Initialize();

	fwMm.ReserveRvaPoison(67ull, fwMm.GetFirmwarePage());

	fwMm.CommitRvaPoison(67ull);

	*(QWORD*)67ull = 0x1122334455667788;

	fwMm.Cleanup();
	return STATUS_SUCCESS;
}