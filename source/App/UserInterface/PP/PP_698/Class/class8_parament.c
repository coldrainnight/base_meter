/*"*****************************************************************************"*/
/*"  FileName: class8_parament.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  参变量类文件   "*/
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
#include "app\userinterface\UI.h"

/*"*******************************************************************************"*/
/*可配置的，如果数据为0xffffffff，则表示没有*/

INT8U Crypt_Type = 0;     /*加密级别 注意要和入参转换*/  //0，明文，1，明文+MAC，2，密文，3，密文+MAC
/*"*******************************************************************************"*/
/*CLASS8属性2支持列表 只读*/
const ST_Parament_Object CLASS8OBISLIST_RD[]=
{
    {0x2015,0x01,E_SEQ_FLOWER_REPORT_STATUS,  TYPE_BITS_32},  /*电能表跟随上报状态字*/
	{0x201e,0x01,E_SEQ_EVENT_START_TIME,      TYPE_DATES},    /*事件发生时间*/	
	{0x2020,0x01,E_SEQ_EVENT_END_TIME,        TYPE_DATES},    /*事件结束时间*/
	{0x2021,0x01,E_SEQ_FREEZE_TIME,           TYPE_DATES},    /*数据冻结时间*/
	{0x2022,0x01,E_SEQ_EVENT_RECORD_NUM,      TYPE_U32},      /*事件记录序号*/
	{0x2023,0x01,E_SEQ_FREEZE_RECORD_NUM,     TYPE_U32},      /*冻结记录序号  */
	{0x2024,0x01,E_SEQ_EVENT_SRC_NUM,         TYPE_ENUM},     /*事件发生源  */
	{0x2025,0x02,E_SEQ_EVENT_CUR_NUM,         ST_2_U32},      /*事件当前值 事件发生次数和事件累计时间 */
	
	{0x3300,0x03,E_SEQ_EVENT_REPORT_STATUS,   TYPE_NULL},     /*事件上报状态 统一填NULL了*/	
	{0x3320,0xff,E_SEQ_ADD_EVENT_REPORT_LIST, AR_OAD},        /*新增事件列表 MAX_IID是个变化的*/ 
	
	{0x4014,0x0e,E_SEQ_CUR_YEAR_SLOT_TABLE,   AR_ST_3_YEARLY_TABLE},    /*当前套时区表   时区表数<=14 */
	{0x4016,0x08,E_SEQ_CUR_DAILY_SLOT_TABLE,  AR_ST_3_DAILY_SLOT_TABLE},/*当前套日时段表 日时段表数（q≤8） */
	{0x4018,0x3f,E_SEQ_CUR_TRIFF_PRICE_TABLE, AR_TRIFF_U32},       /*当前套费率电价 费率数（k≤63）*/
	{0x401a,0x06,E_SEQ_CUR_STEP_PRICE_TABLE,  TYPE_STEP_PRICE},    /*当前套阶梯电价 阶梯数<=6 */
	
	{0x4104,0x01,E_SEQ_REGU_VOLT,            TYPE_VIS_6}, /*额定电压  */
	{0x4105,0x01,E_SEQ_REGU_CURRENT,         TYPE_VIS_6}, /*额定电流基本电流  */
	{0x4106,0x01,E_SEQ_MAX_CURRENT,          TYPE_VIS_6}, /*最大电流  */
	{0x4107,0x01,E_SEQ_ACT_ACCU_GRADE,       TYPE_VIS_4}, /*有功准确度等级  */
	{0x4109,0x01,E_SEQ_ACT_CONST,            TYPE_U32},   /*电能表有功常数  */
	{0x410b,0x01,E_SEQ_METER_TYPE,           TYPE_VIS_32},/*电能表型号  */
	{0x4111,0x01,E_SEQ_SOFT_RECORD_ID,       TYPE_VIS_16},/*软件备案号  */
	{0x8001,0x01,E_SEQ_HOLD_POWER,           TYPE_ENUM},  /*保电 */	
#ifdef THREE_PHASE			
	{0x4108,0x01,E_SEQ_R_ACT_ACCU_GRADE,  TYPE_VIS_4},  /*无功准确度等级  */
	{0x410a,0x01,E_SEQ_R_ACT_CONST,       TYPE_U32},    /*电能表无功常数  */
    {0xFF80,0x01,E_SEQ_DEMAND_TEMP_TIME,  ST_2_U16_U8},/*需量临时数据，当前滑差数和周期数*/
    {0xFF81,0x01,E_SEQ_DEMAND_DATA ,      AR_U64},     /*需量临时数据，当前平均功率值数组*/
    {0xFF82,0x01,E_SEQ_DEMAND_TEMP_VALUE, AR_6_U32},    /*需量临时数据，临时需量*/
#endif	
    {0xF101,0x01,E_SEQ_SAFE_MODE_PARA,TYPE_ENUM},       /*安全模式参数*/
    
    /*自定义的数据*/
    {0xFF20,0x01,E_SEQ_DISP_VERSION,        TYPE_U32},     /*电表显示版本号  改成BCD */    
    {0xFF26,0x01,E_SEQ_CURRENT_TRIFF_NUM,   TYPE_U8},      /*当前费率号*/
    {0xFF27,0x01,E_SEQ_CURRENT_STEP_NUM,    TYPE_U8},      /*当前阶梯号*/
	{0xFF61,0x04,E_SEQ_PARAMENT_CRC,        AR_4_U32},     /*参数CRC值 */

	{0xFF60,0x04,E_SEQ_DL_LENGTH,         ST_3_U16},        /*链路层长度      */	
    
    {0xFF01,0x01,E_SEQ_METER_MODE,        TYPE_U8},         /*表型模式字*/
    {0xFF03,0x01,E_SEQ_RELAY_TYPE,        TYPE_U8},         /*继电器类型*/

    {0xFF04,0x01,E_SEQ_ALM_RELAY_OUT_MODE,TYPE_U32},       /*报警继电器输出模式字 */
	{0xFF05,0x01,E_SEQ_ALM_BL_MODE,       TYPE_U32},        /*背光报警模式字 */

    {0xFF06,0x01,E_SEQ_WAI_RELAY_TIME,    TYPE_U16},      /*外置继电器脉冲方式跳闸维持时间（单位:ms）*/
    {0xFF07,0x01,E_SEQ_WAI_RELAY_AGAIN,   TYPE_U16},      /*外置继电器脉冲方式再次跳闸电量（单位:Wh）*/

    {0xFF11,0x01,E_SEQ_EXTEND_MODE_2,     TYPE_U8},      /*功能扩展模式字2*/
    {0xFF12,0x01,E_SEQ_EXTEND_MODE_3,     TYPE_U8},      /*功能扩展模式字3*/
    {0xFF13,0x01,E_SEQ_EXTEND_MODE_4,     TYPE_U8},      /*功能扩展模式字4*/
    {0xFF14,0x01,E_SEQ_EXTEND_MODE_5,     TYPE_U8},      /*功能扩展模式字5*/
    {0xFF15,0x01,E_SEQ_EXTEND_MODE_6,     TYPE_U8},      /*功能扩展模式字6*/
    {0xFF23,0x01,E_SEQ_PRODUCT_NUMBER_1,  TYPE_VIS_32},  /*生产识别唯一码1*/
    {0xFF24,0x01,E_SEQ_PRODUCT_NUMBER_2,  TYPE_VIS_32},  /*生产识别唯一码2*/
    {0xFF25,0x01,E_SEQ_PRODUCT_NUMBER_3,  TYPE_VIS_32},  /*生产识别唯一码3*/
    
	{0xFF40,0x01,E_SEQ_STATUS_FACTOR,     TYPE_U16},      /*工厂状态  */
    {0xFF41,0x01,E_SEQ_TEST_RELAY,        TYPE_U8},       /*继电器测试命令*/
    {0xFF42,0x01,E_SEQ_TEST_MULTI,        TYPE_U8},       /*多功能端子投切测试*/
    {0xFF43,0x01,E_SEQ_TEST_CHECK,        TYPE_U8},       /*自检测试*/

    {0xFF62,0x01,E_SEQ_STATUS_FACTOR,     TYPE_BOOL},      /*工厂状态  */
    {0xFF67,0x01,E_SEQ_SET_DEFAULT,       TYPE_BOOL},      /*恢复默认参数 */

};

