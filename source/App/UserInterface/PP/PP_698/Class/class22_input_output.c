/*"*****************************************************************************"*/
/*"  FileName: class22_input_output.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ��������ӿ����ļ�   "*/
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
#include "APP\message\MS.h"
#include "App\UserInterface\UI.h"
#include "App\UserInterface\UI_dataProcess.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
/*"************************************************"*/
/*22�������������2�б� �豸�����б�*/
const ST_Class22_ObjectList CLASS22OBISLIST_AID_2[]=
{	/*ע����ָ�룬û�м����飬�������ݶ������������͵ģ��������IID=0,��Ҫ������*/
    #ifdef THREE_PHASE
	{0xF201,0x02,1,AR_ST_3_VIS_5_COMDCB_EN},  /*RS485*/
	#else
    {0xF201,0x01,1,AR_ST_3_VIS_5_COMDCB_EN},  /*RS485*/
    #endif
    
	{0xF202,0x01,2,AR_ST_2_VIS_5_COMDCB},     /*���� */
	{0xF205,NUM_RELAY,3,AR_ST_4_VIS_1_EN_EN_EN},   /*�̵������ */
	{0xF206,0x01,4,AR_TYPE_ENUM},             /*�澯���*/
	{0xF207,0x01,5,AR_TYPE_ENUM},             /*�๦�ܶ���  */
	{0xF209,0x01,6,AR_ST_3_NULL_COMDCB_NULL}, /*�ز�/΢�������߽ӿ�*/
};

