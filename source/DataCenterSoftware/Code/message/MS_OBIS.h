/*"*************************************************************"*/
/*"FileName: MS_OBIS.h"*/
/*"Author:    		Version :  "*/
/*"2019-08-21 11:03:30"*/
/*"Description: 电能表软件平台数据对象标识定义"*/
/*"总字节数:          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/


#include "PlatForm.h"
#ifndef MS_OBIS_H
#define MS_OBIS_H
#define  RAM      0x80000000


typedef struct
{
    INT16U obis; /*"对象标识"*/
    DATA_ADDR addr; /*"数据存储起始地址"*/
    INT16U len; /*"数据长度"*/
    INT8U ObjNum; /*"数据块成员数"*/
    INT8U format_id; /*"数据格式属性"*/
}ST_BLK_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"对象标识"*/
    DATA_ADDR addr; /*"数据存储起始地址"*/
    INT16U len; /*"数据长度"*/
    INT8U format_id; /*"数据格式属性"*/
}ST_SINGLE_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"数据标识"*/
    INT8U format_id; /*"记录存储数据格式"*/
}ST_FIXED_LIST_ATTR;

typedef struct

{
    INT16U obis; /*"关联对象数据标识"*/
    const ST_FIXED_LIST_ATTR *pdatalis1; /*"固定记录内容列表"*/
    const ST_FIXED_LIST_ATTR *pdatalis2; /*"关联对象列表数据对象"*/
    INT8U data_num1; /*"固定记录内容列表数据对象个数"*/
    INT8U data_num2; /*"关联对象列表数据对象个数"*/
}ST_FELXOBJ_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"记录对象标识"*/
    INT16U file; /*"文件系统中对应的虚拟文件名"*/
    INT16U timesAddr; /*"记录次数内卡存储地址"*/
    INT16U fileSize;   /*"记录文件大小"*/
    INT16U startFelxObj; /*"起始记录关联对象标识"*/
    INT16U endFelxObj; /*"结束记录关联对象标识"*/
}ST_FILE_ATTR;

typedef INT8S (* ACTION)(void *pdin, void *pdout);
typedef struct
{
    INT16U obis; /*"对象标识"*/
    ACTION Fuction; /*"执行方法"*/    
}ST_ACTION_ATTR;



