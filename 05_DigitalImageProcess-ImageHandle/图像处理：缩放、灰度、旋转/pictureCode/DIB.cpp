// DIB.cpp: implementation of the DIB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picture.h"
#include "DIB.h"
#include "math.h"
#include "mydialog.h"
#include "mydialog1.h"
#include "mydialog2.h"

#define WIDTHBYTES(bits) ((bits+31)/32*4)
#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
//��Cnumber(x,y,z)������µ��ϣ������ҵĵ�x�У���y�У���z����ɫ������

#define Cnumber1(x,y,z) *(n_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))//���ڱ������ͼ 
#define gray(x,y) *(m_pDIBData+(x)*GetBmpRealWidth()+y) 
#define gray1(x,y) *(n_pDIBData+(x)*GetBmpRealWidth()+y )//���ڱ��ݻҶ�ͼ 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DIB::DIB()
{
	m_pBMI=NULL;
    m_pDIBData=NULL;
}

DIB::~DIB()
{
	if(m_pBMI!=NULL)
		delete m_pBMI;
	if(m_pDIBData!=NULL)
		delete m_pDIBData;
}

int DIB::GetDIBWidth()
{
	return m_pBMI->bmiHeader.biWidth;
}

int DIB::GetDIBHeight()
{
	return m_pBMI->bmiHeader.biHeight;
}

DWORD DIB::GetDIBSize()
{
	return m_pBMI->bmiHeader.biSize;
}

int DIB::GetBmpRealWidth()
{
	return (bfh.bfSize-bfh.bfOffBits)/GetDIBHeight();
}

BOOL DIB::LoadFromFile(LPCTSTR lpszFileName)
{
	CFile file;
	BITMAPINFO* pBMI=NULL;
	BYTE* pDIBData=NULL;

	if(!file.Open(lpszFileName,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("�򲻿��ļ�");
		return FALSE;
	}
	
	if(file.Read(&bfh,sizeof(bfh))!=sizeof(bfh))
	{
		AfxMessageBox("���ļ�����");
		return FALSE;
	}
	if(bfh.bfType!=0x4d42)
	{
		AfxMessageBox("����BMP�ļ�");
		return FALSE;
	}
	if(file.Read(&bih,sizeof(bih))!=sizeof(bih))
	{
		AfxMessageBox("���ļ�����");
		return FALSE;
	}

	if(bih.biBitCount==24)
	{
		pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
	    if(!pBMI)
		{
		    AfxMessageBox("�����ڴ����");
		    return FALSE;
		}
	    memcpy(pBMI,&bih,sizeof(BITMAPINFOHEADER));
	}
	else if(bih.biBitCount==8)
	{
        pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD) * 256];
		memcpy(&pBMI->bmiHeader,&bih,sizeof(BITMAPINFOHEADER));
		file.Read(pBMI->bmiColors,sizeof(RGBQUAD) * 256);
	}
	else
	{
		AfxMessageBox("����8λ�Ҷ�ͼ��24λ���ɫͼ");
		return FALSE;
	}
	
	DWORD dataBytes=bfh.bfSize-bfh.bfOffBits;

	pDIBData=(BYTE*)new char[dataBytes];
	if(!pDIBData)
	{
		AfxMessageBox("�����ڴ����");
		delete pBMI;
		return FALSE;
	}
     if(file.ReadHuge(pDIBData,dataBytes)!=dataBytes)
	{
		AfxMessageBox("�����ڴ����");
		delete pBMI;
		delete pDIBData;
		return FALSE;
	}
    file.Close();
	if(m_pBMI!=NULL)
		delete m_pBMI;
	m_pBMI=pBMI;
	if(m_pDIBData!=NULL)
		delete m_pDIBData;
	m_pDIBData=pDIBData; 

	return true; 

}  


void DIB::ShowDIB(CDC *pDC, int nLeft, int nTop, int nWidth, int nHeight)
{
	int w,h;
	pDC->SetStretchBltMode(COLORONCOLOR);
    
	if(nWidth*GetDIBHeight()>=nHeight*GetDIBWidth())//Ϊ�˰�ͼ��ԭ������ʾ��������ʾ�����С
	{
		w=nHeight*GetDIBWidth()/GetDIBHeight();
		h=nHeight;
	}
	else
	{
		w=nWidth;
		h=nWidth*GetDIBHeight()/GetDIBWidth();
	}
	StretchDIBits(pDC->GetSafeHdc(),
		         nLeft,nTop,w,h,
				 0,0,GetDIBWidth(),GetDIBHeight(),m_pDIBData,
				 m_pBMI,DIB_RGB_COLORS,SRCCOPY);
}

