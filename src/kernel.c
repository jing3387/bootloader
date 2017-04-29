#include "boot.h"
#include "efi_graphics.h"

void kernel_main(struct efi efi)
{
	efi_fill_screen(efi.graphics, 0x00ff00);
}