/*"法定计量模块数据标准对象名"*/ 
 typedef enum 
 {
    E_PA_ENGY_BLK = 0x0000,/*"正向有功电量数据块"*/
      E_PA_ENGY,/*"正向有功总电量"*/
      E_PA_ENGY_T1,/*"正向有功费率1电量"*/
      E_PA_ENGY_T2,/*"正向有功费率2电量"*/
      E_PA_ENGY_T3,/*"正向有功费率3电量"*/
      E_PA_ENGY_T4,/*"正向有功费率4电量"*/
      E_PA_ENGY_T5,/*"正向有功费率5电量"*/
      E_PA_ENGY_T6,/*"正向有功费率6电量"*/
      E_PA_ENGY_T7,/*"正向有功费率7电量"*/
      E_PA_ENGY_T8,/*"正向有功费率8电量"*/
    E_NA_ENGY_BLK = 0x0010,/*"反向有功电量数据块"*/
      E_NA_ENGY,/*"反向有功总电量"*/
      E_NA_ENGY_T1,/*"反向有功费率1电量"*/
      E_NA_ENGY_T2,/*"反向有功费率2电量"*/
      E_NA_ENGY_T3,/*"反向有功费率3电量"*/
      E_NA_ENGY_T4,/*"反向有功费率4电量"*/
      E_NA_ENGY_T5,/*"反向有功费率5电量"*/
      E_NA_ENGY_T6,/*"反向有功费率6电量"*/
      E_NA_ENGY_T7,/*"反向有功费率7电量"*/
      E_NA_ENGY_T8,/*"反向有功费率8电量"*/
    E_ACTIVE_PWR_BLK = 0x0020,/*"有功功率数据块"*/
      E_ACTIVE_PWR,/*"总有功功率"*/
    E_VOLT_BLK = 0x0030,/*"电压数据块"*/
      E_VOLT_LA,/*"A相电压"*/
    E_CURR_BLK = 0x0040,/*"电流数据块"*/
      E_CURR_LA,/*"A相电流"*/
    E_FACTOR_BLK = 0x0050,/*"功率因数数据块"*/
      E_FACTOR,/*"总功率因数"*/
    E_PHASE_BLK = 0x0060,/*"功率因数相角数据块"*/
      E_PHASE,/*"相角"*/
    E_A_ENGY_COM_BLK = 0x0070,/*"组合有功电量数据块"*/
      E_A_ENGY_COM,/*"组合有功总电量"*/
      E_A_ENGY_COM_T1,/*"组合有功费率1电量"*/
      E_A_ENGY_COM_T2,/*"组合有功费率2电量"*/
      E_A_ENGY_COM_T3,/*"组合有功费率3电量"*/
      E_A_ENGY_COM_T4,/*"组合有功费率4电量"*/
      E_A_ENGY_COM_T5,/*"组合有功费率5电量"*/
      E_A_ENGY_COM_T6,/*"组合有功费率6电量"*/
      E_A_ENGY_COM_T7,/*"组合有功费率7电量"*/
      E_A_ENGY_COM_T8,/*"组合有功费率8电量"*/
    E_R_ENGY_I_BLK = 0x0080,/*"I象限无功电量数据块"*/
      E_R_ENGY_I,/*"I象限无功总电量"*/
      E_R_ENGY_I_T1,/*""*/
      E_R_ENGY_I_T2,/*""*/
      E_R_ENGY_I_T3,/*""*/
      E_R_ENGY_I_T4,/*""*/
      E_R_ENGY_I_T5,/*""*/
      E_R_ENGY_I_T6,/*""*/
      E_R_ENGY_I_T7,/*""*/
      E_R_ENGY_I_T8,/*""*/
    E_R_ENGY_II_BLK = 0x0090,/*"II象限无功电量数据块"*/
      E_R_ENGY_II,/*""*/
      E_R_ENGY_II_T1,/*""*/
      E_R_ENGY_II_T2,/*""*/
      E_R_ENGY_II_T3,/*""*/
      E_R_ENGY_II_T4,/*""*/
      E_R_ENGY_II_T5,/*""*/
      E_R_ENGY_II_T6,/*""*/
      E_R_ENGY_II_T7,/*""*/
      E_R_ENGY_II_T8,/*""*/
    E_R_ENGY_III_BLK = 0x00a0,/*"III象限无功电量数据块"*/
      E_R_ENGY_III,/*""*/
      E_R_ENGY_III_T1,/*""*/
      E_R_ENGY_III_T2,/*""*/
      E_R_ENGY_III_T3,/*""*/
      E_R_ENGY_III_T4,/*""*/
      E_R_ENGY_III_T5,/*""*/
      E_R_ENGY_III_T6,/*""*/
      E_R_ENGY_III_T7,/*""*/
      E_R_ENGY_III_T8,/*""*/
    E_R_ENGY_IV_BLK = 0x00b0,/*"IV象限无功电量数据块"*/
      E_R_ENGY_IV,/*""*/
      E_R_ENGY_IV_T1,/*""*/
      E_R_ENGY_IV_T2,/*""*/
      E_R_ENGY_IV_T3,/*""*/
      E_R_ENGY_IV_T4,/*""*/
      E_R_ENGY_IV_T5,/*""*/
      E_R_ENGY_IV_T6,/*""*/
      E_R_ENGY_IV_T7,/*""*/
      E_R_ENGY_IV_T8,/*""*/
    E_REACTIVE_PWR_BLK = 0x00c0,/*"无功功率数据块"*/
      E_REACTIVE_PWR,/*"总无功功率"*/
    E_A_VOLT_CONT_BLK = 0x00d0,/*"A相电压谐波含有率(总=THD)"*/
      E_A_VOLT_CONT_TOTAL,/*"总"*/
      E_A_VOLT_CONT_1,/*"1次"*/
      E_A_VOLT_CONT_2,/*""*/
      E_A_VOLT_CONT_3,/*""*/
      E_A_VOLT_CONT_4,/*""*/
      E_A_VOLT_CONT_5,/*""*/
      E_A_VOLT_CONT_6,/*""*/
      E_A_VOLT_CONT_7,/*""*/
      E_A_VOLT_CONT_8,/*""*/
      E_A_VOLT_CONT_9,/*""*/
      E_A_VOLT_CONT_10,/*""*/
      E_A_VOLT_CONT_11,/*""*/
      E_A_VOLT_CONT_12,/*""*/
      E_A_VOLT_CONT_13,/*""*/
      E_A_VOLT_CONT_14,/*""*/
      E_A_VOLT_CONT_15,/*""*/
    E_A_CURR_CONT_BLK = 0x00e0,/*"A相电流谐波含有率(总=THD)"*/
      E_A_CURR_CONT_TOTAL,/*"总"*/
      E_A_CURR_CONT_1,/*"1次"*/
      E_A_CURR_CONT_2,/*""*/
      E_A_CURR_CONT_3,/*""*/
      E_A_CURR_CONT_4,/*""*/
      E_A_CURR_CONT_5,/*""*/
      E_A_CURR_CONT_6,/*""*/
      E_A_CURR_CONT_7,/*""*/
      E_A_CURR_CONT_8,/*""*/
      E_A_CURR_CONT_9,/*""*/
      E_A_CURR_CONT_10,/*""*/
      E_A_CURR_CONT_11,/*""*/
      E_A_CURR_CONT_12,/*""*/
      E_A_CURR_CONT_13,/*""*/
      E_A_CURR_CONT_14,/*""*/
      E_A_CURR_CONT_15,/*""*/
    E_FREQ = 0x0E00,/*"电网频率"*/
    E_SYS_TIME,/*"系统时间"*/
    E_PD_TIME,/*"掉电时间"*/
    E_A_PULSE_CONST,/*"有功脉冲常数"*/
    E_CURR_LN,/*"零线电流"*/
    E_CURR_RESIDUAL,/*"剩余电流"*/
    E_PULSE_WIDTH,/*"脉冲宽度"*/
    E_A_ENGY_COM_TYPE,/*"有功组合方式特征字"*/
    E_PWR_DIR,/*"功率方向"*/
    E_LRM_TMP_LINE_IN,/*"火线进线温度(秒)"*/
    E_LRM_TMP_LINE_OUT,/*"火线出线温度(秒)"*/
    E_LRM_TMP_N_IN,/*"零线进线温度(秒)"*/
    E_LRM_TMP_N_OUT,/*"零线出线温度(秒)"*/
    E_AMP_ERR,/*"功率幅值误差"*/
    E_A_VOLT_AVAILAB,/*"电压总谐波有效值(含量)"*/
    E_A_CURR_AVAILAB,/*"电流总谐波有效值(含量)"*/
    E_LRM_CLR = 0x0FE0,/*"计量法定数据清零"*/
    E_LRM_JUSTMETER,/*"计量校表"*/
    E_LRM_JUSTIME,/*"修改时钟"*/
    E_LRM_PWR_DOWN,/*"法定模块掉电处理"*/
    E_LRM_PCTR_INIT,/*"脉冲控制口上电初始化"*/
 }EN_LRM_DATA_ID;


