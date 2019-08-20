/*"*****************************************************************************"*/
/*"  FileName: class7_comm_event.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ͨ���¼����ļ�   "*/
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
/*"*******************************************************************************"*/
/*7 �������б�*/
const ST_SPECIAL_CLASS_AID_INFO CLASS7_AID_LIST[]=
{
    /*1.�߼���*/
    /* 2.�¼���¼�� */
    {3, READ_WRITE,  0xff,AR_OAD},       /*3.�����������Ա�*/
    {4, READ_ONLY,   0x01,TYPE_U16},     /*4.��ǰ��¼��*/        
    {5, READ_ONLY,   0x01,TYPE_U16},     /*5.����¼��*/
    /*6,���ò���     ���������б�*/
    {7, READ_ONLY,  0x02,AR_ST_2_ENUM_ST_2_U32 },  /*7.��ǰֵ��¼��*/
    {8, READ_WRITE, 0x01,TYPE_ENUM},     /*8.�ϱ���ʶ*/
    {9, READ_ONLY,  0x01,TYPE_BOOL},     /*9.��Ч��ʶ*/    
    {10,READ_ONLY,  0x02,AR_ST_2_NULL_ST_2_DATES}, /*10.ʱ��״̬��¼��*/
};

/*��Ӧ��������Ե�OBIS�б�֮����������ķֿ�д����Ҫ�ǿ��ǵ������ֽڶ���*/
const ST_CLASS7_OBIS_INFO OBIS_BASE_CLASS7_EVENT[]=
{
    //{3,MAX_FORMAT_ID, FUN_NULL, 0x00,E_PWR_DOWN_EVNT_RCRD}, /*3.�����������Ա�*/
    {4,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_CNT},        /*4.��ǰ��¼��*/        
    {5,FORMAT_U2_UBCD2, FUN_NULL, E_INFO_NULL,E_RCRD_MAX_CNT},        /*5.����¼��*/
    /*6,���ò���     ���������б�*/
    {7,FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL,E_RCRD_CUR_VALUE},      /*7.��ǰֵ��¼��*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},                       /*8.�ϱ���ʶ*/
    //{NULL,FORMAT_COMBENGY, FUN_NULL, 0x00},                       /*9.��Ч��ʶ*/    
    {10,FORMAT_DATETIME, FUN_NULL, E_INFO_NULL,E_RCRD_TIME_STATUS},   /*10.ʱ��״̬��¼��*/     
};    

