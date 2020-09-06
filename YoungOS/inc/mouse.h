/*
 * @Author: Z. Young
 * @Date: 2020-07-19 11:44:10
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 23:57:46
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _MOUSE_H
#define _MOUSE_H
#include "keyboard.h"
#include "naskfunc.h"
#include "int.h"
#include "fifo.h"
#define MOUSECMD_ENABLE			0xf4
#define KEYCMD_SENDTO_MOUSE		0xd4

struct MOUSE_DEC
{
	unsigned char buf[3], phase;
	/* buf用于保存鼠标动作的3字节数据， 
	*phase记录收到一组3字节数据的第几个*/
	int x, y, btn;	
	/*x,y记录位移量；
	* btn记录鼠标点击事件*/
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

#endif // !_MOUSE_H
