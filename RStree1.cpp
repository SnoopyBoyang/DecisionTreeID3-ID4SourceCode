// RStree1.cpp: implementation of the CRStree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ID.h"
#include "RStree1.h"
#include "IDDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int *CRStree::rulelength;
int * CRStree::orderofattr=NULL;
int CRStree::ifBestRulePath=0;
CString ** CRStree::table=NULL;
int CRStree::tablelength=0;
CString ** CRStree::matchtable=NULL;
int CRStree::matchtablelength=0;
int CRStree::tablewidth=0;
CString ** CRStree::datatable=NULL;
int CRStree::datatablelength=0;
int CRStree::datatablewidth=0;
CString ** CRStree::studytable=NULL;
int CRStree::studytablelength=0;
int CRStree::studytablewidth=0;
TreeNode CRStree::RSHeadNode;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRStree::CRStree()
{

}

CRStree::~CRStree()
{

}

int CRStree::AddRuletoTree(CString *object)
{
	TreeNode *CurrentNode,*fson,*son,*tempnext;
	int i,j;
	CurrentNode=&RSHeadNode;
	for(i=0;i<tablewidth-1;i++)
	{
		if((CurrentNode->son==NULL))
		{
			CurrentNode->son=new TreeNode;
			//CurrentNode=CurrentNode->son;
			CurrentNode->son->splitattr=orderofattr[i+1];
			CurrentNode->sonnumber=1;
			CurrentNode->next=CurrentNode->son;
			CurrentNode->son->next=NULL;
			CurrentNode->son->layer=i+1;
			CurrentNode->son->son=NULL;
			CurrentNode->son->sonnumber=0;
			CurrentNode->son->conditionattr=CurrentNode->splitattr;
			CurrentNode->son->conditionattrvalue=object[CurrentNode->splitattr];
			if(i==tablewidth-2)
			{
				CurrentNode->son->decisionattrvalue=object[tablewidth-1];
				///////////////////
				CurrentNode->son->matchnumber=datatablelength+studytablelength;
				//CurrentNode->son->matchnumber=1;
			}
			else
				CurrentNode->son->decisionattrvalue=" ";
			CurrentNode=CurrentNode->son;
		}
		else
		{
			son=CurrentNode->son;
			fson=CurrentNode;
			j=0;
			while((j<CurrentNode->sonnumber)&&(son->conditionattrvalue!=object[son->conditionattr]))
			{
				j++;
				fson=son;
				son=son->next;
			}
			if(j<CurrentNode->sonnumber)
			{
				if(i!=tablewidth-2)
					CurrentNode=son;
				else
					if(son->decisionattrvalue!=object[tablewidth-1])
					{
						while((son->next!=NULL)&&(son->decisionattrvalue!=object[tablewidth-1]))
							son=son->next;
						if(son->decisionattrvalue!=object[tablewidth-1])
						{
							CurrentNode->sonnumber++;
							tempnext=son->next;
							son->next=new TreeNode;
							*(son->next)=*son;
							son->next->decisionattrvalue=object[tablewidth-1];
							son->next->next=tempnext;
							/////////////////////////////////////
							son->next->matchnumber=datatablelength+studytablelength;
							//son->next->matchnumber=1;
							son=son->next;	
							return 0;
						//MessageBox(0,"规则冲突！", "警告",MB_ICONERROR | MB_OK);
						}
						else
							return 0;
					}
					else
					{
						//MessageBox(0,"规则重复！", "警告",MB_ICONERROR | MB_OK);
						return 0;
					}
			}
			else
			{
				son=fson;
				tempnext=son->next;
				son->next=new TreeNode;
				*(son->next)=*son;
				son->next->next=tempnext;
				son->next->sonnumber=0;
				son->next->son=NULL;
				son->next->conditionattrvalue=object[son->next->conditionattr];
				CurrentNode->sonnumber++;
				if(i==tablewidth-2)
				{
					son->next->decisionattrvalue=object[tablewidth-1];
					/////////////////////////////////////////
					son->next->matchnumber=datatablelength+studytablelength;
					//son->next->matchnumber=1;
				}
				else
				{				
					son=son->next;
					CurrentNode=son;
				}
			}
		}
	}
	return 1;
}

