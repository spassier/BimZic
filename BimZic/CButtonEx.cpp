// CButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "CButtonEx.h"
#include "BimZic.h"
#include "BimZicDlg.h"

// CButtonEx
#define BUTTONVMARGIN 0
#define BUTTONHMARGIN 2


IMPLEMENT_DYNAMIC(CButtonEx, CButton)


CButtonEx::CButtonEx()
{
	m_pImage = NULL;
	m_pHotImage = NULL;
	m_pGlyph = NULL;
	m_bTracking = FALSE;
	m_hasglyph=false;
	m_pFont = new Gdiplus::Font(_T("Arial"), theApp.DpiMetrics.PointsToPixels(20), 1);
	//drawFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	drawBrush = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iTWidth = 0;
	m_iTHeight = 0;
	pBackground = NULL;

	_isOn = false;
}

CButtonEx::~CButtonEx()
{
	//clean allocated resources
	SAFE_DELETE(m_pImage);
	SAFE_DELETE(m_pHotImage);
	SAFE_DELETE(m_pGlyph);
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(drawBrush);
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CButtonEx message handlers
void CButtonEx::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
}

BOOL CButtonEx::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

//
void CButtonEx::SetOn()
{
	_isOn = true;
}

void CButtonEx::SetOff()
{
	_isOn = false;
}

//load all necessary images
bool CButtonEx::LoadImages()
{   
	/*
	if (m_hasglyph)
		return LoadImage(m_pathnormal, &m_pImage) && LoadImage(m_pathClicked, &m_pHotImage) && LoadImage(m_pathglyph, &m_pGlyph);
	else*/
		return LoadImage(m_pathnormal, &m_pImage) && LoadImage(m_pathClicked, &m_pHotImage);
}

//load one image
bool CButtonEx::LoadImage(const CString csPath, CGdiPlusImage * *pImage)
{
	bool bResult = false;

	// delete if not null
	if (*pImage != NULL) SAFE_DELETE(*pImage);
		*pImage = new CGdiPlusImage;

	//load image
	if ((*pImage) != NULL)
	{
		if ((*pImage)->Load(csPath))
		{
			bResult = true;
		}
		else
			SAFE_DELETE(*pImage);
	}
	return bResult;
}


