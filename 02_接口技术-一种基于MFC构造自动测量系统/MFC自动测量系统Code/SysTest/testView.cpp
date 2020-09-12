// testView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"
#include "testView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
ON_COMMAND_RANGE(ID_DRAW,ID_WORD,OnMenu)
		//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here

}
CTestView::~CTestView()
{
	// TODO: add construction code here

}



BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	switch(id)
	{
		case ID_DRAW:
			OnMyDraw(pDC);
			break;
		case ID_WORD:
			OnWord(pDC);
			break;
		case ID_PEN:
			OnPen(pDC);
			break;
		case ID_BRUSH:
			OnBrush(pDC);
			break;
	};
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers
void CTestView::OnMenu(UINT nID) 
{
	id=nID;
	Invalidate();
}

void CTestView::OnMyDraw(CDC* pDC) 
{
	// TODO: Add your command handler code here
	pDC->TextOut(20,20,"Point");

	pDC->SetPixel(100,20,RGB(255,0,0));
	pDC->SetPixel(110,20,RGB(0,255,0));
	pDC->SetPixel(120,20,RGB(0,0,255));
	pDC->SetPixel(120,20,RGB(255,255,0));
	pDC->SetPixel(140,20,RGB(255,0,255));
	pDC->SetPixel(150,20,RGB(0,255,255));
	pDC->SetPixel(160,20,RGB(0,0,0));

	//����ֱ��
	pDC->TextOut(20,60,"Line");
	pDC->MoveTo(20,90);
	pDC->LineTo(160,90);
	
	//��������
	POINT polyline[4]={{70,240},{20,190},{70,190},{20,240}};
	POINT polyPolyline[9]={{95,160},{120,185},{120,250},{145,160},{120,185},
						{90,185},{150,185},{80,210},{160,210}};
	DWORD dwPolyPoints[4]={3,2,2,2};

	pDC->Polyline(polyline,4);
	pDC->PolyPolyline(polyPolyline,dwPolyPoints,4);

	//���Ʊ��������(Bezier)
	POINT polyBezier[4]={{20,310},{60,240},{120,300},{160,330}};
	pDC->PolyBezier(polyBezier,4);

	//����Բ��Բ������Բ
	for(int i=0;i<6;i++)
	{
		pDC->Arc(260-5*i,70-5*i,260+5*i,70+5*i,260+5*i,70,260+5*i,70);
	}
	for(i=3;i<6;i++)
	{
		pDC->Arc(260-10*i,70-10*i,260+10*i,70+10*i,
			(int)260+10*i*cos(60*3.1415926/180),
			(int)70+10*i*sin(60*3.1415926/180),
			(int)260+10*i*cos(60*3.1415926/180),
			(int)70-10*i*sin(60*3.1415926/180));

		pDC->Arc(260-10*i,70-10*i,260+10*i,70+10*i,
			(int)260-10*i*cos(60*3.1415926/180),
			(int)70-10*i*sin(60*3.1415926/180),
			(int)260-10*i*cos(60*3.1415926/180),
			(int)70+10*i*sin(60*3.1415926/180));
	}

	pDC->Ellipse(260-50,200-30,260+50,200+30);

	//���ƾ��κ�Բ�Ǿ���
	pDC->Rectangle(190,270,250,310);
	pDC->RoundRect(265,270,330,310,30,20);	

	//�������κ�����
	pDC->Chord(360-70,70-70,360+70,70+70,
		(int)360+70*cos(60*3.1415926/180),
		(int)70+70*sin(60*3.1415926/180),
		(int)360+70*cos(30*3.1415926/180),
		(int)70-70*sin(30*3.1415926/180));
	pDC->Pie(480-70,70-80,480+80,70+80,
		(int)480+70*cos(60*3.1415926/180),
		(int)70+70*sin(60*3.1415926/180),
		(int)480+70*cos(30*3.1415926/180),
		(int)70-70*sin(30*3.1415926/180));

	//���ƶ����
	POINT polygon[3]={{390,160},{430,220},{350,210}};
	POINT polyPolygon[6]={{450,120},{550,320},{480,280},
						{400,300},{550,320},{480,280}};
	int	polygonPoints[2]={3,3};
	pDC->Polygon(polygon,3);
	pDC->PolyPolygon(polyPolygon,polygonPoints,2);
}

void CTestView::OnWord(CDC* pDC) 
{
	// TODO: Add your command handler code here
	pDC->TextOut(10,10,"Ĭ�ϵ�����");

	pDC->SetTextColor(RGB(255,0,0));
	pDC->SetBkColor(RGB(0,0,255));
	pDC->SetTextCharacterExtra(4);
	pDC->TextOut(10,30,"����Ϊ��ɫ���ı�Ϊ��ɫ���ַ����Ϊ4");

	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(10,50,"����Ϊ͸�����ı�Ϊ��ɫ���ַ����Ϊ4");

	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkColor(RGB(255,255,255));

	CFont* pOldFont;
	CFont* pNewFont=new CFont;

	pNewFont->CreateFont(60,20,80,0,
						200,FALSE,FALSE,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH&FF_SWISS,"Aerial");
	pOldFont=(CFont*)pDC->SelectObject(pNewFont);
	pDC->TextOut(10,120,"OOP�������!");

	pDC->SelectObject(pOldFont);
	delete pNewFont;
}

