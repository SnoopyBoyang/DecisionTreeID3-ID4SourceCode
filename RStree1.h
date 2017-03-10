// RStree1.h: interface for the CRStree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREE1_H__A33638B1_8B93_4DDA_8E61_0A8782E22327__INCLUDED_)
#define AFX_RSTREE1_H__A33638B1_8B93_4DDA_8E61_0A8782E22327__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ID3.h"

class CRStree  
{
public:
	void MutiIncreStudy(int studynum,int i,int j);
	void SearchRule(CString *object,TreeNode *CurrentNode);
	void SampleTest(CString filename1,CString filename2);
	void RepairTree(TreeNode **RulePath,int studyno);
	int ifconflict(CString *studyobject,int *markallmatch);
	void Generate_Rule(TreeNode **RulePath,int studyno);
	void ReleaseSpace();
	static int ifBestRulePath;
	void Run();
	static int * rulelength;
	void OutPutRSTree(CString filename);
	static CString ** datatable;
	static int datatablelength;
	static int datatablewidth;
	void RSBuildTree();
	void ReadStudyTable(CString filename);
	void ReadDataTable(CString filename);
	void ReadTable(CString filename);
	static int matchtablelength;
	static CString ** matchtable;
	void SearchMatchTable(TreeNode **RulePath,int no);
	int maxmatchnumber;
	void RSIncrementalStudy(int i,int j);
	void Search(CString *object,TreeNode *CurrentNode);
	void Search1(CString *object,TreeNode *CurrentNode);
	static CString ** studytable;
	static int studytablelength;
	static int studytablewidth;
	TreeNode ** BestRulePath;
	TreeNode ** tempRulePath;
	void OrderAttr(int *&orderofattr);
	static TreeNode RSHeadNode;
	static int * orderofattr;
	static CString **table;
	static int tablewidth;
	static int tablelength;
	int AddRuletoTree(CString *object);
	CRStree();
	virtual ~CRStree();
};
#endif // !defined(AFX_RSTREE1_H__A33638B1_8B93_4DDA_8E61_0A8782E22327__INCLUDED_)
