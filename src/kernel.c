#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"
#include "graphics.h"

void kernel_main(struct efi efi)
{
	putc(efi.graphics, 1, 1, u'μ');
}
