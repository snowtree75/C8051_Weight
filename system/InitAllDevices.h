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
	说明：初始化仪器设备中使用的各种硬件，使其安装用户的需求
				工作
********************************************************/
void Init_Device(void);
/*******************************************************
	说明：初始化软件正常工作使用的变量
********************************************************/
void InitAllDatas(void);

#endif
