#include "imports.hpp"

struct LINNER_ADDRESS
{
	union
	{
		UINT64 AsUINT64;
		struct {
			UINT64 offset : 12;
			UINT64 pte_index : 9;
			UINT64 pde_index : 9;
			UINT64 pdpe_index : 9;
			UINT64 pml4e_index : 9;
			UINT64 sign_extend : 16;
		};

		struct
		{
			QWORD large_offset : 21;
		};

		struct
		{
			QWORD huge_offset : 30;
		};
	};
	LINNER_ADDRESS() : AsUINT64(0) {}
	template<typename T>
	LINNER_ADDRESS(T data) : AsUINT64(reinterpret_cast<UINT64>((QWORD)data)) {}
};

struct BASIC_RANGE_FLAGS
{
	union
	{
		UINT32 AsUINT32;
		struct
		{
			UINT32 OsCommited : 1;
			UINT32 SmmReserved : 1;
			UINT32 OtherReserved : 30;
		};
	};
};

struct _BASIC_RANGE
{
	ULONGLONG Start;
	ULONGLONG End;
	BASIC_RANGE_FLAGS Flags;
};

struct _MEMORY_RANGES
{
	DWORD range_count;
	_BASIC_RANGE ranges[128];
};

struct KMD_MEMORY_BLOCK
{
	QWORD original_linearAddress;
	QWORD linearAddress;
	QWORD physicalAddress;
	QWORD sizeInBytes;

	QWORD to_phys(QWORD virtAddress)
	{
		INT64 delta = (INT64)virtAddress - (INT64)linearAddress;
		if (delta >= 0 && delta < sizeInBytes)
			return physicalAddress + delta;
		return 0x0;
	}

	QWORD to_virt(QWORD physAddress)
	{
		INT64 delta = (INT64)physAddress - (INT64)physicalAddress;
		if (delta >= 0 && delta < sizeInBytes)
			return linearAddress + delta;
		return 0x0;
	}
};

struct KMD_MEMORY_BLOCKS
{
	KMD_MEMORY_BLOCK memoryDescriptor;
	KMD_MEMORY_BLOCK hostPageMap;
	KMD_MEMORY_BLOCK driverCode;
	KMD_MEMORY_BLOCK hostStack;
};

struct KMD_MEMORY_PAGE_INDEX
{
	QWORD guestBlockPtr;
	QWORD hostBlockPtr;
};

struct ALIGN(4096) KMD_MEMORY_MANAGER
{
	KMD_MEMORY_BLOCKS memoryBlocks;
	KMD_MEMORY_PAGE_INDEX pageIndices;
	PHYSICAL_ADDRESS hostDTB;
	PHYSICAL_ADDRESS driverDTB;
};

typedef struct _MEMORY_RANGES* PMEMORY_RANGES;
typedef struct _MEMORY_RANGES MEMORY_RANGES;

KMD_MEMORY_MANAGER* MmKmd = nullptr;

QWORD FindNextInvalid(QWORD start_pa, QWORD end_pa)
{
	auto pMmPfn = MmPfnDatabase();
	for (QWORD current_pa = start_pa; current_pa < end_pa; current_pa += 0x1000)
	{
		PMMPFN MmPfn = &pMmPfn[current_pa >> 12];
		if (MmPfn->PteLong == 0)
			return current_pa;
	}
	return end_pa;
}

QWORD FindNextValid(QWORD start_pa, QWORD end_pa)
{
	auto pMmPfn = MmPfnDatabase();
	for (QWORD current_pa = start_pa; current_pa < end_pa; current_pa += 0x1000)
	{
		PMMPFN MmPfn = &pMmPfn[current_pa >> 12];
		if (MmPfn->PteLong != 0)
			return current_pa;
	}
	return end_pa;
}

