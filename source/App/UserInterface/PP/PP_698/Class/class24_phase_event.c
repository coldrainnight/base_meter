/*"*****************************************************************************"*/
/*"  FileName: class24_phase_event.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  分相事件接口类文件   "*/
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
#include "App\Message\MS.h"
/*"************************************************"*/
/*24类属性列表*/
const ST_CLASS24_AID_INFO CLASS24_AID_LIST[]=
{
    /*,逻辑名*/
    {2, READ_WRITE,  0xff,AR_OAD},       /*关联对象属性表*/
    {3, READ_ONLY,   0x04,ST_4_U16},     /*当前记录数*/        
    {4, READ_ONLY,   0x01,TYPE_U16},     /*最大记录数*/
    /*5,配置参数     单独配置列表*/
    /*6,事件记录表1*/
    /*7,事件记录表2*/
    /*8,事件记录表3*/
    /*9,事件记录表4*/
    {10, READ_ONLY,  0x04,AR_ST_2_U32},   /*当前值记录表*/
    {11, READ_WRITE, 0x01,TYPE_ENUM},     /*上报标识*/
    {12, READ_ONLY,  0x01,TYPE_BOOL},     /*有效标识*/
    
    #ifdef THREE_PHASE
    {13, READ_ONLY,  0x04,ST_4_U32_U32_DATE_S_DATE_S},  /*失压统计*/
    #endif
    
    {14, READ_ONLY,  0xff,AR_ST_2_DATES}, /*时间状态记录表*/
};

/*对应上面的属性的OBIS列表，之所以与上面的分开写，主要是考虑到可能字节对齐*/
const ST_CLASS24_OBIS_INFO OBIS_BASE_CLASS24_EVENT[]=
{
    //{2,MAX_FORMAT_ID, FUN_NULL, 0x00,E_OVER_CURR_LA_EVNT_RCRD},      /*关联对象属性表*/
    {3,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_CNT},                /*当前记录数*/        
    {4,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_MAX_CNT},                /*最大记录数*/
    /*5,配置参数     单独配置列表*/
    /*6,事件记录表1*/
    /*7,事件记录表2*/
    /*8,事件记录表3*/
    /*9,事件记录表4*/
    {10,FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_VALUE},   /*当前值记录表*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},     /*上报标识*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},     /*有效标识*/
    
    {14,FORMAT_DATETIME, FUN_NULL, E_INFO_NULL,E_RCRD_TIME_STATUS}, /*时间状态记录表*/
};   

#ifdef THREE_PHASE
const ST_CLASS24_PHASE_EVENT_OI_OBIS CLASS_24_OI_OBIS[]=
{
    {0x3000, E_OBIS_NULL,E_LOST_VOLT_LA_EVNT_RCRD,E_LOST_VOLT_LB_EVNT_RCRD,E_LOST_VOLT_LC_EVNT_RCRD}, /*失压事件*/  
    {0x3001, E_OBIS_NULL,E_LACK_VOLT_LA_EVNT_RCRD,E_LACK_VOLT_LB_EVNT_RCRD,E_LACK_VOLT_LC_EVNT_RCRD}, /*欠压事件*/ 
    {0x3002, E_OBIS_NULL,E_OVER_VOLT_LA_EVNT_RCRD,E_OVER_VOLT_LB_EVNT_RCRD,E_OVER_VOLT_LC_EVNT_RCRD}, /*过压事件*/ 
    {0x3003, E_OBIS_NULL,E_BRK_PHASE_LA_EVNT_RCRD,E_BRK_PHASE_LB_EVNT_RCRD,E_BRK_PHASE_LC_EVNT_RCRD}, /*断相事件*/ 
    {0x3004, E_OBIS_NULL,E_LOST_CURR_LA_EVNT_RCRD,E_LOST_CURR_LB_EVNT_RCRD,E_LOST_CURR_LC_EVNT_RCRD}, /*失流事件*/ 
    {0x3005, E_OBIS_NULL,E_OVER_CURR_LA_EVNT_RCRD,E_OVER_CURR_LB_EVNT_RCRD,E_OVER_CURR_LC_EVNT_RCRD}, /*过流事件*/ 
    {0x3006, E_OBIS_NULL,E_BRK_CURR_LA_EVNT_RCRD,E_BRK_CURR_LB_EVNT_RCRD,E_BRK_CURR_LC_EVNT_RCRD},    /*断流事件*/ 
    {0x3008, E_OBIS_NULL,E_OVER_LOAD_LA_EVNT_RCRD,E_OVER_LOAD_LB_EVNT_RCRD,E_OVER_LOAD_LC_EVNT_RCRD}, /*功率反向事件*/ 
    /*--下面两个事件有4个OBIS--*/
    {0x3007, E_PWR_REVERSE_EVNT_RCRD,E_PWR_REVERSE_LA_EVNT_RCRD,E_PWR_REVERSE_LB_EVNT_RCRD,E_PWR_REVERSE_LC_EVNT_RCRD}, /*过载事件*/ 
    {0x300B, E_OVER_R_DEMAND_1_EVNT_RCRD,E_OVER_R_DEMAND_2_EVNT_RCRD,E_OVER_R_DEMAND_3_EVNT_RCRD,E_OVER_R_DEMAND_4_EVNT_RCRD} /*电能表无功需量超限事件*/ 
};

