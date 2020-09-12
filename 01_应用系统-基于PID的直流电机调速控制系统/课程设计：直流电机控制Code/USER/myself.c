#include "myself.h"
#include "AD.h"
#include "Serial.h"//����1(P3.0/RxD, P3.1/TxD)
#include "intrins.h"
#include "oled.h"

#define A1 P16 
#define A2 P15 
#define PWM P14 
#define En P17 

bit bFlagTimer0=0,bFlag2ms=0,bFlag20ms=0;
bit TestKeyLastTime=1;//���ڱ�����԰���ǰ��״̬
unsigned char Motor_count = 100;//���ڵ��ռ�ձȿ���
unsigned char time_count = 0;
unsigned char xdata cJudgeKey=0;//�����жϰ����ı���
unsigned int xdata T0Counter2ms=0,T0Counter20ms=0,T0Counter1s = 0;
unsigned char code KeyThreshold[7]={0x40,0x95,0xB6,0xC7,0xD1,0xEA,0xFF};
extern bit aFlagAD;//ADת����־
extern unsigned char ch;      //ADCͨ����
//static unsigned int xdata nroll;  //���nת
unsigned int xdata APhra;  //A���nת
unsigned int xdata BPhra;  //B���nת
unsigned int xdata CSpeed;  //�����ǰ���ٶ� ÿ0.5���һ��

//	����				��ѹֵ		����ADֵ			��ֵ����
//0	KeyStartStop  0     		0x00		  0x00-0x40
//1	KeyDown  		1/2Vcc		0x81			  0x41-0x95	
//2	KeyMiddle  	2/3Vcc		0xAB-0xAC		0x96-0xB6
//3	KeyRight  	3/4Vcc		0xC0-0xC1		0xB7-0xC7
//4	KeyLeft  		4/5Vcc		0xCE-0xCD		0xC8-0xD1
//5	KeyUp    		5/6Vcc		0xD6			  0xD2-0xEA
//6	NoneKey  			Vcc 		0xFF		  	0xEB-0xFF

//Motor_count = 100; //��ʼ��Ϊ100

void DelayNms(int n)//��ʱNms//@11.0592MHz
{
	int i=0;
	for(i=0;i<n;i++) 
		Delay1ms();
}

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void MENU_oLED(void)
{
	OLED_Clear();
	OLED_ShowCHinese(12,0,2);//ֱ  //��������� Hzk[][32]��
	OLED_ShowCHinese(30,0,3);//��
	OLED_ShowCHinese(48,0,4);//��
	OLED_ShowCHinese(66,0,5);//��
	OLED_ShowCHinese(84,0,6);//��
	OLED_ShowCHinese(102,0,7);//��

	OLED_ShowString(0,3,"ADVal:",48);
	
	OLED_ShowString(1,5,"Speed:     r/s",48);

	OLED_ShowString(0,7,"     12/29 10:42",12);
}

void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;	//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;	//���ö�ʱ��ģʽ
	TL0 = 0xCD;	//���ö�ʱ��ֵ
	TH0 = 0xD4;	//���ö�ʱ��ֵ
	PT0 = 1;
	TF0 = 0;  //���TF0��־
	TR0 = 1;  //��ʱ��0��ʼ��ʱ
  ET0 = 1;  //ʹ�ܶ�ʱ��0�ж�
}

void Timer0Interrupt() interrupt 1 using 2 //��ʱ��0���ж�
{
	T0Counter2ms++;
	if(T0Counter2ms>=2)
	{
		bFlag2ms=1;
		T0Counter2ms=0;
		T0Counter20ms++;
		ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | 0x00;//2ms����һ��ADC		
		if(T0Counter20ms >= 100)
		{
			aFlagAD = 0;
			ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | 0x01;//2ms����һ��ADC
		}
		else{}
		
		if(T0Counter20ms >= 250){
				bFlag20ms=!bFlag20ms;
				T0Counter20ms=0;
			  T0Counter1s++;
			  //TestLed = !TestLed;			
			if(T0Counter1s >= 2)
			{
				CSpeed = (APhra+BPhra) / 20;  //25ms����һ��
		  	APhra = 0;BPhra = 0;
			}
		}	
		else{}		
	}
	else{}
}

