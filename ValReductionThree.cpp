// ValReductTwo.cpp : implementation file
//

#include "stdafx.h"
//#include "RSet.h"
//#include "roughsetDoc.h"
#include"stdlib.h"
#include "ValReductionThree.h"
#include<stdio.h>
#include"string.h"

#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
/////////////////////////////////////////////////////////////////////////////
// ValReductionThree

ValReductionThree::ValReductionThree()
{
  array=NULL;
  element=0;
}

ValReductionThree::~ValReductionThree()
{
  	int i,j;
	for(i=0;i<rule_num;i++)
	{
		delete []rule[i];
	}
	delete []rule;
	for(j=0;j<AttCount;j++)
	{
		delete []datasign[j];
		if (cuttab[j]!=NULL) delete []cuttab[j];
	}
	delete []cuttab;
	delete []datatype;
	delete []datasign;
}
/////////////////////////////////////////////////////////////////////////////
// ValReductionThree message handlers

int ** ValReductionThree::generate_rule()
{
	int i,j,k;                                   //循环变量	
	int same=0;               //统计不属于重复记录的个数
	int rec=0;                         //统计新的规则数
	int sum=0;
	int * overlaps;                       //保存重复的记录
	int * att;                                   //约简后的属性
	int * tuple;                                 //保存多余的规则
	overlaps=find_overlap(info,rec_num);
	//pDelTable = new int[con_num+1];
	//for(i = 0;i < con_num+1;i++)
	//	pDelTable[i] = 0;
	if(overlaps!=NULL)
	{
		same_rec=new int[rec_num-overlaps[0]];
		for (i=0;i<(rec_num-overlaps[0]);i++)
		   same_rec[i]=0;
	}
	else
	{	same_rec=new int[rec_num];
		for (i=0;i<(rec_num);i++)
		   same_rec[i]=0;
	}
	int number=0;
	if(overlaps!=NULL)
	{
		rule_num=rec_num-overlaps[0];
		int ** rule=new int * [rule_num];
		for(i=0;i<rule_num;i++)
			rule[i]=new int[con_num+1];
		for(i=0;i<rec_num;i++)
		{
			for(j=0;j<overlaps[0];j++)
				if(i!=overlaps[j+1])
					same++;
			if(same==overlaps[0])
			{
				for(j=0;j<con_num+1;j++)
					rule[rec][j]=info[i][j];
				for (j=0;j<rec_num;j++)
				{
					 for (k=0;k<con_num+1;k++)
					   if (info[i][k]==info[j][k])
							  sum++;
						  else 
							  break;
					if (sum==con_num+1)
						number++;
                    sum=0;
				}
                same_rec[rec]=number;
				rec++;
			}
			same=0;
			number=0;
		}
		for(i=0;i<rec_num;i++)
			delete []info[i];
		delete []info;
	    info=new int *[rule_num];
		for (i=0;i<rule_num;i++)
			info[i]=new int [con_num+1];
		for (i=0;i<rule_num;i++)
			for (j=0;j<con_num+1;j++)
				info[i][j]=rule[i][j];
		for(i=0;i<rule_num;i++)
			delete []rule[i];
		delete []rule;
		rec_num=rule_num;
		delete []overlaps;
	}
	else
		rule_num=rec_num;

	int **information=new int * [rule_num];
	for(i=0;i<rule_num;i++)
	{
		same=0;
		information[i]=new int[con_num+1];
        create_array(i);
        reduct_table();
		if(reduct_att_num>1)
    		att=get_match_most(i);
		else
			att=result[0];
		for(j=0;j<con_num;j++)
		{
			for(k=0;k<att[0];k++)
				if(j!=att[k+1])
					same++;
				else 
					break;
			if(same!=att[0])
	     		information[i][j]=info[i][j];
			else
				information[i][j]=-1;
			same=0;
		}
		information[i][con_num]=info[i][con_num];
		if(result!=NULL)
		{
	    	for(j=0;j<reduct_att_num;j++)
    	    	delete []result[j];
   	    	delete []result;
		}
		result=NULL; 
		if(reductset!=NULL)
        	delete reductset;
		reductset=NULL;
	}

	overlaps=find_overlap(information,rec_num);
	if(overlaps!=NULL)
	{
		rule_num=rec_num-overlaps[0];
		int *same_record;
		int number=0;
		int sum=0;
		same_record=new int [rec_num-overlaps[0]];
		int ** rule=new int * [rule_num];
		for(i=0;i<rule_num;i++)
			rule[i]=new int[con_num+1];
		rec=0;
		for(i=0;i<rec_num;i++)
		{
			for(j=0;j<overlaps[0];j++)
				if(i!=overlaps[j+1])
					same++;
			if(same==overlaps[0])
			{
				for(j=0;j<con_num+1;j++)
					rule[rec][j]=information[i][j];
				for (j=0;j<rule_num;j++)
					{
						if (i!=j)
						{
							for (k=0;k<con_num+1;k++)
								if (information[i][k]==information[j][k])
									sum++;
							if (sum==con_num+1)
								number++;
							sum=0;
						}
					}
				same_record[rec]=same_rec[i]+number;
                rec++;
				number=0;
			}
			same=0;
		}
		for(i=0;i<rec_num;i++)
			delete []information[i];
		delete []information;
		information=rule;
		delete []same_rec;
		rec_num=rule_num;
		same_rec=new int [rule_num];
		for (i=0;i<rule_num;i++)
			same_rec[i]=same_record[i];
		delete []same_record;
		delete []overlaps;
	}

	tuple=del_superflous(information);
	if(tuple!=NULL)
	{
		same=0;
		rec=0;                        
		int *same_record=new int [rule_num-tuple[0]];
		int number=0;
		int sum=0;
		int ** rule=new int * [rule_num-tuple[0]];
 		for(i=0;i<rule_num-tuple[0];i++)
		rule[i]=new int[con_num+1];
		for(i=0;i<rule_num;i++)
		{
			for(j=0;j<tuple[0];j++)
				if(i!=tuple[j+1])
					same++;
			if(same==tuple[0])
			{
				for(j=0;j<con_num+1;j++)
					rule[rec][j]=information[i][j];
				for (j=0;j<rule_num;j++)
					{
						if (i!=j)
						{
							for (k=0;k<con_num+1;k++)
								if (information[i][k]==information[j][k])
									sum++;
							if (sum==con_num+1)
								number++;
							sum=0;
						}
					}
				same_record[rec]=same_rec[i]+number;
                rec++;
				number=0;
				}
			same=0;
		}
		for(i=0;i<rule_num;i++)
			delete []information[i];
		delete []information;
		information=rule;
		delete []same_rec;
		rule_num=rec;
		same_rec=new int [rule_num];
		for (i=0;i<rule_num;i++)
			same_rec[i]=same_record[i];
		delete []same_record;
		delete []tuple;
	}
	return information;
}