/*"需量计量模块数据标准对象名"*/ 
 typedef enum 
 {
    E_PA_MAX_DEMAND_BLK = 0x1000,/*"正向有功最大需量数据块"*/
      E_PA_MAX_DEMAND,/*"正向有功总最大需量数据"*/
      E_PA_MAX_DEMAND_T1,/*"正向有功费率1最大需量数据"*/
      E_PA_MAX_DEMAND_T2,/*"正向有功费率2最大需量数据"*/
      E_PA_MAX_DEMAND_T3,/*"正向有功费率3最大需量数据"*/
      E_PA_MAX_DEMAND_T4,/*"正向有功费率4最大需量数据"*/
      E_PA_MAX_DEMAND_T5,/*"正向有功费率5最大需量数据"*/
      E_PA_MAX_DEMAND_T6,/*"正向有功费率6最大需量数据"*/
      E_PA_MAX_DEMAND_T7,/*"正向有功费率7最大需量数据"*/
      E_PA_MAX_DEMAND_T8,/*"正向有功费率8最大需量数据"*/
    E_NA_MAX_DEMAND_BLK = 0x1010,/*"反向有功最大需量数据块"*/
      E_NA_MAX_DEMAND,/*"反向有功总最大需量数据"*/
      E_NA_MAX_DEMAND_T1,/*"反向有功费率1最大需量数据"*/
      E_NA_MAX_DEMAND_T2,/*"反向有功费率2最大需量数据"*/
      E_NA_MAX_DEMAND_T3,/*"反向有功费率3最大需量数据"*/
      E_NA_MAX_DEMAND_T4,/*"反向有功费率4最大需量数据"*/
      E_NA_MAX_DEMAND_T5,/*"反向有功费率5最大需量数据"*/
      E_NA_MAX_DEMAND_T6,/*"反向有功费率6最大需量数据"*/
      E_NA_MAX_DEMAND_T7,/*"反向有功费率7最大需量数据"*/
      E_NA_MAX_DEMAND_T8,/*"反向有功费率8最大需量数据"*/
    E_DEMAND_PERIOD = 0x1E00,/*"需量周期"*/
    E_DEMAND_SLID_PERIOD,/*"滑差时间"*/
    E_DU_CLR = 0x1FE0,/*"需量计算空间清零"*/
    E_MAX_DEMAND_CLR,/*"最大需量清零"*/
 }EN_DU_DATA_ID;