BOOL DIB::SaveFile(const CString filename)
{
	CFile file(filename,CFile::modeWrite|CFile::modeCreate);
	file.Write(&bfh,sizeof(bfh));
	if(bih.biBitCount==24)
	{
		file.Write(&bih,sizeof(bih));
	}
	else
	{
		file.Write(&m_pBMI->bmiHeader,sizeof(BITMAPINFOHEADER));
		file.Write(m_pBMI->bmiColors,sizeof(RGBQUAD)*256);
	}
	file.Write(m_pDIBData, bfh.bfSize-bfh.bfOffBits);
	return TRUE;
}

BOOL DIB::RGB2GRAY()
{
    int gray;
	int height = GetDIBHeight(); //��
    int width = GetDIBWidth(); //��

	if(bih.biBitCount!=24)
	{
		AfxMessageBox("ԭͼ����24λ���ͼ");
		return false;
	}  

   else
   {
	   for (int x = 0; x < height; x++) //��
		   for (int y = 0; y < width; y++) //��
		   {
				gray=(int)(Cnumber(x,y,0)*0.299+Cnumber(x,y,1)*0.587+Cnumber(x,y,2)*0.114); //���ȷ���
				Cnumber(x,y,0)=gray;Cnumber(x,y,1)=gray;Cnumber(x,y,2)=gray; //ʹ������������			
		   }
   }
   return true;
}

BOOL DIB::CY422() //��ɫͼ���Ӳ��� 
{
	int x,y; 
	double Y,Cr,Cb,i,j,k; 
	if(bih.biBitCount!=24) 
	{
		AfxMessageBox("ԭͼ����24λ���ͼ"); 
		return false; 
	} 
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=1;y<GetDIBWidth();) 
			{ 
				Y=0.299*Cnumber(x,y-1,2)+0.587*Cnumber(x,y-1,1)+0.114*Cnumber(x,y-1,0); 
						//���Ƚ����ر任�� YCrCb, 
						//Ȼ������ 
						//������ȣ��ٷ��任��RGB 
				Cb=0.564*(Cnumber(x,y-1,0)-Y); 
				Cr=0.713*(Cnumber(x,y-1,2)-Y); 
				Y=0.299*Cnumber(x,y,2)+0.587*Cnumber(x,y,1)+0.114*Cnumber(x,y,0); 
				i=(Y+1.402*Cr); 
				j=(Y-0.344*Cb-0.714*Cr); 
				k=(Y+1.772*Cb);
				
				if(i>255) i=255; 
				else if(i<0) i=0; 
				if(j>255) j=255; 
				else if(j<0) j=0; 
				if(k>255) k=255; 
				else if(k<0) k=0; 

				Cnumber(x,y,2)=(BYTE)i; 
				Cnumber(x,y,1)=(BYTE)j; 
				Cnumber(x,y,0)=(BYTE)k; 

				y=y+2; 
			} 
		} 
	} 
	return true; 
}

BOOL DIB::Fanse()//��ɫ�任 
{
	int x,y; 
	BYTE i; 
	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				//���صĲ�ɫֵ��Ϊ�䷴ɫ
				Cnumber(x,y,0)=255-Cnumber(x,y,0); 
				Cnumber(x,y,1)=255-Cnumber(x,y,1); 
				Cnumber(x,y,2)=255-Cnumber(x,y,2);

				if(Cnumber(x,y,0)>255) Cnumber(x,y,0)=255; 
				else if(Cnumber(x,y,0)<0) Cnumber(x,y,0)=0; 
				if(Cnumber(x,y,1)>255) Cnumber(x,y,1)=255; 
				else if(Cnumber(x,y,1)<0) Cnumber(x,y,1)=0; 
				if(Cnumber(x,y,2)>255) Cnumber(x,y,2)=255; 
				else if(Cnumber(x,y,2)<0) Cnumber(x,y,2)=0; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				gray(x,y)=255-gray(x,y);//���صĲ�ɫֵ��Ϊ�䷴ɫ 
				i = gray(x,y); 
				if(i>255) i=255; 
				if(i<0) i=0; 
			}
		} 
		return true;
	}
	

		else 
		{ 
			AfxMessageBox("ԭͼ�������ͼ��Ҷ�ͼ"); 
			return false;
		} 
}

