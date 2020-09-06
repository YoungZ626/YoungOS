/*
 * @Author: Z. Young
 * @Date: 2020-07-25 15:23:34
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 18:16:32
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _MTASK_H
#define _MTASK_H

#include "timer.h"
#include "fifo.h"
#include "dsctbl.h"
#include "memo.h"
#include "console.h"
#define MAX_TASKS 1000	/*最大任务数量*/
#define TASK_GDT0 3			/*定义从GDT的几号开始分配给TSS */
#define MAX_TASKS_LV    100
#define MAX_TASKLEVELS  10
struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};
struct TASK {
	int sel, flags;		/* sel用来存放GDT的编号,selector*/
	int level, priority; /* 优先级,先level,再priority */
	struct FIFO32 fifo;
	struct TSS32 tss;
	struct SEGMENT_DESCRIPTOR ldt[2];
	struct CONSOLE *cons;
	int ds_base;/*数据段地址*/
	int cons_stack;
	struct FILEHANDLE *fhandle;
	int *fat;
	char *cmdline;
	unsigned char langmode, langbyte1;
};
struct TASKLEVEL {
	int running; /*正在运行的任务数量*/
	int now; /*这个变量用来记录当前正在运行的是哪个任务*/
	struct TASK *tasks[MAX_TASKS_LV];
};
struct TASKCTL {
	int now_lv; /*现在活动中的LEVEL */
	char lv_change; /*在下次任务切换时是否需要改变LEVEL */
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};
extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);
void task_idle(void);
struct TASK *task_now(void);

#endif // !_MTASK_H