/*22��OBIS�б� ����2 �豸�����б�*/
const ST_Class22_OAD_OBIS_INFO CLASS22_OI_AID_OBIS[] = 
{
	{1,0x02,E_RS485_1_BAUDRATE/*������ʾ*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},   /*RS485*/
	{2,0x01,E_RS485_1_BAUDRATE/*������ʾ*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*���� */
	{3,NUM_RELAY,E_RELAY_UNIT_POWER, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},             /*�̵������ */
	{4,0x02,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},             /*�澯���*/
	{5,0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_MULTI, E_INFO_NULL},            /*�๦�ܶ���  */
	{6,0x01,E_RS485_1_BAUDRATE/*������ʾ*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*�ز�/΢�������߽ӿ�*/    
};


INT8U get_class22_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{ 
    return get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                                (const ST_PARAMENT_OAD_OBIS_INFO *)CLASS22_OI_AID_OBIS,ARRAY_SIZE(CLASS22_OI_AID_OBIS));
    
}    



/*22���¼�Ԫ�ز���*/
INT8U get_class22_input_output_object(SUB_OBJECT *pObject)
{
    INT8U  i,AID,IID;
    INT16U OI; 
    #ifdef THREE_PHASE
    INT8U MeterType;
    #endif
      
    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*���AIDΪ3*/
    if(AID>3)
    {
        return DAR_4_OANODEF;
    } 

    for(i=0;i<ARRAY_SIZE(CLASS22OBISLIST_AID_2);i++)
    {
        if(OI == CLASS22OBISLIST_AID_2[i].OI)
        {
            pObject->Class = 22;
            if(IID>CLASS22OBISLIST_AID_2[i].MAX_IID)
            {
                return DAR_8_OVERFLOW;
            }   
            else
            {
                pObject->Right_RW = READ_ONLY;
                pObject->element  = i;
                pObject->seq_num  = CLASS22OBISLIST_AID_2[i].Seq_Num;
                if(3==AID)
                {
                    pObject->Pointer = TYPE_U8;
                } 
                else
                {     
                    pObject->Pointer  = CLASS22OBISLIST_AID_2[i].Pointer;
                }

                #ifdef THREE_PHASE
                /*���ܱ���豸������һ��*/
                GetSingle(E_METER_MODE, (INT8U *)&MeterType); 
                if(SMARTMETER == (MeterType&METERTPYE))
                {
                    if(OI_RS485_DEVICE == OI)
                    {
                        pObject->Pointer = AR_ST_3_VIS_6_COMDCB_EN;                        
                    }   
                }                     
                #endif
                
                return DAR_0_OK;
            }    
        }    
    }  

    return DAR_4_OANODEF;
    
}


/***********************************************/
const char string_com_698[2][PORT_DESCRIPT_LEN_6]={"RS485","INFRA"};
//const char string_carrie[MAX_NUM_BYPE_PORT_DESCRIPT] = {0};	//"�ز�"
/*�����̶�����*/
const INT8U rs_para_com_698[]={2,8,1,0,0};
/*return:���ݳ��ȣ�����û��ʲô����*/   


/*****************************************************************************
 �� �� ��  : GetClass22Obis
 ��������  : ��ȡͨ�ſڵĲ�����OBIS
 �������  : ST_OAD Oad  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 05 21
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT16U GetClass22Obis(ST_OAD Oad)
{
    INT16U Oi,OBIS;

    Oi = Oad.Oi.asInt;
    
    /*�ز�*/
    if(OI_CARRIE_DEVICE == Oi)
    {
        OBIS = E_RS485_1_BAUDRATE;
    }  
    else if(OI_IR_DEVEICE == Oi)
    {
       OBIS  = E_RS485_1_BAUDRATE;
    }    
    else //if(OI_RS485_DEVICE == Oi) 
    { 
        #ifdef THREE_PHASE
        if(2 == Oad.IId)
        {    
            OBIS = E_RS485_2_BAUDRATE;
        }
        else
        #endif    
        {
            OBIS = E_RS485_1_BAUDRATE;
        }    
    }  

    return OBIS;
}


/*****************************************************************************
 �� �� ��  : class22_input_output_get
 ��������  : ��������豸GETʵ��
 �������  : ST_OAD Oad    
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 05 22
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT16S class22_input_output_get(ST_OAD Oad, INT8U *pdata)
{
    INT16U           Oi,OBIS;
    INT8U            IID,MAX_IID = 1;
    INT8U            *p_rs_device;
    BOOL             F_zhi;   /*���ܱ��־*/
    
    STR_COMDCB *p_comdcb;      /*�˿ڲ���ָ��*/  
    INT8U *pdst = pdata;

    #ifdef THREE_PHASE
	INT8U MeterType;    
    GetSingle(E_METER_MODE, (INT8U *)&MeterType); 

    if(SMARTMETER == (MeterType&METERTPYE))
    {
       F_zhi = 1; 
    } 
    else
    #endif    
    {
       F_zhi = 0; 
    } 

    Oi   = Oad.Oi.asInt;
    IID  = Oad.IId;    

    /*�����ѱ����̵���Ҳ��*/
    
    /*�豸����*/
    if(3 == Oad.AId)
    {   
        if(0!=F_zhi)
        {
            *pdata = 2;
        }    
        else
        {    
            *pdata = 1;
        } 
        
        return DAR_0_OK;
    }  
    
    /*���ܱ�*/
    if(0!=F_zhi)
    {        
        MAX_IID = 2;        
    }  
    else 
    {
        MAX_IID = 1;
    }  

    /*�ж��ºϷ���*/
    if(IID>MAX_IID)
    {
        return (0-DAR_8_OVERFLOW);
    }        
    
    if(0 == IID)
    {        
       IID = 1;
       
       if(0!=F_zhi)
       {
          MAX_IID = 2;
       }
       else
       {
          MAX_IID = 1;
       }  
    } 
    else
    {
        //IID = MAX_IID;
        MAX_IID = IID;        /*�Ա�ֻ֤��һ������*/
    }    
     
 //------1.�ز�---------------------        
	if(OI_CARRIE_DEVICE == Oi)
	{		
	    //��8�Ų��ţ��ز��˿�������ΪNULL
		p_comdcb = (STR_COMDCB *)pdata;
        GetSingle(E_RS485_1_BAUDRATE, &p_comdcb->commrate);
		p_comdcb->check_bit = 2;
		p_comdcb->data_bit	= 8;
		p_comdcb->stop_bit	= 1;
		p_comdcb->flow_ctr	= 0;
        
        return DAR_0_OK;
        //return sizeof(STR_COMDCB);
	}	
//-------2.485�ͺ���--------------------    
    if((Oi>=OI_RS485_DEVICE)&&(Oi<=OI_IR_DEVEICE))
    { 
       p_rs_device = (INT8U *)pdata;
       
       for(;IID<=MAX_IID;IID++)
       {          
           /*RS485��INFRA*/
           LIB_MemCpy((INT8U*)string_com_698[(INT8U)(Oi-OI_RS485_DEVICE)],p_rs_device,PORT_DESCRIPT_LEN_5);

           #ifdef THREE_PHASE
           if (Oi==OI_RS485_DEVICE)
    	   {
        		//��һ·:RS485�����֧�ֶ�·RS485��������RS4851��
        		if ((IID==0x01)&&(0!=F_zhi)) //���ܱ�
        		{
        			p_rs_device[PORT_DESCRIPT_LEN_5]='1';
                    p_rs_device++;
                }
        		//�ڶ�·:RS4852
        		else if (IID==0x02)
                {                  
        			p_rs_device[PORT_DESCRIPT_LEN_5]='2';
                    p_rs_device++; 
                    Oad.IId = 2;     /*��һ�£�����GetClass22Obis����*/
                }              
    	   }
           #endif
           

           p_rs_device += PORT_DESCRIPT_LEN_5;

           p_comdcb = (STR_COMDCB *)p_rs_device;

           OBIS = GetClass22Obis(Oad);    
           GetSingle(OBIS, &p_comdcb->commrate);           
           
           p_rs_device++;
           LIB_MemCpy((INT8U *)rs_para_com_698, p_rs_device, sizeof(rs_para_com_698));
           p_rs_device += sizeof(rs_para_com_698);
       }     
        
       /*5+5+1=11�������ݳ���Ϊ11��12*/
       //return DAR_0_OK; //(p_rs_device - pdata);
       
    }
//-----------3.�̵������-----------------------------------
    else if(OI_RELAY_DEVICE ==Oi)
    {        
        for(;IID<=MAX_IID;IID++)
        {   
          //OBIS =  E_RELAY_UNIT_POWER+IID-1;
          pdst += UI_GetRelayOutput(&IID,pdst); 
          //pdst+=GetSingle(OBIS, pdst);
        }       
    }
//------------4.�澯���----------------------------------
    else if(OI_ALARM_DEVICE == Oi)
    {
        #ifdef THREE_PHASE
        *pdata = OC_GetAlmRelayCmd();  
        #else
        *pdata = 0;  
        #endif
        
    }
//------------5.�๦�����----------------------------------
    else if(OI_MULTI_DEVICE == Oi)
    {
        UI_GetMultiTermn(pdata);
    }
    else
    {
        return (0-DAR_4_OANODEF);
    }

    return DAR_0_OK;

}

