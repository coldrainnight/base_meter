/*"***********************************************************		
FileName: CLASS17_DISPLAY.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ��ʾ��ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS17_DISPLAY_H_
#define _CLASS17_DISPLAY_H_
/*"******************************************************"*/
#include "class_comm.h"


/*class17 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT2;      /*"��ʾ�����б�"*/  
    INT16U  OBIS_ATT3;      /*"��ʾʱ��"*/  
    INT16U  OBIS_ATT4;      /*"��ʾ����"*/  
}ST_CLASS17_OI_AID_OBIS;


/*�ⲿ�������������*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS17_AID_LIST[];
extern const ST_CLASS17_OI_AID_OBIS CLASS17_OI_AID_OBIS[];

/*"******************************************************"*/
extern INT8U get_special_class_object(SUB_OBJECT *pObject);
extern INT8U set_class17_disp_data(ST_OAD Oad,INT8U *pdata);
extern INT8U *get_class17_disp_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet);
extern INT8U get_class17_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
