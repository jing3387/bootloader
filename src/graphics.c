#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"
#include "graphics.h"
#include "gohu.h"

void clear(struct efi_graphics g)
{
	UINTN i, j, k;
	UINT32 hres, vres, pitch;
	UINT32 *at;

	for (i = 0; i < g.ngop; i++) {
		hres = g.gops[i].Mode->Info->HorizontalResolution;
		vres = g.gops[i].Mode->Info->VerticalResolution;
		pitch = g.gops[i].Mode->Info->PixelsPerScanLine;
		at = (UINT32 *)g.gops[i].Mode->FrameBufferBase;
		for (j = 0; j < vres; j++) {
			for (k = 0; k < hres; k++)
				at[k] = WHITE;
			at += pitch;
		}
	}
}

void putc(struct efi_graphics g, UINT32 row, UINT32 col, CHAR16 c)
{
	UINT8 set, *bitmap;
	UINT32 *at, pitch, x, y;
	UINTN idx, i, j, k;

	for (i = 0; i < g.ngop; i++) {
		for (j = 0; j < font.Chars; j++) {
			idx = j;
			if (c == font.Index[j])
				break;
		}
		bitmap = (UINT8 *)&font.Bitmap[idx * font.Height];
		pitch = g.gops[i].Mode->Info->PixelsPerScanLine;
		y = row * font.Height * pitch;
		x = col * font.Width;
		at = (UINT32 *)g.gops[i].Mode->FrameBufferBase + y + x;
		for (j = 0; j < font.Height; j++) {
			for (k = 0; k < 8; k++) {
				set = bitmap[j] & 1 << (7 - k);
				at[k] = set ? BLACK : WHITE;
			}
			at += pitch;
		}
	}
}
