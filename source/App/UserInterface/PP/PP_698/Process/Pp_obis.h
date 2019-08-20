/*"***********************************************************		
FileName: pp_bois.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 协议处理对象类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_OBIS_H
#define _PP_OBIS_H
/********************************************************/
/*OI定义*/
typedef enum
{    
    /*电量类对象标识*/
    OI_ENGY_COM                   = 0x0000,       /*组合有功电能*/
    OI_PA_ENGY                    = 0x0010,       /*正向有功电能*/
    OI_PA_ENGY_LA                 = 0x0011,       /*A相正向有功电能*/
    OI_PA_ENGY_LB                 = 0x0012,       /*B相正向有功电能*/
    OI_PA_ENGY_LC                 = 0x0013,       /*C相正向有功电能*/
    OI_NA_ENGY                    = 0x0020,       /*反向有功电能*/
    OI_NA_ENGY_LA                 = 0x0021,       /*A相反向有功电能*/
    OI_NA_ENGY_LB                 = 0x0022,       /*B相反向有功电能*/
    OI_NA_ENGY_LC                 = 0x0023,       /*C相反向有功电能*/
    OI_R_ENGY_COM1                = 0x0030,       /*组合无功1电能*/
    OI_R_ENGY_LA_COM1             = 0x0031,       /*A相组合无功1电能*/
    OI_R_ENGY_LB_COM1             = 0x0032,       /*B相组合无功1电能*/
    OI_R_ENGY_LC_COM1             = 0x0033,       /*C相组合无功1电能*/
    OI_R_ENGY_COM2                = 0x0040,       /*组合无功2电能*/
    OI_R_ENGY_LA_COM2             = 0x0041,       /*A相组合无功2电能*/
    OI_R_ENGY_LB_COM2             = 0x0042,       /*B相组合无功2电能*/
    OI_R_ENGY_LC_COM2             = 0x0043,       /*C相组合无功2电能*/
    OI_R_ENGY_I                   = 0x0050,       /*第一象限无功电能*/
    OI_R_ENGY_I_LA                = 0x0051,       /*A相第一象限无功电能*/
    OI_R_ENGY_I_LB                = 0x0052,       /*B相第一象限无功电能*/
    OI_R_ENGY_I_LC                = 0x0053,       /*C相第一象限无功电能*/
    OI_R_ENGY_II                  = 0x0060,       /*第二象限无功电能*/
    OI_R_ENGY_II_LA               = 0x0061,       /*A相第二象限无功电能*/
    OI_R_ENGY_II_LB               = 0x0062,       /*B相第二象限无功电能*/
    OI_R_ENGY_II_LC               = 0x0063,       /*C相第二象限无功电能*/
    OI_R_ENGY_III                 = 0x0070,       /*第三象限无功电能*/
    OI_R_ENGY_III_LA              = 0x0071,       /*A相第三象限无功电能*/
    OI_R_ENGY_III_LB              = 0x0072,       /*B相第三象限无功电能*/
    OI_R_ENGY_III_LC              = 0x0073,       /*C相第三象限无功电能*/
    OI_R_ENGY_IV                  = 0x0080,       /*第四象限无功电能*/
    OI_R_ENGY_IV_LA               = 0x0081,       /*A相第四象限无功电能*/
    OI_R_ENGY_IV_LB               = 0x0082,       /*B相第四象限无功电能*/
    OI_R_ENGY_IV_LC               = 0x0083,       /*C相第四象限无功电能*/
    OI_PAP_ENGY                   = 0x0090,       /*正向视在电能*/
    OI_PAP_ENGY_LA                = 0x0091,       /*A相正向视在电能*/
    OI_PAP_ENGY_LB                = 0x0092,       /*B相正向视在电能*/
    OI_PAP_ENGY_LC                = 0x0093,       /*C相正向视在电能*/
    OI_NAP_ENGY                   = 0x00A0,       /*反向视在电能*/
    OI_NAP_ENGY_LA                = 0x00A1,       /*A相反向视在电能*/
    OI_NAP_ENGY_LB                = 0x00A2,       /*B相反向视在电能*/
    OI_NAP_ENGY_LC                = 0x00A3,       /*C相反向视在电能*/
    /*电量类对象标识 END*/

    /*最大需量类对象标识*/
    OI_PA_MAX_DEMAND              = 0x1010,       /*正向有功最大需量*/
    OI_PA_MAX_DEMAND_LA           = 0x1011,       /*A相正向有功最大需量*/
    OI_PA_MAX_DEMAND_LB           = 0x1012,       /*B相正向有功最大需量*/
    OI_PA_MAX_DEMAND_LC           = 0x1013,       /*C相正向有功最大需量*/
    OI_NA_MAX_DEMAND              = 0x1020,       /*反向有功最大需量*/
    OI_NA_MAX_DEMAND_LA           = 0x1021,       /*A相反向有功最大需量*/
    OI_NA_MAX_DEMAND_LB           = 0x1022,       /*B相反向有功最大需量*/
    OI_NA_MAX_DEMAND_LC           = 0x1023,       /*C相反向有功最大需量*/
    OI_R_MAX_DEMAND_COM1          = 0x1030,       /*组合无功1最大需量*/
    OI_R_MAX_DEMAND_LA_COM1       = 0x1031,       /*A相组合无功1最大需量*/
    OI_R_MAX_DEMAND_LB_COM1       = 0x1032,       /*B相组合无功1最大需量*/
    OI_R_MAX_DEMAND_LC_COM1       = 0x1033,       /*C相组合无功1最大需量*/
    OI_R_MAX_DEMAND_COM2          = 0x1040,       /*组合无功2最大需量*/
    OI_R_MAX_DEMAND_LA_COM2       = 0x1041,       /*A相组合无功2最大需量*/
    OI_R_MAX_DEMAND_LB_COM2       = 0x1042,       /*B相组合无功2最大需量*/
    OI_R_MAX_DEMAND_LC_COM2       = 0x1043,       /*C相组合无功2最大需量*/
    OI_R_MAX_DEMAND_I             = 0x1050,       /*第一象限最大需量*/
    OI_R_MAX_DEMAND_I_LA          = 0x1051,       /*A相第一象限最大需量*/
    OI_R_MAX_DEMAND_I_LB          = 0x1052,       /*B相第一象限最大需量*/
    OI_R_MAX_DEMAND_I_LC          = 0x1053,       /*C相第一象限最大需量*/
    OI_R_MAX_DEMAND_II            = 0x1060,       /*第二象限最大需量*/
    OI_R_MAX_DEMAND_II_LA         = 0x1061,       /*A相第二象限最大需量*/
    OI_R_MAX_DEMAND_II_LB         = 0x1062,       /*B相第二象限最大需量*/
    OI_R_MAX_DEMAND_II_LC         = 0x1063,       /*C相第二象限最大需量*/
    OI_R_MAX_DEMAND_III           = 0x1070,       /*第三象限最大需量*/
    OI_R_MAX_DEMAND_III_LA        = 0x1071,       /*A相第三象限最大需量*/
    OI_R_MAX_DEMAND_III_LB        = 0x1072,       /*B相第三象限最大需量*/
    OI_R_MAX_DEMAND_III_LC        = 0x1073,       /*C相第三象限最大需量*/
    OI_R_MAX_DEMAND_IV            = 0x1080,       /*第四象限最大需量*/
    OI_R_MAX_DEMAND_IV_LA         = 0x1081,       /*A相第四象限最大需量*/
    OI_R_MAX_DEMAND_IV_LB         = 0x1082,       /*B相第四象限最大需量*/
    OI_R_MAX_DEMAND_IV_LC         = 0x1083,       /*C相第四象限最大需量*/
    OI_PAP_MAX_DEMAND             = 0x1090,       /*正向视在最大需量*/
    OI_PAP_MAX_DEMAND_LA          = 0x1091,       /*A相正向视在最大需量*/
    OI_PAP_MAX_DEMAND_LB          = 0x1092,       /*B相正向视在最大需量*/
    OI_PAP_MAX_DEMAND_LC          = 0x1093,       /*C相正向视在最大需量*/
    OI_NAP_MAX_DEMAND             = 0x10A0,       /*反向视在最大需量*/
    OI_NAP_MAX_DEMAND_LA          = 0x10A1,       /*A相反向视在最大需量*/
    OI_NAP_MAX_DEMAND_LB          = 0x10A2,       /*B相反向视在最大需量*/
    OI_NAP_MAX_DEMAND_LC          = 0x10A3,       /*C相反向视在最大需量*/
    /*最大需量类对象标识END*/
    
    /*冻结期间最大需量类对象标识*/
    OI_FREEZE_PA_MAX_DEMAND              = 0x1110,       /*冻结周期内正向有功最大需量*/
    OI_FREEZE_PA_MAX_DEMAND_LA           = 0x1111,       /*A相冻结周期内正向有功最大需量*/
    OI_FREEZE_PA_MAX_DEMAND_LB           = 0x1112,       /*B相冻结周期内正向有功最大需量*/
    OI_FREEZE_PA_MAX_DEMAND_LC           = 0x1113,       /*C相冻结周期内正向有功最大需量*/
    OI_FREEZE_NA_MAX_DEMAND              = 0x1120,       /*冻结周期内反向有功最大需量*/
    OI_FREEZE_NA_MAX_DEMAND_LA           = 0x1121,       /*A相冻结周期内反向有功最大需量*/
    OI_FREEZE_NA_MAX_DEMAND_LB           = 0x1122,       /*B相冻结周期内反向有功最大需量*/
    OI_FREEZE_NA_MAX_DEMAND_LC           = 0x1123,       /*C相冻结周期内反向有功最大需量*/
    OI_FREEZE_R_MAX_DEMAND_COM1          = 0x1130,       /*冻结周期组合无功1最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LA_COM1       = 0x1131,       /*冻结周期A相组合无功1最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LB_COM1       = 0x1132,       /*冻结周期B相组合无功1最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LC_COM1       = 0x1133,       /*冻结周期C相组合无功1最大需量*/
    OI_FREEZE_R_MAX_DEMAND_COM2          = 0x1140,       /*冻结周期组合无功2最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LA_COM2       = 0x1141,       /*冻结周期A相组合无功2最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LB_COM2       = 0x1142,       /*冻结周期B相组合无功2最大需量*/
    OI_FREEZE_R_MAX_DEMAND_LC_COM2       = 0x1143,       /*冻结周期C相组合无功2最大需量*/
    OI_FREEZE_R_MAX_DEMAND_I             = 0x1150,       /*冻结周期第一象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_I_LA          = 0x1151,       /*冻结周期A相第一象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_I_LB          = 0x1152,       /*冻结周期B相第一象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_I_LC          = 0x1153,       /*冻结周期C相第一象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_II            = 0x1160,       /*冻结周期第二象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_II_LA         = 0x1161,       /*冻结周期A相第二象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_II_LB         = 0x1162,       /*冻结周期B相第二象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_II_LC         = 0x1163,       /*冻结周期C相第二象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_III           = 0x1170,       /*冻结周期第三象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_III_LA        = 0x1171,       /*冻结周期A相第三象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_III_LB        = 0x1172,       /*冻结周期B相第三象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_III_LC        = 0x1173,       /*冻结周期C相第三象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_IV            = 0x1180,       /*冻结周期第四象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_IV_LA         = 0x1181,       /*冻结周期A相第四象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_IV_LB         = 0x1182,       /*冻结周期B相第四象限最大需量*/
    OI_FREEZE_R_MAX_DEMAND_IV_LC         = 0x1183,       /*冻结周期C相第四象限最大需量*/
    OI_FREEZE_PAP_MAX_DEMAND             = 0x1190,       /*冻结周期正向视在最大需量*/
    OI_FREEZE_PAP_MAX_DEMAND_LA          = 0x1191,       /*冻结周期A相正向视在最大需量*/
    OI_FREEZE_PAP_MAX_DEMAND_LB          = 0x1192,       /*冻结周期B相正向视在最大需量*/
    OI_FREEZE_PAP_MAX_DEMAND_LC          = 0x1193,       /*冻结周期C相正向视在最大需量*/
    OI_FREEZE_NAP_MAX_DEMAND             = 0x11A0,       /*冻结周期反向视在最大需量*/
    OI_FREEZE_NAP_MAX_DEMAND_LA          = 0x11A1,       /*冻结周期A相反向视在最大需量*/
    OI_FREEZE_NAP_MAX_DEMAND_LB          = 0x11A2,       /*冻结周期B相反向视在最大需量*/
    OI_FREEZE_NAP_MAX_DEMAND_LC          = 0x11A3,       /*冻结周期C相反向视在最大需量*/   
    /*最大需量类对象标识END*/

    /*变量类对象标识*/
    OI_VOLTAGE                    = 0x2000,       /*电压*/
    OI_CURRENT                    = 0x2001,       /*电流*/
    OI_VOLT_ANGLE                 = 0x2002,       /*电压相角*/
    OI_CURR_ANGLE                 = 0x2003,       /*电流相角*/
    OI_ACTIVE_POWER               = 0x2004,       /*有功功率*/
    OI_REACTIVE_POWER             = 0x2005,       /*无功功率*/
    OI_VISUAL_POWER               = 0x2006,       /*视在功率*/
    OI_AVERAGE_ACTIVE_POWER_MIN   = 0x2007,       /*一分钟平均有功功率*/        
    OI_AVERAGE_REACTIVE_POWER_MIN = 0x2008,       /*一分钟平均无功功率*/
    OI_AP_PWR_MIN_AVER            = 0x2009,       /*一分钟平均视在功率*/ 
    OI_FACTOR                     = 0x200A,       /*功率因数*/
    OI_POWER_FREQ                 = 0x200F,       /*电网频率*/
    OI_TEMPERATURE                = 0x2010,       /*表内温度*/
    OI_RTC_POWER_VOL              = 0x2011,       /*时钟电池电压*/
    OI_LOW_POWER_VOL              = 0x2012,       /*停电抄表电池电压*/
    OI_RTC_POWER_TIME             = 0x2013,       /*时钟电池工作时间*/
    OI_STATUS_METER               = 0x2014,       /*电表运行状态字*/
    OI_REPORT_METER               = 0x2015,       /*电能表主动上报*/ 
    OI_CURRENT_P_DEMAND           = 0x2017,       /*当前有功需量*/
    OI_CURRENT_R_DEMAND           = 0x2018,       /*当前无功需量*/
    OI_CURRENT_VISUAL_DEMAND      = 0x2019,       /*当前视在需量*/ 
    OI_PRICE_NOW                  = 0x201A,       /*当前电价*/ 
    OI_TARIFF_PRICE_NOW           = 0x201B,       /*当前费率电价*/ 
    OI_STEP_PRICE_NOW             = 0x201C,       /*当前阶梯电价*/ 
    OI_EVENT_START_TIME           = 0x201E,       /*事件发生时间*/ 
    OI_EVENT_END_TIME             = 0x2020,       /*事件结束时间*/ 
    OI_DATA_FREEZE_TIME           = 0x2021,       /*数据冻结时间*/ 
    OI_EVENT_RECORD_NUM           = 0x2022,       /*事件记录序号*/ 
    OI_FREEZE_RECORD_NUM          = 0x2023,       /*冻结记录序号*/ 
    OI_EVENT_SOURCE               = 0x2024,       /*事件发生源*/ 
    OI_EVENT_CURRENT_VALUE        = 0x2025,       /*事件当前值:事件发生次数,事件累计时间*/ 
    OI_VOLT_IMBALANCE_RATE        = 0x2026,       /*电压不平衡率*/ 
    OI_CURR_IMBALANCE_RATE        = 0x2027,       /*电流不平衡率*/ 
    OI_REROAD_RATE                = 0x2028,       /*负载率*/ 
    OI_VA                         = 0x2028,       /*安时值*/ 
    OI_CURRENT_MONEY_PAG          = 0x202C,       /*（当前）钱包文件:剩余金额、购电次数*/ 
    OI_REMAIN_MONEY_NEGA          = 0x202D,       /*（当前）透支金额*/
    OI_TOTAL_BUY_MONEY            = 0x202E,       /*累计购电金额*/
    OI_ENGY_CONSUMPTION_MONTH     = 0x2031,       /*月度用电量*/
    OI_ENGY_CONSUMPTION_STEP_BILL = 0x2032,       /*阶梯结算用电量*/    
    OI_STATUS_CTR_CMD             = 0x2040,       /*控制命令执行状态字*/
    OI_ERR_CTR_CMD                = 0x2041,       /*控制命令错误状态字*/
    OI_STATUS_CTR_CMD_NEW         = 0x2042,       /*合并后的控制命令状态字*/
    //OI_VOLT_STAT_RT               = 0x2130,       /*总电压合格率 ----已没用*/
    OI_VOLT_STAT_A_RT             = 0x2131,       /*A相电压合格率*/
    OI_VOLT_STAT_B_RT             = 0x2132,       /*B相电压合格率*/
    OI_VOLT_STAT_C_RT             = 0x2133,       /*C相电压合格率*/
    OI_DAY_MAX_DEMAND_TIME        = 0x2140,       /*日最大有功功率及发生时间*/
    OI_MONTH_MAX_DEMAND_TIME      = 0x2141,       /*月最大有功功率及发生时间*/
    /*变量类对象标识END*/

    /*事件类对象标识*/
    OI_LOST_VOLT_EVENT            = 0x3000,       /*电能表失压事件*/
    OI_DOWN_VOLT_EVENT            = 0x3001,       /*电能表欠压事件*/
    OI_VOER_VOLT_EVENT            = 0x3002,       /*电能表过压事件*/
    OI_BREAKE_PHASE_EVENT         = 0x3003,       /*电能表断相事件*/
    OI_LOST_CURR_EVENT            = 0x3004,       /*电能表失流事件*/
    OI_OVER_CURR_EVENT            = 0x3005,       /*电能表过流事件*/
    OI_BREAKE_CURR_EVENT          = 0x3006,       /*电能表断流事件*/
    OI_PWR_REVERSE_EVENT          = 0x3007,       /*电能表功率反向事件*/
    OI_OVER_PWR_EVENT             = 0x3008,       /*电能表过载事件*/    
    OI_OVER_PA_DEMAND_EVENT       = 0x3009,       /*电能表正向有功需量超限事件*/
    OI_OVER_NA_DEMAND_EVENT       = 0x300A,       /*电能表反向有功需量超限事件*/
    OI_OVER_R_DEMAND_EVENT        = 0x300B,       /*电能表无功需量超限事件*/
    OI_FACTOR_LOW_EVENT           = 0x300C,       /*电能表功率因数超下限事件*/
    OI_ALL_LOST_VOL_EVENT         = 0x300D,       /*全失压事件*/    
    OI_AUX_POWER_DOWN_EVENT       = 0x300E,       /*电能表辅助电源掉电事件*/
    OI_VOLT_REVERSE_EVENT         = 0x300F,       /*电能表电压逆相序事件*/
    OI_CURRENT_REVERSE_EVENT      = 0x3010,       /*电能表电流逆相序事件*/
    OI_METER_POWER_DOWN_EVENT     = 0x3011,       /*电能表掉电事件*/     
    OI_PRG_EVENT                  = 0x3012,       /*电能表编程事件*/ 
    OI_MTR_CLR_EVENT              = 0x3013,       /*电能表清零事件*/ 
    OI_DEMAND_CLR_EVENT           = 0x3014,       /*电能表需量清零事件*/ 
    OI_EVENT_CLR_EVENT            = 0x3015,       /*电能表事件清零事件*/ 
    OI_JUST_TIME_EVENT            = 0x3016,       /*电能表校时事件*/ 
    OI_DAY_TBL_PRG_EVENT          = 0x3017,       /*电能表时段表编程事件*/ 
    OI_SEASON_TBL_PRG_EVENT       = 0x3018,       /*电能表时区表编程事件*/ 
    OI_WEEKEND_PRG_EVENT          = 0x3019,       /*电能表周休日编程事件*/    
    OI_BILLING_TIME_PRG_EVENT     = 0x301A,       /*电能表结算日编程事件*/ 
    OI_MTR_COVER_OPEN_EVENT       = 0x301B,       /*电能表开盖事件*/ 
    OI_TAIL_COVER_OPEN_EVENT      = 0x301C,       /*电能表开端钮盒事件*/ 
    OI_VOLT_IMBALANCE_EVENT       = 0x301D,       /*电能表电压不平衡事件*/ 
    OI_CURR_IMBALANCE_EVENT       = 0x301E,       /*电能表电流不平衡事件*/
    OI_RELAY_TRIP_EVENT           = 0x301F,       /*电能表跳闸事件*/ 
    OI_RELAY_CLOSE_EVENT          = 0x3020,       /*电能表合闸事件*/ 
    OI_HOLIDAY_PRG_EVENT          = 0x3021,       /*电能表节假日编程事件*/ 
    OI_A_ENGY_COM_PRG_EVENT       = 0x3022,       /*电能表有功组合方式编程事件*/ 
    OI_R_ENGY_COM_PRG_EVENT       = 0x3023,       /*电能表无功组合方式编程事件*/
    OI_TARIFF_PRG_EVENT           = 0x3024,       /*电能表费率参数表编程事件*/ 
    OI_STEP_PRG_EVENT             = 0x3025,       /*电能表阶梯表编程事件*/ 
    OI_KEY_UPDATE_EVENT           = 0x3026,       /*电能表密钥更新事件*/ 
    OI_CARD_PLUG_IN_ERR_EVENT     = 0x3027,       /*电能表异常插卡事件*/ 
    OI_BUY_ENGY_EVENT             = 0x3028,       /*电能表购电记录*/ 
    OI_REFUND_EVENT               = 0x3029,       /*电能表退费记录*/ 
    OI_CONSTANT_MAGNET_EVENT      = 0x302A,       /*电能表恒定磁场干扰事件*/
    OI_RELAY_ERR_EVENT            = 0x302B,       /*电能表负荷开关误动作事件*/
    OI_PWR_FAILURE_EVENT          = 0x302C,       /*电能表电源异常事件*/ 
    OI_CURR_IMBALANCE_BADLY_EVENT = 0x302D,       /*电能表电流严重不平衡事件*/ 
    OI_RTC_ERR_EVENT              = 0x302E,       /*电能表时钟故障事件*/
    OI_MEASURE_CHIP_ERR_EVENT     = 0x302F,       /*电能表计量芯片故障事件*/
    /*事件类对象标识 END*/

    OI_EVENT_REPORT_STATUS        = 0x3300,       /*事件上报状态*/
    /*事件记录单元*/
    OI_STARDARD_EVENT_RECORD_UINT = 0x3301,       /*标准事件记录单元*/
    OI_PRG_RECORD_EVENT_UINT      = 0x3302,       /*编程记录事件单元*/
    OI_OVER_DEMAND_EVENT_UINT     = 0x3308,       /*电能表需量超限事件单元*/
    OI_POWER_ON_OFF_EVENT_UINT    = 0x3309,       /*停/上电事件记录单元*/
    OI_EVENT_CLR_EVENT_UINT       = 0x330C,       /*事件清零事件记录单元*/
    OI_CARD_PLUG_IN_ERR_EVENT_UINT= 0x3310,       /*异常插卡事件记录单元*/
    OI_REFUND_EVENT_UINT          = 0x3311,       /*退费事件记录单元*/
    OI_CHANGE_MODE_EVENT_UINT     = 0x3312,       /*通信模块变更事件单元*/    
    OI_DAY_TBL_PRG_EVENT_UINT     = 0x3314,       /*电能表时段表编程事件记录单元*/
    OI_HOLIDAY_PRG_EVENT_UINT     = 0x3315,       /*电能表节假日编程事件记录单元*/
    /*事件记录单元END*/
    OI_NEW_NEED_RPT_EVT_LIST      = 0x3320,       /*新增事件列表*/  
    
    OI_DATE_TIME                  = 0x4000,       /*日期时间*/
    OI_METER_ADDR                 = 0x4001,       /*通信地址*/
    OI_METER_ID                	  = 0x4002,       /*表号*/
    OI_USE_ID                	  = 0x4003,       /*用户号*/
    OI_POSITION                	  = 0x4004,       /*设备地址位置*/
    OI_GROUP_ADDR                 = 0x4005,       /*组地址*/
    OI_CLOCK_SOURCE               = 0x4006,       /*时钟源*/
    OI_LCD_PARAMENT               = 0x4007,       /*LCD参数*/
    OI_SLOT_SWITCH_TIME           = 0x4008,       /*两套时区表切换时间*/
    OI_DAILY_SLOT_SWITCH_DATE     = 0x4009,       /*两套日时段切换时间*/
    OI_TRIFF_SWITCH_TIME          = 0x400A,       /*两套分时费率切换时间*/
    OI_STEP_PRICE_SWITCH_TIME     = 0x400B,       /*两套阶梯电价切换时间*/
    OI_SLOT_PARAMENT              = 0x400c,       /*时区时段数*/
    OI_STEP_PRICE_NUM             = 0x400D,       /*阶梯数*/   
    OI_METER_NUMBER               = 0x4010,       /*"计量元件数"*/
    OI_PUBLIC_HOLIDAY             = 0x4011,       /*公共假日表*/
    OI_WEEKED_STATUS              = 0x4012,       /*周休日特征字*/
    OI_CURRENT_SLOT_TABLE         = 0x4014,       /*当前套时区表*/
    OI_BAK_SLOT_TABLE             = 0x4015,       /*备用套时区表*/
    OI_CURRENT_DAILY_SLOT_TABLE   = 0x4016,       /*当前套日时段表*/
    OI_BAK_DAILY_SLOT_TABLE       = 0x4017,       /*备用套日时段表*/
    OI_CURRENT_TRIFF_PRICE        = 0x4018,       /*当前套费率电价*/
    OI_BAK_TRIFF_PRICE            = 0x4019,       /*备用套费率电价*/
    OI_CURRNET_STEP_PRICE         = 0x401A,       /*当前套阶梯电价*/
    OI_BAK_STEP_PRICE             = 0x401B,       /*备用套阶梯电价*/
    OI_SET_PT                     = 0x401C,       /*电流互感器变比*/
    OI_SET_CT                     = 0x401D,       /*电压互感器变比*/
    OI_ALARM_MONEY_LIMIT          = 0x401E,       /*报警金额限制*/
    OI_OTHER_MONEY_LIMIT          = 0x401F,       /*其它金额限制*/
    OI_CARD_PLUG_IN_STATUS        = 0x4022,       /*插卡状态字*/
    
    OI_VOLT_RATE_STATUS           = 0x4030,       /*电压合格率参数*/
    
    OI_DEMAND_PERIOD              = 0x4100,       /*需量周期*/
    OI_DEMAND_SKIP_PERIOD         = 0x4101,       /*滑差时间*/
    OI_JUST_METER_PULSE_WIDTH     = 0x4102,       /*校表脉冲宽度*/
    OI_MANAGE_COD                 = 0x4103,       /*资产管理编码*/
    OI_RATED_VOLT                 = 0x4104,       /*额定电压*/
    OI_RATED_CURR                 = 0x4105,       /*额定电流、基本电流*/
    OI_MAX_CURR                   = 0x4106,       /*额定电流*/
    OI_A_PRECISE                  = 0x4107,       /*有功准确度等级*/
    OI_R_PRECISE                  = 0x4108,       /*无功准确度等级*/    
    OI_ACTIVE_PULSE_CONSTANT      = 0x4109,       /*电能表有功常数*/
    OI_REACTIVE_PULSE_CONSTANT    = 0x410A,       /*电能表无功常数*/
    OI_METER_TYPE                 = 0x410B,       /*电能表型号*/
    
    OI_SOFT_NUMBER_REGISTER       = 0x4111,       /*软件备案号*/
    OI_A_ENGY_COM_TYPE            = 0x4112,       /*有功组合方式特征字*/
    OI_R_ENGY_COM1_TYPE           = 0x4113,       /*无功组合方式1特征字*/
    OI_R_ENGY_COM2_TYPE           = 0x4114,       /*无功组合方式2特征字*/
    
    OI_AUTO_BILL_DAY_PROGRAM      = 0x4116,       /*结算日编程*/
    OI_INTERVAL_DEMAND_FREEZE     = 0x4117,       /*期间需量冻结周期*/

    OI_ELECTRI_DEVICE             = 0x4300,       /*"电气设备*/

    OI_CONNECT                    = 0x4400,       /*"应用连接"*/
    OI_CONNECT_PASS               = 0x4401,       /*"应用连接认证密码"*/
    
    OI_SIGNAL_GPRS                = 0x4500,       /*无线网信号强度*/

    /*冻结对象标识*/
    OI_MMT_FRZ                    = 0x5000,       /*瞬时冻结*/
    //OI_SEC_FRZ                    = 0x5001,       /*秒冻结*/
    OI_MIN_FRZ                    = 0x5002,       /*分钟冻结*/
    OI_HOUR_FRZ                   = 0x5003,       /*小时冻结*/
    OI_DAY_FRZ                    = 0x5004,       /*日冻结*/
    OI_BILL_FRZ                   = 0x5005,       /*结算日冻结*/
    OI_MONTH_FRZ                  = 0x5006,       /*月冻结*/
    OI_YEAR_FRZ                   = 0x5007,       /*年冻结*/
    OI_SEASON_TBL_SW_FRZ          = 0x5008,       /*时区表切换冻结*/
    OI_DAY_TBL_SW_FRZ             = 0x5009,       /*日时段表切换冻结*/
    OI_TARIFF_TBL_SW_FRZ          = 0x500A,       /*费率电价切换冻结*/
    OI_STEP_TBL_SW_FRZ            = 0x500B,       /*阶梯切换冻结*/
    
    OI_STEP_BILL_TBL_SW_FRZ       = 0x5011 ,       /*阶梯切换冻结*/
    /*冻结对象标识 END*/
    
    OI_REMOTE                     = 0x8000,       /*遥控*/
    OI_REMAIN                     = 0x8001,       /*保电*/

    OI_FILE                       = 0xf001,       /*文件*/

    OI_ESAM                       = 0xF100,       /*ESAM*/   
    OI_SAFE_MODE                  = 0xF101,       /*安全模式*/    

    /*输入输出设备类对象标识*/
    OI_RS232_DEVICE               = 0xf200,    /*RS232*/
    OI_RS485_DEVICE               = 0xf201,    /*RS485*/
    OI_IR_DEVEICE                 = 0xf202,    /*红 外*/
    OI_RELAY_DEVICE               = 0xf205,    /*继电器*/
    OI_ALARM_DEVICE               = 0xf206,    /*告警*/
    OI_MULTI_DEVICE               = 0xf207,    /*多功能端子*/
    OI_CARRIE_DEVICE              = 0xf209,    /*载波*/
    /*输入输出设备类对象标识*/

    OI_CRC_DISP                   = 0xf300,    /*自动轮显*/
    OI_KEY_DISP                   = 0xf301,    /*按键轮显*/
    
    /*自定义扩展OI*/    
    OI_METER_MODE                 = 0xFF01,    /*表型模式字*/
    OI_RELAY_TYPE                 = 0xFF03,    /*继电器类型*/
    OI_ALM_RELAY_OUT_MODE         = 0xFF04,    /*报警继电器输出模式字*/
    OI_ALM_BL_MODE                = 0xFF05,    /*背光报警模式字*/  
    OI_WAI_RELAY_TIME             = 0xFF06,    /*外置继电器脉冲方式跳闸维持时间（单位:ms）*/
    OI_WAI_RELAY_AGAIN            = 0xFF07,    /*外置继电器脉冲方式再次跳闸电量（单位:Wh）*/
    OI_SOFT_BAK_NUMBER            = 0xFF08,    /*软件备案号(自定义的备用)*/
    OI_EXTEND_MODE_1              = 0xFF10,    /*功能扩展模式字1*/
    OI_EXTEND_MODE_2              = 0xFF11,    /*功能扩展模式字2*/
    OI_EXTEND_MODE_3              = 0xFF12,    /*功能扩展模式字3*/
    OI_EXTEND_MODE_4              = 0xFF13,    /*功能扩展模式字4*/ 
    OI_EXTEND_MODE_5              = 0xFF14,    /*功能扩展模式字5*/
    OI_EXTEND_MODE_6              = 0xFF15,    /*功能扩展模式字6*/
    OI_EXTEND_MODE_7              = 0xFF16,    /*功能扩展模式字7*/
    OI_EXTEND_MODE_8              = 0xFF17,    /*功能扩展模式字8*/
    OI_EXTEND_MODE_9              = 0xFF18,    /*功能扩展模式字9*/
    OI_EXTEND_MODE_10             = 0xFF19,    /*功能扩展模式字10*/
    OI_DISP_VERSION               = 0xFF20,    /*电表显示版本号*/        
    OI_SOFTWARE_VER               = 0xFF22,    /*软件版本号(自定义)*/ 
    OI_PRODUCT_NUMBER_1           = 0xFF23,    /*生产识别唯一码1*/
    OI_PRODUCT_NUMBER_2           = 0xFF24,    /*生产识别唯一码2*/
    OI_PRODUCT_NUMBER_3           = 0xFF25,    /*生产识别唯一码2*/
    OI_CURRENT_TRIFF_NUM          = 0xFF26,    /*当前费率号*/
    OI_CURRENT_STEP_NUM           = 0xFF27,    /*当前阶梯号*/
    OI_STATUS_FACTOR              = 0xFF40,    /*退出厂内状态,抄读时表示厂内相关状态*/
    OI_TEST_RELAY                 = 0xFF41,    /*继电器测试命令*/
    OI_TEST_MULTI                 = 0xFF42,    /*多功能端子投切测试*/
    OI_TEST_CHECK                 = 0xFF43,    /*自检测试*/
    
    OI_MAX_DL_LEN                 = 0xFF60,    /*数据链路层最大长度*/    

    OI_PARAMENT_CRC               = 0xFF61,    /*参数CRC值 */
    //-------------
    OI_EXIT_FACTORY               = 0xFF62,    /*退出工厂状态*/
    OI_CAL_EVETNT_PARA            = 0xFF67,    /*计算事件默认参数*/
    
    OI_CURRENT_PHASE              = 0xFF75,    /*电流相角*/
    OI_METER_RST_EVENT            = 0xff76,    /*电表复位事件*/    
    OI_OSCILLATOR_ERR_EVENT       = 0xff77,    /*"晶振故障事件"*/
    OI_PD_PON_EVENT               = 0xff78,    /*"掉上电记录"*/
    OI_PARA_ERR_EVENT             = 0xff79,    /*"参数异常记录"*/
    OI_ENG_ERR_EVENT              = 0xff7a,    /*"电量异常记录"*/ 
    
    OI_DEMAND_MODE                = 0xff7b,    /*"需量同步方式"*/ 
    /*自定义扩展OI END*/
    
}EN_OI_698;

