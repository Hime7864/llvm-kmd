#include <intrinsics.hpp>

constexpr UINT32 BENCHMARK_ITERATIONS = 10000;

enum class TscSource : UINT32
{
	Rdtsc = 0,
	Rdtscp,
	MsrTsc
};

enum class BenchTarget : UINT32
{
	EferRead = 0,
	EferWrite,
	HsaveRead,
	HsaveWrite
};

struct TimingStats
{
	UINT64 init;
	UINT64 min;
	UINT64 max;
	UINT64 avg;
};

struct CoreTimingStats
{
	UINT64 init;
	UINT64 min;
	UINT64 max;
	UINT64 sum;
	UINT32 samples;
};

struct BenchmarkContext
{
	TscSource source;
	BenchTarget target;
	CoreTimingStats* per_core;
};

UINT64 read_tsc_source(TscSource source)
{
	switch (source)
	{
	case TscSource::Rdtsc:
		return __rdtsc();
	case TscSource::Rdtscp:
	{
		return __rdtsc();
	}
	case TscSource::MsrTsc:
		return MSR::TSC();
	default:
		return 0;
	}
}

VOID run_benchmark_target(BenchTarget target, UINT64 efer, UINT64 hsave)
{
	switch (target)
	{
	case BenchTarget::EferRead:
		MSR::EFER();
		break;
	case BenchTarget::EferWrite:
		MSR::EFER({ .AsUINT64 = efer });
		break;
	case BenchTarget::HsaveRead:
		MSR::HSAVE_PA();
		break;
	case BenchTarget::HsaveWrite:
		MSR::HSAVE_PA(hsave);
		break;
	default:
		break;
	}
}

ULONG_PTR benchmark_ipi_callback(ULONG_PTR context_ptr)
{
	auto* context = (BenchmarkContext*)context_ptr;
	const auto id = CPUID::current_core_number();
	auto& stats = context->per_core[id];
	const auto efer = MSR::EFER().AsUINT64;
	const auto hsave = MSR::HSAVE_PA();

	stats.init = 0;
	stats.min = ~0ull;
	stats.max = 0;
	stats.sum = 0;
	stats.samples = BENCHMARK_ITERATIONS;

	for (UINT32 i = 0; i < BENCHMARK_ITERATIONS; i++)
	{
		const auto start = read_tsc_source(context->source);
		run_benchmark_target(context->target, efer, hsave);
		const auto end = read_tsc_source(context->source);
		const auto delta = end - start;

		if (i == 0)
			stats.init = delta;
		if (delta < stats.min)
			stats.min = delta;
		if (delta > stats.max)
			stats.max = delta;
		stats.sum += delta;
	}

	return 0;
}

TimingStats run_benchmark(TscSource source, BenchTarget target)
{
	TimingStats result = {};
	const auto cpu_count = KeQueryActiveProcessorCount(0);
	if (cpu_count == 0)
		return result;

	const auto alloc_size = sizeof(CoreTimingStats) * cpu_count;
	auto* per_core = (CoreTimingStats*)ExAllocatePool(NonPagedPool, alloc_size);
	if (!per_core)
		return result;

	memset(per_core, 0, alloc_size);
	BenchmarkContext context = { source, target, per_core };
	KeIpiGenericCall(benchmark_ipi_callback, &context);

	UINT64 init_sum = 0;
	UINT64 global_min = ~0ull;
	UINT64 global_max = 0;
	UINT64 total_sum = 0;
	UINT64 total_samples = 0;

	for (UINT32 i = 0; i < cpu_count; i++)
	{
		const auto& stats = per_core[i];
		init_sum += stats.init;
		if (stats.min < global_min)
			global_min = stats.min;
		if (stats.max > global_max)
			global_max = stats.max;
		total_sum += stats.sum;
		total_samples += stats.samples;
	}

	result.init = init_sum / cpu_count;
	result.min = global_min;
	result.max = global_max;
	result.avg = total_samples ? (total_sum / total_samples) : 0;

	ExFreePool(per_core);
	return result;
}

const char* tsc_source_name(TscSource source)
{
	switch (source)
	{
	case TscSource::Rdtsc:
		return "rdtsc";
	case TscSource::Rdtscp:
		return "rdtscp";
	case TscSource::MsrTsc:
		return "msr::tsc";
	default:
		return "unknown";
	}
}

VOID print_stats(const char* operation, TscSource source, const TimingStats& stats)
{
	printf("%s [%s] init:%llu min:%llu max:%llu avg:%llu\n",
		operation,
		tsc_source_name(source),
		stats.init,
		stats.min,
		stats.max,
		stats.avg);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	constexpr TscSource sources[] =
	{
		TscSource::Rdtsc,
		TscSource::Rdtscp,
		TscSource::MsrTsc
	};

	for (const auto source : sources)
	{
		print_stats("EFER read ", source, run_benchmark(source, BenchTarget::EferRead));
		print_stats("EFER write", source, run_benchmark(source, BenchTarget::EferWrite));
		print_stats("HSAVE read", source, run_benchmark(source, BenchTarget::HsaveRead));
		print_stats("HSAVE write", source, run_benchmark(source, BenchTarget::HsaveWrite));
	}

	return STATUS_SUCCESS;
}