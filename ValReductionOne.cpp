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
	bool RecCon;//行冲突标志，false有冲突，初值
	bool RecEqual;//行相等标志，true相等，初值
	int r1,r2;//行循环变量
	int a1,a2;//列循环变量
	
	//赋初值
	flagtab=new bool *[RecCount];
	for(r1=0;r1<RecCount;r1++)
		flagtab[r1]=new bool[AttCount];
	for(r1=0;r1<RecCount;r1++)
		for(a1=0;a1<AttCount;a1++)
			flagtab[r1][a1]=false;
	delrec=new bool[RecCount];
	for(r1=0;r1<RecCount;r1++)
		delrec[r1]=false;
	//检查每个可能去掉的属性值
	for(r1=0;r1<RecCount;r1++)
		for(a1=0;a1<AttCount-1;a1++)
		{
			RecCon=false;
			for(r2=0;r2<RecCount;r2++)
				if (r1!=r2)
				{
					RecEqual=true;
					for(a2=0;a2<AttCount-1;a2++)//不包括决策属性
						if(a1!=a2)
							if(!flagtab[r1][a2])//&&
								//(!flagtab[r2][a2]))
							{
								if(tab[r1][a2]!=tab[r2][a2])
								{
									RecEqual=false;//两行不等
									break;
								}
						}
					if(RecEqual)//两行条件属性相等
						if(tab[r1][AttCount-1]!=tab[r2][AttCount-1])
							//决策属性不等
						{
							RecCon=true;//有冲突
							break;
						}
				}//r2的循环
			if(!RecCon) //无冲突，该规则的该属性的可以去掉
				flagtab[r1][a1]=true;
		}//a1,列循环结束
}

void ValReductionOne::DelRep()
{
	bool RecEqual;//行相等标志，true相等，初值
	int r1,r2;//行循环变量
	int a1;//列循环变量
	int m;
	same_rec=new int[RecCount];
	for(r1=0;r1<RecCount;r1++)
		same_rec[r1]=0;
	for(r1=0;r1<RecCount-1;r1++)
		if(!delrec[r1])//该行没有被删除
		{
			m=0;
			for(r2=r1+1;r2<RecCount;r2++)
				if(!delrec[r2]) 
				{
					RecEqual=true;//两行相等
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
					if(RecEqual)//该两行相等
					{
						delrec[r2]=true;
						m++;
					}
				}//r2循环结束
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
	bool RecEqual;//行相等标志，true相等，初值
	int r1,r2;//行循环变量
	int a1;//列循环变量
	delrec=new bool[RecCount];
	for(r1=0;r1<RecCount;r1++)
		delrec[r1]=false;
	for(r1=0;r1<RecCount-1;r1++)
		if(!delrec[r1])//该行没有被删除
			for(r2=r1+1;r2<RecCount;r2++)
				if(!delrec[r2]) 
				{
					RecEqual=true;//两行相等
					for(a1=0;a1<AttCount;a1++)
						{
							if(tab[r1][a1]!=tab[r2][a1])
							{
								RecEqual=false;
								break;
							}
						}
					if(RecEqual)//该两行相等
						delrec[r2]=true;//该行可以去掉
				}//r2循环结束
}
