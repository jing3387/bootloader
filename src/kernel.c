#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "boot.h"
#include "graphics.h"

void kernel_main(struct efi efi)
{
	print(efi.graphics, L"Hello, World!");
}
