#include <REGX52.H>
#include "LCD1602.h"

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

#define a_address 0xA0    //at24c04的地址

void Delay(unsigned int a); 

void i2c_start(void);//发送接收的S表示开始
void i2c_stop(void);//发送接收的P表示停止

void i2c_semdbyte(unsigned char byte);//写入一个字节
unsigned char i2c_receivebyte(void);//接收一个字节

void i2c_sendack(unsigned char abit);//发送应答
unsigned char i2c_receiveack();//接收应答

void at24c04_wb(unsigned char wa,Data);//对at24c04写一个字节
unsigned char at24c04_rb(unsigned char wa);//对at24c04读一个字节  wa表示寄存器

void main()
{
	unsigned char d;
	LCD_Init();
	
	at24c04_wb(1,66);
	Delay(20);
	d = at24c04_rb(1);
	LCD_ShowNum(2,1,d,3);
	while(1)
	{
		
	}
}

void at24c04_wb(unsigned char wa,Data)//对at24c04写一个字节  wa表示寄存器
{
	i2c_start();
	i2c_semdbyte(a_address);
	i2c_receiveack();
	i2c_semdbyte(wa);
	i2c_receiveack();
	i2c_semdbyte(Data);
	i2c_receiveack();
	i2c_stop();
}

unsigned char at24c04_rb(unsigned char wa)//对at24c04读一个字节  wa表示寄存器
{
	unsigned char Data;
	
	i2c_start();
	i2c_semdbyte(a_address);
	i2c_receiveack();
	i2c_semdbyte(wa);
	i2c_receiveack();
	
	i2c_start();
	i2c_semdbyte(a_address|0x01);//原本a_address表示写地址,将最后一位变为1,变为读地址
	i2c_receiveack();
	Data = i2c_receivebyte();
	i2c_sendack(1);
	
	i2c_stop();
	return Data;
}

void i2c_start(void)//发送接收的S 表示开始
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
}

void i2c_stop(void)//发送接收的P 表示停止
{
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}

void i2c_semdbyte(unsigned char byte)//写入一个字节
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA = (byte&(0x80>>i));
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}
	
unsigned char i2c_receivebyte(void)//接收一个字节
{
	unsigned char i,byte=0x00;
	
	I2C_SDA = 1;
	for(i=0;i<8;i++)
	{
		I2C_SCL = 1;
		if(I2C_SDA)
			byte |= (0x80>>i); 
		I2C_SCL = 0;
	}
	return byte;
}

void i2c_sendack(unsigned char abit)//发送应答
{
		I2C_SDA = abit;
		I2C_SCL = 0;
	  I2C_SCL = 1;
}

unsigned char i2c_receiveack()//接收应答
{
		unsigned char abit;
		I2C_SDA = 1;
		I2C_SCL = 1;
		abit = I2C_SDA;
	  I2C_SCL = 0;
		return abit;
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