#pragma once

#include "boot.h"

void efi_set_pixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *, UINT32, UINT32, UINT32);

void efi_fill_screen(struct efi_graphics, UINT32);