const ST_CLASS7_COMM_EVENT_OI_OBIS CLASS7_OI_RECORD_OBIS[]=
{
    {0x3011, E_PWR_DOWN_EVNT_RCRD},         /*1.���ܱ�����¼�*/ 
    
    {0x3013, E_MTR_CLR_EVNT_RCRD},          /*3.���ܱ������¼�*/ 
    {0x3015, E_EVNT_CLR_EVNT_RCRD},         /*4.���ܱ��¼������¼�*/ 
    {0x3016, E_JUST_TIME_EVNT_RCRD},        /*5.���ܱ�Уʱ�¼�*/ 
    
    {0x301B, E_MTR_COVER_OPEN_EVNT_RCRD},   /*10.���ܱ����¼�*/ 
    
     {0x302B, E_RELAY_ERR_EVNT_RCRD},        /*21.���ܱ��ɿ��������¼�*/ 
    {0x302C, E_PWR_FAILURE_EVNT_RCRD},      /*22.���ܱ��Դ�쳣�¼�*/ 
    /*----�Զ����¼�*/
    {OI_METER_RST_EVENT, E_SYS_RST_RCRD},   /*23.���ܱ�λ�¼�*/ 
    {OI_OSCILLATOR_ERR_EVENT,E_OSCILLATOR_ERR_RCRD}, /*"��������¼�"*/
    
#ifdef THREE_PHASE
    {OI_PD_PON_EVENT,E_PD_PON_RCRD},         /*"���ϵ��¼"*/
    {OI_PARA_ERR_EVENT,E_PARA_ERR_RCRD},     /*"�����쳣��¼"*/
    {OI_ENG_ERR_EVENT,E_ENG_ERR_RCRD},       /*"�����쳣��¼"*/ 
#endif    

    {0x302E, E_RTC_ERR_EVNT_RCRD},           /*37.���ܱ�ʱ�ӹ����¼�	*/ 
    {0x302F, E_MS_ERR_EVNT_RCRD},            /*38.���ܱ����оƬ�����¼�*/ 
//--------------------------------------------------
#ifdef THREE_PHASE
    {0x3009, E_OVER_PA_DEMAND_EVNT_RCRD}, /*23.���ܱ������й����������¼�*/  
    {0x300A, E_OVER_NA_DEMAND_EVNT_RCRD}, /*24.���ܱ����й����������¼�*/ 
    {0x300C, E_FACTOR_LOW_EVNT_RCRD},     /*25.���ܱ��������������¼�*/ 
    {0x300D, E_ALL_LOST_VOLT_EVNT_RCRD},  /*26.���ܱ�ȫʧѹ�¼�*/ 
    {0x300E, E_OBIS_NULL},                /*27.���ܱ�����Դ�����¼�*/ 
    {0x300F, E_VOLT_INVERSE_EVNT_RCRD},   /*28.���ܱ��ѹ�������¼�*/ 
    {0x3010, E_CURR_INVERSE_EVNT_RCRD},   /*29.���ܱ�����������¼�*/ 
    {0x3014, E_DEMAND_CLR_EVNT_RCRD},     /*30.���ܱ����������¼�*/ 
    {0x301C, E_TAIL_COVER_OPEN_EVNT_RCRD}, /*31.���ܱ���ť���¼�*/ 
    {0x301D, E_VOLT_IMBALANCE_EVNT_RCRD}, /*32.���ܱ��ѹ��ƽ���¼�*/ 
    {0x301E, E_CURR_IMBALANCE_EVNT_RCRD}, /*33.���ܱ������ƽ���¼�*/ 
    {0x3023, E_R_ENGY_COM_PRG_EVNT_RCRD}, /*34.���ܱ��޹���Ϸ�ʽ����¼�*/ 
    {0x302A, E_CONSTANT_MGNT_DISTURB_EVNT_RCRD}, /*35.���ܱ�㶨�ų������¼�*/ 
    {0x302D, E_CURR_IMBALANCE_BADLY_EVNT_RCRD}, /*36.���ܱ�������ز�ƽ���¼�	*/ 
#endif 
};    

/*ע:����������ı���һһ��Ӧ�������У�û�У������֣��¼�����:AID=6*/
const ST_Class7_PARAMENT_ObjectList CLASS7_PARAMENT_LIST[]=
{ 
   {0x3011,1,READ_ONLY, ST_1_U8},      /*1.���ܱ�����¼�*/ 
   {0x302E,1,READ_WRITE, ST_1_U8},      /*12.���ܱ�ʱ�ӹ����¼�	*/ 
   {0x302F,1,READ_WRITE, ST_1_U8},      /*13.���ܱ����оƬ�����¼�*/ 
#ifdef THREE_PHASE
   {0x3009,2,READ_WRITE, ST_2_U32_U8},  /*2.���ܱ������й����������¼�*/
   {0x300A,2,READ_WRITE, ST_2_U32_U8},  /*3.���ܱ����й����������¼�*/
   {0x300C,2,READ_WRITE, ST_2_S16_U8},  /*4..���ܱ��������������¼�*/ 
   {0x300E,1,READ_WRITE, ST_1_U8},      /*5..���ܱ�����Դ�����¼�*/ 
   {0x300F,1,READ_WRITE, ST_1_U8},      /*6.���ܱ��ѹ�������¼�*/ 
   {0x3010,1,READ_WRITE, ST_1_U8},      /*7.���ܱ�����������¼�*/ 
   {0x301D,2,READ_WRITE, ST_2_S16_U8},  /*9.���ܱ��ѹ��ƽ���¼�*/ 
   {0x301E,2,READ_WRITE, ST_2_S16_U8},  /*10.���ܱ������ƽ���¼�*/ 
   {0x302D,2,READ_WRITE, ST_2_S16_U8},  /*11.���ܱ�������ز�ƽ���¼�	*/ 
#endif   
};  