void CTestView::OnPen(CDC* pDC) 
{
	// TODO: Add your command handler code here
	int nPenStyle[]=
	{
		PS_SOLID,
		PS_DASH,
		PS_DOT,
		PS_DASHDOT,
		PS_DASHDOTDOT,
		PS_NULL,
		PS_INSIDEFRAME,
	};
    CPen*	pNewPen;  
	CPen*	pOldPen; 

	//�ò�ͬ���ıʻ�ͼ
	for(int i=0;i<7;i++)
	{
		//�����±�
		pNewPen=new CPen;

	    if( pNewPen->CreatePen( nPenStyle[i], 2, RGB(0,0,0)))
		{
			pOldPen = pDC->SelectObject( pNewPen );

			//���´����ıʻ�ͼ
			pDC->MoveTo(20,60+i*20);
			pDC->LineTo(160,60+i*20);

			//�ָ��豸�������е�ԭ�еı�
			pDC->SelectObject( pOldPen );
		}
		else
		{
			//����������ʾ
			AfxMessageBox("CreatePen Error!");
	    }

		//ɾ���±�
		delete pNewPen;
	}

	//�ò�ͬ��ȵıʻ�ͼ
	for(i=0;i<7;i++)
	{
		//�����±�
		pNewPen=new CPen;

	    if( pNewPen->CreatePen( PS_SOLID, i+1, RGB(0,0,0)))
		{
			pOldPen = pDC->SelectObject( pNewPen );

			//���´����ıʻ�ͼ
			pDC->MoveTo(200,60+i*20);
			pDC->LineTo(340,60+i*20);

			//�ָ��豸�������е�ԭ�еı�
			pDC->SelectObject( pOldPen );
		}
		else
		{
			//����������ʾ
			AfxMessageBox("CreatePen Error!");
	    }

		//ɾ���±�
		delete pNewPen;
	}

	//������ɫ��
	struct	tagColor
	{
		int r,g,b;
	} color[7]=
	{
		{255,0,0},
		{0,255,0},
		{0,0,255},

		{255,255,0},
		{255,0,255},
		{0,255,255},

		{0,0,0},
	};

	//�ò�ͬ��ɫ�ıʻ�ͼ
	for(i=0;i<7;i++)
	{
		//�����±�
		pNewPen=new CPen;

	    if( pNewPen->CreatePen( PS_SOLID, 2, RGB(color[i].r,color[i].g,color[i].b) ) )
		{
			pOldPen = pDC->SelectObject( pNewPen );

			//���´����ıʻ�ͼ
			pDC->MoveTo(380,60+i*20);
			pDC->LineTo(520,60+i*20);

			//�ָ��豸�������е�ԭ�еı�
			pDC->SelectObject( pOldPen );
		}
		else
		{
			//����������ʾ
			AfxMessageBox("CreatePen Error!");
	    }

		//ɾ���±�
		delete pNewPen;
	}
}

void CTestView::OnBrush(CDC* pDC) 
{
	// TODO: Add your command handler code here
	CBrush*		pNewBrush;
	CBrush*		pOldBrush;

	//������ɫ��
	struct	tagColor
	{
		int r,g,b;
	} color[7]=
	{
		{255,0,0},
		{0,255,0},
		{0,0,255},

		{255,255,0},
		{255,0,255},
		{0,255,255},

		{0,0,0},
	};

	//ʹ�ò�ͬ��ɫ��ʵ�廭ˢ
	for(int i=0;i<7;i++)
	{
		//�����»�ˢ
		pNewBrush=new CBrush; 

		if( pNewBrush->CreateSolidBrush( RGB(color[i].r,color[i].g,color[i].b)))
		{
			//ѡ���»�ˢ
			pOldBrush = pDC->SelectObject( pNewBrush );

			//���ƾ���
			pDC->Rectangle(40,20+i*40,200,50+i*40);

			//�ָ��豸��������ԭ�еĻ�ˢ
			pDC->SelectObject( pOldBrush );
		}

		//ɾ���»�ˢ
		delete pNewBrush;
	}

	//ʵ�廭ˢ��ͼ������ֵ
	int	nBrushPattern[6]=
	{
		HS_BDIAGONAL,
		HS_CROSS,
		HS_DIAGCROSS,
		HS_FDIAGONAL,
		HS_HORIZONTAL,
		HS_VERTICAL,
	};

	//ʹ�ò�ͬͼ����ʵ�廭ˢ
	for(i=0;i<6;i++)
	{
		//�����»�ˢ
		pNewBrush=new CBrush; 

		if(pNewBrush->CreateHatchBrush(nBrushPattern[i],RGB(0,0,0)))
		{
			//ѡ���»�ˢ
			pOldBrush = pDC->SelectObject( pNewBrush );

			//���ƾ���
			pDC->Rectangle(240,20+i*40,400,50+i*40);

			//�ָ��豸��������ԭ�еĻ�ˢ
			pDC->SelectObject( pOldBrush );
		}

		//ɾ���»�ˢ
		delete pNewBrush;
	}
}
