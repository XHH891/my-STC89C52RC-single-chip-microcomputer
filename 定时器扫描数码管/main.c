#include <REGX52.H>

unsigned char n_buf[9]={0,10,10,10,10,10,10,10,10};
unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x00};

void Timer0_Init();
void nixie(unsigned char a,b);
void n_loop(void);
void nixie_setbuf(unsigned char a,b);

void main()
{
	Timer0_Init();
	while(1)
	{
	
	}
	
}

void nixie_setbuf(unsigned char a,b)
{
	n_buf[a] = b;
}

void nixie(unsigned char a,b)
{
	
	P0=0x00;
	
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;

	P0 = seg_code[b];
}

void n_loop(void)
{
	static unsigned char i;
	nixie(i,n_buf[i]);
	i++;
	if(i>8)
		i=1;
}

void Timer0_Routine(void) interrupt 1
{
	static unsigned int T;
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	T++;
	if(T>=2)
	{
		T = 0;
		n_loop();
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

