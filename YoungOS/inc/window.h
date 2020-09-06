/*
 * @Author: Z. Young
 * @Date: 2020-07-26 15:41:01
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 15:24:31
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _WINDOW_H
#define _WINDOW_H
#include "graphic.h"
#include "sheet.h"
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);
void change_wtitle8(struct SHEET *sht, char act);
#endif // !_WINDOW_H