void CRStree::OrderAttr(int *&orderofattr)
{
	CString *splitarray;
	int *attrkinds,i,j,p,Max,Maxvalue;
	splitarray=new CString[tablelength];
	attrkinds=new int[tablewidth-1];
	for(i=0;i<tablewidth-1;i++)
		attrkinds[i]=0;	
	for(i=0;i<tablewidth-1;i++)
	{
		for(j=0;j<tablelength;j++)
		{
			splitarray[j]=" ";	
		}
		for(j=0;j<tablelength;j++)
		{
			p=0;
			while((p<tablelength)&&(splitarray[p]!=" ")&&(table[j][i]!=splitarray[p]))
				p++;
			if((splitarray[p]==" "))
			{
				splitarray[p]=table[j][i];
				attrkinds[i]++;
			}
		}
	}
	for(i=0;i<tablewidth-1;i++)
	{
		Max=0;Maxvalue=0;
		for(j=0;j<tablewidth-1;j++)
		{
			if(attrkinds[j]>Maxvalue)
			{
				Max=j;
				Maxvalue=attrkinds[j];
			}
		}
		orderofattr[i]=Max;
		attrkinds[Max]=-attrkinds[Max];
	}
	i=0;j=tablewidth-2;
	while(i<j)
	{
		Max=orderofattr[i];
		orderofattr[i]=orderofattr[j];
		orderofattr[j]=Max;
		i++;j--;
	}
	delete[] splitarray;
	delete[] attrkinds;
}
int markmatch=0;
void CRStree::Search1(CString *object, TreeNode *CurrentNode)
{
	TreeNode *matchson,*matcheveryson,*fson,*son;	
	int j,i;
	if((CurrentNode->layer<=tablewidth-2)&&((ifBestRulePath==0)||(markmatch==0)))
	{
		son=CurrentNode->son;
		fson=CurrentNode;
		matcheveryson=NULL;
		matchson=NULL;
		j=0;
		while((j<CurrentNode->sonnumber)&&((matchson==NULL)||(matcheveryson==NULL)))
		{
			j++;
			if(son->conditionattrvalue==object[son->conditionattr])
				matchson=son;				
			if(son->conditionattrvalue=="-")
				matcheveryson=son;
			fson=son;
			son=son->next;
		}
		if(matchson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matchson;
			if(CurrentNode->layer==tablewidth-2)
			{
				if(matchson->decisionattrvalue==object[tablewidth-1])
				{
					matchson->matchnumber++;
					if((matchson->matchnumber>maxmatchnumber)||((ifBestRulePath==1)&&(BestRulePath[tablewidth-1]->decisionattrvalue!=object[tablewidth-1])))
					{
						maxmatchnumber=matchson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;markmatch=1;
					}
				}
				else
				{
					if(ifBestRulePath==0)
					{
						maxmatchnumber=matchson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;markmatch=0;
					}
				}
			}
			else
				Search(object,matchson);
		}
		if(matcheveryson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matcheveryson;
			if(CurrentNode->layer==tablewidth-2)
			{
				if(matcheveryson->decisionattrvalue==object[tablewidth-1])
				{
					matcheveryson->matchnumber++;
					if((matcheveryson->matchnumber>maxmatchnumber)||((ifBestRulePath!=0)&&(BestRulePath[tablewidth-1]->decisionattrvalue!=object[tablewidth-1])))
					{
						maxmatchnumber=matcheveryson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;markmatch=1;
					}
				}
				else
				{
					if(ifBestRulePath==0)
					{
						maxmatchnumber=matcheveryson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;markmatch=0;
					}
				}
			}
			else
			Search(object,matcheveryson);
		}
	}
}

