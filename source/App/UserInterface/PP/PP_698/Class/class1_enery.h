/*"***********************************************************		
FileName: CLASS1_ENERY.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ��������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS1_ENERY_H_
#define _CLASS1_ENERY_H_
/*"******************************************************"*/
//#include "class_comm.h"

typedef struct
{ 
    //INT8U Offset_List;/*������ܻ�Ҫ������ڶ�����е�λ���ֽ�,������OAD�б��Ӧ*/
    INT16U Obis;      /*"��num�йأ�numС��0x80,����������OBIS������������OBIS��"*/
    INT8U  num;       /*"OAD��ӦOBIS��Ԫ�ظ��� ,���С��0x80,���ʾ��������OBIS���������ǿ�OBIS"*/
    INT8U  Offset;    /*"�����������ݵ�ƫ�� ���̶�Ϊ0���ȱ��������濴������پ����Ƿ�ȥ��"*/

    INT8U  FormatId;  /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;/*"��д����ID"*/
    INT8U  ShowinfoId;/*"��ʾ���ñ�ID"*/
    /*���濴���£�ʵ���������һ���ֽ�*/
}ST_OAD_OBIS_INFO;
    

typedef struct
{
   INT16U OI;             /*OI����*/   
   INT8U  MAX_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/
   INT8U  element;        /*��ӦST_OBJECT_ELC��Ԫ��λ��*/
}ST_Class1_6_Object;    /*����Ԫ��*/
/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern const ST_Class1_6_Object CLASS1OBISLIST[];
extern const ST_OAD_OBIS_INFO stCls1ObisInfo[];
 
/*class1 ����4�б� �߾��ȵ���*/
extern const ST_Class1_6_Object CLASS1_AID_4_OBISLIST[];
extern const ST_OAD_OBIS_INFO stCls1_AID_4_ObisInfo[]; 

extern INT8U Get_Variable_Object(SUB_OBJECT *pObject);
extern INT8U get_class1_6_object(SUB_OBJECT *pObject,INT8U class_num);
extern void get_class1_6_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info);

/*"******************************************************"*/
#endif
