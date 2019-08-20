/*"*****************************************************************************"*/
/*"  FileName: class2_demand.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  需量类文件   "*/
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

#ifdef THREE_PHASE
/*class2 属性2列表*/
const ST_Class1_6_Object CLASS2OBISLIST[MAX_CLASS2_LIST_NUM] =
{
     /*第3列要对应CLS_1_6_OBJECT_ELE，同时变化*/
    {0x1010,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*正向有功最大需量*/
    {0x1020,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*反向有功最大需量*/
    {0x1030,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND_COM},  /*组合无功1最大需量*/
    {0x1040,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND_COM},  /*组合无功2最大需量*/
    {0x1050,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*第一象限无功最大需量*/
    {0x1060,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*第二象限无功最大需量*/
    {0x1070,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*第三象限无功最大需量*/
    {0x1080,MAXTRIFF_IID,E_ELE_R_MAX_DEMAND},      /*第四象限无功最大需量*/ 
    
    {0x1110,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*冻结期间内正向有功最大需量*/
    {0x1120,MAXTRIFF_IID,E_ELE_A_MAX_DEMAND},      /*冻结期间内反向有功最大需量*/
};

/*-----------------------------------------------------------*/
const ST_OAD_OBIS_INFO stCls2ObisInfo[MAX_CLASS2_LIST_NUM] =
{
    {E_PA_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*正向有功最大需量*/
    {E_NA_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_NA_DEMAND},  /*反向有功最大需量*/
    {E_COM1_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_DEMAND},  /*组合无功1最大需量*/
    {E_COM2_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_DEMAND},  /*组合无功2最大需量*/
    {E_I_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_I_DEMAND},    /*第一象限无功最大需量*/
    {E_II_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_II_DEMAND},  /*第二象限无功最大需量*/
    {E_III_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_III_DEMAND},/*第三象限无功最大需量*/
    {E_IV_R_MAX_DEMAND_BLK, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_DEMAND},  /*第四象限无功最大需量*/ 
    
    {E_OBIS_NULL, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*冻结期间内正向有功最大需量*/
    {E_OBIS_NULL, E_TARIFF_ID, 0, FORMAT_DEMAND, FUN_NULL, E_INFO_NOW_TRIFF_PA_DEMAND},  /*冻结期间内反向有功最大需量*/
};


#endif



/*"************************************************"*/

