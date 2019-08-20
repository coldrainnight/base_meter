/*"***********************************************************"
/*"FileName: DU_data.c"				2222
/*"Author:    					Version :"
/*"Date: 2017-11-16 09:11:36"
/*"Description: 需量模块对象列表定义"
/*"总字节数:          "
/*"Function List:                     "
/*"                                    "
/*"History:                           "
/*"<author> <time> <version > <desc>  "
/*"***********************************************************"*/

const ST_BLK_DATA_ATTR stDuBlkDataList[] = 
 {////  
     E_PA_MAX_DEMAND_BLK,EEPROM+(FS_DEMAND + PA_MAX_DEMAND_SHIFT),4,(TARIFF_NUM + 1),0x04,
     E_NA_MAX_DEMAND_BLK,EEPROM+(FS_DEMAND + NA_MAX_DEMAND_SHIFT),4,(TARIFF_NUM + 1),0x04,
}

const ST_SINGLE_DATA_ATTR stDuSingleDataList[] = 
 {  
     E_DEMAND_PERIOD,RAM+&stDemandPara.period,1,0x02,
     E_DEMAND_SLID_PERIOD,RAM+&stDemandPara.slid,1,0x02,
}

const ST_ACTION_ATTR stDuActionList[] = 
 {  
     E_DU_CLR,DU_Clr,
     E_MAX_DEMAND_CLR,DU_MaxDemandClr,
}
