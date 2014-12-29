#pragma once


const unsigned char CINIFILE_UNICODE_BOM[2] = {unsigned char(0xFF), unsigned char(0xFE)};


class cIniManip
{
public:
	cIniManip();
    cIniManip(CString strFile);
    ~cIniManip();
	void setFileName(CString strINIFile);
	CString GetLastErrorDescription();
	bool ReadValue(CString psSection,CString psKey,CString& psValue);
	bool ReadIntValue(CString psSection, CString psKey, int &iValue);
	bool ReadBoolValue(CString psSection, CString psKey, bool &bValue);
	bool WriteValue(CString psSection,CString psKey,CString psValue);
	bool CheckSection(CString strSection);
	bool GetSections();
	bool ReadSection(CString psSection);
	bool WriteSection(CString psSection);
	CStringList *m_sectionDataList;
	CStringList *m_sectionList;
	CString m_strSection;



private:
    CString m_FileName;
    CString m_LastErrorDescription;
    int m_LastErrorNumber;
};