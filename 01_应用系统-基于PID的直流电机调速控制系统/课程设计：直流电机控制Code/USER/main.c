#include	"config.h"
#include	"Serial.h"//����1(P3.0/RxD, P3.1/TxD)
#include <myself.h>
#include	"AD.h"
#include "stdio.h"
#include "math.h"
#include "oled.h"
#include "AnoTc.h"
#include "PWM.h"

#define A1 P16 
#define A2 P15 
#define PWM P25 
#define En P17 
#define EC1 P31 
#define EC2 P32

/**************************************************************
��һ����Ŀ 1��ֱ���������ϵͳ 

	1�����ӵ�����Ƶ�·�壬���Ժ��·���������� ��30 �֣� 
	2������ͨ�� 5 �򰴼����ҿ��Ƶ������ת�����򰴼����Ƶ������/
	ֹͣ�� ��10 �֣�
	3��ͨ����Ŧ������Ƶ��ת�ٻ�Ƕȣ��ɶ�ѡһ������20 �֣� 
	4����ص��ת�ٻ�Ƕȣ�ת��������Ϣ�� LCD1602 �� OLED ����
	ʾ�� ��20 �֣� 
	5��ͨ�� PID ���ڣ�ͨ����Ŧ����ת�ٻ�ת���Ƕȣ��ɶ�ѡһ������
	��ͨ�����ڣ��ڵ�������ʾ���ߣ��ɽ�����������վ���ߣ�����Ӧ��
	����ʾ������ʾĿ��ת�ٻ�Ƕȡ� ��15 �֣� 
	6�������� ��5 �֣�
**************************************************************/

//unsigned int xdata aimspeed=0, last_speed = 0;
//extern unsigned int xdata CSpeed;  //�����ǰ���ٶ� ÿ0.5���һ��


void IncPIDCalc1(unsigned int AimSpeed,unsigned int Current)
{ 
	static double ek = 0;//��ǰ���
	static double ek1 = 0,ek2 = 0; //
	static double Pwm = 0; 
	float Kp = 1.312, Ki = 0.0001, Kd = 0; //��һ���������Ĳ���  1.323
	//double Kp = 1.312, Ki = 0.0001, Kd = 0;
	ek = AimSpeed - Current; //��������
	Pwm += Kp * (ek - ek1) //E[k]��
					+Ki * ek //E[k-1]��                     
					+Kd * ((ek- ek1) - (ek1-ek2)); //E[k-2]��
	//�洢�������´μ���
	ek2 = ek1;
	ek1 = ek;
	//��������ֵ
	
	if(Pwm > 255) Pwm = 250;
	if(Pwm < 0) Pwm = 5;
	//Test_Send_User(AimSpeed,Current,Pwm);   //AimSpeed -32
  PWM_Set(255-Pwm);
}

