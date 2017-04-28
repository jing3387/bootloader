# Schminke
Schminke, affectionately pronounced "schmin-key" though actually pronounced
"schmin-kuh", is my idea of a minimal OS and language. It's planned to be a
unikernel featuring the Schminke language (a derivate of Scheme) and a single
64-bit address space. Currently Schminke only builds for x86_64.

## Dependencies
* `mingw-w64`

## Building
Run `make` to build the image that can be run off a USB.

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
