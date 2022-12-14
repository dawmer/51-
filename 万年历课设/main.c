/*************************************************************************************

*
实验现象：下载程序后插上LCD1602液晶在开发板上，即可显示
		  
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)

	
注意事项：根据自己使用的LCD1602是否带有转接板，如果带有转接板的即为4位，需在LCD.H头文件中
将宏#define LCD1602_4PINS打开，我们这里使用的LCD1602是8位，所以默认将该宏注释										

										  
**************************************************************************************

*/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
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
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
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
//设置 INT0
    IT0=1;//跳变沿出发方式（下降沿）
    EX0=1;//打开 INT0 的中断允许。
    EA=1;//打开总中断
}

void Int0() interrupt 0 //外部中断 0 的中断函数
{
    Delay1ms(10); //延时消抖	
		if(k3==0)
		{
		i++;
	  }
		 Delay1ms(100);
}
void Int1Init()
{
//设置 INT0
    IT1=1;//跳变沿出发方式（下降沿）
    EX1=1;//打开 INT0 的中断允许。
    EA=1;//打开总中断
}
void Int1() interrupt 2 //外部中断 1 的中断函数
{			
    Delay1ms(10); //延时消抖		
		if(k4==0)
		TIME[6-i]++;	
}
void main(void)
{
	  LcdInit();
    Ds1302Init();	   //第一次初始化后就可以注释该条语句，这样下次重启就不会再次?
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
	  LcdWriteCom(0x01);  //清屏	
    if(i==9)
    i=0;			
	}
}
