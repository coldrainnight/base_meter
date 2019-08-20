/*"*************************************************************"*/
/*"FileName: PM_data.c"*/
/*"Author:    		Version :  "*/
/*"2018-08-06 16:28:46"*/
/*"Description: 预付费模块对象列表定义"*/
/*"总字节数: 6          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
const ST_BLK_DATA_ATTR stPmBlkDataList[] = 
 {////  
 };
const ST_SINGLE_DATA_ATTR stPmSingleDataList[] = 
 {  
  {PM_NULL,(0 <<16)+0,0,0x01},
 };
const ST_ACTION_ATTR stPmActionList[] = 
 {  
 };