 // ValBase.cpp: implementation of the ValBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ValBase.h"
#include"string.h"
#include"stdlib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int ValBase::Read_Head()//right return 1,else return -1;
{
	char tempstring[60];
	int i;				//循环变量
	fin.getline(tempstring,30,':');
	if(strcmp(tempstring,"Style")!=0)
		return -1;
	fin.getline(style,30,'\n');
	fin.getline(tempstring,30,':');
	if(strcmp(tempstring,"Stage")!=0)
		return -1;
	fin>>stage;				//读入stage
	fin.getline(tempstring,35,':');
	if(strcmp(tempstring,"\nCondition attributes number")!=0)
		return -1;
	fin>>AttCount;			//读入列数
	AttCount++;				//实际的列数比条件属性多1；

	fin.getline(tempstring,60,':');
	if(strcmp(tempstring,"\nThe Number of Condition attributes deleted")==0)
	{
		int n;
		fin>>atbcount;
		fin.getline(tempstring,60,':');
		if(strcmp(tempstring,"\nThe position of Condition attributes deleted")==0)
		{
			if(atbcount>0)
			{
				atb=new int[atbcount];
				for(n=0;n<atbcount;n++)
					fin>>atb[n];
			}
			else
				atb=NULL;
		}
		else
			return -1;
	}
	else
		return -1;

	fin.getline(tempstring,30,':');
	if(strcmp(tempstring,"\nRecords number")!=0)
		return -1;
	fin>>RecCount;			//读入行数
	datasign=new char *[AttCount];//开辟AttCount(列数)个单元,存储类型标志符
	for(i=0;i<AttCount;i++)
		datasign[i]=new char[20];
	for(i=0;i<AttCount;i++)		//读入类型标志符
		fin>>datasign[i];
	datatype=new int[AttCount];//存储数据类型(1表示int,2为float,3为string)
	for(i=0;i<AttCount;i++)
	{
		fin>>tempstring;
		switch (tempstring[0])
		{
		case 'i':
		case 'I':datatype[i]=1;break;
		case 'f':
		case 'F':datatype[i]=2;break;
		case 's':
		case 'S':datatype[i]=3;break;
		}
	}
	return 1;
}

int ValBase::Read_Data()
{
	char tempstring[20];
	int i,j;
	tabstr=new char **[RecCount];
   	if (tabstr==NULL) return -1;
	for(i=0;i<RecCount;i++)
		tabstr[i]=NULL;
	for(i=0;i<RecCount;i++)
	{
		tabstr[i]=new char *[AttCount];
		if(tabstr[i]==NULL) return -1;
		for(j=0;j<AttCount;j++)
			tabstr[i][j]=NULL;
		for(j=0;j<AttCount;j++)
		{
			fin>>tempstring;
			tabstr[i][j]=new char [strlen(tempstring)+1];
			if(tabstr[i][j]==NULL) return -1;
			strcpy(tabstr[i][j],tempstring);
		}
	}
	return 1;
}

int ValBase::Transfer_Tab()
{
	int i,j,k;		//循环变量
	tab=new int *[RecCount];//建立RecCount*AttCount的表
	if(tab==NULL) return -1;
	for(i=0;i<RecCount;i++)
		tab[i]=NULL;
	AttCount1=AttCount;
	delatt=new bool [AttCount1];
	for(j=0;j<AttCount1;j++)
		if(strcmp(tabstr[0][j],"-")==0)
		{
			delatt[j]=true;
			AttCount--;
		}
		else delatt[j]=false;
	for(i=0;i<RecCount;i++)
	{
		tab[i]=new int [AttCount];
		if(tab[i]==NULL) return -1;
		k=0;
		for(j=0;j<AttCount1;j++)		//按列循环
		{
			if(!delatt[j])
				tab[i][k++]=atoi(tabstr[i][j]);
			delete []tabstr[i][j];
		}
		delete []tabstr[i];
	}
	delete tabstr;
	return 1;
}

int ValBase::Read_Cut()
{
	char sign[100];
	char temp[20];
	int x,y;
	int i;
	if((cuttab=new MyCut *[AttCount1])==NULL)
		return -1;
	for(i=0;i<AttCount1;i++)
		cuttab[i]=NULL;
	if(fin.eof()) return 1;
	fin.getline(sign,100,']');
	while(!fin.eof())
	{
		temp[0]='!';
		fin>>temp;
		if(temp[0]!='!')
		{
			x=atoi(temp);
			fin>>y;
			if((cuttab[x]=new MyCut[y])==NULL)
				return -1;
			for(i=0;i<y;i++)
			{
				fin>>cuttab[x][i].string;
				fin>>cuttab[x][i].x;
			}
		}
	}
	return 1;
}
ValBase::ValBase()
{

}

