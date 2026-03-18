#include <REGX52.H>
#include "LCD1602.h"

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

void Delay(unsigned int a); 
void i2c_start(void);//发送接收的S表示开始
void i2c_stop(void);//发送接收的P表示停止
void i2c_semdbyte(unsigned char byte);//写入一个字节
unsigned char i2c_receivebyte(void);//接收一个字节
void i2c_sendack(unsigned char abit);//发送应答
unsigned char void i2c_receiveack();//接收应答


void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"Hello");
	while(1)
	{
		
	}
}

void i2c_start(void)//发送接收的S 表示开始
{
	I2C_SCL = 1;
	I2C_SDA = 1;
	I2C_SCL = 0;
	I2C_SDA = 0;
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
		I2C_SDA = byte&(0x80>>i);
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}
	
unsigned char i2c_receivebyte(void)//接收一个字节
{
	unsigned char i,byte;
	
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

unsigned char void i2c_receiveack()//接收应答
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