/*"***********************************************************		
FileName: CLASS21_ESAM.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ESAM�ӿ���ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS21_ESAM_H_
#define _CLASS21_ESAM_H_
/*"******************************************************"*/  

/*class19 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT2;      /*"1.���������¼����Ա��"*/  
    INT16U  OBIS_ATT3;      /*"2.���������¼����Ա��"*/  
    INT16U  OBIS_ATT4;      /*"3.���������¼����Ա��"*/       
    INT16U  OBIS_ATT5;      /*"4.���������¼����Ա��"*/  
    INT16U  OBIS_ATT6;      /*"5.���������¼����Ա��"*/  
    INT16U  OBIS_ATT7;      /*"6.���������¼����Ա��"*/       
    INT16U  OBIS_ATT14;      /*"7.���������¼����Ա��"*/  
    INT16U  OBIS_ATT15;      /*"8.���������¼����Ա��"*/  
}ST_CLASS21_OI_AID_OBIS;


/*�ⲿ�������������*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS21_AID_LIST[8];
extern const ST_CLASS21_OI_AID_OBIS CLASS21_OI_AID_OBIS[1];

extern INT8U get_class21_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
