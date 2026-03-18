#include <REGX52.H>
#include "LCD1602.H"

sbit xpt2046_cs   = P3^5;
sbit xpt2046_dclk = P3^6;
sbit xpt2046_din  = P3^4;
sbit xpt2046_dout = P3^7;

#define XPT2046_XP_8      0x9C    //0x8C
#define XPT2046_YP_8      0xDC
#define XPT2046_VBAT_8    0xAC
#define XPT2046_AUX_8     0xEC

#define XPT2046_XP_12     0x94    //0x84
#define XPT2046_YP_12     0xD4
#define XPT2046_VBAT_12   0xA4
#define XPT2046_AUX_12    0xE4

unsigned int xpt2046_readAD(unsigned char command);
void Delay(unsigned int a);

void main()
{
		unsigned char advalue;
		LCD_Init();
		LCD_ShowString(1,1,"hello");
		while(1)
		{
				  advalue = xpt2046_readAD(XPT2046_XP_12);
				  LCD_ShowNum(2,1,advalue,3);
					advalue = xpt2046_readAD(XPT2046_YP_12);
				  LCD_ShowNum(2,5,advalue,3);
					advalue = xpt2046_readAD(XPT2046_VBAT_12);
				  LCD_ShowNum(2,9,advalue,3);
					Delay(5);
		}
}

unsigned int xpt2046_readAD(unsigned char command)
{
		unsigned char i;
		unsigned int advlue=0;
	
		xpt2046_cs = 0;
		xpt2046_dclk = 0;
		
		for(i=0;i<8;i++)
		{
				xpt2046_din = command&(0x80>>i);
				xpt2046_dclk = 1;
				xpt2046_dclk = 0;
		}
		
		for(i=0;i<16;i++)
		{
				xpt2046_dclk = 1;
				xpt2046_dclk = 0;
				if(xpt2046_dout){advlue|=(0x800>>i);}
		}
		xpt2046_cs = 1;
		
		if(command&0x08)
			return advlue>>8;
		else
			return advlue>>4;
		
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