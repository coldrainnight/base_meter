/*"*****************************************************************************"*/
/*"  FileName: class9_freeze.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  冻结类文件   "*/
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

const ST_CLASS9_FREEZE_OI_OBIS CLASS_9_OI_OBIS[]=
{
  {0x5000,E_MMT_FRZ_RCRD},          /*瞬时冻结*/
  {0x5002,E_MIN_FRZ_RCRD },         /*分钟冻结*/
  {0x5003,E_HOUR_FRZ_RCRD},         /*小时冻结*/
  {0x5004,E_DAY_FRZ_RCRD},          /*日冻结*/
  {0x5005,E_MONTH_BILL_FRZ_RCRD},   /*结算日冻结*/
  {0x5006,E_MONTH_FRZ_RCRD},        /*月冻结*/
 };

/* 关联对象列表指针 ST_3_U16_OAD_U16 */

/*9类冻结元素查找*/
INT8U get_class9_freeze_object(SUB_OBJECT *pObject)
{
    INT8U  i,AID;
    INT16U OI;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    //IID = pObject->Oad.IId;

    /*AID最大为3,2为冻结记录表，具体在GET时，特殊判断，不能用GET方法读取*/
    if(EN_CLS9_ATT_3_RELATION_OBJECT_TABLE<AID)
    {
        return DAR_4_OANODEF;
    }    
        
    for(i = 0; i<ARRAY_SIZE(CLASS_9_OI_OBIS); i++)
    {
        if(OI == CLASS_9_OI_OBIS[i].OI)
        {
            pObject->Class  = 9;
            pObject->seq_num= i;
            pObject->element= i;
            pObject->Right_RW = READ_WRITE;    
            pObject->Pointer = (INT8U*)AR_ST_3_U16_OAD_U16;   /*冻结的关联对象类型指针是相同的*/
                
            return DAR_0_OK;
        }    
    }

    return DAR_4_OANODEF;    
}


INT8U get_class9_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
     pObisInfo_bak->Obis = CLASS_9_OI_OBIS[pObject->seq_num].OBIS;

     pObisInfo_bak->num = 1;
     pObisInfo_bak->FormatId = MAX_FORMAT_ID;
     //pObisInfo_bak->ShowinfoId
     return 1;
}    


/*"************************************************"*/

