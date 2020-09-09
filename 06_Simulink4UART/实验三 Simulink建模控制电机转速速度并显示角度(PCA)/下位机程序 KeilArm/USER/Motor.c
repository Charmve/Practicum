#include "Motor.h"
#include "STC15F2K60S2.h"
#include "config.h"

unsigned int us = 0;
unsigned int ms = 0;
unsigned int Pwm_Flag = 10;

void Motor_Front()
{
	P34 = 0;
	P35 = 1;
}

void Motor_Re()
{
	P34 = 1;
	P35 = 0;
}

void Motor_Stop()
{
	P34 = 1;
	P35 = 1;
}

void Pwm_Timer1()   //100us
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xA4;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}

//void Motor_Pwm()   //T = 20ms %50
//{
//	
//	
//	
//}

void Interrupt1()   interrupt 3
{
	us += 1;
	if(us == 200)
	{
		ms += 1;
		us = 0;
	}
	if(ms == 20)
	{
		ms = 0;
	}
	if(ms <= Pwm_Flag)
	{
		Motor_Front();
	}
	else
	{
		Motor_Stop();
	}
	
	
	
}


