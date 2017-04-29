#include "efi_graphics.h"

// TODO: abstract this out to just be "graphics.h" with `FrameBufferBase` as
// `vram`, `PixelsPerScanLine` as `pitch`, and
// `HorizontalResolution`/`VerticalResolution` as `hres`/`vres` respectively.
// Need to use `efi_graphics` as a driver rather than the default
// implementation; though do I have more than one life to waste writing a video
// driver anyways? The optimizations listed below _should_ be enough for
// general purpose use.

// TODO: [Drawing
// text](http://wiki.osdev.org/Drawing_In_Protected_Mode#Drawing_Text)

// FIXME: `putpixel()` is almost always a performance problem. Rewrite
// everything without `putpixel()` and possibly remove `putpixel()` entirely.
void efi_set_pixel(
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 rgb
) {

	EFI_PHYSICAL_ADDRESS vram = gop->Mode->FrameBufferBase;
	UINT32 pitch = gop->Mode->Info->PixelsPerScanLine;
	UINT32 *pixel = (UINT32 *)vram + x + y * pitch;
	*pixel = rgb;
}

// TODO: write `fill_screen()` in terms of `fillrect()`. That is, don't use
// `putpixel()` which recomputes the address n times, where n is the number of
// pixels. For more information see:
// http://wiki.osdev.org/Drawing_In_Protected_Mode#Optimizations
void efi_fill_screen(struct efi_graphics gs, UINT32 rgb)
{
	UINT32 hres, vres;

	rgb |= 0xff000000;
	for (int i = 0; i < gs.ngop; i++) {
		hres = gs.gops[i].Mode->Info->HorizontalResolution;
		vres = gs.gops[i].Mode->Info->VerticalResolution;
		for(int x = 0; x < hres; x += 1) {
			for(int y = 0; y < vres; y += 1) {
				efi_set_pixel(gs.gops + i, x, y, rgb);
			}
		}
	}
}

