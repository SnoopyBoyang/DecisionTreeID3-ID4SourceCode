// IDDlg.cpp : implementation file
//
#include "stdafx.h"
#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>
#include "ValReductionThree.h"
#include "ValReductionOne.h"
#include "RStree1.h"

#include "stdafx.h"
#include "ID.h"
#include "RStree1.h"
#include "IDDlg.h"
#include "ID3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CIDDlg dialog

CIDDlg::CIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIDDlg)
	m_datafile = _T("");
	m_rulefile = _T("");
	m_studyfile = _T("");
	m_ifincrementalstudy = FALSE;
	m_ifrecordtime = FALSE;
	m_samplefile = _T("");
	m_testresultfile = _T("");
	m_iftestsample = FALSE;
	m_rulerecordfile = _T("");
	m_ifmethod = FALSE;
	m_ifaccelarate = FALSE;
	m_increstudynum = 0;
	m_ifmultiincrestudy = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIDDlg)
	DDX_Control(pDX, IDC_LIST1, m_show);
	DDX_Text(pDX, IDC_DATAFILE, m_datafile);
	DDX_Text(pDX, IDC_RULEFILE, m_rulefile);
	DDX_Text(pDX, IDC_STUDYFILE, m_studyfile);
	DDX_Check(pDX, IDC_CHECK1, m_ifincrementalstudy);
	DDX_Check(pDX, IDC_CHECK2, m_ifrecordtime);
	DDX_Text(pDX, IDC_SAMPLEFILE, m_samplefile);
	DDX_Text(pDX, IDC_TESTRESULTFILE, m_testresultfile);
	DDX_Check(pDX, IDC_CHECK3, m_iftestsample);
	DDX_Text(pDX, IDC_RULERECORDFILE, m_rulerecordfile);
	DDX_Check(pDX, IDC_CHECK4, m_ifmethod);
	DDX_Check(pDX, IDC_CHECK5, m_ifaccelarate);
	DDX_Text(pDX, IDC_INCRESTUDYNUM, m_increstudynum);
	DDX_Check(pDX, IDC_CHECK6, m_ifmultiincrestudy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIDDlg, CDialog)
	//{{AFX_MSG_MAP(CIDDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ID3, OnId3)
	ON_BN_CLICKED(IDC_trans, Ontrans)
	ON_BN_CLICKED(IDC_writhead, Onwrithead)
	ON_BN_CLICKED(IDC_dist, Ondist)
	ON_BN_CLICKED(IDC_RSTREE, OnRstree)
	ON_BN_CLICKED(IDC_BUTTON1, OnValReduction)
	ON_BN_CLICKED(IDC_BUTTON2, OnValReduction1)
	ON_BN_CLICKED(IDC_BUTTON3, OnRuleFile)
	ON_BN_CLICKED(IDC_BUTTON4, OnDataFile)
	ON_BN_CLICKED(IDC_BUTTON5, OnStudyFile)
	ON_BN_CLICKED(IDC_BUTTON6, OnSampleFile)
	ON_BN_CLICKED(IDC_BUTTON7, OnTestResultFile)
	ON_BN_CLICKED(IDC_BUTTON8, OnRuleFileOpen)
	ON_BN_CLICKED(IDC_BUTTON9, OnDataFileOpen)
	ON_BN_CLICKED(IDC_BUTTON10, OnStudyFileOpen)
	ON_BN_CLICKED(IDC_BUTTON11, OnSampleFileOpen)
	ON_BN_CLICKED(IDC_BUTTON12, OnTestresultFile)
	ON_BN_CLICKED(IDC_BUTTON13, OnRuleRecordFile)
	ON_BN_CLICKED(IDC_BUTTON14, OnRuleRecordFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIDDlg message handlers

BOOL CIDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIDDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

TreeNode HeadNode;

void CIDDlg::OnId3() 
{//执行ID3和ID4算法
	ID3 id;
	int studynum;
	CString **objects,b;
	TreeNode **firstnodeoflayer;
	int *result;
	int i,j;
	char a[20];
	clock_t start, finish;
    double  duration;
	//读入原始表
	UpdateData(true);
	FILE *fp;
	if(m_datafile=="")
		m_datafile="data.txt";
	if((fp=fopen(m_datafile,"r"))==NULL)
	{
		MessageBox("不能打开数据文件data.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&HeadNode.tablelength);//行数
	fscanf(fp,"%d",&HeadNode.tablewidth);//列数
	HeadNode.table=new CString* [HeadNode.tablelength];
	for(i=0;i<HeadNode.tablelength;i++)
	{
		HeadNode.table[i]=new CString[HeadNode.tablewidth];
	}
	for(i=0;i<HeadNode.tablelength;i++)
		for(j=0;j<HeadNode.tablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			HeadNode.table[i][j]=a;
		}
	fclose(fp);
	if(m_ifincrementalstudy)
	{
		if(m_studyfile=="")
			m_studyfile="study.txt";
		if((fp=fopen(m_studyfile,"r"))==NULL)
		{
			MessageBox("不能打开学习文件study.txt", "Error", 
				MB_ICONERROR | MB_OK);
			return;
		}
		//读入新学习的记录
		int adf;
		fscanf(fp,"%d",&studynum);
		fscanf(fp,"%d",&adf);
		objects=new CString *[studynum];
		for(i=0;i<studynum;i++)
		{
			objects[i]=new CString [HeadNode.tablewidth];	
		}
		char buffer[20];
		for(i=0;i<studynum;i++)
			for(j=0;j<HeadNode.tablewidth;j++)
			{
				fscanf(fp,"%s",buffer);
				objects[i][j]=buffer;
			}
	}
	int *resplitlayerattr;
	int *rebuildrefattrsplit;
	start = clock();
	HeadNode.splitattr=id.ChooseAttr(HeadNode.table,HeadNode.tablelength,HeadNode.tablewidth);
	HeadNode.decisionattrvalue=" ";
	HeadNode.next=NULL;//ok	
	HeadNode.layer=0;
	rebuildrefattrsplit=new int [HeadNode.tablewidth];
	for(i=0;i<HeadNode.tablewidth;i++)
		rebuildrefattrsplit[i]=-1;
	result=new int [HeadNode.tablewidth];
	for(i=0;i<HeadNode.tablewidth;i++)
		result[i]=0;
	firstnodeoflayer=new TreeNode *[HeadNode.tablewidth];
	firstnodeoflayer[0]=&HeadNode;
	for(i=1;i<HeadNode.tablewidth;i++)
		firstnodeoflayer[i]=NULL;
	id.BuildTree(HeadNode,rebuildrefattrsplit,resplitlayerattr,result,firstnodeoflayer,0);
	finish=clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    b.Format( "建树时间为%2.6f seconds\n", duration );
	m_show.InsertString(-1,b);
	if(m_ifincrementalstudy)
	{
		start=clock();
		id.Incrementalstudy(HeadNode,objects,studynum,rebuildrefattrsplit,result,firstnodeoflayer);
		for(i=0;i<studynum;i++)
			delete[] objects[i];
		delete[] objects;
		finish=clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;  
		b.Format( "追加学习时间为%2.6f seconds\n", duration );
		m_show.InsertString(-1,b);	
	}
	id.OutPutTree(HeadNode,result,m_rulerecordfile);
	fclose(fp);
}


void CIDDlg::Ontrans() 
{//粗糙集规则统计，即有规则集计算各种长度规则的个数
	ID3 id;
	id.transform(m_rulefile,m_rulerecordfile);
}

void CIDDlg::Onwrithead() 
{
	//写文件头
	//ID3 a;
	//a.writhead();
	CString **objects;
	int i,j;
	int table_length,table_width;
	char a[20];
	FILE *fp;

	UpdateData(true);
	if(m_rulefile=="") m_rulefile="rule.txt";
	if((fp=fopen(m_rulefile,"r"))==NULL)
	{
		MessageBox("不能打开"+m_rulefile, "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&table_length);//行数
	fscanf(fp,"%d",&table_width);
	objects=new CString* [table_length];
	for(i=0;i<table_length;i++)
	{
		objects[i]=new CString[table_width+3];
	}
	for(i=0;i<table_length;i++)
		for(j=0;j<table_width+3;j++)
		{
			fscanf(fp,"%s",a);				
			objects[i][j]=a;
		}
	fclose(fp);	
	if((fp=fopen(m_rulefile,"w"))==NULL)
	{
		MessageBox("不能打开"+m_rulefile, "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fprintf(fp,"%d  ",table_length);
	fprintf(fp,"%d\n",table_width);
	for(i=0;i<table_length;i++)
	{
		for(j=0;j<table_width;j++)
		{
			fprintf(fp,"%s   ",objects[i][j]);				
		}	
		fprintf(fp,"\n");
	}
	fclose(fp);	
}


void CIDDlg::Ondist() 
{//ID算法识别样本
	int tablelength,tablewidth,i,j;
	CString **objects;
	ID3 id;
	char a[20];
	FILE *fp;
	if(m_samplefile=="")
		m_samplefile="sample.txt";
	if((fp=fopen(m_samplefile,"r"))==NULL)
	{
		MessageBox("不能打开测试文件 样本.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&tablelength);//行数
	fscanf(fp,"%d",&tablewidth);
	objects=new CString* [tablelength];
	for(i=0;i<tablelength;i++)
	{
		objects[i]=new CString[tablewidth];
	}
	for(i=0;i<tablelength;i++)
		for(j=0;j<tablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			objects[i][j]=a;
		}
	fclose(fp);	
	id.test(HeadNode,objects,tablelength);

}

void CIDDlg::OnRstree() 
{
	CRStree m_rs;
	clock_t start, finish;
    double  duration;
	UpdateData(true);
	m_rs.ReadTable(m_rulefile);//读规则表
	if((m_ifincrementalstudy)||(m_ifmultiincrestudy))
		m_rs.ReadStudyTable(m_studyfile);//读学习表
	m_rs.ReadDataTable(m_datafile);//读原始表
	CString b;
	start = clock();
	m_rs.OrderAttr(m_rs.orderofattr);//属性排序
	m_rs.RSBuildTree();//建立规则树
	finish=clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    b.Format( "建树时间为%2.6f seconds\n", duration );
	m_show.InsertString(-1,b);
	int ifmethod,ifaccelarate;
	if(m_ifincrementalstudy)
	{
		m_ifmultiincrestudy=false;
		if(m_ifmethod)
			ifmethod=1;
		else
			ifmethod=0;
		if(m_ifaccelarate)
			ifaccelarate=1;
		else
			ifaccelarate=0;
		start=clock();		
		m_rs.RSIncrementalStudy(ifmethod,ifaccelarate);//追加学习
		finish=clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;  
		b.Format( "追加学习时间为%2.6f seconds\n", duration );
		m_show.InsertString(-1,b);	
	}
	if(m_ifmultiincrestudy)
	{
		if(m_ifmethod)
			ifmethod=1;
		else
			ifmethod=0;
		if(m_ifaccelarate)
			ifaccelarate=1;
		else
			ifaccelarate=0;
		start=clock();		
		m_rs.MutiIncreStudy(m_increstudynum,ifmethod,ifaccelarate);
		finish=clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;  
		b.Format( "追加学习时间为%2.6f seconds\n", duration );
		m_show.InsertString(-1,b);	
	}
	m_rs.ReleaseSpace();//释放空间
	m_rs.OutPutRSTree(m_rulerecordfile);//输出规则统计
	if(m_iftestsample)
		m_rs.SampleTest(m_samplefile,m_testresultfile);//样本统计
}

void CIDDlg::OnValReduction() 
{
	// TODO: Add your control notification handler code here
	ValReductionThree m_Val;
	m_Val.Run("sfs.txt","d.text"); 
	int i,j;
	FILE *fp;
	char a[20];
	CString **datatable;
	int datatablelength,datatablewidth;
	if((fp=fopen("data.txt","r"))==NULL)
	{
		MessageBox("不能打开原始数据表文件", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&datatablelength);//行数
	fscanf(fp,"%d",&datatablewidth);//列数
	datatable=new CString *[datatablelength];
	for(i=0;i<datatablelength;i++)
	{
		datatable[i]=new CString[datatablewidth];
	}
	for(i=0;i<datatablelength;i++)
		for(j=0;j<datatablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			datatable[i][j]=a;
		}
	fclose(fp);
	m_Val.Run1(datatable,datatablelength,datatablewidth);
}

void CIDDlg::OnValReduction1() 
{
	// TODO: Add your control notification handler code here
	ValReductionOne m_Val;
	m_Val.Run("sfs.txt","d.text"); 

}

void CIDDlg::OnRuleFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"rule.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_rulefile=SrcFileDlg.GetPathName();
	UpdateData(false);	
}

void CIDDlg::OnDataFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"data.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_datafile=SrcFileDlg.GetPathName();
	UpdateData(false);	
	
}

void CIDDlg::OnStudyFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"study.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_studyfile=SrcFileDlg.GetPathName();
	UpdateData(false);	
	
}

void CIDDlg::OnSampleFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"sample.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_samplefile=SrcFileDlg.GetPathName();
	UpdateData(false);	
}

void CIDDlg::OnTestResultFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"样本测试结果.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_testresultfile=SrcFileDlg.GetPathName();
	UpdateData(false);	
}

void CIDDlg::OnRuleFileOpen() 
{
	if(m_rulefile=="")
		m_rulefile="rule.txt";
	WinExec("notepad.exe "+m_rulefile,SW_SHOW);
}

void CIDDlg::OnDataFileOpen() 
{
	if(m_datafile=="")
		m_datafile="data.txt";
	WinExec("notepad.exe "+m_datafile,SW_SHOW);
}

void CIDDlg::OnStudyFileOpen() 
{
	if(m_studyfile=="")
		m_studyfile="study.txt";
	WinExec("notepad.exe "+m_studyfile,SW_SHOW);
}

void CIDDlg::OnSampleFileOpen() 
{
	if(m_samplefile=="")
		m_samplefile="sample.txt";
	WinExec("notepad.exe "+m_samplefile,SW_SHOW);
}

void CIDDlg::OnTestresultFile() 
{
	if(m_testresultfile=="")
		m_testresultfile="样本测试结果.txt";
	WinExec("notepad.exe "+m_testresultfile,SW_SHOW);
}

void CIDDlg::OnRuleRecordFile() 
{
	CFileDialog SrcFileDlg(TRUE,NULL,"规则统计.txt",OFN_HIDEREADONLY,"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",NULL);
	SrcFileDlg.DoModal();
	m_rulerecordfile=SrcFileDlg.GetPathName();
	UpdateData(false);	
}

void CIDDlg::OnRuleRecordFileOpen() 
{
	if(m_rulerecordfile=="")
		m_rulerecordfile="规则统计.txt";
	WinExec("notepad.exe "+m_rulerecordfile,SW_SHOW);
}

void CIDDlg::OnOutofmemorySpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

