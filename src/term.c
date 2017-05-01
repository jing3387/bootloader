#include <stddef.h>
#include <stdint.h>
#include "term.h"
#include "vga.h"
#include "font.h"

const size_t TERM_WIDTH = 180;
const size_t TERM_HEIGHT = 64;

static size_t term_row;
static size_t term_col;
static uint32_t term_bkgnd;
static uint32_t term_frgnd;
static struct vga *term_vgas;
static size_t term_nvga;

void term_init(struct vga *vgas, size_t nvga)
{
	term_row = 0;
	term_col = 0;
	term_bkgnd = VGA_ACME;
	term_frgnd = VGA_BLACK;
	term_vgas = vgas;
	term_nvga = nvga;
	term_clear();
}

void term_clear(void)
{
	size_t i, y, x, idx;
	struct vga *vga;

	for (i = 0; i < term_nvga; i++) {
		vga = &term_vgas[i];
		for (y = 0; y < vga->height; y++) {
			for (x = 0; x < vga->width; x++) {
				idx = y * vga->pitch + x;
				vga->mem[idx] = term_bkgnd;
			}
		}
	}
}

void term_putc(uint32_t c)
{
	uint8_t set, *bitmap;
	uint32_t *at, pitch, x, y;
	size_t idx, i, j, k;
	struct vga *vga;

	if (c == '\n') {
		term_col = 0;
		term_row++;
		return;
	}
	for (i = 0; i < term_nvga; i++) {
		for (j = 0; j < font.Chars; j++) {
			idx = j;
			if (c == font.Index[j])
				break;
		}
		bitmap = (uint8_t *)&font.Bitmap[idx * font.Height];
		vga = &term_vgas[i];
		pitch = vga->pitch;
		y = term_row * font.Height * pitch;
		x = term_col * font.Width;
		at = (uint32_t *)vga->mem + y + x;
		for (j = 0; j < font.Height; j++) {
			for (k = 0; k < 8; k++) {
				set = bitmap[j] & 1 << (7 - k);
				at[k] = set ? term_frgnd : term_bkgnd;
			}
			at += pitch;
		}
	}
	// TODO: scrolling.
	term_col++;
	if (term_col > TERM_WIDTH) {
		term_row++;
		term_col = 0;
	}
}
