/*
 * @Author: Z. Young
 * @Date: 2020-07-20 14:26:56
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 02:04:13
 * @Descripttion: 
 * @version: 
 */ 
 
#ifndef _SHEET_H
#define _SHEET_H

#include "memo.h"


#define SHEET_USE  1

#define MAX_SHEETS		256

struct SHEET {
	unsigned char *buf;
    /*vx0,vy0表示图层在画面上位置的坐标，col_inv表示透明色色号，height表示图层高度*/
	/*flags 标识属性 0,未用;1,已使用;bit[5]=1,命令行窗口含光标;bit[4]=1,应用程序任务窗口;*/
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;	
	struct TASK *task;
};

struct SHTCTL {
    /*图层管理*/
	unsigned char *vram, *map;    //VRAM的地址
	int xsize, ysize, top;  //画面的大小，top表示顶层图层高度
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};

struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);
void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1);

#endif // !_SHEET_H
