// ID3.h: interface for the ID3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3_H__DA3338AF_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
#define AFX_ID3_H__DA3338AF_6CA2_11D6_9A55_0050BAF17437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TreeNode
{
	CString **table;
	int tablelength;
	int tablewidth;
	struct TreeNode *son;
	struct TreeNode *next;
	int sonnumber;
	int conditionattr;
	CString conditionattrvalue,decisionattrvalue;//假设决策属性只有一个
	int splitattr;
	int layer;
	int *splitedattr;

public:
	int matchnumber;
};


class ID3  
{
public:
	static TreeNode *savenode;
	void test(TreeNode &HeadNode, CString **object,int studynum);
	void writhead();
	void transform(CString filename1,CString filename2);
	void OutPutTree(TreeNode HeadNode,int *result,CString filename);
	int ChooseAttr(CString **table,int tablelength,int tablewidth);
	void ReadTable(CString **studytable,TreeNode &HeadNode,TreeNode &son,CString *object,int *splitlayerattr,int studyno);
	void Incrementalstudy(TreeNode &HeadNode, CString **object,int studynum,int *&rebuildrefattrsplit,int *&result,TreeNode **&firstnodeoflayer);
	void BuildTree(TreeNode &HeadNode,int *&rebuildrefattrsplit,int *resplitlayerattr,int *&result,TreeNode **&firstnodeoflayer,int ifrebuild);
	ID3();
	virtual ~ID3();

		
};

#endif // !defined(AFX_ID3_H__DA3338AF_6CA2_11D6_9A55_0050BAF17437__INCLUDED_)
