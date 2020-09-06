/*
 * @Author: Z. Young
 * @Date: 2020-07-18 17:04:54
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 17:58:46
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _DSCTBL_H
#define _DSCTBL_H

#include "naskfunc.h"

struct SEGMENT_DESCRIPTOR {
	/*limit描述段长；
	* base描述基址；
	* access_right描述特权级*/
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	/* offset 描述中断或异常处理程序在内存段的偏移；
	* selector描述段选择符；
	* dw_count未使用；
	* access_right描述特权级*/
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_LDT			0x0082
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e

#endif // !_DSCTBL_H
