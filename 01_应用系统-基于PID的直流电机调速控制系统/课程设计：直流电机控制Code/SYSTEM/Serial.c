#include	"Serial.h"

bit bS1TrBusy;//���ڷ���æ��־
bit bS1Rec;//���ڽ��յ����ݱ�־
unsigned char cS1Rec;//���ڽ��յ��ַ�

void Uart1Init(void)		//115200bps@11.0592MHz,T2����ʱ��
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
//	T2L = 0xE8;		//�趨��ʱ��ֵ
//	T2H = 0xFF;		//�趨��ʱ��ֵ
  T2L = (65536 - (MAIN_Fosc/4/BAUD));   //���ò�������װֵ
  T2H = (65536 - (MAIN_Fosc/4/BAUD))>>8;	
	AUXR |= 0x10;		//������ʱ��2
		
////�򿪴����ж�	
	ES=1;           //ʹ�ܴ���1�ж�
  EA = 1;	
}


/*----------------------------
UART �жϷ������
-----------------------------*/	
void Uart1Interrupt() interrupt 4 using 1
{
	if(RI)
	{
		RI=0;				//���RIλ
		cS1Rec = SBUF; //�����ݶ���
		bS1Rec=1;//�ý��յ����ݱ�־λ		
	}
	if(TI)
	{
		TI=0;					//���TIλ
		bS1TrBusy=0; //��æ��־
	}
}

/*----------------------------
���ʹ�������
----------------------------*/
void S1SendData(unsigned char dat)
{
    while (bS1TrBusy);//�ȴ�ǰ������ݷ������
    bS1TrBusy = 1;
    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}


/*----------------------------
�����ַ���
----------------------------*/
void S1SendString(unsigned char *s)
{
    while (*s)                  //����ַ���������־
    {
        S1SendData(*s++);         //���͵�ǰ�ַ�
    }
}
