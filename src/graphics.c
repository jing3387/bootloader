#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"
#include "graphics.h"
#include "terminus.h"

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

// Here be dragons!
void print(struct efi_graphics gs, CHAR16 *input)
{
	UINTN i, j, k;
	UINT32 *at, *ch, pitch;
	CHAR16 *in, idx;
	UINT8 set, *bitmap;

	for (i = 0; i < gs.ngop; i++) {
		in = input;
		at = (UINT32 *)gs.gops[i].Mode->FrameBufferBase;
		pitch = gs.gops[i].Mode->Info->PixelsPerScanLine;
		while (*in) {
			for (j = 0; j < font.Chars; j++) {
				idx = j;
				if (*in == font.Index[j])
					break;
			}
			ch = at;
			bitmap = (UINT8 *)&font.Bitmap[idx * font.Height * 2];
			for (j = 0; j < font.Height * 2; j += 2) {
				for (k = 0; k < 8; k++) {
					set = bitmap[j] & 1 << (7 - k);
					ch[k] = set ? FRGND_COLOR :
						BKGND_COLOR;
				}
				for (k = 0; k < font.Width - 8; k++) {
					set = bitmap[j + 1] & 1 << (7 - k);
					ch[k + 8] = set ? FRGND_COLOR :
						BKGND_COLOR;
				}
				ch += pitch;
			}
			at += font.Width;
			in++;
		}
	}
}
