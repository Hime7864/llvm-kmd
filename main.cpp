#include <intrinsics.hpp>

void efer_read_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 10000; i++)
	{
		auto count = __rdtsc();
		MSR::EFER();
		count = __rdtsc() - count;
		if(min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 efer_read()
{
	auto efer_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(efer_read_ipi, efer_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if(min > efer_tsc[i])
			min = efer_tsc[i];
	}

	ExFreePool(efer_tsc);
	return min - 50;
}

void efer_write_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	const auto efer = MSR::EFER();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 10000; i++)
	{
		auto count = __rdtsc();
		MSR::EFER(efer);
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 efer_write()
{
	auto efer_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(efer_write_ipi, efer_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > efer_tsc[i])
			min = efer_tsc[i];
	}

	ExFreePool(efer_tsc);
	return min - 50;
}

void hsave_read_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 10000; i++)
	{
		auto count = __rdtsc();
		MSR::HSAVE_PA();
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 hsave_read()
{
	auto hsave_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(hsave_read_ipi, hsave_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > hsave_tsc[i])
			min = hsave_tsc[i];
	}

	ExFreePool(hsave_tsc);
	return min - 50;
}

void hsave_write_ipi(UINT64* min)
{
	auto id = CPUID::current_core_number();
	const auto hsave = MSR::HSAVE_PA();
	min[id] = 0xFFFFFFFF;
	for (int i = 0; i < 10000; i++)
	{
		auto count = __rdtsc();
		MSR::HSAVE_PA(hsave);
		count = __rdtsc() - count;
		if (min[id] > count)
			min[id] = count;
	}
	return;
}

UINT64 hsave_write()
{
	auto hsave_tsc = (UINT64*)ExAllocatePool(NonPagedPool, 0x1000);

	KeIpiGenericCall(hsave_write_ipi, hsave_tsc);

	UINT64 min = 0xFFFFFFFF;
	for (int i = 0; i < KeQueryActiveProcessorCount(0); i++)
	{
		if (min > hsave_tsc[i])
			min = hsave_tsc[i];
	}

	ExFreePool(hsave_tsc);
	return min - 50;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	printf("EFER read  : %lli\n", efer_read());
	printf("EFER write : %lli\n", efer_write());
	printf("HSAVE read : %lli\n", hsave_read());
	printf("HSAVE write: %lli\n", hsave_write());
	return STATUS_SUCCESS;
}