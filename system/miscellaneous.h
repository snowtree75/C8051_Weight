#ifndef __MISCELLANEOUS_H__ 
#define __MISCELLANEOUS_H__ 

void getPressData(void);
void delayms(unsigned int t);
void wait(unsigned int n);
void buzzer();
void ResetDevice();
void InitAllDatas(void);
void updateDensor();
unsigned char generateCRC(unsigned char* ptr,int len);
#endif
