/* bootpack�̃��C�� */

#include "bootpack.h"
#include <stdio.h>

#define MAX 100	//?��?������??�Z�I����
#define MAX_PROCESS 5//�ő�I�������꘢?�ʓI?������
#define MAX_SHARE 100//�n?�\�񋟓I�ő勤��?�ʘ���
#define MAX_NAME 20

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);

void task_b_main(struct SHEET *sht_back, struct TASK * task);
void task_c_main(struct SHEET *sht_back, struct TASK * task);
void task_d_main(struct SHEET *sht_back, struct TASK * task);

//struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
void ini_sharctl();
struct SHAR * shar_alloc(char * name);
void share_release(struct SHAR * share);

struct SHAR{
	int flag;//�\��?�ʐ���?��������?
	//int address[MAX_PROCESS];//�e��?���I�n��
	char name[MAX_NAME];//����?�ʓI����
	int shared;//�����I?��
	//int sharept;//��shares0���I��?
	int share_num;
	unsigned char choosing[MAX_PROCESS];
	int number[MAX_PROCESS];
};

struct SHARCTL{
	struct SHAR shares0[MAX_SHARE];
	struct SHAR * shares[MAX_SHARE];//�w��shares0�I�w?
	int using;//?�݈ꋤ��������������?��
};

struct SHARCTL sharctl;

//���n������?��
void ini_sharctl(){
	int i,j;
	for(i=0;i<MAX_SHARE;++i){
		sharctl.shares0[i].flag=0;//?���v�g�p��?
		sharctl.shares0[i].shared=0;//�w????��
		//sharctl.shares0[i].sharept=i;
		for(j=0;j<MAX_PROCESS;++j)
		{
			//sharctl.shares0[i].address[j]=-1;
			sharctl.shares0[i].choosing[j]=0;
			sharctl.shares0[i].number[j]=0;
		}
		sharctl.shares0[i].name[0]='\0';
		sharctl.shares0[i].share_num=0;
	}
	sharctl.using=0;
}

int namecmp(char *name1,char * name2)
{
	int i=0;
	for(i=0;name1[i]!='\0'&&name2[i]!='\0'&&name1[i]==name2[i];++i)
		;
	if(name1[i]=='\0'&&name2[i]=='\0')
		return 1;
	return 0;
}

//���z����?�ʓ����C�Ȗ��̍�?�敪�I����
struct SHAR * shar_alloc(char * name)
{
	int i,j;
	if(sharctl.using==0)
	{
		for(j=0;j<MAX_NAME;++j)
			sharctl.shares0[0].name[j]=name[j];
		sharctl.shares0[0].flag=1;//???���ݎg�p
		sharctl.shares[sharctl.using] = &(sharctl.shares0[0]);
		sharctl.using++;
		sharctl.shares0[0].share_num++;
		return sharctl.shares[sharctl.using-1];
	}
	else if(sharctl.using==MAX_SHARE)
		return 0;
	else
	{
		for(i=0;i<sharctl.using;++i)
			if(namecmp(sharctl.shares[i]->name,name))//���f���̐��ۑ���
			{
				sharctl.shares[i]->share_num++;
				return (sharctl.shares[i]);//�@�ʝQ�������I?��???��??����??�ʁC��SHAR�w?�ԉ�
			}
		for(i=0;i<MAX_SHARE;++i)//�v�L�Q�������I?��??��
			if(sharctl.shares0[i].flag==0)
			{
				for(j=0;j<MAX_NAME;++j)
					sharctl.shares0[i].name[j]=name[j];
				sharctl.shares0[i].flag=1;//???���ݎg�p
				sharctl.shares[sharctl.using] = &(sharctl.shares0[i]);
				sharctl.using++;
				sharctl.shares[sharctl.using-1]->share_num++;
				return sharctl.shares[sharctl.using-1];
			}		
	}
}

void share_release(struct SHAR * share){
	int i=0,j;
	for(i=0;i < sharctl.using;++i)
		if(share==sharctl.shares[i])
		{
			share->flag=0;
			share->name[0]='\0';
			share->share_num=0;
			for(j=i;j<sharctl.using-1;++j)
				sharctl.shares[j]=sharctl.shares[j+1];
			(sharctl.using)--;
		}
}

int max(int *array,int num){
	int i=0;
	int result = -1;
	for(i=0;i<num;++i)
		if(result<array[i])
			result=array[i];
	return result;
	
}