/*"分时模块数据标准对象名"*/ 
 typedef enum 
 {
    E_ACTIVE_CALENDAR_DAY_TBL = 0x2000,/*"当前套日时段表"*/
    E_PASSIVE_CALENDAR_DAY_TBL = 0x2040,/*"备用套日时段表"*/
    E_ACTIVE_CALENDAR_SEASON_TBL = 0x2080,/*"当前套时区表"*/
    E_PASSIVE_CALENDAR_SEASON_TBL = 0x20c0,/*"备用套时区表"*/
    E_SPECIAL_DAY_TBL = 0x2100,/*"公共假日表"*/
    E_TARIFF_NOW = 0x2E00,/*"当前费率号"*/
    E_SEASON_TBL_ACTIVE_PASSIVE_SW_TIME,/*"两套时区表切换时间"*/
    E_PROG_DAY_TBL,/*"编程前日时段表"*/
    E_PROG_SEASON_TBL,/*"编程前年时区表"*/
    E_DAY_TBL_ACTIVE_PASSIVE_SW_TIME,/*"两套日时段切换时间"*/
    E_SEASON_SLOT_NUM,/*"年时区数"*/
    E_DAY_TBL_NUM,/*"日时段表数"*/
    E_DAY_SLOT_NUM,/*"日时段数(每日切换数)"*/
    E_TARIFF_NUM,/*"费率数"*/
    E_SPECIAL_DAY_NUM,/*"公共假日数"*/
    E_PROG_HOLIDAY,/*"编程前节假日"*/
    E_WEEKEND_TYPE,/*"周休日特征字"*/
    E_WEEKEND_DAY_TBL_ID,/*"周休日釆用的日时段表号"*/
    E_CHECK_SWITCH_ACTION = 0x2FE0,/*"切换检查"*/
    E_FIND_NOW_TRIFF_ACTION,/*"查询费率"*/
    E_SYSTIME_UPDATE_ACTION,/*"时间更新重新查询费率"*/
 }EN_TR_DATA_ID;


/*"预付费模块数据标准对象名"*/ 
 typedef enum 
 {
    PM_NULL = 0x3E00,/*"PM_NULL"*/
 }EN_PM_DATA_ID;


/*"电表状态监测模块数据标准对象名"*/ 
 typedef enum 
 {
    E_EVNT_DURATION_BLK = 0x4000,/*"事件累计时间"*/
      E_PWR_DOWN_DURATION,/*"掉电累计时间"*/
      E_OVER_CURR_LA_DURATION,/*"过流事件累计时间"*/
    E_MIN_ACTIVEPOWER_BLK = 0x4040,/*"每分钟平均功率"*/
      E_MIN_ACTIVEPOWER_T,/*"总"*/
      E_MIN_ACTIVEPOWER_A,/*"A相"*/
    E_OVERCURR_IL_THR = 0x4E00,/*"过流事件电流触发下限"*/
    E_OVERCURR_TIME_THR,/*"过流事件判断延时时间"*/
    E_MNT_EVNT_TIME_S,/*"记录起始时间"*/
    E_MNT_EVNT_TIME_E,/*"记录结束时间"*/
    E_RLY_TRIP_CTRL_CURR_THR,/*"继电器拉闸控制电流门限值"*/
    E_FAC_STATUS,/*"工厂状态"*/
    E_CLK_BAT_VOLT,/*"时钟电池电压"*/
    E_RLY_STATUS_NOW,/*"继电器输出当前状态"*/
    E_EVT_LCDALARM_MODE,/*"LCD和继电器事件报警模式字"*/
    E_EVT_BGTALARM_MODE,/*"背光事件报警模式字"*/
    E_MUERR_TIME_THR,/*"计量故障判断时间"*/
    E_CLKERR_TIME_THR,/*"时钟故障判断时间"*/
    E_RST_INFO,/*"复位源"*/
    E_RST_ADDR,/*"复位地址"*/
    E_UPDATA,/*"计量芯传递给管理芯的数据"*/
    E_TMP_LINE_IN,/*"火线进线温度(秒)"*/
    E_TMP_LINE_OUT,/*"火线出线温度(秒)"*/
    E_TMP_N_IN,/*"零线进线温度(秒)"*/
    E_TMP_N_OUT,/*"零线出线温度(秒)"*/
    E_MNT_ESAM_ID,/*"ESAM序号"*/
    E_MNT_MTR_TMP,/*"表内温度"*/
    E_MNT_MTR_HMDTY,/*"表内湿度"*/
    E_MNT_AMP_ERR,/*"功率幅值误差"*/
    E_ACCRCY_RFRSHT,/*"误差自检周期"*/
    E_MNT_EVNT_CLR = 0x4FE0,/*"事件清零"*/
    E_MNT_PWR_DOWN,/*"掉电处理"*/
    E_OUT_FAC,/*"退出工厂"*/
    E_FIRST_PWR_UP_OVER,/*"第一次上电初始化完成"*/
    E_RESET_FAC_REMAIN_TIME,/*"重置工厂状态剩余时间"*/
    E_PWRUP_EVTINIT,/*"上电补事件"*/
    E_EVTPARACAL,/*"计算默认参数"*/
    E_UPDATA_SET,/*"计量芯的数据传递给管理芯"*/
    E_DOWNDATA_GET,/*"管理芯的数据传递给计量芯"*/
 }EN_MNT_DATA_ID;