/*注:这个表和上面的表一一对应，事件参数:AID=5*/
const ST_Class24_PARAMENT_ObjectList CLASS24_PARAMENT_LIST[]=
{
   {4,READ_WRITE, ST_4_U16_U16_S32_U8},   /*3000 失压事件*/  
   {2,READ_WRITE, ST_2_U16_U8},           /*3001 欠压事件*/ 
   {2,READ_WRITE, ST_2_U16_U8},           /*3002 过压事件*/ 
   {3,READ_WRITE, ST_3_U16_S32_U8},       /*3003 断相事件*/ 
   {4,READ_WRITE, ST_4_U16_S32_S32_U8},   /*3004 失流事件*/ 
   {2,READ_WRITE, ST_2_S32_U8},           /*3005 过流事件*/ 
   {3,READ_WRITE, ST_3_U16_S32_U8},       /*3006 断流事件*/     
   {2,READ_WRITE, ST_2_S32_U8},           /*3007 过载事件*/ 
   {2,READ_WRITE, ST_2_S32_U8},           /*3008 功率反向事件*/ 
   {2,READ_WRITE, ST_2_U32_U8}            /*300B电能表无功需量超限事件*/ 
};  

/*24类参数OBIS对应表 AID=5   下面的Seq_Num = OI -0x3000*/
const ST_PARAMENT_OAD_OBIS_INFO CLASS24_PARAMENT_OBIS_LIST[]=
{    
     /*3000 失压事件*/  
    {0, 0x01,E_LOSTVOLT_UU_THR/*"失压事件电压触发上限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {0, 0x01,E_LOSTVOLT_UL_THR/*"失压事件电压恢复下限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},      
    {0, 0x01,E_LOSTVOLT_IL_THR/*"失压事件电流触发下限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {0, 0x01,E_LOSTVOLT_TIME_THR/*"失压事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3001 欠压事件*/ 
    {1, 0x01,E_UNDERVOLT_UU_THR/*"欠压事件电压触发上限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {1, 0x01,E_UNDERVOLT_TIME_THR/*"欠压事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3002 过压事件*/ 
    {2, 0x01,E_OVERVOLT_UL_THR/*"过压事件电压触发下限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {2, 0x01,E_OVERVOLT_TIME_THR/*"过压事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3003 断相事件*/ 
    {3, 0x01,E_BRKPHASE_UU_THR/*"断相事件电压触发上限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {3, 0x01,E_BRKPHASE_IU_THR/*"断相事件电流触发上限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},       
    {3, 0x01,E_BRKPHASE_TIME_THR/*"断相事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3004 失流事件*/ 
    {4, 0x01,E_LOSTCURR_UL_THR/*"失流事件电压触发下限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {4, 0x01,E_LOSTCURR_IU_THR/*"失流事件电流触发上限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {4, 0x01,E_LOSTCURR_IL_THR/*"失流事件电流触发下限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {4, 0x01,E_LOSTCURR_TIME_THR/*"失流事件判定延时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     

    /*3005 过流事件*/ 
    {5, 0x01,E_OVERCURR_IL_THR/*"过流事件电流触发下限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},       
    {5, 0x01,E_OVERCURR_TIME_THR/*"过流事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3006 断流事件*/   
    {6, 0x01,E_FAILCURR_UL_THR/*"断流事件电压触发下限"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {6, 0x01,E_FAILCURR_IU_THR/*"断流事件电流触发上限"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},         
    {6, 0x01,E_FAILCURR_TIME_THR/*"断流事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3007 功率反向事件*/
    {7, 0x01,E_PHSPWRREVERSE_PL_THR/*"功率反向事件有功功率触发下限"*/, 0, FORMAT_PW, FUN_NULL, E_INFO_NULL},       
    {7, 0x01,E_PHSPWRREVERSE_TIME_THR/*"功率反向事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3008 过载事件*/ 
    {8, 0x01,E_UPLOAD_PL_THR/*"过载事件有功功率触发下限"*/, 0, FORMAT_PW, FUN_NULL, E_INFO_NULL},       
    {8, 0x01,E_UPLOAD_TIME_THR/*"过载事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       


    /*300B 电能表无功需量超限事件  OBIS还没有*/
    {0x0B,0x01,E_RDEMAND_PL_THR, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        
    {0x0B,0x01,E_RDEMAND_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},

     /*失压事件的属性13，也作为参变量来获取OBIS 注意序列号为EN_CLS24_ATT_13_LOST_V_STATIC*/
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LOST_VOLT_SUM_CNTS, 0, FORMAT_U4_UBCD4, FUN_NULL, E_SUM_LOST_V_COUNTS},        
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,E_TOTAL_LOSTVOLT_TIME, 0, FORMAT_U4_UBCD4, FUN_NULL, E_SUM_LOST_V_TIME},
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LAST_LOST_VOLT_ST_TM, 0, FORMAT_DATETIME, FUN_NULL, E_LAST_LOST_V_TIME},        
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LAST_LOST_VOLT_END_TM, 0, FORMAT_DATETIME, FUN_NULL, E_LAST_LOST_V_TIME},

     
};

#else
const ST_CLASS24_PHASE_EVENT_OI_OBIS CLASS_24_OI_OBIS[]=
{
    {0x3005, E_OBIS_NULL,E_OBIS_NULL,E_OBIS_NULL,E_OBIS_NULL}, /*过流事件 E_OVER_CURR_LA_EVNT_RCRD*/ 
};

/*注:这个表和上面的表一一对应*/
const ST_Class24_PARAMENT_ObjectList CLASS24_PARAMENT_LIST[]=
{
    {2,READ_WRITE, ST_2_S32_U8}           /*3005 过流事件*/ 
};

/*24类参数OBIS对应表*/
const ST_PARAMENT_OAD_OBIS_INFO CLASS24_PARAMENT_OBIS_LIST[]=
{
    {5,0x01,E_OVERCURR_IL_THR,   0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},            /*3005 过流事件 电流触发下限*/ 
    {5,0x01,E_OVERCURR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL}       /*3005 过流事件 判定延时时间*/ 
};

#endif
/*****************************************************************************
 函 数 名  : Get_Base_Class24_Obis
 功能描述  : 求基本OBIS值
 输入参数  : ST_OAD Oad  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U Get_Base_Class24_Obis(ST_OAD Oad,INT8U *pSeqNum)
{
    INT8U i;
    INT16U Oi,OBIS_EVENT = 0xffff;
    
    Oi  = Oad.Oi.asInt;
    
    for(i=0;i<ARRAY_SIZE(CLASS_24_OI_OBIS);i++)
    {
       if(Oi == CLASS_24_OI_OBIS[i].OI)
       {
          #ifdef THREE_PHASE
          /*功率反向事件有总,无功需量超限有第一象限*/
          if((OI_PWR_REVERSE_EVENT == Oi)||(OI_OVER_R_DEMAND_EVENT == Oi))  
          {  
             OBIS_EVENT = CLASS_24_OI_OBIS[i].OBIS_1;   /*事件记录表，取第一个值*/
          }  
          else
          #endif  
          {
             OBIS_EVENT = CLASS_24_OI_OBIS[i].OBIS_2;   /*事件记录表，取第一个值*/
          } 
          
          *pSeqNum = i;
          break;
       } 
   }  

   return OBIS_EVENT;
    
}    

/*****************************************************************************
 函 数 名  : get_class24_Obis
 功能描述  : 查找7类数据的OBIS
 输入参数  : SUB_OBJECT *pObject                     
             ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 11
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U get_class24_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U  AID,i;
    INT16U OBIS_EVENT;    /*事件OBIS*/

    AID= pObject->Oad.AId;
    /*参数*/
    if((PARAMENT_AID_CLASS24 == AID)||(EN_CLS24_ATT_13_LOST_V_STATIC == AID))
    {
        return get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                             (const ST_PARAMENT_OAD_OBIS_INFO *)CLASS24_PARAMENT_OBIS_LIST,ARRAY_SIZE(CLASS24_PARAMENT_OBIS_LIST));
    }   
    else
    {
       /*1.先求事件的OBIS*/ 
       //Oi = pObject->Oad.Oi.asInt;
       
       OBIS_EVENT = Get_Base_Class24_Obis(pObject->Oad,&i);       

       if(2== AID)   /*关联对象不用加*/
       {
          pObisInfo_bak->Obis = OBIS_EVENT;
          pObisInfo_bak->num = 1;
       }          
       else if((6<= AID)&&(9>= AID))  /*事件记录表*/
       {          
           OBIS_EVENT = *((&CLASS_24_OI_OBIS[i].OBIS_2)+ AID-7);
           if(E_OBIS_NULL == OBIS_EVENT)
           {
               pObisInfo_bak->num = 0;
               return 0;
           } 
           else
           { 
               pObisInfo_bak->Obis = OBIS_EVENT;
               pObisInfo_bak->num = 1;
           }
       }        
       else /*如属性3，4，10，14等*/
       { 
           /*2.得到基本OBIS,然后根据事件记录OBIS，计算具体值*/
           for(i=0;i<ARRAY_SIZE(OBIS_BASE_CLASS24_EVENT);i++)
           {
                if(AID == OBIS_BASE_CLASS24_EVENT[i].Aid)
                {   
                    /*加1是因为是块*/
                    pObisInfo_bak->Obis = (OBIS_EVENT- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[i].ATT_OBIS_BASE)+1;                       
                    pObisInfo_bak->FormatId = OBIS_BASE_CLASS24_EVENT[i].FormatId;
                    break;
                }              
           }

           /*最大记录数*/   /*24类，固定1*/
           if(4 == AID)
           { 
              pObisInfo_bak->num = 1;      
           }
           else
           {
              if(pObject->Oad.IId>=2)
              {   
                 #ifdef THREE_PHASE
                 if((OI_PWR_REVERSE_EVENT!=pObject->Oad.Oi.asInt)&&(OI_OVER_R_DEMAND_EVENT!=pObject->Oad.Oi.asInt))
                 #endif   
                 {
                    pObisInfo_bak->Obis = pObisInfo_bak->Obis-1;  /*只有ABC，没有总*/
                 }    
              }  
              pObisInfo_bak->num = 4;      /*主要是用于显示查找*/
           }             
       }            

       return 1;
    }   

    
}


/*"************************************************"*/
/*24类事件元素查找*/
INT8U get_class24_object(SUB_OBJECT *pObject)
{
    INT8U i,AID,IID;
    INT16U OI;
    const ST_CLASS24_AID_INFO *pCLASS24_AID_LIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId; 

    /*1.查找OI*/
    for(i=0;i<ARRAY_SIZE(CLASS_24_OI_OBIS);i++)
    {
        if(OI == CLASS_24_OI_OBIS[i].OI)
        {
           break; 
        }    
    }    

    if(i>=ARRAY_SIZE(CLASS_24_OI_OBIS))
    {
        return DAR_4_OANODEF;
    }  

    pObject->Class = 24;

    if(14<AID)
    {
       return DAR_4_OANODEF; 
    }    

    #ifdef SINGLE_PHASE  
    if((6 == AID)||(8 == AID)||(9 == AID))
    {
        return DAR_3_NOACESS;
    }       
    #endif
    
    if((1 == AID) ||((6<= AID)&&(9>= AID))) /*逻辑名或事件记录表*/
    {    
        return DAR_0_OK;
    }        

    /*2.查找参数AID = 5*/
    if(5 == AID)
    {
       if(IID > CLASS24_PARAMENT_LIST[i].MAX_IID)
       {
           return DAR_8_OVERFLOW;
       } 
       else
       {
            pObject->seq_num = OI - BASE_EVENT_OI;   
            pObject->Pointer = CLASS24_PARAMENT_LIST[i].Pointer;
            pObject->Right_RW= CLASS24_PARAMENT_LIST[i].Right_RW;
       }         
    }    

    /*3.查找其它参数AID*/    
    for(i=0;i<ARRAY_SIZE(CLASS24_AID_LIST);i++)
    {
        pCLASS24_AID_LIST = &CLASS24_AID_LIST[i];
        if(AID == pCLASS24_AID_LIST->AID)
        {
           if(IID > pCLASS24_AID_LIST->Max_IID)
           {
              return DAR_8_OVERFLOW;
           }  
           else
           {
              pObject->seq_num   = i;
              pObject->Pointer   = pCLASS24_AID_LIST->Pointer;
              pObject->Right_RW  = pCLASS24_AID_LIST->RW;
           } 
        } 
    }     

    #ifdef THREE_PHASE /*只有失压有这个属性*/
    if(13 == AID)    
    { 
        if(OI_LOST_VOLT_EVENT!=OI)
        {    
            return DAR_4_OANODEF;
        }
        else
        {
            pObject->seq_num = EN_CLS24_ATT_13_LOST_V_STATIC;
        }    
    }         
    #endif   
    
    
    return DAR_0_OK;

}

/*****************************************************************************
 函 数 名  : PP_Obis_Oad_class24
 功能描述  : class24 obis转OAD
 输入参数  : INT16U Obis   
             ST_OAD *pOad  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 01 02
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U PP_Obis_Oad_class24(INT16U Obis,ST_OAD *pOad)
{
    INT8U  i,j;
    const INT16U *pObis;
    
    for(i=0;i<ARRAY_SIZE(CLASS_24_OI_OBIS);i++)
    { 
       pObis = &CLASS_24_OI_OBIS[i].OBIS_1;
       for(j=0;j<4;j++)
       {             
           if(Obis == *pObis)
           {              
              break;          
           } 

           pObis++;
       }

       if(j<4)
       {
          break;
       } 
    }  

    if(i<ARRAY_SIZE(CLASS_24_OI_OBIS))
    {    
        pOad->Oi.asInt  = CLASS_24_OI_OBIS[i].OI;
        pOad->AId       = j+0x06;
        pOad->IId       = 0x00;
        return DAR_0_OK;
    }
    else
    {    
        return DAR_FF_OTHERERR;
    }
}   

#ifdef SINGLE_PHASE
/*****************************************************************************
 函 数 名  : Get_Class24_Att3
 功能描述  : 当前记录数
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

当前记录数∷=structure
{
  记录表1当前记录数 long-unsigned，  ----null
  记录表2当前记录数 long-unsigned，
  记录表3当前记录数 long-unsigned，  ----null
  记录表4当前记录数 long-unsigned，  ----null
}

*****************************************************************************/ 
INT8U *Get_Class24_Att3(INT8U *pdata,ST_OAD Oad)
{
    const INT8U ST_4_U16_NULL_3[] = {structure,4,null_data,0,longunsigned,2,null_data,0,null_data,0};

    INT8U *pdst = pdata;
    INT8U Temp_data[32];
    INT8U i;
    INT16U Obis;
    
    Obis = Get_Base_Class24_Obis(Oad,&i);
    
    Obis = Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[0].ATT_OBIS_BASE+1;                          

    GetSingle(Obis, Temp_data);

    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    pdst += EnCode_Data((ST_ENCODE_INFO * )ST_4_U16_NULL_3, Temp_data, pdst, Oad.IId);
    
    return pdst;
} 
/*****************************************************************************
 函 数 名  : Get_Class24_Att10
 功能描述  : 24类当前值记录表
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

    
当前值记录表∷=array structure
{
    事件发生次数 double-long-unsigned，
    事件累计时间 double-long-unsigned，
}

*****************************************************************************/
INT8U *Get_Class24_Att10(INT8U *pdata,ST_OAD Oad)
{
    /*第一个类型，先定义为structe，主要为了好调用编码模块，然后面根据IID=0，再改为array*/
    const INT8U AR_Class24_Att10[]  = {structure,0x04,null_data,0,structure,2,doublelongunsigned,2,doublelongunsigned,2,null_data,0,null_data,0,};

    INT8U *pdst = pdata;
    INT8U Temp_data[32]={0};
    INT8U i;

    INT16U Obis,Event_Obis;

    /*1.事件发生次数*/
    Event_Obis = Get_Base_Class24_Obis(Oad,&i);
    
    Obis = Event_Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[2].ATT_OBIS_BASE+1;                          
    
    GetSingle(Obis, Temp_data);

    /*单相表，24类，只有一个过流，所以这里直接取了，三相表在前面加个获取OBIS的算法即可*/ 
    GetSingle(E_OVER_CURR_LA_DURATION, &Temp_data[4]);
    
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    pdst += EnCode_Data((ST_ENCODE_INFO * )AR_Class24_Att10, Temp_data, pdst, Oad.IId);

    /*如果IID=0，则改成array*/
    if(0 == Oad.IId)
    {
       *(pdata+sizeof(ST_OAD)+1) = array;
    }    
    
    return pdst;
}  


/*****************************************************************************
 函 数 名  : Get_Class24_Att14
 功能描述  : 24类时间状态记录表
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

时间状态记录表∷=array structure
{
    最近一次发生时间 datetime_s，
    最近一次结束时间 datetime_s
}

*****************************************************************************/
INT8U *Get_Class24_Att14(INT8U *pdata,ST_OAD Oad)
{
    /*下面故意改成RAM数据，以便后面更改为空 ,第一个字节故意为struct，以便调用编码接口*/
    INT8U AR_Class24_Att14[]  = {structure,0x04,null_data,0,structure,2,date_time_s,7,date_time_s,7,null_data,0,null_data,0,};

    INT8U *pdst = pdata;
    INT8U Temp_data[32];
    INT8U Temp_data_dst[32];
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSS,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;

    INT16U Obis;
    INT16S Len;
    INT8U i;
    
    Obis = Get_Base_Class24_Obis(Oad,&i);
    
    Obis = Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[3].ATT_OBIS_BASE+1;                          

    Len = GetSingle(Obis, Temp_data);

    /*两个空*/
    if(sizeof(ST_6TIME_FORM) > Len)
    {
        //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[6], 4);
        AR_Class24_Att14[6] = null_data;
        AR_Class24_Att14[7] = 0;
        AR_Class24_Att14[8] = null_data;
        AR_Class24_Att14[9] = 0;
    }  
    else if(2*sizeof(ST_6TIME_FORM) > Len)/*1个空*/
    {
        //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[8], 2);
        AR_Class24_Att14[8] = null_data;
        AR_Class24_Att14[9] = 0;
    }    

    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,Len);

    pdst += EnCode_Data((ST_ENCODE_INFO * )AR_Class24_Att14, Temp_data_dst, pdst, Oad.IId);

    /*如果IID=0，则改成array*/
    if(0 == Oad.IId)
    {
       *(pdata+sizeof(ST_OAD)+1) = array;
    }    
    
    return pdst;
} 

