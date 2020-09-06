/*
 * @Author: Z. Young
 * @Date: 2020-07-20 11:19:39
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 02:05:06
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _MEMORY_H
#define _MEMORY_H

#include "naskfunc.h"

#define MEMMAN_FREES		4090 /* 大约是32KB*/
#define MEMMAN_ADDR			0x003c0000

struct FREEINFO { /* 可用信息 */
	unsigned int addr, size;
};
struct MEMMAN { /* 内存管理 */
	int frees, maxfrees, lostsize, losts,lastIndex; 
	/*空闲块数，空闲内存块曾出现过的最大次数，内存释放失败总大小，内存释放失败总次数， 上次查询到的块号*/
	struct FREEINFO free[MEMMAN_FREES];/*按地址升序记录空闲块*/
};

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

#endif // !_MEMORY_H
