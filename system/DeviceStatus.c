/***********************************************
˵����Ϊ����ʾ��Ƭ������λ����ͨ��״̬��ʹ��һ��
			���������ı���
***********************************************/
unsigned long ulDeviceCount = 0;// one second

/***********************************************
˵����for Timer 
***********************************************/
unsigned long count = 0;


/***********************************************
˵������֤�İ���CPS120�����������Ĵ���ѹǿ���¶�ֵ

***********************************************/
unsigned long 	ulDeviceAtm = 0;
unsigned long  	ulDeviceTempture = 0;

float 					fDeviceAtm = 0.0f;
float  					fDeviceTempture = 0.0f;

/***********************************************
˵����������5·7714���ӵ��¶�ֵ
***********************************************/
unsigned long 	temperature_value[5];
unsigned char 	temperature_flag[5];

float 					fDeviceTemperature[5];

/***********************************************
˵��������ADC0ת���ĵ�λֵ
***********************************************/
unsigned int dianwei0,dianwei1,dianwei2,dianwei3;

unsigned int uiDeviceP1FirstPointDianWei;
unsigned int uiDeviceP1FirstVolumnDianWei;

unsigned int uiDeviceP2FirstPointDianWei;
unsigned int uiDeviceP2FirstVolumnDianWei;
/***********************************************
˵�������洮�ڴ������豸�������ֽ�����
			�Ӹ��ֽ����ݿ��Գ�ȡȡ����
***********************************************/
char   					ucCom0ReceiveByte[32]; 
unsigned char   ucCom1ReceiveByte[256]; 

unsigned char		ucCom0ReceivePointer;
unsigned char		ucCom1ReceivePointer;

/***********************************************
˵����������ͨ������0�����ĳ�����������
***********************************************/
float		fTotalWeight;//��Ʒ��������
float		fCurWeight; 	//����Һ�������
float   fCurWeightPer; // ����Һ�������ռ�������ٷֱ�

/***********************************************
˵����������ͨ������1�����ļ���¯�ӹ���
***********************************************/
unsigned int uiPowerPeriod; // ����Ϊ100%ʱ�ļ���
unsigned int uiCurPower; 		// ����ʹ��ռ�ձȱ�ʾ
unsigned int uiPowerCnt;		// ���ʼ���

///// ����Ϊ�ֶι���
unsigned int uiPower1;
unsigned int uiPower2;
unsigned int uiPower3;
unsigned int uiPower4;

/***********************************************
˵���������������豸�ĵ�ǰ״̬
***********************************************/
unsigned char ucCurDeviceStatus;
unsigned long	ulCountTime;
unsigned long ulFirstPointPeroid;

/***********************************************
˵��������������PID���Ʊ���
***********************************************/
float Sum_G;
float Old_error_G;
/***********************************************
˵���������ż������������Ļ�����
***********************************************/
unsigned long ulTime[10];
unsigned char ucPointer;

float fVelocity;//���٣���/����

/***********************************************
˵����������ԡ�¶���/����
***********************************************/
int 	iCryostatHigh;
int 	iCryostatLow;
int 	iCryostatMid;
int 	iCryostat25per;
int 	iCryostat75per;
unsigned char		ucCryostatStatus;

  // Ϊ�˿��ƿ���ѹ���������Ȱ���ʱ�� ʹ�����±���
float fFirstDerivative1,fFirstDerivative2; //��ԡ�¶�һ�׵���
float fSecondDerivative;//��ԡ�¶ȶ��׵���
unsigned long ulTimes[3];
float	fTemperatures[3];


/***********************************************
˵�������汣������λ�����ݵĲ���
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
˵��������㣬������Ƿ���ֵ�״̬
***********************************************/
/*
unsigned char ucFirstPoint,ucLastPoint;
float fTempTemp;//��ʱ�¶ȣ���Ѱ�����
float fFirstPontTemp;
unsigned char ucisSendFirstPoint;
float fLastPointTemp;//�ɵ㣨������¶�ֵ��
unsigned int uiLastPointTime;//���������ڣ�û���µĻ��յΣ������һ��Ϊ�ɵ�
*/
