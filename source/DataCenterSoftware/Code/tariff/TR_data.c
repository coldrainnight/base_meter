/*"*************************************************************"*/
/*"FileName: TR_data.c"*/
/*"Author:    		Version :  "*/
/*"2018-07-18 09:43:26"*/
/*"Description: 分时模块对象列表定义"*/
/*"总字节数: 127          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\tariff\TR.h"
#include "TR_data.h"
const ST_BLK_DATA_ATTR stTrBlkDataList[] = 
 {////  
  {E_ACTIVE_CALENDAR_DAY_TBL,(E_FILE_TR_PARA <<16)+DAY_TBL1_LIST_SHIFT,sizeof(ST_DAY_TBL),DAY_TBL_NUM,0x17},
  {E_PASSIVE_CALENDAR_DAY_TBL,(E_FILE_TR_PARA <<16)+DAY_TBL2_LIST_SHIFT,sizeof(ST_DAY_TBL),DAY_TBL_NUM,0x17},
  {E_ACTIVE_CALENDAR_SEASON_TBL,(E_FILE_TR_PARA <<16)+SEASON_TBL1_SHIFT,sizeof(ST_SEASON_SLOT_FORM),SEASON_SLOT_NUM,0x14},
  {E_PASSIVE_CALENDAR_SEASON_TBL,(E_FILE_TR_PARA <<16)+SEASON_TBL2_SHIFT,sizeof(ST_SEASON_SLOT_FORM),SEASON_SLOT_NUM,0x14},
  {E_SPECIAL_DAY_TBL,(E_FILE_TR_PARA <<16)+SPECDAY_TBL_SHIFT,sizeof(ST_SPECDAY_FORM),SPEC_DAY_NUM,0x15},
 };
const ST_SINGLE_DATA_ATTR stTrSingleDataList[] = 
 {  
  {E_TARIFF_NOW,(RAM)+(INT32U)&stTrRunInfo.tariffId,1,0x01},
  {E_SEASON_TBL_ACTIVE_PASSIVE_SW_TIME,(RAM)+(INT32U)&stTrPara.SeasonTblSwTime,sizeof(ST_5TIME_FORM),0x16},
  {E_PROG_DAY_TBL,(RAM)+(INT32U)&DCDataBuf[0],42,0x17},
  {E_PROG_SEASON_TBL,(RAM)+(INT32U)&DCDataBuf[0],42,0x25},
  {E_DAY_TBL_ACTIVE_PASSIVE_SW_TIME,(RAM)+(INT32U)&stTrPara.dayTblSwTime,sizeof(ST_5TIME_FORM),0x16},
  {E_SEASON_SLOT_NUM,(RAM)+(INT32U)&stTrPara.seasonSlotNum,1,0x01},
  {E_DAY_TBL_NUM,(RAM)+(INT32U)&stTrPara.dayTblNum,1,0x01},
  {E_DAY_SLOT_NUM,(RAM)+(INT32U)&stTrPara.daySlotNum,1,0x01},
  {E_TARIFF_NUM,(RAM)+(INT32U)&stTrPara.tariffNum,1,0x01},
  {E_SPECIAL_DAY_NUM,(RAM)+(INT32U)&stTrPara.specDayNum,1,0x01},
  {E_PROG_HOLIDAY,(RAM)+(INT32U)&DCDataBuf[0],6,0x15},
  {E_WEEKEND_TYPE,(RAM)+(INT32U)&stTrPara.weekendType,1,0x01},
  {E_WEEKEND_DAY_TBL_ID,(RAM)+(INT32U)&stTrPara.weekendDayTblId,1,0x01},
 };
const ST_ACTION_ATTR stTrActionList[] = 
 {  
     {E_CHECK_SWITCH_ACTION,TR_CheckSwitch},
     {E_FIND_NOW_TRIFF_ACTION,TR_FindNowTriff},
     {E_SYSTIME_UPDATE_ACTION,TR_SysTimeUpdate },
 };