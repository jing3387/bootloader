#pragma once

#include "Uefi.h"
#include "Protocol/GraphicsOutput.h"

struct efi_memory_map {
	UINTN memory_map_size;
	EFI_MEMORY_DESCRIPTOR *memory_map;
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;
};

struct efi_graphics {
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gops;
	UINTN ngop;
};

struct efi {
	struct efi_memory_map memmap;
	struct efi_graphics graphics;
};
