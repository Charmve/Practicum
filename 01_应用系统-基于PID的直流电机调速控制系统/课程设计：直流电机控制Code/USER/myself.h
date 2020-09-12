#ifndef __MYSELF_H__
#define __MYSELF_H__

#include <STC15F2K60S2.h>
sbit TestKey = P5^4;//���԰���
sbit TestLed = P5^5;//����LED
sbit Beep = P2^7;
sbit GDD = P2^6;

void DelayNms(int n);//��ʱNms
void Delay1ms()	;	//@11.0592MHz

void Timer0Init(void);		//1����@11.0592MHz//��ʱ��0��ʼ��
void Int0Init(void);		//�ⲿ�ж�0���½��أ�
bit KeyPress(bit KeyIO);//	����������������������ɺ���
unsigned char Judge5DKeyNum(unsigned char ADCValue);//�жϼ�ֵ
unsigned char * Hex2ASCII(long int long28Value);//28λֵת��Ϊ9λʮ����ASCII
void MyGPIO_Inilize(void);
void MENU_oLED(void);

void Motor_Init( void );
void Motor_Start(void);
void Motor_Stop(void);
void Motor_Turn(bit n);
void Motor_add(); 
void Motor_dec();

//void  ye(int senddate,int m,int n);
#endif