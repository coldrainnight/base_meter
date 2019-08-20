/*"*****************************************************************************"*/
/*"  FileName: class24_phase_event.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �����¼��ӿ����ļ�   "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h" 
#include "App\Message\MS.h"
/*"************************************************"*/
/*24�������б�*/
const ST_CLASS24_AID_INFO CLASS24_AID_LIST[]=
{
    /*,�߼���*/
    {2, READ_WRITE,  0xff,AR_OAD},       /*�����������Ա�*/
    {3, READ_ONLY,   0x04,ST_4_U16},     /*��ǰ��¼��*/        
    {4, READ_ONLY,   0x01,TYPE_U16},     /*����¼��*/
    /*5,���ò���     ���������б�*/
    /*6,�¼���¼��1*/
    /*7,�¼���¼��2*/
    /*8,�¼���¼��3*/
    /*9,�¼���¼��4*/
    {10, READ_ONLY,  0x04,AR_ST_2_U32},   /*��ǰֵ��¼��*/
    {11, READ_WRITE, 0x01,TYPE_ENUM},     /*�ϱ���ʶ*/
    {12, READ_ONLY,  0x01,TYPE_BOOL},     /*��Ч��ʶ*/
    
    #ifdef THREE_PHASE
    {13, READ_ONLY,  0x04,ST_4_U32_U32_DATE_S_DATE_S},  /*ʧѹͳ��*/
    #endif
    
    {14, READ_ONLY,  0xff,AR_ST_2_DATES}, /*ʱ��״̬��¼��*/
};

/*��Ӧ��������Ե�OBIS�б�֮����������ķֿ�д����Ҫ�ǿ��ǵ������ֽڶ���*/
const ST_CLASS24_OBIS_INFO OBIS_BASE_CLASS24_EVENT[]=
{
    //{2,MAX_FORMAT_ID, FUN_NULL, 0x00,E_OVER_CURR_LA_EVNT_RCRD},      /*�����������Ա�*/
    {3,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_CNT},                /*��ǰ��¼��*/        
    {4,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_MAX_CNT},                /*����¼��*/
    /*5,���ò���     ���������б�*/
    /*6,�¼���¼��1*/
    /*7,�¼���¼��2*/
    /*8,�¼���¼��3*/
    /*9,�¼���¼��4*/
    {10,FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_VALUE},   /*��ǰֵ��¼��*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},     /*�ϱ���ʶ*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},     /*��Ч��ʶ*/
    
    {14,FORMAT_DATETIME, FUN_NULL, E_INFO_NULL,E_RCRD_TIME_STATUS}, /*ʱ��״̬��¼��*/
};   

#ifdef THREE_PHASE
const ST_CLASS24_PHASE_EVENT_OI_OBIS CLASS_24_OI_OBIS[]=
{
    {0x3000, E_OBIS_NULL,E_LOST_VOLT_LA_EVNT_RCRD,E_LOST_VOLT_LB_EVNT_RCRD,E_LOST_VOLT_LC_EVNT_RCRD}, /*ʧѹ�¼�*/  
    {0x3001, E_OBIS_NULL,E_LACK_VOLT_LA_EVNT_RCRD,E_LACK_VOLT_LB_EVNT_RCRD,E_LACK_VOLT_LC_EVNT_RCRD}, /*Ƿѹ�¼�*/ 
    {0x3002, E_OBIS_NULL,E_OVER_VOLT_LA_EVNT_RCRD,E_OVER_VOLT_LB_EVNT_RCRD,E_OVER_VOLT_LC_EVNT_RCRD}, /*��ѹ�¼�*/ 
    {0x3003, E_OBIS_NULL,E_BRK_PHASE_LA_EVNT_RCRD,E_BRK_PHASE_LB_EVNT_RCRD,E_BRK_PHASE_LC_EVNT_RCRD}, /*�����¼�*/ 
    {0x3004, E_OBIS_NULL,E_LOST_CURR_LA_EVNT_RCRD,E_LOST_CURR_LB_EVNT_RCRD,E_LOST_CURR_LC_EVNT_RCRD}, /*ʧ���¼�*/ 
    {0x3005, E_OBIS_NULL,E_OVER_CURR_LA_EVNT_RCRD,E_OVER_CURR_LB_EVNT_RCRD,E_OVER_CURR_LC_EVNT_RCRD}, /*�����¼�*/ 
    {0x3006, E_OBIS_NULL,E_BRK_CURR_LA_EVNT_RCRD,E_BRK_CURR_LB_EVNT_RCRD,E_BRK_CURR_LC_EVNT_RCRD},    /*�����¼�*/ 
    {0x3008, E_OBIS_NULL,E_OVER_LOAD_LA_EVNT_RCRD,E_OVER_LOAD_LB_EVNT_RCRD,E_OVER_LOAD_LC_EVNT_RCRD}, /*���ʷ����¼�*/ 
    /*--���������¼���4��OBIS--*/
    {0x3007, E_PWR_REVERSE_EVNT_RCRD,E_PWR_REVERSE_LA_EVNT_RCRD,E_PWR_REVERSE_LB_EVNT_RCRD,E_PWR_REVERSE_LC_EVNT_RCRD}, /*�����¼�*/ 
    {0x300B, E_OVER_R_DEMAND_1_EVNT_RCRD,E_OVER_R_DEMAND_2_EVNT_RCRD,E_OVER_R_DEMAND_3_EVNT_RCRD,E_OVER_R_DEMAND_4_EVNT_RCRD} /*���ܱ��޹����������¼�*/ 
};