void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FIFO32 fifo;
	char s[40];
	int fifobuf[128];
	int mx, my, i, cursor_x, cursor_c;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	
	
	struct SHTCTL *shtctl;
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.'
	};
	unsigned char *buf_back, buf_mouse[256], *buf_win, *buf_win_b;
	struct SHEET *sht_back, *sht_mouse, *sht_win, *sht_win_b[3];
	struct TASK *task_a, *task_b[3];
	struct TIMER *timer;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC�̏��������I������̂�CPU�̊��荞�݋֎~������ */
	fifo32_init(&fifo, 128, fifobuf, 0);
	init_pit();
	
	ini_sharctl();
	
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);
	io_out8(PIC0_IMR, 0xf8); /* PIT��PIC1�ƃL�[�{�[�h������(11111000) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */

	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	init_palette();
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	task_a = task_init(memman);
	fifo.task = task_a;

	/* sht_back */
	sht_back  = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* �����F�Ȃ� */
	init_screen8(buf_back, binfo->scrnx, binfo->scrny);

	/* sht_win_b */
	int position=0x0000f000;
	*((int *)position)=0;
	
	for (i = 0; i < 3; i++) {
		sht_win_b[i] = sheet_alloc(shtctl);
		buf_win_b = (unsigned char *) memman_alloc_4k(memman, 144 * 52);
		sheet_setbuf(sht_win_b[i], buf_win_b, 144, 52, -1); /* �����F�Ȃ� */
		sprintf(s, "task_b%d", i);
		make_window8(buf_win_b, 144, 52, s, 0);
		task_b[i] = task_alloc();
		task_b[i]->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 8;
		if(i==0)
			task_b[i]->tss.eip = (int) &task_b_main;
		else if(i==1)
			task_b[i]->tss.eip = (int) &task_c_main;
		else
			task_b[i]->tss.eip = (int) &task_d_main;
		task_b[i]->tss.es = 1 * 8;
		task_b[i]->tss.cs = 2 * 8;
		task_b[i]->tss.ss = 1 * 8;
		task_b[i]->tss.ds = 1 * 8;
		task_b[i]->tss.fs = 1 * 8;
		task_b[i]->tss.gs = 1 * 8;
		//*((int *) (task_b[i]->tss.esp + 4)) = (int) sht_win_b[i];
		*((int *) (task_b[i]->tss.esp+4)) = (int) sht_win_b[i];
		*((int *) (task_b[i]->tss.esp+8)) = (int)task_b[i];
		task_run(task_b[i]);
	}

	/* sht_win */
	sht_win   = sheet_alloc(shtctl);
	buf_win   = (unsigned char *) memman_alloc_4k(memman, 160 * 52);
	sheet_setbuf(sht_win, buf_win, 144, 52, -1); /* �����F�Ȃ� */
	make_window8(buf_win, 144, 52, "task_a", 1);
	make_textbox8(sht_win, 8, 28, 128, 16, COL8_FFFFFF);
	cursor_x = 8;
	cursor_c = COL8_FFFFFF;
	timer = timer_alloc();
	timer_init(timer, &fifo, 1);
	timer_settime(timer, 50);

	/* sht_mouse */
	sht_mouse = sheet_alloc(shtctl);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	init_mouse_cursor8(buf_mouse, 99);
	mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (binfo->scrny - 28 - 16) / 2;

	sheet_slide(sht_back, 0, 0);
	sheet_slide(sht_win_b[0], 168,  56);
	sheet_slide(sht_win_b[1],   8, 116);
	sheet_slide(sht_win_b[2], 168, 116);
	sheet_slide(sht_win,        8,  56);
	sheet_slide(sht_mouse, mx, my);
	sheet_updown(sht_back,     0);
	sheet_updown(sht_win_b[0], 1);
	sheet_updown(sht_win_b[1], 2);
	sheet_updown(sht_win_b[2], 3);
	sheet_updown(sht_win,      4);
	sheet_updown(sht_mouse,    5);
	sprintf(s, "(%3d, %3d)", mx, my);
	putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);
	sprintf(s, "memory %dMB   free : %dKB",
			memtotal / (1024 * 1024), memman_total(memman) / 1024);
	putfonts8_asc_sht(sht_back, 0, 32, COL8_FFFFFF, COL8_008484, s, 40);

	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			task_sleep(task_a);
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (256 <= i && i <= 511) { /* �L�[�{�[�h�f�[�^ */
				sprintf(s, "%02X", i - 256);
				putfonts8_asc_sht(sht_back, 0, 16, COL8_FFFFFF, COL8_008484, s, 2);
				if (i < 0x54 + 256) {
					if (keytable[i - 256] != 0 && cursor_x < 128) { /* �ʏ핶�� */
						/* �ꕶ���\�����Ă���A�J�[�\����1�i�߂� */
						s[0] = keytable[i - 256];
						s[1] = 0;
						putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, s, 1);
						cursor_x += 8;
					}
				}
				if (i == 256 + 0x0e && cursor_x > 8) { /* �o�b�N�X�y�[�X */
					/* �J�[�\�����X�y�[�X�ŏ����Ă���A�J�[�\����1�߂� */
					putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, " ", 1);
					cursor_x -= 8;
				}
				/* �J�[�\���̍ĕ\�� */
				boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
				sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
			} else if (512 <= i && i <= 767) { /* �}�E�X�f�[�^ */
				if (mouse_decode(&mdec, i - 512) != 0) {
					/* �f�[�^��3�o�C�g�������̂ŕ\�� */
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0) {
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0) {
						s[3] = 'R';
					}
					if ((mdec.btn & 0x04) != 0) {
						s[2] = 'C';
					}
					putfonts8_asc_sht(sht_back, 32, 16, COL8_FFFFFF, COL8_008484, s, 15);
					/* �}�E�X�J�[�\���̈ړ� */
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 1) {
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1) {
						my = binfo->scrny - 1;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);
					sheet_slide(sht_mouse, mx, my);
					if ((mdec.btn & 0x01) != 0) {
						/* ���{�^���������Ă�����Asht_win�𓮂��� */
						sheet_slide(sht_win, mx - 80, my - 8);
					}
				}
			} else if (i <= 1) { /* �J�[�\���p�^�C�} */
				if (i != 0) {
					timer_init(timer, &fifo, 0); /* ����0�� */
					cursor_c = COL8_000000;
				} else {
					timer_init(timer, &fifo, 1); /* ����1�� */
					cursor_c = COL8_FFFFFF;
				}
				timer_settime(timer, 50);//?�u??��I?�u�C�l???�I���x
				boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
				sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
			}
		}
	}
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act)
{
	static char closebtn[14][16] = {
		"OOOOOOOOOOOOOOO@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQQQ@@QQQQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"O$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@"
	};
	int x, y;
	char c, tc, tbc;
	if (act != 0) {
		tc = COL8_FFFFFF;
		tbc = COL8_000084;
	} else {
		tc = COL8_C6C6C6;
		tbc = COL8_848484;
	}
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         xsize - 1, 0        );
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         xsize - 2, 1        );
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         0,         ysize - 1);
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         1,         ysize - 2);
	boxfill8(buf, xsize, COL8_848484, xsize - 2, 1,         xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, xsize - 1, 0,         xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_C6C6C6, 2,         2,         xsize - 3, ysize - 3);
	boxfill8(buf, xsize, tbc,         3,         3,         xsize - 4, 20       );
	boxfill8(buf, xsize, COL8_848484, 1,         ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, 0,         ysize - 1, xsize - 1, ysize - 1);
	putfonts8_asc(buf, xsize, 24, 4, tc, title);
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = COL8_000000;
			} else if (c == '$') {
				c = COL8_848484;
			} else if (c == 'Q') {
				c = COL8_C6C6C6;
			} else {
				c = COL8_FFFFFF;
			}
			buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
	return;
}

