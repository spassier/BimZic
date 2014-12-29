#include "StdAfx.h"
#include "CInifile.h"

//cIniManip functions :


 cIniManip::cIniManip()
  {
    m_FileName = "";
    m_LastErrorDescription = "";
    m_LastErrorNumber = 0;
	m_sectionList = new CStringList();
	m_sectionDataList = new CStringList();
  }
  
  cIniManip::cIniManip(CString strFile)
  {
    m_FileName = "";
    m_LastErrorDescription = "";
    m_LastErrorNumber = 0;
	setFileName(strFile);
	m_sectionList = new CStringList();
	m_sectionDataList = new CStringList();
  }
  
 cIniManip:: ~cIniManip()
  { 
    m_FileName = "";
    m_LastErrorDescription = "";
    m_LastErrorNumber = 0;
	m_sectionList->RemoveAll();
	m_sectionDataList->RemoveAll();
	delete m_sectionList;
    delete m_sectionDataList;
  }


void cIniManip::setFileName(CString striniFile)
{
	CFileFind ff;
	m_FileName = striniFile;
	
	if(ff.FindFile(m_FileName,0))
	{
		ff.FindNextFile();
		m_FileName=ff.GetFilePath();
	}
	else // Create an UNICODE file if not found
	{
		CStdioFile cFile;
		// Create the file
		if (cFile.Open(m_FileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			// Add the unicode BOM tag 0xFEFF (UTF-16 little endian) at the beginning of the file 
			// FIXME: 000D 000A is inserted a the end of the BOM
			cFile.Write(static_cast<LPVOID>(LPVOID(CINIFILE_UNICODE_BOM)), sizeof(CINIFILE_UNICODE_BOM));

			cFile.Close();
		}
	}
}

CString cIniManip::GetLastErrorDescription()
{
	CString retval;

	retval=m_LastErrorDescription;
	return retval;
}
bool cIniManip::ReadValue(CString psSection,CString psKey,CString& psValue)
{
	DWORD RetValue;
	_TCHAR ac_Result[600];
    DWORD  i = 599;
	// Get the info from the .ini file	
	RetValue = GetPrivateProfileString((LPCTSTR)psSection,(LPCTSTR)psKey,	NULL,ac_Result, i, (LPCTSTR)m_FileName);

	psValue=ac_Result;
	return (RetValue >0);
}
bool cIniManip::WriteValue(CString psSection,CString psKey,CString psValue)
{
	BOOL RetValue;
	
	if(psKey.IsEmpty())
		RetValue = WritePrivateProfileString (psSection, NULL, NULL, m_FileName);
//	else if (psValue.IsEmpty())
//			RetValue = WritePrivateProfileString (psSection, psKey, NULL, m_FileName);
	else
		RetValue = WritePrivateProfileString (psSection, psKey, psValue, m_FileName);

	return (RetValue>0);
//TODO : populate lasterror
}
bool cIniManip::CheckSection(CString strSection)
{
	_TCHAR ac_Result[600];	
	DWORD RetValue;

	// Get the info from the .ini file	
	RetValue=GetPrivateProfileString((LPCTSTR)strSection,NULL,
		L"",ac_Result, 599, (LPCTSTR)m_FileName);
	// Return if we could retrieve any info from that section
	return (RetValue>0);


}
bool cIniManip::ReadSection(CString psSection)
{
	DWORD RetValue;

	static _TCHAR ac_Result[32767];  //change size depending on needs
	CString strSectionData;

	RetValue = GetPrivateProfileSection((LPCTSTR)psSection, ac_Result, 32767, (LPCTSTR)m_FileName);
	m_sectionDataList->RemoveAll();
	for(DWORD i=0; i<RetValue; i++)
	{
		if(ac_Result[i] != '\0') {
			strSectionData = strSectionData + ac_Result[i];
		} else {
			if(strSectionData != "") {
				m_sectionDataList->InsertAfter(m_sectionDataList->GetTailPosition(),strSectionData);
			}
			strSectionData = "";
		}
	}
	return (RetValue>1);
}

//Get Sections and populate a string list
//with the result
bool cIniManip::GetSections()
{
	DWORD RetValue;
	 static _TCHAR ac_Result[32767];  //change size depending on needs
	CString strSectionData;
	//get all sections separated by '\0' end is '\0\0'  
	RetValue = GetPrivateProfileSectionNames(ac_Result, 32767, (LPCTSTR)m_FileName);
	m_sectionList->RemoveAll();
	for(DWORD i=0; i<RetValue; i++)
	{
		if(ac_Result[i] != '\0') {
			strSectionData = strSectionData + ac_Result[i];
		} else {
			if(strSectionData != "") {
     
				m_sectionList->InsertAfter(m_sectionList->GetTailPosition(),strSectionData);
			}
			strSectionData = "";
		}
	}
	return (RetValue>0);
}
bool cIniManip::WriteSection(CString psSection)
{
	DWORD RetValue;
	_TCHAR ac_Result[32767];  //change size depending on needs
	int index=0;
	CString elem;	
    const int i =255;
	for (POSITION pos=m_sectionDataList->GetHeadPosition();pos!=NULL;)
	{
		elem=m_sectionDataList->GetAt(pos);
		_stprintf_s(ac_Result+index,i,L"%s",elem);
		index+=elem.GetLength()+1;
		m_sectionDataList->GetNext(pos);
	}
	ac_Result[index]=NULL;

	RetValue=WritePrivateProfileSection(psSection,ac_Result,m_FileName);
	return (RetValue>0);
}

bool cIniManip::ReadIntValue(CString psSection, CString psKey, int &iValue)
{
	DWORD RetValue;
	TCHAR ac_Result[600];
	DWORD  i = 599;

	// Get the info from the .ini file	
	RetValue = GetPrivateProfileString((LPCTSTR)psSection,(LPCTSTR)psKey,	NULL, ac_Result, i, (LPCTSTR)m_FileName);

		
	if(ac_Result[0] != L'')
		iValue = _tstoi(ac_Result);
	else
		iValue = -1;

	return (RetValue >0);
}

bool cIniManip::ReadBoolValue(CString psSection, CString psKey, bool &bValue)
{
	DWORD RetValue;
	TCHAR ac_Result[600];
	DWORD i = 599;
	CString csValue;

	// Get the info from the .ini file	
	RetValue = GetPrivateProfileString((LPCTSTR)psSection,(LPCTSTR)psKey,	NULL, ac_Result, i, (LPCTSTR)m_FileName);

	csValue = ac_Result;
	if (csValue.CompareNoCase(_T("TRUE")) == 0)
		bValue = true;
	else
		bValue = false;
		
	return (RetValue >0);
}