BOOL DIB::ContrastINC() //����Աȶ�
{
	int x,y;
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 
				
				if(r>127.5) r=r+20;//������ֵ��ʹ����� 
				else if(r<127.5) r=r-10;//С����ֵ��ʹ���С
				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>127.5) g=g+20; 
				else if(g<127.5) g=g-10; 
				if(g>255) g=255; 
				else if(g<0) g=0; 
				if(b>127.5) b=b+20; 
				else if(b<127.5) b=b-10; 
				if(b>255) b=255; 
				else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{  
				i=gray(x,y); 
				if (i>127.5) i=i+10; 
				else if (i<127.5) i=i-10; 
				if(i>255)i=255; 
				else if(i<0)i=0; 
				gray(x,y)=(BYTE)i; 
			} 
		} 
		return true;
	} 
	
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++)
			AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
}

BOOL DIB::ContrastIDEC() //��С�Աȶ�
{ 
	int x,y; 
	double r,g,b;
	double i; 
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 

				if(r>127.5) r=r-20;//������ֵ��ʹ���С 
				else if(r<127.5) r=r+10; 
				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>127.5) g=g-20; 
				else if(g<127.5) g=g+10; 
				if(g>255) g=255; else if(g<0) g=0; 
				if(b>127.5) b=b-20; else if(b<127.5) b=b+10; 
				if(b>255) b=255; else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y); 
				if (i>127.5) i=i-10; 
				else if (i<127.5) i=i+10; 
				if(i>255)i=255; 
				else if(i<0)i=0; 
				gray(x,y)=(BYTE)i; 
			} 
		} 
		return true;
	} 
	

	else 
	{ 
		AfxMessageBox("ԭͼ�������ͼ"); 
		return false; 
	} 
}

BOOL DIB::BrightnesINC() //��������
{
	int x,y; 
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2)*1.5; g=Cnumber(x,y,1)*1.5; b=Cnumber(x,y,0)*1.5;

				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>255) g=255; 
				else if(g<0) g=0; 
				if(b>255) b=255; 
				else if(b<0) b=0; 
				
				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y)*1.5; 
				if (i>255)	i=255; 
				if (i<0)	i=0; 
				gray(x,y)= (BYTE)i;
			}
		}
		return true;
	} 
	

	else
	{
		AfxMessageBox("ԭͼ������ʻ�Ҷ�"); 
		return false; 
	} 
}

BOOL DIB::BrightnesDEC() //��С����
{
	int x,y; 
	double r,g,b;
	double i; 
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{ 
				r=Cnumber(x,y,2)*0.5; g=Cnumber(x,y,1)*0.5; b=Cnumber(x,y,0)*0.5; 

				if(r>255) r=255; 
				else if(r<0) r=0; 
				if(g>255) g=255; 
				else if(g<0) g=0;
				if(b>255) b=255; 
				else if(b<0) b=0;

				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if (bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y)*0.5; 
				if (i>255)	i=255; 
				if (i<0)	i=0; 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	}
	
		else
		{
			AfxMessageBox("ԭͼ�������ͼ"); 
			return false; 
		} 
}



BOOL DIB::ExtractRED() //��ȡR���� 
{
	int x,y; 
	double r,g,b; 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("ԭͼ�������ͼ"); 
		return false; 
	} 
	
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=0; b=0;//��ɫ����ɫ������0 
				if(r>255) r=255; 
				else if(r<0) r=0; 
				
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true; 
} 

BOOL DIB::ExtractGREEN()//��ȡG���� 
{ 
	int x,y; 
	double r,g,b; 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("ԭͼ�������ͼ"); 
		return false; 
	} 
	else 
	{
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				g=Cnumber(x,y,1); r=0; b=0;//��ɫ�ͺ�ɫ������0 
				if(g>255) r=255; else if(g<0) r=0;

				Cnumber(x,y,2)=(BYTE)r; Cnumber(x,y,1)=(BYTE)g; Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true;
} 

