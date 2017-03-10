// RSet.h : main header file for the RSET application
//

#if !defined(AFX_RSET_H__F8D53C05_14C3_11D5_BFEA_0040054EF889__INCLUDED_)
#define AFX_RSET_H__F8D53C05_14C3_11D5_BFEA_0040054EF889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRSetApp:
// See RSet.cpp for the implementation of this class
//
#include<string>
#include <fstream>

class CRSetApp : public CWinApp
{ 
public:
	CString g_strFileToOpen;
	bool g_bOpenFile;
	int iIndex;
	long g_lAlgRunning;
	std::string  strLogFileName;
	std::ofstream  logout;
public:
	void WriteLog(const char*);
	void WriteLogAlg(const char*,const char*,const char* =0);
	void WriteLogAlgOK(const char* file,bool bSaved);
	BOOL OpenFile(const CString strTarget);
	BOOL ShowDataOfFile(const CString& strTarget);
	CRSetApp();

private:
	bool m_bAddToRecentFileList;
	void SetAddToRecentFileList(bool);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSetApp)
	public:
	virtual BOOL InitInstance();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRSetApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolLog();
	//}}AFX_MSG
	afx_msg void OnKylinOpenFileByApp();
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSET_H__F8D53C05_14C3_11D5_BFEA_0040054EF889__INCLUDED_)
