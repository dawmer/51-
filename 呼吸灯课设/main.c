
#include "reg52.h"			 
#include "zdinit.h"
#include <I2C.H>
#include "oled.h"
#include "ds1302.h"
//���������ͽ�����������
//typedef unsigned char u8;
//typedef unsigned int u16;


sbit k1=P3^0;
sbit k2=P3^1;
sbit k3=P3^2;
sbit k4=P3^3;
//u8 on[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char on[5]={0xfb,0xf7,0xef,0xdf,0xdf};
//#define SYSCLOCK	11059200		//����Ƶ����11.0592MHZ
#define SYSCLOCK	12000000		//����Ƶ����12MHZ

unsigned char heighHR0, heighLR0;//PWMһ�������иߵ�ƽ���ֽں͵��ֽ�����ֵ
unsigned char lowHR0,lowLR0;//PWMһ�����������е͵�ƽʱ���ֽں͵��ֽ�����ֵ
void PWM_Config(unsigned int fr, unsigned char dc);
void PWM_Close();
char i=0,fx=0,sum[4],n,flag;
int cont=0,vint;
void UART_Init()
{
    TMOD=0x20;//���ö�ʱ��1�Ĺ�����ʽ2---8λ�Զ�װ��
	TH1=0xfd;//���ó�ʼֵ:ʹ������Ϊ9600bps
	TL1=0xfd;
	PCON=0x00;//SMOD=0�����ӱ�
	TR1=1;//�򿪶�ʱ���жϿ���
	REN=1;//�������н���λ
	SM0=0;//���ô��п�ͨѶ��ʽΪ��ʽ1
	SM1=1;
//	EA=1; //�����ж�
}
// void Timer1init()
// {
//	 TMOD=0x10;          //�����ڷ�ʽ1
//	//0 0 ��ʽ0 13λ������ TMOD=0x00
//	//0 1 ��ʽ1 16λ������ TMOD=0x01
//	//1 0 ��ʽ2 �Զ���װ8λ������ TMOD=0x02
//	//1 1 ��ʽ3 T0��Ϊ2��8λ������������T1Ϊ���ж���װ8λ������ TMOD=0x03
//    EA=1;              // �ж�����
//    ET1=1;						 // �ж�1��
//	TH1=(65536-50000)/256; 	     // (65536-50000)/256=60.6875
//	TL1=(65536-50000)%256;      
//	TR1=1;

// }
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;    //��Byte��ֵ��������SBUF
	while(TI==0); //ȷ�Ϸ��Ϳ�����TI��־λ��״̬��ֻ�е�TI������0ʱ���Ż�����ѭ��
	TI=0;         //���������ɱ�־λ
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
	while (1)     //������Ч��
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
		//LED��->��
		PWM_Config(200, cont);//����PWMƵ��Ϊ100HZ,ռ�ձ�Ϊcont%
		delayus(1000);
//		delayus(10000+vint*5/255*60000);
		for(n=0;n<4;n++)
		{
		OLED_ShowNum(n*32+2,0,sum[n],3,16);
		}
		OLED_ShowString(2,2,"2022",16);
		OLED_ShowChar(42,2,TIME[4]/16+'0',16);//��
		OLED_ShowChar(53,2,TIME[4]%16+'0',16);
		OLED_ShowChar(76,2,TIME[3]/16+'0',16);//��
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




/*ʹ��T0����PWM�ߵ͵�ƽ��Ƶ��*/
void PWM_Config(unsigned int fr, unsigned char dc)  //frΪPWM��Ƶ�ʣ�100HZ����˸��dc��PWM��ռ�ձ�(�ߵ�ƽռ�İٷֱ�)
{
	long tmp;
	unsigned int heigh, low;
	
	tmp = SYSCLOCK/12;//һ����������Ƶ��
	tmp = tmp / fr;  //һ������������fr��Ƶ���������ֵ(�����ߵ�ƽ�͵͵�ƽ)
	heigh = (tmp * dc) / 100;//�ߵ�ƽռdc%���������ֵ
	low   = tmp - heigh;//�͵�ƽ�������ֵ
	heigh = 65536-heigh;//�ߵ�ƽ������ֵ
	low   = 65536-low;//�͵�ƽ������ֵ
	
	heighHR0 = (unsigned char)(heigh >> 8);//�ߵ�ƽװ���ֵ�ĸ��ֽ�
	heighLR0 = (unsigned char)heigh;    //�ߵ�ƽװ���ֵ�ĵ��ֽ�
	lowHR0   = (unsigned char)(low >> 8);  //�͵�ƽװ���ֵ�ĸ��ֽ�
	lowLR0   = (unsigned char)low;    //�͵�ƽװ���ֵ�ĵ��ֽ�
	
	TMOD &= 0xF0;//����T0����λ
	TMOD |= 0x01;//�趨T0������ʽ1,16λ���趨ʱ��
	TH0 = heighHR0;
	TL0 = heighLR0;//װ��ߵ�ƽ������ֵ
//	P2 = 0xff;//��ʼʱPWM����ߵ�ƽ
	TR0 = 1;
	ET0 = 1;
	EA  = 1;
}


void PWM_Close()
{
	TR0 = 0;//�ر�Timer0
	ET0 = 0;//�ر�T0�ж�
	P2 = 0XFF;//PWM����ߵ�ƽ
}
/*��ʱ��T0�жϷ���*/
void Timer0_ISP() interrupt 1
{
	//�ж�PWM����������Ǹߵ�ƽ���ǵ͵�ƽ
	if (P2==0xff)//P2==0xff P2&&(0x04<<i)
	{
		TH0 = lowHR0;
		TL0 = lowLR0;//װ�ص͵�ƽ������ֵ
		P2 =on[i];//PWM����͵�ƽ
	}
	else
	{
		TH0 = heighHR0;
		TL0 = heighLR0;//��װװ�ظߵ�ƽ������ֵ
		P2 =0xff;//PWM����ߵ�ƽ
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


