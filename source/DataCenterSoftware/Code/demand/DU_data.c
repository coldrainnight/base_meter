/*"*************************************************************"*/
/*"FileName: DU_data.c"*/
/*"Author:    		Version :  "*/
/*"2018-12-26 17:37:57"*/
/*"Description: 需量模块对象列表定义"*/
/*"总字节数: 34          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\demand\DU.h"
#include "DU_data.h"
const ST_BLK_DATA_ATTR stDuBlkDataList[] = 
 {////  
  {E_PA_MAX_DEMAND_BLK,(EEPROM <<16)+(FS_DEMAND + PA_MAX_DEMAND_SHIFT),4,(TARIFF_NUM + 1),0x03},
  {E_NA_MAX_DEMAND_BLK,(EEPROM <<16)+(FS_DEMAND + NA_MAX_DEMAND_SHIFT),4,(TARIFF_NUM + 1),0x03},
 };
const ST_SINGLE_DATA_ATTR stDuSingleDataList[] = 
 {  
  {E_DEMAND_PERIOD,(RAM)+(INT32U)&stDemandPara.period,1,0x01},
  {E_DEMAND_SLID_PERIOD,(RAM)+(INT32U)&stDemandPara.slid,1,0x01},
 };
const ST_ACTION_ATTR stDuActionList[] = 
 {  
     {E_DU_CLR,DU_Clr},
     {E_MAX_DEMAND_CLR,DU_MaxDemandClr},
 };