#define BASE_EVENT_OI    OI_LOST_VOLT_EVENT    /*事件的基础OI*/

/********************************************************/
#define ATT_CLS1_CLS6_ATT_2_DATA    2  /*1-6类属性2数据*/
#define ATT_L_CURRENT               4  /*零线电流属性*/

/*CLASS1的属性枚举 */
typedef enum
{
    EN_CLS1_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS1_ATT_2_DATA                   = 2,  /*2．总及费率电能量数组*/
    EN_CLS1_ATT_3_SCALER_UNIT            = 3,  /*3．换算及单位*/
    EN_CLS1_ATT_4_HIGH_DATA              = 4,  /*4．高精度总及费率电能量数组*/
    EN_CLS1_ATT_5_HIGH_SCALER_UNIT       = 5   /*5．高精度换算及单位*/
}EN_EVENT_CLASS1_ATT;

/*CLASS2的属性枚举 */
typedef enum
{
    EN_CLS2_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS2_ATT_2_DATA                   = 2,  /*2．总及费率最大需量数组*/
    EN_CLS2_ATT_3_SCALER_UNIT            = 3,  /*3．换算及单位*/
}EN_EVENT_CLASS2_ATT;

/*CLASS3的属性枚举 */
typedef enum
{
    EN_CLS3_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS3_ATT_2_DATA                   = 2,  /*2．分相数值组*/
    EN_CLS3_ATT_3_SCALER_UNIT            = 3,  /*3．换算及单位*/
}EN_EVENT_CLASS3_ATT;

