/*"*****************************************************************************"*/
/*"  FileName: class21_esam.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ESAM接口类文件   "*/
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

#ifdef PM_MODLE


/*21类 ESAM类，也只有一个OI=F100*/
/*21类属性列表*/
const ST_SPECIAL_CLASS_AID_INFO CLASS21_AID_LIST[8]=
{
    /*1.逻辑名*/
    {2, READ_ONLY,  0x01,TYPE_OCTS_8},        /*2.ESAM序列号*/
    {3, READ_ONLY,  0x01,TYPE_OCTS_5},        /*3.ESAM版本号*/        
    {4, READ_ONLY,  0x01,TYPE_OCTS_16},       /*4.对称密钥版本*/
    {5, READ_ONLY,  0x01,TYPE_U32},           /*5．会话时效门限*/
    {6, READ_WRITE, 0x01,TYPE_U32},           /*6．会话时效剩余时间*/
    {7, READ_WRITE, 0x03,ST_3_U32},           /*7．当前计数器*/
    {14,READ_WRITE, 0x01,TYPE_U32},           /*14. 红外认证时效门限*/
    {15,READ_WRITE, 0x01,TYPE_U32}            /*15. 红外认证剩余时间*/
};

/*21类OBIS列表*/
const ST_CLASS21_OI_AID_OBIS CLASS21_OI_AID_OBIS[1] = 
{
    {0xF100,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_IRINEDT_TIME ,E_IRAUTH_RMN_TIME} /*ESAM类*/  
};

/*已特殊处理了，没有用了*/
INT8U get_class21_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  seq_num;
    
   seq_num = pObject->seq_num;
   
   pObisInfo_bak->Obis = *((INT16U *)&CLASS21_OI_AID_OBIS[0] + seq_num+1); 
   pObisInfo_bak->Offset = 0;
   pObisInfo_bak->num  = 1;
   pObisInfo_bak->FormatId = MAX_FORMAT_ID;

   /*显示格式，看是否要特殊定义*/

   return 1;
      
}
#endif

/*"************************************************"*/


