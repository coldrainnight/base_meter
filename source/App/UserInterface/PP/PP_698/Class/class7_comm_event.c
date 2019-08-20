/*"*****************************************************************************"*/
/*"  FileName: class7_comm_event.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  通用事件类文件   "*/
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
/*"*******************************************************************************"*/
/*7 类属性列表*/
const ST_SPECIAL_CLASS_AID_INFO CLASS7_AID_LIST[]=
{
    /*1.逻辑名*/
    /* 2.事件记录表 */
    {3, READ_WRITE,  0xff,AR_OAD},       /*3.关联对象属性表*/
    {4, READ_ONLY,   0x01,TYPE_U16},     /*4.当前记录数*/        
    {5, READ_ONLY,   0x01,TYPE_U16},     /*5.最大记录数*/
    /*6,配置参数     单独配置列表*/
    {7, READ_ONLY,  0x02,AR_ST_2_ENUM_ST_2_U32 },  /*7.当前值记录表*/
    {8, READ_WRITE, 0x01,TYPE_ENUM},     /*8.上报标识*/
    {9, READ_ONLY,  0x01,TYPE_BOOL},     /*9.有效标识*/    
    {10,READ_ONLY,  0x02,AR_ST_2_NULL_ST_2_DATES}, /*10.时间状态记录表*/
};

/*对应上面的属性的OBIS列表，之所以与上面的分开写，主要是考虑到可能字节对齐*/
const ST_CLASS7_OBIS_INFO OBIS_BASE_CLASS7_EVENT[]=
{
    //{3,MAX_FORMAT_ID, FUN_NULL, 0x00,E_PWR_DOWN_EVNT_RCRD}, /*3.关联对象属性表*/
    {4,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_CNT},        /*4.当前记录数*/        
    {5,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_MAX_CNT},        /*5.最大记录数*/
    /*6,配置参数     单独配置列表*/
    {7,FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_VALUE},      /*7.当前值记录表*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},                       /*8.上报标识*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},                       /*9.有效标识*/    
    {10,FORMAT_DATETIME, FUN_NULL, E_INFO_NULL,E_RCRD_TIME_STATUS},   /*10.时间状态记录表*/     
};    

const ST_CLASS7_COMM_EVENT_OI_OBIS CLASS7_OI_RECORD_OBIS[]=
{
    {0x3011, E_PWR_DOWN_EVNT_RCRD},         /*1.电能表掉电事件*/ 
    
    {0x3013, E_MTR_CLR_EVNT_RCRD},          /*3.电能表清零事件*/ 
    {0x3015, E_EVNT_CLR_EVNT_RCRD},         /*4.电能表事件清零事件*/ 
    {0x3016, E_JUST_TIME_EVNT_RCRD},        /*5.电能表校时事件*/ 
    
    {0x301B, E_MTR_COVER_OPEN_EVNT_RCRD},   /*10.电能表开盖事件*/ 
    
     {0x302B, E_RELAY_ERR_EVNT_RCRD},        /*21.电能表负荷开关误动作事件*/ 
    {0x302C, E_PWR_FAILURE_EVNT_RCRD},      /*22.电能表电源异常事件*/ 
    /*----自定义事件*/
    {OI_METER_RST_EVENT, E_SYS_RST_RCRD},   /*23.电能表复位事件*/ 
    {OI_OSCILLATOR_ERR_EVENT,E_OSCILLATOR_ERR_RCRD}, /*"晶振故障事件"*/
    
#ifdef THREE_PHASE
    {OI_PD_PON_EVENT,E_PD_PON_RCRD},         /*"掉上电记录"*/
    {OI_PARA_ERR_EVENT,E_PARA_ERR_RCRD},     /*"参数异常记录"*/
    {OI_ENG_ERR_EVENT,E_ENG_ERR_RCRD},       /*"电量异常记录"*/ 
#endif    

    {0x302E, E_RTC_ERR_EVNT_RCRD},           /*37.电能表时钟故障事件	*/ 
    {0x302F, E_MS_ERR_EVNT_RCRD},            /*38.电能表计量芯片故障事件*/ 
//--------------------------------------------------
#ifdef THREE_PHASE
    {0x3009, E_OVER_PA_DEMAND_EVNT_RCRD}, /*23.电能表正向有功需量超限事件*/  
    {0x300A, E_OVER_NA_DEMAND_EVNT_RCRD}, /*24.电能表反向有功需量超限事件*/ 
    {0x300C, E_FACTOR_LOW_EVNT_RCRD},     /*25.电能表功率因数超下限事件*/ 
    {0x300D, E_ALL_LOST_VOLT_EVNT_RCRD},  /*26.电能表全失压事件*/ 
    {0x300E, E_OBIS_NULL},                /*27.电能表辅助电源掉电事件*/ 
    {0x300F, E_VOLT_INVERSE_EVNT_RCRD},   /*28.电能表电压逆相序事件*/ 
    {0x3010, E_CURR_INVERSE_EVNT_RCRD},   /*29.电能表电流逆相序事件*/ 
    {0x3014, E_DEMAND_CLR_EVNT_RCRD},     /*30.电能表需量清零事件*/ 
    {0x301C, E_TAIL_COVER_OPEN_EVNT_RCRD}, /*31.电能表开端钮盒事件*/ 
    {0x301D, E_VOLT_IMBALANCE_EVNT_RCRD}, /*32.电能表电压不平衡事件*/ 
    {0x301E, E_CURR_IMBALANCE_EVNT_RCRD}, /*33.电能表电流不平衡事件*/ 
    {0x3023, E_R_ENGY_COM_PRG_EVNT_RCRD}, /*34.电能表无功组合方式编程事件*/ 
    {0x302A, E_CONSTANT_MGNT_DISTURB_EVNT_RCRD}, /*35.电能表恒定磁场干扰事件*/ 
    {0x302D, E_CURR_IMBALANCE_BADLY_EVNT_RCRD}, /*36.电能表电流严重不平衡事件	*/ 
#endif 
};    