/*CLASS8属性2支持列表 可读写*/
const ST_Parament_Object CLASS8OBISLIST_WR[]=
{	
	{0x202c,0x02,E_SEQ_CUR_PAY_FILE,      ST_2_U32},      /*(当前）钱包文件 */
	{0x4000,0x01,E_SEQ_DATE_TIME,         TYPE_DATES},    /*日期时间  */
	{0x4001,0x01,E_SEQ_ADDR,              TYPE_OCTS_6},   /*通信地址  */
	{0x4002,0x01,E_SEQ_METER_ID,          TYPE_OCTS_6},   /*表号*/
	{0x4003,0x01,E_SEQ_USERID,            TYPE_OCTS_6},   /*客户编号*/
	{0x4004,0x03,E_SEQ_DEVICE_ADDR,       TYPE_LOCATION}, /*设备地理位置*/
	{0x4005,0x01,E_SEQ_GROUP_ADDR,        AR_OCTS},       /*组地址  */
	{0x4006,0x02,E_SEQ_CLK_SRC,           ST_2_EN},       /*时钟源  */
	{0x4007,0x07,E_SEQ_LCD_PARA,          TYPE_LCD_PARAMENT},/*LCD参数  */
	{0x4008,0x01,E_SEQ_YEAR_SLOT_SWITCH_TIME,TYPE_DATES}, /*两套时区表切换时间  */
	{0x4009,0x01,E_SEQ_DAILY_SLOT_SWITCH_TIME,TYPE_DATES},/*两套日时段切换时间  */
	{0x400a,0x01,E_SEQ_TRIFF_SWITCH_TIME, TYPE_DATES},    /*两套分时费率切换时间  */
	{0x400b,0x01,E_SEQ_STEP_SWITCH_TIME, TYPE_DATES},     /*两套阶梯电价切换时间  */
	{0x400c,0x05,E_SEQ_SLOT_NUM,         ST_5_U8},        /*时区时段数  */
	{0x400d,0x01,E_SEQ_STEP_NUM,         TYPE_U8},        /*阶梯数  */
	//{0x400f,0x01,E_SEQ_PASS_NUM,         TYPE_U8},        /*密钥总条数  这个现在好像只读了 */
	{0x4010,0x01,E_SEQ_METER_NUM,        TYPE_U8},        /*计量元件数  */
	{0x4011,0xFE,E_SEQ_HOLIDAY_NUM,      TYPE_HOLIDAY_TABLE},/*公共假日 公共假日数<=254 */
	{0x4012,0x01,E_SEQ_WEEK_TYPE,        TYPE_BITS_8},    /*周休日特征字  */
	{0x4013,0x01,E_SEQ_WEEK_SLOT_NUM,    TYPE_U8},        /*周休日釆用的日时段表号  */
	
	{0x4015,0x0e,E_SEQ_BAK_YEAR_SLOT_TABLE,AR_ST_3_YEARLY_TABLE},    /*备用套时区表  时区表数<=14*/
	{0x4017,0x08,E_SEQ_BAK_DAILY_SLOT_TABLE,AR_ST_3_DAILY_SLOT_TABLE},/*备用套日时段表 日时段表数（q≤8)*/
	{0x4019,0x3f,E_SEQ_BAK_TRIFF_PRICE_TABLE,AR_TRIFF_U32},                  /*备用套费率电价  费率数（k≤63）*/
	{0x401b,0x06,E_SEQ_BAK_STEP_PRICE_TABLE,TYPE_STEP_PRICE}, /*备用套阶梯电价  阶梯数<=6  */
	{0x401c,0x01,E_SEQ_CT,               TYPE_U32},        /*电流互感器变比*/
	{0x401d,0x01,E_SEQ_PT,               TYPE_U32},        /*电压互感器变比*/ 
	{0x401e,0x02,E_SEQ_ALARM_MONEY_LIMIT,ST_2_U32},        /*报警金额限值  */
	{0x401f,0x03,E_SEQ_OTHER_MONEY_LIMIT,ST_3_U32},        /*其它金额限值  */

	{0x4102,0x01,E_SEQ_PULSE_WIDTH,      TYPE_U8},         /*校表脉冲宽度  */
	{0x4103,0x01,E_SEQ_MANAGE_CODE,      TYPE_VIS_32},     /*资产管理编码  */
	{0x4112,0x01,E_SEQ_ACTIVE_TYPE,      TYPE_BITS_8},     /*有功组合方式特征字  */
	{0x4116,0x03,E_SEQ_BILL_DAY,         TYPE_BILL_DAY},   /*结算日       */
	{0x4401,0x01,E_SEQ_PASS,             TYPE_VIS_8},      /*认证密码  比较特殊，厂外只能写，不能读     */
    {0x8000,0x02,E_SEQ_REMOTE,           ST_2_U32_U16},    /*远程控制参数        */
#ifdef THREE_PHASE	
	{0x4030,0x04,E_SEQ_VOLT_RATE_PARA,     ST_4_U16},               /*电压合格率参数*/
	{0x4100,0x01,E_SEQ_MAX_DEMAND_TIME,    TYPE_U8},                /*最大需量周期*/
	{0x4101,0x01,E_SEQ_SKIP_TIME,          TYPE_U8},                /*功率滑差时间  */
	{0x4113,0x01,E_SEQ_R1_ACTION_TYPE,     TYPE_BITS_8},            /*无功组合方式1特征字  */
	{0x4114,0x01,E_SEQ_R2_ACTION_TYPE,     TYPE_BITS_8},            /*无功组合方式2特征字  */
	{0x4117,0x01,E_SEQ_INTERVAL_DEMAND_TIME,TYPE_TI},               /*期间需量冻结周期*/
	{0x4500,0x01,E_SEQ_SIGNLE,             TYPE_S16},               /*信号强度      */
#endif
    {0xFF08,0x01,E_SEQ_SOFT_BAK_NUMBER,   TYPE_VIS_16},   /*软件备案号(自定义的备用)*/
    {0xFF10,0x01,E_SEQ_EXTEND_MODE_1,     TYPE_U8},      /*功能扩展模式字1*/
    {0xFF16,0x01,E_SEQ_EXTEND_MODE_7,     TYPE_U8},      /*功能扩展模式字7*/
    {0xFF17,0x01,E_SEQ_EXTEND_MODE_8,     TYPE_U8},      /*功能扩展模式字8*/
    {0xFF18,0x01,E_SEQ_EXTEND_MODE_3,     TYPE_U8},      /*功能扩展模式字9*/
    {0xFF19,0x01,E_SEQ_EXTEND_MODE_4,     TYPE_U8},      /*功能扩展模式字10*/
    {0xFF22,0x01,E_SEQ_SOFTWARE_VER,      TYPE_VIS_32},  /*软件版本号(自定义)*/  
};  


