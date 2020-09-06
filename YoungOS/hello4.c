/*
 * @Author: Z. Young
 * @Date: 2020-07-27 11:03:20
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 11:07:42
 * @Descripttion: 
 * @version: 
 */ 
void api_putstr0(char *s);
void api_end(void);

void Main(void)
{
	api_putstr0("hello, world");
	api_end();
}
