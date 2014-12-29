
// BimZic.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BimZic.h"
#include "BimZicDlg.h"
#include "CIniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBimZicApp

BEGIN_MESSAGE_MAP(CBimZicApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBimZicApp construction

CBimZicApp::CBimZicApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	// Single Process
	hMutexOneInstance = NULL;

	m_bIsRTL = false;
}


// The one and only CBimZicApp object

CBimZicApp theApp;


// CBimZicApp initialization

BOOL CBimZicApp::InitInstance()
{
	// Register class
	WNDCLASS wcWndClass;
	GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wcWndClass);   
	wcWndClass.lpszClassName = APP_CLSID; // Change the name of the class. Don't forget to change the Class name property in the .RC
	AfxRegisterClass(&wcWndClass);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(APP_WINDOWSTITLE);

	// Check if application if already running
	// ---------------------------------------
	hMutexOneInstance = CreateMutex(NULL, FALSE, APP_GUID);

	bool bAlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED);

	if (bAlreadyRunning)
	{
		hMutexOneInstance = NULL;

		HWND hPrevInstance = FindWindow(APP_CLSID, NULL);
		if (hPrevInstance != NULL)
		{
			if (IsIconic(hPrevInstance))
				ShowWindowAsync(hPrevInstance, SW_RESTORE);

			SetWindowPos(hPrevInstance, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetWindowPos(hPrevInstance, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		else
		{
			hPrevInstance = FindWindow(NULL, APP_WINDOWSTITLE);
			if (hPrevInstance != NULL)
			{
				SetWindowPos(hPrevInstance, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				SetWindowPos(hPrevInstance, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}

		return FALSE;
	}


	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	Status statusGdiP = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Load application settings
	// -------------------------
	//Log.Write(_T("> Load application settings"));
	if ( !LoadApplicationSettings() )
	{
		AfxMessageBox(_T("Failed to load application settings"), MB_OK | MB_ICONSTOP, 0);
		return FALSE;
	}

	// Load application data
	// -------------------------
	LoadData();

	// Create the Dialog
	// ------------------
	CBimZicDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		//ExitInstance();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
		//ExitInstance();
	}

	// Delete the shell manager created above.
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CBimZicApp::ExitInstance()
{
	SaveData();

	// Shutdown GDI+
	GdiplusShutdown(gdiplusToken);

	// Release Mutex
	if (hMutexOneInstance != NULL)
		ReleaseMutex(hMutexOneInstance);

	return CWinApp::ExitInstance();
}

CString CBimZicApp::GetExePath(void)
{
	TCHAR tzPath[MAX_PATH];

	if (_csExePath.IsEmpty())
	{
		ZeroMemory(tzPath, sizeof(tzPath));

		GetModuleFileName(NULL, tzPath, MAX_PATH);
		_csExePath = tzPath;

		// Remove the .exe name and let the '\' character at the end of the path
		_csExePath = _csExePath.Mid(0, _csExePath.ReverseFind(_T('\\')) + 1);
	}

	return _csExePath;
}

CString CBimZicApp::GetEnvPath(void)
{
	CString csEnvPath;

	csEnvPath = GetExePath() + _T("Data") + _T("\\");

	return csEnvPath;
}

bool CBimZicApp::LoadApplicationSettings(void)
{
	bool bResult = true;
	CString csFullPath;
	
	// ----------------------------
	// Get the Application Settings
	// ----------------------------

	cIniManip SettingsFile(GetEnvPath() + _T("BimZic.dat"));

	if ( SettingsFile.GetSections())
	{
		// FIXME : check section validity ?
		
		CString csKey;
		CString csValue;
		
		// Buttons images
		// ----------------
		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Button"), csButtonImage))
		{
			csFullPath = GetEnvPath() + csButtonImage;
			if (PathFileExists(csFullPath))
				csButtonImage = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csButtonImage.MakeUpper()));
				csButtonImage.Empty();
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Button\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("RedCross"), csCloseButtonImage))
		{
			csFullPath = GetEnvPath() + csCloseButtonImage;
			if (PathFileExists(csFullPath))
				csCloseButtonImage = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csCloseButtonImage.MakeUpper()));
				csCloseButtonImage.Empty();
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"RedCross\" not set"));

		// Background images
		// ----------------
		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("BackgroundImage"), csBackground))
		{
			csFullPath = GetEnvPath() + csBackground;
			if (PathFileExists(csFullPath))
				csBackground = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csBackground.MakeUpper()));
				csBackground.Empty();
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"BackgroundImage\" not set"));

		// Text color
		// ----------
		csValue.Empty();			
		if (!SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("DefaultColor"), csValue))
			AfxMessageBox((_T("....WARNING : \"%s\" not set"), _T("DefaultColor")));
		csDefaultColor = csValue;

		csValue.Empty();			
		if (!SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("ErrorColor"), csValue))
			AfxMessageBox((_T("....WARNING : \"%s\" not set"), _T("ErrorColor")));
		csErrorColor = csValue;

		// Button images
		// ----------------
		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("DefaultImage"), csDefaultImage))
		{
			csFullPath = GetEnvPath() + csDefaultImage;
			if (PathFileExists(csFullPath))
				csDefaultImage = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csDefaultImage.MakeUpper()));
				csDefaultImage.Empty();
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"DefaultImage\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image1"), csImage1))
		{
			csFullPath = GetEnvPath() + csImage1;
			if (PathFileExists(csFullPath))
				csImage1 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage1.MakeUpper()));
				csImage1 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image1\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image2"), csImage2))
		{
			csFullPath = GetEnvPath() + csImage2;
			if (PathFileExists(csFullPath))
				csImage2 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage2.MakeUpper()));
				csImage2 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image2\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image3"), csImage3))
		{
			csFullPath = GetEnvPath() + csImage3;
			if (PathFileExists(csFullPath))
				csImage3 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage3.MakeUpper()));
				csImage3 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image3\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image4"), csImage4))
		{
			csFullPath = GetEnvPath() + csImage4;
			if (PathFileExists(csFullPath))
				csImage4 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage4.MakeUpper()));
				csImage4 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image4\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image5"), csImage5))
		{
			csFullPath = GetEnvPath() + csImage5;
			if (PathFileExists(csFullPath))
				csImage5 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage5.MakeUpper()));
				csImage5 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image5\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Image6"), csImage6))
		{
			csFullPath = GetEnvPath() + csImage6;
			if (PathFileExists(csFullPath))
				csImage6 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csImage6.MakeUpper()));
				csImage6 = csDefaultImage;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Image6\" not set"));

		// Sound tracks
		// ----------------
		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("DefaultTrack"), csDefaultTrack))
		{
			csFullPath = GetEnvPath() + csDefaultTrack;
			if (PathFileExists(csFullPath))
				csDefaultTrack = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csDefaultTrack.MakeUpper()));
				csDefaultTrack.Empty();
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"DefaultTrack\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track1"), csTrack1))
		{
			csFullPath = GetEnvPath() + csTrack1;
			if (PathFileExists(csFullPath))
				csTrack1 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack1.MakeUpper()));
				csTrack1 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track1\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track2"), csTrack2))
		{
			csFullPath = GetEnvPath() + csTrack2;
			if (PathFileExists(csFullPath))
				csTrack2 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack2.MakeUpper()));
				csTrack2 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track2\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track3"), csTrack3))
		{
			csFullPath = GetEnvPath() + csTrack3;
			if (PathFileExists(csFullPath))
				csTrack3 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack3.MakeUpper()));
				csTrack3 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track3\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track4"), csTrack4))
		{
			csFullPath = GetEnvPath() + csTrack4;
			if (PathFileExists(csFullPath))
				csTrack4 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack4.MakeUpper()));
				csTrack4 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track4\" not set"));

		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track5"), csTrack5))
		{
			csFullPath = GetEnvPath() + csTrack5;
			if (PathFileExists(csFullPath))
				csTrack5 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack5.MakeUpper()));
				csTrack5 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track5\" not set"));
		
		if (SettingsFile.ReadValue(SETTINGS_SECTION_COMMON, _T("Track6"), csTrack6))
		{
			csFullPath = GetEnvPath() + csTrack6;
			if (PathFileExists(csFullPath))
				csTrack6 = csFullPath;
			else
			{
				AfxMessageBox((_T("....WARNING : %s not found"), csTrack6.MakeUpper()));
				csTrack6 = csDefaultTrack;
			}
		}
		else
			AfxMessageBox(_T("....WARNING : \"Track6\" not set"));
	}
	else
	{
		bResult = false;
		AfxMessageBox((_T("....Bad INI file or file too long : %s"), GetEnvPath() ));
	}

	return bResult;
}