#else



/*****************************************************************************
 函 数 名  : Get_F_SUM_F_B_PHASE
 功能描述  : 确定是否有总和B相
 输入参数  : ST_OAD Oad        
             INT8U i           
             INT8U j           
             INT8U F_SUM       
             INT8U F_B_PHNASE  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 04 16
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void Get_F_SUM_F_B_PHASE(ST_OAD *Oad,INT8U *i,INT8U *j,INT8U *F_SUM,INT8U *F_B_PHASE)
{
    INT8U IID,F_331 = 0;
    INT16U Oi,UiState;

    GetSingle(E_MU_STATUS, (INT8U *)&UiState);
    F_331 = (UiState&0x01);   /*"bit值为0表示3P4W，为1表示3P3W"*/

    Oi  = Oad->Oi.asInt; 
    IID = Oad->IId;

    if(0 == IID)
    {
        *i= 0;*j=3;
    }    
    else
    {
        *i= IID-1;*j=IID-1;
    }    
    
    if((OI_PWR_REVERSE_EVENT == Oi)||(OI_OVER_R_DEMAND_EVENT == Oi))  /*功率反向事件有总,无功需量超限有第一象限*/
    {
        *F_SUM = 0xff;
    }   
    else
    {
        *F_SUM = 0;
    }    
    

    *F_B_PHASE = 0;
    if(0 != F_331)
    {
       /*无功需量超限*/
       if(OI_OVER_R_DEMAND_EVENT == Oi)   
       {  
           *F_B_PHASE = 0xff;
       }
    } 
    else
    {
        *F_B_PHASE = 0xff;
    }    
}

