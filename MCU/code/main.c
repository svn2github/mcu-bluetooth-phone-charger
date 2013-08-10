#include<reg52.h> 
#include <stdio.h>

const unsigned char num[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0xff, 0x6f};

unsigned char flag, a, temp;

void num_show(unsigned char para)
{
	if(para < 10)
		P0 = num[para];
}

void uart0_init()
{		
	//9600波特率对应 FFD9H
	RCAP2L = 0xD9;
	RCAP2H = 0xFF;
	//RCLK、TCLK置1
	//TR2置1
	T2CON = 0x34;
	//串口工作模式1，接收使能
	SCON = 0x50;
	
	REN=1;    //串口允许接收
	EA=1;     //CPU中断允许
	
	//允许串口中断
	ES = 1;
}

void delay_ms(int ms)
{
	int j = 5000;
	for(;ms>0;ms--)
		for(;j>0;j--);
}

void send_byte(unsigned char c)
{
	SBUF = c;
	while(!TI);	
	TI = 0;
}

void send_str(unsigned char *s)
{
	while(*s != '\0')
	{
		send_byte(*s);
		s++;
	}
}

void main()
{
	uart0_init();

	P0 = 0x01;
	temp = 0;

	while(1){
		send_str("zhengzk\t");
		num_show(temp++);
		if(temp == 10) temp = 0;
		delay_ms(500);delay_ms(500);
	}
}

/*
void ser() interrupt 4
{
	RI=0;
	a=SBUF;
	//send_str("中断");
	//send_byte(a);//注意，把打log加在这里，按原则是不可以的。但是单片机处理速度慢，处理不过来，除非用队列。
	SBUF = 0xaa;
	while(!TI);	
	TI = 0;
	flag=1;
}
*/


