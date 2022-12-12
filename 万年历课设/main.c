/*************************************************************************************

*
ÊµÑéÏÖÏó£ºÏÂÔØ³ÌĞòºó²åÉÏLCD1602Òº¾§ÔÚ¿ª·¢°åÉÏ£¬¼´¿ÉÏÔÊ¾
		  
½ÓÏßËµÃ÷£º (¾ßÌå½ÓÏßÍ¼¿É¼û¿ª·¢¹¥ÂÔ¶ÔÓ¦ÊµÑéµÄ¡°ÊµÑéÏÖÏó¡±ÕÂ½Ú)

	
×¢ÒâÊÂÏî£º¸ù¾İ×Ô¼ºÊ¹ÓÃµÄLCD1602ÊÇ·ñ´øÓĞ×ª½Ó°å£¬Èç¹û´øÓĞ×ª½Ó°åµÄ¼´Îª4Î»£¬ĞèÔÚLCD.HÍ·ÎÄ¼şÖĞ
½«ºê#define LCD1602_4PINS´ò¿ª£¬ÎÒÃÇÕâÀïÊ¹ÓÃµÄLCD1602ÊÇ8Î»£¬ËùÒÔÄ¬ÈÏ½«¸Ãºê×¢ÊÍ										

										  
**************************************************************************************

*/

#include "reg52.h"			 //´ËÎÄ¼şÖĞ¶¨ÒåÁËµ¥Æ¬»úµÄÒ»Ğ©ÌØÊâ¹¦ÄÜ¼Ä´æÆ÷
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
* º¯ Êı Ãû         : main
* º¯Êı¹¦ÄÜ		   : Ö÷º¯Êı
* Êä    Èë         : ÎŞ
* Êä    ³ö         : ÎŞ
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
//ÉèÖÃ INT0
    IT0=1;//Ìø±äÑØ³ö·¢·½Ê½£¨ÏÂ½µÑØ£©
    EX0=1;//´ò¿ª INT0 µÄÖĞ¶ÏÔÊĞí¡£
    EA=1;//´ò¿ª×ÜÖĞ¶Ï
}

void Int0() interrupt 0 //Íâ²¿ÖĞ¶Ï 0 µÄÖĞ¶Ïº¯Êı
{
    Delay1ms(10); //ÑÓÊ±Ïû¶¶	
		if(k3==0)
		{
		i++;
	  }
		 Delay1ms(100);
}
void Int1Init()
{
//ÉèÖÃ INT0
    IT1=1;//Ìø±äÑØ³ö·¢·½Ê½£¨ÏÂ½µÑØ£©
    EX1=1;//´ò¿ª INT0 µÄÖĞ¶ÏÔÊĞí¡£
    EA=1;//´ò¿ª×ÜÖĞ¶Ï
}
void Int1() interrupt 2 //Íâ²¿ÖĞ¶Ï 1 µÄÖĞ¶Ïº¯Êı
{			
    Delay1ms(10); //ÑÓÊ±Ïû¶¶		
		if(k4==0)
		TIME[6-i]++;	
}
void main(void)
{
	  LcdInit();
    Ds1302Init();	   //µÚÒ»´Î³õÊ¼»¯ºó¾Í¿ÉÒÔ×¢ÊÍ¸ÃÌõÓï¾ä£¬ÕâÑùÏÂ´ÎÖØÆô¾Í²»»áÔÙ´Î³
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
	  LcdWriteCom(0x01);  //ÇåÆÁ	
    if(i==9)
    i=0;			
	}
}
