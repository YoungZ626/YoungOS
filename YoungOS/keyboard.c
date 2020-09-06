/*
 * @Author: Z. Young
 * @Date: 2020-07-19 11:39:53
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-25 00:03:30
 * @Descripttion: 
 * @version: 
 */ 
#include "keyboard.h"

struct FIFO32 *keyfifo;
int keydata0;

void inthandler21(int *esp)
/* 来自PS/2键盘的中断 */
{
	int data;
	io_out8(PIC0_OCW2, 0x61); /* 把IRQ-01接收信号结束的信息通知给PIC */
	data = io_in8(PORT_KEYDAT);
	fifo32_put(keyfifo, data + keydata0);
}

void wait_KBC_sendready(void)
{
	/* 等待键盘控制电路准备完毕 */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
}

void init_keyboard(struct FIFO32 *fifo, int data0)
{
	/* 将FIFO缓冲区的信息保存到全局变量里 */
	keyfifo = fifo;
	keydata0 = data0;
	/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
}