void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ColorMatrix ClrMatrix70b = { 0.5f,0.0f,0.0f,0.0f,0.0f,
								0.0f,0.5f,0.0f,0.0f,0.0f,
								0.0f,0.0f,0.5f,0.0f,0.0f,
								0.0f,0.0f,0.0f,0.7f,0.0f,
								0.0f,0.0f,0.0f,0.0f,1.0f };
	
	Graphics DC(lpDrawItemStruct->hDC);	
	CPaintDC PaintDC(this);

	CRect ClientRect;
	GetWindowRect(ClientRect);

	//GetParent()->ScreenToClient(ClientRect);
	m_iWidth = ClientRect.Width();
	m_iHeight = ClientRect.Height();
	Bitmap bmpBuffer(ClientRect.Width(), ClientRect.Height());

	// Draw in the bitmap
	// Draw the different states of button

	Graphics g(&bmpBuffer);

	//need some alpha level 
	ImageAttributes ImgAttralpha70b;
	ImgAttralpha70b.SetColorMatrix(&ClrMatrix70b);
	
	//to avoid stretching with unwished effect
	g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
	g.SetPixelOffsetMode(PixelOffsetModeHalf);
	g.SetSmoothingMode(Gdiplus::SmoothingModeNone);
	
	// destRect la destination de la copie c est a dire la taille du bitmap du bouton
	RectF destrect (0, 0, (Gdiplus::REAL)m_iWidth, (Gdiplus::REAL)m_iHeight);

	// copie le fond couvert par le bouton depuis le background dans le bitmap
	CRect ScrRect;
	GetWindowRect(ScrRect);
	GetParent()->ScreenToClient(ScrRect);
	
	// Give to the button the pointer of the double buffer
	pBackground = ((CBimZicDlg *)(theApp.m_pMainWnd))->m_pDoubleBuffer;
	if(pBackground != NULL )
		g.DrawImage(pBackground, destrect, (Gdiplus::REAL)ScrRect.left, (Gdiplus::REAL)ScrRect.top, (Gdiplus::REAL)m_iWidth, (Gdiplus::REAL)m_iHeight, UnitPixel);

	//normal state we apply som alpha to make it more transparent
	Rect destRect( 0, 0, m_iWidth, m_iHeight);
	
	//<TODO> calculation done by hand
	Rect focusRect(1, 1, m_iWidth - 2, m_iHeight - 2);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		// Lorsque le bouton est appuyer on change l'image (nous on s'en fout pn remet la meme)
		if (m_pHotImage != NULL)
		{
			g.DrawImage(*m_pHotImage, 0, 0, m_iWidth, m_iHeight);
			g.DrawImage(*m_pHotImage, destRect, 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel, &ImgAttralpha70b);	
			
			//add dash edge
			if(m_hasglyph) // Cas du bouton valider
			{
				Pen pen(Color::Black);
				pen.SetDashStyle(Gdiplus::DashStyleDot);
				g.DrawRectangle(&pen,focusRect);
			}
			else
			{/*

				if ( _isOn )
				{
					Pen pen(Color::Red);
					pen.SetWidth(4);
					pen.SetDashStyle(Gdiplus::DashStyleSolid);
					g.DrawRectangle(&pen,focusRect);
				}
				*/
			}
			
		}
	}
	else if (m_bTracking) // Quand la souris est dessus (avec ou sans appui souris a l'exterieur)
	{
		if (m_pImage != NULL)
		{
			// Dans ce cas on affiche l'image + un ombrage
			g.DrawImage(*m_pImage, 0, 0, m_iWidth, m_iHeight);
			g.DrawImage(*m_pImage, destRect, 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel, &ImgAttralpha70b);	
			
			// Si le control a le focus
			if (lpDrawItemStruct->itemState & ODS_FOCUS)
			{
		    	
				//add dash edge
				if(m_hasglyph) // Cas du bouton valider
				{
					Pen pen(Color::Black);
					pen.SetDashStyle(Gdiplus::DashStyleDot);
					g.DrawRectangle(&pen,focusRect);
				}
				else
				{
					/*
					if ( _isOn )
					{
					Pen pen(Color::Red);
					pen.SetWidth(4);
					pen.SetDashStyle(Gdiplus::DashStyleSolid);
					g.DrawRectangle(&pen,focusRect);
					}
					*/
				}
				
			}
		}
	}
	else if (lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		if (m_pImage != NULL)
			g.DrawImage(*m_pImage, 0, 0, m_iWidth, m_iHeight);
		
		//add dash edge
		if(m_hasglyph)
		{
			Pen pen(Color::Black);
			pen.SetDashStyle(Gdiplus::DashStyleDot);
			g.DrawRectangle(&pen,focusRect);
		}
		/*
		else
		{
			Pen pen(Color::Red);
			pen.SetWidth(4);
			pen.SetDashStyle(Gdiplus::DashStyleSolid);
			g.DrawRectangle(&pen,focusRect);
		}*/
		
	}
	else
	{
		// Normal state, no mouse no click
		if (m_pImage != NULL)
			g.DrawImage(*m_pImage, 0, 0, m_iWidth, m_iHeight);
	}

	if ( _isOn && (this->GetState() & 0x0008) )
	{
		Pen pen(Color::Red);
		pen.SetWidth(m_iWidth * 0.04);
		pen.SetDashStyle(Gdiplus::DashStyleSolid);
		g.DrawRectangle(&pen,focusRect);
	}

	// if glyph then do text calculation to take in account glyph to set text position in no glyph then just center text
