/*"*************************************************************"*/
/*"FileName: AUTO_data.c"*/
/*"Author:    		Version :  "*/
/*"2018-05-05 10:04:30"*/
/*"Description: 自动化生产模块对象列表定义"*/
/*"总字节数: 32          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\AutomaticProduction\AUTO.h"
#include "AUTO_data.h"
const ST_BLK_DATA_ATTR stAutoBlkDataList[] = 
 {////  
  {E_PRODUCT_NOUSED,(E_FILE_AUTO_DATA <<16)+PRODUCTID1_SHIFT,32,1,0x1E},
 };
const ST_SINGLE_DATA_ATTR stAutoSingleDataList[] = 
 {  
  {E_PRODUCT_ID1,(E_FILE_AUTO_DATA <<16)+PRODUCTID1_SHIFT,32,0x1E},
  {E_PRODUCT_ID2,(E_FILE_AUTO_DATA <<16)+PRODUCTID2_SHIFT,32,0x1E},
  {E_PRODUCT_ID3,(E_FILE_AUTO_DATA <<16)+PRODUCTID3_SHIFT,32,0x1E},
 };
const ST_ACTION_ATTR stAutoActionList[] = 
 {  
     {E_AUTO_READEXTDATA,AUTO_ReadExtData},
     {E_AUTO_WRTEXTDATA,AUTO_WriteExtData},
 };