void GetPhysicalMemoryDump(PMEMORY_RANGES memory_ranges)
{
	if (!memory_ranges)
		return;

	RtlFillMemory(memory_ranges, 0x1000, 0);
	auto MmPmr = MmGetPhysicalMemoryRanges();
	QWORD MmPmr_counter = 0x0;
	QWORD total_physical_memory = 0x0;

	while (MmPmr->NumberOfBytes.QuadPart) {
		memory_ranges->ranges[memory_ranges->range_count].Start = MmPmr->BaseAddress.QuadPart;
		memory_ranges->ranges[memory_ranges->range_count].End = MmPmr->BaseAddress.QuadPart + MmPmr->NumberOfBytes.QuadPart;
		memory_ranges->ranges[memory_ranges->range_count].Flags.OsCommited = true;
		memory_ranges->range_count++;
		MmPmr_counter += MmPmr->NumberOfBytes.QuadPart;
		MmPmr++;
		if (MmPmr->BaseAddress.QuadPart)
		{
			memory_ranges->ranges[memory_ranges->range_count].Start = memory_ranges->ranges[memory_ranges->range_count - 1].End;
			memory_ranges->ranges[memory_ranges->range_count].End = MmPmr->BaseAddress.QuadPart;
			memory_ranges->ranges[memory_ranges->range_count].Flags.OsCommited = false;
			memory_ranges->range_count++;
		}
		else
		{
			total_physical_memory = 1;
			while (total_physical_memory < MmPmr_counter) {
				total_physical_memory <<= 1;
			}
			if(total_physical_memory > memory_ranges->ranges[memory_ranges->range_count - 1].End)
			{
				memory_ranges->ranges[memory_ranges->range_count].Start = memory_ranges->ranges[memory_ranges->range_count - 1].End;
				memory_ranges->ranges[memory_ranges->range_count].End = total_physical_memory;
				memory_ranges->ranges[memory_ranges->range_count].Flags.OsCommited = false;
				memory_ranges->range_count++;
			}
		}
	}

	ExFreePool(MmPmr);
	return;
}

bool CreateLinearAddress(PHYSICAL_ADDRESS dtb, LINNER_ADDRESS rva, MMPTE_HARDWARE pte)
{
	if (!dtb || !pte.AsUINT64)
		return false;

	auto memoryBlocks = &MmKmd->memoryBlocks;
	auto pageIndices = &MmKmd->pageIndices;

	DWORD idx[4]{
		(DWORD)rva.pml4e_index,
		(DWORD)rva.pdpe_index,
		(DWORD)rva.pde_index,
		(DWORD)rva.pte_index
	};

	auto hostPageMap = &memoryBlocks->hostPageMap;
	if (pageIndices->hostBlockPtr == hostPageMap->sizeInBytes + hostPageMap->linearAddress)
		return false;
	auto pPte = (MMPTE_HARDWARE*)hostPageMap->to_virt(dtb);
	if (!pPte)
		return false;

	for (int i = 0; i < 3; i++)
	{
		auto current_pte = &pPte[idx[i]];
		if (!current_pte->Valid)
		{
			current_pte->PageFrameNumber = (DWORD)(hostPageMap->to_phys(pageIndices->hostBlockPtr) >> 12);
			if (!current_pte->PageFrameNumber)
			{
				return false;
			}
			current_pte->Valid = true;
			current_pte->Dirty1 = true;
			current_pte->Dirty = true;
			current_pte->Write = true;
			current_pte->WriteThrough = true;
			current_pte->NoExecute = true;
			pageIndices->hostBlockPtr += 0x1000;
		}

		pPte = (MMPTE_HARDWARE*)hostPageMap->to_virt(((QWORD)current_pte->PageFrameNumber) << 12);
		if (!pPte)
		{
			return false;
		}
	}
	pPte[idx[3]] = pte;

	return true;
}

bool ReserveMemory(KMD_MEMORY_BLOCK* outBlock, QWORD sizeInBytes)
{
	if (!outBlock)
		return false;

	RtlFillMemory(outBlock, sizeof(KMD_MEMORY_BLOCK), 0);

	if (sizeInBytes & 0xFFF)
		sizeInBytes = (sizeInBytes & ~0xFFF) + 0x1000;

	auto alloc_size = sizeInBytes;
	auto va_alloc = (QWORD)MmAllocateContiguousMemorySpecifyCacheNode(
		alloc_size,
		0,
		(QWORD)-1,
		0,
		MmNonCached,
		MM_ANY_NODE_OK
	);
	RtlFillMemory((PVOID)va_alloc, alloc_size, 0);

	if (!va_alloc)
	{
		return false;
	}

	auto pa_alloc = (QWORD)MmGetPhysicalAddress((PVOID)va_alloc);

	outBlock->linearAddress = va_alloc;
	outBlock->original_linearAddress = va_alloc;
	outBlock->physicalAddress = pa_alloc;
	outBlock->sizeInBytes = sizeInBytes;

	return true;
}

