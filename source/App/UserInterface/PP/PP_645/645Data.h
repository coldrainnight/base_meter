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
/*"节点定义"*/
/*"属性				│长度	│名称												│说明                        "*/
/*"ID					│4		│数据标识										│	                              "*/
/*"Node_mode	│1		│节点模式										│Bit0：1节点；0叶子    "*/
/*"						│			│														│Bit1：1本节点的ID有效；0无效                 "*/
/*"						│			│														│Bit2：1本节点是有压缩；0无                   "*/
/*"						│			│														│Bit3：1下级节点有数据标识；0无               "*/
/*"						│			│														│Bit4-Bit7：压缩的数据标识                    "*/
/*"ID_Start		│1		│压缩数据标识的起始值				│有压缩时有效"*/
/*"ID_end			│1		│压缩数据标识的结束值				│有压缩时有效  "*/
/*"High_Node	│2		│上级节点地址	         			│         "*/
/*"Next_Node	│2		│下个同级节点地址	     			│         "*/
/*"Low_Node		│2		│下级节点地址/叶子对象地址	│"*/
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
/*"地址修正"*/
struct REVISE_ID
{
	INT8U Revise_Id;
};
struct COMMUNAL_REVISE
{	
	INT8U	reviseMode;   /*"地址修正模式"*/
	INT8U	reviseSize;   /*"地址修正步长"*/
	INT8U	reviseMaxNum; /*"地址修正最大偏移数"*/
	INT8U reviseBase;   /*"地址修正起始量"*/
};



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"权限"*/
struct COMMUNAL_PURVIEW
{
	INT8U read_mode;
	INT8U	write_mode[3];
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"方法"*/
struct COMMUNAL_FUNCTION
{
		T645On_Write_Event OnWrite_Beford;
		T645On_Write_Event OnWrite_End;
		T645On_Read_Event OnRead;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"显示"*/
struct COMMUNAL_SHOWINFO
{
   INT8U schar[MAX_SCHAR_NUM];
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"组合"*/
struct COMMUNAL_MATHINFO
{	
	INT8U   Ass_data_addr;/*"组合数据步长"*/
	INT8U   Ass_data_num;/*"组合数据个数"*/
	INT16U 	mathModeObis;	/*"组合模式字对象标识"*/
	INT8U   math_type;/*"计算模式"*/
};


struct COMMUNAL_ID
{	
	INT8U Communal_Trans_Id;  /*"通信传输格式"*/
	INT8U Communal_Show_Id;  /*"显示传输格式"*/	
};

struct COMMUNAL_RANGE 
{	
	INT16U idStartObis;   /*"压缩位起始值/结束数据对象标识"*/
	INT16U idEndObis;  /*"压缩位结束值/结束数据对象标识,考虑到这里只有费率数需要访问数据中心，单独为费率数建立一个对象，节省空间"*/
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*"叶子定义"*/
/*"属性										│长度	│名称				│说明"*/
/*"addr										│4		│地址				│"*/
/*"Communal_Addr_Id				│1		│地址修正ID	│进行地址修正的参数，主要用于压缩数据的地址修正"*/
/*"Communal_Math_Mode_Id	│1		│组合信息ID	│"*/
/*"Communal_Id						│1		│格式信息ID	│"*/
/*"Communal_Purview_Id		│1		│权限信息ID	│"*/
/*"Communal_Function_Id		│1		│方法信息ID	│"*/
/*"Communal_Showinfo_Id		│2		│显示信息ID	│"*/

struct TREE_LEAF
{
	INT16U obis;         /*"叶子对应的数据对象"*/
	INT16U offSet;       /*"叶子对应的数据对象内地址偏移量"*/
	INT8U Communal_Addr_Id;
	INT8U Communal_Math_Mode_Id;
	INT8U Communal_Id;
	INT8U Communal_Purview_Id;/*"读写权限"*/	
	INT8U Communal_Function_Id;/*"读写方法"*/
	INT8U Communal_Showinfo_Id;/*"显示配置表ID"*/
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