BOOL DIB::ExtractBLUE() //��ȡB����
{
	int x,y; 
	double r,g,b;
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("ԭͼ�������ͼ"); 
		return false; 
	} 
	else 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				b=Cnumber(x,y,0); r=0; g=0;//��ɫ����ɫ������0 

				if(b>255) r=255; 
				else if(b<0) r=0; 

				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g;
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
	} 
	return true;
}


BOOL DIB::MirrorUp2Down()//ͼ�����¾��� 
{
	int x,y; 
	double r,g,b;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				if(r>255) r=255; else if(r<0) r=0; 
				if(g>255) g=255; else if(g<0) g=0; 
				if(b>255) b=255; else if(b<0) b=0; 

				//���ذ����¾�����������
				r=Cnumber(GetDIBHeight()-x-1,y,2); (Cnumber(GetDIBHeight()-x-1,y,2))=(BYTE)Cnumber(x,y,2); //��
				g=Cnumber(GetDIBHeight()-x-1,y,1); (Cnumber(GetDIBHeight()-x-1,y,1))=(BYTE)Cnumber(x,y,1);
				b=Cnumber(GetDIBHeight()-x-1,y,0); (Cnumber(GetDIBHeight()-x-1,y,0))=(BYTE)Cnumber(x,y,0); //��

				//���ذ����¾��������� 
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(GetDIBHeight()-x-1,y);
				gray(GetDIBHeight()-x-1,y)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	} 
	

	else 
	{ 
		AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
}

BOOL DIB::MirrorLeft2Right() //ͼ�����Ҿ���
{
	int x,y; 
	double temp;
	double i; 

	if(bih.biBitCount==24)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<=GetDIBWidth()/2;y++) 
			{ 
				if(temp>255) temp=255; else if(temp<0) temp=0; 

				for(int k = 0; k<3; k++)
				{
					//���ذ����Ҿ�����������
					temp=Cnumber(x,GetDIBWidth()-y-1,k); 
					Cnumber(x,GetDIBWidth()-y-1,k)=Cnumber(x,y,k);
					Cnumber(x,y,k)=(BYTE)temp;
				}
			} 
		} 
		return true; 
	}

	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<=int(GetDIBHeight()/2);x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(GetDIBHeight()-x-1,y);

				gray(GetDIBHeight()-x-1,y)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	} 	

	else { 
		AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
}

BOOL DIB::GamaCorrection() //ͼ��٤��У�� 
{ 
	int x,y; 
	double r,g,b;
	double i; 
	
	mydialog dlg; 	
	dlg.m_value=NULL; //��Ӧ������rֵ

	if(dlg.DoModal()!=IDOK) 
	{ 
		return false; 
	} 
	
	float value=dlg.m_value;//�������rֵ���͸�value	
	delete dlg;
	
	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2); g=Cnumber(x,y,1); b=Cnumber(x,y,0); 

				r=255*pow(r/255,value); 
				if(r>255) r=255; else if(r<0) r=0; 
				g=255*pow(g/255,value); 
				if(g>255) g=255; else if(g<0) g=0; 
				b=255*pow(b/255,value); 
				if(b>255) b=255; else if(b<0) b=0;  
					
				Cnumber(x,y,2)=(BYTE)r; 
				Cnumber(x,y,1)=(BYTE)g; 
				Cnumber(x,y,0)=(BYTE)b; 
			} 
		}
		return true;
	}
	
	
	else if(bih.biBitCount==8)
	{
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				i=gray(x,y); 
				i=255*pow(i/255,value); 
				if(i>255) i=255; else if(i<0) i=0;
				gray(x,y)=(BYTE)i;
			}
		}
		return true;
	}
	
		else 
		{
			AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
			return false; 
		} 
}

