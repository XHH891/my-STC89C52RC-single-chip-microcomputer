#include <REGX52.H>

unsigned char keyo=0;

void Timer0_Init();
unsigned char key();
unsigned char Keyr();
void key_loop();

void nixie(unsigned char a,b);
void Delay(unsigned int a);

void main()
{
	unsigned char a,b;
	
	Timer0_Init();
	while(1)
	{
		a = key();
		if(a)
			b = a;
		nixie(1,b);
	}
}

unsigned char key()
{
	unsigned char temp=0;
	temp = keyo;
	keyo = 0;
	return temp;
}

unsigned char Keyr()
{
	unsigned char KeyNumber=0;
	if(P3_1==0) {KeyNumber=1;}
	if(P3_0==0) {KeyNumber=2;}
	if(P3_2==0) {KeyNumber=3;}
	if(P3_3==0) {KeyNumber=4;}
	return KeyNumber;
}

void key_loop(void)
{
	static unsigned char nowstate,laststate;
	
	laststate = nowstate;
	nowstate = Keyr();
	if(laststate==1 && nowstate==0)
	{
		keyo = 1;
	}
	if(laststate==2 && nowstate==0)
	{
		keyo = 2;
	}
	if(laststate==3 && nowstate==0)
	{
		keyo = 3;
	}
	if(laststate==4 && nowstate==0)
	{
		keyo = 4;
	}
}

unsigned int T=0;
void Timer0_Routine(void) interrupt 1
{
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	T++;
	if(T >= 20)
	{
		T = 0;
		key_loop();
	}	
}

void Timer0_Init()
{
	//AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;		  	//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
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
	unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;
	
	P0 = seg_code[b];
	//Delay(1);
	//P0 = 0x00;
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