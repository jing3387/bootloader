ARCH = x86_64

ISO = build/schminke.iso
IMG = build/schminke.img
EFI = build/BOOTX64.efi
DLL = build/schminke.dll
SRC = $(wildcard src/*.c)

prefix=x86_64-w64-mingw32-
CC = $(prefix)gcc
OBJCOPY = $(prefix)objcopy

INCDIR = inc
EDK2 = $(INCDIR)/edk2/MdePkg/Include
INC = -I$(INCDIR) -I$(EDK2) -I$(EDK2)/X64

CFLAGS = -mno-red-zone -fno-stack-protector -Wall $(INC)
LDFLAGS = -shared -nostdlib -Wl,--subsystem,10 -e efi_main

OVMF_CODE = /usr/share/ovmf/ovmf_code_x64.bin
OVMF_VARS = build/ovmf_vars_x64.bin
QEMU_OPTS = -enable-kvm -m 1G \
	    -drive if=pflash,format=raw,readonly,file=$(OVMF_CODE) \
	    -drive if=pflash,format=raw,file=$(OVMF_VARS)

all: $(EFI)

iso: $(ISO)

run: $(ISO) $(OVMF_VARS)
	qemu-system-$(ARCH) $(QEMU_OPTS) -cdrom $(ISO)

$(ISO): $(IMG)
	mkdir -p build/iso
	cp $< build/iso
	xorriso -as mkisofs -R -f -e $(shell basename $<) -no-emul-boot \
		-o $@ build/iso

$(IMG): $(EFI)
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

$(OVMF_VARS):
	cp /usr/share/ovmf/ovmf_vars_x64.bin $@

$(EFI): $(DLL)
	$(OBJCOPY) --target=efi-app-$(ARCH) $< $@

$(DLL): $(SRC)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIB)

.PHONY: clean
clean:
	@rm -rf build