int * ValReductionThree::find_overlap(int * * tab, int number)
{
	int i,j,k,t;                     //循环变量
	int count=0;                     //统计属性值相同的个数
	int find=FALSE;           //查找是否已经被列为重复记录
	int * same_rec=NULL;             //保存重复的记录
	int same_num=0;                  //重复记录的个数
	for(i=0;i<number-1;i++)
	{
		for(j=i+1;j<number;j++)
		{
			for(k=0;k<con_num+1;k++)
				if(tab[i][k]==tab[j][k])
					count++;
				else
					break;
			if(count==(con_num+1))
			{
				if(same_rec==NULL)
                 	same_rec=new int[number];
				for(t=0;t<same_num;t++)
                    if(same_rec[t]==i)
					{
					    find=TRUE;
					    break;
					}
				if(find==FALSE)
			    	same_rec[++same_num]=j;
			}
			find=FALSE;
			count=0;
		}
	}
	if(same_rec!=NULL)
    	same_rec[0]=same_num;
	return same_rec;
}

void ValReductionThree::create_array(int row)
{
	int i,k,l,n;                                //循环变量
	int * att;                                  //条件属性组合
	int find=FALSE;
	int m=0,num=0;
    att=new int[con_num+1];
	element=0;
	for(i=0;i<rec_num;i++)
	{
		if(i!=row) //如果不是同一行
		{
      		if(info[row][con_num]!=info[i][con_num])//如果决策属性不同
			{
	    		for(k=0;k<con_num;k++)
		    		if(info[row][k]!=info[i][k])
					{
			    		att[++m]=k;
					}
		        att[0]=m;//att中保存属性不同的属性编号
		        m=0;
		        for(l=0;l<element;l++)
				{
		     	    if(array[l][0]==att[0])
					{
    		     	    for(n=1;n<=att[0];n++)
			     		    if(array[l][n]==att[n])
                                num++;
			            if(num==att[0])//如果在决策表中能找到该编号
						{
			                find=TRUE;
				     	   	break;
						}
					} 
			    	num=0;
				}
		    	if(find==FALSE)
				{
		    		int ** newarray=new int *[element+1];
			    	for(l=0;l<element;l++)
					{
			     	    newarray[l]=new int[array[l][0]+1];
				     	for(n=0;n<array[l][0]+1;n++)
				    		newarray[l][n]=array[l][n];
					}
		        	if(element>0)
		         		delete []array;
			    	array=newarray;
		    	    array[element]=new int[att[0]+1];
		         	for(l=0;l<att[0]+1;l++)
		    	       	array[element][l]=att[l];
		    	   	element++;
				}
		    	find=FALSE;
	    		num=0;
			}
		}
	}
	delete []att;
}

