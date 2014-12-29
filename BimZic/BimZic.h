
// BimZic.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// Globals
#define	APP_GUID					_T("MTX_Bimedia_BimZic_F5B808E9-873B-445B-AE37-F2D6128B30D4")
#define APP_CLSID					_T("CLS_Bimedia_BimZic_F5B808E9-873B-445B-AE37-F2D6128B30D4")
#define APP_WINDOWSTITLE			_T("BimZic")
#define SETTINGS_SECTION_COMMON		_T("COMMON")
#define DATA_SECTION_DATA			_T("RESULTS")
#define DATA_KEY_VOTES_1			_T("Piste1")
#define DATA_KEY_VOTES_2			_T("Piste2")
#define DATA_KEY_VOTES_3			_T("Piste3")
#define DATA_KEY_VOTES_4			_T("Piste4")
#define DATA_KEY_VOTES_5			_T("Piste5")
#define DATA_KEY_VOTES_6			_T("Piste6")

#define BUTTON_SIZE			200
#define WINDOW_WIDTH		720
#define WINDOW_HEIGHT		580


// CBimZicApp:
// See BimZic.cpp for the implementation of this class
//

class CBimZicApp : public CWinApp
{
public:
	CBimZicApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int	ExitInstance();

	DECLARE_MESSAGE_MAP()
	
public:
	// Graphic Device Interface +
	ULONG_PTR gdiplusToken;
	CDpi DpiMetrics;
	bool m_bIsRTL;

	CString _csExePath;

	// Buttons
	CString csButtonImage;
	CString csCloseButtonImage;

	// Background
	CString csBackground;

	// Text Color
	CString csDefaultColor;
	CString csErrorColor;

	// Image fullpath
	CString csDefaultImage;
	CString csImage1;
	CString csImage2;
	CString csImage3;
	CString csImage4;
	CString csImage5;
	CString csImage6;

	// Sound tracks fullpath
	CString csDefaultTrack;
	CString csTrack1;
	CString csTrack2;
	CString csTrack3;
	CString csTrack4;
	CString csTrack5;
	CString csTrack6;

	// Results data
	int Votes1;
	int Votes2;
	int Votes3;
	int Votes4;
	int Votes5;
	int Votes6;

	// Path management
	CString GetExePath(void);
	CString GetEnvPath(void);
	bool LoadApplicationSettings(void);
	bool LoadData(void);
	bool SaveData(void);
private:
	// Single Process
	HANDLE hMutexOneInstance;
};

extern CBimZicApp theApp;