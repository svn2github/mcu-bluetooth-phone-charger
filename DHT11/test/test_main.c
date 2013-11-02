#include <reg52.h>
#include <lcm_1602.h>
#include "dht11.h"

sbit BUZZER = P1^0;

//中断计数标志
unsigned char flag=0;

extern int temp_value, humi_value;
static void InitTime(void);
void Delay_1ms(unsigned int ms);

void main()
{
/*
	uchar temperature = 0;
	//BUZZER = 0;
	lcm_1602_init();
	lcm_1602_show_string(0, 0, "zhengzekai");

	while(1)
	{
		lcm_1602_show_word(0, 1, '0' + temperature/10);	 
		lcm_1602_show_word(1, 1, '0' + temperature/10%10);
	}	
	//lcm_1602_show_string(6, 1, uctech);
*/

	int i;
	unsigned char mark = 1;
	//先等上电稳定
	Delay_1ms(1000);
	//因为读一次数据dht11 才会触发一次采集数据.
	//即在先使用数据时采集一次数据
	ReadTempAndHumi();
	//因为在两次采集数据需一定的时间间隔,这里还可减少
	Delay_1ms(3000);
	//设定定时器
	InitTime();

	lcm_1602_init();

	lcm_1602_show_string(0, 0, "temp:00");
	lcm_1602_show_string(0, 1, "hmdt:00");

	while(1)
	{
		//三秒读一次温湿度
		if(flag == 60)
		{
			flag = 0;
			mark++;
			/*
			//读温湿度,可检测函数调用是否失败,
			//函数返回OK(1)表示成功,返回ERROR(0)表示失败
			//OK和ERROR是在DHT11.H中定义的宏
			*/
			ReadTempAndHumi();
		}
		if(mark % 2 == 0)
		{
			//显示温度
			//LED1=0;
			//LED2=1;
			for(i=0;i<2;i++)
			{
				//display(temp_value); 
				lcm_1602_show_word(5, 0, '0' + temp_value/10%10);
				lcm_1602_show_word(6, 0, '0' + temp_value%10);
			}		
		}
		else
		{
			//LED1=1;
			//LED2=0;
		//	mark++;
			for(i=0;i<2;i++)
			{
				//显示湿度
				//display(humi_value);
				lcm_1602_show_word(5, 1, '0' + humi_value/10%10);
				lcm_1602_show_word(6, 1, '0' + humi_value%10);
			}
		}
	}
	//return 0;	
}

//设定定时器
static void InitTime(void)
{
	TH0 = (65536-50000);    		//(unsigned char)((65535U - 50000) >> 8);
	TL0 = (65536-50000);    		//(unsigned char)(65535U - 50000) ;
	TMOD = 0X01;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
}

void timer(void) interrupt 1
{
	TH0 =(65535 - 50000);	// (unsigned char)((65535 - 50000) >> 8);
	TL0 =(65535 - 50000);	// (unsigned char)(65535 - 50000);
	flag++;
}