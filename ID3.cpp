// ID3.cpp: implementation of the ID3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ID.h"
#include "ID3.h"
#include <math.h>
#include <stdio.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

//------------------------------------------------
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TreeNode *ID3::savenode=NULL;
ID3::ID3()
{

}

ID3::~ID3()
{

}

void ID3::BuildTree(TreeNode &HeadNode,int *&rebuildrefattrsplit,int *resplitlayerattr,int *&result,TreeNode **&firstnodeoflayer,int ifrebuild)
{
	int *treesplitattr,mark,marknum;
	TreeNode *CurrentNode;//当前处理的结点
	TreeNode *arrayhead,*arrayend;//队列的头、尾指针
	CString *splitarray,*splitdecisionarray;//分割结点时存放条件属性值和决策属性值
	int i,j,i1,j1,i2,j2,qq;//普通计数变量
	int *splitarrayvaluenum;
	//int *treesplitattr;//调整各个结点的splitattr所设计的数组，大小为HeadNode.tablewidth-1
	int deallayer=-1;
	int samelayersplitattr=-1;
	treesplitattr=new int [HeadNode.tablewidth-1];
	for(i=0;i<HeadNode.tablewidth-1;i++)
		treesplitattr[i]=-1;
	arrayhead=arrayend=&HeadNode;
	arrayhead->splitedattr=new int [HeadNode.tablewidth-1];
	for(i=0;i<HeadNode.tablewidth-1;i++)
		arrayhead->splitedattr[i]=-1;
	do
	{
		CurrentNode=arrayhead;
		if(arrayhead->decisionattrvalue==" ")
			CurrentNode->splitattr=ChooseAttr(CurrentNode->table,CurrentNode->tablelength,CurrentNode->tablewidth);
		if(arrayhead->decisionattrvalue==" ")
		{
			//按照CurrentNode->splitattr来分配son
			if((CurrentNode->splitattr!=-1)&&(CurrentNode->tablewidth>1))
			{
				splitarray=new CString[CurrentNode->tablelength];
				splitdecisionarray=new CString[CurrentNode->tablelength];	
				splitarrayvaluenum=new int[CurrentNode->tablelength];
				for(i=0;i<CurrentNode->tablelength;i++)
				{
					splitarray[i]="";
					splitdecisionarray[i]="";
					splitarrayvaluenum[i]=0;
				}
				i=0;
				for(i=0;i<CurrentNode->tablelength;i++)
				{
					j=0;
					while((splitarray[j]!="")&&(CurrentNode->table[i][CurrentNode->splitattr]!=splitarray[j]))
						j++;
					if(splitarray[j]=="")//表示加入新的分割属性值
						splitarray[j]=CurrentNode->table[i][CurrentNode->splitattr];
					splitarrayvaluenum[j]++;
					if(splitdecisionarray[j]=="")//表示加入新的分割决策值
						splitdecisionarray[j]=CurrentNode->table[i][CurrentNode->tablewidth-1];
					else//表示决策值出现冲突
						if(splitdecisionarray[j]!=CurrentNode->table[i][CurrentNode->tablewidth-1])
						{
							if(CurrentNode->layer<HeadNode.tablewidth-2)
								splitdecisionarray[j]=" ";
							else
								splitdecisionarray[j]=CurrentNode->table[i][CurrentNode->tablewidth-1];
						}
				}
				//分配完毕，准备连接结点
				j=0;
				while((j<CurrentNode->tablelength)&&(splitarray[j]!=""))
					j++;
				CurrentNode->sonnumber=j;
				CurrentNode->son=new struct TreeNode [j];
				for(i=0;i<j;i++)//给当前结点链接子结点
				{
					//CurrentNode->son[i].conditionattr=CurrentNode->splitattr;
					CurrentNode->son[i].conditionattrvalue=splitarray[i];					
					CurrentNode->son[i].layer=CurrentNode->layer+1;
					CurrentNode->son[i].tablelength=splitarrayvaluenum[i];
					CurrentNode->son[i].tablewidth=CurrentNode->tablewidth-1;
					CurrentNode->son[i].splitedattr=new int [HeadNode.tablewidth-1];
					for(qq=0;qq<HeadNode.tablewidth-1;qq++)
						CurrentNode->son[i].splitedattr[qq]=CurrentNode->splitedattr[qq];
					if(splitdecisionarray[i]!="")
						CurrentNode->son[i].decisionattrvalue=splitdecisionarray[i];
					if(CurrentNode->son[i].decisionattrvalue==" ")
					{
						CurrentNode->son[i].table=new CString *[splitarrayvaluenum[i]];
						for(j1=0;j1<splitarrayvaluenum[i];j1++)
						{
							CurrentNode->son[i].table[j1]=new CString [CurrentNode->tablewidth-1];
						}
						i2=0;j2=0;
						for(i1=0;i1<CurrentNode->tablelength;i1++)
						{
							if(CurrentNode->table[i1][CurrentNode->splitattr]==CurrentNode->son[i].conditionattrvalue)
							{
								j2=0;
								for(j1=0;j1<CurrentNode->tablewidth;j1++)
								{
									if(j1!=CurrentNode->splitattr)
									{
										CurrentNode->son[i].table[i2][j2]=CurrentNode->table[i1][j1];
										j2++;
									}
								}
								i2++;
							}
						}
						CurrentNode->son[i].splitattr=1;
					}
					else
						CurrentNode->son[i].splitattr=-1;
				}
				for(i=0;i<j;i++)
				{
					if(arrayhead==NULL)
					{
						arrayhead=&CurrentNode->son[i];
						arrayend=&CurrentNode->son[i];
					}
					else
					{
						CurrentNode->son[i].next=arrayend->next;
						arrayend->next=&CurrentNode->son[i];
						arrayend=&CurrentNode->son[i];
					}
				}						
			}
			else
			{
				CurrentNode->son=NULL;
				if(CurrentNode->decisionattrvalue==" ")
					if(CurrentNode->tablewidth==1)
						CurrentNode->decisionattrvalue=CurrentNode->table[0][0];
					else
						CurrentNode->decisionattrvalue="conflict";
				CurrentNode->sonnumber=0;
				CurrentNode->splitattr=-1;
				CurrentNode->tablelength=0;
				CurrentNode->tablewidth=0;
				result[arrayhead->layer]++;
			}
			//把子结点都推入array中
		}
		else
		{			
			arrayhead->son=NULL;
			arrayhead->sonnumber=0;
			result[arrayhead->layer]++;
		}		
		if(arrayhead->decisionattrvalue==" ")
		{
			for(i=0;i<CurrentNode->sonnumber;i++)
				if(ifrebuild==0)
					CurrentNode->son[i].conditionattr=CurrentNode->splitattr;
				else
					CurrentNode->son[i].conditionattr=rebuildrefattrsplit[CurrentNode->layer];/////////////////////////////////
			delete[] splitarray;
			delete[] splitdecisionarray;
			delete[] splitarrayvaluenum;
			if(CurrentNode!=&HeadNode)
			{
				for(i=0;i<CurrentNode->tablelength;i++)
					delete[] CurrentNode->table[i];
				delete[] CurrentNode->table;			
			}
		}
		if((ifrebuild==1)&&(rebuildrefattrsplit[CurrentNode->layer]!=-1))
			CurrentNode->splitattr=rebuildrefattrsplit[CurrentNode->layer];
		//调整CurentNode->splitattr为相对于整个表的
		if(CurrentNode->splitattr!=-1)
		{
			marknum=-1;
			for(j=0;j<HeadNode.tablewidth-1;j++)
			{
				mark=0;
				for(i=0;i<HeadNode.tablewidth-1;i++)
				{
					if(j==CurrentNode->splitedattr[i])
					{
						mark=1;
					}
					if(CurrentNode->splitedattr[i]==-1)
						i=HeadNode.tablewidth;				
				}
				if(mark==0) marknum++;
				if(marknum==CurrentNode->splitattr)
				{
					CurrentNode->splitattr=j;
					j=HeadNode.tablewidth;
				}
			}
		}
		for(i=0;i<CurrentNode->sonnumber;i++)
			CurrentNode->son[i].splitedattr[CurrentNode->layer]=CurrentNode->splitattr;
		arrayhead=arrayhead->next;
		i++;
	}
	while(arrayhead!=arrayend->next);
	for(i=0;i<HeadNode.tablewidth;i++)
			if(firstnodeoflayer[i]!=NULL)
				rebuildrefattrsplit[i]=firstnodeoflayer[i]->splitattr;
	//if(ifrebuild==0)
	//	MessageBox(0,"建树成功！！", "提示", MB_OK);
	return;
}

