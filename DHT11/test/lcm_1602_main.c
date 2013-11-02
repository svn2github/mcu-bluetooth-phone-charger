/******************************************************************
** 1602A液晶屏驱动程序
** 晶 振 频 率:11.0592M 
******************************************************************/

#include <reg52.h>
#include <lcm_1602.h>

sbit LCM_E=P3^5;//定义接口
sbit LCM_RW=P3^6;
sbit LCM_RS=P3^7;
sbit LCM_V0 = P3^3;
#define LCM_Data P2//数据接口

void LCM_WritEDAta(uchar WDLCM);
void LCM_WriteCommand(uchar WCLCM,BuysC);
uchar LCM_ReadData(void);
uchar LCM_ReadStatus(void);
void lcm_1602_init(void);
void lcm_1602_show_word(uchar X, uchar Y, uchar DData);
void lcm_1602_show_string(uchar X, uchar Y, uchar code *DData);
void Delay5Ms(void);
void Delay400Ms(void);

uchar code uctech[] = {"1602A"};
uchar code net[] = {"zhengzekai_test"};

/****************************1602A写数据函数**************************/
/*函数原型:void LCM_WriteData(uchar WDLCM)
/*函数功能:1602A写数据
/*输入参数:要写入的数据
/*输出参数:无
/**********************************************************************/
void LCM_WriteData(uchar WDLCM)
{
	LCM_ReadStatus();//检测忙
	LCM_Data = WDLCM;
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0;//若晶振速度太高可以在这后加小的延时
	LCM_E = 0;//延时
	LCM_E = 1;
}

/****************************1602A写指令函数**************************/
/*函数原型:void LCM_WriteCommand(uchar WCLCM,BuysC)
/*函数功能:1602A写指令
/*输入参数:要写入的指令
/*输出参数:无
/**********************************************************************/
void LCM_WriteCommand(uchar WCLCM,BuysC)//BuysC为0时忽略忙检测
{
	if(BuysC) LCM_ReadStatus();//根据需要检测忙
	LCM_Data = WCLCM;
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
}

/****************************1602A读数据函数**************************/
/*函数原型:uchar LCM_ReadData(void)
/*函数功能:1602A读数据
/*输入参数:无
/*输出参数:1602A返回的数据
/**********************************************************************/
uchar LCM_ReadData(void)
{
	LCM_RS = 1;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}

/****************************1602A读状态函数**************************/
/*函数原型:uchar LCM_ReadData(void)
/*函数功能:1602A读状态
/*输入参数:无
/*输出参数:1602A返回的状态
/**********************************************************************/
uchar LCM_ReadStatus(void)
{
	LCM_Data = 0xFF;
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	while (LCM_Data & 0x80);//检测忙信号
	return(LCM_Data);
}

/****************************1602A初始化函数**************************/
/*函数原型:void lcm_1602_init(void)
/*函数功能:1602A初始化
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void lcm_1602_init(void)
{
	LCM_V0 = 0;	//调节对比度，不然不显示
	LCM_Data = 0;
	LCM_WriteCommand(0x38,0);//三次显示模式设置，不检测忙信号
	Delay5Ms();
	LCM_WriteCommand(0x38,0);
	Delay5Ms();
	LCM_WriteCommand(0x38,0);
	Delay5Ms();
	LCM_WriteCommand(0x38,1);//显示模式设置,开始要求每次检测忙信号
	LCM_WriteCommand(0x08,1);//关闭显示
	LCM_WriteCommand(0x01,1);//显示清屏
	LCM_WriteCommand(0x06,1);// 显示光标移动设置
	LCM_WriteCommand(0x0C,1);// 显示开及光标设置
}

/******************1602A按指定位置显示一个字符函数**********************/
/*函数原型:void lcm_1602_show_word(uchar X, uchar Y, uchar DData)
/*函数功能:1602A按指定位置显示一个字符
/*输入参数:X坐标 Y坐标 要显示的字符
/*输出参数:无
/**********************************************************************/

void lcm_1602_show_word(uchar X, uchar Y, uchar DData)
{
	Y &= 0x1;
	X &= 0xF;//限制X不能大于15，Y不能大于1
	if (Y) X |= 0x40;//当要显示第二行时地址码+0x40;
	X |= 0x80;// 算出指令码
	LCM_WriteCommand(X, 0);//这里不检测忙信号，发送地址码
	LCM_WriteData(DData);
}

/*******************1602A按指定位置显示一串字符函数*********************/
/*函数原型:void lcm_1602_show_string(uchar X, uchar Y, uchar code *DData)
/*函数功能:1602A按指定位置显示一个字符
/*输入参数:X坐标 Y坐标 要显示字符串的首地址
/*输出参数:无
/**********************************************************************/
void lcm_1602_show_string(uchar X, uchar Y, uchar code *DData)
{
	uchar ListLength;
	ListLength = 0;
	Y &= 0x1;
	X &= 0xF;//限制X不能大于15，Y不能大于1
	while (DData[ListLength]>0x20)//若到达字串尾则退出
	{
		if (X <= 0xF)//X坐标应小于0xF
		{
			lcm_1602_show_word(X, Y, DData[ListLength]);//显示单个字符
			ListLength++;
			X++;
		}
	}
}

/********************5ms延时函数************************/
/*函数原型:void Delay5Ms(void)
/*函数功能:5ms延时
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

/********************400ms延时延时函数************************/
/*函数原型:void Delay400Ms(void)
/*函数功能:400ms延时延时
/*输入参数:无
/*输出参数:无
/**********************************************************************/
void Delay400Ms(void)
{
	uchar TempCycA = 5;
	unsigned int TempCycB;
	while(TempCycA--)
	{
		TempCycB=7269;
		while(TempCycB--);
	};
}

/***************************main**********************************/
/*
void main(void)
{
	Delay400Ms();//启动等待，等LCM讲入工作状态	
	lcm_1602_init();//LCM初始化	
	Delay5Ms();//延时片刻(可不要)	
	lcm_1602_show_string(0, 0, net);	
	lcm_1602_show_string(6, 1, uctech);
	lcm_1602_show_word(0,1,'9');
	LCM_ReadData();//测试用句无意义	
	LCM_Data=255;	
	while(1);
}
*/