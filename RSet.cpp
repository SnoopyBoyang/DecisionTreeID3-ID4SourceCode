// RSet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RSet.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "liedoc.h"
#include "RSetDoc.h"
//#include "stdio.h"
#include "RSetView.h"
#include "showdataview.h"
#include "showdatadoc.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include <sys/timeb.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSetApp

BEGIN_MESSAGE_MAP(CRSetApp, CWinApp)
	//{{AFX_MSG_MAP(CRSetApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOL_LOG, OnToolLog)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_MESSAGE(WM_KYLIN_OPENFILE_BY_APP,OnKylinOpenFileByApp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSetApp construction

CRSetApp::CRSetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bAddToRecentFileList=true;
	g_bOpenFile=false;
	g_lAlgRunning=0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRSetApp object

CRSetApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRSetApp initialization

BOOL CRSetApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(6); 
	// Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_RSETTYPE,
		RUNTIME_CLASS(CRSetDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRSetView));
	AddDocTemplate(pDocTemplate);

	CMultiDocTemplate* pDocTemplateShow;
	pDocTemplateShow = new CMultiDocTemplate(
		IDR_SHOWDATATYPE,
		RUNTIME_CLASS(CShowDataDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CShowDataView));
	AddDocTemplate(pDocTemplateShow);

/*	CMultiDocTemplate* pDocLieDoc;
	pDocLieDoc = new CMultiDocTemplate(
		IDR_LIEDOC,
		RUNTIME_CLASS(CLieDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CShowDataView));
	AddDocTemplate(pDocLieDoc);

*/
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

// open log file
	char TmpPath[_MAX_DIR];

// prepare for out file name
	GetTempPath(_MAX_DIR,TmpPath);
	
	std::string strLogFileName=TmpPath;
	strLogFileName+="rsetlog.htm";
	logout.open(strLogFileName.c_str());
	if(logout==0)
		AfxMessageBox("初始化错误,不能写日志文件.");
	else
		logout<<"<font color=green size=4><B>RSet 数据分析平台.</font>"<<"</b><br><br>"<<std::endl;
	WriteLog("进入系统");
