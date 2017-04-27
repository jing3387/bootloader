#include <stddef.h>
#include <stdbool.h>
#include <efi.h>
#include <efilib.h>

static CHAR16 *PROMPT = L"? ";
static UINTN LEN = 1024;
static CHAR16 *QUIT = L"quit";

EFI_STATUS efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	CHAR16 *input;

	InitializeLib(image, systab);
	InitializeUnicodeSupport("en");
	input = AllocateZeroPool(LEN * sizeof(CHAR16));
	while (true) {
		Input(PROMPT, input, LEN);
		if (StrCmp(input, QUIT) == 0)
			break;
		Output(L"\r\n");
		Output(input);
		Output(L"\r\n");
	}
	return EFI_SUCCESS;
}