bool ReleaseMemory(KMD_MEMORY_BLOCK* outBlock)
{
	if (!outBlock)
	{
		return false;
	}
	if (!outBlock->original_linearAddress)
	{
		return false;
	}


	auto va_alloc = (PVOID)outBlock->original_linearAddress;
	auto alloc_size = outBlock->sizeInBytes;

	MmFreeContiguousMemorySpecifyCache((PVOID)va_alloc, alloc_size, MmNonCached);

	return true;
}

bool SetupMemoryManager()
{
	KMD_MEMORY_BLOCK new_alloc{ 0 };

	if (!ReserveMemory(&new_alloc, 0x1000))
		return false;

	MmKmd = (KMD_MEMORY_MANAGER*)new_alloc.linearAddress;

	auto memoryBlocks = &MmKmd->memoryBlocks;

	// memory descriptor
	auto memoryDescriptor = &memoryBlocks->memoryDescriptor;
	memoryDescriptor->linearAddress = new_alloc.linearAddress;
	memoryDescriptor->original_linearAddress = new_alloc.original_linearAddress;
	memoryDescriptor->physicalAddress = new_alloc.physicalAddress;
	memoryDescriptor->sizeInBytes = new_alloc.sizeInBytes;

	// host page map
	if (!ReserveMemory(&new_alloc, 16 << 20))
		return false;

	auto hostPageMap = &memoryBlocks->hostPageMap;
	hostPageMap->linearAddress = new_alloc.linearAddress;
	hostPageMap->original_linearAddress = new_alloc.original_linearAddress;
	hostPageMap->physicalAddress = new_alloc.physicalAddress;
	hostPageMap->sizeInBytes = new_alloc.sizeInBytes;

	// driver code
	QWORD module_base = 0;
	QWORD module_size = 0;
	Utils::SelfModuleBase(&module_base);

	if (module_base)
	{
		int pages_mapped = 0;
		do {
			pages_mapped++;
		} while (Utils::IsAddressValid(module_base + pages_mapped * 4096));
		module_size = (QWORD)(pages_mapped << 12);
	}
	else
	{
		return false;
	}

	if (!ReserveMemory(&new_alloc, module_size))
		return false;

	auto driverCode = &memoryBlocks->driverCode;
	driverCode->original_linearAddress = new_alloc.linearAddress;
	driverCode->linearAddress = module_base;
	driverCode->physicalAddress = new_alloc.physicalAddress;
	driverCode->sizeInBytes = module_size;

	RtlCopyMemory(
		(PVOID)((QWORD)driverCode->original_linearAddress),
		(CONST PVOID)driverCode->linearAddress,
		driverCode->sizeInBytes
	);

	// setup host stack
	auto CoreCount = KeQueryActiveProcessorCount(NULL);
	if (!ReserveMemory(&new_alloc, CoreCount * 0x8000))
		return false;

	auto hostStack = &memoryBlocks->hostStack;
	hostStack->linearAddress = new_alloc.linearAddress;
	hostStack->original_linearAddress = new_alloc.original_linearAddress;
	hostStack->physicalAddress = new_alloc.physicalAddress;
	hostStack->sizeInBytes = new_alloc.sizeInBytes;

	// setup DTBs
	MmKmd->hostDTB = hostPageMap->physicalAddress;
	MmKmd->driverDTB = __readcr3();

	// setup page indices
	auto pageIndices = &MmKmd->pageIndices;
	pageIndices->hostBlockPtr = hostPageMap->linearAddress + 0x1000;

	return true;
}

bool CleanupMemoryManager()
{
	bool results = true;
	KMD_MEMORY_BLOCK old_alloc{ 0 };
	if (!MmKmd)
		return true;

	auto memoryBlocks = &MmKmd->memoryBlocks;

	auto hostPageMap = &memoryBlocks->hostPageMap;
	old_alloc.original_linearAddress = hostPageMap->original_linearAddress;
	old_alloc.sizeInBytes = hostPageMap->sizeInBytes;
	results = ReleaseMemory(&old_alloc);

	auto driverCode = &memoryBlocks->driverCode;
	old_alloc.original_linearAddress = driverCode->original_linearAddress;
	old_alloc.sizeInBytes = driverCode->sizeInBytes;
	results = ReleaseMemory(&old_alloc);

	auto hostStack = &memoryBlocks->hostStack;
	old_alloc.original_linearAddress = hostStack->original_linearAddress;
	old_alloc.sizeInBytes = hostStack->sizeInBytes;
	results = ReleaseMemory(&old_alloc);

	auto memoryDescriptor = &memoryBlocks->memoryDescriptor;
	old_alloc.original_linearAddress = memoryDescriptor->original_linearAddress;
	old_alloc.sizeInBytes = memoryDescriptor->sizeInBytes;
	results = ReleaseMemory(&old_alloc);
	MmKmd = nullptr;

	return results;
}

