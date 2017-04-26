# Schminke
Schminke, affectionately pronounced "schmin-key" though actually pronounced
"schmin-kuh", is my idea of a minimal OS and language. It's planned to be a
unikernel featuring the Schminke language (a derivate of Scheme), a single
64-bit address space, and a flat file system.

## Dependencies
* `nasm`
* `grub-mkrescue`
* `mformat` (included in `mtools`)
* `xorriso` (included in `libisoburn`)
* `qemu`

## Building
* `make`: construct the kernel.
* `make iso`: construct an ISO with a bootloader and the kernel.
* `make run`: run the resulting ISO using QEMU.

## Underlying works
Schminke is a derivative work of a number of projects. For now, each underlying
work has its `LICENSE`, and related files, copied verbatim below (minus
formatting).

### [Blog OS](https://github.com/phil-opp/blog_os)
```
The MIT License (MIT)

Copyright (c) 2015 Philipp Oppermann

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### [Solo5](https://github.com/solo5/solo5)

#### LICENSE
```
ISC License

Copyright (c) 2015-2017 Contributors as noted in the AUTHORS file

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
```

#### AUTHORS
```
Corporate Contributors
======================

Copyright (c) 2015-2017 IBM
Copyright (c) 2016-2017 Docker, Inc.

Individual Contributors
=======================

Adrian-Ken Rueegsegger
Dan Williams (IBM)
Gabriel Jaldon
Hannes Mehnert
Ian Campbell (Docker, Inc.)
Madhuri Yechuri
Martin Lucina (Docker, Inc.)
Michael LeMay
Ricardo Koller (IBM)
Waldir Pimenta
```
