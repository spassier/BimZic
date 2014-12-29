
// BimZicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BimZic.h"
#include "BimZicDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CBimZicDlg dialog




CBimZicDlg::CBimZicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBimZicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	_isSelected = -1;
	_lastClickedButton = NULL;

	m_pBackground = NULL;	
	m_pDoubleBuffer = NULL;
}

CBimZicDlg::~CBimZicDlg()
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pDoubleBuffer);
}

void CBimZicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, _buttonMusic1);
	DDX_Control(pDX, IDC_BUTTON2, _buttonMusic2);
	DDX_Control(pDX, IDC_BUTTON3, _buttonMusic3);
	DDX_Control(pDX, IDC_BUTTON4, _buttonMusic4);
	DDX_Control(pDX, IDC_BUTTON5, _buttonMusic5);
	DDX_Control(pDX, IDC_BUTTON6, _buttonMusic6);
	DDX_Control(pDX, IDC_BUTTON7, _Validate);
	DDX_Control(pDX, IDC_LABEL_1, _text1);
	DDX_Control(pDX, IDC_LABEL_2, _text2);
	DDX_Control(pDX, IDC_LABEL_3, _text3);
	DDX_Control(pDX, IDC_LABEL_4, _text4);
	DDX_Control(pDX, IDC_LABEL_5, _text5);
	DDX_Control(pDX, IDC_LABEL_6, _text6);

	DDX_Control(pDX, IDC_BUTTON8, _redCross);
}

