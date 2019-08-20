/*"*************************************************************"*/
/*"FileName: LRM_data.c"*/
/*"Author:    		Version :  "*/
/*"2019-08-16 13:53:17"*/
/*"Description: 法定计量模块对象列表定义"*/
/*"总字节数: 231          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\LegallyRelevant\LRM.h"
#include "App\LegallyRelevant\SC.h"
#include "LRM_data.h"
const ST_BLK_DATA_ATTR stLrmBlkDataList[] = 
 {////  
  {E_PA_ENGY_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].ComP[0],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x0E},
  {E_NA_ENGY_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].ComP[1],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x0E},
  {E_ACTIVE_PWR_BLK,(RAM)+(INT32U)&stMmtData.actPwr,4,(MU_ELEMENT_NUM + 1),0x09},
  {E_VOLT_BLK,(RAM)+(INT32U)&stMmtData.volt,4,MU_ELEMENT_NUM ,0x06},
  {E_CURR_BLK,(RAM)+(INT32U)&stMmtData.curr,4,MU_ELEMENT_NUM ,0x00},
  {E_FACTOR_BLK,(RAM)+(INT32U)&stMmtData.factor,4,(MU_ELEMENT_NUM + 1),0x0A},
  {E_PHASE_BLK,(RAM)+(INT32U)&stMmtData.VoltCurrAngle[0],4,MU_ELEMENT_NUM,0x0A},
  {E_A_ENGY_COM_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].ComP[0],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x23},
  {E_R_ENGY_I_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].Q[0],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x34},
  {E_R_ENGY_II_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].Q[1],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x34},
  {E_R_ENGY_III_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].Q[2],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x34},
  {E_R_ENGY_IV_BLK,(RAM)+(INT32U)&stNowEngyBak.TariffEngy[0].Q[3],sizeof(ST_ENERGY_FORM),(TARIFF_NUM + 1),0x34},
  {E_REACTIVE_PWR_BLK,(RAM)+(INT32U)&stMmtData.reactPwr,4,(MU_ELEMENT_NUM + 1),0x3A},
  {E_A_VOLT_CONT_BLK,(RAM)+(INT32U)&stMmtData.HRVolt[0][0],2,MU_HARMONIC_NUM,0x3E},
  {E_A_CURR_CONT_BLK,(RAM)+(INT32U)&stMmtData.HRCurr[0][0],2,MU_HARMONIC_NUM,0x3E},
 };
const ST_SINGLE_DATA_ATTR stLrmSingleDataList[] = 
 {  
  {E_FREQ,(RAM)+(INT32U)&stMmtData.freq,4,0x02},
  {E_SYS_TIME,(RAM)+(INT32U)&stSysTime,sizeof(ST_ENERGY_FORM),0x08},
  {E_PD_TIME,(RAM)+(INT32U)&stPdSaveData.pdTime,sizeof(ST_6TIME_FORM),0x0B},
  {E_A_PULSE_CONST,(RAM)+(INT32U)&gMU_Para.ram_adj_data.Ram_ecconst,2,0x0C},
  {E_CURR_LN,(RAM)+(INT32U)&stMmtData.calNcurr,4,0x00},
  {E_CURR_RESIDUAL,(RAM)+(INT32U)&stMmtData.residualCurr,4,0x00},
  {E_PULSE_WIDTH,(RAM)+(INT32U)&stLrmPara.PulseWidth,1,0x01},
  {E_A_ENGY_COM_TYPE,(RAM)+(INT32U)&stLrmPara.AEngyComType,1,0x01},
  {E_PWR_DIR,(RAM)+(INT32U)&stMmtData.PowerDir,1,0x01},
  {E_LRM_TMP_LINE_IN,(RAM)+(INT32U)&stMmtData.gTmnlTmp[E_TEMP_LIN],4,0x39},
  {E_LRM_TMP_LINE_OUT,(RAM)+(INT32U)&stMmtData.gTmnlTmp[E_TEMP_LOUT],4,0x39},
  {E_LRM_TMP_N_IN,(RAM)+(INT32U)&stMmtData.gTmnlTmp[E_TEMP_NIN],4,0x39},
  {E_LRM_TMP_N_OUT,(RAM)+(INT32U)&stMmtData.gTmnlTmp[E_TEMP_NOUT],4,0x39},
  {E_AMP_ERR,(RAM)+(INT32U)&gEDT_Errdata.P_AMP_Err,4,0x3D},
  {E_A_VOLT_AVAILAB,(RAM)+(INT32U)&stMmtData.HRUrms[0],4,0x40},
  {E_A_CURR_AVAILAB,(RAM)+(INT32U)&stMmtData.HRIrms[0],4,0x20},
 };
const ST_ACTION_ATTR stLrmActionList[] = 
 {  
     {E_LRM_CLR,LRM_Clr},
     {E_LRM_JUSTMETER,LRM_JustMtr},
     {E_LRM_JUSTIME,LRM_JustTime},
     {E_LRM_PWR_DOWN,LRM_PwrDown},
     {E_LRM_PCTR_INIT,LRM_PulseCtrInit},
 };