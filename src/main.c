#include "boot.h"
#include "term.h"
#include "lib.h"

__attribute__((noreturn))
void _main(struct boot b)
{
	term_init(b.graphics.vgas, b.graphics.nvga);
	printf(
		"%s\n%s\n%s",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ 12345",
		"abcdefghijklmnopqrstuvwxyz 67890",
		"(){}[]<>$*-+=/#_%^@\\&|~?'\" !,.;:"
	);
	for (;;)
		__asm__("hlt");
}
