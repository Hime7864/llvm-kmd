#include "imports.h"

void resolve_imports()
{

}

NTSTATUS start()
{
	resolve_imports();

	return DriverEntry(nullptr, nullptr);
}