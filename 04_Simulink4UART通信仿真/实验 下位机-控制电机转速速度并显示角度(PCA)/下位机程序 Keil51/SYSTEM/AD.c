#include "AD.h"
#include "intrins.h"

unsigned char high;
unsigned char low;
u16 date,angle;
//unsigned char h8,l8;
unsigned char ang[3]={0};

void ADC_init(void)
{
		P1M0 = 0x00;                               //����P1.0ΪADC��
    P1M1 = 0x01;
    ADCCFG = 0x0f;                           //����ADCʱ��Ϊϵͳʱ��/2/16/16
}

 
void ADC_contrl(void)
{
	  ADC_CONTR = 0x80;                           //ʹ��ADCģ��
    ADC_CONTR |= 0x40;                          //����ADת��
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));                //��ѯADC��ɱ�־
    ADC_CONTR &= ~0x20;                         //����ɱ�־
}

void ADC_data(void)
{
//    ADCCFG = 0x00;                              //���ý�������
//    high = ADC_RES;                              //A�洢ADC��12λ����ĸ�8λ
//    low = ADC_RESL;                               //B[7:4]�洢ADC��12λ����ĵ�4λ,B[3:0]Ϊ0
	
  ADCCFG = 0x20;                              //���ý���Ҷ���
  high = ADC_RES;                              //A[3:0]�洢ADC��12λ����ĸ�4λ,A[7:4]Ϊ0
  low = ADC_RESL;                               //B�洢ADC��12λ����ĵ�8λ
	
}

void Converse(void)   //ת���ɽǶ�
{
	date=high*16*16+low; 
	angle=(360.0/3510.0)*date; //�Ƕȼ��㹫ʽ
	
	ang[0]=(angle>>8)&0xff;  //�Ƕ�ֵ�߰�λ
	ang[1]=angle&0xff;        //�Ƕ�ֵ�ڰ�λ
	
}