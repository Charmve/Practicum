#ifndef __MYSELF_H__
#define __MYSELF_H__

#include "config.h"

extern u16 R;
extern u16 G;
extern u16 Bl;


void DelayNms(int n);//��ʱNms
void Delay1ms();	//@11.0592MHz

void Timer0Init(void);		//1����@11.0592MHz //��ʱ��0��ʼ��

void conversion(uint temp_data);   //������Ϊ��h�ļ���û��uchar��uint֮��Ķ���  ���Ի�����﷨����!!!

extern uchar disp_R[3];  //��
extern uchar disp_G[3];  //��
extern uchar disp_B[3];  //��


#endif