void CRStree::Search(CString *object, TreeNode *CurrentNode)
{
	TreeNode *matchson,*matcheveryson,*fson,*son;	
	int j,i;
	if(CurrentNode->layer<=tablewidth-2)
	{
		son=CurrentNode->son;
		fson=CurrentNode;
		matcheveryson=NULL;
		matchson=NULL;
		j=0;
		while((j<CurrentNode->sonnumber)&&((matchson==NULL)||(matcheveryson==NULL)))
		{
			j++;
			if(son->conditionattrvalue==object[son->conditionattr])
				matchson=son;				
			if(son->conditionattrvalue=="-")
				matcheveryson=son;
			fson=son;
			son=son->next;
		}
		if(matchson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matchson;
			if(CurrentNode->layer==tablewidth-2)
			{
				if(matchson->decisionattrvalue==object[tablewidth-1])
				{
					matchson->matchnumber++;
					if((matchson->matchnumber>maxmatchnumber)||((ifBestRulePath!=0)&&(BestRulePath[tablewidth-1]->decisionattrvalue!=object[tablewidth-1])))
					{
						maxmatchnumber=matchson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;
					}
				}
				else
				{
					if(ifBestRulePath==0)
					{
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;
					}
				}
			}
			else
				Search(object,matchson);
		}
		if(matcheveryson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matcheveryson;
			if(CurrentNode->layer==tablewidth-2)
			{
				if(matcheveryson->decisionattrvalue==object[tablewidth-1])
				{
					matcheveryson->matchnumber++;
					if((matcheveryson->matchnumber>maxmatchnumber)||((ifBestRulePath!=0)&&(BestRulePath[tablewidth-1]->decisionattrvalue!=object[tablewidth-1])))
					{
						maxmatchnumber=matcheveryson->matchnumber;
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;
					}
				}
				else
				{
					if(ifBestRulePath==0)
					{
						for(i=0;i<tablewidth;i++)
							BestRulePath[i]=tempRulePath[i];
						ifBestRulePath=1;
					}
				}
			}
			else
			Search(object,matcheveryson);
		}
	}
}

void CRStree::RSIncrementalStudy(int ifmethod,int ifaccelarate)
{
	int i,j;
	CString *addobject;
	addobject=new CString[tablewidth];
	for(i=0;i<studytablelength;i++)
	{
		BestRulePath=new TreeNode *[tablewidth];
		tempRulePath=new TreeNode *[tablewidth];	
		tempRulePath[0]=&RSHeadNode;
		maxmatchnumber=0;
		if(ifaccelarate==0)
			Search(studytable[i],&RSHeadNode);
		else
			Search1(studytable[i],&RSHeadNode);
		if(ifBestRulePath==0)
		{
			//if(ifmethod==1)
			//{
			//	for(j=0;j<tablewidth;j++)
			//		addobject[j]="-";
			//	addobject[orderofattr[tablewidth-2]]=studytable[i][tablewidth-2];
			//	AddRuletoTree(addobject);
			//	rulelength[1]++;			
			//}
			//else
			{
				if(AddRuletoTree(studytable[i])!=0)
					rulelength[tablewidth-1]++;
			}
		}
		else
			if(BestRulePath[tablewidth-1]->decisionattrvalue!=studytable[i][tablewidth-1])
			{
				if(ifmethod==0)
				{
					SearchMatchTable(BestRulePath,i); //查找匹配表；
					RepairTree(BestRulePath,i);
					for(j=0;j<matchtablelength;j++)
						delete[] matchtable[j];
					delete[] matchtable;
					matchtablelength=0;
				}
				else
				{
					if(AddRuletoTree(studytable[i])!=0)
						rulelength[tablewidth-1]++;
				}
			}
		delete[] BestRulePath;
		delete[] tempRulePath;
		ifBestRulePath=0;
	}
}

void CRStree::SearchMatchTable(TreeNode **RulePath, int no)
{
	int i,j,p;
	int *mark;
	CString *object;
	object=new CString [tablewidth-1];
	for(i=1;i<tablewidth;i++)
	{
		object[RulePath[i]->conditionattr]=RulePath[i]->conditionattrvalue;
	}
	mark=new int[datatablelength+no];
	for(i=0;i<datatablelength+no;i++)
		mark[i]=0;
	for(i=0;i<datatablelength;i++)
	{
		j=0;
		while((j<datatablewidth-1)&&((datatable[i][j]==object[j])||(object[j]=="-")))
			j++;
		if(j==datatablewidth-1)
		{
			mark[i]=1;
			matchtablelength++;
		}
	}
	for(i=0;i<no;i++)
	{
		j=0;
		while((j<tablewidth-1)&&((studytable[i][j]==object[j])||(object[j]=="-")))
			j++;
		if(j==tablewidth-1)
		{
			mark[datatablelength+i]=1;
			matchtablelength++;
		}
	}
	matchtable=new CString*[matchtablelength];
	for(i=0;i<matchtablelength;i++)
		matchtable[i]=new CString[tablewidth];
	p=0;
	for(i=0;i<datatablelength;i++)
	{
		if(mark[i]==1)
		{
			for(j=0;j<tablewidth;j++)
				matchtable[p][j]=datatable[i][j];
			p++;
		}
	}
	for(i=0;i<no;i++)
	{
		if(mark[datatablelength+i]==1)
		{
			for(j=0;j<tablewidth;j++)
				matchtable[p][j]=studytable[i][j];
			p++;
		}
	}
	delete[] object;
	delete[] mark;
}