void ID3::Incrementalstudy(TreeNode &HeadNode, CString **object,int studynum,int *&rebuildrefattrsplit,int *&result,TreeNode **&firstnodeoflayer)
{
	
	TreeNode *CurrentNode,*son,*fson,*addnode;
	int i,j,p,attr,i1,i2,tempattr,savei1;
	int *treesplitattr;
	int *resplitlayerattr1;
	int *splitlayerattr,splitlayerattrnum;
	splitlayerattr=new int [HeadNode.tablewidth-1];
	treesplitattr=new int [HeadNode.tablewidth-1];
	p=0;
	for(i=0;i<studynum;i++)//添加的记录条数
	{
		for(j=0;j<HeadNode.tablewidth-1;j++)
			treesplitattr[j]=-1;
		for(splitlayerattrnum=0;splitlayerattrnum<HeadNode.tablewidth-1;splitlayerattrnum++)
			splitlayerattr[splitlayerattrnum]=-1;	
		splitlayerattrnum=0;
		CurrentNode=&HeadNode;
		son=&CurrentNode->son[0];
		if((CurrentNode->sonnumber==1)&&(son->decisionattrvalue!=" "))//表示根结点就是叶结点
		{
			if(CurrentNode->decisionattrvalue!=object[i][CurrentNode->tablewidth-1])
			{
				ReadTable(object,HeadNode,*CurrentNode,object[i],splitlayerattr,i);;
				result[0]=0;
				BuildTree(*CurrentNode,rebuildrefattrsplit,resplitlayerattr1,result,firstnodeoflayer,1);

			}
		}
		else
		{
			for(j=0;j<HeadNode.tablewidth-1;j++)//表示添加某条记录中的各个属性
			{
				treesplitattr[CurrentNode->splitattr]=1;
				splitlayerattr[splitlayerattrnum++]=CurrentNode->splitattr;
				son=&CurrentNode->son[0];
				fson=CurrentNode;
				p=0;
				while((p<CurrentNode->sonnumber)&&(son!=NULL)&&(son->conditionattrvalue!=object[i][CurrentNode->splitattr]))
				{
					fson=son;
					son=son->next;				
					p++;
				}
				if(p<CurrentNode->sonnumber)
				{
					if((son->decisionattrvalue!=" ")&&(son->decisionattrvalue!="conflict"))
					{
						if(son->decisionattrvalue!=object[i][HeadNode.tablewidth-1])
						{
							if(j!=HeadNode.tablewidth-2)
							{
								treesplitattr[son->splitattr]=1;
								splitlayerattr[splitlayerattrnum++]=son->splitattr;
								result[son->layer]--;
								son->decisionattrvalue=" ";
								if(son->splitattr==-1)
								{
									i1=0;
									while((treesplitattr[i1]!=-1)&&(i1<HeadNode.tablewidth-1))
										i1++;
									if(i1<HeadNode.tablewidth-1)
										son->splitattr=i1;
								}
								ReadTable(object,HeadNode,*son,object[i],splitlayerattr,i);
								resplitlayerattr1=new int [HeadNode.tablewidth];
								for(i1=0;i1<HeadNode.tablewidth;i1++)
									resplitlayerattr1[i1]=-1;
								savei1=i1=son->layer;
								while((i1<HeadNode.tablewidth-1)&&(rebuildrefattrsplit[i1]!=-1))////////////////////////////////
								{
									tempattr=attr=rebuildrefattrsplit[i1];
									for(i2=0;i2<savei1;i2++)
										if(rebuildrefattrsplit[i2]<tempattr)
											attr--;
									resplitlayerattr1[i1]=attr;
									i1++;
								}
								resplitlayerattr1[HeadNode.tablewidth-1]=-1;
								BuildTree(*son,rebuildrefattrsplit,resplitlayerattr1,result,firstnodeoflayer,1);
								delete[] resplitlayerattr1;
								j=HeadNode.tablewidth;//表示该记录添加完毕
							}	
							else
							{
								if(son->decisionattrvalue!="conflict")
								{
									//son->decisionattrvalue="conflict";
									son->decisionattrvalue=object[i][HeadNode.tablewidth-1];
									result[son->layer]++;
								}
							}
						}
						else
							j=HeadNode.tablewidth;//表示该记录添加完毕
					}
					else
						if(son->decisionattrvalue=="conflict")
							j=HeadNode.tablewidth;
						else
							CurrentNode=son;
				}
				else
				{
					TreeNode *savenode=fson;
					addnode=new TreeNode;
					for(i1=j;i1<HeadNode.tablewidth-2;i1++)
					{
						addnode->tablelength=1;
						addnode->tablewidth=CurrentNode->tablewidth-1;
						addnode->sonnumber=0;
						if((rebuildrefattrsplit[CurrentNode->layer+1]!=-1)&&(CurrentNode->layer+1<HeadNode.tablewidth-1))
							addnode->splitattr=rebuildrefattrsplit[CurrentNode->layer+1];
						else
						{
							i2=0;
							while((treesplitattr[i2]!=-1)&&(i2<HeadNode.tablewidth-1))
								i2++;
							if(i2<HeadNode.tablewidth-1)
								addnode->splitattr=i2;
						}
						addnode->conditionattr=CurrentNode->splitattr;
						addnode->conditionattrvalue=object[i][CurrentNode->splitattr];
						addnode->decisionattrvalue=" ";
						if((fson!=NULL)&&(fson->next!=NULL))
							addnode->next=fson->next;
						else
							addnode->next=NULL;
						addnode->layer=CurrentNode->layer+1;
						if(addnode->splitattr!=-1)
							treesplitattr[addnode->splitattr]=1;
						if(fson!=NULL)
							fson->next=addnode;
						CurrentNode->sonnumber++;
						CurrentNode=addnode;
						addnode->son=new TreeNode [1];
						addnode=&addnode->son[0];
						fson=NULL;								
					}
					CurrentNode->sonnumber=1;
					addnode->tablelength=1;
					addnode->tablewidth=1;
					addnode->son=NULL;
					addnode->next=NULL;
					addnode->splitattr=-1;
					addnode->sonnumber=0;
					addnode->conditionattr=CurrentNode->splitattr;
					addnode->conditionattrvalue=object[i][CurrentNode->splitattr];
					addnode->decisionattrvalue=object[i][HeadNode.tablewidth-1];
					addnode->layer=CurrentNode->layer+1;
					j=HeadNode.tablewidth;//表示该记录添加完毕
					result[HeadNode.tablewidth-1]++;
				}
			}		
		}
	}
	//MessageBox(0,"新实例学习成功！！", "提示", MB_OK);
	return;
}
void ID3::ReadTable(CString **studytable,TreeNode &HeadNode,TreeNode &son, CString *object,int *splitlayerattr,int studyno)
{
	int i,j,p,mark;
	int tablelength,tablewidth;
	int *remainattr;
	tablelength=HeadNode.tablelength;
	tablewidth=HeadNode.tablewidth;
	remainattr=new int [tablewidth-1];
	for(i=0;i<tablewidth-1;i++)
		remainattr[i]=-1;
	j=0;
	int *tablemark;
	tablemark=new int[HeadNode.tablelength+studyno+1];
	for(i=0;i<HeadNode.tablelength+studyno+1;i++)
		tablemark[i]=0;
	for(i=0;i<tablewidth-1;i++)//查找剩下没有划分的属性
	{
		p=0;
		mark=1;
		while((p<tablewidth-1)&&(mark==1)&&(splitlayerattr[p]!=-1))
		{
			if(i==splitlayerattr[p])
				mark=0;
			p++;
		}
		if(mark==1)
		{
			remainattr[j]=i;
			j++;
		}
	}
	son.tablewidth=j+1;
	son.tablelength=0;
	for(i=0;i<tablelength;i++)
	{
		j=0;
		mark=1;
		while((j<tablewidth-1)&&(splitlayerattr[j]!=-1)&&(mark==1))
		{
			if(HeadNode.table[i][splitlayerattr[j]]!=object[splitlayerattr[j]])
				mark=0;
			j++;
		}
		if(mark==1)
		{
			son.tablelength++;
			tablemark[i]=1;
		}
	}
	for(i=0;i<studyno+1;i++)
	{
		j=0;
		mark=1;
		while((j<tablewidth-1)&&(splitlayerattr[j]!=-1)&&(mark==1))
		{
			if(studytable[i][splitlayerattr[j]]!=object[splitlayerattr[j]])
				mark=0;
			j++;
		}
		if(mark==1)
		{
			tablemark[HeadNode.tablelength+i]=1;
			son.tablelength++;
		}
	}
	son.table=new CString* [son.tablelength];
	for(i=0;i<son.tablelength;i++)
	{
		son.table[i]=new CString[son.tablewidth];
	}
	int i1=0;
	for(i=0;i<HeadNode.tablelength+studyno+1;i++)
	{
		if(i<HeadNode.tablelength)
		{
			j=0;
			if(tablemark[i]==1)
			{
				j=0;
				while((i1<son.tablelength)&&(j<son.tablewidth-1)&&(remainattr[j]!=-1))
				{
					son.table[i1][j]=HeadNode.table[i][remainattr[j]];
					j++;
				}
				son.table[i1][son.tablewidth-1]=HeadNode.table[i][tablewidth-1];
				i1++;
			}
		}
		else
		{
			if(tablemark[i]==1)
			{
				j=0;
				while((i1<son.tablelength)&&(j<son.tablewidth-1)&&(remainattr[j]!=-1))
				{
					son.table[i1][j]=studytable[i-HeadNode.tablelength][remainattr[j]];
					j++;
				}
				son.table[i1][son.tablewidth-1]=studytable[i-HeadNode.tablelength][tablewidth-1];
				i1++;
			}
		}
	}
}