void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
	sheet_refresh(sht, x, y, x + l * 8, y + 16);
	return;
}

void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	boxfill8(sht->buf, sht->bxsize, COL8_848484, x0 - 2, y0 - 3, x1 + 1, y0 - 3);
	boxfill8(sht->buf, sht->bxsize, COL8_848484, x0 - 3, y0 - 3, x0 - 3, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_FFFFFF, x0 - 3, y1 + 2, x1 + 1, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_FFFFFF, x1 + 2, y0 - 3, x1 + 2, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_000000, x0 - 1, y0 - 2, x1 + 0, y0 - 2);
	boxfill8(sht->buf, sht->bxsize, COL8_000000, x0 - 2, y0 - 2, x0 - 2, y1 + 0);
	boxfill8(sht->buf, sht->bxsize, COL8_C6C6C6, x0 - 2, y1 + 1, x1 + 0, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_C6C6C6, x1 + 1, y0 - 2, x1 + 1, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, c,           x0 - 1, y0 - 1, x1 + 0, y1 + 0);
	return;
}

/*
//����?�s�I?�ʗ^?��I?�s?����?�C���ȕs��?��?������
void task_b_main(struct SHEET *sht_win_b,struct TASK * task)
{
	char s[12];
	
	int j=0;

	int position = 0x0000f000;

	for (;;) {
		//count++;
		if(j<MAX){
		//++*((int *)position);
		myadd(position);
		++j;
		}
		if(j==MAX){
			sprintf(s,"%11d",*((int *)position));
			putfonts8_asc_sht(sht_win_b, 24, 28, COL8_000000, COL8_C6C6C6, s, 11);
			//break;
			task_sleep(task);
			//break;
		}
		
		
	}
}

void task_c_main(struct SHEET *sht_win_b,struct TASK * task)
{
	char s[12];
	
	int j=0;

	int position = 0x0000f000;

	for (;;) {
		//count++;
		if(j<MAX){
		//++*((int *)position);
		mysub(position);
		++j;
		}
		if(j==MAX){
			sprintf(s,"%11d",*((int *)position));
			putfonts8_asc_sht(sht_win_b, 24, 28, COL8_000000, COL8_C6C6C6, s, 11);
			//break;
			task_sleep(task);
			//break;
		}
		}
	}
*/