/*"用户接口模块数据标准对象名"*/ 
 typedef enum 
 {
    E_SAFE_MODE_LIST = 0x5000,/*"安全模式参数"*/
    E_NO_BACKUP_BEGIN_BLOCK = 0x5080,/*"！！是否RAM备份分割！！"*/
    E_CYCLE_DIS_OBJ_LIST = 0x5100,/*"循环显示列表"*/
    E_KEY_DIS_OBJ_LIST = 0x5180,/*"按键显示列表"*/
    E_PWRUP_DIS_ALL_TIME = 0x5E00,/*"上电全显时间"*/
    E_BACKLIGHT_SHOW_TIME,/*"背光点亮时间"*/
    E_BACKLIGHT_SHOW_TIME_ON_VIEW,/*"显示查看背光点亮时间"*/
    E_PD_NOKEY_RETURN_TIME,/*"无电按键屏幕驻留最大时间"*/
    E_PWR_SHOW_POINT,/*"显示功率（最大需量）小数位数"*/
    E_ENGY_SHOW_POINT,/*"显示电能小数位数"*/
    E_DIS_SEG_MODE_CTRL_12,/*"液晶①②字样意义"*/
    E_NOKEY_RETURN_TIME,/*"无按键返回时间"*/
    E_CYCLE_SHOW_TIME,/*"循显间隔时间"*/
    E_CYCLE_SHOW_NUM,/*"循显项目数"*/
    E_KEY_SHOW_NUM,/*"按显项目数"*/
    E_RS485_1_BAUDRATE,/*"第1路485波特率"*/
    E_RPTRST_TIME,/*"645主动上报状态字自复位延时时间"*/
    E_MTR_RUN_MODE,/*"电表运行特征字"*/
    E_EXT_MODE1,/*"扩展参数1"*/
    E_EXT_MODE2,/*"扩展参数2"*/
    E_EXT_MODE3,/*"扩展参数3"*/
    E_EXT_MODE4,/*"扩展参数4"*/
    E_EXT_MODE5,/*"扩展参数5"*/
    E_EXT_MODE6,/*"扩展参数6"*/
    E_EXT_MODE7,/*"扩展参数7"*/
    E_EXT_MODE8,/*"扩展参数8"*/
    E_EXT_MODE9,/*"扩展参数9"*/
    E_EXT_MODE10,/*"扩展参数10"*/
    E_COMM_ADDR,/*"通信地址"*/
    E_GROUP_ADDR,/*"组地址"*/
    E_AUTHEN_PASSWORD,/*"认证密码"*/
    E_645REPORT_MODE,/*"645主动上报模式字"*/
    E_NO_BACKUP_BEGIN,/*"！！是否RAM备份分割！！"*/
    E_REGU_VOLT,/*"额定电压"*/
    E_REGU_CURR,/*"额定电流"*/
    E_MAX_CURR,/*"最大电流"*/
    E_SOFT_VER_DATE,/*"软件版本日期"*/
    E_HARD_VER_DATE,/*"硬件版本日期"*/
    E_ACT_ACCU_GRADE,/*"有功准确度等级"*/
    E_FAC_NUM,/*"厂家代码"*/
    E_SOFTWARE_VER,/*"软件版本号"*/
    E_HARDWARE_VER,/*"硬件版本号"*/
    E_LONGITUDE,/*"经度"*/
    E_LATITUDE,/*"纬度"*/
    E_HEIGHT,/*"高度"*/
    E_SOFT_RECORD_ID,/*"软件备案号"*/
    E_MANUFACTOR_EXT_INFO,/*"厂家扩展信息"*/
    E_SUP_PROFILE_LIST,/*"支持规约列表"*/
    E_PROTOCOL_VER,/*"协议版本号"*/
    E_METER_TYPE,/*"电表型号"*/
    E_ASSET_NUM,/*"资产管理编码"*/
    E_PRODUCT_DATE,/*"生产日期"*/
    E_PRO_CONSISTENCY,/*"协议一致性块"*/
    E_FUN_CONSISTENCY,/*"功能一致性块"*/
    E_PP_OAD,/*"全局OAD"*/
    E_PP_Program_OAD,/*"编程OAD"*/
    E_CUR_CHANNL,/*"当前通信通道"*/
    E_RUN_STA_1,/*"电表运行状态字1"*/
    E_RUN_STA_2,/*"电表运行状态字2"*/
    E_RUN_STA_3,/*"电表运行状态字3"*/
    E_RUN_STA_4,/*"电表运行状态字4"*/
    E_RUN_STA_5,/*"电表运行状态字5"*/
    E_RUN_STA_6,/*"电表运行状态字6"*/
    E_RUN_STA_7,/*"电表运行状态字7"*/
    E_METER_SAMPLE_STATUS,/*"计量芯采样状态字"*/
    E_SAMPLE_STATUS,/*"采样状态字"*/
    E_CLK_BAT_TIME,/*"时钟电池运行时间"*/
    E_RELAY_UNIT_POWER,/*"跳闸继电器单元(特殊处理)"*/
    E_SOFT_VER_32,/*"内部软件版本32字节"*/
    E_UIDATA_CLR = 0x5FE0,/*"电表用户数据清零"*/
    E_SHOW_SCREEN_APPOINT,/*"显示指定屏（功能模块）"*/
    E_SHOW_FLASH,/*"显示即时刷新"*/
    E_SHOW_SCREEN_ALL,/*"全屏显示"*/
    E_SHOW_SCREEN_NEXT,/*"显示下翻"*/
    E_SHOW_SCREEN_LAST,/*"显示上翻"*/
    E_AUTHALLOW_KEEP,/*"双芯认证持续允许"*/
    E_PRGEVENT_RCRD,/*"编程事件记录"*/
    E_SHOW_SCREEN_DLT698,/*"显示查看（698协议）"*/
 }EN_UI_DATA_ID;