void ValReductionThree::reduct_table()
{
	int i,j,k;                                  //循环变量
	int redu_num=0;                       //约简属性集的元素个数
    int att_num=0;              //除去核属性后其余的属性组合数
	int *** DNF=NULL;                 //由整数表示的积之和
	int item_num;                   //积之和表达式的项数
	int find=FALSE;
   
    	reductset=new int[con_num+1];

	reductset[0]=0;
	for(i=0;i<element;i++)
		if(array[i][0]==1)
		{
			reductset[++redu_num]=array[i][1];
			reductset[0]=redu_num;
		}
	if(redu_num>0)
	{
		for(i=0;i<element;i++)
			if(array[i][0]>1)
			{
				for(j=1;j<=array[i][0];j++)
				{
                    for(k=1;k<=redu_num;k++)
						if(array[i][j]==reductset[k])
						{
							find=TRUE;
							break;
						}
					if(find==TRUE)
						break;
				}
				if(find==FALSE)
					att_num++;
				find=FALSE;
			}
		if(att_num>0)          //如果存在不包含核属性的属性组合
		{
			int ** newarray=new int *[att_num];
			att_num=0;
    		for(i=0;i<element;i++)
	    		if(array[i][0]>1)
				{
			    	for(j=1;j<=array[i][0];j++)
					{
                        for(k=1;k<=redu_num;k++)
					    	if(array[i][j]==reductset[k])
							{
					    		find=TRUE;
						    	break;
							}
				    	if(find==TRUE)
					    	break;
					}
				    if(find==FALSE)
					{
     			    	newarray[att_num]=new int[array[i][0]+1];
			     		for(k=0;k<array[i][0]+1;k++)
				        		newarray[att_num][k]=array[i][k]; 
			   		att_num++;
					}
			    	find=FALSE;
				}
	    	element=att_num;
			delete []array;
	    	array=newarray;
		}
		else
		{
			reduct_att_num=1;
			result=new int * [reduct_att_num];
			result[0]=new int[redu_num+1];
			result[0][0]=redu_num;
			for(i=0;i<redu_num;i++)
				result[0][i+1]=reductset[i+1];
			selectsort(result[0]);
			return;
		}
	}
	if(element>1)
		DNF=CNFtoDNF(element,item_num);
	else
	{
		DNF=new int ** [array[0][0]];
		for(i=0;i<array[0][0];i++)
		{
			DNF[i]=new int * [2];
            DNF[i][0]=new int[1];
			DNF[i][0][0]=1;
			DNF[i][1]=new int[1];
			DNF[i][1][0]=(int)array[0][i+1];
		}
		item_num=array[0][0];
	}
	find_best_reduct(DNF,item_num);
	if(DNF!=NULL)
	{
		int number;
		for(i=0;i<item_num;i++)
		{
			number=DNF[i][0][0]+1;                   //加1是因为第一个值是计数器
			for(j=0;j<number;j++)
				delete []DNF[i][j];
			delete []DNF[i];
		}
		delete []DNF;
	}
}

