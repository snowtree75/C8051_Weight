#include "includes.h"

/**********************************************************
  EMI配置程序  数据地址总线非复用 用于FM20L08   
**********************************************************/
void EMI_Init()
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = EMI0_PAGE;
	EMI0CF    = 0x33;
	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}

void Init_Device(void)
{
	OscillatorInit();
	
	PortIOInit();
	
	VoltageReferenceInit();
	ADC0Init();
	Comparator01Init();
	
	SPIInit();
	AD7714Init();
		
	Timer01Init();
	//InitTimer0();
	//  timer2_Init();
	Timer3Init();
	Timer4Init();
	//	 EMI_Init();
	UART0Init();
	UART1Init();
	SMBusInit();	 	 
}

int Delta;
void InitAllDatas(void){
	// COM0，COM1接受数据缓冲区指针
	ucCom0ReceivePointer = 0;
	ucCom1ReceivePointer = 0;
	// 控制加热炉功率变量赋初值
	uiPowerPeriod = 100; // 功率为100%时的计数
	uiPowerCnt = 0;		// 功率计数
	uiCurPower = 0;
	// 初始化仪器设备
	ucCurDeviceStatus = READY;
	ulCountTime = 0;
	ulFirstPointPeroid = 0;
	
	///////////////////////////////////
	// 为了防止在计算时被零除
	fTotalWeight = 100;
	/////////////////////////////////
	// 以下的赋初值为测使用，实际情况应该从数据库中读取或从上位机传来
	uiPower1 = 45;
	uiPower2 = 50;
	uiPower3 = 50;
	uiPower4 = 50;
	
	/////////////////////////////////////////////////
	// 指向滴数时间的缓冲区元素的指针
	ucPointer = 0;
	
	/////////////////////////////////////////////////
	// 以下为测试使用
	iCryostatHigh = 70;
	iCryostatLow  = 60;
	
	////////////////////////////////////////////////////
	// 这部分的考虑是为了防止上下限太小了，避免频繁启动压缩机
	Delta = iCryostatHigh - iCryostatLow;
	if(abs(Delta)<4){
		iCryostatMid = (iCryostatHigh + iCryostatLow) / 2;
		
		iCryostatLow = iCryostatMid - Delta / 2;
		iCryostatHigh = iCryostatMid + Delta / 2;
	}else{
		iCryostatMid = (iCryostatHigh + iCryostatLow) / 2;
	}
	////////////////////////////////////////////////////	
	iCryostat25per = iCryostatLow + Delta/4;
	iCryostat75per = iCryostatHigh - Delta/4;
	
	ucCryostatStatus = 0;//工作状态（0:停止，1：降温，2：升温）
	
	fFirstDerivative1 = 0;
	fFirstDerivative2 = 0; //冷浴温度一阶导数
	fSecondDerivative = 0;//冷浴温度二阶导数
	
	memset(ulTimes,0,3*sizeof(long));
	memset(fTemperatures,0,3*sizeof(float));	
	/**********************************************/
	fFlask1Weight = 0;
	fFlask2Weight = 0;
	fTotalWeight = 0;
	fCurPurePervious100Weight = 0;
	fCurPureNext100Weight = 0;
	fCurWeight = 0;
	fCurWeightPer = 0;
	fCurPureWeight = 0;
	fCurPurePervious100Weight = 0;
	
	/**********************************************/
	
	/***********************************************
	说明：初馏点，终馏点是否出现的状态
	***********************************************/
	bFirstPoint = 0;
	allowFirstPoint = 0;
	bLastPoint = 0;
	iFirst300Delay = 300;
	fFirstPontTemp = 0;
	bSendFirstPoint = 0;
	
	volumnThresholdForLastpoint = 94;
	fMaxTemp = -100.0;	
	iLastPointDelayThreashold = 180;
	bSendLastPoint = 0;
	
	iCoolingDelay = 180;
}