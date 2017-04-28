#include "Uefi.h"
#include "boot.h"

EFI_STATUS efi_main(EFI_HANDLE ih, EFI_SYSTEM_TABLE *st)
{
	EFI_STATUS status;
	EFI_INPUT_KEY key;

	st->ConOut->OutputString(st->ConOut, L"Getting memory map.\n\r");
	struct memory_map mmap;
	status = get_memory_map(st, &mmap);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(st->ConOut, L"Error getting memory map.\n\r");
		return status;
	}
	st->ConOut->OutputString(st->ConOut, L"Got memory map.\n\r");
	st->ConOut->OutputString(st->ConOut, L"Exiting boot services.\n\r");
	status = st->BootServices->ExitBootServices(ih, mmap.map_key);
	if (EFI_ERROR(status)) {
		st->ConOut->OutputString(st->ConOut, L"Error exiting boot services.\n\r");
		return status;
	}
	return status;
}
