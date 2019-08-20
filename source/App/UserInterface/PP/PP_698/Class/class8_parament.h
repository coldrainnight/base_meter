/*"***********************************************************		
FileName: CLASS8_PARAMENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 参变量类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS8_PARAMENT_H_
#define _CLASS8_PARAMENT_H_
/*"******************************************************"*/ 
#include "class_comm.h"
/*"-------------------------------------------------------"*/

/*8类分两个列表，只读和可写列表*/
typedef struct
{
   INT16U OI;             /*OI内容*/   
   INT8U  MAX_IID;        /*MAX_IID,看是否有必要定义*/ 
   INT8U  Seq_Num;        /*列表序列号，同时也凑成4字节对讲，主要是为了CLASS8 ST_OAD_OBIS_INFO列表查找用*/

   const INT8U *Pointer;  /*类型指针*/   
}ST_Parament_Object;    /*属性元素*/

typedef struct
{
   INT16U OI;             /*OI内容*/   
   INT8U  AID;            /*AID值*/
   INT8U  Seq_Num;        /*列表序列号*/
   
   INT8U  MAX_IID;        /*MAX_IID,看是否有必要定义*/
   INT8U  Right_RW;       /*读写权限,0:只读，FF:可读写*/
   //INT8U  Reser[2];
   const INT8U *Pointer;  /*类型指针*/   
}ST_Parament_SEPECIAL_Object;    /*属性元素*/