//��һ��ͼƬת���ǶȺ󷵻��µ�ͼƬ
CBitmap* DIB::BmpRotate(CBitmap* cBmp, float Angle)
{
	BITMAP bmp;
	cBmp->GetBitmap(&bmp);
	BYTE *pBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight], *TempBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight];
	cBmp->GetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, pBits);
	//#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
	Angle = float (Angle*3.1415926 / 180);
	int interval=bmp.bmWidthBytes/bmp.bmWidth;
	double rx0 = bmp.bmWidth*0.5;
	double ry0 = bmp.bmHeight*0.5;

	for (int j = 0; j < bmp.bmHeight; j ++)
	{
		for (int i = 0; i < bmp.bmWidth; i++)
		{
			for (int k = 0; k < interval; k++)
			{
				TempBits[i*bmp.bmWidthBytes + j*interval + k] = 0xff;
			}
		}
	}

	for (j = 0; j < bmp.bmHeight; j++)
	{
		for (int i = 0; i< bmp.bmWidth; i++)
		{
			int tempI, tempJ;
			tempI = int ((i-rx0)*cos(Angle) + (j-ry0)*sin(Angle)+rx0);
			tempJ = int (-(i-rx0)*sin(Angle) + (j-ry0)*cos(Angle)+ry0);
			if (tempI>0&&tempI<bmp.bmWidth)
				if (tempJ>0 && tempJ < bmp.bmHeight)
				{
					for (int m = 0; m < interval;m++)
						TempBits[i*bmp.bmWidthBytes + j*interval+m] = pBits[tempI*bmp.bmWidthBytes + interval*tempJ+m];
				}
		}
	}
	CBitmap *m_bitmap;
	m_bitmap = new CBitmap;
	m_bitmap->CreateBitmapIndirect(&bmp);
	m_bitmap->SetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, TempBits);
	delete pBits;
	delete TempBits;

	return m_bitmap;
}

/*
BOOL DIB::Rotation90() //ͼ������ת90��(ֻ�ܶ�������ͼ�����) 
{ 
	int x,y; 
	double r,g,b; //��������
	double i,j;

	if(bih.biBitCount==24)
	{  
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=x;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(y,x,2);Cnumber(y,x,2)=(byte)r; 
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(y,x,1);Cnumber(y,x,1)=(byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(y,x,0);Cnumber(y,x,0)=(byte)b;
				//��ͼ��������ذ�ת�ý������൱�ھ���ת�ã� 
			} 
		} 
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(GetDIBHeight()-1-x,y,2);Cnumber(GetDIBHeight()-1-x,y,2)= (byte)r;
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(GetDIBHeight()-1-x,y,1);Cnumber(GetDIBHeight()-1-x,y,1)= (byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(GetDIBHeight()-1-x,y,0);Cnumber(GetDIBHeight()-1-x,y,0)= (byte)b; 
		/*for(x=0;x<GetDIBHeight();x++)
		{ 
			for(y=0;y<GetDIBWidth();y++)
			{
				for(int k = 0; k<3; k++)
					Cnumber(x,y,k) = Cnumber1(y,x,k);
					//b=Cnumber(x,y,k);Cnumber(x,y,k)=Cnumber(y,x,k);Cnumber(y,x,k)=(byte)b;

				
				#define Cnumber(x,y,z) *(m_pDIBData+(x)*GetBmpRealWidth()+(y)*3+(z))
				src[(i * desW + j) * channel + k] = tempSrc[((srcH - 1 - j) * srcW + i) * channel + k];
				src[(i * desW + j) * channel + k] = tempSrc[((srcH - 1 - i) * srcW + srcW - 1 - j) * channel + k];
				
			} 
		}
		//m_pBMI->bmiHeader.biHeight = GetDIBWidth();
		//m_pBMI->bmiHeader.biWidth = GetDIBHeight();
		AfxMessageBox("Hello"); 
		return true; 
	} 

	else if(bih.biBitCount==8)
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 	
 
				i=gray(y,x); 
				gray(y,x)=(BYTE)gray(x,y); 
				gray(x,y)=(BYTE)i; 
				if(i>255)	i=255; 
				else if(i<0)	i=0; 
			} 
			for(x=0;x<=int(GetDIBHeight()/2);x++) 
			{ 
				for(y=0;y<GetDIBWidth();y++) 
				{ 
					j=gray(GetDIBHeight()-x-1,y); 
					(gray(GetDIBHeight()-x-1,y))=(BYTE)gray(x,y); gray(x,y)=(BYTE)j; 
				}
			}
		}
		return true;
	} 
	

	else { 
		AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
}

*/