void main()
{
	extern bit bS1Rec;//���ڽ��յ����ݱ�־
	extern bit bFlag2ms,bFlag20ms;//2ms��־	
	extern bit bFlagAD;//ADת����־	
	extern bit aFlagAD;//ADת����־
	extern unsigned char cS1Rec;//���ڽ��յ��ַ�
	extern unsigned int cADCResult;//AD��8λ
  extern unsigned int AngADResult;	 //���ڶ�ȡ��ť�ĽǶ�
	extern unsigned char ch;      //ADCͨ����	
	extern bit aFlagAD;//ADת����־
//	extern unsigned int xdata CSpeed;  //�����ǰ���ٶ� ÿ0.5���һ��
	
	unsigned int xdata aimspeed=0, last_speed = 0;
	extern unsigned int xdata CSpeed;  //�����ǰ���ٶ� ÿ0.5���һ��
	
	unsigned char xdata cKeyPressNumber=0x06;
	unsigned char* xdata cindexTemp=0;
	unsigned int xdata current_speed = 0;
	unsigned int ucount = 0,tcount = 0;//���δ�ӡ
	bit motorflag = 1;
	bit flag = 0;
	bFlag2ms=0;bFlag20ms=0;
	GDD = 0;
	
	//Beep = 0;
	MyGPIO_Inilize();//GPIO	
	Uart1Init();//����1��ʼ��
	Timer0Init();//��ʱ��0��ʼ��
	Int0Init(); //�ⲿ�жϳ�ʼ��
	ADCInit(); //ADC��ʼ��
	Motor_Init(); //�����ʼ��
	OLED_Init();	//��ʼ��OLED  	
	MENU_oLED();  //OLED�˵���ʾ
	PWM_Init(0x00);	
	//IncPIDInit();

	while(1)
	{	
		//��ȡ����ֵ
		if(bFlagAD==1) //2m��ȡһ�ΰ���ADֵ�����򰴼��Ͳ��򰴼���
		{
			bFlagAD=0;
			ucount++;
			cKeyPressNumber=Judge5DKeyNum(cADCResult);//��ȡ����ֵ			
		}	
		else{}							
		
		//����������
		if(bFlag20ms == 0){
			tcount++; 
			OLED_ShowString(0,7,"     12/29 10:42",12);
//			OLED_ShowNum(52,4,CSpeed,4,16);
			if(tcount>= 60) {
				tcount = 0;
				OLED_ShowString(0,7,"     12/29 10:43",12);
			}
		}
		else {
			if (tcount == 0) OLED_ShowString(0,7,"     12/29 10 43",12);
			else OLED_ShowString(0,7,"     12/29 10 42",12);
		}
		
		//ʵʱ��ʾ��ťADֵ
		if(aFlagAD == 0) 
		{
			ET0 = 0;ch = 1; //�ض�ʱ��
			ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
			while(aFlagAD == 0) ;
					OLED_ShowNum(52,2,AngADResult,3,16);
			    //PWM_Set(255-aimspeed);
			    //Test_Send_User(aimspeed,CSpeed,00);
			//�޸ģ����ݶ�����AD��Ӧ�ı����Ϊ�ٶȵ�Ŀ��ֵ
			ch = 0;ET0 = 1;
		}
		else{	}
		
		//ÿ50ms����һ��PID
		if(ucount >= 10){  //20ms
			ucount = 0;
			current_speed = CSpeed;
			current_speed = (last_speed + CSpeed)/2;
			aimspeed = 1.3813 * AngADResult + 47.978; //���Ա��Ŀ��ֵ
			OLED_ShowNum(52,4,current_speed*3/2,4,16);
			IncPIDCalc1(aimspeed,current_speed);
			Test_Send_User(AngADResult,current_speed,00);   //AimSpeed -32
			if(!(abs(current_speed-aimspeed) <= 5))
			{
				TestLed = !TestLed; //directing the PID is runnning
			} 
			else{
				Beep = !Beep;
				//DelayNms(1000);
				Beep = !Beep;
				//DelayNms(1000);
			}
			last_speed = CSpeed;
	  }
		else{}
		
		switch(cKeyPressNumber) //���ݰ���ִ�� 
		{ 
			case 0: //KeyStartStop ֱ�����������ֹͣ
			{
				if(!flag)
				{					
					flag = 1;			
					Motor_Start();	
				}
				else{
					flag = 0;
					Motor_Stop();
				}  
				cKeyPressNumber=0x06;break;//���ڷ���00; 
			}
				
			case 1://KeyDown ���¼�ÿ�μ�5%	
			{
				  TestLed = !TestLed;
				  //Motor_dec();
					//S1SendData(0x01);
					cKeyPressNumber=0x06;break;//���ڷ���01; 
			}
				
			case 2:	//KeyMiddle 
			{	
					//S1SendData(0x02);          							
			    cKeyPressNumber=0x06;break;//���ڷ���02; 
			}
				
			case 3: //KeyRight ֱ�������ת
			{
					Motor_Turn(0x01); //1��ת
				  OLED_ShowCHinese(110,2,8);//��		
					//S1SendData(0x03);
					cKeyPressNumber=0x06;break;//���ڷ���03; 
			}
				
			case 4: //KeyLeft  ֱ�������ת
			{				
					Motor_Turn(0x00); //0 ��ת
				OLED_ShowCHinese(110,2,9);//��
					//S1SendData(0x04);
					cKeyPressNumber=0x06;break;//���ڷ���04; 
			}
				
			case 5: //KeyUp	���ϼ�ÿ����5%	
			{
					Motor_add();
					//S1SendData(0x05);
					cKeyPressNumber=0x06;break;//���ڷ���05; 
			}
				
			default:// NoneKey	
				  //S1SendData(0x06);
					break;
			}	
	}
}