int *** ValReductionThree::CNFtoDNF(int element,int& item_num)
{
	int i,j,k,t;                                          //循环变量
	int sum=0;                                            //属性相同的个数
	int * att;                                            //可删除的属性组
	int del=0;                                            //可删除的属性组个数
	char * newstr;                                        //由两条属性组成的新字符串
	int count=0;
	int att_num=0;
	int diff_num=0;
	int length;
	int *** DNF;
	bool find=FALSE;
	att=new int[element];
	selectsort(array,element);
	for(k=0;k<element-1;k++)
	    for(i=k+1;i<element;i++)
	    	if(array[k][0]<array[i][0])
			{
		        for(j=1;j<=array[k][0];j++)
				{
					find=FALSE;
					for(t=1;t<=array[i][0];t++)
						if(array[k][j]==array[i][t])
						{
							sum++;
							find=TRUE;
							break;
						}
					if(find==FALSE)
						break;
				}
		    	if(sum==array[k][0])
				{
					find=FALSE;
					for(j=1;j<=del;j++)
						if(att[j]==i)
						{
							find=TRUE;
							break;
						}
					if(find==FALSE)
					{
				    	att[++del]=i;
				    	att[0]=del;
					}
				}
				sum=0;
			}
	if(del>0)
	{
    	int ** newarray=new int *[element-del];
    	for(i=0;i<element;i++)
		{
	    	for(j=1;j<=del;j++)
	        	if(i!=att[j])
				{
		    	   	diff_num++;
					if(diff_num==att[0])
					{
				    	newarray[count]=new int[array[i][0]+1];
			        	for(k=0;k<array[i][0]+1;k++)
				        	newarray[count][k]=array[i][k];
				    	count++;
				    	break;
					}
				}
			diff_num=0;
		}
		delete []array;
		array=newarray;
	}
	delete []att;
	element=element-del;
   	struct intTostr
	{
    	char ** strarray;
		int item_num;
	};
	if(element>1)
	{
		intTostr * transfer=new intTostr[element];
		for(i=0;i<element;i++)
		{
			transfer[i].item_num=array[i][0];
			transfer[i].strarray=new char * [array[i][0]];
			for(j=0;j<array[i][0];j++)
			{
				transfer[i].strarray[j]=new char[2];
				transfer[i].strarray[j][0]=(char)(array[i][j+1]+1);
				transfer[i].strarray[j][1]='\0';
            }
		}
		while(element>1)
		{
	    	count=0;
			char ** stratt=new char * [transfer[0].item_num*transfer[1].item_num];
			for(j=0;j<transfer[0].item_num;j++)
				for(k=0;k<transfer[1].item_num;k++)
				{
					if(strcmp(transfer[0].strarray[j],transfer[1].strarray[k])==0||
					   charin(transfer[0].strarray[j],transfer[1].strarray[k])==TRUE||
					   charin(transfer[1].strarray[k],transfer[0].strarray[j])==TRUE)
					{
						if(strlen(transfer[0].strarray[j])<strlen(transfer[1].strarray[k]))
						{
							stratt[count]=new char[strlen(transfer[1].strarray[k])+1];
							strcpy(stratt[count++],transfer[1].strarray[k]);
						}
						else
						{
							stratt[count]=new char[strlen(transfer[0].strarray[j])+1];
							strcpy(stratt[count++],transfer[0].strarray[j]);
						}
					}
					else
					{
						newstr=new char[strlen(transfer[0].strarray[j])+
							            strlen(transfer[1].strarray[k])+1];
						strcpy(newstr,transfer[0].strarray[j]);
						strcat(newstr,transfer[1].strarray[k]);
						stratt[count]=new char[strlen(transfer[0].strarray[j])+
							                   strlen(transfer[1].strarray[k])+1];
						strcpy(stratt[count++],newstr);
						delete []newstr;
					}
				}
			selectsort(stratt,count);
			int * over=new int[count];
			sum=0;
			for(j=0;j<count-1;j++)
				for(k=j+1;k<count;k++)
			    	if(strstr(stratt[k],stratt[j])!=NULL)
					{
						find=FALSE;
						for(i=0;i<sum;i++)
							if(over[i]==k)
							{
								find=TRUE;
								break;
							}
						if(find==FALSE)
				        	over[sum++]=k;
					}
			if(sum)
			{
		    	char ** replace=new char * [count-sum];
		    	diff_num=0;
				att_num=0;
		    	for(j=0;j<count;j++)
				{
			    	for(k=0;k<sum;k++)
				    	if(j!=over[k])
						{
					    	diff_num++;
					    	if(diff_num==sum)
							{
					        	replace[att_num]=new char[strlen(stratt[j])+1];
					        	strcpy(replace[att_num],stratt[j]);
					           	att_num++;
							}
						}
			    	diff_num=0;
				}
				for(j=0;j<transfer[0].item_num*transfer[1].item_num;j++)
					delete []stratt[j];
		    	delete []stratt;
		    	stratt=replace;
			}
		    delete []over;
			/* 修改原strarray中的第一项  */
			intTostr * new_transfer=new intTostr[element-1];
			if(att_num==0)
				att_num=count;
			new_transfer[0].item_num=att_num;
			new_transfer[0].strarray=new char * [att_num];
			for(j=0;j<att_num;j++)
			{
				new_transfer[0].strarray[j]=new char[strlen(stratt[j])+1];
				strcpy(new_transfer[0].strarray[j],stratt[j]);
			}
			for(j=1;j<element-1;j++)
			{
                new_transfer[j].strarray=new char * [transfer[j+1].item_num];
				new_transfer[j].item_num=transfer[j+1].item_num;
				for(k=0;k<transfer[j+1].item_num;k++)
				{
					new_transfer[j].strarray[k]=new char[strlen(transfer[j+1].strarray[k])+1];
					strcpy(new_transfer[j].strarray[k],transfer[j+1].strarray[k]);
				}
			}
			for(j=0;j<element;j++)
			{
				for(k=0;k<transfer[j].item_num;k++)
         			delete []transfer[j].strarray[k];
				delete []transfer[j].strarray;
			}
			delete []transfer;
			transfer=new_transfer;
			element--;
			for(j=0;j<count-sum;j++)
				delete []stratt[j];
			delete []stratt;
		}
		DNF=new int ** [transfer[0].item_num];
		for(i=0;i<transfer[0].item_num;i++)
		{
			length=strlen(transfer[0].strarray[i]);
			DNF[i]=new int * [length+1];
			DNF[i][0]=new int[1];
			DNF[i][0][0]=length;                      //属性组合的属性个数
			for(j=1;j<=length;j++)
			{
                DNF[i][j]=new int[1];
				DNF[i][j][0]=(int)transfer[0].strarray[i][j-1]-1;
			}
		}
		item_num=transfer[0].item_num;
		for(i=0;i<element;i++)
		{
			for(j=0;j<transfer[i].item_num;j++)
				delete []transfer[i].strarray[j];
			delete []transfer[i].strarray;
		}
		delete []transfer;
		return(DNF);
	}
	else
	{
        intTostr * transfer=new intTostr[element];
		transfer[0].item_num=array[0][0];
		transfer[0].strarray=new char * [array[0][0]];
		for(j=0;j<array[0][0];j++)
		{
			transfer[0].strarray[j]=new char[2];
			transfer[0].strarray[j][0]=(char)(array[0][j+1]+1);
			transfer[0].strarray[j][1]='\0';
        }
		DNF=new int ** [transfer[0].item_num];
		for(i=0;i<transfer[0].item_num;i++)
		{
			length=strlen(transfer[0].strarray[i]);
			DNF[i]=new int * [length+1];
			DNF[i][0]=new int[1];
			DNF[i][0][0]=length;                      //属性组合的属性个数
			for(j=1;j<=length;j++)
			{
                DNF[i][j]=new int[1];
				DNF[i][j][0]=(int)transfer[0].strarray[i][j-1]-1;
			}
		}
		item_num=transfer[0].item_num;
		for(i=0;i<element;i++)
		{
			for(j=0;j<transfer[i].item_num;j++)
				delete []transfer[i].strarray[j];
			delete []transfer[i].strarray;
		}
		delete []transfer;
		return(DNF);
	}
}

