#include "boot.h"
#include "font.h"
#include "term.h"

EFI_STATUS get_graphics(EFI_SYSTEM_TABLE *st, struct vga **vgas, size_t *nvga)
{
	EFI_STATUS status;
	UINTN id;
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	UINTN nhandle;
	EFI_HANDLE *handles;
	UINTN i;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	UINTN infosz;
	struct vga *vga;

	status = st->BootServices->LocateHandleBuffer(
		ByProtocol,
		&gop_guid,
		NULL,
		&nhandle,
		&handles
	);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			u"Error locating graphics output protocol.\n\r"
		);
		return status;
	}
	status = st->BootServices->AllocatePool(
		EfiLoaderData,
		sizeof(struct vga) * nhandle,
		(void **)vgas
	);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			u"Error allocating memory for VGAs.\n\r"
		);
		return status;
	}
	for (i = 0; i < nhandle; i++) {
		status = st->BootServices->HandleProtocol(
			handles[i],
			&gop_guid,
			(void **)&gop
		);
		if (EFI_ERROR(status)) {
			st->ConOut->OutputString(
				st->ConOut,
				u"Error handling graphics output protocol.\n\r"
			);
			continue;
		}
		vga = *vgas + i;
		for (id = 0; id < gop->Mode->MaxMode; id++) {
			status = gop->QueryMode(
				gop, id, &infosz, &info
			);
			if (EFI_ERROR(status)) {
				st->ConOut->OutputString(
					st->ConOut,
					u"Couldn't query graphics mode.\n\r"
				);
				return status;
			}
			status = gop->SetMode(gop, id);
			if (status == EFI_UNSUPPORTED) {
				continue;
			} else if (EFI_ERROR(status)) {
				st->ConOut->OutputString(
					st->ConOut,
					u"Failed to set graphics mode.\n\r"
				);
				return status;
			} else if (
				((gop->Mode->Info->PixelFormat
				  == PixelRedGreenBlueReserved8BitPerColor)
				 ||
				 (gop->Mode->Info->PixelFormat
				  == PixelBlueGreenRedReserved8BitPerColor))
				&& ((gop->Mode->Info->HorizontalResolution
						/ font.Width)
					>= TERM_WIDTH)
				&& ((gop->Mode->Info->VerticalResolution
						/ font.Height)
					>= TERM_HEIGHT)
			) {
				vga->width = gop->Mode->Info->HorizontalResolution;
				vga->height = gop->Mode->Info->VerticalResolution;
				vga->pitch = gop->Mode->Info->PixelsPerScanLine;
				vga->mem = (uint32_t *)gop->Mode->FrameBufferBase;
				break;
			}
		}
	}
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			u"Error finding desired graphics mode.\n\r"
		);
		return status;
	}
	*nvga = nhandle;
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
				u"Error freeing memory map buffer.\n\r"
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
			u"Error allocating memory map buffer.\n\r"
		);
		st->BootServices->FreePool(*buf);
		*buf = NULL;
		return status;
	}
	return status;
}

EFI_STATUS get_memory_map(EFI_SYSTEM_TABLE *st, struct efi_memory_map *mm)
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
			u"Error initially allocating memory map buffer.\n\r"
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
			u"Error getting memory map.\n\r"
		);
		return status;
	}
	mm->memory_map_size = bufsz / mm->descriptor_size;
	mm->memory_map = buf;
	return status;
}

void _main(struct boot) __attribute__((noreturn));

EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st)
{
	EFI_STATUS status;
	struct boot b;

	status = get_graphics(st, &b.graphics.vgas, &b.graphics.nvga);
	if (EFI_ERROR(status))
		return status;
	status = get_memory_map(st, &b.memmap);
	if (EFI_ERROR(status))
		return status;
	status = st->BootServices->ExitBootServices(ih, b.memmap.map_key);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(
			st->ConOut,
			u"Error exiting boot services.\n\r"
		);
		return status;
	}
	_main(b);
}