/*****************************************************************************
 函 数 名  : Get_Class24_Att3
 功能描述  : 当前记录数
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/ 
/*三相表分两种情况，总ABC和ABC，如果是331表，则没有B相*/
INT8U *Get_Class24_Att3(INT8U *pdata,ST_OAD Oad)
{    
    INT8U *pdst = pdata;
    INT8U Temp_data[32];
    INT8U i,j,k;
    INT16U Obis,OBIS_BASE;
    INT8U F_SUM,F_B_PHASE;  /*总和B相标志:不等于0表示有*/

    /*1.第1步，确定总和B相是否打包空*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);   
    
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    if(0 == Oad.IId)
    {
       *pdst++ = structure;
       *pdst++ = 4;
    } 
    /*2018-03-26下面获取数据，要与底层商量好，如何获取数据，分IID=0和IID不等0情况，底层如何回数据*/
    
    OBIS_BASE = Get_Base_Class24_Obis(Oad,&k);
    OBIS_BASE = OBIS_BASE- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[0].ATT_OBIS_BASE+1;                          

    for(;i<=j;i++)
    {    
    
        if(((0 ==i)&&(0 == F_SUM))||((2==i)&&(0 == F_B_PHASE)))
        {           
            pdst += EnCode_Data((ST_ENCODE_INFO * )TYPE_NULL, Temp_data, pdst, 0);
            continue;   /*直接返回算了，免得OBIS不对，取数可能异常了*/
        } 

        
        if(0 == F_SUM)
        {
           Obis = OBIS_BASE+i-1; 
        }  
        else
        {
           Obis = OBIS_BASE+i;
        }             

        GetSingle(Obis, Temp_data); 
        
        pdst += EnCode_Data((ST_ENCODE_INFO * )TYPE_U16, Temp_data, pdst, 0);
        
    }    
    
    return pdst;
}  

