/*
 * @Author: Z. Young
 * @Date: 2020-07-28 21:19:45
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 21:21:20
 * @Descripttion: 
 * @version: 
 */ 
/*
 * @Descripttion: 
 * @version: 
 * @Author: Z. Young
 * @Date: 2020-07-17 21:56:00
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-31 14:51:53
 */ 
#include "graphic.h"

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		/* C语言中的static char语句只能用于数据，相当于汇编中的DB指令 */
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:亮红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	unsigned char table2[216 * 3];
	int r, g, b;
	set_palette(0, 15, table_rgb);
	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
				table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
				table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
			}
		}
	}
	set_palette(16, 231, table2);
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 记录中断许可标志的值 */
	io_cli(); 					/* 将中断许可标志置为0,禁止中断 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 复原中断许可标志 */
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void init_screen8(char *vram, int x, int y, char *bkgname)
{
	int *fat;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	boxfill8(vram, x, COL8_008484, 0, 0, x - 1, y - 29);
	fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
	set_backbroung(fat, vram, x, y, bkgname);
	memman_free_4k(memman, (int) fat, 4 * 2880);
	
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);
}

int set_backbroung(int *fat, char *vram, int x, int y, char *bkgname)
/*设置桌面背景*/
{
	int i, j, x0, y0, fsize, info[4];
	unsigned char *filebuf, r, g, b;
	struct RGB *picbuf;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FILEINFO *finfo;
	struct DLL_STRPICENV *env;
	finfo = file_search(bkgname, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	/*读取图片文件*/
	if (finfo == 0) {
		return -1;
	}
	fsize = finfo->size;
	filebuf = (unsigned char *) memman_alloc_4k(memman, fsize);
	filebuf = file_loadfile2(finfo->clustno, &fsize, fat);
	env = (struct DLL_STRPICENV *) memman_alloc_4k(memman, sizeof(struct DLL_STRPICENV));
	info_JPEG(env, info, fsize, filebuf);
	picbuf = (struct RGB *) memman_alloc_4k(memman, info[2] * info[3] * sizeof(struct RGB));
	decode0_JPEG(env, fsize, filebuf, 4, (unsigned char *) picbuf, 0);
	x0 = (int) (x - info[2]);
	y0 = (int) (y - info[3]);
	
	for (i = 0; i < info[3]; i++) 
	{
		for (j = 0; j < info[2]; j++) 
		{
			r = picbuf[i * info[2] + j].r;
			g = picbuf[i * info[2] + j].g;
			b = picbuf[i * info[2] + j].b;
			vram[(y0 + i) * x + (x0 + j)] = rgb2pal(r, g, b, j, i, binfo->vmode);
		}
	}
	memman_free_4k(memman, (int) filebuf, fsize);
	memman_free_4k(memman, (int) picbuf , info[2] * info[3] * sizeof(struct RGB));
	memman_free_4k(memman, (int) env , sizeof(struct DLL_STRPICENV));
	return 0;
}
void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
/*在屏幕指定位置打印字符*/
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
}


void putfont32(char *vram, int xsize, int x, int y, char c, char *font1, char *font2)
{
	int i,k,j,f;
	char *p ;
	j=0;
	p=vram+(y+j)*xsize+x;
	j++;
	//上半部分
	for(i=0;i<16;i++)
	{
		for(k=0;k<8;k++)
		{
			if(font1[i]&(0x80>>k))
			{
				p[k+(i%2)*8]=c;
			}
		}
		for(k=0;k<8/2;k++)
		{
			f=p[k+(i%2)*8];
			p[k+(i%2)*8]=p[8-1-k+(i%2)*8];
			p[8-1-k+(i%2)*8]=f;
		}
		if(i%2)
		{
			p=vram+(y+j)*xsize+x;
			j++;
		}
	}
	//下半部分
	for(i=0;i<16;i++)
	{
		for(k=0;k<8;k++)
		{
			if(font2[i]&(0x80>>k))
			{
				p[k+(i%2)*8]=c;
			}
		}
		for(k=0;k<8/2;k++)
		{
			f=p[k+(i%2)*8];
			p[k+(i%2)*8]=p[8-1-k+(i%2)*8];
			p[8-1-k+(i%2)*8]=f;
		}
		if(i%2)
		{
			p=vram+(y+j)*xsize+x;
			j++;
		}
	}
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
/*在屏幕指定位置打印字符串*/
{
	extern char hankaku[4096];
	struct TASK *task = task_now();
	char *nihongo = (char *) *((int *) 0x0fe8), *font;
	int k, t;

	if (task->langmode == 0) {
		for (; *s != 0x00; s++) {
			putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
			x += 8;
		}
	}
	if (task->langmode == 1) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if ((0x81 <= *s && *s <= 0x9f) || (0xe0 <= *s && *s <= 0xfc)) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				if (0x81 <= task->langbyte1 && task->langbyte1 <= 0x9f) {
					k = (task->langbyte1 - 0x81) * 2;
				} else {
					k = (task->langbyte1 - 0xe0) * 2 + 62;
				}
				if (0x40 <= *s && *s <= 0x7e) {
					t = *s - 0x40;
				} else if (0x80 <= *s && *s <= 0x9e) {
					t = *s - 0x80 + 63;
				} else {
					t = *s - 0x9f;
					k++;
				}
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font);
				putfont8(vram, xsize, x    , y, c, font + 16);
			}
			x += 8;
		}
	}
	if (task->langmode == 2) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if (0x81 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );
				putfont8(vram, xsize, x    , y, c, font + 16);
			}
			x += 8;
		}
	}
	if (task->langmode == 3) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if (0xa1 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
				}
			} else {
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = nihongo + (k * 94 + t) * 32;
				putfont32(vram,xsize,x-8,y,c,font,font+16);
			}
			x += 8;
		}
	}
}

void init_mouse_cursor8(char *mouse, char bc)
/* 鼠标的数据准备（16x16） */
{
	static char cursor[16][16] = {
		"*...............",
		"**..............",
		"*O*.............",
		"*OO*............",
		"*OOO*...........",
		"*OOOO*..........",
		"*OOOOO*.........",
		"*OOOOOO*........",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOOO*****.....",
		"*OO**OO*........",
		"*O*.*OO*........",
		"**...*OO*.......",
		"*....*OO*.......",
		"......**........"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = bc;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
}

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}



unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb)
{
	if (cb == 8) {
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1; /*判断是偶数还是奇数*/
	y &= 1;
	i = table[x + y * 2]; /*用来生成中间色的常量*/
	r = (r * 21) / 256; /* r为0～20*/
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4; /* r为0～5*/
	g = (g + i) / 4;
	b = (b + i) / 4;
	return((unsigned short) (16 + r + g * 6 + b * 36));
	} else {
	return((unsigned short) (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | (b >> 3)));
	}
}