#include <reg52.h>
#include<intrins.h>
 
sbit DS18b20_DataPort=P1^1; //引脚选择

void delay_us(unsigned int t)
{
	for(;t>0;t--);
}

bit DS18b20_reset(void)  //返回0-有设备连接1-无设备连接
{
    unsigned char i;
    bit RstFlag;
     RstFlag=1;
     DS18b20_DataPort=0;
    delay_us(600);//器件反应时间最少480us
     DS18b20_DataPort=1;
     delay_us(60); //15us-60us
     for (i=0;i<16;i++) //60us-240us
     {
          delay_us(15);
          if (DS18b20_DataPort==0)
            RstFlag=0;
    }
     delay_us(240);//  低电平持续时间最少240us
     return RstFlag;
}



void DS1820_write(unsigned char command)
{
    unsigned char i;
     DS18b20_DataPort=1;
     delay_us(1);
     for (i=0;i<8;i++)
     {
          EA=0;
         DS18b20_DataPort=0;
          delay_us(15);
          DS18b20_DataPort=ch&0x1;
          EA=1;
          delay_us(50);
          DS18b20_DataPort=1;
          command=command>>1;
          delay_us(1);
     }
}


unsigned char DS1820_read(void)
{
    unsigned char i,date = 0;
     DS18b20_DataPort = 1;
     delay_us(1);
     for (i = 0;i < 8;i++)
     {
         EA = 0;
         DS18b20_DataPort = 0;
         delay_us(10);
          DS18b20_DataPort = 1;
          delay_us(2);
          date = date>>1;
          if (DS18b20_DataPort == 1)
          {
              date += 0x80;
          }
          EA = 1;
          delay_us(50);
 }
 return date;
}



void DS1820_skipROM(void)  //主机发送跳过读序列号的操作指令
{
     DS18b20_reset();
     DS18b20_write(0xcc);
}
void DS1820_startADC(void)
{

     DS18b20_reset();  //复位
     DS18b20_write(0xcc);  //广播
     DS18b20_write(0x44);  //启动AD转换 12bit700ms
}



unsigned int DS1820_getTempValue(void)
{
    unsigned char i,j;
    unsigned int T;
     DS18b20_skipROM();
     DS18b20_write(0xBE); //读取温度的命令
     i=DS18b20_read();
     j=DS18b20_read();
     DS18b20_startADC();
     T = i+j*256;
     if (T==0xffff)
        return 0xffff;
     if (T>0x8000) //温度为负号
     {
          T = -T;
          return (0x8000+T*5/80);   // 计算温度的值
     }
     else                //温度为正值
          return (T*5/80);
}


//主函数
/*
void main(void)
{   
    int w;
    Init();//系统初始化
      
       while(1)
       {
        w = DS1820_getTempValue();  //将温度值送出
        _nop_;
        _nop_;
        _nop_;
       }
    //while(1);
}
*/