bool CreateHostPageMappings()
{
	auto memoryBlocks = &MmKmd->memoryBlocks;

	// memory descriptor
	auto memoryDescriptor = &memoryBlocks->memoryDescriptor;
	for (QWORD i = 0; i < memoryDescriptor->sizeInBytes; i += 0x1000)
	{
		LINNER_ADDRESS rva = memoryDescriptor->original_linearAddress + i;
		auto pte = Utils::GetPTE(rva.AsUINT64);

		auto results = CreateLinearAddress(
			MmKmd->hostDTB,
			rva,
			pte
		);

		if (!results)
		{
			printf("memory descriptor");
			return results;
		}
	}

	//hostPageMap
	auto hostPageMap = &memoryBlocks->hostPageMap;
	for (QWORD i = 0; i < hostPageMap->sizeInBytes; i += 0x1000)
	{
		LINNER_ADDRESS rva = hostPageMap->original_linearAddress + i;
		auto pte = Utils::GetPTE(rva.AsUINT64);

		auto results = CreateLinearAddress(
			MmKmd->hostDTB,
			rva,
			pte
		);

		if (!results)
		{
			printf("hostPageMap");
			return results;
		}
	}

	// driverCode
	auto driverCode = &memoryBlocks->driverCode;
	for (QWORD i = 0; i < driverCode->sizeInBytes; i += 0x1000)
	{
		LINNER_ADDRESS rva = driverCode->original_linearAddress + i;
		auto pte = Utils::GetPTE(rva.AsUINT64);
		rva.AsUINT64 = driverCode->linearAddress + i;
		pte.PageFrameNumber = (DWORD)((driverCode->physicalAddress + i) >> 12);
		auto results = CreateLinearAddress(
			MmKmd->hostDTB,
			rva,
			pte
		);
		if (!results)
		{
			printf("driverCode");
			return results;
		}
	}

	// hostStack
	auto hostStack = &memoryBlocks->hostStack;
	for (QWORD i = 0; i < hostStack->sizeInBytes; i += 0x1000)
	{
		LINNER_ADDRESS rva = hostStack->original_linearAddress + i;
		auto pte = Utils::GetPTE(rva.AsUINT64);
		auto results = CreateLinearAddress(
			MmKmd->hostDTB,
			rva,
			pte
		);
		if (!results)
		{
			printf("hostStack");
			return results;
		}
	}
	return true;
}

QWORD NOINLINE CallOnHost(FnPtr* func, ...)
{
	__asm
	{
		push rdx
		push r8
		push r9
	}
	auto memoryBlocks = &MmKmd->memoryBlocks;

	auto hostStack = &memoryBlocks->hostStack;
	auto core_id = CPUID::current_core_number();

	auto stack_base = hostStack->linearAddress + (core_id * 0x8000) + 0x6000;
	auto host_dtb = MmKmd->hostDTB;
	auto driver_dtb = MmKmd->driverDTB;
	QWORD function_result = 0x0;
	QWORD function_ptr = (QWORD)func;
	__asm
	{
		pop r8
		pop rdx
		pop rcx
		// save registers
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15
		push rcx
		push rdx
		push r8

		// mov ctx to gprs
		mov r10, stack_base
		mov r11, host_dtb
		mov r12, driver_dtb
		mov r13, rsp
		mov r14, rbp
		mov r15, function_ptr

		// switch to host context
		mfence
		cli
		mov rsp, r10
		mov rbp, r10
		mov cr3, r11
		

		//push registers again to host stack
		push r10
		push r11
		push r12
		push r13
		push r14
		push rcx
		push rdx
		push r8

		// call function ptr
		call r15
		//capture return value
		mov r15, rax

		// restore host registers
		pop r8
		pop rdx
		pop rcx
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10

		// switch back to driver context
		mov cr3, r12
		mov rsp, r13
		mov rbp, r14
		sti
		mfence

		mov function_result, r15

		// restore registers
		pop r8
		pop rdx
		pop rcx
		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
	}
	return function_result;
}

