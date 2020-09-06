/*
 * @Author: Z. Young
 * @Date: 2020-07-28 15:04:47
 * @LastEditors: Z. Young
 * @LastEditTime: 2020-07-28 15:38:59
 * @Descripttion: 
 * @version: 
 */ 
#ifndef _JPEG_H
#define _JPEG_H
typedef unsigned char UCHAR;

typedef struct
{
    int elem; 
    unsigned short code[256];
    unsigned char  size[256];
    unsigned char  value[256];
}HUFF;

typedef struct
{
    // SOF
    int width;
    int height;
    // MCU
    int mcu_width;
    int mcu_height;

    int max_h,max_v;
    int compo_count;
    int compo_id[3];
    int compo_sample[3];
    int compo_h[3];
    int compo_v[3];
    int compo_qt[3];

    // SOS
    int scan_count;
    int scan_id[3];
    int scan_ac[3];
    int scan_dc[3];
    int scan_h[3];  
    int scan_v[3];  
    int scan_qt[3]; 
    
    // DRI
    int interval;

    int mcu_buf[32*32*4]; 
    int *mcu_yuv[4];
    int mcu_preDC[3];
    
    // DQT
    int dqt[3][64];
    int n_dqt;
    
    // DHT
    HUFF huff[2][3];
    
    
    // FILE i/o
	unsigned char *fp, *fp1;
    unsigned long bit_buff;
    int bit_remain;
    int width_buf;

	int base_img[64][64]; 

    /* for dll 
    
    JPEG *jpeg = (JPEG *)malloc(sizeof(JPEG) + 256);
    */
    int dummy[64];
    
}JPEG;

/* for 16bit */
#ifndef PIXEL16
#define PIXEL16(r, g, b)	((r) << 11 | (g) << 5 | (b))
	/* 0 <= r <= 31, 0 <= g <= 63, 0 <= b <= 31 */
#endif

struct DLL_STRPICENV{
	int work[64 * 1024 / 4];
};
struct RGB{
	unsigned char b,g,r,t;
};

int info_JPEG(struct DLL_STRPICENV *env, int *info, int size, UCHAR *fp);
int decode0_JPEG(struct DLL_STRPICENV *env, int size, UCHAR *fp, int b_type, UCHAR *buf, int skip);

void jpeg_idct_init(int base_img[64][64]);
int jpeg_init(JPEG *jpeg);
// int jpeg_header(JPEG *jpge);
void jpeg_decode(JPEG *jpeg, unsigned char *rgb,int b_type);

#endif // !_JPEG_H
