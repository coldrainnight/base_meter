/*"***********************************************************		
FileName: CLASS20_CONNECT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: Ӧ��������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS20_CONNECT_H_
#define _CLASS20_CONNECT_H_
/*"******************************************************"*/ 

/*"-------------------------------------------------------"*/

/*class20 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT3;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT4;      /*"���������¼����Ա��"*/       
    INT16U  OBIS_ATT5;      /*"���������¼����Ա��"*/  
}ST_CLASS20_OI_AID_OBIS;

/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern const INT8U C20_Att3_FactoryVersion[];
extern const ST_SPECIAL_CLASS_AID_INFO CLASS20_AID_LIST[3];
extern const ST_CLASS20_OI_AID_OBIS CLASS20_OI_AID_OBIS[1];


extern INT8U *get_class20_connect_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet);
extern INT8U get_class20_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
