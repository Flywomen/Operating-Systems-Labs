#include "apilib.h"

void HariMain(void)
{
	static int a = 1;
	int addr = &a;
	api_showAddr(&a);
	while(a);
	api_end();
}