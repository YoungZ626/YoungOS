/*
 * @Author: Z. Young
 * @Date: 2020-07-17 21:40:33
 * @LastEditTime: 2020-07-28 23:44:52
 * @LastEditors: Z. Young
 * @Description: 
 * @FilePath: \YoungOS\bootpack.h
 */ 
#ifndef _BOOTPACK_H
#define _BOOTPACK_H

/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* 启动区读磁盘读到此为止 */
	char leds; /* 启动时键盘的LED的状态 */
	char vmode; /* 显卡模式为多少位彩色 */
	char reserve;
	short scrnx, scrny; /* 画面分辨率 */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG		0x00100000



#endif // !_BOOTPACK_H
