#ifndef __SPI_H__
#define __SPI_H__

void SPIInit();

/**********************************************************
  SPI向AD7714写一个字节程序
**********************************************************/
void spiwrite(unsigned char ch,unsigned char channel);
/**********************************************************
  SPI从AD7714读一个字节程序
**********************************************************/
unsigned char spiread(unsigned char channel);

#endif