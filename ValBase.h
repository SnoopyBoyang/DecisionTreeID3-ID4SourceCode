 // ValBase.h: interface for the ValBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALBASE_H__0F1CC441_1BE2_11D5_BEE4_0050FC0BE958__INCLUDED_)
#define AFX_VALBASE_H__0F1CC441_1BE2_11D5_BEE4_0050FC0BE958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RSet.h"
#include <fstream.h>
#include "MyCut.h"

class ValBase  
{
public:
	void ReadReductionTable(CString **table,int &tablelength,int tablewidth);
	int RunOne(char *s);
	ValBase();
	virtual ~ValBase();
	
protected:
	
	MyCut **cuttab;
	int AttCount;			//决策表列数(属性数),实际的列数	
	int AttCount1;//
	int RecCount;			//决策表行数(记录数)
	char **datasign;			//数据类型标志符
	int *datatype;			//数据类型(1为int,2为float,3为string)
	int stage;				//文件标志
	char style[15];			//文件类型标志
	int **tab;
	bool *delatt;//标记在属性约简中去掉的属性列
	ofstream fout;
	bool **flagtab;//标记哪些条件属性可以去掉，
						//false该属性值不能去掉，初值
	bool *delrec;//标记哪些行可以去掉，false该行不能去掉，初值
	int * same_rec;
	void Save_File(char *r);
	int *atb;
	int atbcount;
private:
	
	int Read_Head();		//读文件头
	int Read_Data();		//读正文
    int Transfer_Tab();
	int Read_Cut();		//读断点
	ifstream fin;
	//ofstream fout;
	char ***tabstr;
};

#endif // !defined(AFX_VALBASE_H__0F1CC441_1BE2_11D5_BEE4_0050FC0BE958__INCLUDED_)
