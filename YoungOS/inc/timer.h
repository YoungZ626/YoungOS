/*
 * @Author: Z. Young
 * @Date: 2020-07-24 10:56:17
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-27 17:44:46
 * @Descripttion: timer.c
 * @version: 1.30.0.0
 */ 
#ifndef _TIMER_H
#define _TIMER_H

#include "bootpack.h"
#include "naskfunc.h"
#include "int.h"
#include "fifo.h"
#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040
#define TIMER_FLAGS_ALLOC 1 /* 已配置状态 */
#define TIMER_FLAGS_USING 2 /* 定时器运行中 */
#define MAX_TIMER 500
struct TIMER {
	struct TIMER *next_timer;
	/* 指向下一个定时器，即
 	* 超时值刚好大于(或等于)自身超时值的定时器。*/
	unsigned int timeout;/*超时时间，单位10ms*/
	char flags, flags2; 
	/* flags=0/1/2,标识定时器未使用/已分配/正使用;*/
	/*flags2用于区分是否需要在应用程序结束时自动取消*/
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count, next_time;
	/* count,当前系统已计时值；
 	* next_time,即将超时定时器的超时值*/
	struct TIMER *t0;	//timer链表表头记录
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;

void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);
int timer_cancel(struct TIMER *timer);
void timer_cancelall(struct FIFO32 *fifo);

#endif // !_TIMER_H
