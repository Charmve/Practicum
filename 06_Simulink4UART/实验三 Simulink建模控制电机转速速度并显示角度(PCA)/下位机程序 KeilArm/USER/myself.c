#include "myself.h"
#include	<STC15F2K60S2.h>
#include	"Serial.h"//����1(P3.0/RxD, P3.1/TxD)
#include	"intrins.h"

sbit s2=P1^1;        //TCS3200 S2 
sbit s3=P1^0;        //TCS3200 S3
                     //TCS3200 S0 ģ���ڲ�Ĭ������
                     //TCS3200 S1 ģ���ڲ�Ĭ������
                     //TCS3200 OE ģ���ڲ��ӵ�

sbit test_pin=P1^2;  //��ʾ������������ţ���֪����ʱ���ж�Ƶ��
//��������������
uchar ge,shi,bai;
uchar rp=3,gp=3,bp=4; //����������ӣ����廷�������޸�
uchar count;          //��ɫ��־λ��0:�� 1:�� 2:����

////��ʾ����
//uchar disp_R[3];  //��
//uchar disp_G[3];  //��
//uchar disp_B[3];  //��	            

u16 R = 0;
u16 G = 0;
u16 Bl = 0;


//*********************************************
//��ֵת������ʮ��ǧ��ASCII��
//*********************************************
//void conversion(uint temp_data)  
//{  
//    bai=temp_data/100+0x30 ;
//    temp_data=temp_data%100;   //ȡ������
//    shi=temp_data/10+0x30 ;
//    ge=temp_data%10+0x30;      //ȡ������
//}

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

void Timer0Init(void)		//10����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ    //ͬʱ�򿪶�ʱ��1����������������   P3.5
	TMOD|=0x40;
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	TL1 = 0x00;		
	TH1 = 0x00;	
	TF0 = 0;		//���TF0��־
	
	
	EA=1;            //�����ж�
 	ET0=1;           //����ʱ��0�ж�
	
	TR0=1;           
	TR1=1;           //����
}

/*******************************************
* ��������: c10ms_out() 
* ��������: ��ʱ�ж�0�������
            �޸���ɫ��־disp_tc��0:�� 1:�� 2:����
            ����S0 S1 S2 ѡ���˲���
            �������壬��ȡɫֵ
* ��ڲ���: ��
* ���ڲ���: ��
/********************************************/
void Timer0Interrupt() interrupt 1 using 2 //��ʱ��0���ж�
{
	u16 temp;
	test_pin=!test_pin; //���Զ�ʱ���ж�Ƶ�����ţ�������ʾ�����۲�
	TR0=0;              //�رն�ʱ
	TR1=0;              //�رռ���
//	S1SendData(count);
	//   count+1ʵ���ȼ����ɫ,�ټ����ɫ,Ȼ�����ɫ,ѭ�����       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //ѡ���˲���Ϊ��ɫ     
		
		//	temp=(8<<TH1)+TL1;
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		
	//	temp=(TH1<<8)+TL1;    //�������ʱ���� TCS230 �����������	
		temp/=rp;			
		R = temp;
		
//		conversion(temp);
//		disp_R[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
//		disp_R[1]=shi;
//		disp_R[0]=bai;
	}	
	else if(count==1)
	{            
		count++;
		s2=0;s3=1;            //ѡ���˲���Ϊ��ɫ
	//	temp=(TH1<<8)+TL1;    //�������ʱ���� TCS230 �����������	
		//	temp=(8<<TH1)+TL1;
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		
	temp/=gp;	
		G = temp;
//				if(G>0xFF)
//			G=0xFF;		temp/=bp
//		conversion(temp);
//		disp_G[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
//		disp_G[1]=shi;
//		disp_G[0]=bai;
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //ѡ���˲���Ϊ��ɫ
		
		//	temp=(8<<TH1)+TL1;
		
		//temp=(TH1<<8)+TL1;    //�������ʱ���� TCS230 �����������	
		
		temp=TH1;
		temp=(temp<<8)+TL1;
		temp/=bp;	
		Bl = temp;
//				if(Bl>0xFF)
//			Bl=0xFF;
//		conversion(temp);
//		disp_B[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
//		disp_B[1]=shi;
//		disp_B[0]=bai;	
		
	}
	//��ʱ���������ظ���ֵ
//	TH0=0xE0;
//	TL0=0x00; //11��0592M ����Ϊ10ms
//	TL0 = 0x00;		//���ö�ʱ��ֵ
//	TH0 = 0xDC;		//���ö�ʱ��ֵ
	
	TL1=0x00;//����������
	TH1=0x00;//����������
	TR0=1;   //�򿪶�ʱ��
	TR1=1;   //�򿪼�����
}