BOOL DIB::Rotation90() //ͼ������ת90��(ֻ�ܶ�������ͼ�����) 
{ 
	int x,y; 
	double r,g,b;
	double i,j; 
	
	if(GetDIBWidth()%GetDIBHeight())
	{
		AfxMessageBox("�����һ��������ͼƬ:)"); 
		return false; 
	}

	if(bih.biBitCount==24) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=x;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(y,x,2);Cnumber(y,x,2)=(byte)r; 
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(y,x,1);Cnumber(y,x,1)=(byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(y,x,0);Cnumber(y,x,0)=(byte)b;
				//��ͼ��������ذ�ת�ý������൱�ھ���ת�ã� 
			} 
		} 
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				r=Cnumber(x,y,2);Cnumber(x,y,2)=Cnumber(GetDIBHeight()-1-x,y,2);Cnumber(GetDIBHeight()-1-x,y,2)= (byte)r;
				g=Cnumber(x,y,1);Cnumber(x,y,1)=Cnumber(GetDIBHeight()-1-x,y,1);Cnumber(GetDIBHeight()-1-x,y,1)= (byte)g; 
				b=Cnumber(x,y,0);Cnumber(x,y,0)=Cnumber(GetDIBHeight()-1-x,y,0);Cnumber(GetDIBHeight()-1-x,y,0)= (byte)b; 
				//��ͼ������ת�ú��ٽ�ͼ������ֵ���������У��ۺ�ת���뾵�񣬼��ɴﵽͼ����ת90
			} 
		} 
		return true; 
	}  

	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 	
 
				i=gray(y,x); gray(y,x)=(BYTE)gray(x,y); gray(x,y)=(BYTE)i; 
				if(i>255)	i=255; 
				else if(i<0)	i=0; 
			}
		}
		for(x=0;x<GetDIBHeight()/2;x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 					
				j=gray(x,y); gray(x,y)=gray(GetDIBHeight()-x-1,y);gray(GetDIBHeight()-x-1,y)=(byte)j;
			}
		}
		return true;
	} 
	

	else { 
		AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
}


BOOL DIB::Resort1()//ͼ��ԭ����һ 
{
	
	int x,y; 

	if(bih.biBitCount==24) 
	{ 
		for(y=0;y<GetDIBWidth();y++) 
		{ 
			for(x=0;x<GetDIBHeight();x++)
			{	 
				//��Ԥ�ȱ����ԭͼ���¸��Ƹ�Cnumber(x,y,z)
				Cnumber(x,y,0)=Cnumber1(x,y,0); 
				Cnumber(x,y,1)=Cnumber1(x,y,1);
				Cnumber(x,y,2)=Cnumber1(x,y,2); 
			} 
		} 
		return true;
	}
	
	
	else if(bih.biBitCount==8) 
	{ 
		for(x=0;x<GetDIBHeight();x++) 
		{ 
			for(y=0;y<GetDIBWidth();y++) 
			{ 
				gray(x,y)=gray1(x,y); //��Ԥ�ȱ����ԭͼ���¸��Ƹ� gray(x,y)
			} 
		}
		return true;
	} 
	
	else { 
		AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
		return false; 
	} 
	
	return true;
} 

