#include "apilib.h"
// char products[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
// 'q','r','s','t','u','v','w','x','y','z'};
int buffer_size = 5;
int time_waiting = 20000000;
void HariMain(void)
{
	char s[100];
	struct SHAR * shar = (struct SHAR *)api_sharealloc("aaaa",26);//suc

	int in = api_getin(shar);
	int out = api_getout(shar);
	while(1){
		while(in == out){
			// sprintf(s," %d",in);
			// api_putstr0(s);
			api_putstr1("\nThe store is empty,waiting......\n",32);
			int i =0;
			for(;i<time_waiting;i++);
			// api_set_time_waiting(100);
			in = api_getin(shar);
		}

		int out = api_getout(shar);
		int data = api_get(shar,out);
		sprintf(s," %d", data);
		// api_putstr0(s);
		api_putstr1("\nThe consumer is eating......\n",32);
		int j =0;
		for(;j<time_waiting;j++);
		// api_set_time_waiting(100);
		out = (out + 1) % buffer_size;
		api_changeout(shar,out);
		api_change(shar,out,0);
		//out = api_getout(shar);
	}
	api_end();
}
