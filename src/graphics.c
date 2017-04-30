#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"
#include "graphics.h"
#include "font8x8_basic.h"

void clear(struct efi_graphics gs)
{
	UINTN i, j, k;
	UINT32 hres, vres, pitch;
	UINT32 *at;

	for (i = 0; i < gs.ngop; i++) {
		hres = gs.gops[i].Mode->Info->HorizontalResolution;
		vres = gs.gops[i].Mode->Info->VerticalResolution;
		pitch = gs.gops[i].Mode->Info->PixelsPerScanLine;
		at = (UINT32 *)gs.gops[i].Mode->FrameBufferBase;
		for (j = 0; j < vres; j++) {
			for (k = 0; k < hres; k++)
				at[k] = BKGND_COLOR;
			at += pitch;
		}
	}
}

void print(struct efi_graphics gs, char *input)
{
	UINTN i, j, k;
	char *bitmap;
	UINT32 *at, *ch, pitch;
	UINT8 *in, set;

	for (i = 0; i < gs.ngop; i++) {
		in = (UINT8 *)input;
		at = (UINT32 *)gs.gops[i].Mode->FrameBufferBase;
		while (*in) {
			ch = at;
			pitch = gs.gops[i].Mode->Info->PixelsPerScanLine;
			bitmap = font8x8_basic[*in];
			for (j = 0; j < 8; j++) {
				for (k = 0; k < 8; k++) {
					set = bitmap[j] & 1 << k;
					ch[k] = set ? 
						FRGND_COLOR : 
						BKGND_COLOR;
				}
				ch += pitch;
			}
			at += 8;
			in++;
		}
	}
}