/*"记录快照模块(数据)模块数据标准对象名"*/ 
 typedef enum 
 {
    E_RCRD_CNT_BLK = 0x6000,/*"记录累计次数数据块"*/
      E_RCRD_CNT_BLK_PWR_DOWN,/*"掉电事件次数"*/
    E_RCRD_CUR_CNT = 0x6080,/*"当前记录数数据块"*/
      E_RCRD_CUR_CNT_PD,/*"掉电事件当前记录"*/
    E_RCRD_MAX_CNT = 0x6100,/*"最大记录数数据块"*/
      E_RCRD_MAX_CNT_PD,/*"掉电事件最大记录数"*/
    E_RCRD_CUR_VALUE = 0x6180,/*"当前值记录表数据块"*/
      E_RCRD_CUR_VALUE_PD,/*"掉电事件当前值记录表"*/
    E_RCRD_TIME_STATUS = 0x6200,/*"时间状态记录表数据块"*/
      E_RCRD_TIME_STATUS_PD,/*"掉电事件时间状态记录表"*/
    E_MOUTH_BILLDAY = 0x6280,/*"月结算日参数块"*/
      E_MOUTH_BILLDAY1,/*"第一月结算日"*/
      E_MOUTH_BILLDAY2,/*"第二月结算日"*/
      E_MOUTH_BILLDAY3,/*"第三月结算日"*/
    PWR_DOWN_OBJ_LIST = 0x6E00,/*"掉电事件关联对象"*/
    MTR_COVER_OPEN_OBJ_LIST,/*"开表盖事件关联对象"*/
    PRG_EVT_OBJ_LIST,/*"编程事件关联对象"*/
    CLRMTR_OBJ_LIST,/*"电表清零"*/
    EVNT_CLR_OBJ_LIST,/*"事件清零"*/
    DAY_TBL_PRG_OBJ_LIST,/*"日时段表编程事件"*/
    SEASON_TBL_PRG_OBJ_LIST,/*"年时区表编程事件"*/
    WEEKEND_PRG_OBJ_LIST,/*"周休日编程"*/
    A_ENGY_COM_OBJ_LIST,/*"组合有功模式字编程事件"*/
    BILL_DAY_PRG_OBJ_LIST,/*"结算日编程事件"*/
    JUSTTIME_EVT_OBJ_LIST,/*"校时事件"*/
    RELAY_ERR_OBJ_LIST,/*"继电器故障事件"*/
    PWR_FAILURE_OBJ_LIST,/*"电源异常事件"*/
    HOLIDAY_PRG_OBJ_LIST,/*"节假日编程事件"*/
    OVERCURR_OBJ_LIST,/*"A相过流事件"*/
    RTC_ERR_OBJ_LIST,/*"时钟故障关联对象"*/
    MS_ERR_OBJ_LIST,/*"计量芯片故障关联对象"*/
    MMT_FRZ_OBJ_LIST,/*"瞬时冻结"*/
    MIN_FRZ_OBJ_LIST,/*"分钟冻结"*/
    E_LD_TYPE1_PERIOD,/*"第一类负荷间隔"*/
    HOUR_FRZ_OBJ_LIST,/*"小时冻结"*/
    E_LD_TYPE2_PERIOD,/*"第二类负荷间隔"*/
    DAY_FRZ_OBJ_LIST,/*"日冻结"*/
    E_LD_TYPE3_PERIOD,/*"第三类负荷间隔"*/
    MOUTH_BILL_FRZ_OBJ_LIST,/*"月结算"*/
    E_LD_TYPE4_PERIOD,/*"第四类负荷间隔"*/
    MOUTH_FRZ_OBJ_LIST,/*"月冻结"*/
    E_LD_TYPE5_PERIOD,/*"第五类负荷间隔"*/
    SEASON_TBL_SW_FRZ_OBJ_LIST,/*"时区表切换冻结"*/
    E_LD_TYPE6_PERIOD,/*"第六类负荷间隔"*/
    DAY_TBL_SW_FRZ_OBJ_LIST,/*"日时段表切换冻结"*/
    E_LD_ST_TIME,/*"负荷记录起始时间"*/
    E_TIME_STAMP,/*"时间戳"*/
    E_OPER_CODE,/*"操作者代码"*/
    E_FRZ_ST_TIME,/*"冻结发生时间"*/
    E_RCRD_CLR = 0x6FE0,/*"记录清零"*/
    E_RCRD_ADD_ONE_ROAD,/*""*/
    E_RCRD_TRIGGER_RCRD,/*"触发一次冻结"*/
    E_RCRD_ADD_MULT_ROAD,/*""*/
    E_RCRD_TIME_ADJUST,/*"改时间记录调整方法"*/
    E_RCRD_DEL_ONE_ROAD,/*""*/
    E_RCRD_PD_ACTION,/*"记录模块掉电处理"*/
    E_RCRD_DEL_ALL_ROAD,/*""*/
 }EN_RCRD_DATA_ID;


