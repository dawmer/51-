
#include "reg52.h"			 
#include "zdinit.h"
#include <I2C.H>
#include "oled.h"
#include "ds1302.h"
//对数据类型进行声明定义
//typedef unsigned char u8;
//typedef unsigned int u16;


sbit k1=P3^0;
sbit k2=P3^1;
sbit k3=P3^2;
sbit k4=P3^3;
//u8 on[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char on[5]={0xfb,0xf7,0xef,0xdf,0xdf};
//#define SYSCLOCK	11059200		//晶振频率是11.0592MHZ
#define SYSCLOCK	12000000		//晶振频率是12MHZ

unsigned char heighHR0, heighLR0;//PWM一个周期中高电平高字节和低字节重载值
unsigned char lowHR0,lowLR0;//PWM一个机器周期中低电平时高字节和低字节重载值
void PWM_Config(unsigned int fr, unsigned char dc);
void PWM_Close();
char i=0,fx=0,sum[4],n,flag;
int cont=0,vint;
void UART_Init()
{
    TMOD=0x20;//设置定时器1的工作方式2---8位自动装填
	TH1=0xfd;//设置初始值:使比特率为9600bps
	TL1=0xfd;
	PCON=0x00;//SMOD=0，不加倍
	TR1=1;//打开定时器中断开关
	REN=1;//打开允许串行接收位
	SM0=0;//设置串行口通讯方式为方式1
	SM1=1;
//	EA=1; //打开总中断
}
// void Timer1init()
// {
//	 TMOD=0x10;          //工作于方式1
//	//0 0 方式0 13位计数器 TMOD=0x00
//	//0 1 方式1 16位计数器 TMOD=0x01
//	//1 0 方式2 自动重装8位计数器 TMOD=0x02
//	//1 1 方式3 T0分为2个8位独立计数器，T1为无中断重装8位计数器 TMOD=0x03
//    EA=1;              // 中断允许
//    ET1=1;						 // 中断1打开
//	TH1=(65536-50000)/256; 	     // (65536-50000)/256=60.6875
//	TL1=(65536-50000)%256;      
//	TR1=1;

// }
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;    //把Byte的值赋给缓存SBUF
	while(TI==0); //确认发送控制器TI标志位的状态，只有当TI不等于0时，才会跳出循环
	TI=0;         //清除发送完成标志位
}
void main()
{
	keyInit();
	OLED_Init();
	OLED_Clear();
	PCF8591init();
//	UART_Init();
	Ds1302Init();
//	Timer1init();
	while (1)     //呼吸灯效果
	{
		Ds1302ReadTime();
		vint=IRcvByte(0x90);
//		UART_SendByte(vint*5/255*10);
		if(fx==0)
		{
			cont+=(11-vint*5/255*2);
			if(cont>=80)
			{
				fx=1;
				sum[i]++;
			}				
		}
		else
		{
			cont-=(11-vint*5/255*2);
			if(cont<=0)
			{
				cont=1;
				fx=0;
			}				
		}
		//LED亮->暗
		PWM_Config(200, cont);//配置PWM频率为100HZ,占空比为cont%
		delayus(1000);
//		delayus(10000+vint*5/255*60000);
		for(n=0;n<4;n++)
		{
		OLED_ShowNum(n*32+2,0,sum[n],3,16);
		}
		OLED_ShowString(2,2,"2022",16);
		OLED_ShowChar(42,2,TIME[4]/16+'0',16);//月
		OLED_ShowChar(53,2,TIME[4]%16+'0',16);
		OLED_ShowChar(76,2,TIME[3]/16+'0',16);//日
		OLED_ShowChar(87,2,TIME[3]%16+'0',16);		
		OLED_ShowChar(2,4,TIME[2]/16+'0',16);
		OLED_ShowChar(13,4,TIME[2]%16+'0',16);
		OLED_ShowString(22,4,":",16);
		OLED_ShowChar(30,4,TIME[1]/16+'0',16);
		OLED_ShowChar(41,4,TIME[1]%16+'0',16);
		OLED_ShowString(50,4,":",16);
		OLED_ShowChar(58,4,TIME[0]/16+'0',16);
		OLED_ShowChar(69,4,TIME[0]%16+'0',16);
		OLED_ShowString(2,6,"vin:",8);
		OLED_ShowNum(32,6,vint*5/255,1,8);
		OLED_ShowString(41,6,".",8);
		OLED_ShowNum(45,6,vint*5*10/255%10,1,8);
		OLED_ShowString(66,6,"speed:",8);
		OLED_ShowNum(110,6,11-vint*5/255*2,2,8);
//		PWM_Close();
	} 
}




/*使用T0配置PWM高低电平的频率*/
void PWM_Config(unsigned int fr, unsigned char dc)  //fr为PWM的频率，100HZ无闪烁，dc是PWM的占空比(高电平占的百分比)
{
	long tmp;
	unsigned int heigh, low;
	
	tmp = SYSCLOCK/12;//一个机器周期频率
	tmp = tmp / fr;  //一个机器周期中fr的频率所需计数值(包括高电平和低电平)
	heigh = (tmp * dc) / 100;//高电平占dc%，所需计数值
	low   = tmp - heigh;//低电平所需计数值
	heigh = 65536-heigh;//高电平计数初值
	low   = 65536-low;//低电平计数初值
	
	heighHR0 = (unsigned char)(heigh >> 8);//高电平装入初值的高字节
	heighLR0 = (unsigned char)heigh;    //高电平装入初值的低字节
	lowHR0   = (unsigned char)(low >> 8);  //低电平装入初值的高字节
	lowLR0   = (unsigned char)low;    //低电平装入初值的低字节
	
	TMOD &= 0xF0;//清零T0控制位
	TMOD |= 0x01;//设定T0工作方式1,16位可设定时器
	TH0 = heighHR0;
	TL0 = heighLR0;//装入高电平计数初值
//	P2 = 0xff;//开始时PWM输出高电平
	TR0 = 1;
	ET0 = 1;
	EA  = 1;
}


void PWM_Close()
{
	TR0 = 0;//关闭Timer0
	ET0 = 0;//关闭T0中断
	P2 = 0XFF;//PWM输出高电平
}
/*定时器T0中断服务*/
void Timer0_ISP() interrupt 1
{
	//判断PWM现在输出的是高电平还是低电平
	if (P2==0xff)//P2==0xff P2&&(0x04<<i)
	{
		TH0 = lowHR0;
		TL0 = lowLR0;//装载低电平计数初值
		P2 =on[i];//PWM输出低电平
	}
	else
	{
		TH0 = heighHR0;
		TL0 = heighLR0;//重装装载高电平计数初值
		P2 =0xff;//PWM输出高电平
	}
}

void extern0_ISP() interrupt 0
{
	if(k3==0)
	{
		delayus(1000);
		if(k3==0)
		{
			cont=1;
			fx=0;
			i++;
			if(i>=4)
				i=0;
		}
	}
		
}


