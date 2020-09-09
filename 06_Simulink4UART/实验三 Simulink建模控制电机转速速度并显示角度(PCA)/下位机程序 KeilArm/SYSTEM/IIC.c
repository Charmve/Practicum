#include  "IIC.h"
#include	"intrins.h"
#include	<STC15F2K60S2.h>
#include	"Serial.h"
#define nop _nop_();
#define IIC_ADDR 0x2A//Ӳ����ַ
sbit SCL=P0^6;
sbit SDA=P0^7;
// ------------------------------------------------------------
// IO��ģ��I2Cͨ��
// ------------------------------------------------------------


/*****************�����Ƕ�IIC���ߵĲ����ӳ���***/
/*****************��������**********************/
void IICStart( )
{
	SCL=0;                  //
	SDA=1;	
	nop;nop;	
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	SDA=0;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;
                  //
}

/*****************ֹͣIIC����****************/
void IICStop( )
{
	SCL=0;                  //
	SDA=0;	
	nop;nop;	
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	SDA=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;
}



/***************��IIC���߲���Ӧ��*******************/
void IICACK( )
{
	SCL=0;                  
	SDA=0;//
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;

}

/**************���Ӧ��λ*******************/
//�ȴ�Ӧ���źŵ���
//����ֵ: 0,����Ӧ��ʧ��
//        1,����Ӧ��ɹ�
bit IICReadAck( )
{
	unsigned char ucErrTime=0;

	SCL=0;                  
	SDA=1;//��IO���øߵ�ƽ	
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;
	while(SDA==1)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{

			IICStop( );
			return(0);//δ���յ�Ӧ��
		}
	}
	nop;nop;	
	SCL=0;
	return(1);
}

/*****************����IIC���߲���Ӧ��***************/
void IICNoAck( )
{
	
	SCL=0;                  
	SDA=1;//
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;

}

/*******************��IIC����д����*********************/
void IICSendByte(unsigned char sendbyte )
{
	unsigned char data j=8;

	for(;j>0;j--)
	   {
			 SCL=0;
			 sendbyte<<=1;        //����C51����ʵ�����������ʼ�ջ�ʹCY=sendbyte^7;
			 SDA=CY;
			 nop;nop;
			 SCL=1;
			 nop;nop;nop;nop;nop;nop;nop;			 
	   }
	SCL=0;
}

/**********************��IIC�����϶������ӳ���**********/
unsigned char IICReadByte( )
{
	unsigned char cReceiveData=0,i=8;
	SCL=0;
	SDA=1;//��IO���øߵ�ƽ	
	nop;nop;
	while(i--)
	   {
			 SCL=1;
			 nop;nop;nop;nop;
			 cReceiveData=(cReceiveData<<1)|SDA;
			 SCL=0;
			 nop;nop;nop;nop;nop;nop
			 nop;nop;nop;nop;nop;nop
	   }
	return(cReceiveData);
}


unsigned int Read2Byte(unsigned char RegisterAddress)//
{
	unsigned int RegisterData;//��Ŷ�����2���ֽ�
  IICStart( ); //��ʼ�ź�
	IICSendByte((IIC_ADDR<<1)|0 );//����������ַ+д����

	if(IICReadAck( )==1)		//�ȴ�Ӧ��
	{		

		
		IICSendByte(RegisterAddress );	//д�Ĵ�����ַ
		if(IICReadAck( )==0) return(0);		//�ȴ�Ӧ��
		IICStart( );
		IICSendByte((IIC_ADDR<<1)|1 );//����������ַ+������
		if(IICReadAck( )==0) return(0);		//�ȴ�Ӧ��
		RegisterData=IICReadByte( )<<8;//��ȡ����
		IICAck( );	//����ACK
		RegisterData|=IICReadByte( );  //��ȡ����
		IICNoAck( );	//����nACK
		IICStop( );			//����һ��ֹͣ����
		return RegisterData;		
	}
	else//��Ӧ��������LED����
	{
//		TestLed=0;
		return 0;	
		
	}
}