BOOL DIB::Convert24toGray() //24λ���ɫת8λ�Ҷ�ͼ
{ 
	int i,j,n; 
	int DataByte; 
	BITMAPINFO* pBMI = NULL;//�¶���λͼ��Ϣͷ�ṹָ��pBMI 
	BYTE* pDIBData = NULL;//�¶���ָ��λͼ���ػҶ�ֵ��ָ��pDIBData
	
	//�жϴ�ͼ���Ƿ�Ϊ 24 λͼ 
	if(bih.biBitCount!=24) 
	{ 
		AfxMessageBox("ԭͼ���� 24 λ���ɫͼ"); 
		return false; 
	} 

	long w=GetDIBWidth();
	if(w%4!=0)//�жϿ���ǲ���4�ı���
	{
		n=w/4; 
		w=4*n+4;
	} 
	
	DataByte=w;//һ�д���0������
	int supply = DataByte - GetDIBWidth(); //��0�ĸ���
	pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256]; //Ϊ��Ϣͷ�����ڴ�ռ� 
	pDIBData=(BYTE*)new char[GetDIBHeight()*DataByte];//Ϊ��ͼ������ص�����ڴ�ռ䣨������ 
	
	//�����µ���ɫ�� 
	for(i=0;i<256;i++)
	{ 
		pBMI->bmiColors[i].rgbRed= (unsigned char)i; 
		pBMI->bmiColors[i].rgbGreen=(unsigned char)i; 
		pBMI->bmiColors[i].rgbBlue=(unsigned char)i; 
		pBMI->bmiColors[i].rgbReserved=0;
	} 
		
	for(i=0;i<GetDIBHeight();i++) {
		for(j=0;j<GetDIBWidth();j++) //������
			*(pDIBData+i*(GetDIBWidth()+supply)+j)=(byte)(0.299*Cnumber(i,j,2)+0.587*Cnumber(i,j,1)+0.114*Cnumber(i,j,0));//�Ҷ�ֵ�������ȷ��̼��� 
		
		for(int k = 0; k<(DataByte-GetDIBWidth()); k++)//����0
			*(pDIBData+i*supply+(i+1)*GetDIBWidth()) = (byte) 0x00;
	}	

	bih.biBitCount=8;//������ɫ���
	pBMI->bmiHeader.biClrUsed=256;
    pBMI->bmiHeader.biSizeImage=GetDIBHeight()*DataByte;
	bfh.bfOffBits=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)* 256;//ʵ��ͼ������ƫ���� 
	bfh.bfSize=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + GetDIBHeight()*DataByte;//�ļ��ܴ�С 
	memcpy(&pBMI->bmiHeader,&bih,sizeof(BITMAPINFOHEADER)); 
	
	if(m_pBMI!=NULL)// �ж�ԭͼ��Ϣͷ�Ƿ�Ϊ��
	delete m_pBMI;
	m_pBMI=pBMI; 

	if(m_pDIBData!=NULL)//�ж�ԭͼ�����Ƿ�Ϊ��
	delete m_pDIBData; 
	m_pDIBData=pDIBData; 
	
	return true; 
}

BOOL DIB::ScalingNearest()  //ͼ������
{	
	mydialog1 dlg;
	float times = 0;
	dlg.m_scaling=NULL;//��Ӧ����������ϵ��
	if (dlg.DoModal() != IDOK)
	{
		return false;	//����
	}	
	times = dlg.m_scaling;//�����������ϵ�����͸�ExpScalValue	
	delete dlg;

	int width = GetDIBWidth();
	int height = GetDIBHeight();
	int MYDRAW_HEIGHT = height * times;
	int MYDRAW_WIDTH = width * times;
	int T_width = WIDTHBYTES(MYDRAW_WIDTH * bih.biBitCount);		
	int L_width = WIDTHBYTES(width * bih.biBitCount);				
	long T_size = T_width * MYDRAW_HEIGHT;							
	long L_size = L_width * height;									
	BYTE* pColorData = (BYTE*)new char[L_size];						
	memset(pColorData, 0, L_size);									
	BYTE* pColorDataMid = (BYTE*)new char[T_size];					
	memset(pColorDataMid, 0, T_size);								
	memcpy(pColorData, m_pDIBData, L_size);							
	bih.biWidth = m_pBMI->bmiHeader.biWidth = MYDRAW_WIDTH;
	bih.biHeight = m_pBMI->bmiHeader.biHeight = MYDRAW_HEIGHT;
	bih.biSizeImage = m_pBMI->bmiHeader.biSizeImage = T_size;
	bfh.bfSize = T_size + bfh.bfOffBits;
	for (int hnum = 0; hnum < MYDRAW_HEIGHT; hnum++)
		for (int wnum = 0; wnum < MYDRAW_WIDTH; wnum++)
		{
			double d_original_img_hnum = hnum * height / (double)MYDRAW_HEIGHT;
			double d_original_img_wnum = wnum * width / (double)MYDRAW_WIDTH;
			int i_original_img_hnum = (int)d_original_img_hnum;
			int i_original_img_wnum = (int)d_original_img_wnum;
			double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;   
			double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;
			int original_point_a = i_original_img_hnum * L_width + i_original_img_wnum * 3;
			int original_point_b = i_original_img_hnum * L_width + (i_original_img_wnum + 1) * 3;
			int original_point_c = (i_original_img_hnum + 1) * L_width + i_original_img_wnum * 3;
			int original_point_d = (i_original_img_hnum + 1) * L_width + (i_original_img_wnum + 1) * 3;
			if (i_original_img_hnum + 1 >= height - 1)
			{
				original_point_c = original_point_a;
				original_point_d = original_point_b;
			}
			if (i_original_img_wnum + 1 >= width - 1)
			{
				original_point_b = original_point_a;
				original_point_d = original_point_c;
			}
			int pixel_point = hnum * T_width + wnum * 3;
			pColorDataMid[pixel_point] =
				pColorData[original_point_a] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c] * distance_to_a_y * distance_to_a_x;
			pColorDataMid[pixel_point + 1] =
				pColorData[original_point_a + 1] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b + 1] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c + 1] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c + 1] * distance_to_a_y * distance_to_a_x;
			pColorDataMid[pixel_point + 2] =
				pColorData[original_point_a + 2] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
				pColorData[original_point_b + 2] * distance_to_a_x * (1 - distance_to_a_y) +
				pColorData[original_point_c + 2] * distance_to_a_y * (1 - distance_to_a_x) +
				pColorData[original_point_c + 2] * distance_to_a_y * distance_to_a_x;
		}
		if (m_pDIBData != NULL)
			delete m_pDIBData;
		m_pDIBData = pColorDataMid;
		delete pColorData;
}

