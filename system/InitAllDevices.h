#ifndef __INITALLDEVICES_H__
#define __INITALLDEVICES_H__
#include "config.h"
#include "miscellaneous.h"
#include "timer.h"
#include "uart.h"
#include "Oscillator.h"
#include "PORT.h"
#include "IE.h"
#include "SMBus.h"
#include "ADC7714.h"
#include "SPI.h"
#include "ADC.h"
#include "VoltageReference.h"
#include "Comparator.h"
#include "DeviceStatus.h"

/*******************************************************
	˵������ʼ�������豸��ʹ�õĸ���Ӳ����ʹ�䰲װ�û�������
				����
********************************************************/
void Init_Device(void);
/*******************************************************
	˵������ʼ�������������ʹ�õı���
********************************************************/
void InitAllDatas(void);

#endif