/*****************************************************************************
 �� �� ��  : ��������豸SET����ʵ��
 ��������  : 
 �������  : Oad��pdata
 �������  : DAR
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 05 22
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
/*֧��ͨѶ�ڵ����ã��ڳ���ʹ��*/
INT8U class22_input_output_set(ST_OAD Oad, INT8U *pdata)
{
    INT16U     Oi;
    STR_COMDCB *p_comdcb;      /*�˿ڲ���ָ��*/
    //UINT8 temp;
    INT8U AID,ChangePort; 
    INT16U OBIS;
    #ifdef THREE_PHASE
    INT8U MeterType;
    #endif
    
    AID = Oad.AId;
    
    if(ATTRIBUTE_2_SET_PARAMENT!=AID)
    {
        return 0;
    }

    Oi = Oad.Oi.asInt; 

    /*���ⲻ����*/
    if(OI_IR_DEVEICE == Oi)
    {
        return DAR_16_BAUDERR;
    }
    
    //---------------------------
    if((OI_RS485_DEVICE == Oi)||(OI_CARRIE_DEVICE == Oi))
    { 
        if(OI_CARRIE_DEVICE == Oi)
        {
            p_comdcb   = (STR_COMDCB *)pdata;    /*NULL*/
            ChangePort = E_COM_RS485_1;
        }    
        else
        {
            ChangePort = E_COM_RS485_1;
            #ifdef THREE_PHASE
            GetSingle(E_METER_MODE, (INT8U *)&MeterType); 
            if(SMARTMETER == (MeterType&METERTPYE))
            {
               p_comdcb = (STR_COMDCB *)(pdata+6);  /*RS4851��RS4852*/ 
               if(2 == (*(pdata+5)))
               {
                  ChangePort = E_COM_RS485_2;  
               } 
            }
            else
            #endif    
            {
               p_comdcb   = (STR_COMDCB *)(pdata+5);  /*RS485*/               
            }    
        }  

        OBIS = GetClass22Obis(Oad);        

        mBpsChangeFlg[ChangePort] = TRUE;
        
        return SetSingle(OBIS,&p_comdcb->commrate,1);    
               
    }

    return DAR_4_OANODEF;

}    
 