/*注:这个表和上面的表不是一一对应，有则有，没有，则不体现，事件参数:AID=6*/
const ST_Class7_PARAMENT_ObjectList CLASS7_PARAMENT_LIST[]=
{ 
   {0x3011,1,READ_ONLY, ST_1_U8},      /*1.电能表掉电事件*/ 
   {0x302E,1,READ_WRITE, ST_1_U8},      /*12.电能表时钟故障事件	*/ 
   {0x302F,1,READ_WRITE, ST_1_U8},      /*13.电能表计量芯片故障事件*/ 
#ifdef THREE_PHASE
   {0x3009,2,READ_WRITE, ST_2_U32_U8},  /*2.电能表正向有功需量超限事件*/
   {0x300A,2,READ_WRITE, ST_2_U32_U8},  /*3.电能表反向有功需量超限事件*/
   {0x300C,2,READ_WRITE, ST_2_S16_U8},  /*4..电能表功率因数超下限事件*/ 
   {0x300E,1,READ_WRITE, ST_1_U8},      /*5..电能表辅助电源掉电事件*/ 
   {0x300F,1,READ_WRITE, ST_1_U8},      /*6.电能表电压逆相序事件*/ 
   {0x3010,1,READ_WRITE, ST_1_U8},      /*7.电能表电流逆相序事件*/ 
   {0x301D,2,READ_WRITE, ST_2_S16_U8},  /*9.电能表电压不平衡事件*/ 
   {0x301E,2,READ_WRITE, ST_2_S16_U8},  /*10.电能表电流不平衡事件*/ 
   {0x302D,2,READ_WRITE, ST_2_S16_U8},  /*11.电能表电流严重不平衡事件	*/ 
#endif   
};  