/*CLASS4的属性枚举 */
typedef enum
{
    EN_CLS4_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS4_ATT_2_DATA                   = 2,  /*2．总及分相数值组*/
    EN_CLS4_ATT_3_SCALER_UNIT            = 3,  /*3．换算及单位*/
}EN_EVENT_CLASS4_ATT;

/*CLASS6的属性枚举 */
typedef enum
{
    EN_CLS6_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS6_ATT_2_DATA                   = 2,  /*2．数值*/
    EN_CLS6_ATT_3_SCALER_UNIT            = 3,  /*3．换算及单位*/
}EN_EVENT_CLASS6_ATT;

/*CLASS7的属性枚举 */
typedef enum
{
    EN_CLS7_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS7_ATT_2_EVENT_RECORD_TABLE     = 2,  /*2．事件记录表*/
    EN_CLS7_ATT_3_RELATION_OBJECT_TABLE  = 3,  /*3．关联对象属性表*/
    EN_CLS7_ATT_4_CURRENT_RECORD_NUM     = 4,  /*4．当前记录数*/
    EN_CLS7_ATT_5_MAX_RECORD_NUM         = 5,  /*5．最大记录数*/
    EN_CLS7_ATT_6_SET_PARAMENT           = 6,  /*6．配置参数*/
    EN_CLS7_ATT_7_CURRENT_RECORD_TABLE   = 7,  /*7．当前值记录表*/
    EN_CLS7_ATT_8_REPORT_FLAG            = 8,  /*8．上报标识*/
    EN_CLS7_ATT_9_VALID_FLAG             = 9,   /*9．有效标识*/
    EN_CLS7_ATT_10_TIME_STATUS_RECORD    = 10  /*10．时间状态记录表*/
}EN_EVENT_CLASS7_ATT;

