
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ValReductTwo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CValReductTwo window
//struct CutRecord
//{
	//int iColumn;
	//int iCuts;
	//char** cpCut;
	//CutRecord* p;
//};
#include<fstream.h>
#include"ValBase.h"

class ValReductionThree:ValBase //属性约简后的值约简的算法1
{
private:
	void Save_File2(char *);
    int **info;                       //属性约简后的信息表
    int rule_num;       //规则数目
    int **array;         //决策矩阵 
	int element;          //决策矩阵中元素的个数
    int * reductset;      //值约简属性集
    int **result;          //值约简结果
	int reduct_att_num; //有几组值约简结果
    int ** generate_rule();                        //生成规则
    int * find_overlap(int ** tab,int number);     //查找重复记录
    void create_array(int row);  //设置决策矩阵
    void  reduct_table(); //计算属性组合
    int *** CNFtoDNF(int element,int& item_num); //和之积转化为积之和
    int * get_best_att(int *** style,int num); //得到最好的属性组合
	void find_best_reduct(int *** DNF,int& item_num); //输出规则
	int * get_match_most(int row); //得到匹配最多的属性组合
	int * del_superflous(int ** tab);              //删除多余的规则
    int charin(const char * str1,const char * str2);      //判断str1中的字符是否完全属于str2
	void selectsort(int ** att,int num); //用选择法排序找出属性数目最小者
    void selectsort(char ** str,int num);  //用选择法排序找出属性数目最小者
    void selectsort(int * att);//用选择法对约简后的属性重新排列
    int get_max(int * att,const int num);   //得到匹配次数最多的属性组合      
	int con_num;                      //约简后的条件属性个数
	int *same_rec;
	int rec_num;                      //信息表记录数
	int **rule;
// Construction
public:
	ValReductionThree();
	virtual ~ValReductionThree();
	bool Run(char *s,char *r);
	bool Run1(CString **table,int &tablelength,int tablewidth);
// Overrides 
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValReductTwo)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_VALREDUCTTWO_H__B758B5C1_1A7F_11D4_95BC_52544C303D44__INCLUDED_)
