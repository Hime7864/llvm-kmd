#pragma once

class FWA
{
private:
	static PHYSICAL_MEMORY_RANGE fw_range[31];
	static UINT64 fw_range_count;
	static UINT64 page_idx;

	static bool NAKED is_zero_page(PVOID page);
public:
	static void Initialize();

	static void Cleanup();

	static UINT32 pages_free();

	static UINT32 pages_total();

	static PHYSICAL_ADDRESS ReservePages(SIZE_T pages);
};

