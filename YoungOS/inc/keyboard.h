/*
 * @Author: Z. Young
 * @Date: 2020-07-19 11:40:53
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 17:41:09
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "int.h"
#include "bootpack.h"
#include "fifo.h"
#include "naskfunc.h"
#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
void init_keyboard(struct FIFO32 *fifo, int data0);
void wait_KBC_sendready(void);
void inthandler21(int *esp);
#endif // !_KEYBOARD_H