/*CLASS8特殊属性支持列表 看整理下，是否有只读的，还是都可以读写的，然后再重新建只读列表*/
const ST_Parament_SEPECIAL_Object CLASS8_OBISLIST_SPECIAL[]=
{
    {0x2015,0x04,E_SEQ_FLOW_REPORT_MODE,   0x01,READ_WRITE,TYPE_BITS_32},      /*电能表跟随上报模式字*/
    {0x3320,0x03,E_SEQ_EVENT_NEED_REPORT_LIST,0x01,READ_ONLY,AR_N_OI},             /*需上报事件对象列表，只读*/
    {0x4000,0x03,E_SEQ_JUST_TIME_MODE,     0x01,READ_WRITE,TYPE_ENUM},         /*4000 校时模式*/
    {0x8000,0x04,E_SEQ_ALARM_STATUS,       0x01,READ_ONLY,TYPE_BITS_8},       /*告警状态，只读*/
    {0x8000,0x05,E_SEQ_CMD_STATUS,         0x01,READ_ONLY,TYPE_BITS_8},       /*命令状态，只读*/
    //{0x8001,0x03,E_SEQ_MAX_NO_COMM_TIME,   0x01,READ_WRITE,TYPE_U16},          /*允许与主站最大无通信时长（分钟），0表示不自动保电*/
    //{0x8001,0x04,E_SEQ_AUTO_HOLD_TIME,     0x01,READ_WRITE,TYPE_U16},          /*上电自动保电时长（分钟），0表示上电不自动保电*/
    {0xf101,0x03,E_SEQ_SAFE_MODE_PARA,     0x01,READ_WRITE,AR_ST_2_OI_U16},    /*显式安全模式参数  ,虽然特殊处理，但也不能删除，否则会提示未定义。*/
    {0x4500,0x09,E_SEQ_SIGNLE,             0x01,READ_WRITE,TYPE_S16},          /*信号强度      */
};

