#define MAX_PROCESS 5//最大的共享同一个?量的?程数量
#define MAX_NAME 20
struct SHAR{
	int flag;//表示?量是否?于共享状?
	int mode;//0 or 1,   0表示shared仅是一个整形变量，1表示shared为指针
	int inicialed;
	char name[MAX_NAME];//共享?量的名称
	int shared;//共享的?量
	//解决?争条件需要的数据??
	int share_num;
	unsigned char choosing[MAX_PROCESS];
	int number[MAX_PROCESS];
};
void api_putchar(int c);
void api_putstr0(char *s);
void api_putstr1(char *s, int l);
void api_end(void);
int api_openwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
void api_initmalloc(void);
char *api_malloc(int size);
void api_free(char *addr, int size);
void api_point(int win, int x, int y, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_linewin(int win, int x0, int y0, int x1, int y1, int col);
void api_closewin(int win);
int api_getkey(int mode);
int api_alloctimer(void);
void api_inittimer(int timer, int data);
void api_settimer(int timer, int time);
void api_freetimer(int timer);
void api_beep(int tone);
int api_fopen(char *fname);
void api_fclose(int fhandle);
void api_fseek(int fhandle, int offset, int mode);
int api_fsize(int fhandle, int mode);
int api_fread(char *buf, int maxsize, int fhandle);
int api_cmdline(char *buf, int maxsize);

int api_jingzheng(void);
struct SHAR * api_sharealloc(char *name,int mode);
char api_get(struct SHAR * shar,int position);
void api_change(struct SHAR * shar,int position,int ch);
int api_getin(struct SHAR *shar);
int api_getout(struct SHAR *shar);
void api_changein(struct SHAR *shar,int in);
void api_changeout(struct SHAR *shar,int out);//将out值覆盖掉原out值
void api_set_time_waiting(int time);