/*"记录快照模块(文件)标准对象名"*/
 typedef enum 
 {
    E_PWR_DOWN_EVNT_RCRD = 0x7000,  /*"掉电事件"*/
    E_MTR_COVER_OPEN_EVNT_RCRD,  /*"开表盖事件"*/
    E_MTR_CLR_EVNT_RCRD,  /*"电表清零"*/
    E_EVNT_CLR_EVNT_RCRD,  /*"事件清零"*/
    E_JUST_TIME_EVNT_RCRD,  /*"校时事件"*/
    E_RELAY_ERR_EVNT_RCRD,  /*"继电器故障事件"*/
    E_PWR_FAILURE_EVNT_RCRD,  /*"电源异常事件"*/
    E_OSCILLATOR_ERR_RCRD,  /*"晶振故障事件"*/
    E_MGMT_COMM_ERR_RCRD,  /*"管理单元通信异常"*/
    E_SYS_RST_RCRD,  /*"复位故障事件"*/
    E_RTC_ERR_EVNT_RCRD,  /*"时钟故障事件"*/
    E_NONLEGAL_UPGRADE_RCRD,  /*"管理单元升级"*/
    E_NONLEGAL_REPLACE_RCRD,  /*"管理单元更换"*/
    E_BROADCAST_JUST_TIME_RCRD,  /*"广播校时事件"*/
    E_MS_ERR_EVNT_RCRD,  /*"计量芯片故障事件"*/
    E_EVT_END_RCRD,  /*"事件类对象结束"*/
    E_MMT_FRZ_RCRD,  /*"瞬时冻结"*/
    E_MIN_FRZ_RCRD,  /*"分钟冻结"*/
    E_HOUR_FRZ_RCRD,  /*"小时冻结"*/
    E_DAY_FRZ_RCRD,  /*"日冻结"*/
    E_MONTH_BILL_FRZ_RCRD,  /*"月结算"*/
    E_MONTH_FRZ_RCRD,  /*"月冻结"*/
    E_LR_TYPE1_RCRD,  /*"负荷记录Type1记录"*/
    E_LR_TYPE2_RCRD,  /*"负荷记录Type2记录"*/
    E_LR_TYPE3_RCRD,  /*"负荷记录Type3记录"*/
    E_LR_TYPE4_RCRD,  /*"负荷记录Type4记录"*/
    E_STEP_BILL_FRZ_RCRD,  /*"结束记录"*/
}EN_RCRD_FILE_ID;


