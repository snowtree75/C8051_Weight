#ifndef __SPI_H__
#define __SPI_H__

void SPIInit();

/**********************************************************
  SPI��AD7714дһ���ֽڳ���
**********************************************************/
void spiwrite(unsigned char ch,unsigned char channel);
/**********************************************************
  SPI��AD7714��һ���ֽڳ���
**********************************************************/
unsigned char spiread(unsigned char channel);

#endif