/*///////////////////////////////////////////////////
d
////////////////////////////////////////////////////

BOOL DIB::ScalingNearest()
{	
	BYTE* pDIBData = NULL;//�¶���ָ��λͼ���ػҶ�ֵ��ָ��pDIBData
	BYTE* pDIBDataS = NULL;//��̬����洢���ź��ͼƬ��������Ϣ
	
	float ExpScalValue = 0;
	int FloatToIntwidth, FloatToIntheight;//С���������(float To Int)
	int RotateAngle = 90; //Ĭ��������ת�Ƕ�

	mydialog1 dlg;
	dlg.m_scaling=NULL;//��Ӧ����������ϵ��
	if (dlg.DoModal() != IDOK)
	{
		return false;	// ����
	}	
	ExpScalValue = dlg.m_scaling;//�����������ϵ�����͸�ExpScalValue	
	delete dlg;

	//���ExpScalValue����С������Ҫ������
	//�����������Ž��ȡ��
	FloatToIntwidth = (int)(ExpScalValue*GetDIBWidth());
	FloatToIntheight = (int)(ExpScalValue*GetDIBHeight());

	//ͼ��ÿһ�е��ֽ���������4��������
	int lineByte2 = (FloatToIntwidth * bih.biBitCount / 8 + 3) / 4 * 4;
	pDIBDataS = new BYTE[lineByte2 * FloatToIntheight];//Ϊ���ź�ͼ�����洢�ռ�

	int pre_x, pre_y, after_x, after_y; //����ǰ���Ӧ�����ص�����

	if(bih.biBitCount==24) 
	{
		for (int x = 0; x<FloatToIntheight; x++)
		{
			for (int y = 0; y<FloatToIntwidth; y++)
			{
				for (int k = 0; k < 3; k++)
				{
					after_x = x;
					after_y = y;
					pre_x = (int)(after_x / ExpScalValue);//ȡ������ֵ����Ϊ�����ڽ���ֵ������ȡ������
					pre_y = (int)(after_y / ExpScalValue); 
					if (pre_x >= 0 && pre_x < GetDIBHeight() && pre_y >= 0 && pre_y < GetDIBWidth())//��ԭͼ��Χ��
						*(pDIBDataS+x*lineByte2+y*3+k)= *(pDIBData + pre_x * GetDIBWidth()*3 + pre_y*3+k);
				}
			}
		}
		
		delete[] pDIBDataS;
		if(m_pDIBData!=NULL)//�ж�ԭͼ�����Ƿ�Ϊ��
			delete m_pDIBData; //�ͷ��ڴ�
		m_pDIBData = pDIBDataS; 
		m_pBMI->bmiHeader.biHeight = GetDIBWidth();
		m_pBMI->bmiHeader.biWidth = GetDIBHeight();
		return true;
	}
	
	
	else if(bih.biBitCount==8)
	{
		
		
		
		AfxMessageBox("ԭͼ����24λ���ͼ��8λ�Ҷ�"); 
		return false;
	}
	
	
		else 
		{
			AfxMessageBox("ԭͼ�������ͼ��Ҷ�"); 
			return false; 
		} 
}
*/

BOOL DIB::UserTip()
{
	mydialog2 dlg;
	if (dlg.DoModal() != IDOK)
	{
		return false;	// ����
	}
	delete dlg;

	return true;
}
