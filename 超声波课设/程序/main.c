
#include<REG52.h>
#include<intrins.h>
#include <uart.h>
#include"lcd.h"

sbit Trig = P1^0;
sbit Echo = P1^1;
sbit Trig1 = P1^2;
sbit Echo1 = P1^3;


unsigned char PuZh[]=" distance ";
unsigned char code ASCII[13] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};

static unsigned char DisNum = 0; //��ʾ��ָ��				  
       unsigned int  time=0;
	   unsigned long S=0,S1;
	   bit      flag =0;
	   unsigned char disbuff[4]	   ={ 0,0,0,0,};
	   unsigned char disbuff1[4]	   ={ 0,0,0,0,};
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/10;     //�������CM
	 if((S>=7000)||flag==1) //����������Χ��ʾ��-��
	 {	 
	     flag=0;
		 LCD_pos(0,0);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[10]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[12]);
	 }
	 else
	 {
	  disbuff[0]=S%10000/1000;
	  disbuff[1]=S%10000%1000/100;
	  disbuff[2]=S%10000%1000%100/10;
	  disbuff[3]=S%10000%10;
	  	 LCD_pos(0,0);
		 LcdWriteData(ASCII[disbuff[0]]);
		 LcdWriteData(ASCII[10]);
		 LcdWriteData(ASCII[disbuff[1]]);
		 LcdWriteData(ASCII[disbuff[2]]);
		 LcdWriteData(disbuff[3]+'0');
		 LcdWriteData(ASCII[12]);
		 UART_SendByte('A');
		 UART_SendByte(ASCII[disbuff[0]]);
		 UART_SendByte(ASCII[10]);
		 UART_SendByte(ASCII[disbuff[1]]);
		 UART_SendByte(ASCII[disbuff[2]]);
		 UART_SendByte(ASCII[disbuff[3]]);
		 UART_SendByte('\r');
		 UART_SendByte('\n');
	 }
	}
void Conut1(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S1=(time*1.7)/10;     //�������mM
	 if((S1>=7000)||flag==1) //����������Χ��ʾ��-��
	 {	 
	     flag=0;
		 LCD_pos(1,0);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[10]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[11]);
		 LcdWriteData(ASCII[12]);
	 }
	 else
	 {
	  disbuff1[0]=S1%10000/1000;
	  disbuff1[1]=S1%10000%1000/100;
	  disbuff1[2]=S1%10000%1000%100/10;
	  disbuff1[3]=S1%10000%10;
	  	 LCD_pos(1,0);
		 LcdWriteData(ASCII[disbuff1[0]]);
		 LcdWriteData(ASCII[10]);
		 LcdWriteData(ASCII[disbuff1[1]]);
		 LcdWriteData(ASCII[disbuff1[2]]);
		 LcdWriteData(disbuff1[3]+'0');
		 LcdWriteData(ASCII[12]);
		 UART_SendByte('B');
		 UART_SendByte(ASCII[disbuff1[0]]);
		 UART_SendByte(ASCII[10]);
		 UART_SendByte(ASCII[disbuff1[1]]);
		 UART_SendByte(ASCII[disbuff1[2]]);
		 UART_SendByte(ASCII[disbuff1[3]]);
		 UART_SendByte('\r');
		 UART_SendByte('\n');
	 }
	}
void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
  {
    flag=1;							 //�ж������־
  }

 void  StartModule() 		         //����ģ��
  {
	  Trig=1;			                     //����һ��ģ��
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
  }
void  StartModule1() 		         //����ģ��
  {
	  Trig1=1;			                     //����һ��ģ��
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig1=0;
  }


void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}

void main(void)
{

	 TMOD=0x01;		   //��T0Ϊ��ʽ1��GATE=1��
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //����T0�ж�
	 EA=1;			   //�������ж�	
	LcdInit();
	UartInit();
	while(1)
	{
		 StartModule();
	     while(!Echo);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(Echo);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut();			//����
		 delayms(30);
		 StartModule1();
		 while(!Echo1);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(Echo1);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut1();			//����
		 delayms(30);
	}
			
}