void ValReductionThree::find_best_reduct(int *** DNF,int& item_num)
{
	int i,j,k;                                     //循环变量
    int att_num;                                   //属性数目
    int * best_att;                            //最佳约简所在的属性集合
    best_att=get_best_att(DNF,item_num);
	reduct_att_num=best_att[0];
	result=new int * [best_att[0]];
    for(i=0;i<best_att[0];i++)
	{
    	att_num=DNF[best_att[i+1]][0][0];
    	int * best_redu=new int[reductset[0]+att_num+1];
    	best_redu[0]=reductset[0]+att_num;
    	for(j=0;j<reductset[0];j++)
            best_redu[j+1]=reductset[j+1];
    	for(j=reductset[0]+1;j<reductset[0]+att_num+1;j++)
		{
        	k=j-reductset[0];
    		best_redu[j]=DNF[best_att[i+1]][k][0];
		}
    	selectsort(best_redu);	
		result[i]=new int[reductset[0]+att_num+1];
		for(j=0;j<reductset[0]+att_num+1;j++)
	    	result[i][j]=best_redu[j];
		delete []best_redu;
	}
	delete []best_att;
}

int * ValReductionThree::get_best_att(int ***style, int num)
{
	int i;
	int min=0;
	int count=0;
    int * att=new int[num+1];
	min=style[0][0][0];
	for(i=0;i<num;i++)
		if(min>style[i][0][0])
			min=style[i][0][0];
	for(i=0;i<num;i++)
		if(style[i][0][0]==min)
			att[++count]=i;
	att[0]=count;
    return att;
}