/*ע:����������ı�һһ��Ӧ���¼�����:AID=5*/
const ST_Class24_PARAMENT_ObjectList CLASS24_PARAMENT_LIST[]=
{
   {4,READ_WRITE, ST_4_U16_U16_S32_U8},   /*3000 ʧѹ�¼�*/  
   {2,READ_WRITE, ST_2_U16_U8},           /*3001 Ƿѹ�¼�*/ 
   {2,READ_WRITE, ST_2_U16_U8},           /*3002 ��ѹ�¼�*/ 
   {3,READ_WRITE, ST_3_U16_S32_U8},       /*3003 �����¼�*/ 
   {4,READ_WRITE, ST_4_U16_S32_S32_U8},   /*3004 ʧ���¼�*/ 
   {2,READ_WRITE, ST_2_S32_U8},           /*3005 �����¼�*/ 
   {3,READ_WRITE, ST_3_U16_S32_U8},       /*3006 �����¼�*/     
   {2,READ_WRITE, ST_2_S32_U8},           /*3007 �����¼�*/ 
   {2,READ_WRITE, ST_2_S32_U8},           /*3008 ���ʷ����¼�*/ 
   {2,READ_WRITE, ST_2_U32_U8}            /*300B���ܱ��޹����������¼�*/ 
};  

