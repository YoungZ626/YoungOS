/*
 * @Author: Z. Young
 * @Date: 2020-07-26 21:45:21
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-27 17:41:39
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _CMOS_H
#define _CMOS_H
#include "naskfunc.h"
#define cmos_index 0x70
#define cmos_data 0x71
/*cMOS中相天信息偏移*/
#define CMOS_CUR_SEC 0X0//CMOS中当前秒值(BCD)
#define CMOS_ALA_SEC 0x1//CMOS中报警秒值(BCD)
#define CMOS_CUR_MIN 0x2//CMOS中当前分钟(BCD)
#define CMOS_ALA_MIN 0x3//CMOS中报警分钟(BCD)
#define CMOS_CUR_HOUR 0X4//CMOS中当前小时(BCD)
#define CMOS_ALA_HOUR 0x5//CMOS中报警小时(BCD)
#define CMOS_WEER_DAY 0X6//CMOS中一周中当前天(BCD)
#define CMOS_MON_DAY 0x7//CMOS中一月中当前日(BCD)
#define CMOS_CUR_MON 0x8//CMOS中当前月份(BCD)
#define CMOS_CUR_YEAR 0x9//CMOS中当前年份(BCD)
#define CMOS_DEV_TYPE 0x12//CMOS中驱动器格式
#define CMOS_CUR_CEN 0X32//CMOS中当前世纪(BCD)

#define BCD_HEX(n) ((n>>4)*10) + (n &0xf)//BCD转十六进制

#define BCD_ASCII_first(n) (((n<<4)>>4)+0x30)//取BCD的个位并以字符输出
#define BCD_ASCII_S(n) ((n<<4) + 0×30)//取BCD的十位并以字符输出
unsigned int get_hour_hex();
unsigned int get_min_hex();
unsigned int get_sec_hex();
unsigned int get_day_of_month();
unsigned int get_day_of_week();
unsigned int get_mon_hex();
unsigned int get_year();
#endif // !_CMOS_H
