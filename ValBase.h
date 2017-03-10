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
	int AttCount;			//���߱�����(������),ʵ�ʵ�����	
	int AttCount1;//
	int RecCount;			//���߱�����(��¼��)
	char **datasign;			//�������ͱ�־��
	int *datatype;			//��������(1Ϊint,2Ϊfloat,3Ϊstring)
	int stage;				//�ļ���־
	char style[15];			//�ļ����ͱ�־
	int **tab;
	bool *delatt;//���������Լ����ȥ����������
	ofstream fout;
	bool **flagtab;//�����Щ�������Կ���ȥ����
						//false������ֵ����ȥ������ֵ
	bool *delrec;//�����Щ�п���ȥ����false���в���ȥ������ֵ
	int * same_rec;
	void Save_File(char *r);
	int *atb;
	int atbcount;
private:
	
	int Read_Head();		//���ļ�ͷ
	int Read_Data();		//������
    int Transfer_Tab();
	int Read_Cut();		//���ϵ�
	ifstream fin;
	//ofstream fout;
	char ***tabstr;
};

#endif // !defined(AFX_VALBASE_H__0F1CC441_1BE2_11D5_BEE4_0050FC0BE958__INCLUDED_)
