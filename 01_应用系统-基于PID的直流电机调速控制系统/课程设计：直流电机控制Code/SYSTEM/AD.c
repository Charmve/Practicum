#include	<STC15F2K60S2.h>
#include	"AD.h"
#include "myself.h"
#include	"Serial.h"
bit bFlagAD=0;
bit aFlagAD=0;
unsigned int cADCResult=0;
unsigned int AngADResult = 0;

unsigned char ch = 0;      //ADCͨ����

void ADCInit()
{
		P1ASF = 0x03;                   //����P11��P10��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
	  PADC = 1;
    ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START | ch;//��Ϊ����ٶ�
		IE = IE | 0xa0 ;                //ʹ��ADC�ж�
		DelayNms(2);//ADC�ϵ粢��ʱ
}

void adc_isr() interrupt 5 using 3//AD �жϴ�����
{
    ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
	
	  if(!ch)  {cADCResult = ADC_RES; bFlagAD = 1;}
		else {AngADResult = ADC_RES; aFlagAD = 1;}
}