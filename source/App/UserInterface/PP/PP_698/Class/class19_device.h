/*"***********************************************************		
FileName: CLASS19_DEVICE.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: �豸������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS19_DEVICE_H_
#define _CLASS19_DEVICE_H_
/*"******************************************************"*/
#include "class_comm.h"


/*class19 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT2;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT3;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT4;      /*"���������¼����Ա��"*/       
    INT16U  OBIS_ATT5;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT6;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT7;      /*"���������¼����Ա��"*/       
    INT16U  OBIS_ATT8;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT9;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT10;     /*"���������¼����Ա��"*/     
}ST_CLASS19_OI_AID_OBIS;

/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS19_AID_LIST[9];
extern const ST_CLASS19_OI_AID_OBIS CLASS19_OI_AID_OBIS[];


#define MAX_CLASS19_AID_LIST_NUM   ARRAY_SIZE(CLASS19_AID_LIST)


extern INT8U get_class19_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
