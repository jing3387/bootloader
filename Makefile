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

all: $(EFI)

$(EFI): $(DLL)
	$(OBJCOPY) --target=efi-app-$(ARCH) $< $@

$(DLL): $(SRC)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIB)

.PHONY: clean
clean:
	@rm -r build