BEGIN_MESSAGE_MAP(CBimZicDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CBimZicDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBimZicDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBimZicDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CBimZicDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBimZicDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CBimZicDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CBimZicDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CBimZicDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CBimZicDlg message handlers

BOOL CBimZicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set Window name
	SetWindowText(_T("BimZic"));

	// Adjust main window size and position
	SetWindowFromClientArea(GetSystemMetrics(SM_CXSCREEN),  GetSystemMetrics(SM_CYSCREEN)/*theApp.DpiMetrics.ScaleX(WINDOW_WIDTH), theApp.DpiMetrics.ScaleY(WINDOW_HEIGHT)*/, true);	

	// Compute form factor
	InitFormFactor();

	// Load UI images
	if (m_pBackground != NULL)
		SAFE_DELETE(m_pBackground);
	
	m_pBackground = new CGdiPlusImage;
	
	if (!m_pBackground->Load(theApp.csBackground))
		AfxMessageBox(_T("ERROR : background loading failure"));

	// Create the Double Buffer
	if (m_pDoubleBuffer != NULL)
		SAFE_DELETE(m_pDoubleBuffer);
	
	m_pDoubleBuffer = new Bitmap(GetSystemMetrics(SM_CXSCREEN),  GetSystemMetrics(SM_CYSCREEN));//(theApp.DpiMetrics.ScaleX(WINDOW_WIDTH), theApp.DpiMetrics.ScaleX(WINDOW_HEIGHT));

	// Window is fullscreen
	MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
  	
	// Window is TopMoast (must be force in spite of the topmost initialization perhasp because of the movewindow call)
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	// Set controls
	_buttonMusic1.MoveWindow(theApp.DpiMetrics.ScaleX(_hSizeMarging/*30*/), theApp.DpiMetrics.ScaleY(_vSizeMarging/*30*/), theApp.DpiMetrics.ScaleX(_imageSize/*BUTTON_SIZE*/), theApp.DpiMetrics.ScaleY(_imageSize/*BUTTON_SIZE*/), TRUE);
	_buttonMusic1.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage1, theApp.csImage1, _T("") , false);

	_buttonMusic2.MoveWindow(theApp.DpiMetrics.ScaleX(2 * _hSizeMarging + 1 * _imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_imageSize), TRUE);
	_buttonMusic2.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage2, theApp.csImage2, _T("") , false);

	_buttonMusic3.MoveWindow(theApp.DpiMetrics.ScaleX(3 * _hSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_imageSize), TRUE);
	_buttonMusic3.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage3, theApp.csImage3, _T("") , false);

	_buttonMusic4.MoveWindow(theApp.DpiMetrics.ScaleX(_hSizeMarging), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize/*BUTTON_SIZE*/), theApp.DpiMetrics.ScaleY(_imageSize/*BUTTON_SIZE*/), TRUE);
	_buttonMusic4.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage4, theApp.csImage4, _T("") , false);

	_buttonMusic5.MoveWindow(theApp.DpiMetrics.ScaleX(2 * _hSizeMarging + 1 * _imageSize), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_imageSize), TRUE);
	_buttonMusic5.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage5, theApp.csImage5, _T("") , false);

	_buttonMusic6.MoveWindow(theApp.DpiMetrics.ScaleX(3 * _hSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_imageSize), TRUE);
	_buttonMusic6.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csImage6, theApp.csImage6, _T("") , false);

	// Initialize Validate ² red cross buttons
	_redCross.MoveWindow(theApp.DpiMetrics.ScaleX(GetSystemMetrics(SM_CXSCREEN) - _vSizeButton), theApp.DpiMetrics.ScaleY(0), theApp.DpiMetrics.ScaleX(_vSizeButton), theApp.DpiMetrics.ScaleY(_vSizeButton), TRUE);
	_redCross.SetButton(_T(""),	theApp.csDefaultColor, theApp.m_bIsRTL, theApp.csCloseButtonImage, theApp.csCloseButtonImage, _T("") , true);
	//_redCross.SetFocus();

	_Validate.MoveWindow(theApp.DpiMetrics.ScaleX(2 * _hSizeMarging + _imageSize + ((_imageSize - (_vSizeButton * 4)) / 2)), theApp.DpiMetrics.ScaleY((int)(_vSizeButton * 0.10f) + 3 * _vSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleX(_vSizeButton * 4), theApp.DpiMetrics.ScaleY(_vSizeButton), TRUE);
	_Validate.SetButton(_T("Valider"), _T("16777215"), theApp.m_bIsRTL, theApp.csButtonImage, theApp.csButtonImage, _T("") , true);
	//_Validate.SetFocus();

	// Initialize Static controls
	_text1.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">01 - Capital Cities</font></b>"));
	_text1.MoveWindow(theApp.DpiMetrics.ScaleX(_hSizeMarging), theApp.DpiMetrics.ScaleY(_vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	_text2.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">02 - Cocoon Comets</font></b>"));
	_text2.MoveWindow(theApp.DpiMetrics.ScaleX(2 * _hSizeMarging + _imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	_text3.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">03 - Cocoon On My Way</font></b>"));
	_text3.MoveWindow(theApp.DpiMetrics.ScaleX(3 * _hSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging + _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	_text4.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">04 - Kiss Drive</font></b>"));
	_text4.MoveWindow(theApp.DpiMetrics.ScaleX(_hSizeMarging), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	_text5.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">05 - Moby</font></b>"));
	_text5.MoveWindow(theApp.DpiMetrics.ScaleX(2 * _hSizeMarging + _imageSize), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	_text6.SetWindowText(_T("<b><FONT face=\"Stencil\" size=\"12\" color=\"#FFFFFF\">06 - Otis Redding</font></b>"));
	_text6.MoveWindow(theApp.DpiMetrics.ScaleX(3 * _hSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleY(2 * _vSizeMarging + 2 * _imageSize), theApp.DpiMetrics.ScaleX(_imageSize), theApp.DpiMetrics.ScaleY(_vSizeMarging));

	//ShowWindow(SW_SHOW);

	return FALSE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBimZicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC PaintDC(this); // device context for painting
		// Set graphic output to doublebuffer
		Graphics db(((CBimZicDlg *)(theApp.m_pMainWnd))->m_pDoubleBuffer);   
		// Draw background
		if (m_pBackground != NULL)
			db.DrawImage(*m_pBackground, 0, 0, GetSystemMetrics(SM_CXSCREEN),  GetSystemMetrics(SM_CYSCREEN));//theApp.DpiMetrics.ScaleX(WINDOW_WIDTH), theApp.DpiMetrics.ScaleY(WINDOW_HEIGHT));

		// Set output to DC
		Graphics dc(PaintDC);

		_text1.Reset(); //redraw background of control
		_text2.Reset();
		_text3.Reset();
		_text4.Reset();
		_text5.Reset();
		_text6.Reset();

		// Draw doublebuffer
		dc.DrawImage(((CBimZicDlg *)(theApp.m_pMainWnd))->m_pDoubleBuffer, 0, 0);

		CDialogEx::OnPaint();
		
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBimZicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CBimZicDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;//CDialogEx::OnEraseBkgnd(pDC);
}

void CBimZicDlg::OnBnClickedButton1()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	// Si le precedent bouton selectionné est le meme, alors on le reset et on quit
	if ( _lastClickedButton == &_buttonMusic1)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic1.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic1 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack1, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 1;
		_buttonMusic1.SetOn();
		_lastClickedButton = &_buttonMusic1;
	}
}

void CBimZicDlg::OnBnClickedButton2()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	// Si le precedent bouton selectionné est le meme, alors on le reset et on quit
	if ( _lastClickedButton == &_buttonMusic2)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic2.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic2 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack2, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 2;
		_buttonMusic2.SetOn();
		_lastClickedButton = &_buttonMusic2;
	}
}

void CBimZicDlg::OnBnClickedButton3()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	if ( _lastClickedButton == &_buttonMusic3)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic3.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic3 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack3, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 3;
		_buttonMusic3.SetOn();
		_lastClickedButton = &_buttonMusic3;
	}
}

void CBimZicDlg::OnBnClickedButton4()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	if ( _lastClickedButton == &_buttonMusic4)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic4.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic4 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack4, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 4;
		_buttonMusic4.SetOn();
		_lastClickedButton = &_buttonMusic4;
	}
}