/*CLASS8对应的序列号*/
typedef enum
{
  E_SEQ_CUR_PAY_FILE   = 1,     /*(当前）钱包文件 */
  E_SEQ_DATE_TIME,              /*日期时间  */
  E_SEQ_ADDR,                   /*通信地址  */
  E_SEQ_METER_ID,               /*表号*/
  E_SEQ_USERID,                 /*客户编号*/
  E_SEQ_DEVICE_ADDR,            /*设备地理位置*/
  E_SEQ_GROUP_ADDR,             /*组地址  */
  E_SEQ_CLK_SRC,                /*时钟源  */
  E_SEQ_LCD_PARA,               /*LCD参数  */
  E_SEQ_YEAR_SLOT_SWITCH_TIME,  /*两套时区表切换时间  */
  E_SEQ_DAILY_SLOT_SWITCH_TIME, /*两套日时段切换时间  */
  E_SEQ_TRIFF_SWITCH_TIME,      /*两套分时费率切换时间  */
  E_SEQ_STEP_SWITCH_TIME,       /*两套阶梯电价切换时间  */
  E_SEQ_SLOT_NUM,               /*时区时段数  */
  E_SEQ_STEP_NUM,               /*阶梯数  */
  //E_SEQ_PASS_NUM,               /*密钥总条数  这个现在好像只读了 */
  E_SEQ_METER_NUM,              /*计量元件数  */
  E_SEQ_HOLIDAY_NUM,            /*公共假日 公共假日数<=254 */
  E_SEQ_WEEK_TYPE,              /*周休日特征字  */
  E_SEQ_WEEK_SLOT_NUM,          /*周休日釆用的日时段表号  */

  E_SEQ_BAK_YEAR_SLOT_TABLE,    /*备用套时区表  时区表数<=14*/
  E_SEQ_BAK_DAILY_SLOT_TABLE,   /*备用套日时段表 日时段表数（q≤8)*/
  E_SEQ_BAK_TRIFF_PRICE_TABLE,  /*备用套费率电价  费率数（k≤63）*/
  E_SEQ_BAK_STEP_PRICE_TABLE,   /*备用套阶梯电价  阶梯数<=6  */
  E_SEQ_CT,                     /*电流互感器变比*/
  E_SEQ_PT,                     /*电压互感器变比*/ 
  E_SEQ_ALARM_MONEY_LIMIT,      /*报警金额限值  */
  E_SEQ_OTHER_MONEY_LIMIT,      /*其它金额限值  */
  //E_SEQ_ALARM_POWER_LIMIT,      /*报警电量限值  电量限值是不是不用了?*/
  //E_SEQ_OTHER_POWER_LIMIT,      /*其它电量限值  */
  E_SEQ_SAFE_TIME,              /*安全认证有效时长 最新协议删掉了? */

  E_SEQ_PULSE_WIDTH,            /*校表脉冲宽度  */
  E_SEQ_MANAGE_CODE,            /*资产管理编码  */
  E_SEQ_METER_RUN_TYPE,         /*电能表运行特征字1  */
  E_SEQ_ACTIVE_TYPE,            /*有功组合方式特征字  */
  E_SEQ_BILL_DAY,               /*结算日       */
  E_SEQ_PASS,                   /*认证密码  比较特殊，厂外只能写，不能读     */
  E_SEQ_REMOTE,                 /*遥控        */

  E_SEQ_VOLT_RATE_PARA,         /*电压合格率参数*/
  E_SEQ_MAX_DEMAND_TIME,        /*最大需量周期*/
  E_SEQ_SKIP_TIME,              /*功率滑差时间  */
  E_SEQ_R1_ACTION_TYPE,         /*无功组合方式1特征字  */
  E_SEQ_R2_ACTION_TYPE,         /*无功组合方式2特征字  */
  E_SEQ_INTERVAL_DEMAND_TIME,   /*期间需量冻结周期*/
  E_SEQ_SIGNLE,                 /*信号强度      */
  
  
  #ifdef THREE_PHASE
  E_SEQ_DEMAND_TEMP_TIME ,      /*需量临时数据，当前滑差数和周期数*/
  E_SEQ_DEMAND_DATA,            /*需量临时数据，当前平均功率值数组*/
  E_SEQ_DEMAND_TEMP_VALUE,      /*需量临时数据，正向有功临时需量*/
  #endif
 
  E_SEQ_SAFE_MODE_PARA,         /*安全模式参数*/
  E_SEQ_FLOWER_REPORT_STATUS = 100,  /*电能表跟随上报状态字*/
  E_SEQ_EVENT_START_TIME,       /*事件发生时间*/	
  E_SEQ_EVENT_END_TIME,         /*事件结束时间*/
  E_SEQ_FREEZE_TIME,            /*数据冻结时间*/
  E_SEQ_EVENT_RECORD_NUM,       /*事件记录序号*/
  E_SEQ_FREEZE_RECORD_NUM,      /*冻结记录序号  */
  E_SEQ_EVENT_SRC_NUM,          /*事件发生源  */
  E_SEQ_EVENT_CUR_NUM,          /*事件当前值  */

  E_SEQ_EVENT_REPORT_STATUS,    /*事件上报状态 统一填NULL了*/	
  E_SEQ_ADD_EVENT_REPORT_LIST,  /*新增事件列表 MAX_IID是个变化的*/ 
  //E_SEQ_EVENT_REPORT_LIST,      /*需上报事件列表*/ 
  
  E_SEQ_CUR_YEAR_SLOT_TABLE,    /*当前套时区表   时区表数<=14 */
  E_SEQ_CUR_DAILY_SLOT_TABLE,   /*当前套日时段表 日时段表数（q≤8） */
  E_SEQ_CUR_TRIFF_PRICE_TABLE,  /*当前套费率电价 费率数（k≤63）*/
  E_SEQ_CUR_STEP_PRICE_TABLE,   /*当前套阶梯电价 阶梯数<=6 */

  E_SEQ_REGU_VOLT,              /*额定电压  */
  E_SEQ_REGU_CURRENT,           /*额定电流基本电流  */
  E_SEQ_MAX_CURRENT,            /*最大电流  */
  E_SEQ_ACT_ACCU_GRADE,         /*有功准确度等级  */
  E_SEQ_ACT_CONST,              /*电能表有功常数  */
  E_SEQ_METER_TYPE,             /*电能表型号  */
  E_SEQ_SOFT_RECORD_ID,         /*软件备案号  */
  E_SEQ_HOLD_POWER,             /*保电 */	


  E_SEQ_R_ACT_ACCU_GRADE,       /*无功准确度等级  */
  E_SEQ_R_ACT_CONST,            /*电能表无功常数  */
  
  E_SEQ_FLOW_REPORT_MODE,       /*电能表跟随上报模式字*/
  E_SEQ_EVENT_NEED_REPORT_LIST, /*需上报事件对象列表，只读*/
  E_SEQ_JUST_TIME_MODE,         /*4000 校时模式*/
  E_SEQ_ALARM_STATUS,           /*告警状态，只读*/
  E_SEQ_CMD_STATUS,             /*命令状态，只读*/
  E_SEQ_MAX_NO_COMM_TIME,       /*允许与主站最大无通信时长（分钟），0表示不自动保电*/
  E_SEQ_AUTO_HOLD_TIME,         /*上电自动保电时长（分钟），0表示上电不自动保电*/
  E_SEQ_PRIO_SAFE_MODE_PARA,   /*显式安全模式参数*/  

  /*下面为自定义参数*/   
  E_SEQ_DL_LENGTH,              /*链路层长度      */
  
  E_SEQ_METER_MODE,             /*表型模式字*/
  E_SEQ_RELAY_TYPE,             /*继电器类型*/
  E_SEQ_ALM_RELAY_OUT_MODE,     /*报警继电器输出模式字*/
  E_SEQ_ALM_BL_MODE,            /*背光报警模式字*/  
  E_SEQ_WAI_RELAY_TIME,         /*外置继电器脉冲方式跳闸维持时间（单位:ms）*/
  E_SEQ_WAI_RELAY_AGAIN,        /*外置继电器脉冲方式再次跳闸电量（单位:Wh）*/
  E_SEQ_SOFT_BAK_NUMBER ,       /*软件备案号(自定义的备用)*/
  E_SEQ_EXTEND_MODE_1,          /*功能扩展模式字1*/
  E_SEQ_EXTEND_MODE_2,          /*功能扩展模式字2*/
  E_SEQ_EXTEND_MODE_3,          /*功能扩展模式字3*/
  E_SEQ_EXTEND_MODE_4,          /*功能扩展模式字4*/
  E_SEQ_EXTEND_MODE_5,          /*功能扩展模式字5*/
  E_SEQ_EXTEND_MODE_6,          /*功能扩展模式字6*/
  E_SEQ_EXTEND_MODE_7,          /*功能扩展模式字7*/
  E_SEQ_EXTEND_MODE_8,          /*功能扩展模式字8*/
  E_SEQ_EXTEND_MODE_9,          /*功能扩展模式字9*/
  E_SEQ_EXTEND_MODE_10,         /*功能扩展模式字10*/
  E_SEQ_DISP_VERSION,           /*电表显示版本号*/        
  E_SEQ_SOFTWARE_VER,           /*软件版本号(自定义)*/ 
  E_SEQ_PRODUCT_NUMBER_1,       /*生产识别唯一码1*/
  E_SEQ_PRODUCT_NUMBER_2,       /*生产识别唯一码2*/
  E_SEQ_PRODUCT_NUMBER_3,       /*生产识别唯一码3*/
  E_SEQ_CURRENT_TRIFF_NUM,      /*当前费率号*/
  E_SEQ_CURRENT_STEP_NUM,       /*当前阶梯号*/
  E_SEQ_STATUS_FACTOR,          /*电能表厂内状态*/
  E_SEQ_TEST_RELAY,             /*继电器测试命令*/
  E_SEQ_TEST_MULTI,             /*多功能端子投切测试*/
  E_SEQ_TEST_CHECK,             /*自检测试*/
  E_SEQ_PARAMENT_CRC,           /*参数CRC值 */
  
  E_SEQ_SET_DEFAULT,            /*恢复默认参数 */
  E_MAX_SEQ_PARAMENT            /*最大序列号*/

}EN_CLASS_8_SEQ_NUM;

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern INT8U Crypt_Type;


extern INT8U get_class8_object(SUB_OBJECT *pObject);
extern INT8S get_oi_right_type(INT8U ServiceType, SUB_OBJECT *pObject);
extern INT8U *Get_safe_mode(ST_OAD Oad, INT8U *pdata);
extern INT8U setLcdPara(INT8U *pdata);
extern INT8U GetLcdPara(INT8U *pdata);
extern INT8U get_class_parament_Obis(INT8U seq_num, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak,
                                    const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser,INT8U list_num);
extern INT8U get_class8_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT8U Action_safe_mode(INT8U AId,INT8U *Ioaddr,INT8U Iooffset);

/*"******************************************************"*/
#endif
