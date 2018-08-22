#ifndef __DEVICESTATUS_H__
#define __DEVICESTATUS_H__
/***********************************************
说明：为了显示单片机与上位机的通信状态，使用一个
			连续计数的变量
***********************************************/
extern unsigned long count;
/***********************************************
说明：保存了板载CPS120传感器测量的大气压强与温度值
			分别以无符号长整型与double类型保存
***********************************************/
extern unsigned long 		ulDeviceAtm;
extern unsigned long  	ulDeviceTempture;

extern float 						fDeviceAtm;
extern float  					fDeviceTempture;

/***********************************************
说明：保存了5路7714连接的温度值
***********************************************/
extern unsigned long temperature_value[5];
extern unsigned char temperature_flag[5];

extern float 				fDeviceTemperature[5];

/***********************************************
说明：保存ADC0转换的电位值
***********************************************/
extern unsigned int uiDeviceP1FirstPointDianWei;
extern unsigned int uiDeviceP1FirstVolumnDianWei;

extern unsigned int uiDeviceP2FirstPointDianWei;
extern unsigned int uiDeviceP2FirstVolumnDianWei;

/***********************************************
说明：保存串口从其他设备发来的数据
***********************************************/
extern char   					ucCom0ReceiveByte[32]; // 天平
extern unsigned char   	ucCom1ReceiveByte[256]; // 上位机

extern unsigned char		ucCom0ReceivePointer;
extern unsigned char		ucCom1ReceivePointer;
/***********************************************
说明：保存着通过串口0发来的重量数据
***********************************************/
extern float		fTotalWeight;//样品的总重量
extern float 		fFlask1Weight;
extern float 		fFlask2Weight;
extern float		fCurPurePervious100Weight;
extern float 		fCurPureNext100Weight;
extern float		fCurWeight; 	//回收液体的重量
extern float 		fCurPureWeight;
extern float 		fCurWeightPer;
extern float 		fCurPurePervious100Weight;
/***********************************************
说明：保存着通过串口1发来的加热炉子功率
***********************************************/
extern unsigned int uiPowerPeriod; // 功率为100%时的计数
extern unsigned int uiCurPower; 		// 功率使用占空比表示
extern unsigned int uiPowerCnt;		// 功率计数

//////
extern unsigned int uiPower1;
extern unsigned int uiPower2;
extern unsigned int uiPower3;
extern unsigned int uiPower4;
/***********************************************
说明：保存着仪器工作状态和计时时间
***********************************************/
extern unsigned char ucCurDeviceStatus;//仪器工作状态
extern unsigned long ulCountTime;//计时时间
extern unsigned long ulFirstPointPeroid;//保存初馏点时间
extern unsigned char currentCommand;
extern bit furnanceWorking;
extern bit cryostatWorking;
extern bit fanWorking;
extern unsigned char  tianping_status;
extern unsigned char 	second;
/***********************************************
说明：保存着仪器PID控制变量
***********************************************/
#define PID_KP (0.2)
#define PID_KI (0.01)
#define PID_KD (0.01)

#define PID_MAX (100)
#define PID_MIN (0)
#define SAMPLE_RATE (1)

extern float Sum_G;
extern float Old_error_G;

/***********************************************
说明：保存着计算仪器滴数的缓冲区
***********************************************/
extern unsigned long ulTime[10];
extern unsigned char ucPointer;

extern float fVelocity;//滴速，滴/分钟

extern float fPrePureWeight;
extern float fCurVelocity;
extern float fCurError;
extern float fPreError;
extern float fCurPower;
extern unsigned char ucPowerChangePeriod;
/***********************************************
说明：保存冷浴温度上/下限
***********************************************/
extern int 	iCryostatHigh;
extern int 	iCryostatLow;
extern int  iCryostatMid;
extern int 	iCryostat25per;
extern int 	iCryostat75per;
extern unsigned char		ucCryostatStatus;

  // 为了控制开关压缩机、加热棒的时机 使用如下变量
extern float fFirstDerivative1,fFirstDerivative2; //冷浴温度一阶导数
extern float fSecondDerivative;//冷浴温度二阶导数
extern unsigned long ulTimes[3];
extern float	fTemperatures[3];

/***********************************************
说明：保存保存了上位机传递的参数
***********************************************/
extern unsigned char ucMethod;
extern float 	fStreamCorrect;
extern float 	fCondensorCorrect;
extern float 	fAtmCorrect;
extern float 	fJingbuTemp;
/***********************************************
说明：初馏点，终馏点是否出现的状态
***********************************************/
extern bit SHAOPING_OPEN;

extern bit bFirstPoint;
extern bit bLastPoint;
extern bit allowFirstPoint;
extern bit bSendFirstPoint;
//extern bit bRecvFirstPointACK;

extern float 	fFirstPontTemp;
extern int 		iFirstPointDelay;
extern int 		iFirst300Delay;


extern int 		iVolumnThresholdForLastpoint;
extern int 		iLastPointFindThreshold;
extern int 		iLastPointDelayThreashold;
extern int 		iMaxStreamTemperature;
extern int 		iDryPointDelay;
extern int 		iVolumnDelay;

extern float 	fRetrieveVolecity;

extern float 					fMaxTemp;
extern unsigned int 	uiTempPower;//临时功率变量，调整功率用
extern bit						bSendLastPoint;
//extern bit						bRecvLastPointACK;
extern float 					fLastPointTemp;//干点（终馏点温度值）
extern unsigned int 	uiLastPointTime;//在三分钟内，没有新的回收滴，则最后一滴为干点
//extern unsigned int 	uiSendTimeOut;

extern int 						iCoolingDelay;
#endif 