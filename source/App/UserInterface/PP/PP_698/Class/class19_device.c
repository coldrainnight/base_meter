/*"*****************************************************************************"*/
/*"  FileName: class19_device.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  设备管理类文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h" 
/*"*******************************************************************************"*/

/*19类属性列表*/
const ST_SPECIAL_CLASS_AID_INFO CLASS19_AID_LIST[9]=
{
    /*,逻辑名*/
    {2, READ_ONLY,    0x01,TYPE_VIS_32},         /*设备描述符*/
    {3, READ_ONLY,    0x06,TYPE_VERSION_INFO},   /*版本信息*/        
    {4, READ_ONLY,    0x02,TYPE_DATES},          /*生产日期*/
    {5, READ_ONLY,    0xFF,AR_OI},               /*子设备列表*/
    {6, READ_ONLY,    0xFF,AR_VIS},              /*支持规约列表*/
    {7, READ_WRITE,   0x01,TYPE_BOOL},           /*允许跟随上报*/
    {8, READ_WRITE,   0x01,TYPE_BOOL},           /*允许主动上报*/
    {9, NO_READ_WRITE,0x01,TYPE_BOOL},           /*允许与主站通话*/
    {10,READ_WRITE,   0x03,AR_OAD}               /*上报通道*/
};

/*19类OBIS列表*/
const ST_CLASS19_OI_AID_OBIS CLASS19_OI_AID_OBIS[] = 
{
    {0x4300, /*电气设备*/     
     E_METER_TYPE,          /*设备描述符*/
     E_FAC_NUM,             /*版本信息  在getobis时,把obis num置为6*/      
     E_PRODUCT_DATE,        /*生产日期*/
     E_OBIS_NULL,           /*子设备列表*/
     E_SUP_PROFILE_LIST,    /*支持规约列表*/
     E_OBIS_NULL,        /*允许跟随上报*/
     E_OBIS_NULL,          /*允许主动上报*/
     E_OBIS_NULL,           /*允许与主站通话*/
     E_OBIS_NULL,           /*上报通道*/
    } 
};

/*版本信息的6个OBIS，修改为这样定义，因为可能是不连续的*/ 
const INT16U OBIS_VERSION[]=
{E_FAC_NUM,E_SOFTWARE_VER,E_SOFT_VER_DATE,
 E_HARDWARE_VER,E_HARD_VER_DATE,E_MANUFACTOR_EXT_INFO}; 

INT8U get_class19_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  AID,i;
   ST_PARAMENT_OAD_OBIS_INFO *ptmpObisInfo_bak;

   AID    = pObject->Oad.AId;   
  
   pObisInfo_bak->Offset = 0;

   if(EN_CLS19_ATT_3_VERSION_INFO == AID)
   {
      ptmpObisInfo_bak = pObisInfo_bak;
      for(i=0;i<ARRAY_SIZE(OBIS_VERSION);i++)
      {
         ptmpObisInfo_bak->num  = 1;   /*版本信息有6个连续的OBIS*/
         ptmpObisInfo_bak->Obis = OBIS_VERSION[i];
         ptmpObisInfo_bak++;
      }  
      
      return 6;
   }
   else
   { 
      pObisInfo_bak->Obis = *((INT16U *)&CLASS19_OI_AID_OBIS[0].OBIS_ATT2 + AID-2); 
      pObisInfo_bak->num = 1;
   }
   
   /*显示格式，看是否要特殊定义*/

   return 1;
      
}
/*"************************************************"*/
