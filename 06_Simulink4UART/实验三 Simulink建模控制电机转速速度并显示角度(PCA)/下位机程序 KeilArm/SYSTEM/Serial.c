#include	"Serial.h"

bit bS1TrBusy;//���ڷ���æ��־
bit bS1Rec,uart_flag;//���ڽ��յ����ݱ�־
unsigned char cS1Rec[7]={0};//���ڽ��յ��ַ�
unsigned char sum2;
sbit LED=P5^5;
unsigned int sum;
unsigned char count=1;

void Uart1Init(void)		//115200bps@24MHz,T1����ʱ��
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xCC;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	
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
		RI=0;
		cS1Rec[count]=SBUF;

		if(count==1&&cS1Rec[count]==0xAA)           //֡ͷ������ֽ�
		{
			count=2;
			sum=0xAA;
		}
		
		else if(count==2&&cS1Rec[count]==0x05)
		{
			count=3;
			sum+=0x05;
		}
		
		else if(count==3&&cS1Rec[count]==0xAF)
		{
			count=4;
			sum+=0xAF;
		}
		
		else if(count==4&&cS1Rec[count]==0xF1)
		{
			count=5;
			sum+=0xF1;
		}
		
		else if(count==5&&cS1Rec[count]==0x01)
		{
			count=6;
			sum+=0x01;                                
		}
		
		else if(count==6)             //���ݣ�һ���ֽ�
		{
			count++;
			sum+=cS1Rec[count];   //У���
			sum2=sum&0xff;//ȡ��8λ
		}
		
		else if(count==7&&sum2==cS1Rec[7])   //��У������
		{
			//ES=0;            //�رմ��ڣ�ʹ�����ݽ��������´�
			uart_flag=1;     //һ֡����������ñ�־λΪ1
			count=1;         //������һ�����¿�ʼ
		}
		
		else              //������ȷ����������
		{
			count=1;
		}
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
