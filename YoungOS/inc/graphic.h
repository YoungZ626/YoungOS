/*
 * @Author: Z. Young
 * @Date: 2020-07-18 17:04:02
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 21:21:58
 * @Descripttion: 
 * @version: 
 */ 

#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#include "naskfunc.h"
#include "memo.h"
#include "bootpack.h"
#include "file.h"
#include "jpeg.h"
#include "multitask.h"
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y, char *bkgname);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);
void putfont32(char *vram, int xsize, int x, int y, char c, char *font1, char *font2);
int set_backbroung(int *fat, char *vram, int x, int y, char *bkgname);
unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

#endif // !_GRAPHIC_H