#define MAX_CLASS7_EVENT_ATT   EN_CLS7_ATT_9_VALID_FLAG

/*CLASS7的方法枚举 */
typedef enum
{
    EN_CLS7_METHOD_1_RESET               = 1,  /*1．复位*/
    EN_CLS7_METHOD_2_DO                  = 2,  /*2．执行*/
    //EN_CLS7_METHOD_3_RECORD              = 3,  /*3．触发一次记录*/
    EN_CLS7_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4．添加一个事件关联对象属性*/
    EN_CLS7_METHOD_5_DEL_RELATION_OBJECT = 5   /*5．删除一个事件关联对象属性*/
}EN_EVENT_CLASS7_METHOD;

/*CLASS8的属性枚举 */
typedef enum
{
    EN_CLS8_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS8_ATT_2_PARAMENT               = 2   /*2．参数*/
}EN_EVENT_CLASS8_ATT;


/*CLASS9的属性枚举 */
typedef enum
{
    EN_CLS9_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS9_ATT_2_FREEZE_RECORD_TABLE    = 2,  /*2．事件记录表*/
    EN_CLS9_ATT_3_RELATION_OBJECT_TABLE  = 3,  /*3．关联对象属性表*/
}EN_EVENT_CLASS9_ATT;

/*CLASS9的方法枚举 */
typedef enum
{
    EN_CLS9_METHOD_1_RESET               = 1,  /*1．复位*/
    EN_CLS9_METHOD_2_DO                  = 2,  /*2．执行*/
    EN_CLS9_METHOD_3_RECORD              = 3,  /*3．触发一次冻结*/
    EN_CLS9_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4．添加一个事件关联对象属性*/
    EN_CLS9_METHOD_5_DEL_RELATION_OBJECT = 5,  /*5．删除一个事件关联对象属性*/
    //----
    EN_CLS9_METHOD_7_MULI_ADD_RELATION_OBJECT = 7,  /*7．批量添加冻结对象属性*/
    EN_CLS9_METHOD_8_CLR_RELATION_OBJECT = 8   /*8．清除关联对象属性表*/
}EN_EVENT_CLASS9_METHOD;

