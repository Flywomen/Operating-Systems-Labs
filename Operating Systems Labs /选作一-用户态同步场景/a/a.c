#include "apilib.h"
int time_waiting = 200000000;
int buffer_size = 5;
void HariMain(void)
{
	char s[35];
	struct SHAR * shar = (struct SHAR *)api_sharealloc("aaaa",26);//suc


	int in = api_getin(shar);
	int out = api_getout(shar);


	while(1){
		while((in + 1) % 5 == out){
			api_putstr1("\nThe store is full,waiting......\n",31);
			//sprintf(s," %x",shar);
			//api_putstr0(s);
			int data = 0;
			in = api_getin(shar);
			data = api_get(shar,in);
			sprintf(s," %d",data);
			api_putstr0(s);
			int i = 1;
			for(;i<time_waiting;i++);
			// api_set_time_waiting(100);
			out = api_getout(shar);
		}
		if(in==0){
			api_putstr0("a");
			api_change(shar,in,'a'); //97
		}
		if(in==1){
			api_putstr0("b");
			api_change(shar,in,'b');
		}
		if(in==2){
			api_putstr0("c");
			api_change(shar,in,'c');
		}
		if(in==3){
			api_putstr0("d");
			api_change(shar,in,'d');
		}
		if(in==4){
			api_putstr0("e");
			api_change(shar,in,'e');
		}

		int i =0;
		for(;i<time_waiting;i++);
		// api_set_time_waiting(100);

		in = (in + 1) % buffer_size;
		api_changein(shar,in);
		in = api_getin(shar);
		sprintf(s,"%d",in);
		api_putstr0(in);
	}


	// sprintf(s,"%x",shar);
	// api_putstr0(s);
	// api_putstr0("\n");
	//
	// api_change(shar,0,105);//suc
	// char ch = api_get(shar,0);//suc
	// sprintf(s," %c",ch);
	// api_putstr0(s);
	//api_changein(shar,98);//suc

	//int in = api_getin(shar);//suc
	//sprintf(s," %d",in);
	//api_putstr0(s);

	//api_changeout(shar,88);//suc
	//int out = api_getout(shar);//suc
	//sprintf(s," %d",out);
	//api_putstr0(s);

	api_end();
}
