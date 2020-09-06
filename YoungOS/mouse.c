/*
 * @Author: Z. Young
 * @Date: 2020-07-19 11:43:11
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 23:45:32
 * @Descripttion: 
 * @version: 
 */ 
#include "mouse.h"

struct FIFO32 *mousefifo;
int mousedata0;

void inthandler2c(int *esp)
/* 来自PS/2鼠标的中断 */
{
	int data;
	io_out8(PIC1_OCW2, 0x64); /* 把IRQ-12接收信号结束的信息通知给PIC1 */
	io_out8(PIC0_OCW2, 0x62); /* 把IRQ-02接收信号结束的信息通知给PIC0 */
	data = io_in8(PORT_KEYDAT);
	fifo32_put(mousefifo, data + mousedata0);
}

void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec)
{
	/* 将FIFO缓冲区的信息保存到全局变量里 */
	mousefifo = fifo;
	mousedata0 = data0;
	/* 鼠标有效 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* 顺利的话，ACK(0xfa)会被发送*/
	mdec->phase = 0; /* 等待鼠标的0xfa的阶段*/
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	/*鼠标解码*/
	if (mdec->phase == 0) {
		/* 等待鼠标的0xfa的阶段 */
		if (dat == 0xfa) {
			mdec->phase = 1;
		}        
		return 0;
	}
	if (mdec->phase == 1) {
		/* 等待鼠标第一字节的阶段 */
		mdec->buf[0] = dat;
		mdec->phase = 2;
		return 0;
	}
	/* 鼠标第1字节数据bit[3]=1,bit[7..6]=00,
	* 若鼠标第1字节数据不满足以上状态则表明 
	* 鼠标数据传递可能有误,所以在此丢弃并继
	* 续等待并解析第一字节数据。
	* 
	* bit[2..0]置位时分别代表鼠标滑轮点击, 鼠
	* 标右击和鼠标左击。bit[5..4]分别跟鼠标上
	* 下和左右移动的方向, 值为0时表示往上或右
	* 移动,值为1时表示往下或左移动。
	* 
	* 接收到第一个字节后置phase=2表示接下来接
	* 收鼠标第二字节数据。*/
	if (mdec->phase == 2) {
		/* 等待鼠标第二字节的阶段 */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3) {
		/* 等待鼠标第三字节的阶段 */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		/* 若鼠标第1字节bit[5..4]皆为1,则表示鼠标分别 
		* 在往下或左方向移动, 鼠标将这两个方向分别视 
		* 为y和x的负方向。与此对应, 此时鼠标中断传送 
		* 上来的y和x是一个负数( 的低8位)。在32位补码 
		* 表示数中, 将y和x的高24位扩展为1时, 就得到y 
		* 和x的负数值, 从而获得了鼠标位移的真实值。*/
		if ((mdec->buf[0] & 0x10) != 0) {
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[0] & 0x20) != 0) {
			mdec->y |= 0xffffff00;
		}     
		/* 鼠标的y方向与画面符号相反 */   
		mdec->y = - mdec->y; 
		return 1;
	}
	/* 应该不可能到这里来 */
	return -1; 
}
