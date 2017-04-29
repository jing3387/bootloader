ARCH = x86_64

IMG = build/schminke.img
EFI = build/BOOTX64.efi
DLL = build/schminke.dll
SRC = $(wildcard src/*.c)

prefix=x86_64-w64-mingw32-
CC = $(prefix)gcc
OBJCOPY = $(prefix)objcopy

EDK2 = edk2/MdePkg/Include
INCDIR = inc
INC = -I$(INCDIR) -I$(EDK2) -I$(EDK2)/X64
CFLAGS = -mno-red-zone -fno-stack-protector -Wall $(INC)
LDFLAGS = -shared -nostdlib -Wl,--subsystem,10 -e efi_main

OVMF_BIOS = build/OVMF-pure-efi.fd
QEMU_OPTS = -pflash $(OVMF_BIOS)

all: $(EFI)

run: $(EFI) $(OVMF_BIOS)
	mkdir -p build/EFI/BOOT
	cp $(EFI) build/EFI/BOOT
	qemu-system-$(ARCH) $(QEMU_OPTS) -hda fat:rw:build

$(OVMF_BIOS):
	cp /usr/share/edk2.git/ovmf-x64/OVMF-pure-efi.fd $@

$(EFI): $(DLL)
	$(OBJCOPY) --target=efi-app-$(ARCH) $< $@

$(DLL): $(SRC)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIB)

.PHONY: clean
clean:
	@rm -r build