/*CLASS17的属性枚举 */
typedef enum
{
    EN_CLS17_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS17_ATT_2_DISP_LIST              = 2,  /*2．显示对象列表*/
    EN_CLS17_ATT_3_DISP_TIME              = 3,  /*3．显示时间*/
    EN_CLS17_ATT_4_DISP_PARAMENT          = 4,  /*4. 显示参数*/
}EN_EVENT_CLASS17_ATT;

/*CLASS17的方法枚举 */
typedef enum
{
    EN_CLS17_METHOD_1_RESET               = 1,  /*1．复位*/
    EN_CLS17_METHOD_2_DO                  = 2,  /*2．执行*/
    EN_CLS17_METHOD_3_UP_DISP             = 3,  /*3．下翻*/
    EN_CLS17_METHOD_4_DOWN_DISP           = 4,  /*4．上翻*/
    EN_CLS17_METHOD_5_DISP_LOOK           = 5,  /*5．显示查看*/ 
    EN_CLS17_METHOD_6_DISP_ALL            = 6,  /*6．全显*/
}EN_EVENT_CLASS17_METHOD;


/*CLASS19的属性枚举 */
typedef enum
{
    EN_CLS19_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS19_ATT_2_DEVICE_DESC            = 2,  /*2．设备描述符*/
    EN_CLS19_ATT_3_VERSION_INFO           = 3,  /*3．版本信息*/
    EN_CLS19_ATT_4_PRODUCT_TIME           = 4,  /*4. 生产日期*/
    EN_CLS19_ATT_5_SUB_DEVICE_LIST        = 5,  /*5. 子设备列表*/
    EN_CLS19_ATT_6_PROTOCOL_LIST          = 6,  /*6. 支持规约列表*/
    EN_CLS19_ATT_7_FOLLOW_REPORT          = 7,  /*7. 允许跟随上报*/
    EN_CLS19_ATT_8_ACTIVE_REPORT          = 8,  /*8. 允许主动上报*/
    EN_CLS19_ATT_9_COMM_MASTER            = 9,  /*9. 允许与主站通话*/
    EN_CLS19_ATT_10_REPORT_CHANNEL        = 10,  /*10. 上报通道*/   
}EN_EVENT_CLASS19_ATT;

