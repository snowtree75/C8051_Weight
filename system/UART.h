#ifndef __UART_H__
#define __UART_H__

void UART0Init();
void UART1Init();
void sendcom0(unsigned char dat);
void sendscom0(void *ptr,unsigned int n);

void sendcom1computer_byte(unsigned char dat);

void sendcom1computer_float(unsigned char mod,float dat);
#endif