#include <REGX52.H>
#include "LCD1602.H"

sbit DQ = P3^7;

unsigned char onewire_init()//初始化
{
		unsigned char i,ackbit;
		DQ = 1;
		DQ = 0;
		i = 227;while (--i);  //delay 500us
		DQ = 1;
		i = 29;while (--i); //delay 70us
		ackbit = DQ;
		i = 227;while (--i);  //delay 500us
		return ackbit;
}

void one_sendbit(unsigned char Bit)//写1或者0  若写1,则经过10微秒后从0变1;若写0,则DQ = Bit;为0,经过两段时延后写入
{
		unsigned char i;
	
		DQ = 0;
		i = 4;while (--i);  //delay 10us
		DQ = Bit;
		i = 22;while (--i); //delay 50us
		DQ = 1;
}

unsigned char one_receivebit()//读1或者0
{
		unsigned char i,Bit;
		
		DQ = 0;
		i = 2;while (--i);  //delay 5us
		DQ = 1;
		i = 2;while (--i);  //delay 5us
		Bit = DQ;
		i = 22;while (--i); //delay 50us
	
		return Bit;
}

void one_sendbyte(unsigned char Byte)//写整个字节
{
		unsigned char i;
	
		for(i=0;i<8;i++)
		{
				one_sendbit(Byte&(0x00<<i));
		}	
}

unsigned char one_receivebyte()
{
		unsigned char i,Byte;
		for(i=0;i<8;i++)
		{
				if(one_receivebit())
				{
					Byte |= (0x01<<i);
				}
		}
		return Byte;
}

void main()
{
	unsigned char ack=1;
	LCD_Init();
	ack = onewire_init();
	LCD_ShowNum(1,1,ack,3);
	while(1)
	{
	
	}
}