/*CLASS19的方法枚举 */
typedef enum
{
    EN_CLS19_METHOD_1_RESET               = 1,  /*1．复位*/
    EN_CLS19_METHOD_2_DO                  = 2,  /*2．执行*/
    EN_CLS19_METHOD_3_METER_CLR           = 3,  /*3. 数据初始化*/
    EN_CLS19_METHOD_4_SET_DEFAULT         = 4,  /*4. 恢复出厂参数*/
    EN_CLS19_METHOD_5_EVENT_CLR           = 5,  /*5. 事件初始化*/ 
    EN_CLS19_METHOD_6_DEMAND_CLR          = 6,  /*6. 需量初始化*/
}EN_EVENT_CLASS19_METHOD;


/*CLASS20的属性枚举 */
typedef enum
{
    EN_CLS20_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS20_ATT_2_OBJECT_LIST            = 2,  /*2．对象列表*/
    EN_CLS20_ATT_3_APP_INFO               = 3,  /*3．应用语境信息*/
    EN_CLS20_ATT_4_CLIENT_ADDR            = 4,  /*4．当前连接的客户机地址*/
    EN_CLS20_ATT_5_ID_VERIFICTION         = 5,  /*5．身份验证机制*/
}EN_EVENT_CLASS20_ATT;

/*CLASS22的属性枚举 */
typedef enum
{
    EN_CLS22_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS22_ATT_2_DEVICE_LIST            = 2,  /*2. 设备对象列表*/
    EN_CLS22_ATT_3_DEVICE_NUM             = 3,  /*3. 设备对象数量*/
    EN_CLS22_ATT_4_SET_PARA               = 4,  /*4. 配置参数*/
}EN_EVENT_CLASS22_ATT;

