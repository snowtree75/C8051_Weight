#ifndef __DEVICESTATUS_H__
#define __DEVICESTATUS_H__
/***********************************************
˵����Ϊ����ʾ��Ƭ������λ����ͨ��״̬��ʹ��һ��
			���������ı���
***********************************************/
extern unsigned long count;
/***********************************************
˵���������˰���CPS120�����������Ĵ���ѹǿ���¶�ֵ
			�ֱ����޷��ų�������double���ͱ���
***********************************************/
extern unsigned long 		ulDeviceAtm;
extern unsigned long  	ulDeviceTempture;

extern float 						fDeviceAtm;
extern float  					fDeviceTempture;

/***********************************************
˵����������5·7714���ӵ��¶�ֵ
***********************************************/
extern unsigned long temperature_value[5];
extern unsigned char temperature_flag[5];

extern float 				fDeviceTemperature[5];

/***********************************************
˵��������ADC0ת���ĵ�λֵ
***********************************************/
extern unsigned int uiDeviceP1FirstPointDianWei;
extern unsigned int uiDeviceP1FirstVolumnDianWei;

extern unsigned int uiDeviceP2FirstPointDianWei;
extern unsigned int uiDeviceP2FirstVolumnDianWei;

/***********************************************
˵�������洮�ڴ������豸����������
***********************************************/
extern char   					ucCom0ReceiveByte[32]; // ��ƽ
extern unsigned char   	ucCom1ReceiveByte[256]; // ��λ��

extern unsigned char		ucCom0ReceivePointer;
extern unsigned char		ucCom1ReceivePointer;
/***********************************************
˵����������ͨ������0��������������
***********************************************/
extern float		fTotalWeight;//��Ʒ��������
extern float 		fFlask1Weight;
extern float 		fFlask2Weight;
extern float		fCurPurePervious100Weight;
extern float 		fCurPureNext100Weight;
extern float		fCurWeight; 	//����Һ�������
extern float 		fCurPureWeight;
extern float 		fCurWeightPer;
extern float 		fCurPurePervious100Weight;
/***********************************************
˵����������ͨ������1�����ļ���¯�ӹ���
***********************************************/
extern unsigned int uiPowerPeriod; // ����Ϊ100%ʱ�ļ���
extern unsigned int uiCurPower; 		// ����ʹ��ռ�ձȱ�ʾ
extern unsigned int uiPowerCnt;		// ���ʼ���

//////
extern unsigned int uiPower1;
extern unsigned int uiPower2;
extern unsigned int uiPower3;
extern unsigned int uiPower4;
/***********************************************
˵������������������״̬�ͼ�ʱʱ��
***********************************************/
extern unsigned char ucCurDeviceStatus;//��������״̬
extern unsigned long ulCountTime;//��ʱʱ��
extern unsigned long ulFirstPointPeroid;//��������ʱ��
extern unsigned char currentCommand;
extern bit furnanceWorking;
extern bit cryostatWorking;
extern bit fanWorking;
extern unsigned char  tianping_status;
extern unsigned char 	second;
/***********************************************
˵��������������PID���Ʊ���
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
˵���������ż������������Ļ�����
***********************************************/
extern unsigned long ulTime[10];
extern unsigned char ucPointer;

extern float fVelocity;//���٣���/����

extern float fPrePureWeight;
extern float fCurVelocity;
extern float fCurError;
extern float fPreError;
extern float fCurPower;
extern unsigned char ucPowerChangePeriod;
/***********************************************
˵����������ԡ�¶���/����
***********************************************/
extern int 	iCryostatHigh;
extern int 	iCryostatLow;
extern int  iCryostatMid;
extern int 	iCryostat25per;
extern int 	iCryostat75per;
extern unsigned char		ucCryostatStatus;

  // Ϊ�˿��ƿ���ѹ���������Ȱ���ʱ�� ʹ�����±���
extern float fFirstDerivative1,fFirstDerivative2; //��ԡ�¶�һ�׵���
extern float fSecondDerivative;//��ԡ�¶ȶ��׵���
extern unsigned long ulTimes[3];
extern float	fTemperatures[3];

/***********************************************
˵�������汣������λ�����ݵĲ���
***********************************************/
extern unsigned char ucMethod;
extern float 	fStreamCorrect;
extern float 	fCondensorCorrect;
extern float 	fAtmCorrect;
extern float 	fJingbuTemp;
/***********************************************
˵��������㣬������Ƿ���ֵ�״̬
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
extern unsigned int 	uiTempPower;//��ʱ���ʱ���������������
extern bit						bSendLastPoint;
//extern bit						bRecvLastPointACK;
extern float 					fLastPointTemp;//�ɵ㣨������¶�ֵ��
extern unsigned int 	uiLastPointTime;//���������ڣ�û���µĻ��յΣ������һ��Ϊ�ɵ�
//extern unsigned int 	uiSendTimeOut;

extern int 						iCoolingDelay;
#endif 