#include "boot.h"

void set_pixel(
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 rgb
) {

	EFI_PHYSICAL_ADDRESS base = gop->Mode->FrameBufferBase;
	UINT32 w = gop->Mode->Info->PixelsPerScanLine;
	UINT32 *addr = (UINT32 *)base + x + y * w;
	*addr = rgb;
}

void fill_screen(struct efi_graphics gs, UINT32 rgb)
{
	UINT32 hres, vres;

	rgb |= 0xff000000;
	for (int i = 0; i < gs.ngop; i++) {
		hres = gs.gops[i].Mode->Info->HorizontalResolution;
		vres = gs.gops[i].Mode->Info->VerticalResolution;
		for(int x = 0; x < hres; x += 1) {
			for(int y = 0; y < vres; y += 1) {
				set_pixel(gs.gops + i, x, y, rgb);
			}
		}
	}
}

void kernel_main(struct efi efi)
{
	fill_screen(efi.graphics, 0x00ff00);
}
