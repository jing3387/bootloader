#pragma once

#include <stdint.h>

enum vga_color {
	VGA_BLACK = 0xFF000000,
	VGA_ACME = 0xFFFFFFEA
};

struct vga {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t *mem;
};
