#include "imports.hpp"

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

void log_to_file(PVOID buffer, DWORD size)
{
	WriteDataToDiskKernel(L"\\??\\C:\\Temp\\svm_log.bin", buffer, size);
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	auto tsc_ratio = MSR::read_tsc_ratio();
	printf("TSC RATIO MSR: 0x%llX\n", tsc_ratio);
	//auto efer = MSR::read_efer();
	//printf("EFER MSR: 0x%llX\n", efer.AsUINT64);
	//MSR::write_efer(efer); // Re-write the same value to demonstrate MSR write
	return STATUS_SUCCESS;
}