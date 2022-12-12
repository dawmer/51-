/*************************************************************************************

*
ʵ���������س�������LCD1602Һ���ڿ������ϣ�������ʾ
		  
����˵���� (�������ͼ�ɼ��������Զ�Ӧʵ��ġ�ʵ�������½�)

	
ע����������Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע��										

										  
**************************************************************************************

*/

#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "lcd.h"
#include"ds1302.h"
#include"temp.h"
void Int1Init();
void Int0Init();
sbit k3=P3^2;
sbit k4=P3^3;
uint a[7];
//extern sj[12];
int i=0;
extern uint TIME[7];

/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LCD_pos(uchar x,uchar y)
{
	uchar pos;
  if(x==0)x=0x80;
  else if(x==1)x=0x90;
	else if(x==2)x=0x88;
	else if(x==3)x=0x98;
	pos=x+y;
	LcdWriteCom(pos);
}
void Int0Init()
{
//���� INT0
    IT0=1;//�����س�����ʽ���½��أ�
    EX0=1;//�� INT0 ���ж�����
    EA=1;//�����ж�
}

void Int0() interrupt 0 //�ⲿ�ж� 0 ���жϺ���
{
    Delay1ms(10); //��ʱ����	
		if(k3==0)
		{
		i++;
	  }
		 Delay1ms(100);
}
void Int1Init()
{
//���� INT0
    IT1=1;//�����س�����ʽ���½��أ�
    EX1=1;//�� INT0 ���ж�����
    EA=1;//�����ж�
}
void Int1() interrupt 2 //�ⲿ�ж� 1 ���жϺ���
{			
    Delay1ms(10); //��ʱ����		
		if(k4==0)
		TIME[6-i]++;	
}
void main(void)
{
	  LcdInit();
    Ds1302Init();	   //��һ�γ�ʼ����Ϳ���ע�͸�����䣬�����´������Ͳ����ٴγ
	while(1)
	{
		Int0Init();
		Int1Init();
		wdh();
		if(i==7)
	  Ds1302Init();
		sjh();
		LCD_pos(0,0);
		LcdWriteData('2');
		LcdWriteData('0');	
		LcdWriteData('0'+sj[0]);
		LcdWriteData('0'+sj[1]);		
		LcdWriteData('-');
		LcdWriteData('0'+sj[2]);
		LcdWriteData('0'+sj[3]);
		LcdWriteData('-');
		LcdWriteData('0'+sj[4]);
		LcdWriteData('0'+sj[5]);
		LCD_pos(1,0);
		LcdWriteData('0'+sj[6]);
		LcdWriteData('0'+sj[7]);
		LcdWriteData('-');
		LcdWriteData('0'+sj[8]);
		LcdWriteData('0'+sj[9]);
		LcdWriteData(':');
		LcdWriteData('0'+sj[10]);
		LcdWriteData('0'+sj[11]);
		LCD_pos(3,0);	
		LcdWriteData('0'+i);
		if(i==7)
	  LcdWriteCom(0x01);  //����	
    if(i==9)
    i=0;			
	}
}
