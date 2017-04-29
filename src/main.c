#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"

void set_pixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 w, UINT32 h, UINT32 rgb)
{
    UINT32 *addr = (UINT32 *)gop->Mode->FrameBufferBase + w + h * gop->Mode->Info->PixelsPerScanLine;
    *addr = rgb;
}

void fill_screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 rgb)
{
    rgb |= 0xff000000;
    for(int x = 0; x < gop->Mode->Info->HorizontalResolution; x += 1) {
        for(int y = 0; y < gop->Mode->Info->VerticalResolution; y += 1) {
            set_pixel(gop, x, y, rgb);
        }
    }
}


EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st)
{
	UINTN i;
	EFI_STATUS status;
	UINTN ngop;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gops;
	struct memory_map mmap;

	st->ConOut->OutputString(st->ConOut, L"Getting graphics.\n\r");
	status = get_graphics(st, &gops, &ngop);
	if (EFI_ERROR(status))
		return status;
	st->ConOut->OutputString(st->ConOut, L"Got graphics.\n\r");
	st->ConOut->OutputString(st->ConOut, L"Getting memory map.\n\r");
	status = get_memory_map(st, &mmap);
	if (EFI_ERROR(status))
		return status;
	// We've got the memory map, don't call any other UEFI services!
	status = st->BootServices->ExitBootServices(ih, mmap.map_key);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(st->ConOut, L"Error exiting boot services.\n\r");
		return status;
	}
	for (i = 0; i < ngop; i++)
		fill_screen(gops + i, 0xff0000);
	return status;
}
