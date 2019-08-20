/*"*****************************************************************************"*/
/*"  FileName: class2_demand.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �������ļ�   "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h"

#ifdef THREE_PHASE
/*class2 ����2�б�*/
const ST_Class1_6_Object CLASS2OBISLIST[MAX_CLASS2_LIST_NUM] =
{
     /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/
    {0x1010,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*�����й��������*/
    {0x1020,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*�����й��������*/
    {0x1030,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND_COM},  /*����޹�1�������*/
    {0x1040,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND_COM},  /*����޹�2�������*/
    {0x1050,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*��һ�����޹��������*/
    {0x1060,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*�ڶ������޹��������*/
    {0x1070,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*���������޹��������*/
    {0x1080,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*���������޹��������*/ 
    
    {0x1110,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*�����ڼ��������й��������*/
    {0x1120,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*�����ڼ��ڷ����й��������*/
};

/*-----------------------------------------------------------*/
const ST_OAD_OBIS_INFO stCls2ObisInfo[MAX_CLASS2_LIST_NUM] =
{
    {E_PA_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*�����й��������*/
    {E_NA_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_NA_DEMAND},  /*�����й��������*/
    {E_COM1_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_DEMAND},  /*����޹�1�������*/
    {E_COM2_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_DEMAND},  /*����޹�2�������*/
    {E_I_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_I_DEMAND},    /*��һ�����޹��������*/
    {E_II_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_II_DEMAND},  /*�ڶ������޹��������*/
    {E_III_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_III_DEMAND},/*���������޹��������*/
    {E_IV_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_DEMAND},  /*���������޹��������*/ 
    
    {E_OBIS_NULL, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*�����ڼ��������й��������*/
    {E_OBIS_NULL, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*�����ڼ��ڷ����й��������*/
};


#endif



/*"************************************************"*/

