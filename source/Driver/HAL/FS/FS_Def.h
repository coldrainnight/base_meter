
#ifndef FS_DEF_H
#define FS_DEF_H


#include "system\Sys_def.h"

typedef enum
{
    /*EEP 内文件定义*/

    /*参数文件*/
    E_FILE_LRM = 0,/*法定计量参数文件*/
    E_FILE_UI_PARA_WITH_BACKUP,/*用户接口有RAM备份的参数文件*/
    E_FILE_UI_PARA_NO_BACKUP,/*用户接口参数文件*/
    E_FILE_PM_PARA,/*预付费参数文件*/
    E_FILE_TR_PARA,/*分时参数文件*/
    E_FILE_RCRD_PARA,/*记录快照参数文件*/
    E_FILE_MNT_PARA,/*电表状态监测参数文件*/
    
    #if defined (THREE_PHASE)
    E_FILE_DU_PARA, /*需量参数文件*/
    #endif
    
    /*数据文件*/
    E_FILE_LRM_DATA,    /*法定模块数据文件*/
    E_FILE_PD_DATA,/*掉电存储文件*/
    E_FILE_UI_DATA,     /*交互模块数据文件*/
    E_FILE_MNT,         /*电表状态监测模块存储文件*/
    
    #if defined (THREE_PHASE)  
    E_FILE_DU_DATA,     /*需量数据文件*/
    #endif
    
    E_FILE_PM_DATA,     /*预付费数据文件*/
    E_FILE_RCRD_DATA,     /*记录快照数据文件*/
    E_FILE_AUTO_DATA,     /*自动生产数据文件*/

    
    /*记录快照存储文件*/
    #if defined (SINGLE_PHASE)
    E_FILE_MIN_FRZ_IDX,/*分钟冻结索引*/

    #elif defined (THREE_PHASE)
    E_FILE_PRG_EVNT,/*编程事件记录*/
    E_FILE_OSCILLATOR_ERR_EVNT,/*" 晶振故障事件 "*/
    E_FILE_SYS_RST_EVNT,/*复位故障事件*/
    E_FILE_MONTH_BILL_FRZ,
    E_FILE_MIN_FRZ_IDX,/*分钟冻结索引*/

    #endif

    E_FILE_EEP_END,/*EEP ID结束*/
/////////////////////////////////////////////////////////////////
/////FLASH 内文件定义/////////////////////

    E_FILE_FLASH_START = 0x1000,



    /*记录快照存储文件*/
    #if defined (SINGLE_PHASE)
    E_FILE_OVER_CURR_LA_EVNT,/*过流A*/
    E_FILE_PWR_DOWN_EVNT, /*掉电*/  

    E_FILE_MTR_COVER_OPEN_EVNT,/*开盖事件*/
    E_FILE_RELAY_TRIP_EVNT,/*跳闸事件事件*/
    E_FILE_RELAY_CLOSE_EVNT,/*合闸事件事件*/
    
    E_FILE_PRG_EVNT,/*编程*/
    E_FILE_MTR_CLR_EVNT, /*电表清零*/
    E_FILE_EVNT_CLR_EVNT,/*事件清零*/
    E_FILE_JUST_TIME_EVNT,/*校时事件*/
    E_FILE_BROADCAST_JUST_TIME_EVNT,/*广播校时事件*/
    E_FILE_DAY_TBL_PRG_EVNT,/*日时段表编程*/
    E_FILE_YEAR_TBL_PRG_EVNT,/*年时区表编程*/
    E_FILE_WEEKEND_PRG_EVNT,/*周休日编程事件*/
    E_FILE_HOLIDAY_PRG_EVNT,/*节假日编程*/
    E_FILE_OSCILLATOR_ERR_EVNT,/*" 晶振故障事件 "*/
    E_FILE_SYS_RST_EVNT,/*复位故障事件*/
    E_FILE_BILL_DAY_PRG_EVNT ,/*结算日编程事件*/
    E_FILE_A_ENGY_COM_PRG_EVNT,/*有功组合模式字编程事件*/
    E_FILE_TRIFF_PRG_EVNT,/*费率表编程事件*/
    E_FILE_STEP_PRG_EVNT,/*阶梯表编程事件*/
    E_FILE_KEY_UPDATE_EVNT,/*密钥更新事件*/
    E_FILE_CARD_PLUG_IN_ERR_EVNT,/*插卡异常事件*/
    E_FILE_BUY_ENGY_EVNT,/*购电事件*/
    E_FILE_REFUND_EVNT,/*退费事件*/
    E_FILE_RELAY_ERR_EVNT,/*继电器错误*/
    E_FILE_PWR_FAILURE_EVNT,/*电源异常事件*/
    E_FILE_RTC_ERR_EVNT,/*时钟故障事件*/
    E_FILE_MS_ERR_EVNT,/*计量芯片故障事件*/
	E_FILE_MGMT_COMM_ERR_EVNT,/*通信异常事件*/
	E_FILE_UPGRADE_EVNT,/*非法定单元升级事件*/
	E_FILE_NOLEG_REPALCE_EVNT,/*非法定单元更换事件*/
	

    E_FILE_MMT_FRZ,/*瞬时冻结*/
    
    E_FILE_MIN_FRZ,/*分钟冻结*/
    E_FILE_HOUR_FRZ,/*小时冻结*/
    E_FILE_DAY_FRZ,/*日冻结*/
    E_FILE_MONTH_BILL_FRZ,/*月结算冻结*/
    E_FILE_MONTH_FRZ ,/*月冻结*/
    E_FILE_DAY_TBL_SW_FRZ,/*日时段表切换冻结*/
    E_FILE_YEAR_TBL_SW_FRZ,/*年市区表切换*/
    E_FILE_TRIFF_SW_FRZ,/*费率电价切换*/
    E_FILE_STEP_SW_FRZ, /*" 阶梯切换冻结 "*/
    E_FILE_STEP_BILL_FRZ, /*" 阶梯结算冻结 "*/

    
    E_FILE_PD_PON_EVNT,/*" 掉上电记录 "*/
    E_FILE_MSRL_EVNT,
	E_FILE_MSRN_EVNT,



    #elif defined (THREE_PHASE)
    E_FILE_PD_PON_EVNT,/*" 掉上电记录 "*/
    E_FILE_PARA_ERR_EVNT, /*" 参数错误记录 "*/
    E_FILE_ENG_ERR_EVNT, /*" 电量错误记录 "*/
    
    E_FILE_LOST_VOLT_LA_EVNT      ,/*A相失压*/
    E_FILE_LOST_VOLT_LB_EVNT      ,/*B相失压*/
    E_FILE_LOST_VOLT_LC_EVNT      ,/*C相失压*/

    E_FILE_LACK_VOLT_LA_EVNT      ,/*A相欠压*/
    E_FILE_LACK_VOLT_LB_EVNT      ,/*B相欠压*/
    E_FILE_LACK_VOLT_LC_EVNT      ,/*C相欠压*/

    E_FILE_OVER_VOLT_LA_EVNT  ,/*A相过压*/
    E_FILE_OVER_VOLT_LB_EVNT  ,/*B相过压*/
    E_FILE_OVER_VOLT_LC_EVNT  ,/*C相过压*/

    E_FILE_BRK_PHASE_LA_EVNT  ,/*A相断相*/
    E_FILE_BRK_PHASE_LB_EVNT  ,/*B相断相*/
    E_FILE_BRK_PHASE_LC_EVNT  ,/*C相断相*/

    E_FILE_LOST_CURR_LA_EVNT   ,/*A相失流*/
    E_FILE_LOST_CURR_LB_EVNT    ,/*B相失流*/
    E_FILE_LOST_CURR_LC_EVNT    ,/*C相失流*/

    E_FILE_OVER_CURR_LA_EVNT    ,/*A相过流*/
    E_FILE_OVER_CURR_LB_EVNT    ,/*B相过流*/
    E_FILE_OVER_CURR_LC_EVNT    ,/*C相过流*/

    E_FILE_BRK_CURR_LA_EVNT     ,/*A相断流*/
    E_FILE_BRK_CURR_LB_EVNT     ,/*B相断流*/
    E_FILE_BRK_CURR_LC_EVNT     ,/*C相断流*/

    E_FILE_PWR_REVERSE_EVNT         ,/*功率反向*/
    E_FILE_PWR_REVERSE_LA_EVNT      ,/*A功率反向*/
    E_FILE_PWR_REVERSE_LB_EVNT      ,/*B功率反向*/
    E_FILE_PWR_REVERSE_LC_EVNT      ,/*C功率反向*/

    E_FILE_OVER_LOAD_LA_EVNT        ,
    E_FILE_OVER_LOAD_LB_EVNT        ,
    E_FILE_OVER_LOAD_LC_EVNT        ,

    E_FILE_OVER_PA_DEMAND_EVNT      ,
    E_FILE_OVER_NA_DEMAND_EVNT      ,

    E_FILE_OVER_R_DEMAND_I_EVNT     ,
    E_FILE_OVER_R_DEMAND_II_EVNT    ,
    E_FILE_OVER_R_DEMAND_III_EVNT   ,
    E_FILE_OVER_R_DEMAND_IV_EVNT    ,

    E_FILE_FACTOR_LOW_EVNT      ,
    E_FILE_ALL_LOST_VOLT_EVNT   ,
    E_FILE_AUX_PWR_DOWN_EVNT    , 
    E_FILE_VOLT_INVERSE_EVNT    ,
    E_FILE_CURR_INVERSE_EVNT    ,

    E_FILE_PWR_DOWN_EVNT        ,
    E_FILE_MTR_CLR_EVNT         ,

    E_FILE_DEMAND_CLR_EVNT      ,
    E_FILE_EVNT_CLR_EVNT        ,
    E_FILE_JUST_TIME_EVNT       ,
    E_FILE_DAY_TBL_PRG_EVNT     ,
    E_FILE_YEAR_TBL_PRG_EVNT  ,
    E_FILE_WEEKEND_PRG_EVNT     ,
    E_FILE_BILL_DAY_PRG_EVNT    ,

    E_FILE_MTR_COVER_OPEN_EVNT  ,

    E_FILE_TAIL_COVER_OPEN_EVNT ,

    E_FILE_VOLT_IMBALANCE_EVNT  ,
    E_FILE_CURR_IMBALANCE_EVNT  ,

    E_FILE_RELAY_TRIP_EVNT      ,
    E_FILE_RELAY_CLOSE_EVNT     ,
    E_FILE_HOLIDAY_PRG_EVNT     ,
    E_FILE_A_ENGY_COM_PRG_EVNT  ,
    E_FILE_R_ENGY_COM_PRG_EVNT  ,

    E_FILE_TRIFF_PRG_EVNT               ,
    E_FILE_STEP_PRG_EVNT                ,
    E_FILE_KEY_UPDATE_EVNT              ,
    E_FILE_CARD_PLUG_IN_ERR_EVNT        ,
    E_FILE_BUY_ENGY_EVNT                ,
    E_FILE_REFUND_EVNT                  ,

    E_FILE_CONSTANT_MGNT_DISTURB_EVNT   ,

    E_FILE_RELAY_ERR_EVNT               ,
    E_FILE_PWR_FAILURE_EVNT             ,

    E_FILE_CURR_IMBALANCE_BADLY_EVNT    ,
    E_FILE_RTC_ERR_EVNT,/*时钟故障事件*/
    E_FILE_MS_ERR_EVNT,/*计量芯片故障事件*/
    E_FILE_SPI_ERR_EVNT,
    E_FILE_UPGRADE_EVNT,
    E_FILE_NOLEG_REPALCE_EVNT,

    /*" 冻结文件 "*/

    E_FILE_MMT_FRZ              ,
    E_FILE_MIN_FRZ              ,
    E_FILE_HOUR_FRZ,/*小时冻结*/
    E_FILE_DAY_FRZ              ,
    E_FILE_MONTH_FRZ            ,
    E_FILE_YEAR_TBL_SW_FRZ    ,    
    E_FILE_DAY_TBL_SW_FRZ       ,
    E_FILE_TRIFF_SW_FRZ     ,
    E_FILE_STEP_SW_FRZ      , /*" 阶梯切换冻结 "*/
    E_FILE_STEP_BILL_FRZ        , /*" 阶梯结算冻结 "*/
    
    #endif

    
    
    E_FILE_FLASH_END


}ENUM_FS_ID;




#endif

