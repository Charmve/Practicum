#ifndef		__AD_H
#define		__AD_H

#include "config.h"

void ADC_init(void);//��ʼ��
void ADC_contrl(void);//AD��ؿ���
void ADC_data(void);//���ݸ�ʽ
void Converse(void);//ת���Ƕ�

extern unsigned char high;  //�߰�λ
extern unsigned char low;  //�Ͱ�λ
extern u16 date,angle;
//extern unsigned char h8,l8;
extern unsigned char ang[];
#endif