/*24�����OBIS��Ӧ�� AID=5   �����Seq_Num = OI -0x3000*/
const ST_PARAMENT_OAD_OBIS_INFO CLASS24_PARAMENT_OBIS_LIST[]=
{    
     /*3000 ʧѹ�¼�*/  
    {0, 0x01,E_LOSTVOLT_UU_THR/*"ʧѹ�¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {0, 0x01,E_LOSTVOLT_UL_THR/*"ʧѹ�¼���ѹ�ָ�����"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},      
    {0, 0x01,E_LOSTVOLT_IL_THR/*"ʧѹ�¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {0, 0x01,E_LOSTVOLT_TIME_THR/*"ʧѹ�¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3001 Ƿѹ�¼�*/ 
    {1, 0x01,E_UNDERVOLT_UU_THR/*"Ƿѹ�¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {1, 0x01,E_UNDERVOLT_TIME_THR/*"Ƿѹ�¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3002 ��ѹ�¼�*/ 
    {2, 0x01,E_OVERVOLT_UL_THR/*"��ѹ�¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {2, 0x01,E_OVERVOLT_TIME_THR/*"��ѹ�¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3003 �����¼�*/ 
    {3, 0x01,E_BRKPHASE_UU_THR/*"�����¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {3, 0x01,E_BRKPHASE_IU_THR/*"�����¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},       
    {3, 0x01,E_BRKPHASE_TIME_THR/*"�����¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3004 ʧ���¼�*/ 
    {4, 0x01,E_LOSTCURR_UL_THR/*"ʧ���¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {4, 0x01,E_LOSTCURR_IU_THR/*"ʧ���¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {4, 0x01,E_LOSTCURR_IL_THR/*"ʧ���¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},      
    {4, 0x01,E_LOSTCURR_TIME_THR/*"ʧ���¼��ж���ʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     

    /*3005 �����¼�*/ 
    {5, 0x01,E_OVERCURR_IL_THR/*"�����¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},       
    {5, 0x01,E_OVERCURR_TIME_THR/*"�����¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3006 �����¼�*/   
    {6, 0x01,E_FAILCURR_UL_THR/*"�����¼���ѹ��������"*/, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},       
    {6, 0x01,E_FAILCURR_IU_THR/*"�����¼�������������"*/, 0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},         
    {6, 0x01,E_FAILCURR_TIME_THR/*"�����¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

    /*3007 ���ʷ����¼�*/
    {7, 0x01,E_PHSPWRREVERSE_PL_THR/*"���ʷ����¼��й����ʴ�������"*/, 0, FORMAT_PW, FUN_NULL, E_INFO_NULL},       
    {7, 0x01,E_PHSPWRREVERSE_TIME_THR/*"���ʷ����¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       

     /*3008 �����¼�*/ 
    {8, 0x01,E_UPLOAD_PL_THR/*"�����¼��й����ʴ�������"*/, 0, FORMAT_PW, FUN_NULL, E_INFO_NULL},       
    {8, 0x01,E_UPLOAD_TIME_THR/*"�����¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},       


    /*300B ���ܱ��޹����������¼�  OBIS��û��*/
    {0x0B,0x01,E_RDEMAND_PL_THR, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        
    {0x0B,0x01,E_RDEMAND_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},

     /*ʧѹ�¼�������13��Ҳ��Ϊ�α�������ȡOBIS ע�����к�ΪEN_CLS24_ATT_13_LOST_V_STATIC*/
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LOST_VOLT_SUM_CNTS, 0, FORMAT_U4_UBCD4, FUN_NULL, E_SUM_LOST_V_COUNTS},        
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,E_TOTAL_LOSTVOLT_TIME, 0, FORMAT_U4_UBCD4, FUN_NULL, E_SUM_LOST_V_TIME},
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LAST_LOST_VOLT_ST_TM, 0, FORMAT_DATETIME, FUN_NULL, E_LAST_LOST_V_TIME},        
    {EN_CLS24_ATT_13_LOST_V_STATIC,0x01,LAST_LOST_VOLT_END_TM, 0, FORMAT_DATETIME, FUN_NULL, E_LAST_LOST_V_TIME},

     
};

#else
const ST_CLASS24_PHASE_EVENT_OI_OBIS CLASS_24_OI_OBIS[]=
{
    {0x3005, E_OBIS_NULL,E_OBIS_NULL,E_OBIS_NULL,E_OBIS_NULL}, /*�����¼� E_OVER_CURR_LA_EVNT_RCRD*/ 
};

/*ע:����������ı�һһ��Ӧ*/
const ST_Class24_PARAMENT_ObjectList CLASS24_PARAMENT_LIST[]=
{
    {2,READ_WRITE, ST_2_S32_U8}           /*3005 �����¼�*/ 
};

/*24�����OBIS��Ӧ��*/
const ST_PARAMENT_OAD_OBIS_INFO CLASS24_PARAMENT_OBIS_LIST[]=
{
    {5,0x01,E_OVERCURR_IL_THR,   0, FORMAT_A_4, FUN_NULL, E_INFO_NULL},            /*3005 �����¼� ������������*/ 
    {5,0x01,E_OVERCURR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL}       /*3005 �����¼� �ж���ʱʱ��*/ 
};

#endif
/*****************************************************************************
 �� �� ��  : Get_Base_Class24_Obis
 ��������  : �����OBISֵ
 �������  : ST_OAD Oad  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
          /*���ʷ����¼�����,�޹����������е�һ����*/
          if((OI_PWR_REVERSE_EVENT == Oi)||(OI_OVER_R_DEMAND_EVENT == Oi))  
          {  
             OBIS_EVENT = CLASS_24_OI_OBIS[i].OBIS_1;   /*�¼���¼��ȡ��һ��ֵ*/
          }  
          else
          #endif  
          {
             OBIS_EVENT = CLASS_24_OI_OBIS[i].OBIS_2;   /*�¼���¼��ȡ��һ��ֵ*/
          } 
          
          *pSeqNum = i;
          break;
       } 
   }  

   return OBIS_EVENT;
    
}    

/*****************************************************************************
 �� �� ��  : get_class24_Obis
 ��������  : ����7�����ݵ�OBIS
 �������  : SUB_OBJECT *pObject                     
             ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 11
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U get_class24_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U  AID,i;
    INT16U OBIS_EVENT;    /*�¼�OBIS*/

    AID= pObject->Oad.AId;
    /*����*/
    if((PARAMENT_AID_CLASS24 == AID)||(EN_CLS24_ATT_13_LOST_V_STATIC == AID))
    {
        return get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                             (const ST_PARAMENT_OAD_OBIS_INFO *)CLASS24_PARAMENT_OBIS_LIST,ARRAY_SIZE(CLASS24_PARAMENT_OBIS_LIST));
    }   
    else
    {
       /*1.�����¼���OBIS*/ 
       //Oi = pObject->Oad.Oi.asInt;
       
       OBIS_EVENT = Get_Base_Class24_Obis(pObject->Oad,&i);       

       if(2== AID)   /*���������ü�*/
       {
          pObisInfo_bak->Obis = OBIS_EVENT;
          pObisInfo_bak->num = 1;
       }          
       else if((6<= AID)&&(9>= AID))  /*�¼���¼��*/
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
       else /*������3��4��10��14��*/
       { 
           /*2.�õ�����OBIS,Ȼ������¼���¼OBIS���������ֵ*/
           for(i=0;i<ARRAY_SIZE(OBIS_BASE_CLASS24_EVENT);i++)
           {
                if(AID == OBIS_BASE_CLASS24_EVENT[i].Aid)
                {   
                    /*��1����Ϊ�ǿ�*/
                    pObisInfo_bak->Obis = (OBIS_EVENT- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[i].ATT_OBIS_BASE)+1;                       
                    pObisInfo_bak->FormatId = OBIS_BASE_CLASS24_EVENT[i].FormatId;
                    break;
                }              
           }

           /*����¼��*/   /*24�࣬�̶�1*/
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
                    pObisInfo_bak->Obis = pObisInfo_bak->Obis-1;  /*ֻ��ABC��û����*/
                 }    
              }  
              pObisInfo_bak->num = 4;      /*��Ҫ��������ʾ����*/
           }             
       }            

       return 1;
    }   

    
}


