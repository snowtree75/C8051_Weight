#include "includes.h"
#include "IE.h"

void IE_enable(void){
  EIE1=0xa0;//�Ƚ���0,1�������ж�ʼ��1010 xxxx
	ES0=1;//////UART0�ж�ʹ��
	EIE2|=0x40;//ES1=1;//////UART1�ж�ʹ��
	EIE1|=0x02;//IIC�ж�ʹ��
	EX0=1;
	ET0=1;
	EIE2 |= 0x02;   
  EA=1;///////���ж�ʹ��
}

/*

	EIE1=0xa0;//�Ƚ���0,1�������ж�ʼ��1010 xxxx
	ES0=1;//////UART0�ж�ʹ��
	EIE2|=0x40;//ES1=1;//////UART1�ж�ʹ��
	EIE1|=0x02;//IIC�ж�ʹ��
	EA=1;///////���ж�ʹ��

*/