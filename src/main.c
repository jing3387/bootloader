#include "boot.h"
#include "term.h"

__attribute__((noreturn))
void _main(struct boot b)
{
	term_init(b.graphics.vgas, b.graphics.nvga);
	term_putc(u'λ');
	term_putc(u'\n');
	term_putc(u'μ');
	for (;;)
		__asm__("hlt");
}
