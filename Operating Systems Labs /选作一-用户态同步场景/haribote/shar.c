#include "bootpack.h"

struct SHARCTL{
	struct SHAR shares0[MAX_SHARE];
	struct SHAR * shares[MAX_SHARE];//ָ��shares0��ָ?
	int using;//?��һ�������˶����˸�?��
};

struct SHARCTL sharctl;

//��ʼ������?��
void ini_sharctl(){
	int i,j;
	for(i=0;i<MAX_SHARE;++i){
		sharctl.shares0[i].flag=0;//?��ûʹ��״?
		sharctl.shares0[i].shared=0;//ָ????��
		//sharctl.shares0[i].shared_ch=0;
		sharctl.shares0[i].inicialed=0;//��ʾû�г�ʼ��
		sharctl.shares0[i].mode=0;//��ʾΪ���α���
		sharctl.shares0[i].in=0;
		sharctl.shares0[i].out=0;
		for(j=0;j<MAX_PROCESS;++j)
		{
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

//���乲��?���ڴ棬��������?���ֵ�����
struct SHAR * shar_alloc(char * name,int mode)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	int i,j;
	if(sharctl.using==0)
	{
		for(j=0;j<MAX_NAME;++j)
			sharctl.shares0[0].name[j]=name[j];
		sharctl.shares0[0].flag=1;//???����ʹ��
		sharctl.shares[sharctl.using] = &(sharctl.shares0[0]);
		sharctl.using++;
		sharctl.shares0[0].share_num++;

		// if(mode>0)//��ʾΪ����
		// {
		// 	sharctl.shares[sharctl.using-1]->shared_ch = (char *)memman_alloc_4k(memman,mode+1);//����mode���ֽڵ�����
		// 	sharctl.shares[sharctl.using-1]->mode=mode;
		// }

		return sharctl.shares[sharctl.using-1];
	}
	else if(sharctl.using==MAX_SHARE)
		return 0;
	else
	{
		for(i=0;i<sharctl.using;++i)
			if(namecmp(sharctl.shares[i]->name,name))//�ж������Ƿ���ͬ
			{
				sharctl.shares[i]->share_num++;
				return (sharctl.shares[i]);//�����ҵ�ͬ����?��???��??����??������SHARָ?����
			}
		for(i=0;i<MAX_SHARE;++i)//û���ҵ�ͬ����?��??��
			if(sharctl.shares0[i].flag==0)
			{
				for(j=0;j<MAX_NAME;++j)
					sharctl.shares0[i].name[j]=name[j];
				sharctl.shares0[i].flag=1;//???����ʹ��
				sharctl.shares[sharctl.using] = &(sharctl.shares0[i]);
				sharctl.using++;
				sharctl.shares[sharctl.using-1]->share_num++;

			// if(mode>0)//��ʾΪ����
			// {
			// 	sharctl.shares[sharctl.using-1]->shared_ch = (char *)memman_alloc_4k(memman,mode+1);//����mode���ֽڵ�����
			// 	sharctl.shares[sharctl.using-1]->mode=mode;
			// }

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
			share->shared=0;
			share->mode=0;
			share->inicialed=0;
			for(j=i;j<sharctl.using-1;++j)
				sharctl.shares[j]=sharctl.shares[j+1];
			(sharctl.using)--;
		}
}


char get(struct SHAR *shar, int position){
	io_cli();
	return shar->shared_ch[position];
	io_sti();
}

void change(struct SHAR *shar, int position,int ch)
{
	io_cli();
	shar->shared_ch[position]=ch;
	io_sti();
}
int getin(struct SHAR *shar){
	//io_cli();
	return shar->in;
	//io_sti();
}
int getout(struct SHAR *shar){
	//io_cli();
	return shar->out;
	//io_sti();
}
void changein(struct SHAR *shar,int in){
	io_cli();
	shar->in = in;
	io_sti();
}
void changeout(struct SHAR *shar,int out)
{
	io_cli();
	shar->out = out;
	io_sti();
}
