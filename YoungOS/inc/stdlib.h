/*
 * @Author: Z. Young
 * @Date: 2020-08-02 17:08:40
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-02 17:09:15
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _STDLIB_H
#define _STDLIB_H

#define FILE 	int

int putchar(int c);
int getchar();
int puts(const char *str);
char *gets(char *str);
int printf(const char *format, ...);
int scanf(const char *format, ...);
void exit(int status);
void *malloc(int size);
void free(void *p);


#endif // !_STDLIB_H