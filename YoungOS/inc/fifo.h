/*
 * @Author: Z. Young
 * @Date: 2020-07-18 17:02:44
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 14:57:46
 * @Descripttion: 
 * @version: 
 */
 
 #ifndef _FIFO_H
 #define _FIFO_H

 #define FLAGS_OVERRUN		0x0001

struct FIFO32 {
	int *buf; /* 队列空间首地址 */
	int p, q, size, free, flags;/* p队尾，q队头， size队列大小， flags状态为1时表示满了*/
	struct TASK *task;/*指向当前关联的任务*/
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

#endif // !_FIFO_H
