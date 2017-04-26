ARCH ?= x86_64

kernel := build/kernel-$(ARCH).bin
iso := build/schminke-$(ARCH).iso
linker_script := src/arch/$(ARCH)/linker.ld
grub_cfg := src/arch/$(ARCH)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(ARCH)/*.asm)
assembly_object_files := $(patsubst src/arch/$(ARCH)/%.asm, \
	build/arch/$(ARCH)/%.o, $(assembly_source_files))

all: $(kernel)

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso)

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/iso/boot/grub
	@cp $(kernel) build/iso/boot/kernel.bin
	@cp $(grub_cfg) build/iso/boot/grub
	@grub-mkrescue -o $(iso) build/iso 2> /dev/null
	@rm -r build/iso

$(kernel): $(assembly_object_files) $(linker_script)
	@ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files)

build/arch/$(ARCH)/%.o: src/arch/$(ARCH)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@

.PHONY: clean
clean:
	@rm -r build