void CRStree::ReadTable(CString filename)//读入规则表
{
	int i,j,eachrulelength;
	FILE *fp;
	char a[20];
	if(filename=="") filename="rule.txt";
	if((fp=fopen(filename,"r"))==NULL)
	{
		MessageBox(0,"不能打开原始表文件", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&tablelength);//行数
	fscanf(fp,"%d",&tablewidth);//列数
	table=new CString* [tablelength];
	for(i=0;i<tablelength;i++)
	{
		table[i]=new CString[tablewidth];
	}
	rulelength=new int [tablewidth];
	for(i=0;i<tablewidth;i++)
		rulelength[i]=0;
	orderofattr=new int [tablewidth-1];
	for(i=0;i<tablelength;i++)
	{
		eachrulelength=0;
		for(j=0;j<tablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			table[i][j]=a;
			if((table[i][j]!="-")&&(j!=tablewidth-1))
				eachrulelength++;
		}
		rulelength[eachrulelength]++;
	}
	fclose(fp);
}

void CRStree::ReadStudyTable(CString filename)//读入学习表
{
	int i,j;
	FILE *fp;
	char a[20];
	if(filename=="") filename="study.txt";
	if((fp=fopen(filename,"r"))==NULL)
	{
		MessageBox(0,"不能打开学习表文件", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&studytablelength);//行数
	fscanf(fp,"%d",&studytablewidth);//列数
	studytable=new CString *[studytablelength];
	for(i=0;i<studytablelength;i++)
	{
		studytable[i]=new CString[studytablewidth];
	}
	for(i=0;i<studytablelength;i++)
		for(j=0;j<studytablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			studytable[i][j]=a;
		}
	fclose(fp);

}

void CRStree::ReadDataTable(CString filename)//读入原始数据表
{
	int i,j;
	FILE *fp;
	char a[20];
	if(filename=="") filename="data.txt";
	if((fp=fopen(filename,"r"))==NULL)
	{
		MessageBox(0,"不能打开原始数据表文件", "Error", 
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

}

void CRStree::RSBuildTree()
{
	int i;
	RSHeadNode.layer=0;
	RSHeadNode.son=NULL;
	RSHeadNode.sonnumber=0;
	RSHeadNode.conditionattr=-1;
	RSHeadNode.splitattr=orderofattr[0];
	RSHeadNode.next=NULL;
	for(i=0;i<tablelength;i++)
		AddRuletoTree(table[i]);	
}

void CRStree::OutPutRSTree(CString filename)
{
	FILE *fp;
	int i;
	float averagerulelength;
	float rulenum;
	if(filename=="") filename="规则统计.txt";
	if((fp=fopen(filename,"w"))==NULL)
	{
		MessageBox(0,"不能打开输出文件 规则统计.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	averagerulelength=0;
	rulenum=0;
	fprintf(fp,"条件属性个数\t规则个数\n");
	for(i=0;i<tablewidth;i++)
	{
		fprintf(fp,"%d\t\t   %d\n",i,rulelength[i]);
		if(rulelength[i]!=0)
		{
			rulenum=rulenum+rulelength[i];
			averagerulelength=averagerulelength+i*rulelength[i];
		}
	}
	fprintf(fp,"规则平均长度为%f\n",averagerulelength/rulenum);
	fprintf(fp,"规则总数为%.0f\n",rulenum);
	fclose(fp);
	//MessageBox(0,"输出成功！！", "提示", MB_OK);
}


void CRStree::Run()
{
	ReadTable("rule.txt");
	ReadStudyTable("study.txt");
	ReadDataTable("data.txt");
	OrderAttr(orderofattr);
	RSBuildTree();
	//RSIncrementalStudy();
	ReleaseSpace();
	OutPutRSTree("规则统计.txt");
}

void CRStree::ReleaseSpace()
{
	int i;
	for(i=0;i<tablelength;i++)
		delete[] table[i];
	delete[] table;
	for(i=0;i<studytablelength;i++)
		delete[] studytable[i];
	delete[] studytable;
	for(i=0;i<datatablelength;i++)
		delete[] datatable[i];
	delete[] datatable;
	delete[] orderofattr;
}

void CRStree::Generate_Rule(TreeNode **RulePath,int studyno)
{
	int i,j,p=0,markallmatchnum=0;
	CString *studyobject;
	studyobject=studytable[studyno];
	int *markallmatch,mark;
	markallmatch=new int [tablewidth-1];
	for(p=0;p<tablewidth-1;p++)
	{
		if(RulePath[p]->conditionattrvalue=="-")
		{
			markallmatch[p]=1;
			markallmatchnum++;
		}
		else
			markallmatch[p]=0;
	}
	p=0;
	if(matchtablelength!=0)
	{
		mark=0;
		for(i=0;i<tablewidth-1;i++)
		{
			if(markallmatch[i]==1)
			{
				markallmatch[i]=0;
				if(ifconflict(studyobject,markallmatch)==0)
				{
					j=markallmatchnum;
					i=markallmatchnum;//说明没有冲突
					mark=1;
				}
				markallmatch[i]=1;
			}
		}
	}
	else
		AddRuletoTree(studyobject);

}

int CRStree::ifconflict(CString *studyobject, int *markallmatch)
{
	int i,j,mark;
	for(i=0;i<matchtablelength;i++)
	{
		mark=0;
		for(j=0;j<tablewidth-1;j++)
		{
			if(markallmatch[j]==0)
			{
				if(studyobject[j]!=matchtable[i][j])
				{
					mark=1;
					j=tablewidth;
				}
			}
		}
		if(mark==0) 
			return 1;//表示有冲突
	}
	return 0;//表示没冲突
}

void CRStree::RepairTree(TreeNode **RulePath, int studyno)
{
	int i,j,p,markallmatchnum=0;
	CString *studyobject;
	studyobject=studytable[studyno];
	int *markallmatch,*mark;
	mark=new int[tablewidth-1];
	markallmatch=new int[tablewidth-1];
	int rule_length=0;
	for(p=0;p<tablewidth-1;p++)
	{
		if(RulePath[p+1]->conditionattrvalue=="-")
		{
			markallmatch[markallmatchnum]=RulePath[p+1]->conditionattr;
			mark[RulePath[p+1]->conditionattr]=0;
			markallmatchnum++;
		}
		else
		{
			mark[RulePath[p+1]->conditionattr]=1;rule_length++;
		}
	}
	int **distinguish_attr;
	//rulelength[rule_length]--;
	distinguish_attr=new int *[matchtablelength];
	for(i=0;i<matchtablelength;i++)
		distinguish_attr[i]=new int[markallmatchnum];
	int *distinguishnum;
	distinguishnum=new int[matchtablelength];
	for(i=0;i<matchtablelength;i++)
		distinguishnum[i]=0;
	for(i=0;i<matchtablelength;i++)
	{
		for(j=0;j<markallmatchnum;j++)
		{
			if(studyobject[markallmatch[j]]!=matchtable[i][markallmatch[j]])
			{
				distinguish_attr[i][j]=1;
				distinguishnum[i]++;
			}
			else
				distinguish_attr[i][j]=0;
		}
	}

	p=0;
	for(i=0;i<matchtablelength;i++)
	{
		if(distinguishnum[i]==0)
		{
			for(j=0;j<markallmatchnum;j++)
				{
					mark[markallmatch[j]]=1;
				}
			i=matchtablelength;//跳出循环
		}
		if(distinguishnum[i]==1)
		{
			for(j=0;j<markallmatchnum;j++)
				if(distinguish_attr[i][j]==1)
				{
					mark[markallmatch[j]]=1;
					j=markallmatchnum;
				}
		}
	}
	int ifok;
	for(i=0;i<matchtablelength;i++)
	{
		if(distinguishnum[i]==0)
		{
			i=matchtablelength;//跳出循环；÷
		}
		ifok=0;
		if(distinguishnum[i]>1)
		{
			for(j=0;j<markallmatchnum;j++)
			{
				if(distinguish_attr[i][j]==1)
				{
					if(mark[markallmatch[j]]==1)
					{
						ifok=1;
						j=markallmatchnum;
					}
				}

			}
			if(ifok==0)
			{
				for(j=0;j<markallmatchnum;j++)
					if(distinguish_attr[i][j]==1)
					{
						mark[markallmatch[j]]=1;
						j=markallmatchnum;
					}
			}
		}
	}
	rule_length=0;
	for(i=0;i<tablewidth-1;i++)
		if(mark[i]==1)
			rule_length++;
	//rulelength[rule_length]=rulelength[rule_length]+matchtablelength;
	CString *add_rule;
	add_rule=new CString[tablewidth];
	for(j=0;j<tablewidth-1;j++)//添加当前学习记录
	{
		if(mark[j]==1)
			add_rule[j]=studytable[studyno][j];
		else
			add_rule[j]="-";
	}
	add_rule[tablewidth-1]=studytable[studyno][tablewidth-1];
	if(AddRuletoTree(add_rule)!=0)
		rulelength[rule_length]++;			
	//if((RulePath[i]->conditionattrvalue=="-")&&(mark[RulePath[i]->conditionattr]==1))
		//	RulePath[i]->conditionattrvalue=studyobject[RulePath[i]->conditionattr];
	for(i=0;i<matchtablelength;i++)
	{
		for(j=0;j<tablewidth-1;j++)
		{
			if(mark[j]==1)
				add_rule[j]=matchtable[i][j];
			else
				add_rule[j]="-";
		}
		add_rule[tablewidth-1]=matchtable[i][tablewidth-1];
		if(AddRuletoTree(add_rule)!=0)
			rulelength[rule_length]++;	
	}
}

void CRStree::SampleTest(CString filename1,CString filename2)
{
	int i,j;
	FILE *fp;
	char a[20];
	CString **sampletable;
	int sampletablelength,sampletablewidth;
	if(filename1=="") filename1="Sample.txt";
	if((fp=fopen(filename1,"r"))==NULL)
	{
		MessageBox(0,"不能打开原始数据表文件", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	fscanf(fp,"%d",&sampletablelength);//行数
	fscanf(fp,"%d",&sampletablewidth);//列数
	if(sampletablewidth!=tablewidth)
	{	
		MessageBox(0,"样本数据和原始数据不符", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	sampletable=new CString *[sampletablelength];
	for(i=0;i<sampletablelength;i++)
	{
		sampletable[i]=new CString[sampletablewidth];
	}
	for(i=0;i<sampletablelength;i++)
		for(j=0;j<sampletablewidth;j++)
		{
			fscanf(fp,"%s",a);				
			sampletable[i][j]=a;
		}
	fclose(fp);

	int correct=0,wrong=0,cannot=0;
	for(i=0;i<sampletablelength;i++)
	{
		BestRulePath=new TreeNode *[tablewidth];
		tempRulePath=new TreeNode *[tablewidth];	
		tempRulePath[0]=&RSHeadNode;
		maxmatchnumber=0;
		SearchRule(sampletable[i],&RSHeadNode);
		if(ifBestRulePath==1)
		{
			if(BestRulePath[tablewidth-1]->decisionattrvalue==sampletable[i][tablewidth-1])
				correct++;
			else
				wrong++;
		}
		else
			cannot++;
		delete[] BestRulePath;
		delete[] tempRulePath;
		ifBestRulePath=0;
	}

	if(filename2=="") filename2="样本测试结果.txt";
	if((fp=fopen(filename2,"w"))==NULL)
	{
		MessageBox(0,"不能打开 样本测试结果.txt", "Error", 
			MB_ICONERROR | MB_OK);
		return;
	}
	float all;
	float each;
	all=correct+wrong+cannot;
	fprintf(fp,"正确识别数：%d\n",correct);
	fprintf(fp,"错误识别数：%d\n",wrong);
	fprintf(fp,"未识别数：  %d\n",cannot);
	each=correct;
	fprintf(fp,"\n正确识别率：%.1f%%\n",(each/all)*100);
	each=wrong;
	fprintf(fp,"错误识别率：%.1f%%\n",(each/all)*100);
	each=cannot;
	fprintf(fp,"未识别率：  %.1f%%\n",(each/all)*100);
	fclose(fp);

}

void CRStree::SearchRule(CString *object, TreeNode *CurrentNode)
{
	TreeNode *matchson,*matcheveryson,*fson,*son;	
	int j,i;
	if(CurrentNode->layer<=tablewidth-2)
	{
		son=CurrentNode->son;
		fson=CurrentNode;
		matcheveryson=NULL;
		matchson=NULL;
		j=0;
		while((j<CurrentNode->sonnumber)&&((matchson==NULL)||(matcheveryson==NULL)))
		{
			j++;
			if(son->conditionattrvalue==object[son->conditionattr])
				matchson=son;				
			if(son->conditionattrvalue=="-")
				matcheveryson=son;
			fson=son;
			son=son->next;
		}
		if(matchson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matchson;
			if(CurrentNode->layer==tablewidth-2)
			{
				matchson->matchnumber++;
				if(matchson->matchnumber>maxmatchnumber)
				{
					maxmatchnumber=matchson->matchnumber;
					for(i=0;i<tablewidth;i++)
						BestRulePath[i]=tempRulePath[i];
					ifBestRulePath=1;
				}
			}
			else
				Search(object,matchson);
		}
		if(matcheveryson!=NULL)
		{
			tempRulePath[CurrentNode->layer+1]=matcheveryson;
			if(CurrentNode->layer==tablewidth-2)
			{
				matcheveryson->matchnumber++;
				if(matcheveryson->matchnumber>maxmatchnumber)
				{
					maxmatchnumber=matcheveryson->matchnumber;
					for(i=0;i<tablewidth;i++)
						BestRulePath[i]=tempRulePath[i];
					ifBestRulePath=1;
				}
			}
			else
			Search(object,matcheveryson);
		}
	}
}

void CRStree::MutiIncreStudy(int multistudynum,int ifmethod,int ifaccelarate)
{
	int i,j,p;
	CString *addobject;
	addobject=new CString[tablewidth];
	int studylength;
	studylength=studytablelength/multistudynum;
	int startposition,endposition=-1;
	for(p=0;p<multistudynum;p++)
	{
		startposition=endposition+1;
		endposition=startposition+studylength;
		if(endposition>studytablelength) 
			endposition=studytablelength;
		if(startposition>studytablelength)
			startposition=studytablelength;

		for(i=startposition;i<endposition;i++)
		{
			BestRulePath=new TreeNode *[tablewidth];
			tempRulePath=new TreeNode *[tablewidth];	
			tempRulePath[0]=&RSHeadNode;
			maxmatchnumber=0;
			if(ifaccelarate==0)
				Search(studytable[i],&RSHeadNode);
			else
				Search1(studytable[i],&RSHeadNode);
			if(ifBestRulePath==0)
			{
				//if(ifmethod==1)
				//{
				//	for(j=0;j<tablewidth;j++)
				//		addobject[j]="-";
				//	addobject[orderofattr[tablewidth-2]]=studytable[i][tablewidth-2];
				//	AddRuletoTree(addobject);
				//	rulelength[1]++;			
				//}
				//else
				{
					if(AddRuletoTree(studytable[i])!=0)
						rulelength[tablewidth-1]++;
				}
			}
			else
				if(BestRulePath[tablewidth-1]->decisionattrvalue!=studytable[i][tablewidth-1])
				{
					if(ifmethod==0)
					{
						SearchMatchTable(BestRulePath,i); //查找匹配表；
						RepairTree(BestRulePath,i);
						for(j=0;j<matchtablelength;j++)
							delete[] matchtable[j];
						delete[] matchtable;
						matchtablelength=0;
					}
					else
					{
						if(AddRuletoTree(studytable[i])!=0)
							rulelength[tablewidth-1]++;
					}
				}
				delete[] BestRulePath;
				delete[] tempRulePath;
				ifBestRulePath=0;
		}	
	}
}
