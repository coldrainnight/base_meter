/*"*************************************************************"*/
/*"FileName: UI_data.c"*/
/*"Author:    		Version :  "*/
/*"2019-08-12 18:56:03"*/
/*"Description: 用户接口模块对象列表定义"*/
/*"总字节数: 449          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\UserInterface\UI.h"
#include "UI_data.h"
const ST_BLK_DATA_ATTR stUiBlkDataList[] = 
 {////  
  {E_SAFE_MODE_LIST,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara4Hex.SafeMode,4,MAX_OI_NUM_SAFE_MODE,0x05},
  {E_NO_BACKUP_BEGIN_BLOCK,(RAM)+(INT32U)0,0,0,0x01},
  {E_CYCLE_DIS_OBJ_LIST,(E_FILE_UI_PARA_NO_BACKUP <<16)+CYC_OBJ_LIST_SHIFT,sizeof(ST_DISP_PARA_OBJ),CYC_DISP_MAX,0x0F},
  {E_KEY_DIS_OBJ_LIST,(E_FILE_UI_PARA_NO_BACKUP <<16)+KEY_OBJ_LIST_SHIFT,sizeof(ST_DISP_PARA_OBJ),KEY_DISP_MAX,0x0F},
 };
const ST_SINGLE_DATA_ATTR stUiSingleDataList[] = 
 {  
  {E_PWRUP_DIS_ALL_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.pwrUpFullShowT,1,0x01},
  {E_BACKLIGHT_SHOW_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.BacklightTime,2,0x0C},
  {E_BACKLIGHT_SHOW_TIME_ON_VIEW,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.DisChecklightTime,2,0x0C},
  {E_PD_NOKEY_RETURN_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.LPKeyDisTime,2,0x0C},
  {E_PWR_SHOW_POINT,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.pwrDmdPnt,1,0x01},
  {E_ENGY_SHOW_POINT,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.engyPnt,1,0x01},
  {E_DIS_SEG_MODE_CTRL_12,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.Lcd12Func,1,0x01},
  {E_NOKEY_RETURN_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.keyRetTime,2,0x0C},
  {E_CYCLE_SHOW_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.cycleShowTime,2,0x0C},
  {E_CYCLE_SHOW_NUM,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.cycleShowNum,1,0x01},
  {E_KEY_SHOW_NUM,(RAM)+(INT32U)&StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara.keyShowNum,1,0x01},
  {E_RS485_1_BAUDRATE,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.BaudRate[E_COM_RS485_1],1,0x01},
  {E_RPTRST_TIME,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.evtRptRstTime,1,0x01},
  {E_MTR_RUN_MODE,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.mtrRunMode,1,0x01},
  {E_EXT_MODE1,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[0],1,0x01},
  {E_EXT_MODE2,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[1],1,0x01},
  {E_EXT_MODE3,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[2],1,0x01},
  {E_EXT_MODE4,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[3],1,0x01},
  {E_EXT_MODE5,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[4],1,0x01},
  {E_EXT_MODE6,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[5],1,0x01},
  {E_EXT_MODE7,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[6],1,0x01},
  {E_EXT_MODE8,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[7],1,0x01},
  {E_EXT_MODE9,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[8],1,0x01},
  {E_EXT_MODE10,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara1Hex.ExMode[9],1,0x01},
  {E_COMM_ADDR,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara6Hex.CommAddr,6,0x11},
  {E_GROUP_ADDR,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara6Hex.GroupAddr,6,0x11},
  {E_AUTHEN_PASSWORD,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara8Hex.AuthenPassword,8,0x0D},
  {E_645REPORT_MODE,(RAM)+(INT32U)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara8Hex.Evt645RptMode,1,0x0D},
  {E_NO_BACKUP_BEGIN,(RAM)+(INT32U)0,1,0x01},
  {E_REGU_VOLT,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP_6HEX,VoltRegu),6,0x18},
  {E_REGU_CURR,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP_6HEX,CurrRegu),6,0x18},
  {E_MAX_CURR,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP_6HEX,MaxCurr),6,0x18},
  {E_SOFT_VER_DATE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP_6HEX,SoftVerDate),6,0x18},
  {E_HARD_VER_DATE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_6HEX,hardVerDate),6,0x18},
  {E_ACT_ACCU_GRADE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,ActAccuGrade),4,0x1B},
  {E_FAC_NUM,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,FacNum),4,0x1B},
  {E_SOFTWARE_VER,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,SoftwareVers),4,0x1B},
  {E_HARDWARE_VER,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,HardwareVers),4,0x1B},
  {E_LONGITUDE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,longitude),4,0x1B},
  {E_LATITUDE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,latitude),4,0x1B},
  {E_HEIGHT,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_4HEX,height),4,0x1B},
  {E_SOFT_RECORD_ID,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_16HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_16HEX,SoftRecordID),8,0x1D},
  {E_MANUFACTOR_EXT_INFO,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_8HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_8HEX,ManufactorExtInfo),8,0x1F},
  {E_SUP_PROFILE_LIST,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_32HEX,SupProfileList),32,0x1E},
  {E_PROTOCOL_VER,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_2HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_2HEX,ProtocolVers),16,0x0C},
  {E_METER_TYPE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_32HEX,MeterType),10,0x1E},
  {E_ASSET_NUM,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_32HEX,AssetNum),32,0x1E},
  {E_PRODUCT_DATE,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_7HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_7HEX,ProductDate),7,0x26},
  {E_PRO_CONSISTENCY,(RAM)+(INT32U)&ConnectResInfo[0],8,0x1F},
  {E_FUN_CONSISTENCY,(RAM)+(INT32U)&ConnectResInfo[8],16,0x1D},
  {E_PP_OAD,(RAM)+(INT32U)&CurOad,sizeof(ST_OAD),0x05},
  {E_PP_Program_OAD,(RAM)+(INT32U)&Program_Oad,sizeof(ST_OAD)*10,0x27},
  {E_CUR_CHANNL,(RAM)+(INT32U)&ComPort,1,0x01},
  {E_RUN_STA_1,(RAM)+(INT32U)&RunStatus.word1.word,2,0x0C},
  {E_RUN_STA_2,(RAM)+(INT32U)&RunStatus.word2.word,2,0x0C},
  {E_RUN_STA_3,(RAM)+(INT32U)&RunStatus.word3.word,2,0x0C},
  {E_RUN_STA_4,(RAM)+(INT32U)&RunStatus.word4.word,2,0x0C},
  {E_RUN_STA_5,(RAM)+(INT32U)&RunStatus.word5.word,2,0x0C},
  {E_RUN_STA_6,(RAM)+(INT32U)&RunStatus.word6.word,2,0x0C},
  {E_RUN_STA_7,(RAM)+(INT32U)&RunStatus.word7.word,2,0x0C},
  {E_METER_SAMPLE_STATUS,(RAM)+(INT32U)&MeterSampleStatus.word,2,0x0C},
  {E_SAMPLE_STATUS,(RAM)+(INT32U)&SampleStatus.word,2,0x0C},
  {E_CLK_BAT_TIME,(E_FILE_UI_DATA <<16)+FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin),4,0x05},
  {E_RELAY_UNIT_POWER,(RAM)+(INT32U)&RunStatus.word7.word,4,0x05},
  {E_SOFT_VER_32,(E_FILE_UI_PARA_NO_BACKUP <<16)+CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT+ FIND(ST_CM_PARA_NO_RAM_BACKUP_32HEX,SoftVer32),32,0x1E},
 };
const ST_ACTION_ATTR stUiActionList[] = 
 {  
     {E_UIDATA_CLR,UI_DataClr},
     {E_SHOW_SCREEN_APPOINT,DM_ShowSpec},
     {E_SHOW_FLASH,DM_ShowFlash},
     {E_SHOW_SCREEN_ALL,DM_ShowAll},
     {E_SHOW_SCREEN_NEXT,DM_ShowNext},
     {E_SHOW_SCREEN_LAST,DM_ShowLast},
     {E_AUTHALLOW_KEEP,TCA_AuthAllow_KeepS},
     {E_PRGEVENT_RCRD,PrgEvent_Rcrd},
     {E_SHOW_SCREEN_DLT698,DM_DLT698ShowSpec},
 };