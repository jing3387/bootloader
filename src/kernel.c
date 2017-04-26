#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_LOADED_IMAGE *loaded_image = NULL;
	EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
	EFI_STATUS status;

	InitializeLib(image, systab);
	status = uefi_call_wrapper(
		systab->BootServices->HandleProtocol,
		3,
		image,
		&loaded_image_protocol,
		(void **) &loaded_image
	);
	if (EFI_ERROR(status)) {
		Print(L"handleprotocol: %r\n", status);
	}

	Print(L"Image base: %lx\n", loaded_image->ImageBase);
	Print(L"Image size: %lx\n", loaded_image->ImageSize);
	Print(L"Image file: %s\n", DevicePathToStr(loaded_image->FilePath));
	for (;;) __asm__("hlt");
}
