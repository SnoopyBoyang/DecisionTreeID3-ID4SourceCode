
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

class ValReductionThree:ValBase //����Լ����ֵԼ����㷨1
{
private:
	void Save_File2(char *);
    int **info;                       //����Լ������Ϣ��
    int rule_num;       //������Ŀ
    int **array;         //���߾��� 
	int element;          //���߾�����Ԫ�صĸ���
    int * reductset;      //ֵԼ�����Լ�
    int **result;          //ֵԼ����
	int reduct_att_num; //�м���ֵԼ����
    int ** generate_rule();                        //���ɹ���
    int * find_overlap(int ** tab,int number);     //�����ظ���¼
    void create_array(int row);  //���þ��߾���
    void  reduct_table(); //�����������
    int *** CNFtoDNF(int element,int& item_num); //��֮��ת��Ϊ��֮��
    int * get_best_att(int *** style,int num); //�õ���õ��������
	void find_best_reduct(int *** DNF,int& item_num); //�������
	int * get_match_most(int row); //�õ�ƥ�������������
	int * del_superflous(int ** tab);              //ɾ������Ĺ���
    int charin(const char * str1,const char * str2);      //�ж�str1�е��ַ��Ƿ���ȫ����str2
	void selectsort(int ** att,int num); //��ѡ�������ҳ�������Ŀ��С��
    void selectsort(char ** str,int num);  //��ѡ�������ҳ�������Ŀ��С��
    void selectsort(int * att);//��ѡ�񷨶�Լ����������������
    int get_max(int * att,const int num);   //�õ�ƥ����������������      
	int con_num;                      //Լ�����������Ը���
	int *same_rec;
	int rec_num;                      //��Ϣ���¼��
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
