/*"*****************************************************************************"*/
/*"  FileName: class6_data_varible.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  数据变量类文件   "*/
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
/*class6 属性2列表*/
const ST_Class1_6_Object CLASS6OBISLIST[MAX_CLASS6_LIST_NUM] =
{
     /*第3列要对应CLS_1_6_OBJECT_ELE，同时变化*/    
    {0x200f,1,E_ELE_FEQ},            /* 1.电网频率 */
    {0x2011,1,E_ELE_CLK_VOLT},       /* 2.时钟电池电压 */    
    {0x2014,7,E_ELE_STATUS_8},       /* 3.电能表运行状态字 */
    {0x201A,1,E_ELE_PRICE},          /* 4.当前电价 */
    {0x201B,1,E_ELE_PRICE},          /* 5.当前费率电价 */
    {0x201C,1,E_ELE_PRICE},          /* 6.当前阶梯电价 */
    {0x202d,1,E_ELE_MONEY},          /* 7.（当前）透支金额*/ 
    {0x202E,1,E_ELE_MONEY},          /* 8.累计购电金额 */
    {0x2031,1,E_ELE_A_ENGY},         /*9.月度用电量*/
    {0x2032,1,E_ELE_A_ENGY},         /*10.阶梯结算用电量*/
    {0x2040,1,E_ELE_STATUS_16},      /*11.控制命令执行状态字*/
    {0x2041,1,E_ELE_STATUS_16},      /*12.控制命令错误状态字*/
    {0x2042,1,E_ELE_STATUS_32},      /*13.控制命令执行状态字*/
    {0x4022,1,E_ELE_STATUS_16},      //14插卡状态字		
    {0x2013,1,E_ELE_CLK_WORK_TIME},  /* 15.时钟电池工作时间 */   

#ifdef THREE_PHASE
    {0x2010,1,E_ELE_TEMP},           /* 16.表内温度  */
    {0x2012,1,E_ELE_LOW_POWER_VOLT}, /* 17.停电抄表电池电压  */ 
    {0x2017,1,E_ELE_CURR_A_DEMAND},  /* 18.当前有功需量 */    
    {0x2018,1,E_ELE_CURR_R_DEMAND},  /* 19.当前无功需量 */
    {0x2026,1,E_ELE_IMBANLCE_RATE},  /* 20.电压不平衡率 */
    {0x2027,1,E_ELE_IMBANLCE_RATE},  /* 21.电流不平衡率 */
    {0x2029,4,E_ELE_VAH},            /* 22.安时值 */    
    {0x2131,2,E_ELE_VOLT_RATE},      /*23.A电压合格率 当日电压合格率与当月电压合格率*/
    {0x2132,2,E_ELE_VOLT_RATE},      /*24.B电压合格率*/
    {0x2133,2,E_ELE_VOLT_RATE}       /*25.C电压合格率*/       
#endif
};


/*class6 属性2 OBIS列表*/
const ST_OAD_OBIS_INFO stCls6ObisInfo[MAX_CLASS6_LIST_NUM] =
{
    {E_FREQ,           1, 0, FORMAT_FREQ,  FUN_NULL,     E_INFO_NULL},   /* 1.电网频率 */
    {E_CLK_BAT_VOLT,   1, 0, FORMAT_BV,    FUN_NULL,     E_INFO_NULL},   /* 2.时钟电池电压 */    
    {E_RUN_STA_1,      7, 0, FORMAT_WORD,  FUN_RUNSTATUS,E_INFO_NULL},   /* 3.电能表运行状态字 */
#ifdef PM_MODLE    
    {E_CUR_PRICE,      1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_PRICE},   /* 4.当前电价 */
    {E_TARIFF_PRICE,   1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_TRIFF_PRICE},   /* 5.当前费率电价 */
    {E_STEPLEVEL_PRICE,1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_STEP_PRICE},   /* 6.当前阶梯电价 */
    {E_OVERDRAFT_MONEY,1, 0, FORMAT_MONEY, FUN_NULL,     E_INFO_OVERDRAW_MONEY},   /* 7.（当前）透支金额*/ 
    {E_BUY_ENGY_TOTAL, 1, 0, FORMAT_MONEY, FUN_NULL,     E_INFO_SUM},   /* 8.累计购电金额 */
    {E_ENGY_CONSUMPTION_MONTH,     1, 0, FORMAT_BILLENGY, FUN_NULL, E_INFO_NOW_LAST_SUM_ENGY},   /*9 .月度用电量*/
    {E_ENGY_CONSUMPTION_STEP_BILL, 1, 0, FORMAT_BILLENGY, FUN_NULL, E_INFO_NOW_STEP_SUM_ENGY},   /*10.阶梯结算用电量*/
    {E_CTRCMD_STATUS,  1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*11.控制命令执行状态字*/
    {E_CTRERR_STATUS,  1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*12.控制命令错误状态字*/
    {E_CTRCMD_STATUS,  2, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*13.控制命令执行状态字 两个状态*/
    {E_INSETCARD_STATUS,1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},         /*14插卡状态字 */
#endif    
    {E_CLK_BAT_TIME,  1, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_TIME},       /*15.时钟电池工作时间 */   

#ifdef THREE_PHASE
    {E_MTR_TEMPERATRUE, 1, 0, FORMAT_S2_1_UBCD2_1, FUN_NULL, E_INFO_NULL}, /* 16.表内温度  */
    {E_DISP_BAT_VOLT, 1, 0, FORMAT_BV, FUN_NULL, E_INFO_NULL},             /* 17.停电抄表电池电压  */
    {E_CURRENT_A_DEMAND, 1, 0, FORMAT_PW, FUN_NULL, E_INFO_NOW_PA_DEMAND}, /* 18.当前有功需量 */    
    {E_CURRENT_R_DEMAND, 1, 0, FORMAT_R_PW, FUN_NULL, E_INFO_NOW_R_DEMAND},/* 19.当前无功需量 */	
    {E_VOLTIMB_RATE, 1, 0, FORMAT_U2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},    /* 20.电压不平衡率 */
    {E_CURRIMB_RATE, 1, 0, FORMAT_U2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},    /* 21.电流不平衡率 */
    {E_AH_BLK, E_PHASE_VARIBLE_SUM_ID, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},  /* 22.安时值 */    
    {E_DA_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL/*显示函数中特殊处理*/},  /*23.A电压合格率 当日电压合格率与当月电压合格率*/
    {E_DB_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*24.B电压合格率*/
    {E_DC_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*25.C电压合格率*/        
#endif	
};
/*"************************************************"*/

