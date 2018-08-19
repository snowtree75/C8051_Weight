#include "includes.h"

/**********************************************************
  EMI���ó���  ���ݵ�ַ���߷Ǹ��� ����FM20L08   
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
	// COM0��COM1�������ݻ�����ָ��
	ucCom0ReceivePointer = 0;
	ucCom1ReceivePointer = 0;
	// ���Ƽ���¯���ʱ�������ֵ
	uiPowerPeriod = 100; // ����Ϊ100%ʱ�ļ���
	uiPowerCnt = 0;		// ���ʼ���
	uiCurPower = 0;
	// ��ʼ�������豸
	ucCurDeviceStatus = READY;
	ulCountTime = 0;
	ulFirstPointPeroid = 0;
	
	///////////////////////////////////
	// Ϊ�˷�ֹ�ڼ���ʱ�����
	fTotalWeight = 100;
	/////////////////////////////////
	// ���µĸ���ֵΪ��ʹ�ã�ʵ�����Ӧ�ô����ݿ��ж�ȡ�����λ������
	uiPower1 = 45;
	uiPower2 = 50;
	uiPower3 = 50;
	uiPower4 = 50;
	
	/////////////////////////////////////////////////
	// ָ�����ʱ��Ļ�����Ԫ�ص�ָ��
	ucPointer = 0;
	
	/////////////////////////////////////////////////
	// ����Ϊ����ʹ��
	iCryostatHigh = 70;
	iCryostatLow  = 60;
	
	////////////////////////////////////////////////////
	// �ⲿ�ֵĿ�����Ϊ�˷�ֹ������̫С�ˣ�����Ƶ������ѹ����
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
	
	ucCryostatStatus = 0;//����״̬��0:ֹͣ��1�����£�2�����£�
	
	fFirstDerivative1 = 0;
	fFirstDerivative2 = 0; //��ԡ�¶�һ�׵���
	fSecondDerivative = 0;//��ԡ�¶ȶ��׵���
	
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
	˵��������㣬������Ƿ���ֵ�״̬
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