void task_b_main(struct SHEET *sht_win_b,struct TASK * task)
{
	char s[12];
	int j=0,k;
	struct SHAR * shar_b = shar_alloc("sharedint");
	shar_b->shared=0;
	int * position=&(shar_b->shared);//������?�ʓI�n���ۑ��N���C�X�e��?��?������
	for (;;) {
		
		if(j<MAX){
			//?����
			
			shar_b->choosing[0]=1;
			shar_b->number[0]=max(shar_b->number,2)+1;
			shar_b->choosing[0]=0;
			for(k=0;k<2;++k){
				while(shar_b->choosing[k]);
				while(shar_b->number[k]!=0&&(shar_b->number[k] < shar_b->number[0]||(shar_b->number[k]==shar_b->number[0]&&k<0)));
				}

			myadd(position);//��?�������?�E���
			shar_b->number[0]=0;
			
			//myadd(position);//��?�������?�E���
			++j;
		}
		if(j==MAX){
			
			sprintf(s,"%11d",(shar_b->shared));
			putfonts8_asc_sht(sht_win_b, 24, 28, COL8_000000, COL8_C6C6C6, s, 11);
		}
	}
}
void task_c_main(struct SHEET *sht_win_b,struct TASK * task)
{
	char s[12];
	int j=0,k;
	//���z����?��
	struct SHAR * shar_c = shar_alloc("sharedint");
	int *position = &(shar_c->shared);
	for (;;) {
		
		if(j<MAX){
			//?����
			
			shar_c->choosing[1]=1;
			shar_c->number[1]=max(shar_c->number,2)+1;
			shar_c->choosing[1]=0;
			for(k=0;k<2;++k){
				while(shar_c->choosing[k]);
				while(shar_c->number[k]!=0&&(shar_c->number[k]<shar_c->number[1]||(shar_c->number[k]==shar_c->number[1]&&k<1)));
				}
			mysub(position);//��?�������?�E���
			shar_c->number[1]=0;
			
			//mysub(position);//��?�������?�E���
			++j;
		}
		if(j==MAX){
			
			sprintf(s,"%11d",(shar_c->shared));
			putfonts8_asc_sht(sht_win_b, 24, 28, COL8_000000, COL8_C6C6C6, s, 11);
		}
		
	}
}

void task_d_main(struct SHEET *sht_win_b,struct TASK * task)
{

	char s[12];
	int j=0,k;
	//���z����?��
	struct SHAR * shar_c = shar_alloc("sharedint");
	int *position = &(shar_c->shared);
	for (;;) {
		
		if(j<MAX){
			//?����
			
			shar_c->choosing[2]=1;
			shar_c->number[2]=max(shar_c->number,2)+1;
			shar_c->choosing[2]=0;
			for(k=0;k<3;++k){
				while(shar_c->choosing[k]);
				while(shar_c->number[k]!=0&&(shar_c->number[k]<shar_c->number[2]||(shar_c->number[k]==shar_c->number[2]&&k<2)));
				}
			myadd(position);//��?�������?�E���
			shar_c->number[2]=0;
			
			//myadd(position);//��?�������?�E���
			++j;
		}
		if(j==MAX){
			
			sprintf(s,"%11d",(shar_c->shared));
			putfonts8_asc_sht(sht_win_b, 24, 28, COL8_000000, COL8_C6C6C6, s, 11);
		}

	}
}

