/*
 * @Author: Z. Young
 * @Date: 2020-07-26 15:09:07
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-01 00:42:23
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdio.h>
#include <string.h>
#include "memo.h"
#include "graphic.h"
#include "multitask.h"
#include "sheet.h"
#include "file.h"
#include "dsctbl.h"
#include "window.h"
#include "bootpack.h"

#define ADR_DISKIMG		0x00100000
struct CONSOLE {/* 管理命令行窗口的结构体类型*/
	/* sht,管理命令行窗口画面的结构体指针;
	* cur_x,cur_y,cur_c,光标坐标和色号;
	* timer,命令行窗口定时器指针。*/
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
	struct TIMER *timer;
};
struct FILEHANDLE { /* 管理打开文件的结构体类型。*/
	/* buf指向缓存文件内容的地址
	* 文件大小
	* 文件当前位置*/
	char *buf;
	int size;
	int pos;
};
void console_task(struct SHEET *sheet, unsigned int memtotal);
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_newline(struct CONSOLE *cons);
void cons_putstr0(struct CONSOLE *cons, char *s);
void cons_putstr1(struct CONSOLE *cons, char *s, int l);
void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned int memtotal);
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);
void cmd_exit(struct CONSOLE *cons, int *fat);
void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline);
int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline);
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
int *inthandler0c(int *esp);
int *inthandler0d(int *esp);
void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col);
void cmd_langmode(struct CONSOLE *cons, char *cmdline);
#endif // !_CONSOLE_H
