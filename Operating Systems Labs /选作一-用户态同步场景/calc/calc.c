int buffer_size = 5;
int time_waiting = 100000000;
int data;
void HariMain(void)
{
	char s[100];
	struct SHAR * shar = (struct SHAR *)api_sharealloc("aaaa",26);//suc
	while(1){
		int i =0;
		for(;i<5;i++){
			data = api_get(shar,i);
			sprintf(s," %d",data);
			api_putstr0(s);
		}
		api_putstr0("\n");
		for(;i<time_waiting;i++);
	}

}
