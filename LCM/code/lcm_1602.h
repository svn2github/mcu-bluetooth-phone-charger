#ifndef	_LCM_1602_H_
#define _LCM_1602_H_

#define uchar unsigned char
#define uint unsigned int

extern void LCM_Init(void);
extern void DisplayOneChar(uchar X, uchar Y, uchar DData);
extern void DisplayListChar(uchar X, uchar Y, uchar code *DData);