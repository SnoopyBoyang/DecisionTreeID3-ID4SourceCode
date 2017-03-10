// IDDlg.h : header file
//

#if !defined(AFX_IDDLG_H__DA3338A7_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
#define AFX_IDDLG_H__DA3338A7_6CA2_11D6_9A55_0050BAF17437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIDDlg dialog

class CIDDlg : public CDialog
{
// Construction
public:
	CIDDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIDDlg)
	enum { IDD = IDD_ID_DIALOG };
	CListBox	m_show;
	CString	m_datafile;
	CString	m_rulefile;
	CString	m_studyfile;
	BOOL	m_ifincrementalstudy;
	BOOL	m_ifrecordtime;
	CString	m_samplefile;
	CString	m_testresultfile;
	BOOL	m_iftestsample;
	CString	m_rulerecordfile;
	BOOL	m_ifmethod;
	BOOL	m_ifaccelarate;
	int		m_increstudynum;
	BOOL	m_ifmultiincrestudy;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnId3();
	afx_msg void OnStudy();
	afx_msg void Ontrans();
	afx_msg void Onwrithead();
	afx_msg void Onstat();
	afx_msg void Ondist();
	afx_msg void OnRstree();
	afx_msg void OnValReduction();
	afx_msg void OnValReduction1();
	afx_msg void OnRuleFile();
	afx_msg void OnDataFile();
	afx_msg void OnStudyFile();
	afx_msg void OnSampleFile();
	afx_msg void OnTestResultFile();
	afx_msg void OnRuleFileOpen();
	afx_msg void OnDataFileOpen();
	afx_msg void OnStudyFileOpen();
	afx_msg void OnSampleFileOpen();
	afx_msg void OnTestresultFile();
	afx_msg void OnRuleRecordFile();
	afx_msg void OnRuleRecordFileOpen();
	afx_msg void OnOutofmemorySpin2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDDLG_H__DA3338A7_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