void Int0Init(void)		//�ⲿ�ж�0\1���½��أ�
{
	  IT0 = 0;    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
	  EX0 = 1;    //ʹ��INT0�ж�
    IT1 = 0;    //����INT1���ж����� (1:���½��� 0:�����غ��½���)
    EX1 = 1;    //ʹ��INT1�ж�
	  PX0 = 0;
	  PX1 = 0;
    EA = 1;
}

//�жϷ������
void exint0() interrupt 0   //INT0�ж����
{
  APhra++; 
	//TestLed = !TestLed;     //�����Կ�ȡ��
}

void exint1() interrupt 2 //INT1�ж����
{
  BPhra++; 
	//TestLed = !TestLed;     //�����Կ�ȡ��
}


bit KeyPress(bit KeyIO)
{
	cJudgeKey=(cJudgeKey<<1) | KeyIO; // �ж�ֵ����һλ ��������ǰɨ��ֵ�����
	if(cJudgeKey==0x00)//�������£�״̬�ȶ���KeyIO����Ϊ0��̧��Ϊ1
	{
		TestKeyLastTime=KeyIO;//��¼״̬
	}
	else if((cJudgeKey==0xFF) && (TestKeyLastTime!=KeyIO))//�����Ӱ��µ�̧���ȶ�
	{
		TestKeyLastTime=KeyIO;
		return 1;
	}			
	else 
	{

	}
	return 0;
}

unsigned char Judge5DKeyNum(unsigned char ADCValue)
{
	static unsigned char xdata cFlag5D=0;//�������Ӷ���ļ�ֵ
	static unsigned char xdata c5DkeyLastTime=0;//�����ϴΰ���ֵ
//	static unsigned char xdata cNumLastTime=6;//��¼�ϴ�ȷ�ϵļ�ֵ
	static unsigned char xdata cKeyPressNum;
	unsigned char c5DkeyNow=7;

	
//	unsigned char c5DkeyNow=7;
//	unsigned char i=0;
//	//�жϵ�ǰ����ADC��Ӧ�İ���ֵ

//	˼·�뵥����������
	//����ADֵ�õ���ǰ��ֵ
	if(ADCValue<=KeyThreshold[0]) c5DkeyNow=0;
	else if (ADCValue>KeyThreshold[0] && ADCValue<=KeyThreshold[1]) c5DkeyNow=1;
	else if (ADCValue>KeyThreshold[1] && ADCValue<=KeyThreshold[2]) c5DkeyNow=2;
	else if (ADCValue>KeyThreshold[2] && ADCValue<=KeyThreshold[3]) c5DkeyNow=3;
	else if (ADCValue>KeyThreshold[3] && ADCValue<=KeyThreshold[4]) c5DkeyNow=4;
	else if (ADCValue>KeyThreshold[4] && ADCValue<=KeyThreshold[5]) c5DkeyNow=5;
	else  c5DkeyNow=6;

	//��¼����
	if(c5DkeyNow==6)//̧�𣬼�¼һ��
		cFlag5D=(cFlag5D<<1)&0;//���Ƽ�¼����
	else if(c5DkeyNow==c5DkeyLastTime)//AD�жϵļ�ֵ���ϴ���ͬ��
		cFlag5D=(cFlag5D<<1)|1;//���Ƽ�¼����
	else //������������η�̧��Ҳ���ϴβ�ͬ�����������ܳ���
		cFlag5D=(cFlag5D<<1)& 0;//���Ƽ�¼����
	
	c5DkeyLastTime=c5DkeyNow;//��¼��ǰAD���ļ�ֵ	
	
	//�жϼ�ֵ
	if(cFlag5D==0xFF)//����8�ζ���һ��
		cKeyPressNum=c5DkeyNow;	//��¼��ǰ��ֵ
	
  if(cFlag5D==0x00 && cKeyPressNum !=6 )//������Ч̧����ǰһ��Ϊ��Ч����
	{
		c5DkeyNow=cKeyPressNum;	
		cKeyPressNum=0x06;
		return c5DkeyNow;
	}
	else
		return 0x06;		
}