/*"************************************************"*/
/*class8 属性8 OBIS列表*/
const ST_PARAMENT_OAD_OBIS_INFO stCls8ObisInfo[] =
{
    {E_SEQ_FLOWER_REPORT_STATUS,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_EVTRPTSTATUS, E_INFO_NULL},   /*电能表跟随上报状态字*/
	{E_SEQ_EVENT_START_TIME   ,0x01,E_SYS_TIME/*只是用于显示*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*事件发生时间*/	
	{E_SEQ_EVENT_END_TIME     ,0x01,E_SYS_TIME/*只是用于显示*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*事件结束时间*/
	{E_SEQ_FREEZE_TIME        ,0x01,E_SYS_TIME/*只是用于显示*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*数据冻结时间*/
#ifdef PM_MODLE  
	{E_SEQ_EVENT_RECORD_NUM   ,0x01,E_CT_RATIO/*只是用于显示*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*事件记录序号 只是用于显示*/
	{E_SEQ_FREEZE_RECORD_NUM  ,0x01,E_CT_RATIO/*只是用于显示*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*冻结记录序号 只是用于显示 */

	{E_SEQ_EVENT_CUR_NUM      ,0x01,E_CT_RATIO/*只是用于显示*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*事件当前值  只是用于显示*/
#endif
    {E_SEQ_ADD_EVENT_REPORT_LIST,E_RELATION_OBJECT_ID,E_OBIS_NULL,0,MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},/*新增事件列表 MAX_IID是个变化的*/ 
	
#ifdef PM_MODLE  
	{E_SEQ_CUR_TRIFF_PRICE_TABLE,E_TRIFF_ID_PHASE,E_TARIFF_TABLE1, 0, FORMAT_PRICE, FUN_NULL, E_INFO_NOW_TRIFF_PRICE},        	/*当前套费率电价 费率数（k≤63）*/

	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_SETPVAULE_CURBLK, 0, FORMAT_STEPENGY, FUN_NULL, E_INFO_NOW_STEP_PRICE_MONEY}, /*当前套阶梯电量 <=6 */
	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_SETPPRICE_CURBLK, 0, FORMAT_PRICE, FUN_NULL, E_INFO_NOW_STEP_PRICE},  		  /*当前套阶梯电价 <=7 */
	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_STEP_YEARBILL_CURBLK, 0, FORMAT_YEARBILL, FUN_NULL, E_INFO_NOW_STEP},  	      /*当前套阶梯结算日<=4 */
#endif    
	{E_SEQ_REGU_VOLT,       0x01,E_REGU_VOLT, 0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*额定电压  */
	{E_SEQ_REGU_CURRENT,    0x01,E_REGU_CURR, 0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*额定电流基本电流  */
	{E_SEQ_MAX_CURRENT,     0x01,E_MAX_CURR,  0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*最大电流  */
	{E_SEQ_ACT_ACCU_GRADE,  0x01,E_ACT_ACCU_GRADE, 0, FORMAT_ASCII4, FUN_NULL, E_INFO_NULL}, /*有功准确度等级  */
	{E_SEQ_ACT_CONST,       0x01,E_A_PULSE_CONST, 0, FORMAT_CONST, FUN_NULL, E_INFO_CONST},   /*电能表有功常数  */
	{E_SEQ_METER_TYPE,      0x01,E_METER_TYPE, 0, FORMAT_ASCII32, FUN_NULL, E_INFO_NULL}, /*电能表型号  */
	{E_SEQ_SOFT_RECORD_ID,  0x01,E_SOFT_RECORD_ID, 0, FORMAT_ASCII16, FUN_NULL, E_INFO_NULL},     /*软件备案号  */
	{E_SEQ_HOLD_POWER,      0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*保电 */	

#ifdef THREE_PHASE			
	{E_SEQ_R_ACT_ACCU_GRADE,0x01,E_NE_ACCU_GRADE, 0, FORMAT_ASCII4, FUN_NULL, E_INFO_NULL},      /*无功准确度等级  */
	{E_SEQ_R_ACT_CONST,     0x01,E_R_PULSE_CONST, 0, FORMAT_CONST, FUN_NULL, E_INFO_R_CONST},    /*电能表无功常数  */
#endif	

#ifdef PM_MODLE  
    {E_SEQ_SAFE_MODE_PARA,0x01,E_SAFE_MODE,0,MAX_FORMAT_ID,FUN_NULL,E_INFO_NULL},
#endif

/*---------------上面是只读的参数--------------------*/
//----------------------------------------------------------------
/*---------------下面是可读写的参数--------------------*/
#ifdef PM_MODLE  
	{E_SEQ_CUR_PAY_FILE,0x01,E_REMAIN_MONEY, 0, FORMAT_MONEY, FUN_NULL, E_INFO_REMAIN_MONEY},      /*(当前）钱包文件  当前剩余金额*/
	{E_SEQ_CUR_PAY_FILE,0x01,E_BUY_ENGY_CNT, 0, FORMAT_BUYTIMES, FUN_NULL, E_INFO_NULL},      /*(当前）钱包文件  购电次数*/
#endif
    {E_SEQ_DATE_TIME,0x01,E_SYS_TIME, 0, FORMAT_DATETIME, FUN_SET_TIME, E_INFO_NOW_TIME},    /*日期时间  */
    //{E_SEQ_DATE_TIME,0x01,E_SYS_TIME, 0, FORMAT_TIME, FUN_NULL, 0x13},        /*日期时间  */
	{E_SEQ_ADDR,0x01,E_COMM_ADDR, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_NULL},          /*通信地址  */
	//{E_SEQ_ADDR,0x01,E_COMM_ADDR, 0, FORMAT_ADDR_L, FUN_NULL, 0x0B},          /*通信地址  */
#ifdef PM_MODLE  
	{E_SEQ_METER_ID,0x01,E_METERID, 0, FORMAT_MTRNO_H, FUN_NULL, E_INFO_METER_NUM},       /*表号*/
	{E_SEQ_METER_ID,0x01,E_METERID, 0, FORMAT_MTRNO_L, FUN_NULL, E_INFO_METER_NUM},       /*表号*/
	{E_SEQ_USERID,0x01,E_USERID, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_CUSTOM_NUM},           /*客户编号*/	
#endif
	//{E_SEQ_USERID,0x01,E_USERID, 0, FORMAT_ADDR_L, FUN_NULL, 0x17},           /*客户编号*/
	{E_SEQ_DEVICE_ADDR,0x03,E_LONGITUDE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},   /*设备地理位置*/
	{E_SEQ_GROUP_ADDR,0x01,E_GROUP_ADDR, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_NULL},   /*组地址  */
	{E_SEQ_CLK_SRC,0x02,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*时钟源  */

    #if DEBUG_PARAMENT_SINGLE_OBIS
    {E_SEQ_LCD_PARA,0x01,E_PWRUP_DIS_ALL_TIME, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},         /*LCD参数1 上电全显时间 */
	{E_SEQ_LCD_PARA,0x01,E_BACKLIGHT_SHOW_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},        /*LCD参数2 背光点亮时间 */
	{E_SEQ_LCD_PARA,0x01,E_BACKLIGHT_SHOW_TIME_ON_VIEW, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},/*LCD参数3 显示查看背光点亮时间 */
	{E_SEQ_LCD_PARA,0x01,E_PD_NOKEY_RETURN_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},       /*LCD参数4 无电按键屏幕驻留最大时间 */
	{E_SEQ_LCD_PARA,0x01,E_ENGY_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD参数5 显示电能小数位数 */
	{E_SEQ_LCD_PARA,0x01,E_PWR_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},            /*LCD参数5 显示功率（最大需量）小数位数 */
	{E_SEQ_LCD_PARA,0x01,E_DIS_SEG_MODE_CTRL_12, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},       /*LCD参数7 液晶①②字样意义  */
    #else
    {E_SEQ_LCD_PARA,0x01,E_PWRUP_DIS_ALL_TIME, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},         /*LCD参数1 上电全显时间 */
	{E_SEQ_LCD_PARA,0x03,E_BACKLIGHT_SHOW_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},      /*LCD参数2 背光点亮时间 */
	{E_SEQ_LCD_PARA,0x01,E_ENGY_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD参数5 显示功率（最大需量）小数位数 */
	{E_SEQ_LCD_PARA,0x01,E_PWR_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD参数5 显示功率（最大需量）小数位数 */
	{E_SEQ_LCD_PARA,0x01,E_DIS_SEG_MODE_CTRL_12, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},       /*LCD参数7 液晶①②字样意义  */
    #endif
    
#ifdef PM_MODLE  
	{E_SEQ_TRIFF_SWITCH_TIME,0x01,E_TRSWITCH_TIME, 0, FORMAT_SWTIME, FUN_NULL, E_INFO_TIME},    /*两套分时费率切换时间  */
	{E_SEQ_STEP_SWITCH_TIME,0x01,E_STEPSWITCH_TIME, 0, FORMAT_SWTIME, FUN_NULL, E_INFO_TIME},    /*两套阶梯电价切换时间  */
#endif
#ifdef PM_MODLE  
	{E_SEQ_STEP_NUM,0x01,E_STEP_NUM, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*阶梯数  */
#endif
	{E_SEQ_METER_NUM,0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*计量元件数  */
#ifdef PM_MODLE  
	{E_SEQ_BAK_TRIFF_PRICE_TABLE,E_TRIFF_ID_PHASE,E_TARIFF_TABLE2, 0, FORMAT_PRICE, FUN_NULL, E_INFO_TRIFF_PRICE},    /*备用套费率电价  费率数（k≤63）*/
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_SETPVAULE_PASBLK, 0, FORMAT_STEPENGY, FUN_NULL, E_INFO_STEP_PRICE_MONEY},    /*备用套阶梯电量  阶梯数<=6  */
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_SETPPRICE_PASBLK, 0, FORMAT_PRICE, FUN_NULL, E_INFO_STEP_PRICE},       /*备用套阶梯电价  <=7  */
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_STEP_YEARBILL_PASBLK, 0, FORMAT_YEARBILL, FUN_NULL, E_INFO_STEP},/*备用套阶梯结算日  <=4 */
	{E_SEQ_CT,                   0x01,E_CT_RATIO, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},    /*电流互感器变比*/
	{E_SEQ_PT,                   0x01,E_PT_RATIO, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},    /*电压互感器变比*/ 
	{E_SEQ_ALARM_MONEY_LIMIT,0x01,E_ALRM_MONEY_THR1, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*报警金额限值 报警金额门限值1*/
	{E_SEQ_ALARM_MONEY_LIMIT,0x01,E_ALRM_MONEY_THR2, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*报警金额限值 报警金额门限值2 */
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_OVERDRAW_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*其它金额限值 透支金额限值*/
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_CORNER_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*其它金额限值 囤积金额限值 */
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_RELAY_CLOSE_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*其它金额限值 合闸允许金额限值  */
#endif
	{E_SEQ_PULSE_WIDTH,0x01,E_PULSE_WIDTH, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*校表脉冲宽度  */
	{E_SEQ_MANAGE_CODE,0x01,E_ASSET_NUM, 0, FORMAT_ASCII32, FUN_NULL, E_INFO_NULL},    /*资产管理编码  */
	{E_SEQ_ACTIVE_TYPE,0x01,E_A_ENGY_COM_TYPE, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*有功组合方式特征字  */
	{E_SEQ_BILL_DAY,E_BILL_DATE_ID,E_MOUTH_BILLDAY, 0, FORMAT_BILLDATE, FUN_NULL, E_INFO_NULL},    /*结算日       */
	{E_SEQ_PASS,0x01,E_AUTHEN_PASSWORD, 0, FORMAT_ASCII8, FUN_NULL, E_INFO_NULL},    /*认证密码  比较特殊，厂外只能写，不能读     */
#ifdef PM_MODLE  
    {E_SEQ_REMOTE,0x01,E_RLY_TRIP_CTRL_CURR_THR, 0, FORMAT_P4, FUN_NULL, E_INFO_NULL},  /*继电器拉闸电流门限值        */
    {E_SEQ_REMOTE,0x01,E_TRIPCURR_TIME, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},         /*超电流门限保护延时时间        */
#endif

#ifdef THREE_PHASE
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTCHECK_UU_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*电压合格率参数*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTCHECK_UL_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*电压合格率参数*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTHEALTH_UU_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*电压合格率参数*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTHEALTH_UL_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*电压合格率参数*/

    {E_SEQ_MAX_DEMAND_TIME,      0x01,E_DEMAND_PERIOD, 0, FORMAT_WORD, FUN_SET_DU_PARA, E_INFO_NULL},                /*最大需量周期*/
	{E_SEQ_SKIP_TIME,            0x01,E_DEMAND_SLID_PERIOD, 0, FORMAT_WORD, FUN_SET_DU_PARA, E_INFO_NULL},           /*需量滑差时间  */
	{E_SEQ_R1_ACTION_TYPE,       0x01,E_R_ENGY_COM_TYPE1, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},            /*无功组合方式1特征字  */
	{E_SEQ_R2_ACTION_TYPE,       0x01,E_R_ENGY_COM_TYPE2, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},            /*无功组合方式2特征字  */
	{E_SEQ_INTERVAL_DEMAND_TIME, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*期间需量冻结周期*/
	{E_SEQ_SIGNLE,               0x01,E_SIGNAL_STRENGTH, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},               /*信号强度      */
    //--
    {E_SEQ_DEMAND_TEMP_TIME,     0x02,E_DEMAND_TEMP_SKIPTIMES_COUNT, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*需量临时数据，当前滑差数和周期数*/
    {E_SEQ_DEMAND_DATA,          0x01,E_DEMAND_AV_POWER_BLK, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*需量临时数据，当前平均功率值数组*/
    {E_SEQ_DEMAND_TEMP_VALUE,    0x06,E_CURRENT_PA_DEMAND_TEMP, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},         /*需量临时数据 */
#endif    

    /*特殊属性(AID)OAD OBIS列表*/ 
    {E_SEQ_FLOW_REPORT_MODE, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*电能表跟随上报模式字*/
    {E_SEQ_EVENT_NEED_REPORT_LIST,  0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*需上报事件列表*/ 
    {E_SEQ_JUST_TIME_MODE,   0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*4000 校时模式*/
    {E_SEQ_ALARM_STATUS,     0x01,E_OBIS_NULL/*读前处理*/, 0, MAX_FORMAT_ID, FUN_RLYSTATUS, E_INFO_NULL},    /*告警状态，只读*/
    {E_SEQ_CMD_STATUS,       0x01,E_OBIS_NULL/*读前处理*/, 0, MAX_FORMAT_ID, FUN_RLYSTATUS, E_INFO_NULL},    /*命令状态，只读*/
    //{E_SEQ_MAX_NO_COMM_TIME, 0x01,E_FREQ/*暂不支持*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*允许与主站最大无通信时长（分钟），0表示不自动保电*/
    //{E_SEQ_AUTO_HOLD_TIME,   0x01,E_FREQ/*暂不支持*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*上电自动保电时长（分钟），0表示上电不自动保电*/
    {E_SEQ_PRIO_SAFE_MODE_PARA,   0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*显式安全模式参数*/

    /*自定义协议OBIS列表*/
	{E_SEQ_DL_LENGTH,   0x04,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*链路层长度      */

    //{E_SEQ_METER_MODE,  0x01,E_METER_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*表型模式字*/
    //{E_SEQ_RELAY_TYPE,  0x01,E_RELAY_TYPE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*继电器类型*/

    {E_SEQ_ALM_RELAY_OUT_MODE, 0x01,E_EVT_LCDALARM_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},      /*报警继电器输出模式字*/
    {E_SEQ_ALM_BL_MODE,  0x01,E_EVT_BGTALARM_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*背光报警模式字*/  

    //{E_SEQ_WAI_RELAY_TIME, 0x01,E_RELAY_ACT_TIME, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*外置继电器脉冲方式跳闸维持时间（单位:ms）*/ 
    //{E_SEQ_WAI_RELAY_AGAIN,0x01,E_RELAY_ACT_ENGY, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*外置继电器脉冲方式再次跳闸电量（单位:Wh）*/ 
    {E_SEQ_SOFT_BAK_NUMBER,0x01,E_SOFT_RECORD_ID, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*软件备案号(自定义的备用)*/ 
    {E_SEQ_EXTEND_MODE_1,0x01,E_EXT_MODE1, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字1*/ 
    {E_SEQ_EXTEND_MODE_2,0x01,E_EXT_MODE2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字2*/ 
    {E_SEQ_EXTEND_MODE_3,0x01,E_EXT_MODE3, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字3*/ 
    {E_SEQ_EXTEND_MODE_4,0x01,E_EXT_MODE4, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字4*/ 
    {E_SEQ_EXTEND_MODE_5,0x01,E_EXT_MODE5, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字5*/ 
    {E_SEQ_EXTEND_MODE_6,0x01,E_EXT_MODE6, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字7*/ 
    {E_SEQ_EXTEND_MODE_7,0x01,E_EXT_MODE7, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字7*/ 
    {E_SEQ_EXTEND_MODE_8,0x01,E_EXT_MODE8, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字8*/ 
    {E_SEQ_EXTEND_MODE_9,0x01,E_EXT_MODE9, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*功能扩展模式字9*/ 
    {E_SEQ_EXTEND_MODE_10,0x01,E_EXT_MODE10, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},           /*功能扩展模式字10*/ 
    {E_SEQ_DISP_VERSION,0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},/*电表显示版本号*/          
    {E_SEQ_SOFTWARE_VER,0x01,E_SOFT_VER_32, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},           /*自定义软件版本号*/          
    {E_SEQ_PRODUCT_NUMBER_1,0x01,E_PRODUCT_ID1, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*生产识别唯一码1*/ 
    {E_SEQ_PRODUCT_NUMBER_2,0x01,E_PRODUCT_ID2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*生产识别唯一码2*/ 
    {E_SEQ_PRODUCT_NUMBER_3,0x01,E_PRODUCT_ID3, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*生产识别唯一码3*/ 
    //{E_SEQ_CURRENT_STEP_NUM,0x01,E_CUR_STEPNUM, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*当前阶梯号*/ 
    {E_SEQ_STATUS_FACTOR,0x01,E_FAC_STATUS, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*电能表厂内状态*/ 
    {E_SEQ_TEST_RELAY,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*继电器测试命令*/
    {E_SEQ_TEST_MULTI,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*多功能端子投切测试*/
    {E_SEQ_TEST_CHECK,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*自检测试*/
    {E_SEQ_PARAMENT_CRC,0x04,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*参数CRC值 */
    
    {E_SEQ_SET_DEFAULT, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*恢复默认参数 */
    /*注:下面这个最好不要去掉，以便序列号与上一个不同，在程序处理时不异常*/
    {E_MAX_SEQ_PARAMENT,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*最后以一个不存在的序列号，防止程序出现异常情况 */
};
/*"************************************************"*/

const ST_Parament_Object* const CLASS8OBISLIST[] =
{
    CLASS8OBISLIST_RD,
    CLASS8OBISLIST_WR,
};

const INT8U CLASS8_OI_NUM[]=
{
   ARRAY_SIZE(CLASS8OBISLIST_RD),
   ARRAY_SIZE(CLASS8OBISLIST_WR),
};    



/*****************************************************************************
 函 数 名  : get_class_parament_Obis
 功能描述  : 参数类OBIS获取函数
 输入参数  : INT8U seq_num                                   
             ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak        
             const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser  
             INT8U list_num    :取一类的OBIS最大值，主要是为了简化功能                             
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 17
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U get_class_parament_Obis(INT8U seq_num, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak,
                                    const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser,INT8U list_num)
{
    INT8U i,j,num_obis = 0;
    ST_PARAMENT_OAD_OBIS_INFO *pObisInfo = pObisInfo_bak;
    INT16U temp_obis = 0xffff;
        
    for(i=0;i<list_num;i++)
    {
        if(seq_num == pObisInfo_ser[i].Seq_Num)
        {
           /*list_num-i，即还剩下这么多个*/
           for(j=0;j<list_num-i;j++)
           {       
               /*先做下容错，后面再优化参数OBIS列表，可以去掉*/
               if(temp_obis!=pObisInfo_ser[i+j].Obis)
               { 
                   LIB_MemCpy((INT8U*)&pObisInfo_ser[i+j], (INT8U*)&pObisInfo[num_obis], sizeof(ST_PARAMENT_OAD_OBIS_INFO));
                   temp_obis = pObisInfo_ser[i+j].Obis;
                   num_obis++;
               }   

               /*2018-01-19:容下错，防止越界*/
               if(num_obis>=MAX_OBIS_NUM_ONE_OAD)
               {
                   return 0; 
               } 

               /*继续比较下一个，看是否有多个*/
               if(seq_num != pObisInfo_ser[i+j+1].Seq_Num)  
               {
                   return num_obis;
                   //break;
               } 
           }

           /*实际可以不用，这里只是为了可靠性 因为这里，可能是最后一个的，所以要用这个*/
           return num_obis;  
        }   
    }    

    
    return 0;

}


/*****************************************************************************
 函 数 名  : get_class8_Obis
 功能描述  : 获取8类的OBIS
 输入参数  : SUB_OBJECT *pObject  
             INT16U *pObis        
             ST_OBIS_ID *pObisId  
 输出参数  : 无
 返 回 值  : j:OBISINFO列表个数
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 11
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U get_class8_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U num;
    
    num = get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                                stCls8ObisInfo,ARRAY_SIZE(stCls8ObisInfo));

    
    return num;
}    


/*8类事件元素查找*/
INT8U get_class8_object(SUB_OBJECT *pObject)
{
    INT8U  i,j,AID,IID;//,OI_NUM;
    INT16U OI;
    const ST_Parament_Object *pCLASS8_OBISLIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*1.先查找特殊的OAD*/
    if(2<AID) /*特殊OAD，只有AID不是属性2的*/
    {    
        for(i=0;i<ARRAY_SIZE(CLASS8_OBISLIST_SPECIAL);i++)
        {
            if((OI==CLASS8_OBISLIST_SPECIAL[i].OI)&&(AID==CLASS8_OBISLIST_SPECIAL[i].AID))
            {
                pObject->Class   = 8;
                if(IID>CLASS8_OBISLIST_SPECIAL[i].MAX_IID)
                {
                    return DAR_8_OVERFLOW;
                }    
                pObject->Right_RW= CLASS8_OBISLIST_SPECIAL[i].Right_RW;
                pObject->seq_num = CLASS8_OBISLIST_SPECIAL[i].Seq_Num;
                pObject->Pointer = CLASS8_OBISLIST_SPECIAL[i].Pointer;
                return DAR_0_OK;
            }    
        } 

        return DAR_4_OANODEF;
    }
    
    /*-----下面是是AID=2或1的OI查找----*/
    
    /*2.查找可读写的OI*/ /*3.查找只读的OI*/ /*放在一起*/
    for(j=0;j<ARRAY_SIZE(CLASS8OBISLIST);j++)
    {    
        pCLASS8_OBISLIST = CLASS8OBISLIST[j];
        
        for(i=0;i<CLASS8_OI_NUM[j];i++)
        {
            if(OI==pCLASS8_OBISLIST[i].OI)
            {
                pObject->Class   = 8;
                if(IID>pCLASS8_OBISLIST[i].MAX_IID)
                {
                    /*提示时区表数，时段表数，费率数超*/
                    if((OI_CURRENT_SLOT_TABLE == OI)||(OI_BAK_SLOT_TABLE == OI))
                    {
                        return DAR_17_YEARZONE;
                    } 
                    else if((OI_CURRENT_DAILY_SLOT_TABLE == OI)||(OI_BAK_DAILY_SLOT_TABLE == OI))
                    {
                        return DAR_18_DAILYSLOT;                        
                    }
                    else if((OI_CURRENT_TRIFF_PRICE == OI)||(OI_BAK_TRIFF_PRICE == OI))
                    {
                        return DAR_19_TRIFF;         
                    }
                    else
                    {    
                        return DAR_8_OVERFLOW;
                    }
                }  
                
                if(j==0)
                {
                    pObject->Right_RW= READ_ONLY;
                }
                else
                {     
                    pObject->Right_RW= READ_WRITE;
                }

                if(OI_CONNECT_PASS == OI)
                {
                    pObject->Right_RW= WRITE_ONLY;
                }  
                else if((OI_EVENT_START_TIME <= OI)&&(OI_EVENT_CURRENT_VALUE>=OI))
                {
                    pObject->Right_RW= NO_READ_WRITE;
                } 
                
                
                pObject->seq_num = pCLASS8_OBISLIST[i].Seq_Num;
                pObject->Pointer = pCLASS8_OBISLIST[i].Pointer;
                
                return DAR_0_OK;
            }    
        } 
    }

    return DAR_4_OANODEF;
}

/***************************下面是安全模式处理******************************/
/*"***************************************************************************************"*/ 
/*-------------------------------------------------------------------*/
/*默认安全模式参数,具体OI*/
const ST_F101_SAFE_MODE Default_OI_Safe_Mode[]=
{
   {0x202C,0x80,0x04},   /*（当前）钱包文件*/
   {0x202F,0x41,0x80},   /*电能表主动上报*/
   {0x4000,0x81,0x84},   /*日期时间*/
   {0x4001,0x81,0x04},   /*通信地址*/
   {0x4002,0x84,0x04},   /*表号*/
   {0x4003,0x44,0x00},   /*客户编号*/
   {0x400A,0x41,0x00},   /*两套分时费率切换时间*/
   {0x400B,0x41,0x00},   /*两套阶梯切换时间*/
   {0x401C,0x41,0x00},   /*电流互感器变比*/
   {0x401D,0x41,0x00},   /*电压互感器变比*/
   {0x401E,0x41,0x00},   /*金额限值*/
   {0x4018,0x40,0x00},   /*当前套费率电价*/
   {0x4019,0x41,0x00},   /*备用套费率电价*/
   {0x401A,0x40,0x00},   /*当前套阶梯参数*/
   {0x401B,0x41,0x00},   /*备用套阶梯参数*/
   {0x4111,0x81,0x04},   /*备案号*/
   {0x5000,0x41,0x40},   /*瞬时冻结*/
   {0xF100,0x84,0x14},   /*ESAM*/
   {0xF101,0x81,0x24},   /*安全模式参数*/
};


const ST_F101_SAFE_MODE Default_TYPE_Safe_Mode[]=
{
   {0x0fff,0x80,0x04},   /*当前电能0x0ZZZ 1类*/
   {0x1fff,0x40,0x00},   /*最大需量0x1ZZZ 2类*/
   {0x2fff,0x40,0x40},   /*变量0x2ZZZ */
   {0x3fff,0x41,0x10},   /*事件0x3ZZZ */
   {0x4FFF,0x41,0x10},   /*参变量4ZZZ*/
   {0x50FF,0x41,0x10},   /*冻结50ZZ*/
   {0x80FF,0x41,0x10},   /*控制80ZZ*/
   {0xF2FF,0x88,0x8f},   /*输入输出接口设备F2ZZ*/
};

/*****************************************************************************
 函 数 名  : add_oi_right
 功能描述  : 添加OI的安全模式
 输入参数  : ST_F101_SAFE_MODE *p_oi_right
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期一 2017年4月17日
    作    者   : xyt
    修改内容   : 新生成函数
return:0，成功，-1:失败
*****************************************************************************/
INT8S add_oi_right(ST_F101_SAFE_MODE *p_oi_right)
{
    INT8U  i;
    INT16U OI,OBIS;   
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);

    OI=p_oi_right->Oi.asInt;
        
    // 1.先查找是否已有这个OI,如果有，则直接代替
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(Array_Safe_Mode[i].Oi.asInt== OI)
        {
           /*这里本来可能要反一下,但为了使用，以及读时不用反，所以这里不反了，用的时候反一下*/
           Array_Safe_Mode[i].Rigth2 = p_oi_right->Rigth2;
           Array_Safe_Mode[i].Rigth3 = p_oi_right->Rigth3;
           //len = sizeof(ST_F101_SAFE_MODE);
           OBIS = E_SAFE_MODE_LIST+i+1;
           
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
            
           return 0;
        }
    }

    // 2.如果没有，则找第一个为OXFFFF的
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(0xffff == Array_Safe_Mode[i].Oi.asInt)
        {
           Array_Safe_Mode[i].Oi.asInt= OI;	          
           Array_Safe_Mode[i].Rigth2 = p_oi_right->Rigth2;
           Array_Safe_Mode[i].Rigth3 = p_oi_right->Rigth3;
           
           OBIS = E_SAFE_MODE_LIST+i+1;
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
           
           return 0;
        }
    }

    // 3.如果添加不进，则报失败
    return (-1);
}
/*****************************************************************************
 函 数 名  : del_oi_right
 功能描述  : 删除OI的安全模式
 输入参数  : ST_F101_SAFE_MODE *p_oi_right
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期一 2017年4月17日
    作    者   : xyt
    修改内容   : 新生成函数
return:0，成功，-1:失败
*****************************************************************************/
INT8S del_oi_right(ST_F101_SAFE_MODE *p_oi_right)
{
    INT8U  i;
    INT16U OI,OBIS; 
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);

    OI=p_oi_right->Oi.asInt; 

    // 1.先查找是否已有这个OI,如果有，则删除
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(Array_Safe_Mode[i].Oi.asInt== OI)
        {
           Array_Safe_Mode[i].Oi.asInt= 0xffff; 
	       Array_Safe_Mode[i].Rigth2 = 0xff;
           Array_Safe_Mode[i].Rigth3 = 0xff;	
              
           OBIS = E_SAFE_MODE_LIST+i+1;
           
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
           
           return 0;
        }
    }

    return (-1);
}
/*****************************************************************************
 函 数 名  : check_right_oi
 功能描述  : 根据能力模式，返回是否有此权限
 输入参数  : INT8U ServiceType 服务类型
             INT8U Rigth2  读取设置权限 参考 698协议 附录表F.2　安全模式定义
             INT8U Rigth3  操作代码权限
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期一 2017年4月17日
    作    者   : xyt
    修改内容   : 新生成函数

return:0,可以执行，非0，不能执行
*****************************************************************************/
INT8S check_right_oi(INT8U ServiceType,INT8U Rigth2,INT8U Rigth3)
{
    INT8U temp_crypt_type;
    
    INT8U right_value;    

    if(EN_GETREQUEST ==ServiceType)        //读取
    {
        right_value = Rigth2>>4;
    }
    else if(EN_SETREQUEST ==ServiceType)   //设置
    {
        right_value = Rigth2&0x0f;
    }
    else if(EN_ACTIONREQUEST ==ServiceType)//操作
    {
        right_value = Rigth3>>4;
    }
    else
    {

    }

    if(right_value&0x08)
    {
	  temp_crypt_type = 0;
    }
    else if(right_value&0x04)
    {
	  temp_crypt_type = 1;
    }
    else if(right_value&0x02)
    {
	  temp_crypt_type = 2;
    }	
    else if(right_value&0x01)
    {
	  temp_crypt_type = 3;
    }		
	else
	{
		temp_crypt_type = 4;  //0不允许读写
	}
	

    if(Crypt_Type>=temp_crypt_type)
    {
        return 0;
    }
    else
    {
        return (-1);
    }
}
/**********************************************************************************/
const INT16U NO_DIRECT_SET_OI[]={0x4002,0x4003,0x400a,0x400b,0x4019,0x401b,0x401c,0x401d,0x401e};

/*红外可以直接读的数据*/           /*"钱包,  日期时间, 月度用电量,通信地址, 表号, 备案号"*/
const INT16U DIRECT_READ_OI_INFRARED[]={0x202C, 0x2031, 0x2032, 0x4000,0x4001, 0x4002, 0x4111};
/*****************************************************************************
 函 数 名  : get_oi_right_type
 功能描述  : 获取OI的能力权限
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期一 2017年4月17日
    作    者   : xyt
    修改内容   : 新生成函数
return:0,可以执行，非0，不能执行
*****************************************************************************/
INT8S get_oi_right_type(INT8U ServiceType, SUB_OBJECT *pObject)
{
    INT8U i;
    INT8U tmp;//,relayMode;

    INT8U OI_0;
    INT16U OI;

    INT8U Rigth2;
    INT8U Rigth3;
    INT16U OBIS;
 
    ST_F101_SAFE_MODE TmpSafe_Mode;
	    
    OI = pObject->Oad.Oi.asInt;    

    OI_0 = pObject->Oad.Oi.asByte[OIH]; //(OI&0xff00)>>8; //

#ifdef PM_MODLE  
	GetSingle(E_RELAY_TYPE,(INT8U *)&relayMode);        /*"允许明文拉合闸操作"*/
	if(relayMode & ENMWRELAY)                          /*"明文拉合闸模式字打开"*/
	{
		if((OI_REMOTE==OI)||(OI_REMAIN==OI))
		{
			return 0;	
		}
	}

    
    /*      红外安全认证没通过，不允许操作设置与方法      */
    if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
    {    

    }
#endif    
	    
    /*判断是否是一类参数，一类参数不能直接设置，必须走ACTION*/
    if(EN_SETREQUEST == ServiceType)
    {    
        /*2017-08-02:厂外不准设安全模式*/		
        /*
		if(((OI_SAFE_MODE==OI)&&(2==AID))&&(FALSE == ChkStatus(E_FAC)))
		{
			 return (-1); 
		}	*/	
	
        for(i=0;i<sizeof(NO_DIRECT_SET_OI)/sizeof(INT16U);i++)
        {
            if(OI == NO_DIRECT_SET_OI[i])
            {
                return (-1);
            }    
        }    
    }
	else if(EN_GETREQUEST == ServiceType)
	{
		
	}

	/*加密级别为3，就没什么好判的了，直接返回成功*/
	if(Crypt_Type>=3)
	{
		return 0;
	}

    // 1.先查找显示安全模式
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        OBIS = E_SAFE_MODE_LIST+i+1;
        GetSingle(OBIS, (INT8U *)&TmpSafe_Mode);
        
        /*LIB_MemCpy((INT8U *)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara4Hex.SafeMode[i], 
                    (INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));*/

        if(TmpSafe_Mode.Oi.asInt == OI)
        {
           /*这里要反一下，以便和传下来的数据对应*/
           Rigth2 = TmpSafe_Mode.Rigth3;
           Rigth3 = TmpSafe_Mode.Rigth2;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    // 2.查找默认OI安全模式
     for(i=0;i<sizeof(Default_OI_Safe_Mode)/sizeof(ST_F101_SAFE_MODE);i++)
    {
        if(Default_OI_Safe_Mode[i].Oi.asInt == OI)
        {
           /*注:这里不用反*/
           Rigth2 = Default_OI_Safe_Mode[i].Rigth2;
           Rigth3 = Default_OI_Safe_Mode[i].Rigth3;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    // 3.查找默认类安全模式
    //转换成类OI，前几个取高半字节，后面几个取高字节
    tmp=((OI_0&0xf0)>>4);
    if(tmp<=4)
    {
        OI_0 = OI_0|0x0f;
    }

    for(i=0;i<sizeof(Default_TYPE_Safe_Mode)/sizeof(ST_F101_SAFE_MODE);i++)
    {			
        if(Default_TYPE_Safe_Mode[i].Oi.asByte[OIH] == OI_0)
        {
           /*注:这里不用反*/
           Rigth2 = Default_TYPE_Safe_Mode[i].Rigth2;
           Rigth3 = Default_TYPE_Safe_Mode[i].Rigth3;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    return 0;

}

/*****************************************************************************
 函 数 名  : get_num_oi_safe_mode
 功能描述  : 获取显示安全模式内容
 输入参数  : INT8U *pdata  数据缓存地址
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 05 23
    作    者   : x
    修改内容   : 新生成函数
    
暂不支持IID!=0的情形，因为不好判断IID对应哪个(如果中间删了一个的话)
*****************************************************************************/
INT8U get_num_oi_safe_mode(INT8U *pdata)
{
    INT8U i;
    INT8U len = 0;
    
    ST_F101_SAFE_MODE TmpSafe_Mode;
    
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);
    
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        //GetSingle(E_SAFE_MODE_LIST+i, (INT8U *) &TmpSafe_Mode);
        //LIB_MemCpy((INT8U *)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara4Hex.SafeMode[i], 
        //           (INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));
        LIB_MemCpy((INT8U *) &Array_Safe_Mode[i],(INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));
        
        if(TmpSafe_Mode.Oi.asInt != 0xffff)
        {
           LIB_MemCpy((INT8U*)&TmpSafe_Mode,pdata,sizeof(ST_F101_SAFE_MODE));
           len += sizeof(ST_F101_SAFE_MODE);
           pdata += sizeof(ST_F101_SAFE_MODE);
        }
    }    

    return len;
}


/*****************************************************************************
 函 数 名  : Reset_safe_mode
 功能描述  : 复位安全模式参数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 11 07
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
static void Reset_safe_mode(void)
{
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];
   
    LIB_MemSet(0xff, (INT8U*)Array_Safe_Mode,  MAX_OI_NUM_SAFE_MODE*sizeof(ST_F101_SAFE_MODE));  
 
    SetSingle(E_SAFE_MODE_LIST, (INT8U*)Array_Safe_Mode, MAX_OI_NUM_SAFE_MODE*sizeof(ST_F101_SAFE_MODE));    
}    


//return:0,ok,not 0:not ok
INT8U Action_safe_mode(INT8U AId,INT8U *Ioaddr,INT8U Iooffset)
{
    INT8U i;
	INT8U ret= DAR_0_OK;      

    if(1==AId)
    {
        /*
        方法1：复位（参数）
        参数∷=integer（0）
        复位时，清空属性3。
        */
        Reset_safe_mode();
    }
    else if(128==AId)
    {
        /*
        方法128：删除显式安全模式参数（对象标识）
        对象标识∷=OI
        */         

        if(0!=del_oi_right((ST_F101_SAFE_MODE *)Ioaddr))
        {
            ret = DAR_0_OK;
        }
    }
    else if((129==AId)||(127==AId)||(3==AId))
    {
        /*
        方法129：批量增加显式安全模式参数（array 安全模式参数）
        安全模式参数∷=structure
        {
        对象标识    OI，
        安全模式    long-unsigned
        }
        */           

        /*如果属性设置，则先清掉所有安全模式*/
        if(3 == AId)
        {
            Reset_safe_mode();
        }    
        /*如果属性设置，则先清掉所有安全模式 END*/
        
        for(i=0;i<DIV_4(Iooffset);i++)
        {               
            if(0!=add_oi_right((ST_F101_SAFE_MODE *)(Ioaddr+MULTI_4(i))))
            {
               ret = DAR_FF_OTHERERR;
               break;
            }
        }
    }
    else
    {
        ret = DAR_5_OANOADAPT;
    }  
    
	return (ret);
   
}


/*****************************************************************************
 函 数 名  : Get_safe_mode
 功能描述  : 获取显示安全模式数据，不支持子IID，因为添加删除顺序是随机的，所
             以IID对应的数据不是一一对应的
 输入参数  : ST_OAD Oad    
             INT8U *pdata  
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2018 01 19
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Get_safe_mode(ST_OAD Oad, INT8U *pdata)
{
    //INT8U ServiceType;
    //INT8U AId;
    INT8U i,len;
    INT8U array_num;
    ST_F101_SAFE_MODE tmp_L[MAX_OI_NUM_SAFE_MODE];
    INT8U *pdst = pdata;

    /*
    属性3（显式安全模式参数）∷=array 安全模式参数
    安全模式参数∷=structure
    {
      对象标识   OI，
      安全模式   long-unsigned
    }
    */
           
    pdst   = EnCodeOAD(&Oad,pdst);//OAD
    *pdst++=0x01;	 /*表示返回的数据是data，不是DAR*/
    len = get_num_oi_safe_mode((INT8U*)&tmp_L);
    
    *pdst++ =array;
    if(0== len)
    {
        *pdst++=0;
    }
    else
    {
        /*数组*/		        
        array_num = DIV_4(len);
        
        *pdst++ = array_num;

        for(i=0;i<array_num;i++)
        {
            //EncodeComplexData((INT8U *)C8_ATT3_F101,p);
            pdst += EnCode_Data((ST_ENCODE_INFO *)ST_2_OI_U16,(INT8U *)&tmp_L[i],pdst,0); 
        }
    }
    
    return pdst;
    
} 

#if DEBUG_PP_698_CODE
/*下面两个函数为读前函数与写前函数举例*/
INT8U GetLcdPara(INT8U *pdata)
{
    if(CurOad.IId == 1)
    {    
        *pdata=60;
    } 

    return DAR_0_OK;
    
} 

INT8U setLcdPara(INT8U *pdata)
{
    ST_OAD Oad;
    
    GetSingle(E_PP_OAD,(INT8U *)&Oad);  
    
    if(Oad.IId == 1)
    {
        if(*pdata>=60)
        {
            return DAR_8_OVERFLOW;
        }    
    } 

    return DAR_0_OK;
    
} 
#endif

/*"************************************************"*/


