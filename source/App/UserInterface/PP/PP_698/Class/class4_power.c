/*"*****************************************************************************"*/
/*"  FileName: class4_phase_varible.c"*/
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
/************************************************************/
/*class4 ����2�б�*/
const ST_Class1_6_Object CLASS4OBISLIST[MAX_CLASS4_LIST_NUM] =
{
     /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/
     {0x2004,CLASS4_MAX_IID,E_ELE_A_POWER},      /*�й�����*/

#ifdef THREE_PHASE
    {0x2005,CLASS4_MAX_IID,E_ELE_R_POWER},      /*�޹�����*/
    {0x2006,CLASS4_MAX_IID,E_ELE_V_POWER},      /*���ڹ���*/
    {0x2007,CLASS4_MAX_IID,E_ELE_A_POWER},      /*һ����ƽ���й�����*/
    {0x2008,CLASS4_MAX_IID,E_ELE_R_POWER},      /*һ����ƽ���޹�����*/
#endif

    {0x200a,CLASS4_MAX_IID,E_ELE_FACTOR}        /*��������*/

};

/*-----------------------------------------------------------*/
const ST_OAD_OBIS_INFO stCls4ObisInfo[MAX_CLASS4_LIST_NUM] =
{
    {E_ACTIVE_PWR_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_PW, FUN_NULL, E_INFO_PA_POWER},    /*�й�����*/
#ifdef THREE_PHASE
    {E_REACTIVE_PWR_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_R_PW, FUN_NULL, E_INFO_RA_POWER},    /*�޹�����*/
    {E_AP_PWR_BLK,      E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_VA_PW, FUN_NULL, E_INFO_SUM_POWER},  /*���ڹ���*/
    {E_MINACTPOWER_BLK, E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_PW, FUN_NULL, E_INFO_PA_POWER},      /*һ����ƽ���й�����*/
    {E_MINREACTPOWER_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_R_PW, FUN_NULL, E_INFO_RA_POWER},   /*һ����ƽ���޹�����*/
#endif

    {E_FACTOR_BLK, E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_FACTOR, FUN_NULL, E_INFO_COS_CREDIT_SUM}      /*��������*/

};


/*"************************************************"*/


