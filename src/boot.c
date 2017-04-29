#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"

EFI_STATUS get_graphics(EFI_SYSTEM_TABLE *st, EFI_GRAPHICS_OUTPUT_PROTOCOL **gops, UINTN *ngop)
{
	EFI_STATUS status;
	UINTN id;
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	UINTN handle_count;
	EFI_HANDLE *handle_buffer = NULL;
	UINTN i;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
	UINTN infosz;

	status = st->BootServices->LocateHandleBuffer(
		ByProtocol,
		&gop_guid,
		NULL,
		&handle_count,
		&handle_buffer
	);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			L"Error locating graphics output protocol.\n\r"
		);
		return status;
	}
	status = st->BootServices->AllocatePool(
		EfiLoaderData,
		sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL) * handle_count,
		(void **)gops
	);
	for (i = 0; i < handle_count; i++) {
		status = st->BootServices->HandleProtocol(
			handle_buffer[i],
			&gop_guid,
			(void **)&gops[i]
		);
		if (EFI_ERROR(status)) {
			st->ConOut->OutputString(
				st->ConOut,
				L"Error handling graphics output protocol.\n\r"
			);
			continue;
		}
		for (id = 0; id < gops[i]->Mode->MaxMode; id++) {
			status = gops[i]->QueryMode(
				gops[i], id, &infosz, &info
			);
			if (EFI_ERROR(status)) {
				st->ConOut->OutputString(
					st->ConOut,
					L"Couldn't query graphics mode.\n\r"
				);
				return status;
			}
			status = gops[i]->SetMode(gops[i], id);
			if (status == EFI_UNSUPPORTED) {
				continue;
			} else if (EFI_ERROR(status)) {
				st->ConOut->OutputString(
					st->ConOut,
					L"Failed to set graphics mode.\n\r"
				);
				return status;
			} else {
				break;
			}
		}
	}
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			L"Error finding desired graphics mode.\n\r"
		);
		return status;
	}
	return status;
}

EFI_STATUS mmap_realloc(EFI_SYSTEM_TABLE *st, void **buf, UINTN bufsz)
{
	EFI_STATUS status;

	if (buf == NULL)
		return EFI_INVALID_PARAMETER;
	if (*buf != NULL) {
		status = st->BootServices->FreePool(*buf);
		if (EFI_ERROR(status)) {
			st->ConOut->OutputString(
				st->ConOut,
				L"Error freeing memory map buffer.\n\r"
			);
			return status;
		}
		*buf = NULL;
	}
	status = st->BootServices->AllocatePool(
		EfiLoaderData,
		bufsz,
		buf
	);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			L"Error allocating memory map buffer.\n\r"
		);
		st->BootServices->FreePool(*buf);
		*buf = NULL;
		return status;
	}
	return status;
}

EFI_STATUS get_memory_map(EFI_SYSTEM_TABLE *st, struct memory_map *mm)
{
	EFI_STATUS status;
	EFI_MEMORY_DESCRIPTOR *buf;
	UINTN bufsz = 1024 * 1024;

	status = st->BootServices->AllocatePool(
		EfiLoaderData,
		bufsz,
		(void **)&buf
	);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			L"Error initially allocating memory map buffer.\n\r"
		);
		return status;
	}
	do {
		status = st->BootServices->GetMemoryMap(
			&bufsz,
			buf,
			&mm->map_key,
			&mm->descriptor_size,
			&mm->descriptor_version
		);
		if (status == EFI_BUFFER_TOO_SMALL) {
			status = mmap_realloc(st, (void **)&buf, bufsz);
			if (EFI_ERROR(status))
				return status;
		}
	} while (status == EFI_BUFFER_TOO_SMALL);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			L"Error getting memory map.\n\r"
		);
		return status;
	}
	mm->memory_map_size = bufsz / mm->descriptor_size;
	mm->memory_map = buf;
	return status;
}