/*CLASS24的属性枚举 */
typedef enum
{
    EN_CLS24_ATT_1_LOGIC_NUM              = 1,  /*1．逻辑名*/
    EN_CLS24_ATT_2_RELATION_OBJECT_TABLE  = 2,  /*2．关联对象属性表*/
    EN_CLS24_ATT_3_CURRENT_RECORD_NUM     = 3,  /*3．当前记录数*/
    EN_CLS24_ATT_4_MAX_RECORD_NUM         = 4,  /*4．最大记录数*/
    EN_CLS24_ATT_5_SET_PARAMENT           = 5,  /*5．配置参数*/
    EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE   = 6,  /*6．事件记录表*/
    EN_CLS24_ATT_7_EVENT_RECORD_2_TABLE   = 7,  /*7．事件记录表*/
    EN_CLS24_ATT_8_EVENT_RECORD_3_TABLE   = 8,  /*8．事件记录表*/
    EN_CLS24_ATT_9_EVENT_RECORD_4_TABLE   = 9,  /*9．事件记录表*/
    EN_CLS24_ATT_10_CURRENT_RECORD_TABLE  = 10,  /*10．当前值记录表*/
    EN_CLS24_ATT_11_REPORT_FLAG           = 11,  /*11．上报标识*/
    EN_CLS24_ATT_12_VALID_FLAG            = 12,  /*12．有效标识*/
    EN_CLS24_ATT_13_LOST_V_STATIC         = 13,  /*13.失压统计*/
    EN_CLS24_ATT_14_TIME_STATUS_RECORD    = 14  /*14．时间状态记录表*/
}EN_EVENT_CLASS24_ATT;