//	pDocTemplateShow->OpenDocumentFile(NULL);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// App command to run the dialog
void CRSetApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRSetApp message handlers
void CRSetApp::WriteLog(const char* str)
{
	if(logout!=0)
	{
	//	CTime time;
	//	time=CTime::GetCurrentTime();
	//	logout<<time.GetYear()<<"年"<<time.GetMonth()<<"月";
	//	logout<<time.GetDay()<<"日"<<time.GetHour()<<":";
	//	logout<<time.GetMinute()<<":"<<time.GetSecond()<<'\t';

		CString b;
		struct _timeb timebuffer;
		char *timeline;
		_ftime( &timebuffer );
		timeline = ctime( & ( timebuffer.time ) );
		b.Format("The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );
		


		logout<<(LPCTSTR)b<<"<br><br>"<<std::endl;
	}
}
void CRSetApp::WriteLogAlg(const char* strAlgName,const char* strFileName1,const char* strFileName2)
{
	if(logout!=0)
	{
	//	CTime time;
	//	time=CTime::GetCurrentTime();
	//	logout<<time.GetYear()<<"年"<<time.GetMonth()<<"月";
	//	logout<<time.GetDay()<<"日"<<time.GetHour()<<":";
	//	logout<<time.GetMinute()<<":"<<time.GetSecond()<<'\t';

		CString b;
		struct _timeb timebuffer;
		char *timeline;
		_ftime( &timebuffer );
		timeline = ctime( & ( timebuffer.time ) );
		b.Format("The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );
		
		
		logout<<(LPCTSTR)b<<"执行算法<font color=red>"<<strAlgName<<"</font><br>";
		logout<<"&nbsp;&nbsp;输入文件1: <a href=\""<<strFileName1<<"\">"<<strFileName1<<"</a><br>";
		if(strFileName2)
			logout<<"&nbsp;&nbsp;输入文件2:<a href=\""<<strFileName2<<"\">"<<strFileName2<<"</a><br>";
		logout<<std::endl;
			
	}
}

void CRSetApp::WriteLogAlgOK(const char* file,bool bSaved)
{
	if(logout!=0)
	{
			CString b;
		struct _timeb timebuffer;
		char *timeline;
		_ftime( &timebuffer );
		timeline = ctime( & ( timebuffer.time ) );
		b.Format("The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );
		

		logout<<LPCTSTR(b)<<"保存文件.<br>";
		if(bSaved)
			logout<<"&nbsp;&nbsp;结果文件(已保存):";
		else
			logout<<"&nbsp;&nbsp;结果文件(临时文件):";
		logout<<"<a href=\""<<file<<"\">"<<file<<"</a><br>";
		logout<<"<br>"<<std::endl;
		
			
	}
}

BOOL CRSetApp::ShowDataOfFile(const CString &strTarget)
{
	CString strDocName;
	CDocTemplate* pSelectedTemplate;
	POSITION pos=GetFirstDocTemplatePosition();
	while(pos!=NULL)
	{
		pSelectedTemplate=GetNextDocTemplate(pos);
		ASSERT(pSelectedTemplate!=NULL);
		ASSERT(pSelectedTemplate->IsKindOf(
			RUNTIME_CLASS(CDocTemplate)));
		pSelectedTemplate->GetDocString(strDocName,CDocTemplate::docName);
		TRACE(strDocName);
		if(strDocName=="ShData")
		{
			// to see if the document has been opened
			CDocument* pSelectedDoc;
			POSITION docpos=pSelectedTemplate->GetFirstDocPosition();
			while(docpos!=NULL)
			{
				pSelectedDoc=pSelectedTemplate->GetNextDoc(docpos);
				if(pSelectedDoc->GetPathName()==strTarget)
				{
					POSITION viewpos=pSelectedDoc->GetFirstViewPosition();
					CView* pView=pSelectedDoc->GetNextView(viewpos);
					if(pView)
					{
						// how could i active this view?
						((CMDIFrameWnd *)AfxGetMainWnd())->MDIActivate(pView->GetParent());
	
//						AfxMessageBox("aaa");
					}
					return true;
				}
			}
			
			SetAddToRecentFileList(false);	// not add to recent file list
			pSelectedTemplate->OpenDocumentFile(strTarget);
			SetAddToRecentFileList(true);  //restore to the default status
			return true;
		}
	}
	TRACE("in showdataoffile,file can't be opened!\n");
	return FALSE;
}

BOOL CRSetApp::OpenFile(const CString strTarget)
{
	CString strDocName;
	CDocTemplate* pSelectedTemplate;
	POSITION pos=GetFirstDocTemplatePosition();
	while(pos!=NULL)
	{

		pSelectedTemplate=GetNextDocTemplate(pos);
		ASSERT(pSelectedTemplate!=NULL);
		ASSERT(pSelectedTemplate->IsKindOf(
			RUNTIME_CLASS(CDocTemplate)));
		pSelectedTemplate->GetDocString(strDocName,CDocTemplate::docName);
		TRACE(strDocName);
		if(strDocName=="RSet")
		{
			CDocument* pSelectedDoc;
			POSITION docpos=pSelectedTemplate->GetFirstDocPosition();
			while(docpos!=NULL)
			{
				pSelectedDoc=pSelectedTemplate->GetNextDoc(docpos);
				if(pSelectedDoc->GetPathName()==strTarget)
				{
					POSITION viewpos=pSelectedDoc->GetFirstViewPosition();
					CView* pView=pSelectedDoc->GetNextView(viewpos);
					if(pView)
					{
						// how could i active this view?
						((CMDIFrameWnd *)AfxGetMainWnd())->MDIActivate(pView->GetParent());
	
//						AfxMessageBox("aaa");
					}
					return true;
				}
			}
			SetAddToRecentFileList(false);
			pSelectedTemplate->OpenDocumentFile(strTarget);
			SetAddToRecentFileList(true);
			return true;
		}
	}
	TRACE("in showdataoffile,file can't be opened!\n");
	return FALSE;

} 


void CRSetApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	if(m_bAddToRecentFileList)	
	CWinApp::AddToRecentFileList(lpszPathName);
}

void CRSetApp::SetAddToRecentFileList(bool value)
{
	m_bAddToRecentFileList=value;
}

int CRSetApp::ExitInstance() 
{
	WriteLog("退出系统");
	logout<<"<font color=red>(警告:临时文件被删除.)</font><br>";
	logout<<"bug报告和问题反馈:<a href= \"mailto:icst@cqupt.edu.cn\">icst@cqupt.edu.cn</a><br>"<<std::endl;

	logout.close();
	//delete temp files produced in the app
	for(int i=0;i<iIndex;i++)
	{
		char tmpPath[_MAX_DIR],fileNameBuff[_MAX_DIR];
		::GetTempPath(_MAX_DIR,tmpPath);
		sprintf(fileNameBuff,"%s~~Rs@%d.tmp",tmpPath,i);
		::DeleteFile(fileNameBuff);
	}
	return CWinApp::ExitInstance();
}

void CRSetApp::OnKylinOpenFileByApp()
{
	OpenFile(g_strFileToOpen);
}

BOOL CRSetApp::OnIdle(LONG lCount) 
{

	if(g_lAlgRunning)
		((CMainFrame*)AfxGetMainWnd())->EnableToolBar(false);
	else
		((CMainFrame*)AfxGetMainWnd())->EnableToolBar(true);
	return CWinApp::OnIdle(lCount);
}

void CRSetApp::OnToolLog() 
{
char TmpPath[_MAX_PATH];
	GetTempPath(_MAX_DIR,TmpPath);
	std::string strLogFileName=TmpPath;
	strLogFileName+="rsetlog.htm";
	::ShellExecute(0,0,strLogFileName.c_str(),0,0,SW_SHOWNORMAL);
}
