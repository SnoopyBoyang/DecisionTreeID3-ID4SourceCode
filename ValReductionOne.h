// ValReductionOne.h: interface for the ValReductionOne class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALREDUCTIONONE_H__C8BFA0C1_C2C3_11D4_9B83_0080C84A0D59__INCLUDED_)
#define AFX_VALREDUCTIONONE_H__C8BFA0C1_C2C3_11D4_9B83_0080C84A0D59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"string.h"
#include<fstream.h>
#include"ValBase.h"

class ValReductionOne :ValBase 
{
private:
	void DelRep2();
	void DelRep();			//ɾ���ظ�����
	void CheckAtt();		//����ɾ��������
public:
	bool  Run(char *s,char *r);           //��ӣ���ִ��
	ValReductionOne();
	virtual ~ValReductionOne();
    
};

#endif // !defined(AFX_VALREDUCTIONONE_H__C8BFA0C1_C2C3_11D4_9B83_0080C84A0D59__INCLUDED_)