/*****************************************************************************
 函 数 名  : Get_Class24_Att10
 功能描述  : 24类当前值记录表
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

    
当前值记录表∷=array structure
{
    事件发生次数 double-long-unsigned，
    事件累计时间 double-long-unsigned，
}
*****************************************************************************/
/*三相表分两种情况，总ABC和ABC，如果是331表，则没有B相*/
INT8U *Get_Class24_Att10(INT8U *pdata,ST_OAD Oad)
{
    /*第一个类型，先定义为structe，主要为了好调用编码模块，然后面根据IID=0，再改为array*/

    #if ADD_TIME_SET_0

    #else
    const INT8U CONST_ST_2_U32_U32[]  ={structure,2,doublelongunsigned,2,doublelongunsigned,2};
    #endif
    
    INT8U ST_2_U32_U32[]  ={structure,2,doublelongunsigned,2,doublelongunsigned,2};

    INT8U *pdst = pdata;
    INT8U Temp_data[32]={0}; 

    INT16U Obis,Event_Obis;
    INT8U i,j,k; 
    INT8U F_SUM,F_B_PHASE;  /*总和B相标志:不等于0表示有*/ 

    /*1.第1步，确定总和B相是否打包空*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);
    
    pdst = EnCodeOAD(&Oad, pdst);

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    if(0 == Oad.IId)
    {
       *pdst++ = array;
       *pdst++ = 4;
    }

    Event_Obis = Get_Base_Class24_Obis(Oad,&k);
    
    Event_Obis = Event_Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[2].ATT_OBIS_BASE+1;                          

    for(;i<=j;i++)
    {
        #if ADD_TIME_SET_0
        #else
        LIB_ConstMemCpy(CONST_ST_2_U32_U32, ST_2_U32_U32, sizeof(ST_2_U32_U32)); 
        #endif
        
        if(((0 ==i)&&(0 == F_SUM))||((2==i)&&(0 == F_B_PHASE)))
        {            
            pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_NULL_NULL, Temp_data, pdst, 0);
            continue;   /*直接返回算了，免得OBIS不对，取数可能异常了*/
        } 

        
        if(0 == F_SUM)
        {
           Obis = Event_Obis+i-1; 
        }  
        else
        {
           Obis = Event_Obis+i;
        }   

        /*发生次数*/
        GetSingle(Obis, Temp_data);

        /*要转换一下*/
        /*累计时间*/   /*算累计时间OBIS要-1，因为LIST_RECORD_TIME中用的是记录OBIS列表，不用加1*/
        Obis = Get_Duration_OBIS(Obis-OBIS_BASE_CLASS24_EVENT[2].ATT_OBIS_BASE+BASE_CLASS24_OBIS-1);//Event_Obis- BASE_CLASS7_OBIS +E_EVNT_DURATION_BLK+1;                          

        if(NULL !=Obis)
        {          
            GetSingle(Obis, &Temp_data[4]);
        }
        else
        {
            #if ADD_TIME_SET_0
            Temp_data[4] = 0;
            Temp_data[5] = 0;
            Temp_data[6] = 0;
            Temp_data[7] = 0;
            #else
            ST_2_U32_U32[4] = null_data;
            ST_2_U32_U32[5] = 0;
            #endif
        }    
            
        pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_U32_U32, Temp_data, pdst, 0);

    }    
    
    return pdst;
}  

