#pragma once

#include <stddef.h>
#undef NULL
#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"
#include "vga.h"

struct efi_memory_map {
	UINTN memory_map_size;
	EFI_MEMORY_DESCRIPTOR *memory_map;
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;
};

struct graphics {
	struct vga *vgas;
	size_t nvga;
};

struct boot {
	struct efi_memory_map memmap;
	struct graphics graphics;
};