/*7类参数OBIS对应表 与CLASS7_PARAMENT_LIST 对应，尤其是SEQ 为OI-0x3000 */
const ST_PARAMENT_OAD_OBIS_INFO CLASS7_PARAMENT_OBIS_LIST[]=
{    
     /*3011 电能表掉电事件*/ 
    {0x11, 0x01,E_OBIS_NULL/*已特殊处理*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*1.电能表掉电事件*/  
    /*{0x302E 电能表时钟故障事件*/ 
    {0x2E,0x01,E_CLKERR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},   /*12.电能表时钟故障事件	*/ 
    /*{0x302F 电能表计量芯片故障事件*/ 
    {0x2F,0x01,E_MUERR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},              /*13.电能表计量芯片故障事件*/  
#ifdef THREE_PHASE
    /*0x3009  电能表正向有功需量超限事件*/
    {0x09, 0x01,E_AADEMAND_PL_THR/*"有功需量超限事件需量触发下限"*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},      
    {0x09, 0x01,E_AADEMAND_TIME_THR/*"需量超限事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      

    /*0x300A 电能表反向有功需量超限事件*/
    {0x0A, 0x01,E_NADEMAND_PL_THR/*"有功需量超限事件需量触发下限"*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, 
    {0x0A, 0x01,E_NADEMAND_TIME_THR/*"需量超限事件判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL}, 

    /*0x300C 电能表功率因数超下限事件*/
    {0x0C, 0x01,E_OVERFACTOR_RATE_THR/*"总功率因数超下限阀值"*/, 0, FORMAT_S2_1_UBCD2_1, FUN_NULL, E_INFO_NULL},      /*4.电能表功率因数超下限事件*/   
    {0x0C, 0x01,E_OVERFACTOR_TIME_THR/*"总功率因数超下限判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      

    /*0x300E 电能表辅助电源掉电事件*/
    {0x0E, 0x01,E_OBIS_NULL/*无OBIS*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*5.电能表辅助电源掉电事件*/   

    /*{0x300F 电能表电压逆相序事件 */
    {0x0F, 0x01,E_VOLTREVERSE_TIME_THR/*"电压逆相序事件判定时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*6.电能表电压逆相序事件*/   

    /*{0x3010 电能表电流逆相序事件*/
    {0x10, 0x01,E_CURRREVERSE_TIME_THR/*"电流逆相序事件判定时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*7.电能表电流逆相序事件*/   

    /*{0x301D 电能表电压不平衡事件 */
    {0x1D, 0x01,E_VOLTIMB_RATE_THR/*"电压不平衡率限值"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},      /*9.电能表电压不平衡事件*/    
    {0x1D, 0x01,E_VOLTIMB_TIME_THR/*"电压不平衡率判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*9.电能表电压不平衡事件*/    

    /*{0x301E 电能表电流不平衡事件*/ 
    {0x1E,0x01,E_CURRIMB_RATE_THR/*"电流不平衡率限值"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},     /*10.电能表电流不平衡事件*/ 
    {0x1E,0x01,E_CURRIMB_TIME_THR/*"电流不平衡率判定延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     /*10.电能表电流不平衡事件*/ 

    /*{0x302D 电能表电流严重不平衡事件	*/ 
    {0x2D,0x01,E_CURRBADIMB_RATE_THR/*"电流严重不平衡限值"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},     /*11.电能表电流严重不平衡事件	*/ 
    {0x2D,0x01,E_CURRBADIMB_TIME_THR/*"电流严重不平衡触发延时时间"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     /*11.电能表电流严重不平衡事件	*/ 

#endif
};
/*****************************************************************************
 函 数 名  : Get_Base_Class7_Obis
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
INT16U Get_Base_Class7_Obis(ST_OAD Oad)
{
    INT8U i;
    INT16U Oi,OBIS_EVENT = 0xffff;

    Oi  = Oad.Oi.asInt;
    
    for(i=0;i<ARRAY_SIZE(CLASS7_OI_RECORD_OBIS);i++)
    {
       if(Oi == CLASS7_OI_RECORD_OBIS[i].OI)
       {
          OBIS_EVENT = CLASS7_OI_RECORD_OBIS[i].OBIS;
          break;
       } 
    }  

   return OBIS_EVENT;
    
}    


/*****************************************************************************
 函 数 名  : get_class7_Obis
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
INT8U get_class7_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U  AID,i;
    INT16U OBIS_EVENT;    /*事件OBIS*/

    /*参数*/
    if(EN_CLS7_ATT_6_SET_PARAMENT == pObject->Oad.AId)
    {
        return get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                             (const ST_PARAMENT_OAD_OBIS_INFO *)CLASS7_PARAMENT_OBIS_LIST,ARRAY_SIZE(CLASS7_PARAMENT_OBIS_LIST));
    }        
    else
    {
       //Oi = pObject->Oad.Oi.asInt;
       AID= pObject->Oad.AId;
       OBIS_EVENT = Get_Base_Class7_Obis(pObject->Oad);
#ifdef PM_MODLE  
       if((OI_CARD_PLUG_IN_ERR_EVENT == pObject->Oad.Oi.asInt)&&(0x0b==AID))   /*异常插卡次数*/
       {
           pObisInfo_bak->Obis = E_UNLAW_PLUG_CNT;
           pObisInfo_bak->num  = 1;  
       } 
       else 
#endif        
        if((EN_CLS7_ATT_2_EVENT_RECORD_TABLE== AID)||(EN_CLS7_ATT_3_RELATION_OBJECT_TABLE== AID))  /*关联对象和记录表*/
       {           
           pObisInfo_bak->Obis = OBIS_EVENT;
           pObisInfo_bak->num = 1;
       }
       else
       { 
           /*2.得到基本OBIS,然后根据事件记录OBIS，计算具体值*/
           for(i=0;i<ARRAY_SIZE(OBIS_BASE_CLASS7_EVENT);i++)
           {
                if(AID == OBIS_BASE_CLASS7_EVENT[i].Aid)
                {   
                    /*加1是因为是块*/
                    pObisInfo_bak->Obis     = (OBIS_EVENT- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[i].ATT_OBIS_BASE)+1; 
                    pObisInfo_bak->FormatId = OBIS_BASE_CLASS7_EVENT[i].FormatId;
                    break;
                }              
           }
           /*当前值记录表，有两个OBIS*/
           if(EN_CLS7_ATT_7_CURRENT_RECORD_TABLE!=AID)
           { 
              pObisInfo_bak->num = 1;      /*7类，固定1*/
           }
           else
           {
              pObisInfo_bak->num = 2;
              pObisInfo_bak[1].Obis = (OBIS_EVENT- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[i].ATT_OBIS_BASE)+1;    
           }  

            //pObisInfo_bak->Offset = 0;

            /*显示格式*/
            //LIB_MemCpy((INT8U*)&OBIS_BASE_CLASS7_EVENT[i].FormatId, &pObisInfo_bak->FormatId, 3);
           
       }             
              
       return 1;
    }   
    
}


/*"************************************************"*/
/*7类事件元素查找*/
INT8U get_class7_object(SUB_OBJECT *pObject)
{
    INT8U i,AID,IID;
    INT16U OI;
    const ST_SPECIAL_CLASS_AID_INFO *pCLASS7_AID_LIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;   
    

    /*1.查找OI*/
    for(i=0;i<ARRAY_SIZE(CLASS7_OI_RECORD_OBIS);i++)
    {
        if((OI == CLASS7_OI_RECORD_OBIS[i].OI)&&(E_OBIS_NULL!=CLASS7_OI_RECORD_OBIS[i].OBIS))
        {
           break; 
        }    
    }    

    if(i>=ARRAY_SIZE(CLASS7_OI_RECORD_OBIS))
    {
        return DAR_4_OANODEF;
    }  

    pObject->Class = 7;
    

    if(EN_CLS7_ATT_2_EVENT_RECORD_TABLE >= AID)  /*罗辑名或事件记录表*/
    {   
        return DAR_0_OK;
    }    

    /*2.查找参数AID*/
    if(EN_CLS7_ATT_6_SET_PARAMENT == AID)
    {
        for(i=0;i<ARRAY_SIZE(CLASS7_PARAMENT_LIST);i++)
        {
           if(OI == CLASS7_PARAMENT_LIST[i].OI)
            {
               if(IID > CLASS7_PARAMENT_LIST[i].Max_IID)
               {
                   return DAR_8_OVERFLOW;
               } 
               else
               {    
                    pObject->seq_num = OI-BASE_EVENT_OI;
                    pObject->Pointer = CLASS7_PARAMENT_LIST[i].Pointer;
                    pObject->Right_RW= CLASS7_PARAMENT_LIST[i].Right_RW;
                    return DAR_0_OK;
               } 
            }   
        }    
        
    }  
    else if((OI_CARD_PLUG_IN_ERR_EVENT == OI)&&(0x0b==AID))   /*异常插卡次数*/
    {
       pObject->Pointer   = TYPE_U32;
       pObject->Right_RW  = READ_ONLY; 
       return DAR_0_OK;
    }  

    /*3.查找其它参数AID*/    
    for(i=0;i<ARRAY_SIZE(CLASS7_AID_LIST);i++)
    {
        pCLASS7_AID_LIST = &CLASS7_AID_LIST[i];
        if(AID == pCLASS7_AID_LIST->AID)
        {
           if(IID > pCLASS7_AID_LIST->Max_IID)
           {
              return DAR_8_OVERFLOW;
           }  
           else
           {
              pObject->seq_num = i;
              pObject->Pointer   = pCLASS7_AID_LIST->Pointer;
              pObject->Right_RW  = pCLASS7_AID_LIST->Right_RW;
              return DAR_0_OK;
           } 
        } 
    }   

    return DAR_4_OANODEF;

}


/*****************************************************************************
 函 数 名  : PP_Obis_Oad_class7
 功能描述  : class7 obis转OAD
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
INT8U PP_Obis_Oad_class7(INT16U Obis,ST_OAD *pOad)
{
    INT8U  i; 
    
    for(i=0;i<ARRAY_SIZE(CLASS7_OI_RECORD_OBIS);i++)
    {
       if(Obis == CLASS7_OI_RECORD_OBIS[i].OBIS)
       {
          pOad->Oi.asInt = CLASS7_OI_RECORD_OBIS[i].OI;
          pOad->AId = 0x02;
          pOad->IId = 0x00;
          
          return DAR_0_OK;
       } 
    }  

    return DAR_FF_OTHERERR;
}    

/*累计时间和记录对象的OBIS列表*/
const ST_OBIS_2 LIST_RECORD_TIME[]=
{
    #ifdef THREE_PHASE
    {E_OVER_CURR_LA_EVNT_RCRD,E_EVT_OVER_CURR_LA_DURATION}, 
    #else
    //{E_OVER_CURR_LA_EVNT_RCRD,E_OVER_CURR_LA_DURATION}, 
    #endif
    {E_PWR_DOWN_EVNT_RCRD,E_PWR_DOWN_DURATION},    
   #ifdef THREE_PHASE    
    {E_OVER_CURR_LB_EVNT_RCRD,E_EVT_OVER_CURR_LB_DURATION}, 
    {E_OVER_CURR_LC_EVNT_RCRD,E_EVT_OVER_CURR_LC_DURATION},     
    {E_LOST_VOLT_LA_EVNT_RCRD,E_EVT_LOST_VOLT_LA_DURATION},    
    {E_LOST_VOLT_LB_EVNT_RCRD,E_EVT_LOST_VOLT_LB_DURATION},  
    {E_LOST_VOLT_LC_EVNT_RCRD,E_EVT_LOST_VOLT_LC_DURATION},  
    {E_ALL_LOST_VOLT_EVNT_RCRD,E_EVT_LOST_VOLT_ALL_DURATION},  
    {E_LOST_CURR_LA_EVNT_RCRD,E_EVT_LOST_CURR_LA_DURATION}, 
    {E_LOST_CURR_LB_EVNT_RCRD,E_EVT_LOST_CURR_LB_DURATION},  
    {E_LOST_CURR_LC_EVNT_RCRD,E_EVT_LOST_CURR_LC_DURATION}, 
    {E_BRK_PHASE_LA_EVNT_RCRD,E_EVT_BREAK_PHASE_LA_DURATION}, 
    {E_BRK_PHASE_LB_EVNT_RCRD,E_EVT_BREAK_PHASE_LB_DURATION}, 
    {E_BRK_PHASE_LC_EVNT_RCRD,E_EVT_BREAK_PHASE_LC_DURATION},
    {E_BRK_CURR_LA_EVNT_RCRD,E_EVT_FAIL_CURR_LA_DURATION}, 
    {E_BRK_CURR_LB_EVNT_RCRD,E_EVT_FAIL_CURR_LB_DURATION}, 
    {E_BRK_CURR_LC_EVNT_RCRD,E_EVT_FAIL_CURR_LC_DURATION}, 
    {E_OVER_VOLT_LA_EVNT_RCRD,E_EVT_OVER_VOLT_LA_DURATION}, 
    {E_OVER_VOLT_LB_EVNT_RCRD,E_EVT_OVER_VOLT_LB_DURATION}, 
    {E_OVER_VOLT_LC_EVNT_RCRD,E_EVT_OVER_VOLT_LC_DURATION}, 
    {E_LACK_VOLT_LA_EVNT_RCRD,E_EVT_UNDER_VOLT_LA_DURATION}, 
    {E_LACK_VOLT_LB_EVNT_RCRD,E_EVT_UNDER_VOLT_LB_DURATION}, 
    {E_LACK_VOLT_LC_EVNT_RCRD,E_EVT_UNDER_VOLT_LC_DURATION}, 
    {E_PWR_REVERSE_EVNT_RCRD,E_EVT_PWR_REVERSE_DURATION}, 
    {E_PWR_REVERSE_LA_EVNT_RCRD,E_EVT_PWR_REVERSE_LA_DURATION}, 
    {E_PWR_REVERSE_LB_EVNT_RCRD,E_EVT_PWR_REVERSE_LB_DURATION}, 
    {E_PWR_REVERSE_LC_EVNT_RCRD,E_EVT_PWR_REVERSE_LC_DURATION}, 
    {E_OVER_LOAD_LA_EVNT_RCRD,E_EVT_OVER_LOAD_LA_DURATION},
    {E_OVER_LOAD_LB_EVNT_RCRD,E_EVT_OVER_LOAD_LB_DURATION},
    {E_OVER_LOAD_LC_EVNT_RCRD,E_EVT_OVER_LOAD_LC_DURATION},
    {E_OVER_PA_DEMAND_EVNT_RCRD,E_EVT_OVER_DEMAND_PA_DURATION},
    {E_OVER_NA_DEMAND_EVNT_RCRD,E_EVT_OVER_DEMAND_NA_DURATION},
    {E_OVER_R_DEMAND_1_EVNT_RCRD,E_EVT_OVER_DEMAND_I_DURATION},
    {E_OVER_R_DEMAND_2_EVNT_RCRD,E_EVT_OVER_DEMAND_II_DURATION},
    {E_OVER_R_DEMAND_3_EVNT_RCRD,E_EVT_OVER_DEMAND_III_DURATION},
    {E_OVER_R_DEMAND_4_EVNT_RCRD,E_EVT_OVER_DEMAND_IV_DURATION},
    {E_VOLT_IMBALANCE_EVNT_RCRD,E_EVT_VOLT_IMB_DURATION},
    {E_CURR_IMBALANCE_EVNT_RCRD,E_EVT_CURR_IMB_DURATION},
    {E_CURR_IMBALANCE_BADLY_EVNT_RCRD,E_EVT_CURR_BADLY_IMB_DURATION},
    {E_FACTOR_LOW_EVNT_RCRD,E_EVT_OVER_FACTOR_DURATION},
    {E_VOLT_INVERSE_EVNT_RCRD,E_EVT_VOLT_REVERSE_DURATION},
    {E_CURR_INVERSE_EVNT_RCRD,E_EVT_CURR_REVERSE_DURATION},   
   #endif
};    

/*****************************************************************************
 函 数 名  : Get_Duration_OBIS
 功能描述  : 获取累计时间的OBIS
 输入参数  : Record_Obis 记录的OBIS
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 04 16
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U Get_Duration_OBIS(INT16U Record_Obis)
{
    INT8U i;

    for(i=0;i<ARRAY_SIZE(LIST_RECORD_TIME);i++)
    {
        if(LIST_RECORD_TIME[i].OBIS_1 == Record_Obis)
        {
            return LIST_RECORD_TIME[i].OBIS_2;
        }    
    }    

    return NULL;
}    
/*****************************************************************************
 函 数 名  : Get_Class7_Att7
 功能描述  : 获取当前值记录表
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

//--
    当前值记录表∷=array 当前值
    当前值∷=structure
    {
        事件发生源  instance-specific，
        累计时间及发生次数  structure
        {
          事件发生次数 double-long-unsigned，  
          事件累计时间 double-long-unsigned   
        }
    }
    18字节:01 01 02 02 0(*) 02 02 06 **** 06 ****       
 //--    
*****************************************************************************/
INT8U *Get_Class7_Att7(INT8U *pdata,ST_OAD Oad)
{    
    INT8U *pdst = pdata;

    INT8U i,k;
    INT8U IID;
    
    INT8U Temp_data[32];
   
    INT16U OI;
    INT8U Is_OI_3023;  
    INT16U Obis,Event_Obis; 
    #if ADD_TIME_SET_0

    #else
    const INT8U CONST_ST_2_NULL_ST_2_U32_U32[10] = {structure,2,null_data,0,structure,2,doublelongunsigned,4,doublelongunsigned,4}; 
    #endif
    
    INT8U ST_2_NULL_ST_2_U32_U32[10] = {structure,2,null_data,0,structure,2,doublelongunsigned,4,doublelongunsigned,4}; 
   
    LIB_MemSet(0, Temp_data , 8);	
   
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/
 
	OI  = Oad.Oi.asInt;
	IID = Oad.IId;
	
	Is_OI_3023 = (OI_R_ENGY_COM_PRG_EVENT == OI);  /*无功组合编程事件*/
	
	i=IID;
	k=IID;
    
	if(0 == IID) 
	{		
        *pdst++ = array;
		if(0 == Is_OI_3023)
		{
            *pdst++ = 1;
			i=1;k=1;
		}
		else
		{
            *pdst++ = 2;
			i=1;k=2;
		}
	}

	
	for(;i<=k;i++)
	{		
	    #if ADD_TIME_SET_0

        #else
        LIB_ConstMemCpy(CONST_ST_2_NULL_ST_2_U32_U32, ST_2_NULL_ST_2_U32_U32, sizeof(ST_2_NULL_ST_2_U32_U32));
        #endif
        
        if(0 != Is_OI_3023)
        {    
            ST_2_NULL_ST_2_U32_U32[2] = enum_t;
            ST_2_NULL_ST_2_U32_U32[3] = 1;
        }

        /*2018-03-19无功组合编程的OBIS要特殊处理下，估计 */
        
        /*1.事件发生次数 double-long-unsigned*/
        Event_Obis = Get_Base_Class7_Obis(Oad);

        /*因为是块，所以OBIS要+1*/   /*无功组合编程事件OBIS如何处理*/
        Obis = Event_Obis- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[2].ATT_OBIS_BASE+1;                          

        /*无功组合编程事件有两个OBIS*/
        if((0!=Is_OI_3023)&&(2==i))
        {
            Obis++;
        }    
        
        GetSingle(Obis, Temp_data);

        /*2.事件累计时间 double-long-unsigned 这个OBIS还要再注意下*/

        /*因为是块，所以OBIS要+1*/  /*无功组合编程事件OBIS如何处理*/
        Obis = Get_Duration_OBIS(Event_Obis);//Event_Obis- BASE_CLASS7_OBIS +E_EVNT_DURATION_BLK+1;                          

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
            ST_2_NULL_ST_2_U32_U32[8] = null_data;
            ST_2_NULL_ST_2_U32_U32[9] = 0;
            #endif
        }      

        /*3023第一个数据是源枚举*/
        if(0 != Is_OI_3023)
        {
            LIB_MemCpy(&Temp_data[0], &Temp_data[1], sizeof(INT32U)*2);
            Temp_data[0] = i-1;
        }    
        
        pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_NULL_ST_2_U32_U32, Temp_data, pdst, 0);         
		
	}

    return pdst;
    
}   

/*****************************************************************************
 函 数 名  : Get_Class7_Att10
 功能描述  : 获取时间状态记录表
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
    
    当前值记录表∷=array 当前值
    当前值∷=structure
    {
        事件发生源  instance-specific，
        累计时间及发生次数  structure
        {
             最近一次发生时间 datetime_s，
             最近一次结束时间 datetime_s  
        }
    }
    24字节:01 01 02 02 0(*) 02 02 1c *******(size(7)) 1c *******(size(7))

*****************************************************************************/
INT8U *Get_Class7_Att10(INT8U *pdata,ST_OAD Oad)
{
    INT8U *pdst = pdata;  
    INT8U Temp_data[32];
    INT8U Temp_data_dst[32];
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSS,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;

    INT8U i,k;
    
    INT8U IID;
    INT16U OI;
    INT16U Obis,Event_Obis;
    INT16S Len;
    INT8U Is_OI_3023; 

    const INT8U CONST_ST_2_NULL_ST_2_DATES[10] = {structure,2,null_data,0,structure,2,date_time_s,7,date_time_s,7};
    
    INT8U ST_2_NULL_ST_2_DATES[10];// = {structure,2,null_data,0,structure,2,date_time_s,7,date_time_s,7};

    LIB_MemSet(0, Temp_data , 14);	
   
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/

    OI  = Oad.Oi.asInt;
	IID = Oad.IId;
	
	Is_OI_3023 = (OI_R_ENGY_COM_PRG_EVENT == OI);  /*无功组合编程事件*/
	
	i=IID;
	k=IID;
    
	if(0 == IID) 
	{		
        *pdst++ = array;
		if(0 == Is_OI_3023)
		{
            *pdst++ = 1;
			i=1;k=1;
		}
		else
		{
            *pdst++ = 2;
			i=1;k=2;
		}
	}
	
	for(;i<=k;i++)
	{				
        LIB_ConstMemCpy(CONST_ST_2_NULL_ST_2_DATES, ST_2_NULL_ST_2_DATES, sizeof(ST_2_NULL_ST_2_DATES));
            
        if(0 != Is_OI_3023)
        {    
            ST_2_NULL_ST_2_DATES[2] = enum_t;
            ST_2_NULL_ST_2_DATES[3] = 1;
        }

        /*2018-03-19无功组合的OBIS要特殊处理下，估计 */
        
        /*1.事件发生次数 double-long-unsigned*/
        Event_Obis = Get_Base_Class7_Obis(Oad);

        /*因为是块，所以OBIS要+1*/
        Obis = Event_Obis- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[3].ATT_OBIS_BASE+1;                          

        /*无功组合编程事件有两个OBIS*/
        if((0!=Is_OI_3023)&&(2==i))
        {
            Obis++;
        } 
        Len = GetSingle(Obis, Temp_data);
       
        /*两个空*/
        if(sizeof(ST_6TIME_FORM) > Len)
        {
            //LIB_MemSet(0,(INT8U *) &ST_2_NULL_ST_2_DATES[6], 4);
            ST_2_NULL_ST_2_DATES[6] = null_data;
            ST_2_NULL_ST_2_DATES[7] = 0;
            ST_2_NULL_ST_2_DATES[8] = null_data;
            ST_2_NULL_ST_2_DATES[9] = 0;
            
        }  
        else if(2*sizeof(ST_6TIME_FORM) > Len)/*1个空*/
        {
            //LIB_MemSet(0,(INT8U *) &ST_2_NULL_ST_2_DATES[8], 2);
            ST_2_NULL_ST_2_DATES[8] = null_data;
            ST_2_NULL_ST_2_DATES[9] = 0;
        }   

        FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,Len);

        /*3023第一个数据是源枚举*/
        if(0 != Is_OI_3023)
        {
            LIB_MemCpy(&Temp_data_dst[0], &Temp_data_dst[1], sizeof(INT32U)*2);
            Temp_data_dst[0] = i-1;
        }  
        
        pdst += EnCode_Data((ST_ENCODE_INFO * )ST_2_NULL_ST_2_DATES, Temp_data_dst, pdst, 0); 
        
    }    
    
    return pdst;
}  
/*"************************************************"*/