/*	if (m_hasglyph)
	{
		if (!theApp.m_bIsRTL)
		{
			RectF boundRecttext(BUTTONHMARGIN + (Gdiplus::REAL)(m_iWidth - (m_iTWidth + theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()))) / 2,
													BUTTONVMARGIN + (Gdiplus::REAL)(m_iHeight - m_iTHeight) / 2,
													(Gdiplus::REAL)m_iTWidth,
													(Gdiplus::REAL)m_iTHeight);
			g.DrawString(m_csCaption, -1, m_pFont, boundRecttext, &drawFormat, drawBrush);

			RectF boundRectglyph(BUTTONHMARGIN + m_iTWidth + (Gdiplus::REAL)(m_iWidth - (m_iTWidth + theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()))) / 2,
													BUTTONVMARGIN + (Gdiplus::REAL)(m_iHeight - theApp.DpiMetrics.ScaleY(m_pGlyph->GetHeight())) / 2,
													(Gdiplus::REAL)theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()),
													(Gdiplus::REAL)theApp.DpiMetrics.ScaleY(m_pGlyph->GetHeight()));
			g.DrawImage(*m_pGlyph, boundRectglyph);
		}
		else
		{
			RectF boundRecttext(BUTTONHMARGIN + theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()) + (Gdiplus::REAL)(m_iWidth - (m_iTWidth + theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()))) / 2,
													BUTTONVMARGIN + (Gdiplus::REAL)(m_iHeight - m_iTHeight) / 2,
													(Gdiplus::REAL)m_iTWidth,
													(Gdiplus::REAL)m_iTHeight);
			g.DrawString(m_csCaption, -1, m_pFont, boundRecttext, &drawFormat, drawBrush);

			// Add the width of the Glyph because in RTL the glyph is mirrored and draw to the left of the top-left corner
			RectF boundRectglyph(BUTTONHMARGIN + (Gdiplus::REAL)theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()) + (Gdiplus::REAL)(m_iWidth - (m_iTWidth + theApp.DpiMetrics.ScaleX(m_pGlyph->GetWidth()))) / 2,
													BUTTONVMARGIN + (Gdiplus::REAL)(m_iHeight - theApp.DpiMetrics.ScaleY(m_pGlyph->GetHeight())) / 2,
													(Gdiplus::REAL)theApp.DpiMetrics.ScaleX(-m_pGlyph->GetWidth()),
													(Gdiplus::REAL)theApp.DpiMetrics.ScaleY(m_pGlyph->GetHeight()));
			g.DrawImage(*m_pGlyph, boundRectglyph);
		}
	}
	else*/
	{
		//draw text button
		RectF boundRecttext((Gdiplus::REAL)(m_iWidth - m_iTWidth) / 2,
												BUTTONVMARGIN + (Gdiplus::REAL)(m_iHeight - m_iTHeight) / 2,
												(Gdiplus::REAL)m_iTWidth,
												(Gdiplus::REAL)m_iTHeight);
		g.DrawString(m_csCaption, -1, m_pFont, boundRecttext, &drawFormat, drawBrush);
	}	

	// Draw result to DC
	DC.DrawImage(&bmpBuffer, 0, 0);
}


void CButtonEx::SetButton(CString csCaption, CString csColor, bool isRTL, CString pathnormal,CString pathClicked, CString pathglyph, bool hasglyph)
{   
	// if owner draw button 
	if(GetSafeHwnd())
		ModifyStyle(0, BS_OWNERDRAW, 0);

	// By default buttons are not selected
	_isOn = false;

	// Set RTL flag & change Format
	m_isRTL = isRTL;
	if (m_isRTL)
	{
		drawFormat.SetFormatFlags(StringFormatFlagsDirectionRightToLeft);
	}
	else
	{
		drawFormat.SetFormatFlags(0);
	}

	// Button text
	m_csCaption = csCaption;
	
	// Text color
	m_csColor = csColor;
	
	//TODO : use color value
  if (drawBrush != NULL)
		SAFE_DELETE(drawBrush);
	
	Color c;
	c.SetFromCOLORREF(_ttol(m_csColor.GetString()));
	drawBrush = new SolidBrush(c);

	// Has a glyph ? ici est remplacer par est ce qu'il a une bordure lors de la selection ?
	m_hasglyph = hasglyph;

	// Bitmap path
	m_pathnormal = pathnormal;
	m_pathClicked = pathClicked;
	m_pathglyph = pathglyph;
    
	// Load button images
	LoadImages();
	
	// Need to know the limit for text, say we have big limit...
	RectF layoutRect(0, 0, 1024, 1024);
	// For calculation
	RectF boundRecttext;
	// Get dc to reference
	CPaintDC PaintDC(this);
	// Create Graphic
	Graphics Grafx(PaintDC); 
	// Get string area based on font and calculation
	Grafx.MeasureString(m_csCaption, -1, m_pFont, layoutRect, &drawFormat, &boundRecttext);
	// Set Height and widht for button text
	m_iTWidth = (int)boundRecttext.Width + 1;
	m_iTHeight = (int)boundRecttext.Height + 1;
	
	Invalidate();
}


void CButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// To track the onOver
	CButton::OnMouseMove(nFlags, point);
	// if not tracking we track
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		Invalidate(FALSE);
	}
}

void CButtonEx::OnMouseLeave()
{
	CButton::OnMouseLeave();
	// Stop tracking
	m_bTracking = FALSE;
	Invalidate(FALSE);
}