unsigned char* Hex2ASCII(long int long28Value)//8λֵת��ΪASCII
{
	unsigned char xdata * cindexTempHex=0;
	char xdata i=0;
	
	for(i=7;i>=0;i=i-2)//��λ��ǰ
	{
		cindexTempHex[i]=long28Value;//����Ĭ����������ת����charΪ8λ��ȡ��lont int �ĵ�8λ
		cindexTempHex[i-1]=cindexTempHex[i]>>4;//ȡ��8λ�и�4λ	
		cindexTempHex[i]=cindexTempHex[i]-(cindexTempHex[i-1]<<4);//ȡ��8λ�еĵ�4λ
		long28Value=long28Value>>8;//��8λ������ϣ�����			
	}
//	S1SendData(0xAA);		
	for(i=0;i<=7;i++)
	{

//		S1SendData(cindexTempHex[i]);		
		if(cindexTempHex[i]<=9) cindexTempHex[i]+=0x30;//С��9ת��ASCII
		else cindexTempHex[i]=cindexTempHex[i]+55;//����9����ת��ASCII		
//		S1SendData(cindexTempHex[i]);			
	}
	cindexTempHex[8]=0;//������һ��������
	return cindexTempHex;
}

void MyGPIO_Inilize()
{
	GPIO_Inilize(GPIO_P0,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P1,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P2,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P3,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P4,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P5,GPIO_PullUp);//IO��ʼ��//����׼˫���
	GPIO_Inilize(GPIO_P5.4,GPIO_HighZ);//���԰����ڸ�������	
	DelayNms(1);//�յ��ã�����ռ�õ�128�ֽڵ�Data�ռ�
}

void Motor_Init(void)
{
	A1 = 0;
	A2 = 0;
	En = 0;
}

void Motor_Start(void)
{
	En = 1;
	A1 = 0;
	A2 = 1;
}

void Motor_Stop(void)
{
	A1 = 0;
	A2 = 0;	
}

void Motor_Turn(bit n)
{
	En = 1;
	if(n){ 	A1 = 0;A2 = 0;	DelayNms(200);A1 = 1; A2 = 0;}
	else{ A1 = 0;A2 = 0; DelayNms(200); A1 = 0; A2 = 1;}
}
	
//������ټ���
void Motor_add()
{
	En = 1;
	
}	
void Motor_dec()
{
	En = 1;
	
}

//void  ye(int senddate,unsigned int n,unsigned int m)
//{
//	if(senddate==0x02&&n==0) n++;
//	else if(senddate==0x04&&n==1)n--;
//	
//   if(n==0){
//		OLED_ShowString(0,0,"C00:",16);
//	  OLED_ShowString(0,2,"C01:",16);		
//	  OLED_ShowString(0,4,"C02:",16);
//   }
//		
//	else if(n==1){		
//		if(m==0){
//			OLED_ShowString(0,0,"mm0:",16);
//			OLED_ShowString(0,2,"mm1:",16);		
//			OLED_ShowString(0,4,"mm2:",16);	}
//			if(m==0x02){
//				OLED_ShowString(0,0,"km0:",16);
//				OLED_ShowString(0,2,"km1:",16);		
//				OLED_ShowString(0,4,"km2:",16);	}
//			else if(m==0x04){
//				OLED_ShowString(0,0,"lm0:",16);
//				OLED_ShowString(0,2,"lm1:",16);		
//				OLED_ShowString(0,4,"lm2:",16);	}		
//		}
//}
	