/*****************************************************************************
 函 数 名  : Get_Class24_Att14
 功能描述  : 24类时间状态记录表
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 03
    作    者   : xyt
    修改内容   : 新生成函数

时间状态记录表∷=array structure
{
    最近一次发生时间 datetime_s，
    最近一次结束时间 datetime_s
}

*****************************************************************************/
/*三相表分两种情况，总ABC和ABC，如果是331表，则没有B相*/
INT8U *Get_Class24_Att14(INT8U *pdata,ST_OAD Oad)
{
    /*下面故意改成RAM数据，以便后面更改为空 ,第一个字节故意为struct，以便调用编码接口*/

    const INT8U CONST_ST_2_DATES[]= {structure,2,date_time_s,2,date_time_s,2};

    INT8U ST_2_DATES[sizeof(CONST_ST_2_DATES)];//= {structure,2,date_time_s,2,date_time_s,2};
    
    INT8U *pdst = pdata;
    INT8U Temp_data[32];
    INT8U Temp_data_dst[32];
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSS,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;
    
    INT16S Len;

    INT16U Obis,Event_Obis;
    INT8U i,j,k; 
    INT8U F_SUM,F_B_PHASE;  /*总和B相标志:不等于0表示有*/

    /*1.第1步，确定总和B相是否打包空*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);
    
    pdst = EnCodeOAD(&Oad, pdst);

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    if(0 == Oad.IId)
    {
       *pdst++ = array;
       *pdst++ = 4;
    }
    
    Event_Obis = Get_Base_Class24_Obis(Oad,&k);
    
    Event_Obis = Event_Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[3].ATT_OBIS_BASE+1;                          

    for(;i<=j;i++)
    {     
        LIB_ConstMemCpy(CONST_ST_2_DATES, ST_2_DATES, sizeof(ST_2_DATES)); 

        if(((0 ==i)&&(0 == F_SUM))||((2==i)&&(0 == F_B_PHASE)))
        {            
            pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_NULL_NULL, Temp_data, pdst, 0);
            continue;   /*直接返回算了，免得OBIS不对，取数可能异常了*/
        } 
        
        if(0 == F_SUM)
        {
           Obis = Event_Obis+i-1; 
        }  
        else
        {
           Obis = Event_Obis+i;
        }   
    
        Len = GetSingle(Obis, Temp_data);
        
        /*两个空*/
        if(sizeof(ST_6TIME_FORM) > Len)
        {
            //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[6], 4);
            ST_2_DATES[2] = null_data;
            ST_2_DATES[3] = 0;
            ST_2_DATES[4] = null_data;
            ST_2_DATES[5] = 0;
        }  
        else if(2*sizeof(ST_6TIME_FORM) > Len)/*1个空*/
        {
            //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[8], 2);
            ST_2_DATES[4] = null_data;
            ST_2_DATES[5] = 0;
        }  
        else
        {
            ST_2_DATES[2] = date_time_s;
            ST_2_DATES[3] = 2;
            ST_2_DATES[4] = date_time_s;
            ST_2_DATES[5] = 2;
        }    
         
        FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,Len);

        pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_DATES, Temp_data_dst, pdst, 0);

    }
    
    return pdst;
}  
/*****************************************************************************
 函 数 名  : 失压统计打包
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 04 25
    作    者   : xyt
    修改内容   : 新生成函数

属性13（失压统计）∷=structure
{
事件发生总次数   double-long-unsigned，
事件总累计时间   double-long-unsigned（单位：秒，无换算），
最近一次失压发生时间 date_time_s，
最近一次失压结束时间 date_time_s
}

*****************************************************************************/
INT8U *Get_Class24_Att13(INT8U *pdata,ST_OAD Oad)
{
    const INT16U Obis[MAX_OBIS_NUM_ONE_OAD] ={LOST_VOLT_SUM_CNTS,E_TOTAL_LOSTVOLT_TIME,LAST_LOST_VOLT_ST_TM,LAST_LOST_VOLT_END_TM};
    
    INT8U *pdst=pdata;
    INT8U Temp_data[32],IID;
    INT8U Temp_data_dst[32];
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSS,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;

    LIB_MemSet(0, Temp_data, 32);

    pdst = EnCodeOAD(&Oad, pdst);

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    IID = Oad.IId;
    
    if(0 == IID)
    {    
        GetList((INT16U *)Obis, 4, Temp_data, 32);

        LIB_MemCpy(Temp_data, Temp_data_dst, sizeof(INT32U)*2);
        
        FRMT_NdataChange(Temp_data+8,Temp_data_dst+8,inFormat,outFormat,12);        
        
    }    
    else
    {
        GetSingle(Obis[IID-1], Temp_data);
        if((3 == IID)||(4 == IID))
        {
            FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,6);
        }   
        else
        {
            LIB_MemCpy(Temp_data, Temp_data_dst, sizeof(INT32U));
        }    
    }    

    pdst += EnCode_Data((ST_ENCODE_INFO *)ST_4_U32_U32_DATE_S_DATE_S,Temp_data_dst,pdst,IID);

    return pdst;
    
}
#endif



/*"************************************************"*/

