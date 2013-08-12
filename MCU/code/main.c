#include <reg52.h> 
#include <stdio.h>
#include <lcm_1602.h>

uchar code asdf[] = {"zhengzekai"};	//不加“code”声明，显示不出来，奇怪。。

unsigned char flag, temp;
unsigned short int uart_rx = 0;

void uart_init()
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
	uart_init();
	lcm_1602_init();

	temp = 0;
	flag = 0;
	uart_rx = 0;

	lcm_1602_show_string(0, 0, "helloworld=");
	while(1){
		lcm_1602_show_word(11, 0, uart_rx%100/10+'0');	//十位
		lcm_1602_show_word(12, 0, uart_rx%10+'0');		//个位

		if(flag == 1){	//处理中断
			flag = 0;
		}
		//send_str("zhengzk\t");

		delay_ms(500);delay_ms(500);
	}
}

void ser() interrupt 4
{
	RI=0;
	uart_rx = SBUF;
	SBUF = uart_rx; //奇怪，这句不加就收不到数据。。。
	while(!TI);	
	TI = 0;
	flag=1;	 //让主循环知道有中断了
}



