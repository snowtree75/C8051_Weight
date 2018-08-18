/***********************************************
说明：为了显示单片机与上位机的通信状态，使用一个
			连续计数的变量
***********************************************/
unsigned long ulDeviceCount = 0;// one second

/***********************************************
说明：for Timer 
***********************************************/
unsigned long count = 0;


/***********************************************
说明：保证的板载CPS120传感器测量的大气压强与温度值

***********************************************/
unsigned long 	ulDeviceAtm = 0;
unsigned long  	ulDeviceTempture = 0;

float 					fDeviceAtm = 0.0f;
float  					fDeviceTempture = 0.0f;

/***********************************************
说明：保存了5路7714连接的温度值
***********************************************/
unsigned long 	temperature_value[5];
unsigned char 	temperature_flag[5];

float 					fDeviceTemperature[5];

/***********************************************
说明：保存ADC0转换的点位值
***********************************************/
unsigned int dianwei0,dianwei1,dianwei2,dianwei3;

unsigned int uiDeviceP1FirstPointDianWei;
unsigned int uiDeviceP1FirstVolumnDianWei;

unsigned int uiDeviceP2FirstPointDianWei;
unsigned int uiDeviceP2FirstVolumnDianWei;
/***********************************************
说明：保存串口从其他设备发来的字节数据
			从该字节数据可以抽取取参数
***********************************************/
char   					ucCom0ReceiveByte[32]; 
unsigned char   ucCom1ReceiveByte[256]; 

unsigned char		ucCom0ReceivePointer;
unsigned char		ucCom1ReceivePointer;

/***********************************************
说明：保存着通过串口0发来的称重重量数据
***********************************************/
float		fTotalWeight;//样品的总重量
float		fCurWeight; 	//回收液体的重量
float   fCurWeightPer; // 回收液体的重量占总重量百分比

/***********************************************
说明：保存着通过串口1发来的加热炉子功率
***********************************************/
unsigned int uiPowerPeriod; // 功率为100%时的计数
unsigned int uiCurPower; 		// 功率使用占空比表示
unsigned int uiPowerCnt;		// 功率计数

///// 以下为分段功率
unsigned int uiPower1;
unsigned int uiPower2;
unsigned int uiPower3;
unsigned int uiPower4;

/***********************************************
说明：保存着仪器设备的当前状态
***********************************************/
unsigned char ucCurDeviceStatus;
unsigned long	ulCountTime;
unsigned long ulFirstPointPeroid;

/***********************************************
说明：保存着仪器PID控制变量
***********************************************/
float Sum_G;
float Old_error_G;
/***********************************************
说明：保存着计算仪器滴数的缓冲区
***********************************************/
unsigned long ulTime[10];
unsigned char ucPointer;

float fVelocity;//滴速，滴/分钟

/***********************************************
说明：保存冷浴温度上/下限
***********************************************/
int 	iCryostatHigh;
int 	iCryostatLow;
int 	iCryostatMid;
int 	iCryostat25per;
int 	iCryostat75per;
unsigned char		ucCryostatStatus;

  // 为了控制开关压缩机、加热棒的时机 使用如下变量
float fFirstDerivative1,fFirstDerivative2; //冷浴温度一阶导数
float fSecondDerivative;//冷浴温度二阶导数
unsigned long ulTimes[3];
float	fTemperatures[3];


/***********************************************
说明：保存保存了上位机传递的参数
***********************************************/
unsigned char ucMethod;
unsigned char ucP1;
unsigned char ucP2;
unsigned char ucP3;
unsigned char ucP4;
unsigned char ucLow;
unsigned char ucHigh;
unsigned int uiStreamCorrect1;
unsigned int uiStreamCorrect2;
unsigned int uiCondensorCorrect;
unsigned int uiAtmCorrect;
unsigned char ucAuxtemp;
unsigned char ucJingbuTemp;

/***********************************************
说明：初馏点，终馏点是否出现的状态
***********************************************/
/*
unsigned char ucFirstPoint,ucLastPoint;
float fTempTemp;//临时温度，找寻终馏点
float fFirstPontTemp;
unsigned char ucisSendFirstPoint;
float fLastPointTemp;//干点（终馏点温度值）
unsigned int uiLastPointTime;//在三分钟内，没有新的回收滴，则最后一滴为干点
*/
