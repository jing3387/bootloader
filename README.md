# Schminke
Schminke, affectionately pronounced "schmin-key" though actually pronounced
"schmin-kuh", is my idea of a minimal OS and language. It's planned to be a
unikernel featuring the Schminke language (a derivate of Scheme), a single
64-bit address space, and a flat file system.

## Dependencies

### Manjaro (Arch Linux)
* `clang`
* `base-devel`
* `gnu-efi-libs`
* `ovmf`
* `mtools`
* `qemu`

## Building
* `make` to build the image that can be run off a USB.
* `make run` to run the image under QEMU.
* `make clean` to remove the `build/` directory.

## Usage

### Interacting with the monitor (QEMU)
Schminke runs in `-nographics` mode under QEMU. This allows the output of the
console to go directly to your terminal. There are some keybindings installed
so that you can interact with the monitor in a manner similar to GNU Screen.
These keybindings are:

```
C-a h    print this help
C-a x    exit emulator
C-a s    save disk data back to file (if -snapshot)
C-a t    toggle console timestamps
C-a b    send break (magic sysrq)
C-a c    switch between console and monitor
C-a C-a  sends C-a
```

### Interacting with Schminke
After booting Schminke you'll be placed at a prompt. For now you can input some
text and that text will be echo'd back. To quit the just enter `quit`.

The plan is to have a REPL for the Schminke language that acts as a shell.