void imlp_CopySinglePage(PVOID dest, UINT64 source, UINT64 count)
{
	for (QWORD current = 0; current < count; current += 0x1000)
	{
		__invlpg((PVOID)((QWORD)dest + current));
		__invlpg((PVOID)((QWORD)source + current));

		memcpy(
			(PVOID)((QWORD)dest + current), 
			(PVOID)((QWORD)source + current), 
			0x1000
		);
	}
	return;
}

bool CopyReclaimedMemory(PVOID buffer, PHYSICAL_ADDRESS physical, SIZE_T count)
{
	auto cb = count & ~0xFFF;
	QWORD buf = (QWORD)((QWORD)buffer & ~0xFFF);
	auto pa = (QWORD)(physical & ~0xFFF);

	for (int i = 0; i < cb; i += 0x1000)
	{
		auto pte = Utils::GetPTE((QWORD)buf);
		if (!pte.AsUINT64)
			return false;
		// destination mapping
		auto status = CreateLinearAddress(
			MmKmd->hostDTB,
			buf,
			pte
		);
		if (!status)
			return false;
		// source mapping
		pte.PageFrameNumber = (DWORD)(pa >> 12);
		status = CreateLinearAddress(
			MmKmd->hostDTB,
			pa,
			pte
		);
		if (!status)
			return false;

		buf += 0x1000;
		pa += 0x1000;
	}

	auto last_irql = __readcr8();
	__writecr8(DISPATCH_LEVEL);
	
	CallOnHost((FnPtr*)imlp_CopySinglePage, buffer, physical, count);
	
	__writecr8(last_irql);

	return true;
}

UINT8 sanitize_char(UINT8 c)
{
	if (c < 32 || c > 126)
		return '.';
	return c;
}

void dbg_print_page(QWORD buffer, QWORD base)
{
	auto page = buffer & ~0xFFF;
	auto addr = base;
	for (int i = 0; i < 4096;)
	{
		auto ptr = (UINT8*)(page + i);
		printf("%p : %02X%02X%02X%02X %02X%02X%02X%02X  %02X%02X%02X%02X %02X%02X%02X%02X  %02X%02X%02X%02X %02X%02X%02X%02X  %02X%02X%02X%02X %02X%02X%02X%02X : %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 
			addr,
			ptr[0x00], ptr[0x01], ptr[0x02], ptr[0x03], ptr[0x04], ptr[0x05], ptr[0x06], ptr[0x07],
			ptr[0x08], ptr[0x09], ptr[0x0A], ptr[0x0B], ptr[0x0C], ptr[0x0D], ptr[0x0E], ptr[0x0F],
			ptr[0x10], ptr[0x11], ptr[0x12], ptr[0x13], ptr[0x14], ptr[0x15], ptr[0x16], ptr[0x17],
			ptr[0x18], ptr[0x19], ptr[0x1A], ptr[0x1B], ptr[0x1C], ptr[0x1D], ptr[0x1E], ptr[0x1F],
			sanitize_char(ptr[0x00]), sanitize_char(ptr[0x01]), sanitize_char(ptr[0x02]), sanitize_char(ptr[0x03]), sanitize_char(ptr[0x04]), sanitize_char(ptr[0x05]), sanitize_char(ptr[0x06]), sanitize_char(ptr[0x07]),
			sanitize_char(ptr[0x08]), sanitize_char(ptr[0x09]), sanitize_char(ptr[0x0A]), sanitize_char(ptr[0x0B]), sanitize_char(ptr[0x0C]), sanitize_char(ptr[0x0D]), sanitize_char(ptr[0x0E]), sanitize_char(ptr[0x0F]),
			sanitize_char(ptr[0x10]), sanitize_char(ptr[0x11]), sanitize_char(ptr[0x12]), sanitize_char(ptr[0x13]), sanitize_char(ptr[0x14]), sanitize_char(ptr[0x15]), sanitize_char(ptr[0x16]), sanitize_char(ptr[0x17]),
			sanitize_char(ptr[0x18]), sanitize_char(ptr[0x19]), sanitize_char(ptr[0x1A]), sanitize_char(ptr[0x1B]), sanitize_char(ptr[0x1C]), sanitize_char(ptr[0x1D]), sanitize_char(ptr[0x1E]), sanitize_char(ptr[0x1F])

		);
		i += 32;
		addr += 32;
	}
	return;
}

NTSTATUS WriteDataToDiskKernel(
	PCWSTR FilePath,     // File path to write to
	PVOID Data,          // Pointer to data buffer
	SIZE_T DataLength    // Length of the data to write
) {
	NTSTATUS Status{ 0 };
	HANDLE FileHandle{ 0 };  // Initialize to NULL to avoid undefined behavior
	OBJECT_ATTRIBUTES ObjectAttributes{ 0 };
	IO_STATUS_BLOCK IoStatusBlock{ 0 };
	UNICODE_STRING UnicodeFilePath{ 0 };

	// Initialize the file path
	RtlInitUnicodeString(&UnicodeFilePath, FilePath);

	InitializeObjectAttributes(
		&ObjectAttributes,
		&UnicodeFilePath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);

	// Create or open the file
	Status = ZwCreateFile(
		&FileHandle,
		GENERIC_ALL,
		&ObjectAttributes,
		&IoStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OVERWRITE_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);

	if (!NT_SUCCESS(Status)) {
		printf("Failed to create/open file: 0x%X\n", Status);
		return Status;
	}

	// Write data to the file
	Status = ZwWriteFile(
		FileHandle,
		NULL,
		NULL,
		NULL,
		&IoStatusBlock,
		Data,
		(ULONG)DataLength,
		NULL,
		NULL
	);

	if (!NT_SUCCESS(Status)) {
		printf("Failed to write to file: 0x%X\n", Status);
	}

	// Close the file handle
	if (FileHandle) {
		ZwClose(FileHandle);
	}

	return Status;
}

QWORD GetFreePage(_BASIC_RANGE* range)
{
	auto current_page = range->Start;
	auto range_size = range->End - range->Start;
	//range_size = 0x1000;
	auto allocate_buffer = (PVOID)ExAllocatePool(NonPagedPool, range_size);
	if (allocate_buffer == 0)
		return 0x0ULL;

	RtlFillMemory(allocate_buffer, range_size, 0xFF);
	bool test = CopyReclaimedMemory(allocate_buffer, current_page, range_size);
	if (test)
	{
		for (QWORD offset = 0; offset < range_size; offset += 0x1000)
		{
			auto ptr = (UINT8*)((QWORD)allocate_buffer + offset);
			bool is_free = true;
			int skip_bytes = 100;
			for (int i = 0; i < 0x1000; i++)
			{
				if (ptr[i] != 0x00)
				{
					if (skip_bytes)
					{
						skip_bytes--;
						continue;
					}
					is_free = false;
					break;
				}
			}
			if (is_free)
			{
				current_page += offset;
				printf(" Free UEFI page found at %p\n", current_page);
				//ExFreePool(allocate_buffer);
				//return current_page;
			}
		}
	}
	else
		printf(" CopyReclaimedMemory failed for range %p - %p\n", range->Start, range->End);
	ExFreePool(allocate_buffer);
	return 0x0ULL;
}

void ScanForFreeUefiPages(PMEMORY_RANGES memory_ranges)
{
	if (!memory_ranges)
		return;

	auto range = &memory_ranges->ranges[3];
	if (!range->Flags.OsCommited)
	{
		auto page = GetFreePage(range);
		if (page)
		{
			printf(" Found free UEFI page at %p\n", page);
		}
	}

	range = &memory_ranges->ranges[5];
	if (!range->Flags.OsCommited)
	{
		auto page = GetFreePage(range);
		if (page)
		{
			printf(" Found free UEFI page at %p\n", page);
		}
	}

	range = &memory_ranges->ranges[7];
	if (!range->Flags.OsCommited)
	{
		auto page = GetFreePage(range);
		if (page)
		{
			printf(" Found free UEFI page at %p\n", page);
		}
	}

	range = &memory_ranges->ranges[9];
	if (!range->Flags.OsCommited)
	{
		auto page = GetFreePage(range);
		if (page)
		{
			printf(" Found free UEFI page at %p\n", page);
		}
	}

	for (int i = 0; i < memory_ranges->range_count; i++)
	{
	    range = &memory_ranges->ranges[i];
		if (!range->Flags.OsCommited)
		{
			printf("Non OS Commited %02d: %p - %p (%p)\n", i,
				range->Start,
				range->End,
				range->End - range->Start);
		}
	}
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	
	auto memory_ranges = (PMEMORY_RANGES)ExAllocatePool(NonPagedPool, 0x1000);

	SetupMemoryManager();

	CreateHostPageMappings();

	GetPhysicalMemoryDump(memory_ranges);

	ScanForFreeUefiPages(memory_ranges);

	CleanupMemoryManager();

	ExFreePool(memory_ranges);

	return STATUS_SUCCESS;
}