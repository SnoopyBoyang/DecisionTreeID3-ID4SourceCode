// ID.h : main header file for the ID application
//

#if !defined(AFX_ID_H__DA3338A5_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
#define AFX_ID_H__DA3338A5_6CA2_11D6_9A55_0050BAF17437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIDApp:
// See ID.cpp for the implementation of this class
//

class CIDApp : public CWinApp
{
public:
	CIDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID_H__DA3338A5_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
