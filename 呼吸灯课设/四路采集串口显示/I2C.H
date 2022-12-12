#ifndef __i2c_h_
#define __i2c_h_ 
extern bit ack;
//起动总线函数
extern void Start_I2c();
//结束总线函数  
extern void Stop_I2c();
//应答子函数
extern void Ack_I2c(bit a);
//字节数据发送函数
extern void  SendByte(unsigned char  c);
//有子地址发送多字节数据函数               
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;
//无子地址发送多字节数据函数   
extern bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);
//无子地址读字节数据函数               
extern unsigned char RcvByte();
void PCF8591init();
unsigned char IRcvByte(unsigned char sla);
extern unsigned int  D[5];
#endif           
