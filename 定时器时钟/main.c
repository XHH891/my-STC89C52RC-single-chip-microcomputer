#include <REGX52.H>

unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char sec=40,min=38,hour=15;

void Delay(unsigned int a);
void Timer0_Init();
void nixie(unsigned char a,b);

void main()
{
	sec=40;
	min=38;
	hour=15;
	Timer0_Init();
	while(1)
	{
		nixie(1,(hour/10)%10);
		Delay(1);
		nixie(2,hour%10);
		Delay(1);
		nixie(3,(min/10)%10);
		Delay(1);
		nixie(4,min%10);
		Delay(1);
		nixie(5,(sec/10)%10);
		Delay(1);
		nixie(6,sec%10);
		Delay(1);
	}
}



void Timer0_Routine(void) interrupt 1
{
	static unsigned int T;
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	T++;
	if(T >= 1000)
	{
		T = 0;
		sec++;
		if(sec >= 60)
		{
			sec = 0;
			min++;
			if(min>=60)
			{
				min=0;
				hour++;
				if(hour >= 24)
					hour = 0;
			}
		}
	}	
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

void Timer0_Init()
{
	//AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;		 	//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	//中断操作
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

void nixie(unsigned char a,b)
{
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