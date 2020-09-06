/*
 * @Author: Z. Young
 * @Date: 2020-07-28 18:37:43
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 21:31:18
 * @Descripttion: 
 * @version: 
 */ 
#include "apilib.h"

void Main(void)
{
	int fh;
	char c, cmdline[30], *p;

	api_cmdline(cmdline, 30);
	for (p = cmdline; *p > ' '; p++) { }	/*跳过之前的内容，直到遇到空格*/
	for (; *p == ' '; p++) { }	/*跳过空格*/
	fh = api_fopen(p);
	if (fh != 0) {
		for (;;) {
			if (api_fread(&c, 1, fh) == 0) {
				break;
			}
			api_putchar(c);
		}
	} else {
		api_putstr0("File not found.\n");
	}
	api_end();
}
