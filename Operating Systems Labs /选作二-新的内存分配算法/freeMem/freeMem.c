#include "apilib.h"

void HariMain(void)
{
	char *a[6] = {0};
	api_initmalloc();
	a[0] = api_malloc(256);
	a[1] = api_malloc(512);
	a[2] = api_malloc(128);
	a[3] = api_malloc(64);
	a[4] = api_malloc(128);
	a[5] = api_malloc(1024);
	api_free(a[1], 512);
	api_free(a[3], 64);
	api_free(a[5], 1024);
	api_printFreeMem();
	char *b = api_malloc(48);
	api_putstr0("--After malloc--\n");
	api_printFreeMem();
	api_end();
}
