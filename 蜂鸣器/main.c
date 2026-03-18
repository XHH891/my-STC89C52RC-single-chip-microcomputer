#include <REGX52.H>

unsigned char keynum;
unsigned int i;

sbit buzzer = P2^5;

unsigned char key();
void nixie(unsigned char a,b);
void buzzer_time(unsigned char ms);
void Delay(unsigned int a);

void main()
{
		while(1)
		{
			keynum = key();
			if(keynum)
			{
				buzzer_time(50);
				nixie(1,keynum);
			}
		}
}

void buzzer_time(unsigned char ms)
{
	unsigned int i;
	for(i=0;i<ms;i++)
	{
		buzzer = !buzzer;
		Delay(1);
	}
}

unsigned char key()
{
	if(P3_1 == 0)
	{
		Delay(1);
		while(P3_1 ==0);
		Delay(1);
		return 1;
	}
	if(P3_0 == 0)
	{
		Delay(1);
		while(P3_0 ==0);
		Delay(1);
		return 2;
	}
	if(P3_2 == 0)
	{
		Delay(1);
		while(P3_2 ==0);
		Delay(1);
		return 3;
	}
	if(P3_3 == 0)
	{
		Delay(1);
		while(P3_3 ==0);
		Delay(1);
		return 4;
	}
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