global start

section .text
bits 32
start:
        mov esp, stack_top

        call check_multiboot
        call check_cpuid
        call check_long_mode

        ; Print 'OK' to the screen.
        mov dword [0xb8000], 0x2f4b2f4f
        hlt

; Prints `ERR: ` and the given error code to screen and hangs.
;
; Typically one will `jmp` to this function as it does not return.
;
; Parameter: error code (in ascii) in al
error:
        mov dword [0xb8000], 0x4f524f45
        mov dword [0xb8004], 0x4f3a4f52
        mov dword [0xb8008], 0x4f204f20
        mov byte  [0xb800a], al
        hlt

; Throw error 0 if eax doesn't contain the Multiboot 2 magic value
; (0x36d76289).
multiboot_magic equ 0x36d76289
check_multiboot:
        cmp eax, multiboot_magic
        jne .no_multiboot
        ret
.no_multiboot:
        mov al, "0"
        jmp error

; Throw error 1 if the CPU doesn't support the CPUID command.
check_cpuid:
        ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
        ; in the FLAGS register. If we can flip it, CPUID is available.

        ; Copy FLAGS in to EAX via stack
        pushfd
        pop eax
        ; Copy to ECX as well for comparing later on
        mov ecx, eax
        ; Flip the ID bit
        xor eax, 1 << 21
        ; Copy EAX to FLAGS via the stack
        push eax
        popfd
        ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
        pushfd
        pop eax
        ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
        ; ID bit back if it was ever flipped).
        push ecx
        popfd
        ; Compare EAX and ECX. If they are equal then that means the bit wasn't
        ; flipped, and CPUID isn't supported.
        cmp eax, ecx
        je .no_cpuid
        ret
.no_cpuid:
        mov al, "1"
        jmp error

; Throw error 2 if the CPU doesn't support Long Mode.
check_long_mode:
        ; Test if extended processor info is available.
        mov eax, 0x80000000
        cpuid
        cmp eax, 0x80000001
        jb .no_long_mode
        ; Use extended info to test if long mode is available.
        mov eax, 0x80000001
        cpuid
        test edx, 1 << 29
        jz .no_long_mode
        ret
.no_long_mode:
        mov al, "2"
        jmp error

section .bss
stack_bottom:
        resb 64
stack_top:
