/*"*****************************************************************************"*/
/*"  FileName: class4_phase_varible.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  功率类文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h" 
/************************************************************/
/*class4 属性2列表*/
const ST_Class1_6_Object CLASS4OBISLIST[MAX_CLASS4_LIST_NUM] =
{
     /*第3列要对应CLS_1_6_OBJECT_ELE，同时变化*/
     {0x2004,CLASS4_MAX_IID,E_ELE_A_POWER},      /*有功功率*/

#ifdef THREE_PHASE
    {0x2005,CLASS4_MAX_IID,E_ELE_R_POWER},      /*无功功率*/
    {0x2006,CLASS4_MAX_IID,E_ELE_V_POWER},      /*视在功率*/
    {0x2007,CLASS4_MAX_IID,E_ELE_A_POWER},      /*一分钟平均有功功率*/
    {0x2008,CLASS4_MAX_IID,E_ELE_R_POWER},      /*一分钟平均无功功率*/
#endif

    {0x200a,CLASS4_MAX_IID,E_ELE_FACTOR}        /*功率因数*/

};

/*-----------------------------------------------------------*/
const ST_OAD_OBIS_INFO stCls4ObisInfo[MAX_CLASS4_LIST_NUM] =
{
    {E_ACTIVE_PWR_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_PW, FUN_NULL, E_INFO_PA_POWER},    /*有功功率*/
#ifdef THREE_PHASE
    {E_REACTIVE_PWR_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_R_PW, FUN_NULL, E_INFO_RA_POWER},    /*无功功率*/
    {E_AP_PWR_BLK,      E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_VA_PW, FUN_NULL, E_INFO_SUM_POWER},  /*视在功率*/
    {E_MINACTPOWER_BLK, E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_PW, FUN_NULL, E_INFO_PA_POWER},      /*一分钟平均有功功率*/
    {E_MINREACTPOWER_BLK,E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_R_PW, FUN_NULL, E_INFO_RA_POWER},   /*一分钟平均无功功率*/
#endif

    {E_FACTOR_BLK, E_PHASE_VARIBLE_SUM_ID, 0, FORMAT_FACTOR, FUN_NULL, E_INFO_COS_CREDIT_SUM}      /*功率因数*/

};


/*"************************************************"*/


