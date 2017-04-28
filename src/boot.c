#include "Uefi.h"
#include "boot.h"

EFI_STATUS realloc_buffer(EFI_SYSTEM_TABLE *st, void **buf, UINTN bufsz)
{
	EFI_STATUS status;

	if (buf == NULL)
		return EFI_INVALID_PARAMETER;
	if (*buf != NULL) {
		status = st->BootServices->FreePool(*buf);
		if (EFI_ERROR(status))
			return status;
		*buf = NULL;
	}
	if (bufsz > 0) {
		status = st->BootServices->AllocatePool(EfiLoaderData, bufsz, buf);
		if (EFI_ERROR(status)) {
			st->BootServices->FreePool(*buf);
			*buf = NULL;
			return status;
		}
	}
	return status;
}

EFI_STATUS get_memory_map(EFI_SYSTEM_TABLE *st, struct memory_map *mm)
{
	EFI_STATUS status;
	EFI_MEMORY_DESCRIPTOR *buf;
	UINTN bufsz;

	do {
		status = realloc_buffer(st, (void **)&buf, bufsz);
		if (EFI_ERROR(status))
			return status;
		status = st->BootServices->GetMemoryMap(
			&bufsz,
			buf,
			&mm->map_key,
			&mm->descriptor_size,
			&mm->descriptor_version
		);
	} while (status == EFI_BUFFER_TOO_SMALL);
	if (EFI_ERROR(status))
		return status;
	mm->memory_map_size = bufsz / mm->descriptor_size;
	mm->memory_map = buf;
	return status;
}