/*****************************************************************************
 �� �� ��  : class22_input_output_action
 ��������  : ��������豸������������ʵ��
 �������  : ST_OAD Oad    
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : dar
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 05 22
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U class22_input_output_action(ST_OAD Oad, INT8U *pdata)
{
    INT16U            Oi;
    STR_RS_DEVICE_SET *p_rs_device;
    INT8U temp,ChangePort;
    INT8U AID; 
    INT16U OBIS;

    AID = Oad.AId;
    
    if((METHOD_CLASS22_127!=AID)&&(METHOD_CLASS22_128!=AID))
    {
        return DAR_4_OANODEF;
    }

    Oi = Oad.Oi.asInt;

    /*���ⲻ����*/
    if(OI_IR_DEVEICE == Oi)
    {
        return DAR_16_BAUDERR;
    }    

    //--------1.����������-------------------
    if((OI_RS485_DEVICE == Oi)||(OI_CARRIE_DEVICE == Oi))
    {
        p_rs_device = (STR_RS_DEVICE_SET *)pdata;

        if(OI_CARRIE_DEVICE == Oi)  /*�ز�*/
        {
            OBIS       = E_RS485_1_BAUDRATE;
            ChangePort = E_COM_RS485_1;
        } 
        else //(OI_RS485_DEVICE == Oi)
        {    
            #ifdef THREE_PHASE            
            if(2 == p_rs_device->OAD.IId)
            {
                OBIS = E_RS485_2_BAUDRATE;
                ChangePort = E_COM_RS485_2;
            }  
            else
            #endif    
            {
                OBIS = E_RS485_1_BAUDRATE;
                ChangePort = E_COM_RS485_1;
            }  
        }        

        mBpsChangeFlg[ChangePort] = TRUE;
        
        return SetSingle(OBIS,&p_rs_device->port_com_dcb.commrate,1);
    }
    //---------------------------
    else if(OI_RELAY_DEVICE == Oi)
    {
        /*����ʱ����д*/
        return DAR_1_HW_FAIL;
    }
    //----------------------------------------------
    else if(OI_MULTI_DEVICE == Oi)
    {
        temp =  *(INT8U *)(pdata+4);
        
        if(temp>E_MULTI_TRAFFSWITCH)
        {
            return DAR_FF_OTHERERR;
        }
        #ifdef SINGLE_PHASE
        if(E_MULTI_DEMANDPERIOD == temp)
        {
            return DAR_FF_OTHERERR;
        }
        #endif
        
        OC_MultiTermnSw((EN_OC_MultiTermnMode) temp);

        return DAR_0_OK;
    }
    else
    {
        return DAR_4_OANODEF;
    }

}

/*"************************************************"*/