bool CBimZicApp::LoadData(void)
{
	CString csFullPath;
	CString csValue;
	bool bResult = true;

	csFullPath = GetEnvPath() + _T("Results.txt");
	if (PathFileExists(csFullPath))
	{
		cIniManip DataFile(csFullPath);

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_1, csValue);
		if (!csValue.IsEmpty())
			Votes1 = _tstoi(csValue);
		else
			Votes1 = 0;

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_2, csValue);
		if (!csValue.IsEmpty())
			Votes2 = _tstoi(csValue);
		else
			Votes2 = 0;

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_3, csValue);
		if (!csValue.IsEmpty())
			Votes3 = _tstoi(csValue);
		else
			Votes3 = 0;

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_4, csValue);
		if (!csValue.IsEmpty())
			Votes4 = _tstoi(csValue);
		else
			Votes4 = 0;

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_5, csValue);
		if (!csValue.IsEmpty())
			Votes5 = _tstoi(csValue);
		else
			Votes5 = 0;

		csValue.Empty();
		DataFile.ReadValue(DATA_SECTION_DATA, DATA_KEY_VOTES_6, csValue);
		if (!csValue.IsEmpty())
			Votes5 = _tstoi(csValue);
		else
			Votes5 = 0;
	}
	else
	{
		// The file does not exist so all variables are reset to their default value
		Votes1 = 0;
		Votes2 = 0;
		Votes3 = 0;
		Votes4 = 0;
		Votes5 = 0;
		Votes6 = 0;
		
		bResult = false; 
	}
	
	return bResult;
}

bool CBimZicApp::SaveData(void)
{
	CString csFullPath;
	CString csValue;
	bool bResult = true;

	csFullPath = GetEnvPath() + _T("Results.txt");
	cIniManip DataFile(csFullPath);

	csValue.Empty();
	csValue.Format(_T("%d"), Votes1);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_1, csValue);
	
	csValue.Empty();
	csValue.Format(_T("%d"), Votes2);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_2, csValue);
	
	csValue.Empty();
	csValue.Format(_T("%d"), Votes3);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_3, csValue);

	csValue.Empty();
	csValue.Format(_T("%d"), Votes4);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_4, csValue);

	csValue.Empty();
	csValue.Format(_T("%d"), Votes5);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_5, csValue);
	
	csValue.Empty();
	csValue.Format(_T("%d"), Votes6);
	DataFile.WriteValue(DATA_SECTION_DATA, DATA_KEY_VOTES_6, csValue);

	return bResult;
}