/*"自动化生产模块数据标准对象名"*/ 
 typedef enum 
 {
    E_PRODUCT_NOUSED = 0x8000,/*"自动化预留块数据"*/
    E_PRODUCT_ID1 = 0x8E00,/*"电能表唯一标识1"*/
    E_PRODUCT_ID2,/*"电能表唯一标识2"*/
    E_PRODUCT_ID3,/*"电能表唯一标识3"*/
    E_AUTO_READEXTDATA = 0x8FE0,/*"自动化生产抄读命令"*/
    E_AUTO_WRTEXTDATA,/*"自动化生产设置命令"*/
 }EN_SMT_DATA_ID;


/*"记录快照模块关联对象数据标准对象名"*/
 typedef enum 
 {
    E_EVT_LST_START_NULL,  /*"记录起始对象列表"*/
    E_EVTLST_CLRMTR,  /*"电表清零"*/
    E_EVTLST_PD_S,  /*"掉电事件起始"*/
    E_EVTLST_PD_E,  /*"掉电事件结束"*/
    E_EVTLST_JUSTTIME_S,  /*"校时事件起始"*/
    E_EVTLST_JUSTTIME_E,  /*"校时事件结束"*/
    E_EVTLST_MTR_COVER_OPEN_S,  /*"开表盖事件起始"*/
    E_EVTLST_MTR_COVER_OPEN_E,  /*"开表盖事件结束"*/
    E_EVTLST_RELAY_ERR_S,  /*"继电器故障起始"*/
    E_EVTLST_RELAY_ERR_E,  /*"继电器故障结束"*/
    E_EVTLST_SOSCILLATOR_ERR_S,  /*"晶振故障数据列表"*/
    E_EVTLST_SYS_RST_S,  /*"复位故障事件数据"*/
    E_EVTLST_MGMT_ERR_S,  /*"管理单元通信异常起始"*/
    E_EVTLST_MGMT_ERR_E,  /*"管理单元通信异常结束"*/
    E_EVTLST_UPGRADE_S,  /*"非法定单元升级起始"*/
    E_EVTLST_UPGRADE_E,  /*"非法定单元升级结束"*/
    E_EVTLST_NOLEG_REPALCE_S,  /*"非法定单元更换起始"*/
    E_EVTLST_NOLEG_REPALCE_E,  /*"非法定单元更换结束"*/
    EVTLST_END,  /*"事件对象结束"*/
    E_EVTLST_PWR_FAILURE_S,  /*"电源异常起始"*/
    E_EVTLST_PWR_FAILURE_E,  /*"电源异常结束"*/
    E_EVTLST_EVNT_CLR,  /*"事件清零"*/
    E_EVTLST_RTC_ERR_S,  /*"时钟故障事件起始"*/
    E_EVTLST_MS_ERR_S,  /*"计量芯片故障事件起始"*/
    E_EVTLST_MS_ERR_E,  /*"计量芯片故障事件结束"*/
    E_FRZ_LST_MON_FRZ,  /*"瞬时冻结"*/
    E_FRZ_LST_MIN_FRZ,  /*"分钟冻结"*/
    E_FRZ_LST_HOUR_FRZ,  /*"小时冻结"*/
    E_FRZ_LST_DAY_FRZ,  /*"日冻结"*/
    E_FRZ_LST_MOUTH_BILL_FRZ,  /*"月结算冻结"*/
    E_FRZ_LST_MOUTH_FRZ,  /*"月冻结"*/
    E_FRZ_LST_LR_TYPE1,  /*"负荷记录Type1"*/
    E_FRZ_LST_LR_TYPE2,  /*"负荷记录Type2"*/
    E_FRZ_LST_LR_TYPE3,  /*"负荷记录Type3"*/
    E_FRZ_LST_LR_TYPE4,  /*"负荷记录Type4"*/
    E_FRZ_LST_LR_TYPE5,  /*"负荷记录Type5"*/
}EN_FELXOBJ_ID;

extern const ST_FMT_ATTR stFormatList[65];

#endif