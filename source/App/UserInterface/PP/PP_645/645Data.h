#ifndef _645DATA_H
#define _645DATA_H

#include "system\Sys_def.h"
#include "Driver\Hal\DIS\DIS.h"
#include "App\LegallyRelevant\LRM_Config.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"
#include "App\UserInterface\PP\PP_645\PP_Offset.h"
#ifdef RCRD_MODLE
#include "App\record\RCRD_dataProcess.h"
#endif

typedef INT8U  (* T645On_Read_Event)(INT8U *pdata);
typedef INT8U  (* T645On_Write_Event)(INT8U *pdata);


struct MODE_BIT
{
	INT8U mode:1;
	INT8U ID_Enable:1;
	INT8U zip:1;
	INT8U Low_Node_Num:1;
	INT8U num :4;
};
union NODE_MODE{
	INT8U	byte;
	struct MODE_BIT bit;		
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"�ڵ㶨��"*/
/*"����				������	������												��˵��                        "*/
/*"ID					��4		�����ݱ�ʶ										��	                              "*/
/*"Node_mode	��1		���ڵ�ģʽ										��Bit0��1�ڵ㣻0Ҷ��    "*/
/*"						��			��														��Bit1��1���ڵ��ID��Ч��0��Ч                 "*/
/*"						��			��														��Bit2��1���ڵ�����ѹ����0��                   "*/
/*"						��			��														��Bit3��1�¼��ڵ������ݱ�ʶ��0��               "*/
/*"						��			��														��Bit4-Bit7��ѹ�������ݱ�ʶ                    "*/
/*"ID_Start		��1		��ѹ�����ݱ�ʶ����ʼֵ				����ѹ��ʱ��Ч"*/
/*"ID_end			��1		��ѹ�����ݱ�ʶ�Ľ���ֵ				����ѹ��ʱ��Ч  "*/
/*"High_Node	��2		���ϼ��ڵ��ַ	         			��         "*/
/*"Next_Node	��2		���¸�ͬ���ڵ��ַ	     			��         "*/
/*"Low_Node		��2		���¼��ڵ��ַ/Ҷ�Ӷ����ַ	��"*/
struct TREE_NODE
{
	INT32U ID;
	union NODE_MODE Node_mode;
	INT8U Range_ID;
	//INT8U ID_end;
	//INT16U High_Node;
	INT16U Next_Node;
	INT16U Low_Node;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"��ַ����"*/
struct REVISE_ID
{
	INT8U Revise_Id;
};
struct COMMUNAL_REVISE
{	
	INT8U	reviseMode;   /*"��ַ����ģʽ"*/
	INT8U	reviseSize;   /*"��ַ��������"*/
	INT8U	reviseMaxNum; /*"��ַ�������ƫ����"*/
	INT8U reviseBase;   /*"��ַ������ʼ��"*/
};



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"Ȩ��"*/
struct COMMUNAL_PURVIEW
{
	INT8U read_mode;
	INT8U	write_mode[3];
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"����"*/
struct COMMUNAL_FUNCTION
{
		T645On_Write_Event OnWrite_Beford;
		T645On_Write_Event OnWrite_End;
		T645On_Read_Event OnRead;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"��ʾ"*/
struct COMMUNAL_SHOWINFO
{
   INT8U schar[MAX_SCHAR_NUM];
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"���"*/
struct COMMUNAL_MATHINFO
{	
	INT8U   Ass_data_addr;/*"������ݲ���"*/
	INT8U   Ass_data_num;/*"������ݸ���"*/
	INT16U 	mathModeObis;	/*"���ģʽ�ֶ����ʶ"*/
	INT8U   math_type;/*"����ģʽ"*/
};


struct COMMUNAL_ID
{	
	INT8U Communal_Trans_Id;  /*"ͨ�Ŵ����ʽ"*/
	INT8U Communal_Show_Id;  /*"��ʾ�����ʽ"*/	
};

struct COMMUNAL_RANGE 
{	
	INT16U idStartObis;   /*"ѹ��λ��ʼֵ/�������ݶ����ʶ"*/
	INT16U idEndObis;  /*"ѹ��λ����ֵ/�������ݶ����ʶ,���ǵ�����ֻ�з�������Ҫ�����������ģ�����Ϊ����������һ�����󣬽�ʡ�ռ�"*/
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"Ҷ�Ӷ���"*/
/*"����										������	������				��˵��"*/
/*"addr										��4		����ַ				��"*/
/*"Communal_Addr_Id				��1		����ַ����ID	�����е�ַ�����Ĳ�������Ҫ����ѹ�����ݵĵ�ַ����"*/
/*"Communal_Math_Mode_Id	��1		�������ϢID	��"*/
/*"Communal_Id						��1		����ʽ��ϢID	��"*/
/*"Communal_Purview_Id		��1		��Ȩ����ϢID	��"*/
/*"Communal_Function_Id		��1		��������ϢID	��"*/
/*"Communal_Showinfo_Id		��2		����ʾ��ϢID	��"*/

struct TREE_LEAF
{
	INT16U obis;         /*"Ҷ�Ӷ�Ӧ�����ݶ���"*/
	INT16U offSet;       /*"Ҷ�Ӷ�Ӧ�����ݶ����ڵ�ַƫ����"*/
	INT8U Communal_Addr_Id;
	INT8U Communal_Math_Mode_Id;
	INT8U Communal_Id;
	INT8U Communal_Purview_Id;/*"��дȨ��"*/	
	INT8U Communal_Function_Id;/*"��д����"*/
	INT8U Communal_Showinfo_Id;/*"��ʾ���ñ�ID"*/
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#endif


extern  const int Protocol_Tree_Root[35];
extern  const struct COMMUNAL_REVISE Protocol_Revise[29];
extern  const struct REVISE_ID Protocol_Revise_Id[29];
extern  const ST_FMT_ATTR Protocol_Format[65];
extern  const struct COMMUNAL_PURVIEW Protocol_Purview[6];
extern  const struct COMMUNAL_FUNCTION Protocol_Function[11];
extern  const struct TREE_NODE Protocol_Tree_Node[439];
extern  const struct TREE_LEAF Protocol_Tree_Leaf[248];
extern  const struct COMMUNAL_SHOWINFO Protocol_Showinfo[26];
extern  const struct COMMUNAL_ID Protocol_Id[76];
extern  const struct COMMUNAL_MATHINFO Protocol_MATHINFO[5];
extern  const struct COMMUNAL_RANGE Protocol_Range[50];
