#ifndef	_LCM_1602_H_
#define _LCM_1602_H_

#define uchar unsigned char
#define uint unsigned int

extern void lcm_1602_init(void);
extern void lcm_1602_show_word(uchar X, uchar Y, uchar DData);
extern void lcm_1602_show_string(uchar X, uchar Y, uchar code *DData);

#endif