int ID3::ChooseAttr(CString **table, int tablelength, int tablewidth)
{
	CString **temptable;
	double *E;
	int *attrnum;
	int **posnum;
	double a;
	int i,j,i1,j1,i2;
	double result;
	temptable=new CString* [tablelength];
	for(i=0;i<tablelength;i++)
	{
		temptable[i]=new CString[tablewidth];
		for(j=0;j<tablewidth;j++)
			temptable[i][j]=" ";
	}
	attrnum=new int[tablewidth];
	for(i=0;i<tablewidth;i++)
		attrnum[i]=0;
	E=new double[tablewidth-1];
	for(j=0;j<tablewidth;j++)
		for(i=0;i<tablelength;i++)
		{
			i1=0;
			while((temptable[i1][j]!=" ")&&(temptable[i1][j]!=table[i][j]))
				i1++;
			if(temptable[i1][j]==" ")
				temptable[i1][j]=table[i][j];
		}
	for(j=0;j<tablewidth;j++)
	{
		i=0;
		while((i<tablelength)&&(temptable[i][j]!=" "))
			i++;
		attrnum[j]=i;
	}
	float all;
	int p1,q1;
	for(j=0;j<tablewidth-1;j++)
	{
		posnum=new int* [attrnum[j]];//posnum的纵坐标表示某个属性的各个取值，横坐标为决策值的各个取值，其内容为计数器
		for(i=0;i<attrnum[j];i++)
		{
			posnum[i]=new int[attrnum[tablewidth-1]];
		}
		for(i=0;i<attrnum[j];i++)
			for(j1=0;j1<attrnum[tablewidth-1];j1++)
				posnum[i][j1]=0;
		for(i=0;i<tablelength;i++)//查找当前记录在posnum中对应哪个计数器
		{
			i1=0;i2=0;
			while(table[i][j]!=temptable[i1][j])
				i1++;
			while(table[i][tablewidth-1]!=temptable[i2][tablewidth-1])
				i2++;
			posnum[i1][i2]++;
		}
		result=0;
		for(p1=0;p1<attrnum[j];p1++)
		{
			all=0;
			for(q1=0;q1<attrnum[tablewidth-1];q1++)
				all=all+posnum[p1][q1];
			for(q1=0;q1<attrnum[tablewidth-1];q1++)
			{
				a=posnum[p1][q1]/all;
				if(a>0)
					a=log(a)/log(2);
				else
					a=0;
				a=posnum[p1][q1]*a;
				result=result-a;
			}
		}
		E[j]=result;
		for(i=0;i<attrnum[j];i++)
		{
			delete[] posnum[i];
		}
		delete[] posnum;
	}
	double min=999999999999999;
	int mini=0;
	for(j=0;j<tablewidth-1;j++)
	{
		if(E[j]<min) 
		{
			min=E[j];
			mini=j;
		}
	}
	for(i=0;i<tablelength;i++)
	{
		delete[] temptable[i];		
	}
	delete[] temptable;
	delete[] attrnum;
	delete[] E;
	return mini;	
}

