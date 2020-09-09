#include	"config.h"
#include "intrins.h"
#include <Motor.h>
#include	"Serial.h"
#include "AD.h"

#define TurnOn 0;  //����
#define TurnOff 1; //����
 
unsigned char speed;
unsigned int time;
unsigned char receive,i;

sbit LED=P5^5;
	
void Timer0Init(void);		//1ms@11.0592MHz
void PWM_init();
void PWM_Contrl(unsigned char count);

void main()
{
	extern bit uart_flag;//���ڽ��յ����ݱ�־
	
	Motor_Init();  //ֱ�������ʼ��
	Exinit();      //�ⲿ�жϳ�ʼ��
	
	ADC_init();    //AD��ʼ��
	
	PS=1;          //�������ȼ�Ϊ��
	PT0=0;
	Uart1Init();  //���ڳ�ʼ��
	Timer0Init();  //��ʱ��0��ʼ��
	PWM_init();     //PCA��ʼ��
	EA = 1; 

  while(1)
	{	
		ADC_contrl(); //AD����
		ADC_data();   //��������
		Converse();   //�Ƕ�ת��
		Motor_TN();

		if(uart_flag==1)   //����ȷ���յ�һ֡����
		{
			LED=TurnOn;
			receive=cS1Rec[6];
			
			
			uart_flag=0;     //��־λ����
			
		}
		PWM_Contrl(receive);
	}
}


void Timer0Init(void)		//1ms@24MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	ET0=1;
}

void Timer0(void) interrupt 1 using 1
{
	time++;
	if(time==1000)  //ʱ�䵽��һ��
	{
		time=0;
		EX0=0;
		speed=countfall/390;	//�����ٶ�
		ang[2]=speed;
		countfall=0;
		for(i=0;i<3;i++)
		{
			S1SendData(ang[i]);
		}
		if(i==3)
		{
			i=0;
		}
		EX0=1;
	}
	
}

void PWM_init()
{
    CCON = 0x00;
    CMOD = 0x00;                                //PCAʱ��Ϊϵͳʱ��/12 3K
    CL = 0x00;
    CH = 0x00;
    CCAPM0 = 0x42;                              //PCAģ��0ΪPWM����ģʽ
    PCA_PWM0 = 0x00;                            //PCAģ��0���8λPWM
}

void PWM_contrl(unsigned char count)
{
    CCAP0L = count;                              //PWMռ�ձ�Ϊ[(100H-count)/100H]
    CCAP0H = count;

    CR = 1;                                     //����PCA��ʱ��
}
