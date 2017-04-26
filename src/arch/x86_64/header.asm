magic equ 0xe85250d6
arch equ 0
header_len equ header_end - header_start
checksum equ 0x100000000 - (magic + arch + header_len)
type equ 0
flags equ 0
size equ 8

section .header
header_start:
        dd magic
        dd arch
        dd header_len
        dd checksum

        ; Insert optional multiboot tags here.

        ; Mandatory end tag.
        dw type
        dw flags
        dd size
header_end:
