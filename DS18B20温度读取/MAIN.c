#include <REGX52.H>
#include "LCD1602.H"

/************************* ROM 指令集 *************************/
#define DS18B20_CMD_SEARCH_ROM      0xF0   // 搜索ROM
#define DS18B20_CMD_READ_ROM        0x33   // 读ROM
#define DS18B20_CMD_MATCH_ROM       0x55   // 匹配ROM
#define DS18B20_CMD_SKIP_ROM        0xCC  // 跳过ROM
#define DS18B20_CMD_ALARM_SEARCH    0xEC   // 报警搜索

/************************* 功能指令集 *************************/
#define DS18B20_CMD_CONVERT_T        0x44  // 温度转换
#define DS18B20_CMD_WRITE_SCRATCHPAD 0x4E  // 写暂存器
#define DS18B20_CMD_READ_SCRATCHPAD  0xBE  // 读暂存器
#define DS18B20_CMD_COPY_SCRATCHPAD  0x48  // 复制暂存器到EEPROM
#define DS18B20_CMD_RECALL_E2        0xB8  // 从EEPROM召回数据
#define DS18B20_CMD_READ_POWER_SUPPLY 0xB4 // 读供电模式

sbit DQ = P3^7;

/*-----------单总线相关函数---------*/
unsigned char onewire_init();
void one_sendbit(unsigned char Bit);//写1或者0  若写1,则经过10微秒后从0变1;若写0,则DQ = Bit;为0,经过两段时延后写入
unsigned char one_receivebit();//读1或者0
void one_sendbyte(unsigned char Byte);//写整个字节
unsigned char one_receivebyte();//读整个字节
/*-----------单总线相关函数---------*/

/*----------DS18B20相关函数----------*/
void ds18b20_convertT();
float ds18b20_readt();
/*----------DS18B20相关函数----------*/

void main()
{
	float T;
	
	LCD_Init();
	LCD_ShowString(1,1,"hello");
	
	while(1)
	{
		ds18b20_convertT();
		T = ds18b20_readt();
		if(T<0)
		{
			LCD_ShowChar(2,1,'-');
			T=-T;
		}
		else
			LCD_ShowChar(2,1,'+');
		
		LCD_ShowNum(2,2,T,3);
		LCD_ShowChar(2,5,'.');
		LCD_ShowNum(2,6,(unsigned long)(T*10000)%10000,4);
	}
}

/*----------DS18B20相关函数----------*/
void ds18b20_convertT(void)//触发温度转换
{
		onewire_init();
		one_sendbyte(DS18B20_CMD_SKIP_ROM);
		one_sendbyte(DS18B20_CMD_CONVERT_T);
}

float ds18b20_readt(void)//读取温度
{
		unsigned char tlsb,tmlb;
		int temp;
		float t;
		onewire_init();//单总线初始化：建立通信的前提
		one_sendbyte(DS18B20_CMD_SKIP_ROM);
		one_sendbyte(DS18B20_CMD_READ_SCRATCHPAD);//告诉传感器 “要读温度数据了”
		tlsb = one_receivebyte();// 先读低8位（暂存器第0字节）
		tmlb = one_receivebyte();// 后读高8位（暂存器第1字节）
		temp = (tmlb<<8)|tlsb;//把两个 8 位字节合成完整的温度数据
		t = temp/16.0;//把原始数据转换成可读的浮点温度
		return t;
}


/*----------DS18B20相关函数----------*/

/*-----------单总线相关函数---------*/
unsigned char onewire_init(void)//初始化
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

unsigned char one_receivebit(void)//读1或者0
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
				one_sendbit(Byte&(0x01<<i));
		}	
}

unsigned char one_receivebyte(void)//读整个字节
{
		unsigned char i,Byte=0x00;
		for(i=0;i<8;i++)
		{
				if(one_receivebit())
				{
					Byte |= (0x01<<i);
				}
		}
		return Byte;
}
/*-----------单总线相关函数---------*/