#pragma once

//#include <gdiplusheaders.h>
#include "CGdiPlusImage.h"


// CButtonEx


class CButtonEx : public CButton
{
	DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx();
	virtual ~CButtonEx();
	//manage owner draw button
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow(void);
	//set button parameters
	void SetButton(CString csCaption, CString csColor, bool isRTL, CString pathnormal, CString pathClicked, CString pathglyph=_T(""), bool hasglyph=false);
	//load images in button
	bool LoadImages();
	bool LoadImage(const CString csPath, CGdiPlusImage * * pImage);
	//
	void SetOn();
	void SetOff();
	
	//images
	CGdiPlusImage *m_pImage;
	CGdiPlusImage *m_pHotImage;
	CGdiPlusImage *m_pGlyph;
	CString m_pathnormal;
	CString m_pathClicked;
	CString m_pathglyph;
	//font
	Gdiplus::Font *m_pFont;
	//colormatrix for alpha
	ColorMatrix m_GlowColor;
	//brush color
	SolidBrush  *drawBrush;
	//string format
	StringFormat drawFormat;
    //is RTL text ?
	bool m_isRTL;
	//text button
	CString m_csCaption;
	//text color
	CString m_csColor;
    //bitmap for transparent background
	Bitmap *pBackground;	
	//get tracking of mouse position
	BOOL m_bTracking;
	//has glyph in button ?
    bool m_hasglyph;
	//button size ( fixed or calculated)
	int m_iWidth;
	int m_iHeight;
	//text size
	int m_iTWidth;
	int m_iTHeight;

	//
	bool _isOn;			// Dans le cas du mode switch donne l'etat du bouton (enfoncé = on, relacher = off)

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