int * ValReductionThree::get_match_most(int row)
{
	int i,j,k,col;
	int * count;                         //统计匹配的次数
	int num=0;                           //统计匹配属性的个数
	int att_num=result[0][0];            //得到需检验的属性个数
	
	int * val=new int[att_num];
	count=new int[reduct_att_num];
	for(i=0;i<reduct_att_num;i++)
	{
		count[i]=0;
		for(j=0;j<att_num;j++)
		{
			col=result[i][j+1];
			val[j]=info[row][col];
		}
		for(j=0;j<rule_num;j++)
		{
			if(j!=row)
			{
	     		for(k=0;k<att_num;k++)
				{
		    		col=result[i][k+1];
		    		if(info[j][col]==val[k])
						num++;
					else
						break;
				}
				if(num==att_num)
					count[i]++;
				num=0;
			}
		}
	}
    int term=get_max(count,reduct_att_num);
	delete []val;
	delete []count;
	return result[term];
}

int * ValReductionThree::del_superflous(int ** tab)
{
	int i,j,k;                               //循环变量
	int count=0;                             //统计多余的规则
	int * tuple=NULL;                        //记录多余的规则
	int same=0;                              //统计属性值相同的个数
	int mark=0;                              //统计被标识的属性个数
	int rec=0;                               //检查是否已经属于被删除记录
	for(i=0;i<rule_num;i++)
		for(j=0;j<rule_num-1;j++)
		{
			if(j!=i)
			{
    			for(k=0;k<con_num+1;k++)
	    			if(tab[i][k]==tab[j][k])
		    			same++;
		    		else if(tab[j][k]==-1)
			    		mark++;
					else 
	 					break;
	    		if(same+mark==con_num+1&&same!=con_num+1)
				{
		    		if(tuple==NULL)
		     	    	tuple=new int[rule_num];
					for(k=0;k<count;k++)
						if(i!=tuple[k+1])
							rec++;
						else
							break;
					if(rec==count)
		     	    	tuple[++count]=i;
				}
		    	same=0;
		    	mark=0;
				rec=0;
			}
		}
	if(tuple!=NULL)
		tuple[0]=count;
	return tuple;
}

int ValReductionThree::get_max(int * att,const int num)
{
	int i;
	int max=0;
	for(i=0;i<num;i++)
		if(att[max]<att[i])
			max=i;
	return max;
}

int ValReductionThree::charin(const char * str1,const char * str2)
{
	int i;                                    //循环变量
	int length;                               //字符串长度
	char c;                                   //从字符串中取出的字符
	int sum=0;                                //str1中包含在str2中的字符数
	length=strlen(str1);
	for(i=0;i<length;i++)
	{
        c=str1[i];
		if(strchr(str2,c)!=NULL)
			sum++;
		else
			return FALSE;
	}
	if(sum==length)
		return TRUE;
	else
		return FALSE;
}


