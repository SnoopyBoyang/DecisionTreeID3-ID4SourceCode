// ValReductionOne.cpp: implementation of the ValReductionOne class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "rset.h"

#include "ValReductionOne.h"
#include"stdlib.h"
#include"string.h"


//////////////////////////////////////////////////////////////////////

void ValReductionOne::CheckAtt()
{
	bool RecCon;//�г�ͻ��־��false�г�ͻ����ֵ
	bool RecEqual;//����ȱ�־��true��ȣ���ֵ
	int r1,r2;//��ѭ������
	int a1,a2;//��ѭ������
	
	//����ֵ
	flagtab=new bool *[RecCount];
	for(r1=0;r1<RecCount;r1++)
		flagtab[r1]=new bool[AttCount];
	for(r1=0;r1<RecCount;r1++)
		for(a1=0;a1<AttCount;a1++)
			flagtab[r1][a1]=false;
	delrec=new bool[RecCount];
	for(r1=0;r1<RecCount;r1++)
		delrec[r1]=false;
	//���ÿ������ȥ��������ֵ
	for(r1=0;r1<RecCount;r1++)
		for(a1=0;a1<AttCount-1;a1++)
		{
			RecCon=false;
			for(r2=0;r2<RecCount;r2++)
				if (r1!=r2)
				{
					RecEqual=true;
					for(a2=0;a2<AttCount-1;a2++)//��������������
						if(a1!=a2)
							if(!flagtab[r1][a2])//&&
								//(!flagtab[r2][a2]))
							{
								if(tab[r1][a2]!=tab[r2][a2])
								{
									RecEqual=false;//���в���
									break;
								}
						}
					if(RecEqual)//���������������
						if(tab[r1][AttCount-1]!=tab[r2][AttCount-1])
							//�������Բ���
						{
							RecCon=true;//�г�ͻ
							break;
						}
				}//r2��ѭ��
			if(!RecCon) //�޳�ͻ���ù���ĸ����ԵĿ���ȥ��
				flagtab[r1][a1]=true;
		}//a1,��ѭ������
}

void ValReductionOne::DelRep()
{
	bool RecEqual;//����ȱ�־��true��ȣ���ֵ
	int r1,r2;//��ѭ������
	int a1;//��ѭ������
	int m;
	same_rec=new int[RecCount];
	for(r1=0;r1<RecCount;r1++)
		same_rec[r1]=0;
	for(r1=0;r1<RecCount-1;r1++)
		if(!delrec[r1])//����û�б�ɾ��
		{
			m=0;
			for(r2=r1+1;r2<RecCount;r2++)
				if(!delrec[r2]) 
				{
					RecEqual=true;//�������
					for(a1=0;a1<AttCount;a1++)
						if((!flagtab[r1][a1])&&(!flagtab[r2][a1]))
						{
							if(tab[r1][a1]!=tab[r2][a1])
							{
								RecEqual=false;
								break;
							}
						}
						else if(((flagtab[r1][a1])&&(!flagtab[r2][a1]))||((!flagtab[r1][a1])&&(flagtab[r2][a1])))
						{
							RecEqual=false;
							break;
						}
					if(RecEqual)//���������
					{
						delrec[r2]=true;
						m++;
					}
				}//r2ѭ������
			same_rec[r1]=m;
		}
}

bool ValReductionOne::Run(char *s,char *r)
{
	if(RunOne(s)!=1) return false;
	DelRep2();
	CheckAtt();
	DelRep();
	Save_File(r);
	return true;
}


ValReductionOne::ValReductionOne()
{
}

ValReductionOne::~ValReductionOne()
{

	int i,j;
	for(i=0;i<RecCount;i++)
	{
		delete []tab[i];
		delete []flagtab[i];
	}
	delete []tab;
	for(j=0;j<AttCount;j++)
	{
		delete []datasign[j];
		if (cuttab[j]!=NULL) delete []cuttab[j];
	}
	delete []cuttab;
	delete []datatype;
	delete []datasign;
	delete []flagtab;
	delete []delrec;
	delete []same_rec;
}


void ValReductionOne::DelRep2()
{
	bool RecEqual;//����ȱ�־��true��ȣ���ֵ
	int r1,r2;//��ѭ������
	int a1;//��ѭ������
	delrec=new bool[RecCount];
	for(r1=0;r1<RecCount;r1++)
		delrec[r1]=false;
	for(r1=0;r1<RecCount-1;r1++)
		if(!delrec[r1])//����û�б�ɾ��
			for(r2=r1+1;r2<RecCount;r2++)
				if(!delrec[r2]) 
				{
					RecEqual=true;//�������
					for(a1=0;a1<AttCount;a1++)
						{
							if(tab[r1][a1]!=tab[r2][a1])
							{
								RecEqual=false;
								break;
							}
						}
					if(RecEqual)//���������
						delrec[r2]=true;//���п���ȥ��
				}//r2ѭ������
}
