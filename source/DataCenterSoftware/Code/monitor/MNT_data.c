/*"*************************************************************"*/
/*"FileName: MNT_data.c"*/
/*"Author:    		Version :  "*/
/*"2019-08-21 11:03:30"*/
/*"Description: 电能表状态监测模块对象列表定义"*/
/*"总字节数: 187          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\monitor\MNT.h"
#include "MNT_data.h"
const ST_BLK_DATA_ATTR stMntBlkDataList[] = 
 {////  
  {E_EVNT_DURATION_BLK,(RAM)+(INT32U)&EvntTimeAdd[0],4,2,0x05},
  {E_MIN_ACTIVEPOWER_BLK,(RAM)+(INT32U)&minActPower[0],4,2,0x09},
 };
const ST_SINGLE_DATA_ATTR stMntSingleDataList[] = 
 {  
  {E_OVERCURR_IL_THR,(RAM)+(INT32U)&MntPara.OverCurrLimitIL,4,0x05},
  {E_OVERCURR_TIME_THR,(RAM)+(INT32U)&MntPara.OverCurrDelayT,1,0x01},
  {E_MNT_EVNT_TIME_S,(RAM)+(INT32U)&DCDataBuf[0],6,0x0B},
  {E_MNT_EVNT_TIME_E,(RAM)+(INT32U)&DCDataBuf[6],6,0x0B},
  {E_RLY_TRIP_CTRL_CURR_THR,(RAM)+(INT32U)&MntPara.RlyTripCtrlCurr,4,0x20},
  {E_FAC_STATUS,(RAM)+(INT32U)&FacStatusFlg,1,0x01},
  {E_CLK_BAT_VOLT,(RAM)+(INT32U)&BatVolt[0],2,0x28},
  {E_RLY_STATUS_NOW,(RAM)+(INT32U)&RelayStatusNow,1,0x01},
  {E_EVT_LCDALARM_MODE,(RAM)+(INT32U)&MntPara.mntAlarmMode[EA_LCD],4,0x05},
  {E_EVT_BGTALARM_MODE,(RAM)+(INT32U)&MntPara.mntAlarmMode[EA_BGT],4,0x05},
  {E_MUERR_TIME_THR,(RAM)+(INT32U)&MntPara.MuErrDelayT,1,0x2C},
  {E_CLKERR_TIME_THR,(RAM)+(INT32U)&MntPara.ClkErrDelayT,1,0x2C},
  {E_RST_INFO,(RAM)+(INT32U)&mucRstInfo,1,0x01},
  {E_RST_ADDR,(RAM)+(INT32U)&err_code_addr,4,0x05},
  {E_UPDATA,(RAM)+(INT32U)&mMntUpData,sizeof(MNT_UP_DATA),0x01},
  {E_TMP_LINE_IN,(RAM)+(INT32U)&gTmnlTmp[E_TEMP_LIN],2,0x38},
  {E_TMP_LINE_OUT,(RAM)+(INT32U)&gTmnlTmp[E_TEMP_LOUT],2,0x38},
  {E_TMP_N_IN,(RAM)+(INT32U)&gTmnlTmp[E_TEMP_NIN],2,0x38},
  {E_TMP_N_OUT,(RAM)+(INT32U)&gTmnlTmp[E_TEMP_NOUT],2,0x38},
  {E_MNT_ESAM_ID,(RAM)+(INT32U)&gGlxChgEsamID[0],0,0x0D},
  {E_MNT_MTR_TMP,(RAM)+(INT32U)&gMtrTmp,2,0x38},
  {E_MNT_MTR_HMDTY,(RAM)+(INT32U)&gMtrHmdty,2,0x3C},
  {E_MNT_AMP_ERR,(RAM)+(INT32U)&mPwrAmpErr,4,0x3D},
  {E_ACCRCY_RFRSHT,(RAM)+(INT32U)&MntPara.AccrcyChkRfrshT,1,0x01},
 };
const ST_ACTION_ATTR stMntActionList[] = 
 {  
     {E_MNT_EVNT_CLR,MNT_EvntClr},
     {E_MNT_PWR_DOWN,MNT_PwrDown},
     {E_OUT_FAC,MNT_OutFacCmd},
     {E_FIRST_PWR_UP_OVER,MNT_FirstPwrUpOver},
     {E_RESET_FAC_REMAIN_TIME,MNT_ReSetFacRemainTime},
     {E_PWRUP_EVTINIT,MNT_PwrUpEvtInit},
     {E_EVTPARACAL,MNT_EvtParaCal},
     {E_UPDATA_SET,MNT_UpDataSet},
     {E_DOWNDATA_GET,MNT_DownDataGet},
 };