void ValReductionThree::selectsort(int ** att,int num)
{
	int i,j;                                  //循环变量
	int min_item;
	for(i=0;i<num-1;i++)
	{
		min_item=i;
		for(j=i+1;j<num;j++)
			if(att[j][0]<att[min_item][0])
				min_item=j;
		if(min_item!=i)
		{
			int * transfer=new int[att[i][0]+1];
			for(j=0;j<att[i][0]+1;j++)
				transfer[j]=att[i][j];
			delete att[i];
			att[i]=new int[att[min_item][0]+1];
			for(j=0;j<att[min_item][0]+1;j++)
				att[i][j]=att[min_item][j];
			delete att[min_item];
			att[min_item]=new int[transfer[0]+1];
			for(j=0;j<transfer[0]+1;j++)
				att[min_item][j]=transfer[j];
			delete []transfer;
		}
	}
}

void ValReductionThree::selectsort(char ** str,int num)
{
	int i,j;                                  //循环变量
	int min_item;
	for(i=0;i<num-1;i++)
	{
		min_item=i;
		for(j=i+1;j<num;j++)
			if(strlen(str[j])<strlen(str[min_item]))
				min_item=j;
		if(min_item!=i)
		{
			char * transfer=new char[strlen(str[i])+1];
			strcpy(transfer,str[i]);
			delete str[i];
			str[i]=new char[strlen(str[min_item])+1];
			strcpy(str[i],str[min_item]);
			delete str[min_item];
			str[min_item]=new char[strlen(transfer)+1];
			strcpy(str[min_item],transfer);
			delete []transfer;
		}
	}
}

void ValReductionThree::selectsort(int * att)
{
	int i,j;                                 //循环变量
	int min_item;                            //具有最小值的项
	int transfer;       
	for(i=0;i<att[0]-1;i++)
	{
		min_item=i;
		for(j=i+1;j<att[0];j++)
			if(att[j+1]<att[min_item+1])
				min_item=j;
		if(min_item!=i)
		{
			transfer=att[i+1];
			att[i+1]=att[min_item+1];
			att[min_item+1]=transfer;
		}
	}
}


bool ValReductionThree::Run(char *s,char *r)
{	
	if(RunOne(s)!=1) return false;
	con_num=AttCount-1;
	rec_num=RecCount;
	info=tab;
	rule=generate_rule();
	Save_File2(r);
	return true;
}

bool ValReductionThree::Run1(CString **table,int &tablelength,int tablewidth)
{	
	int i,j;
	char *tempstr;
	ReadReductionTable(table,tablelength,tablewidth);
	con_num=AttCount-1;
	rec_num=RecCount;
	info=new int *[tablelength];
	for(i=0;i<tablelength;i++)
		info[i]=new int[tablewidth];
	for(i=0;i<tablelength;i++)
		for(j=0;j<tablewidth;j++)
		{
			tempstr=table[i][j].GetBuffer(1);
			info[i][j]=atoi(tempstr);
		}
	rule=generate_rule();
	Save_File2("df");
	return true;
}

 void ValReductionThree::Save_File2(char *r)
{

	fout.open("规则.txt",ios::out|ios::trunc);
	int i,j,m,k;//循环变量
	//输出文件头
	fout<<"Style:rule"<<'\n';		//表示为规则文件
	fout<<"Stage:0"<<'\n';			//为规则文件这个值无用
	fout<<"Condition attributes number:"<<AttCount1-1<<'\n';//列数
	fout<<"The Number of Condition attributes deleted: "<<atbcount<<'\n';
	fout<<"The position of Condition attributes deleted: ";
//	fout<<atbcount<<'\n';
	for(i=0;i<atbcount;i++)
		fout<<atb[i]<<' ';
	fout<<'\n';
	fout<<"Rules number:"<<rule_num<<'\n';//行数
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
	for(i = 0;i <rule_num;i++)
	{
		k=0;
		for(j = 0;j<AttCount1;j++)
		{
			if(delatt[j])
			{
				fout<<'-'<<' ';
				continue;
			}
			if(rule[i][k]==-1)
				fout<<'-'<<' ';
			else
				if(cuttab[j]!=NULL)
					{					
						for(m=0;m<RecCount;m++)
							if(cuttab[j][m].x==(rule[i][k]))
							{
								fout<<cuttab[j][m].string<<' ';
								break;
							}
					}
				else
					fout<<rule[i][k]<<' ';
			k++;
		}
		fout<<1.0<<' ';
		fout<<same_rec[i]<<' ';
		fout<<same_rec[i];
		fout<<'\n';
	}

//写入文件尾部	while(!fin)
fout.close();
}


