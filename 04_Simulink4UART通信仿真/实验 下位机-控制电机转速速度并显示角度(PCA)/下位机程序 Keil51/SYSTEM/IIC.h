#ifndef		__IIC_H
#define		__IIC_H






//void Delay(unsigned char DelayCount);
void IICStart( );
void IICStop( );
bit  IICReadAck( );
void IICNoAck( );
void IICAck( );
void IICSendByte(unsigned char sendbyte);
unsigned char IICReadByte( );
//bit IICWriteReadData(unsigned char ControlByte,unsigned char Address,unsigned char *DataBuff, unsigned char ByteQuantity );
//bit IICWriteReadManyData(unsigned char ControlByte,unsigned char Address,unsigned char *DataBuff, unsigned char ByteQuantity );//��д����1����������(16�ֽ�)������



#endif