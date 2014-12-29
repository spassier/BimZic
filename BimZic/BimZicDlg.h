
// BimZicDlg.h : header file
//

#pragma once


#include "CButtonEx.h"
#include "afxwin.h"
#include "XHTMLStatic.h"


// CBimZicDlg dialog
class CBimZicDlg : public CDialogEx
{
// Construction
public:
	CBimZicDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CBimZicDlg(); // Manually added

// Dialog Data
	enum { IDD = IDD_BIMZIC_DIALOG };

// Implementation
protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//
	int _imageSize;
	int _vSizeButton;
	int _vSizeMarging;
	int _hSizeMarging;
	
	// Control variable
	CXHTMLStatic _text1;
	CXHTMLStatic _text2;
	CXHTMLStatic _text3;
	CXHTMLStatic _text4;
	CXHTMLStatic _text5;
	CXHTMLStatic _text6;

	// Controls
	CButtonEx _buttonMusic1;
	CButtonEx _buttonMusic2;
	CButtonEx _buttonMusic3;
	CButtonEx _buttonMusic4;
	CButtonEx _buttonMusic5;
	CButtonEx _buttonMusic6;
	CButtonEx _redCross;
	CButtonEx _Validate;
	CButtonEx *_lastClickedButton;

	CGdiPlusImage *m_pBackground;	
	Bitmap *m_pDoubleBuffer;

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();	

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

	int _isSelected;

protected:
	//virtual void OnOK();
	//virtual void OnCancel();
private:
	void SetWindowFromClientArea(int iClientWidth, int iClientHeight, bool bCentered);
	void InitFormFactor();
};
