# Schminke
Schminke, affectionately pronounced "schmin-key" though actually pronounced
"schmin-kuh", is my idea of a minimal OS and language. It's planned to be a
unikernel featuring the Schminke language (a derivate of Scheme) and a single
64-bit address space. Currently Schminke only builds for x86_64.

## Dependencies
* `mingw-w64`

### Development
When developing Schminke you will probably want to run it under a virtual
machine. All development of Schminke is done using QEMU, therefore we have the
following dependencies:

* `qemu`
* `mtools`
* `ovmf`

#### OVMF
`ovmf` is a part of the `edk2` project. You can download a version of `ovmf`
from the [Arch linux package
mirror](https://www.archlinux.org/packages/extra/any/ovmf/download/). After
downloading from the mirror extract the archive with `tar xJf` and place the
contents of `/usr/share/ovmf` into the base directory of the Schminke repo.
Please take note of `License.txt` under `/usr/share/licenses/ovmf`. You could
also extract from the off [official
RPMs](https://www.kraxel.org/repos/jenkins/edk2/) but I find the Arch linux
package mirror to be easier.

## Building
* `make` to build the image that can be run off a USB.
* `make run` to run the image under QEMU.
* `make clean` to remove the `build/` directory.

### Creating a bootable USB
Simply format your USB to be FAT32 and have the following structure:

```
.
└── EFI
    └── BOOT
            └── BOOTX64.EFI
```

That is, create the directory `/EFI/BOOT` and place the result of `make`,
namely `BOOTX64.EFI`, in the `/EFI/BOOT` directory

#### Macintosh
To use the bootable USB on a Macintosh you'll first have to install
[rEFInd](http://www.rodsbooks.com/refind/); its suggested you also install
[rEFInd Minimal](https://evanpurkhiser.com/rEFInd-minimal/) because it looks
good. If you're using a current version of macOS you'll have to [disable
SIP](http://www.rodsbooks.com/refind/sip.html#disable_in_osx) first by entering
recovery mode (Command-R at boot), bringing up a terminal, entering `csrutils
disable`, and restarting.

## Usage

### Interacting with the QEMU monitor
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