/*7�����OBIS��Ӧ�� ��CLASS7_PARAMENT_LIST ��Ӧ��������SEQ ΪOI-0x3000 */
const ST_PARAMENT_OAD_OBIS_INFO CLASS7_PARAMENT_OBIS_LIST[]=
{    
     /*3011 ���ܱ�����¼�*/ 
    {0x11, 0x01,E_OBIS_NULL/*�����⴦��*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*1.���ܱ�����¼�*/  
    /*{0x302E ���ܱ�ʱ�ӹ����¼�*/ 
    {0x2E,0x01,E_CLKERR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},   /*12.���ܱ�ʱ�ӹ����¼�	*/ 
    /*{0x302F ���ܱ����оƬ�����¼�*/ 
    {0x2F,0x01,E_MUERR_TIME_THR, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},              /*13.���ܱ����оƬ�����¼�*/  
#ifdef THREE_PHASE
    /*0x3009  ���ܱ������й����������¼�*/
    {0x09, 0x01,E_AADEMAND_PL_THR/*"�й����������¼�������������"*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},      
    {0x09, 0x01,E_AADEMAND_TIME_THR/*"���������¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      

    /*0x300A ���ܱ����й����������¼�*/
    {0x0A, 0x01,E_NADEMAND_PL_THR/*"�й����������¼�������������"*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, 
    {0x0A, 0x01,E_NADEMAND_TIME_THR/*"���������¼��ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL}, 

    /*0x300C ���ܱ��������������¼�*/
    {0x0C, 0x01,E_OVERFACTOR_RATE_THR/*"�ܹ������������޷�ֵ"*/, 0, FORMAT_S2_1_UBCD2_1, FUN_NULL, E_INFO_NULL},      /*4.���ܱ��������������¼�*/   
    {0x0C, 0x01,E_OVERFACTOR_TIME_THR/*"�ܹ��������������ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      

    /*0x300E ���ܱ�����Դ�����¼�*/
    {0x0E, 0x01,E_OBIS_NULL/*��OBIS*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*5.���ܱ�����Դ�����¼�*/   

    /*{0x300F ���ܱ��ѹ�������¼� */
    {0x0F, 0x01,E_VOLTREVERSE_TIME_THR/*"��ѹ�������¼��ж�ʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*6.���ܱ��ѹ�������¼�*/   

    /*{0x3010 ���ܱ�����������¼�*/
    {0x10, 0x01,E_CURRREVERSE_TIME_THR/*"�����������¼��ж�ʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*7.���ܱ�����������¼�*/   

    /*{0x301D ���ܱ��ѹ��ƽ���¼� */
    {0x1D, 0x01,E_VOLTIMB_RATE_THR/*"��ѹ��ƽ������ֵ"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},      /*9.���ܱ��ѹ��ƽ���¼�*/    
    {0x1D, 0x01,E_VOLTIMB_TIME_THR/*"��ѹ��ƽ�����ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},      /*9.���ܱ��ѹ��ƽ���¼�*/    

    /*{0x301E ���ܱ������ƽ���¼�*/ 
    {0x1E,0x01,E_CURRIMB_RATE_THR/*"������ƽ������ֵ"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},     /*10.���ܱ������ƽ���¼�*/ 
    {0x1E,0x01,E_CURRIMB_TIME_THR/*"������ƽ�����ж���ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     /*10.���ܱ������ƽ���¼�*/ 

    /*{0x302D ���ܱ�������ز�ƽ���¼�	*/ 
    {0x2D,0x01,E_CURRBADIMB_RATE_THR/*"�������ز�ƽ����ֵ"*/, 0, FORMAT_S2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},     /*11.���ܱ�������ز�ƽ���¼�	*/ 
    {0x2D,0x01,E_CURRBADIMB_TIME_THR/*"�������ز�ƽ�ⴥ����ʱʱ��"*/, 0, FORMAT_U1_UBCD2, FUN_NULL, E_INFO_NULL},     /*11.���ܱ�������ز�ƽ���¼�	*/ 

#endif
};
/*****************************************************************************
 �� �� ��  : Get_Base_Class7_Obis
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
 �� �� ��  : get_class7_Obis
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
INT8U get_class7_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U  AID,i;
    INT16U OBIS_EVENT;    /*�¼�OBIS*/

    /*����*/
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
       if((OI_CARD_PLUG_IN_ERR_EVENT == pObject->Oad.Oi.asInt)&&(0x0b==AID))   /*�쳣�忨����*/
       {
           pObisInfo_bak->Obis = E_UNLAW_PLUG_CNT;
           pObisInfo_bak->num  = 1;  
       } 
       else 
#endif        
        if((EN_CLS7_ATT_2_EVENT_RECORD_TABLE== AID)||(EN_CLS7_ATT_3_RELATION_OBJECT_TABLE== AID))  /*��������ͼ�¼��*/
       {           
           pObisInfo_bak->Obis = OBIS_EVENT;
           pObisInfo_bak->num = 1;
       }
       else
       { 
           /*2.�õ�����OBIS,Ȼ������¼���¼OBIS���������ֵ*/
           for(i=0;i<ARRAY_SIZE(OBIS_BASE_CLASS7_EVENT);i++)
           {
                if(AID == OBIS_BASE_CLASS7_EVENT[i].Aid)
                {   
                    /*��1����Ϊ�ǿ�*/
                    pObisInfo_bak->Obis     = (OBIS_EVENT- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[i].ATT_OBIS_BASE)+1; 
                    pObisInfo_bak->FormatId = OBIS_BASE_CLASS7_EVENT[i].FormatId;
                    break;
                }              
           }
           /*��ǰֵ��¼��������OBIS*/
           if(EN_CLS7_ATT_7_CURRENT_RECORD_TABLE!=AID)
           { 
              pObisInfo_bak->num = 1;      /*7�࣬�̶�1*/
           }
           else
           {
              pObisInfo_bak->num = 2;
              pObisInfo_bak[1].Obis = (OBIS_EVENT- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[i].ATT_OBIS_BASE)+1;    
           }  

            //pObisInfo_bak->Offset = 0;

            /*��ʾ��ʽ*/
            //LIB_MemCpy((INT8U*)&OBIS_BASE_CLASS7_EVENT[i].FormatId, &pObisInfo_bak->FormatId, 3);
           
       }             
              
       return 1;
    }   
    
}


/*"************************************************"*/
/*7���¼�Ԫ�ز���*/
INT8U get_class7_object(SUB_OBJECT *pObject)
{
    INT8U i,AID,IID;
    INT16U OI;
    const ST_SPECIAL_CLASS_AID_INFO *pCLASS7_AID_LIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;   
    

    /*1.����OI*/
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
    

    if(EN_CLS7_ATT_2_EVENT_RECORD_TABLE >= AID)  /*�޼������¼���¼��*/
    {   
        return DAR_0_OK;
    }    

    /*2.���Ҳ���AID*/
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
    else if((OI_CARD_PLUG_IN_ERR_EVENT == OI)&&(0x0b==AID))   /*�쳣�忨����*/
    {
       pObject->Pointer   = TYPE_U32;
       pObject->Right_RW  = READ_ONLY; 
       return DAR_0_OK;
    }  

    /*3.������������AID*/    
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
 �� �� ��  : PP_Obis_Oad_class7
 ��������  : class7 obisתOAD
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

/*�ۼ�ʱ��ͼ�¼�����OBIS�б�*/
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
 �� �� ��  : Get_Duration_OBIS
 ��������  : ��ȡ�ۼ�ʱ���OBIS
 �������  : Record_Obis ��¼��OBIS
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 04 16
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
 �� �� ��  : Get_Class7_Att7
 ��������  : ��ȡ��ǰֵ��¼��
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

//--
    ��ǰֵ��¼���=array ��ǰֵ
    ��ǰֵ��=structure
    {
        �¼�����Դ  instance-specific��
        �ۼ�ʱ�估��������  structure
        {
          �¼��������� double-long-unsigned��  
          �¼��ۼ�ʱ�� double-long-unsigned   
        }
    }
    18�ֽ�:01 01 02 02 0(*) 02 02 06 **** 06 ****       
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
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/
 
	OI  = Oad.Oi.asInt;
	IID = Oad.IId;
	
	Is_OI_3023 = (OI_R_ENGY_COM_PRG_EVENT == OI);  /*�޹���ϱ���¼�*/
	
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

        /*2018-03-19�޹���ϱ�̵�OBISҪ���⴦���£����� */
        
        /*1.�¼��������� double-long-unsigned*/
        Event_Obis = Get_Base_Class7_Obis(Oad);

        /*��Ϊ�ǿ飬����OBISҪ+1*/   /*�޹���ϱ���¼�OBIS��δ���*/
        Obis = Event_Obis- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[2].ATT_OBIS_BASE+1;                          

        /*�޹���ϱ���¼�������OBIS*/
        if((0!=Is_OI_3023)&&(2==i))
        {
            Obis++;
        }    
        
        GetSingle(Obis, Temp_data);

        /*2.�¼��ۼ�ʱ�� double-long-unsigned ���OBIS��Ҫ��ע����*/

        /*��Ϊ�ǿ飬����OBISҪ+1*/  /*�޹���ϱ���¼�OBIS��δ���*/
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

        /*3023��һ��������Դö��*/
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
 �� �� ��  : Get_Class7_Att10
 ��������  : ��ȡʱ��״̬��¼��
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
    
    ��ǰֵ��¼���=array ��ǰֵ
    ��ǰֵ��=structure
    {
        �¼�����Դ  instance-specific��
        �ۼ�ʱ�估��������  structure
        {
             ���һ�η���ʱ�� datetime_s��
             ���һ�ν���ʱ�� datetime_s  
        }
    }
    24�ֽ�:01 01 02 02 0(*) 02 02 1c *******(size(7)) 1c *******(size(7))

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
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    OI  = Oad.Oi.asInt;
	IID = Oad.IId;
	
	Is_OI_3023 = (OI_R_ENGY_COM_PRG_EVENT == OI);  /*�޹���ϱ���¼�*/
	
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

        /*2018-03-19�޹���ϵ�OBISҪ���⴦���£����� */
        
        /*1.�¼��������� double-long-unsigned*/
        Event_Obis = Get_Base_Class7_Obis(Oad);

        /*��Ϊ�ǿ飬����OBISҪ+1*/
        Obis = Event_Obis- BASE_CLASS7_OBIS +OBIS_BASE_CLASS7_EVENT[3].ATT_OBIS_BASE+1;                          

        /*�޹���ϱ���¼�������OBIS*/
        if((0!=Is_OI_3023)&&(2==i))
        {
            Obis++;
        } 
        Len = GetSingle(Obis, Temp_data);
       
        /*������*/
        if(sizeof(ST_6TIME_FORM) > Len)
        {
            //LIB_MemSet(0,(INT8U *) &ST_2_NULL_ST_2_DATES[6], 4);
            ST_2_NULL_ST_2_DATES[6] = null_data;
            ST_2_NULL_ST_2_DATES[7] = 0;
            ST_2_NULL_ST_2_DATES[8] = null_data;
            ST_2_NULL_ST_2_DATES[9] = 0;
            
        }  
        else if(2*sizeof(ST_6TIME_FORM) > Len)/*1����*/
        {
            //LIB_MemSet(0,(INT8U *) &ST_2_NULL_ST_2_DATES[8], 2);
            ST_2_NULL_ST_2_DATES[8] = null_data;
            ST_2_NULL_ST_2_DATES[9] = 0;
        }   

        FRMT_NdataChange(Temp_data,Temp_data_dst,inFormat,outFormat,Len);

        /*3023��һ��������Դö��*/
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


