/*
 * @Author: Z. Young
 * @Date: 2020-07-26 15:07:24
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 15:00:22
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _FILE_H
#define _FILE_H
#include "memo.h"
struct FILEINFO {
	unsigned char name[8], ext[3], type;/*文件名 扩展 属性*/
	/*type 0x20-普通文件,0x01-只读文件, 0x02-隐藏文件,0x04-系统文件,0x10-目录*/
	char reserve[10]; /* 保留未用 */
	unsigned short time, date, clustno; /*文件时间、日期、扇区号*/
	unsigned int size; /*文件大小*/
};
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
char *file_loadfile2(int clustno, int *psize, int *fat);

#endif // !_FILE_H