void CBimZicDlg::OnBnClickedButton5()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	if ( _lastClickedButton == &_buttonMusic5)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic5.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic5 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack5, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 5;
		_buttonMusic5.SetOn();
		_lastClickedButton = &_buttonMusic5;
	}
}

void CBimZicDlg::OnBnClickedButton6()
{
		//UpdateData(true);

	// TODO: Add your control notification handler code here
	if ( _lastClickedButton == &_buttonMusic6)
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_buttonMusic6.SetOff();
		_lastClickedButton = NULL;
		
		return;
	}
	
	// Si le bouton precedent est different du bouton courant
	if ( _lastClickedButton != &_buttonMusic6 )
	{
		// Et si le precedent pas null
		if ( _lastClickedButton != NULL)
		{
			_lastClickedButton->SetOff();
		}
		
		PlaySound(theApp.csTrack6, NULL, SND_FILENAME | SND_ASYNC);
		_isSelected = 6;
		_buttonMusic6.SetOn();
		_lastClickedButton = &_buttonMusic6;
	}
}


void CBimZicDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
/*
void CBimZicDlg::OnOK()
{
	// ENTER key pressed = "Next" button clicked
	//if (m_BtnNext.GetFocus() == GetDlgItem(IDC_POPUP_WELCOME_BUTTON_NEXT))
		//OnBnClickedNext();

	CDialogEx::OnOK();
}

void CBimZicDlg::OnCancel()
{
	// ESCAPE key pressed = Red cross clicked
	AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);

//	CDialog::OnCancel();
}
*/
void CBimZicDlg::SetWindowFromClientArea(int iClientWidth, int iClientHeight, bool bCentered)
{
	int iX, iY, iWidth, iHeight;

	// Window width = 2 * (horizontal size of edge + border) + client width
	iWidth = (GetSystemMetrics(SM_CXEDGE) + GetSystemMetrics(SM_CXBORDER)) * 2 + iClientWidth;
	// Window height = 2 * (vertical size of edge + border) + size of caption + client height
	iHeight = (GetSystemMetrics(SM_CYEDGE) + GetSystemMetrics(SM_CYBORDER)) * 2 + GetSystemMetrics(SM_CYCAPTION) + iClientHeight;
	
	if (bCentered)
	{
		iX = (GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2;
		iY = (GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2;
	}
	else
	{
		iX = 0;
		iY = 0;
	}

	CWnd::MoveWindow(iX, iY, iWidth, iHeight, TRUE);
}

/* Sinon en cas de retour d'un etat minimized les composant qui ont le focus ne sont pas redessinés.. */
void CBimZicDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	/*
	if ( _buttonMusic1.GetState() & 0x0008)
		_buttonMusic1.RedrawWindow();

	if ( _buttonMusic2.GetState() & 0x0008)
		_buttonMusic2.RedrawWindow();

	if ( _buttonMusic3.GetState() & 0x0008)
	_buttonMusic3.RedrawWindow();
	
	if ( _buttonMusic4.GetState() & 0x0008)
		_buttonMusic4.RedrawWindow();

	if ( _buttonMusic5.GetState() & 0x0008)
		_buttonMusic5.RedrawWindow();
	
	if ( _buttonMusic6.GetState() & 0x0008)
		_buttonMusic6.RedrawWindow();

	if ( _Validate.GetState() & 0x0008)
		_Validate.RedrawWindow();
	
	_text1.RedrawWindow();
	_text2.RedrawWindow();
	_text3.RedrawWindow();
	_text4.RedrawWindow();
	_text5.RedrawWindow();
	_text6.RedrawWindow();
	*/

	RedrawWindow();
	//Invalidate();
}

void CBimZicDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	switch (_isSelected)
	{
		case 1:
			theApp.Votes1++;
			break;
		case 2:
			theApp.Votes2++;
			break;
		case 3:
			theApp.Votes3++;
			break;
		case 4:
			theApp.Votes4++;
			break;
		case 5:
			theApp.Votes5++;
			break;
		case 6:
			theApp.Votes6++;
			break;
		case -1:
			break;
		default:
			AfxMessageBox(_T("ERROR : Vote invalid"));
			break;
	}

	// backup a chaque fois au cas ou...
	theApp.SaveData();

	// Reset last button selected
	if ( _lastClickedButton )
	{
		PlaySound(NULL, 0, 0);
		_isSelected = -1;
		_lastClickedButton->SetOff();
		_lastClickedButton = NULL;
	}
}


void CBimZicDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CBimZicDlg::InitFormFactor()
{
	int Height = GetSystemMetrics(SM_CYSCREEN);
	int Width = GetSystemMetrics(SM_CXSCREEN);

	_imageSize = (int)(Height * 0.36f); // Taille des buttons = 36% de la hauteur
	_vSizeMarging = (int)(Height * 0.05f); // Taille de la marge vertical = 5% de la hauteur
	_vSizeButton = (int)(Height * 0.07f); // Taille veretical des bouton validate et redcross = 7%

	_hSizeMarging = (Width - (_imageSize * 3)) / 4;
}