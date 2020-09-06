/*
 * @Author; Z. Young
 * @Date; 2020-07-26 19;09;31
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-08-04 20:56:31
 * @Descripttion; 
 * @version; 
 */ 
#include "cmos.h"
/*
* 程序功能：读取CMOS时间并显示
* CMOS读写方法如下：
* 1.向地址端口70H写入要访问的单元地址
* 2.从71H端口读出数据
* 注意：CMOS中存放的是BCD码
* 数码：  0    1    2    3    4
* BCD码：0000 0001 0010 0011 0100 
* 数码：  5    6    7    8    9
* BCD码：0101 0110 0111 1000 1001 
* CMOS RAM中时间格式如下：
* 秒：00H
* 分：02H
* 时：04H
* 日：07H
* 月：08H
* 年：09H
* BCD码转ASCII码：BCD + 30H = ASCII
*/
unsigned char read_cmos(unsigned char p)
{
    unsigned char data;
    io_out8(cmos_index, p);
    data = io_in8(cmos_data);
    io_out8(cmos_index, 0x80);
    return data;
}

unsigned int get_hour_hex()
/*获取小时*/
{
    return BCD_HEX(read_cmos(CMOS_CUR_HOUR));
}

unsigned int get_min_hex()
/*获取分钟*/
{
    return BCD_HEX(read_cmos(CMOS_CUR_MIN));
}

unsigned int get_sec_hex()
/*获取秒*/
{
    return BCD_HEX(read_cmos(CMOS_CUR_SEC));
}

unsigned int get_day_of_month()
/*获取月日*/
{
    return BCD_HEX(read_cmos(CMOS_MON_DAY));
}

unsigned int get_day_of_week()
/*获取星期几*/
{
    return BCD_HEX(read_cmos(CMOS_WEER_DAY));
}

unsigned int get_mon_hex()
/*获取月份*/
{
    return BCD_HEX(read_cmos(CMOS_CUR_MON));
}

unsigned int get_year()
/*获取年份*/
{
    return (BCD_HEX(read_cmos(CMOS_CUR_CEN)) * 100) + BCD_HEX(read_cmos(CMOS_CUR_YEAR)) + 1980;
}