/*CLASS24的方法枚举 */
typedef enum
{
    EN_CLS24_METHOD_1_RESET               = 1,  /*1．复位*/
    EN_CLS24_METHOD_2_DO                  = 2,  /*2．执行*/
   // EN_CLS24_METHOD_3_RECORD              = 3,  /*3．触发一次记录*/
    EN_CLS24_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4．添加一个事件关联对象属性*/
    EN_CLS24_METHOD_5_DEL_RELATION_OBJECT = 5   /*5．删除一个事件关联对象属性*/
}EN_EVENT_CLASS24_METHOD;

typedef enum
{
    EN_METHOD_1_RESET_ESAM          = 1,  /*1．复位*/
    EN_METHOD_2_DO_ESAM             = 2,  /*2．执行*/
    EN_METHOD_3_GET_ESAM            = 3,  /*3.ESAM数据读取*/
    EN_METHOD_4_SET_ESAM            = 4,  /*4. 数据更新*/
    EN_METHOD_5_RELEASE_ESAM        = 5,  /*5. 协商失效*/
    EN_METHOD_6_PURSE_ESAM          = 6,  /*钱包操作（开户、充值、退费）*/
    EN_METHOD_7_KEY_UPDATE_ESAM     = 7,  /*7. 密钥更新*/
    EN_METHOD_8_CERT_UPDATE_ESAM    = 8,  /*8．证书更新*/
    EN_METHOD_9_AUTHTIMESET_ESAM    = 9,  /*9．设置协商时效*/
    EN_METHOD_10_INIT_PACKET_ESAM   = 10,  /*10. 钱包初始化*/
    EN_METHOD_11_FARCHEK_ESAM       = 11,  /*11. 红外认证请求*/
    EN_METHOD_12_FARAUTH_ESAM       = 12,  /*12. 红外认证指令*/
}EN_METHOD_ESAM;    

typedef enum
{
    EN_POWER_OIB2_SUM                = 0,/*合相*/   
    EN_POWER_OIB2_A                  = 1,/*A 相*/   
    EN_POWER_OIB2_B                  = 2,/*B 相*/   
    EN_POWER_OIB2_C                  = 3,/*C 相*/   
}EN_CLASS1_POWER_OIB2;

/*计量元件数单相表为1，三相三线表为2，三相四线表为3。*/
typedef enum
{
   EN_METER_NUM_SINGLE   = 1,
   EN_METER_NUM_3_3_1    = 2,
   EN_METER_NUM_3_4_1    = 3,
}EN_DATA_MEMTER_NUM;


#define  MASK_BLOCK_DATA            0x80  /*块数据子网掩码*/
/********************************************************/
#endif