/*"************************************************"*/
/*24���¼�Ԫ�ز���*/
INT8U get_class24_object(SUB_OBJECT *pObject)
{
    INT8U i,AID,IID;
    INT16U OI;
    const ST_CLASS24_AID_INFO *pCLASS24_AID_LIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId; 

    /*1.����OI*/
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
    
    if((1 == AID) ||((6<= AID)&&(9>= AID))) /*�߼������¼���¼��*/
    {    
        return DAR_0_OK;
    }        

    /*2.���Ҳ���AID = 5*/
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

    /*3.������������AID*/    
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

    #ifdef THREE_PHASE /*ֻ��ʧѹ���������*/
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
 �� �� ��  : PP_Obis_Oad_class24
 ��������  : class24 obisתOAD
 �������  : INT16U Obis   
             ST_OAD *pOad  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 01 02
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
 �� �� ��  : Get_Class24_Att3
 ��������  : ��ǰ��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

��ǰ��¼����=structure
{
  ��¼��1��ǰ��¼�� long-unsigned��  ----null
  ��¼��2��ǰ��¼�� long-unsigned��
  ��¼��3��ǰ��¼�� long-unsigned��  ----null
  ��¼��4��ǰ��¼�� long-unsigned��  ----null
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
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    pdst += EnCode_Data((ST_ENCODE_INFO * )ST_4_U16_NULL_3, Temp_data, pdst, Oad.IId);
    
    return pdst;
} 
/*****************************************************************************
 �� �� ��  : Get_Class24_Att10
 ��������  : 24�൱ǰֵ��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

    
��ǰֵ��¼���=array structure
{
    �¼��������� double-long-unsigned��
    �¼��ۼ�ʱ�� double-long-unsigned��
}

*****************************************************************************/
INT8U *Get_Class24_Att10(INT8U *pdata,ST_OAD Oad)
{
    /*��һ�����ͣ��ȶ���Ϊstructe����ҪΪ�˺õ��ñ���ģ�飬Ȼ�������IID=0���ٸ�Ϊarray*/
    const INT8U AR_Class24_Att10[]  = {structure,0x04,null_data,0,structure,2,doublelongunsigned,2,doublelongunsigned,2,null_data,0,null_data,0,};

    INT8U *pdst = pdata;
    INT8U Temp_data[32]={0};
    INT8U i;

    INT16U Obis,Event_Obis;

    /*1.�¼���������*/
    Event_Obis = Get_Base_Class24_Obis(Oad,&i);
    
    Obis = Event_Obis- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[2].ATT_OBIS_BASE+1;                          
    
    GetSingle(Obis, Temp_data);

    /*�����24�ֻ࣬��һ����������������ֱ��ȡ�ˣ��������ǰ��Ӹ���ȡOBIS���㷨����*/ 
    GetSingle(E_OVER_CURR_LA_DURATION, &Temp_data[4]);
    
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    pdst += EnCode_Data((ST_ENCODE_INFO * )AR_Class24_Att10, Temp_data, pdst, Oad.IId);

    /*���IID=0����ĳ�array*/
    if(0 == Oad.IId)
    {
       *(pdata+sizeof(ST_OAD)+1) = array;
    }    
    
    return pdst;
}  


/*****************************************************************************
 �� �� ��  : Get_Class24_Att14
 ��������  : 24��ʱ��״̬��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

ʱ��״̬��¼���=array structure
{
    ���һ�η���ʱ�� datetime_s��
    ���һ�ν���ʱ�� datetime_s
}

*****************************************************************************/
INT8U *Get_Class24_Att14(INT8U *pdata,ST_OAD Oad)
{
    /*�������ĳ�RAM���ݣ��Ա�������Ϊ�� ,��һ���ֽڹ���Ϊstruct���Ա���ñ���ӿ�*/
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

    /*������*/
    if(sizeof(ST_6TIME_FORM) > Len)
    {
        //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[6], 4);
        AR_Class24_Att14[6] = null_data;
        AR_Class24_Att14[7] = 0;
        AR_Class24_Att14[8] = null_data;
        AR_Class24_Att14[9] = 0;
    }  
    else if(2*sizeof(ST_6TIME_FORM) > Len)/*1����*/
    {
        //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[8], 2);
        AR_Class24_Att14[8] = null_data;
        AR_Class24_Att14[9] = 0;
    }    

    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,Len);

    pdst += EnCode_Data((ST_ENCODE_INFO * )AR_Class24_Att14, Temp_data_dst, pdst, Oad.IId);

    /*���IID=0����ĳ�array*/
    if(0 == Oad.IId)
    {
       *(pdata+sizeof(ST_OAD)+1) = array;
    }    
    
    return pdst;
} 

