#ifndef __SMBUS_H__
#define __SMBUS_H__

 /*               ---SMBUS-----I2C----
************************************************/
#define  SMB_DATA_LEN   4              //�����ȡ���ݳ���

#define  WRITE          0x00           // WRITE direction bit
#define  READ           0x01           // READ direction bit
#define  SLAVE_ADDR     0x50           // Device address for slave target  (0x28>>1)

#define  SMB_BUS_ERROR  0x00           // (all modes) BUS ERROR
#define  SMB_START      0x08           // (MT & MR)��ʼ�����ѷ��� ���ӵ�ַ//+R/W װ�뵽SMB0DAT ��0 STA
#define  SMB_RP_START   0x10           // (MT & MR)�ظ���ʼ�����ѷ��� ���ӵ�
//ַ+R/W װ�뵽SMB0DAT ��0 STA
#define  SMB_MTADDACK   0x18           // (MT)�ӵ�ַ+W �ѷ��� �յ�ACK ��Ҫ//���͵�����װ�뵽SMB0DAT
#define  SMB_MTADDNACK  0x20           // (MT)�ӵ�ַ+W �ѷ��� �յ�NACK ȷ//�ϲ�ѯ���� ��λSTO+STA
#define  SMB_MTDBACK    0x28           // (MT)�����ֽ��ѷ��� �յ�ACK
#define  SMB_MTDBNACK   0x30           // (MT)�����ֽ��ѷ��� �յ�NACK
#define  SMB_MTARBLOST  0x38           // (MT)����ʧ��
#define  SMB_MRADDACK   0x40           // (MR)�ӵ�ַ+R �ѷ��� �յ�ACK
#define  SMB_MRADDNACK  0x48           // (MR)�ӵ�ַ+R �ѷ��� �յ�NACK
#define  SMB_MRDBACK    0x50           // (MR)�����ֽ��յ� ACK�ѷ���
#define  SMB_MRDBNACK   0x58           // (MR)�����ֽ��յ� NACK�ѷ���

extern bit CPS_Wake;              
extern unsigned char  	TARGET;
extern unsigned char 	SMB_DATA_IN[4]; 

void SMBusInit(void);
void SMBusRead(void);
void SMBusWrite(void);

#endif