#include <REGX52.H>
#include <INTRINS.H>
#include "LCD1602.h"

sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;
                       //年、星期、月、日、时、分、秒   WP
unsigned char w_d[] = {0x8C,0x8A,0x88,0x86,0x84,0x82,0x80,0x8E};   

void Ds1302_init(void);
void DS1302_settime(unsigned char *Dtime);//对多个数据写入
void DS1302_readtime(unsigned char *Dtime);//对多个数据读出
unsigned char DS1302_readbyte(unsigned char command);//单个数据读出
void DS1302_writebyte(unsigned char command,Data);//单个数据写入
void Delay(unsigned int a);
void nixie(unsigned char a,b);

void main()
{                       //年 星期 月 日 时 分 秒 
	unsigned char Dtime[] = {26, 3, 1,21,23,00,59};//为了方便读写,用一个数组保存所有时间数据
	Ds1302_init();
	LCD_Init();
	DS1302_settime(Dtime);
	while(1)
	{
		DS1302_readtime();
		
		LCD_ShowNum(1,1,Dtime[0],2);
		LCD_ShowNum(1,4,Dtime[1],1);
		LCD_ShowNum(1,6,Dtime[2],2);
		LCD_ShowNum(1,9,Dtime[3],2);
		LCD_ShowNum(1,12,Dtime[4],2);
		LCD_ShowNum(1,15,Dtime[5],2);
		LCD_ShowNum(2,1,Dtime[6],2);
	}
}

void Ds1302_init(void)//初始化
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}
void DS1302_settime(unsigned char *Dtime)
{
	unsigned char i;
	DS1302_writebyte(w_d[7],0x00);//取消写保护
	
	for(i=0;i<7;i++)
		DS1302_writebyte(w_d[i],(Dtime[i]/10)*16+Dtime[i]%10);
	
	DS1302_writebyte(w_d[7],0x80);//打开写保护
}
void DS1302_readtime(unsigned char *Dtime)
{
	unsigned char t,i;
	
	for(i=0;i<7;i++)
	{
		t = DS1302_readbyte(w_d[i]);
		Dtime[i] = t/16*10 + t%16;
	}
}


void DS1302_writebyte(unsigned char command,Data)
{
	unsigned char i;
	DS1302_CE = 1;
	
	for(i=0;i<8;i++)
	{
		DS1302_IO = command&(0x01<<i);//拿到command的第一位
		DS1302_SCLK = 1;//注意需不需要加延时,防止速度过快
		DS1302_SCLK = 0;//每次电量下降,写入一个数据
	}
	
	for(i=0;i<8;i++)
	{
		DS1302_IO = Data&(0x01<<i);//拿到command的第一位
		DS1302_SCLK = 1;//注意需不需要加延时,防止速度过快
		DS1302_SCLK = 0;
	} 
	DS1302_CE=0;
}

unsigned char DS1302_readbyte(unsigned char command)
{
	unsigned char i,d=0x00;
	
	command |= 0x01;//由于读寄存器最后一位一定是1,所以直接将最后一位变为一
	DS1302_CE = 1;
	
	for(i=0;i<8;i++)
	{
		DS1302_IO = command&(0x01<<i);//拿到command的第一位
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK = 1; 
		DS1302_SCLK = 0;
		if(DS1302_IO)
			d |= 0x01<<i;//如果端口状态为1那就置为1
	}
	DS1302_CE = 0;
	DS1302_IO=0;
	return d;
}

void nixie(unsigned char a,b)
{
	unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;
	
	P0 = seg_code[b];
	Delay(1);
	P0 = 0x00;
}

void Delay(unsigned int a)
{
	unsigned char i,j;

	while(a)
	{
		_nop_();
		i = 4;
		j = 239;
		do
		{
			while(--j);
		}while(--i);
		a--;
	}
}