#else



/*****************************************************************************
 �� �� ��  : Get_F_SUM_F_B_PHASE
 ��������  : ȷ���Ƿ����ܺ�B��
 �������  : ST_OAD Oad        
             INT8U i           
             INT8U j           
             INT8U F_SUM       
             INT8U F_B_PHNASE  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 04 16
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void Get_F_SUM_F_B_PHASE(ST_OAD *Oad,INT8U *i,INT8U *j,INT8U *F_SUM,INT8U *F_B_PHASE)
{
    INT8U IID,F_331 = 0;
    INT16U Oi,UiState;

    GetSingle(E_MU_STATUS, (INT8U *)&UiState);
    F_331 = (UiState&0x01);   /*"bitֵΪ0��ʾ3P4W��Ϊ1��ʾ3P3W"*/

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
    
    if((OI_PWR_REVERSE_EVENT == Oi)||(OI_OVER_R_DEMAND_EVENT == Oi))  /*���ʷ����¼�����,�޹����������е�һ����*/
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
       /*�޹���������*/
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
 �� �� ��  : Get_Class24_Att3
 ��������  : ��ǰ��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/ 
/*�����������������ABC��ABC�������331����û��B��*/
INT8U *Get_Class24_Att3(INT8U *pdata,ST_OAD Oad)
{    
    INT8U *pdst = pdata;
    INT8U Temp_data[32];
    INT8U i,j,k;
    INT16U Obis,OBIS_BASE;
    INT8U F_SUM,F_B_PHASE;  /*�ܺ�B���־:������0��ʾ��*/

    /*1.��1����ȷ���ܺ�B���Ƿ�����*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);   
    
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    if(0 == Oad.IId)
    {
       *pdst++ = structure;
       *pdst++ = 4;
    } 
    /*2018-03-26�����ȡ���ݣ�Ҫ��ײ������ã���λ�ȡ���ݣ���IID=0��IID����0������ײ���λ�����*/
    
    OBIS_BASE = Get_Base_Class24_Obis(Oad,&k);
    OBIS_BASE = OBIS_BASE- BASE_CLASS24_OBIS +OBIS_BASE_CLASS24_EVENT[0].ATT_OBIS_BASE+1;                          

    for(;i<=j;i++)
    {    
    
        if(((0 ==i)&&(0 == F_SUM))||((2==i)&&(0 == F_B_PHASE)))
        {           
            pdst += EnCode_Data((ST_ENCODE_INFO * )TYPE_NULL, Temp_data, pdst, 0);
            continue;   /*ֱ�ӷ������ˣ����OBIS���ԣ�ȡ�������쳣��*/
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
 �� �� ��  : Get_Class24_Att10
 ��������  : 24�൱ǰֵ��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

    
��ǰֵ��¼���=array structure
{
    �¼��������� double-long-unsigned��
    �¼��ۼ�ʱ�� double-long-unsigned��
}
*****************************************************************************/
/*�����������������ABC��ABC�������331����û��B��*/
INT8U *Get_Class24_Att10(INT8U *pdata,ST_OAD Oad)
{
    /*��һ�����ͣ��ȶ���Ϊstructe����ҪΪ�˺õ��ñ���ģ�飬Ȼ�������IID=0���ٸ�Ϊarray*/

    #if ADD_TIME_SET_0

    #else
    const INT8U CONST_ST_2_U32_U32[]  ={structure,2,doublelongunsigned,2,doublelongunsigned,2};
    #endif
    
    INT8U ST_2_U32_U32[]  ={structure,2,doublelongunsigned,2,doublelongunsigned,2};

    INT8U *pdst = pdata;
    INT8U Temp_data[32]={0}; 

    INT16U Obis,Event_Obis;
    INT8U i,j,k; 
    INT8U F_SUM,F_B_PHASE;  /*�ܺ�B���־:������0��ʾ��*/ 

    /*1.��1����ȷ���ܺ�B���Ƿ�����*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);
    
    pdst = EnCodeOAD(&Oad, pdst);

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

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
            continue;   /*ֱ�ӷ������ˣ����OBIS���ԣ�ȡ�������쳣��*/
        } 

        
        if(0 == F_SUM)
        {
           Obis = Event_Obis+i-1; 
        }  
        else
        {
           Obis = Event_Obis+i;
        }   

        /*��������*/
        GetSingle(Obis, Temp_data);

        /*Ҫת��һ��*/
        /*�ۼ�ʱ��*/   /*���ۼ�ʱ��OBISҪ-1����ΪLIST_RECORD_TIME���õ��Ǽ�¼OBIS�б����ü�1*/
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
 �� �� ��  : Get_Class24_Att14
 ��������  : 24��ʱ��״̬��¼��
 �������  : INT8U *pdata  
             ST_OAD Oad    
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

ʱ��״̬��¼���=array structure
{
    ���һ�η���ʱ�� datetime_s��
    ���һ�ν���ʱ�� datetime_s
}

*****************************************************************************/
/*�����������������ABC��ABC�������331����û��B��*/
INT8U *Get_Class24_Att14(INT8U *pdata,ST_OAD Oad)
{
    /*�������ĳ�RAM���ݣ��Ա�������Ϊ�� ,��һ���ֽڹ���Ϊstruct���Ա���ñ���ӿ�*/

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
    INT8U F_SUM,F_B_PHASE;  /*�ܺ�B���־:������0��ʾ��*/

    /*1.��1����ȷ���ܺ�B���Ƿ�����*/    
    Get_F_SUM_F_B_PHASE(&Oad,&i,&j,&F_SUM,&F_B_PHASE);
    
    pdst = EnCodeOAD(&Oad, pdst);

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

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
            continue;   /*ֱ�ӷ������ˣ����OBIS���ԣ�ȡ�������쳣��*/
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
        
        /*������*/
        if(sizeof(ST_6TIME_FORM) > Len)
        {
            //LIB_MemSet(0,(INT8U *) &AR_Class24_Att14[6], 4);
            ST_2_DATES[2] = null_data;
            ST_2_DATES[3] = 0;
            ST_2_DATES[4] = null_data;
            ST_2_DATES[5] = 0;
        }  
        else if(2*sizeof(ST_6TIME_FORM) > Len)/*1����*/
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
 �� �� ��  : ʧѹͳ�ƴ��
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 25
    ��    ��   : xyt
    �޸�����   : �����ɺ���

����13��ʧѹͳ�ƣ���=structure
{
�¼������ܴ���   double-long-unsigned��
�¼����ۼ�ʱ��   double-long-unsigned����λ���룬�޻��㣩��
���һ��ʧѹ����ʱ�� date_time_s��
���һ��ʧѹ����ʱ�� date_time_s
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

    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

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

