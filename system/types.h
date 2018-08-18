#ifndef __types_h__
#define __types_h__
		
typedef unsigned char  BOOLEAN;       //注意：不要使用bit定义，因为在结构体里无法使用
typedef unsigned char  INT8U;         //无符号8位数
typedef signed   char  INT8S;         //有符号8位数
typedef unsigned int   INT16U;        //无符号16位数
typedef signed   int   INT16S;        //有符号16位数
typedef unsigned long  INT32U;        //无符号32位数
typedef signed   long  INT32S;        //有符号32位数
typedef float          FP32;          //单精度浮点数
typedef double         FP64;          //双精度浮点数

#define BYTE           INT8S          //兼容以前版本的数据类型
#define UBYTE          INT8U          //uC/OS-II可以不用这些数据类型
#define WORD           INT16S
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U


#endif