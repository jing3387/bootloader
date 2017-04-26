ARCH = x86_64

KERNEL := build/kernel
KSO := $(KERNEL).so
EFI := build/BOOTX64.efi
IMG := $(KERNEL).img

INC := $(wildcard src/*.h)
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, build/%.o, $(SRC))

LIBDIR := /usr/lib
LIB := -lefi -lgnuefi

EFIINC        = /usr/include/efi
EFIINCS       = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFI_CRT_OBJ   = /usr/lib/crt0-efi-$(ARCH).o
EFI_LDS       = /usr/lib/elf_$(ARCH)_efi.lds
OVMF          = /usr/share/ovmf/ovmf_code_x64.bin

QEMU_OPTS     = -enable-kvm -m 64 -device VGA

CC = gcc
CFLAGS = -c -fno-stack-protector -fpic -fshort-wchar -mno-red-zone $(EFIINCS)
ifeq ($(ARCH),x86_64)
	CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LD = ld
LDFLAGS = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic \
	  -L $(LIBDIR)

all: $(IMG)

run: $(IMG)
	qemu-system-$(ARCH) -bios $(OVMF) -usb -usbdevice disk::$<

$(IMG): $(EFI)
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

$(EFI): $(KSO)
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
		-j .rela -j .reloc -j .eh_frame --target=efi-app-$(ARCH) $< $@

$(KSO): $(OBJ) $(EFI_CRT_OBJ)
	ld $(LDFLAGS) $^ $(LIB) -o $@

build/%.o: src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	@rm -r build
