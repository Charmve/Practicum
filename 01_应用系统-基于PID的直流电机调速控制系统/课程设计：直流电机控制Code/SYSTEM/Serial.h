#ifndef		__SERIAL_H
#define		__SERIAL_H

#include	<config.h>


void Uart1Init(void);	//��ʱ2��Ϊ�����ʷ�������Ĭ��8λ���ݣ�
//����1(P3.0/RxD, P3.1/TxD)


void S1SendData(unsigned char dat);//��ѯ��ʽ����
void S1SendString(unsigned char *s);
unsigned char S1ReceiveData(void);//��ѯ��ʽ����
#endif