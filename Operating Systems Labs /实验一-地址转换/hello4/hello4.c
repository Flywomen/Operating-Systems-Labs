#include "apilib.h"

void HariMain(void)
{
	static int a = 1;
	static char b = 'b';
	static char *c = "c";
	api_putstr0(a);
	//api_putstr0(&a);
	api_putstr0(b);
	api_putstr0(&b);
	api_putstr0(c);
	//api_putstr0("hello, world\n");
	api_end();
}