void ID3::OutPutTree(TreeNode HeadNode,int *result,CString filename)
{
	FILE *fp;
	int i;
	if(filename=="")
		filename="规则统计.TXT";
	if((fp=fopen(filename,"w"))==NULL)
	{
		MessageBox(0,"不能打开输出文件outputtree.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	float averagerulelength=0;
	float rulenum=0;
	fprintf(fp,"条件属性个数\t规则个数\n");
	for(i=0;i<HeadNode.tablewidth;i++)
	{
		fprintf(fp,"%d\t\t   %d\n",i,result[i]);
		if(result[i]!=0)
		{
			rulenum=rulenum+result[i];
			averagerulelength=averagerulelength+i*result[i];
		}
	}
	fprintf(fp,"规则平均长度为%f\n",averagerulelength/rulenum);
	fprintf(fp,"规则总数为%.0f\n",rulenum);
	fclose(fp);

	//MessageBox(0,"输出成功！！", "提示", MB_OK);
}

void ID3::transform(CString filename1,CString filename2)
{
	FILE *fp1,*fp2;
	int *result;
	int i,j,i1,tablelength,tablewidth;
	char a[20];
	if(filename1=="") filename1="规则.txt";
	if(filename2=="") filename1="规则统计.txt";
	if((fp1=fopen(filename1,"r"))==NULL)
	{
		MessageBox(0,"不能打开", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	if((fp2=fopen(filename2,"w"))==NULL)
	{
		MessageBox(0,"不能打开转换输出文件", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp1,"%d",&tablelength);//行数
	fscanf(fp1,"%d",&tablewidth);//列数
	result=new int [tablewidth];
	for(i=0;i<tablewidth;i++)
		result[i]=0;
	for(i=0;i<tablelength;i++)
	{
		i1=tablewidth-1;
		for(j=0;j<tablewidth;j++)
		{
			fscanf(fp1,"%s",a);
			if(a[0]=='-')
				i1--;
		}
		result[i1]++;
	}
	float averagerulelength=0;
	float rulenum=0;
	fprintf(fp2,"条件属性个数\t规则个数\n");
	for(i=0;i<tablewidth;i++)
	{
		fprintf(fp2,"%d\t\t   %d\n",i,result[i]);
		if(result[i]!=0)
		{
			rulenum=rulenum+result[i];
			averagerulelength=averagerulelength+i*result[i];
		}
	}
	fprintf(fp2,"规则平均长度为%f\n",averagerulelength/rulenum);
	fprintf(fp2,"规则总数为%.0f\n",rulenum);
	fclose(fp1);
	fclose(fp2);
	//MessageBox(0,"输出成功！！", "提示", MB_OK);
	
	
}

void ID3::writhead()
{
	FILE *fp;
	int i;
	if((fp=fopen("temp.TXT","w"))==NULL)
	{
		MessageBox(0,"不能打开temp.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	for(i=0;i<279;i++)
	{
		fprintf(fp,"A  ");
	}

	for(i=0;i<279;i++)
	{
		fprintf(fp,"Integer  ");
	}
	fclose(fp);
}

void ID3::test(TreeNode &HeadNode, CString **object, int studynum)
{
	TreeNode *CurrentNode,*son,*fson;
	int i,j,p;
	int correctdis=0,wrongdis=0,cannotdis=0;
	for(i=0;i<studynum;i++)//添加的记录条数
	{
		CurrentNode=&HeadNode;
		son=&CurrentNode->son[0];
		if((CurrentNode->sonnumber==1)&&(son->decisionattrvalue!=" "))//表示根结点就是叶结点
		{
			if(CurrentNode->decisionattrvalue!=object[i][CurrentNode->tablewidth-1])
			{
				wrongdis++;
			}
			else
				correctdis++;
		}
		else
		{
			for(j=0;j<HeadNode.tablewidth-1;j++)//表示添加某条记录中的各个属性
			{
				son=&CurrentNode->son[0];
				fson=NULL;
				p=0;
				while((p<CurrentNode->sonnumber)&&(son!=NULL)&&(son->conditionattrvalue!=object[i][CurrentNode->splitattr]))
				{
					fson=son;
					son=son->next;				
					p++;
				}
				if(p<CurrentNode->sonnumber)
				{
					if((son->decisionattrvalue!=" ")&&(son->decisionattrvalue!="conflict"))
					{
						if(son->decisionattrvalue!=object[i][HeadNode.tablewidth-1])
							wrongdis++;
						else
							correctdis++;
						j=HeadNode.tablewidth;//表示该记录添加完毕
					}
					else
						if(son->decisionattrvalue=="conflict")
						{
							cannotdis++;
							j=HeadNode.tablewidth;
						}
						else
							CurrentNode=son;
				}
				else
				{
					cannotdis++;
					j=HeadNode.tablewidth;
				}
			}		
		}
	}
	FILE *fp;
	float all,each;
	all=correctdis+wrongdis+cannotdis;
	if((fp=fopen("样本测试结果.TXT","w"))==NULL)
	{
		MessageBox(0,"不能打开 样本测试结果文件.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	else
	{
		each=correctdis;
		fprintf(fp,"正确识别率:%.1f%%\n",(each/all)*100);
		each=wrongdis;
		fprintf(fp,"错误识别率:%.1f%%\n",(each/all)*100);
		each=cannotdis;
		fprintf(fp,"未识别率:%.1f%%\n",(each/all)*100);
	}
	fclose(fp);
	return;
}
