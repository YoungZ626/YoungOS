/*
 * @Author: Z. Young
 * @Date: 2020-07-26 15:39:56
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-31 18:19:55
 * @Descripttion: 
 * @version: 
 */ 
#include "window.h"
/**
 * @description: 
 * @param {unsigned char * buf}  指定vram地址
 * @param {int xsize}  设置窗口长度
 * @param {int ysize}  设置窗口高度
 * @param {char *title}  设置窗度标题
 * @param {char act}  活跃标志
 * @return: 
 */
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act)
{
	boxfill8(buf, xsize, COL8_C6C6C6, 0, 0, xsize - 1, 0 );
	boxfill8(buf, xsize, COL8_FFFFFF, 1, 1, xsize - 2, 1 );
	boxfill8(buf, xsize, COL8_C6C6C6, 0, 0, 0, ysize - 1);
	boxfill8(buf, xsize, COL8_FFFFFF, 1, 1, 1, ysize - 2);
	boxfill8(buf, xsize, COL8_848484, xsize - 2, 1, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, xsize - 1, 0, xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_C6C6C6, 2, 2, xsize - 3, ysize - 3);
	boxfill8(buf, xsize, COL8_848484, 1, ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, 0, ysize - 1, xsize - 1, ysize - 1);
	make_wtitle8(buf, xsize, title, act);
}
/**
 * @description: 
 * @param {unsigned char * buf}  指定vram地址
 * @param {int xsize}  设置窗口长度
 * @param {char *title}  设置窗度标题
 * @param {char act}  活跃标志
 * @return: 
 */
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act)
{
	static char closebtn[14][16] = {
		"QQQQQQ$$$$QQQQQQ",
		"QQQQ$$$$$$$$QQQQ",
		"QQQ$$$$$$$$$$QQQ",
		"QQ$$@@$$$$@@$$QQ",
		"QQ$$$@@$$@@$$$QQ",
		"Q$$$$$@@@@$$$$$Q",
		"Q$$$$$$@@$$$$$$Q",
		"Q$$$$$@@@@$$$$$Q",
		"Q$$$$@@$$@@$$$$Q",
		"QQ$$@@$$$$@@$$QQ",
		"QQ$$$$$$$$$$$$QQ",
		"QQQ$$$$$$$$$$QQQ",
		"QQQQ$$$$$$$$QQQQ",
		"QQQQQQ$$$$QQQQQQ"
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
	boxfill8(buf, xsize, tbc, 3, 3, xsize - 4, 20);
	putfonts8_asc(buf, xsize, 24, 4, tc, title);
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = COL8_FFFFFF;
			} else if (c == '$') {
				c = COL8_FF0000;
			} else if (c == 'Q') {
				c = COL8_C6C6C6;
			} else {
				c = COL8_FFFFFF;
			}
			buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
}

/**
 * @description: 字符串显示
 * @param {struct SHEET *sht} 指定要显示的SHEET
 * @param {int x} 显示位置的x坐标
 * @param {int y} 显示位置的y坐标
 * @param {int c} 显示字符颜色
 * @param {int b} 显示背景颜色
 * @param {char *s} 显示字符串
 * @param {int l} 显示字符串长度
 * @return: 
 */
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	struct TASK *task = task_now();
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	if (task->langmode != 0 && task->langbyte1 != 0) {
		putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
		sheet_refresh(sht, x - 8, y, x + l * 8, y + 16);
	} else {
		putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
		sheet_refresh(sht, x, y, x + l * 8, y + 16);
	}
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
	boxfill8(sht->buf, sht->bxsize, c, x0 - 1, y0 - 1, x1 + 0, y1 + 0);
}


void change_wtitle8(struct SHEET *sht, char act)
{
	int x, y, xsize = sht->bxsize;
	char c, tc_new, tbc_new, tc_old, tbc_old, *buf = sht->buf;
	if (act != 0) {
		tc_new  = COL8_FFFFFF;
		tbc_new = COL8_000084;
		tc_old  = COL8_C6C6C6;
		tbc_old = COL8_848484;
	} else {
		tc_new  = COL8_C6C6C6;
		tbc_new = COL8_848484;
		tc_old  = COL8_FFFFFF;
		tbc_old = COL8_000084;
	}
	for (y = 3; y <= 20; y++) {
		for (x = 3; x <= xsize - 4; x++) {
			c = buf[y * xsize + x];
			if (c == tc_old && x <= xsize - 22) {
				c = tc_new;
			} else if (c == tbc_old) {
				c = tbc_new;
			}
			buf[y * xsize + x] = c;
		}
	}
	sheet_refresh(sht, 3, 3, xsize, 21);
}
