#ifndef __types_h__
#define __types_h__
		
typedef unsigned char  BOOLEAN;       //ע�⣺��Ҫʹ��bit���壬��Ϊ�ڽṹ�����޷�ʹ��
typedef unsigned char  INT8U;         //�޷���8λ��
typedef signed   char  INT8S;         //�з���8λ��
typedef unsigned int   INT16U;        //�޷���16λ��
typedef signed   int   INT16S;        //�з���16λ��
typedef unsigned long  INT32U;        //�޷���32λ��
typedef signed   long  INT32S;        //�з���32λ��
typedef float          FP32;          //�����ȸ�����
typedef double         FP64;          //˫���ȸ�����

#define BYTE           INT8S          //������ǰ�汾����������
#define UBYTE          INT8U          //uC/OS-II���Բ�����Щ��������
#define WORD           INT16S
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U


#endif