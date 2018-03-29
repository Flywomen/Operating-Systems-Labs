/* メモリ関係 */

#include "bootpack.h"

#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;

	/* 386か、486以降なのかの確認 */
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ((eflg & EFLAGS_AC_BIT) != 0) { /* 386ではAC=1にしても自動で0に戻ってしまう */
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; /* キャッシュ禁止 */
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; /* キャッシュ許可 */
		store_cr0(cr0);
	}

	return i;
}

void memman_init(struct MEMMAN *man)
{
	man->frees = 0;			/* あき情報の個数 */
	man->maxfrees = 0;		/* 状況観察用：freesの最大値 */
	man->lostsize = 0;		/* 解放に失敗した合計サイズ */
	man->losts = 0;			/* 解放に失敗した回数 */
	return;
}

unsigned int memman_total(struct MEMMAN *man)
/* あきサイズの合計を報告 */
{
	unsigned int i, t = 0;
	for (i = 0; i < man->frees; i++) {
		t += man->free[i].size;
	}
	return t;
}

//man->free sort by address
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size)
/* 確保 */
{
	int choice = 2;
	unsigned int i, a, addr, tar, maxOff;
	int mark = 0;
	switch(choice)
	{
		case 0://First-fit
		{
			for (i = 0; i < man->frees; i++) {
				if (man->free[i].size >= size)
				{
					a = man->free[i].addr;
					man->free[i].addr += size;
					man->free[i].size -= size;
					if (man->free[i].size == 0)
					{
						man->frees--;
						for (; i < man->frees; i++)
						{
							man->free[i] = man->free[i + 1];
						}
					}
					return a;
				}
			}
			break;
		}	
		case 1://Best-fit
		{
			for (i = 0; i < man->frees; ++i)
			{
				if (man->free[i].size == size)
				{
					addr = man->free[i].addr;
					man->frees--;
					for (; i < man->frees && (i + 1 < MEMMAN_FREES); ++i)
					{
						man->free[i] = man->free[i + 1];
					}
					return addr;
				}
				else if (man->free[i].size > size && (mark == 0))
				{
					a = i;
					mark = 1;
				}
			}
			if (mark == 1)
			{
				addr = man->free[a].addr;
				man->free[a].addr += size;
				man->free[a].size -= size;
				return addr;
			}
			break;	
		}
		case 2://Worst-fit
		{
			tar = maxOff = 0;
			for (i = 0; i < man->frees; i++)
			{
				if (man->free[i].size >= size)
				{
					if (man->free[i].size - size >= maxOff)
					{
						maxOff = man->free[i].size - size;
						tar = i;
						mark = 1;
					}
				}
			}
			if (mark == 1)
			{
				a = man->free[tar].addr;
				man->free[tar].addr += size;
				man->free[tar].size -= size;
				if (man->free[tar].size == 0)
				{
					man->frees--;
					for (; tar < man->frees && tar + 1 < MEMMAN_FREES; tar++)
					{
						man->free[tar] = man->free[tar + 1];
					}
				}
				return a;
			}
			break;	
		}
		default:
			break;
	}
	return 0; /* あきがない */
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
/* 解放 */
{
	int i, j;
	
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	/* free[i - 1].addr < addr < free[i].addr */
	if (i > 0) {
		/* 前がある */
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			/* 前のあき領域にまとめられる */
			man->free[i - 1].size += size;
			if (i < man->frees) {
				/* 後ろもある */
				if (addr + size == man->free[i].addr) {
					/* なんと後ろともまとめられる */
					man->free[i - 1].size += man->free[i].size;
					/* man->free[i]の削除 */
					/* free[i]がなくなったので前へつめる */
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1]; /* 構造体の代入 */
					}
				}
			}
			return 0; /* 成功終了 */
		}
	}
	/* 前とはまとめられなかった */
	if (i < man->frees) {
		/* 後ろがある */
		if (addr + size == man->free[i].addr) {
			/* 後ろとはまとめられる */
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; /* 成功終了 */
		}
	}
	/* 前にも後ろにもまとめられない */
	if (man->frees < MEMMAN_FREES) {
		/* free[i]より後ろを、後ろへずらして、すきまを作る */
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees; /* 最大値を更新 */
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; /* 成功終了 */
	}
	/* 後ろにずらせなかった */
	man->losts++;
	man->lostsize += size;
	return -1; /* 失敗終了 */
}

unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size)
{
	unsigned int a;
	size = (size + 0xfff) & 0xfffff000;
	a = memman_alloc(man, size);
	return a;
}

int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size)
{
	int i;
	size = (size + 0xfff) & 0xfffff000;
	i = memman_free(man, addr, size);
	return i;
}
