#include <REGX52.H>

sbit motor = P1^0;

/*========数码管=========*/
void nixie(unsigned char a,b);
void n_loop(void);
void nixie_setbuf(unsigned char a,b);
unsigned char n_buf[9]={0,10,10,10,10,10,10,10,10};
/*========数码管=========*/

/*========按键===========*/
unsigned char key();
unsigned char Keyr();
void k_loop();
unsigned char keyo=0;
/*========按键===========*/

/*========PWM============*/
unsigned char counter,compare;
/*========PWM============*/

void Delay(unsigned int a);
void Timer0_Init();

void main()
{
	unsigned char keynum,speed=0;
		Timer0_Init();
		
		while(1)
		{
				keynum = key();
			if(keynum == 1)
			{
				speed++;
				speed%=4;
				if(speed == 0){compare=0;}
				if(speed == 1){compare=50;}
				if(speed == 2){compare=75;}
				if(speed == 3){compare=100;}
			}
			nixie_setbuf(1,speed);
		}
}

/*========PWM============*/

/*========PWM============*/

/*========按键===========*/
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

void k_loop(void)
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
/*========按键===========*/

/*========数码管=========*/
void nixie_setbuf(unsigned char a,b)
{
	n_buf[a] = b;
}
void nixie(unsigned char a,b)
{
	unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x00};
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
/*========数码管=========*/
unsigned int t1,t2;
void Timer0_Routine(void) interrupt 1
{
	
	TL0 = 0xA4;			  //设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	
	counter++;
	counter%=100;
	if(counter<compare)
		motor = 1;
	else
		motor = 0;
	
	t1++;
	t2++;
	if(t1>=2)
	{
		t1 = 0;
		n_loop();
	}
	if(t2>=20)
	{
		t2 = 0;
		k_loop();
	}
}

void Timer0_Init()
{
	//AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;		 	//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0xA4;			//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	//中断操作
	ET0 = 1;
	EA = 1;
	PT0 = 0;
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