ValBase::~ValBase()
{

}

int ValBase::RunOne(char *s)
{
	//strcpy(s,"data.txt");
	fin.open("data.txt",ios::in|ios::nocreate);
	if(fin.fail())
	{
		fin.close();
		return -1;
	}
	if(Read_Head()!=1)
	{
		fin.close();
		return -1;
	}
	if(Read_Data()!=1)
	{
		fin.close();
		return -1;
	}
	if(Transfer_Tab()!=1)
	{
		fin.close();
		return -1;
	}
	if(Read_Cut()!=1)
	{
		fin.close();
		return -1;
	}

	fin.close();
	return 1;
}

void ValBase::Save_File(char *r)
{
	fout.open("规则.txt",ios::out|ios::trunc);
	int i,j,m,k;//循环变量
	int tempcount=0;
	//输出文件头
	fout<<"Style:rule"<<'\n';		//表示为规则文件
	fout<<"Stage:0"<<'\n';			//为规则文件这个值无用
	fout<<"Condition attributes number:"<<AttCount1-1<<'\n';//列数
	/////////////////插进
    fout<<"The Number of Condition attributes deleted: "<<atbcount<<'\n';
	fout<<"The position of Condition attributes deleted: ";
//	fout<<atbcount<<'\n';
	for(i=0;i<atbcount;i++)
		fout<<atb[i]<<' ';
	fout<<'\n';
//////////
	for(i=0;i<RecCount;i++)//除去冗余记录个数
		if(!delrec[i]) tempcount++;
	fout<<"Rules number:"<<tempcount<<'\n';//行数
    fout<<"Blocks number:0\n";//块数
	for(j=0;j<AttCount1;j++)
		fout<<datasign[j]<<' ';
	fout<<'\n';
	for(j=0;j<AttCount1;j++)
	{
		switch (datatype[j])
		{
		case 1:fout<<"Integer"<<' ';break;
		case 2:fout<<"Float"<<' ';break;
		case 3:fout<<"String"<<' ';break;
		}
	}
	fout<<'\n';
	//输出正文
	for(i=0;i<RecCount;i++)
	{
		if (!delrec[i])
		{
			k=0;
			for(j=0;j<AttCount1;j++)
			{
				if(delatt[j])
				{
					fout<<'-'<<' ';
					continue;
				}
				if(!flagtab[i][k])
				{
					if(cuttab[j]!=NULL)
					{
						for(m=0;m<RecCount;m++)
							if(cuttab[j][m].x==(tab[i][k]))
							{
								fout<<cuttab[j][m].string<<' ';
								break;
							}
					}
					else
						fout<<tab[i][k]<<' ';
				}

				else 
					fout<<'-'<<' ';
				k++;
			}
			fout<<1.0<<' ';
			fout<<same_rec[i]+1<<' ';
			fout<<same_rec[i]+1<<' ';
			fout<<'\n';
		}
	}

//写入文件尾部	while(!fin)
fout.close();

}

void ValBase::ReadReductionTable(CString **table, int &tablelength, int tablewidth)
{
	int i,j;				//循环变量
	stage=3;				//读入stage
	AttCount=tablewidth;			//读入列数
	atbcount=0;
	atb=NULL;
	RecCount=tablelength;			//读入行数
	datasign=new char *[AttCount];//开辟AttCount(列数)个单元,存储类型标志符
	for(i=0;i<AttCount;i++)
		datasign[i]=new char[20];
	for(i=0;i<AttCount;i++)		//读入类型标志符
		datasign[i]="A";
	datatype=new int[AttCount];//存储数据类型(1表示int,2为float,3为string)
	for(i=0;i<AttCount;i++)
	{
		datatype[i]=3;		
	}

	tabstr=new char **[RecCount];
	for(i=0;i<RecCount;i++)
		tabstr[i]=NULL;
	for(i=0;i<RecCount;i++)
	{
		tabstr[i]=new char *[AttCount];
		for(j=0;j<AttCount;j++)
			tabstr[i][j]=NULL;
		for(j=0;j<AttCount;j++)
		{
			tabstr[i][j]=new char [20];
			strcpy(tabstr[i][j],table[i][j].GetBuffer(1));
		}
	}
	return;
}
