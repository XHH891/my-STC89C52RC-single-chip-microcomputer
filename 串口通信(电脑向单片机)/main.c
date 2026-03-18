//电脑向单片机发送数据
#include <REGX52.H>

void Delay(unsigned int a);
void nixie(unsigned char a,b);
void UART_Init();
void UART_Routine();
void UART_SendByte(unsigned char Byte);

unsigned int a;

void main()
{	
	
	UART_Init();
	while(1)
	{

	}
}

void UART_SendByte(unsigned int Byte)//发送数据
{
	SBUF=Byte;
	while(TI==0);//如果数据比较多,会发送一段时间,期间TI=0,当发送完成是变为1
	TI=0;//重置TI,为下一次做准备
}

//串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI == 1)//中断分为两种,发送中断和接收中断,接收中断为1
	{
		P2 = SBUF;
		UART_SendByte('<');
		RI = 0;
	}
}

void UART_Init()//配置寄存器
{
	SCON = 0x50;
	PCON &= 0x7F;
	TMOD &= 0x0F;		 	//设置定时器模式
	TMOD |= 0x20;			//设置定时器模式
	TL1 = 0xFA;			//设置定时初始值
	TH1 = 0xFA;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	EA  = 1;
	ES = 1;
}	

void Delay(unsigned int a)
{
	unsigned char i,j;

	while(a)
	{
		i = 4;
		j = 239;
		do
		{
			while(--j);
		}while(--i);
		a--;
	}
}
