ARCH = x86_64

DLL = build/schminke.dll
EFI = build/BOOTX64.efi
IMG = build/schminke.img
SRC = $(wildcard src/*.c)

QEMU_OPTS = -nographic \
	    -drive if=pflash,format=raw,readonly,file=ovmf_code_x64.bin \
	    -drive if=pflash,format=raw,file=ovmf_vars_x64.bin

CC = x86_64-w64-mingw32-gcc
CFLAGS = -shared -nostdlib -mno-red-zone -fno-stack-protector -Wall \
         -e EfiMain
OBJCOPY = x86_64-w64-mingw32-objcopy

all: $(EFI)

run: $(IMG)
	qemu-system-$(ARCH) $(QEMU_OPTS) -usb -usbdevice disk::$<

$(IMG): $(EFI)
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $< ::/EFI/BOOT

$(EFI): $(DLL)
	$(OBJCOPY) --target=efi-app-$(ARCH) $< $@

$(DLL): $(SRC)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	@rm -r build
