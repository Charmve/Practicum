#include "Motor.h"
#include "AD.h"

sbit enable=P2^2;  //ʹ��
sbit A1=P2^0;   //�������1
sbit A2=P2^1;  //�������2

unsigned int countfall;
//unsigned char speed;
//unsigned int time1;
void Motor_Init()
{

	A1=0;
	A2=0;

}

void Motor_TN()
{
	enable=1;
	A1=1;
	A2=0;
}

void Motor_TP()
{
	enable=1;
	A1=0;
	A2=1;
}

void Exinit()
{
    IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
    EX0 = 1;                        //enable INT0 interrupt
   // EA = 1;                         //open global interrupt switch

}

void Exint0() interrupt 0           //(location at 0003H)
{
	countfall++;	  //���½��ظ���
}

//void panduany(void)
//{
//		if(countfall==390)       //�Ƶ�һȦ�����жϣ���ʾ�ٶ�
//		{
//			EX0=0;
//			countfall=0;
//			TR0=0;
//			speed=1000/time1;
//			ang[2]=speed;
//		//	LCD_Write_String(0,1,tempy);//��ʾ��Һ���ڶ���		
//			time1=0;
//			TR0=1;
//			EX0=1;
//		}	
//}



