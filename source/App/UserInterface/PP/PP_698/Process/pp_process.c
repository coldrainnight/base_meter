/*"*****************************************************************************"*/
/*"  FileName: pp_process.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  Э�����ݴ����ļ�   "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "APP\message\MS.h"
#include "APP\UserInterface\PP\PP_698\Class\class8_parament.h" 
#include "app\userinterface\ProtocolStack\PS_698\AL_698.h"
#include "App\UserInterface\UI.h"

/*Э�鴦��ģ�����ݱ�������*/
INT8U gServiceChoice;       /*CHOICE��ʽ ��ȫ�ֱ������Ա㴦�����֡*/

INT8U *pEndEncodeAddr;   /*Ӧ�ò�������ձ��볤��*/

/*����֡�ݴ�����*/
INT8U gBak_data[MAXGETRECORDLISTNUMBER][LEN_NEXT_FRAM_RECORD_DATA];

INT8U Record_Num_List; /*�ֱ������ݴ浱ǰ��recordlist��recordlistnum*/       

/*���OAD*/
ST_OAD Program_Oad[10]; 




/*****************************************************************************
 �� �� ��  : PP_PrgEvent
 ��������  : ��¼��̼�¼
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 21
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
/*"��¼��̼�¼"*/   
void PP_PrgEvent(ST_OAD Oad)
{
    ST_OAD Tmp_Oad[10];

    /*�����ж��Ƿ����Զ���ģ���ֹ�˳���Ҳ��¼��̼�¼*/
    if(Oad.Oi.asInt>=OI_METER_MODE)
    {
        return;
    }     

    /*����¼��������뷽����Ҫ������״̬*/
    if((OI_PRG_EVENT == Oad.Oi.asInt)&&(8!=Oad.AId))
    {
        Push(E_PRGEVENT_START,FALSE);/*"��̽���"*/
    }      
    
    if(ChkStatus(E_PRGEVENT_START) == FALSE)  /*"���δ��ʼ"*/   
    {
        LIB_MemSet(0xFF, (INT8U *)&Program_Oad[0], 40);        
        LIB_MemCpy((INT8U *)&Oad, (INT8U *)&Program_Oad[0], sizeof(ST_OAD));
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_S);
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_E);
        Push(E_PRGEVENT_START,TRUE);
    }
    else/*"д��������¼���¼"*/   
    {
        LIB_MemCpy((INT8U *)&Program_Oad[0], (INT8U *)&Tmp_Oad[1], sizeof(ST_OAD)*9);
        LIB_MemCpy((INT8U *)&Oad, (INT8U *)&Tmp_Oad[0], sizeof(ST_OAD));        
        LIB_MemCpy((INT8U *)&Tmp_Oad[0], (INT8U *)&Program_Oad[0], sizeof(ST_OAD)*10); 
        
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_E);
    }
}


/*"��̼�¼����"*/   
INT8S PrgEvent_Rcrd(void *pdin, void *pdout)
{
	ST_OAD Oad;
	INT8U *pdata;
    
	pdata = (INT8U *)pdin;	
	LIB_MemCpy(pdata,(INT8U *)&Oad,sizeof(ST_OAD));

	PP_PrgEvent(Oad);

	return DAR_0_OK;
}

/*return:0:ok, 0xff:not ok*/
INT8U check_iid(INT8U ServiceType, ST_OAD Oad)
{
	INT8U  iid;
	INT8S  Ret = DAR_0_OK;
	INT16U OI;
 
	iid      = Oad.IId;
	OI       = Oad.Oi.asInt; 

    if(0 == Oad.AId)
    {
        return DAR_5_OANOADAPT;
    }    

	switch(OI)
	{
	    #if 0 /*��get_oad_object�����ж�*/
		case OI_STATUS_METER:   /*����״̬��*/
			if(iid>MAX_IID_2014)
			{
				Ret = DAR_8_OVERFLOW;
			}
			break;
        #endif    
		case OI_PUBLIC_HOLIDAY:        
			/*�������գ�����ֱ����0���*/
			if((0 == iid)&&(EN_SETREQUEST == ServiceType))
			{
				Ret = DAR_5_OANOADAPT;
			}            
			break;
       #if 0     
       /*ʱ�����ж�*/     
       case OI_CURRENT_SLOT_TABLE:
       case OI_BAK_SLOT_TABLE:   
            if(MAX_IID_YEALY_SLOT_TABLE<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }    
            break;  
        #endif     
        /*ʱ�����ж�*/        
        case OI_BAK_DAILY_SLOT_TABLE:
        case OI_CURRENT_DAILY_SLOT_TABLE:    
            /*���ܱ�ʱ�α��̣�һ��ֻ������һ��ʱ�α�*/
			if((0 == iid)&&(EN_SETREQUEST == ServiceType))
			{
				Ret = DAR_5_OANOADAPT;
			}  
            
            #if 0 
            if(MAX_IID_DAILY_SLOT_TABLE<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }   
            #endif
            
            break;
       #if 0      
       /*�������ж�*/     
       case OI_CURRENT_TRIFF_PRICE:
       case OI_BAK_TRIFF_PRICE:   
            if(MAX_IID_TRIFF<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }    
            break;  
       #endif     
	}
	
	return Ret;
	
}


/*****************************************************************************
 �� �� ��  : Check_Special_OAD
 ��������  : ���������Ĳ�����OAD������ǵģ���ֱ�ӷ���
 �������  : ST_OAD Oad  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��7��10��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_Special_OAD(ST_OAD Oad)
{
    INT8U  AID;
    INT16U OI;

    OI  = Oad.Oi.asInt; 
	AID = Oad.AId;

	/*45000900 �ź�ǿ������*/ /*����ֱ��������ȷ�Ϻ�������ͺ����ѯ��������*/ 
    if((OI_SIGNAL_GPRS==OI)||((OI_ESAM == OI)&&((0x0B ==AID)||(0x0C ==AID))))  
    {
	    return DAR_0_OK;   
    }	

    /*2017-10-31:���ں���Ȩ�޿��ƺ��棬��ֹ����û��ͨ����֤����� ,2018-06-22����������Ȩ�޻��Ƿſ�*/    
    if(((OI_REMOTE==OI)&&(131==AID))||                     /*֧�����������բ*/ 
	    ((OI_REPORT_METER==OI)&&(127 ==AID)))              /*�ϱ�״̬ȷ��*/ 
    {
        return DAR_0_OK;
    } 

    return DAR_FF_OTHERERR;
    
}    

/*****************************************************************************
 �� �� ��  : check_object_right
 ��������  : ������Ȩ��
 �������  : TypeService:��������
             pObject:
             pdata:���������õ�Դ���ݵ�ַ���Ա�������ָ��Ƚϣ�������ʱ��û����
 �������  : ��
 �� �� ֵ  : DAR_0_OK:Ok,Others:not ok
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 09
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U check_object_right(INT8U ServiceType,SUB_OBJECT *pObject,INT8U *pData)
{
    INT8U  iid;
    INT16U OI;
    INT8S  Ret;
    
    
    /*2017-11-18 ���ڣ�ֱ���˳�*/
    if (TRUE == ChkStatus(E_FAC))
    {
        return DAR_0_OK;
    }  

    if(DAR_0_OK == Check_Special_OAD(pObject->Oad))
    {
        return DAR_0_OK;
    }    

    OI  = pObject->Oad.Oi.asInt;
    iid = pObject->Oad.IId;  

#ifdef PM_MODLE       
    /*-----------------�����ж�-------------------------*/    
    GetSingle(E_HANGUP_FLAG, &F_ESAM_IS_OFF);


    /*2017-08-17:ESAM���𣬲������������������ 2017-10-31 ������û�����⣬�����з�����������?�������Ĳ�����Ҳ������?*/
    if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
    {     
        if((OI_REMOTE == OI)||(OI_REMAIN == OI))
        {       
            temp_data[0] = 0; 
            temp_data[1] = 0; 
            SetSingle(E_CTRCMD_STATUS,&temp_data[0],2);       /*1.��������ִ��״̬��*/       
            
            if (0!=F_ESAM_IS_OFF)
            {    
                /*2017-11-29 Զ�̿��ƣ��ù����־*/  
                temp_data[0] = 1;                          
            }              
            
            SetSingle(E_CTRERR_STATUS, &temp_data[0], 2);    /*2.�����Ƿ�����ô���״̬��*/         
        }
        
        if (0!=F_ESAM_IS_OFF)
        {            
            return DAR_31_HANGUP;
        }	        
    }   
    /*-----------------�����ж�------END-------------------*/    
#endif
    /*���ܱ�Ӧ֧��"�ָ���������"�����ؾܾ���д�� DAR=3����������action�����Բ����ö�дȨ�޿���*/
    if((4 == pObject->Oad.AId )&&(OI_ELECTRI_DEVICE==OI)&&(EN_ACTIONREQUEST == ServiceType))
    {
        return DAR_3_NOACESS;
    }  

    /*�ж�дȨ�ޣ���Щ����Ϊֻ�����ԣ����벻�ܶ���*/
    if(((EN_SETREQUEST == ServiceType)&&(0==((pObject->Right_RW)& WRITE_ONLY)))||
        ((EN_GETREQUEST == ServiceType)&& (0==((pObject->Right_RW)& READ_ONLY))))/*(OI_CONNECT_PASS==OI)))*/	/*(0==((pObject->Right_RW)& READ_ONLY)))*/
    {
        return DAR_3_NOACESS;
    }    

    /*�жϰ�ȫģʽ*/
    Ret = get_oi_right_type(ServiceType,pObject);
    if(DAR_0_OK!=Ret)
    {
        return DAR_20_NO_RIGHT;
    }  

    Ret = check_iid(ServiceType,pObject->Oad);    
    if(DAR_0_OK!=Ret)
    {
        return Ret;
    }    
    
    #if 0    
    /*�����಻�������ò���?�����ǲ����еĲ�ȫ?*/
	if((d->Class == 9)&&(d->Oad.AId == 4)&&(ACTIONREQUEST !=ServiceType))
	{
		d->Value = VALUEDAR8;
	}
    #endif

    /*ESAM��֧������ָ�룬���Բ��жϣ�һ�㲻����PointerΪNULL����Ϊ�˿ɿ��ԣ����������ж�һ�£���ֹ������ָ��*/
    if((NULL != pObject->Pointer)&&(OI_ESAM!=OI))
    {
        if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
        {		     
            /*2017-12-09 �����IID����Ҫ�ٿ�����*/
            if(EN_ACTIONREQUEST == ServiceType)
            {
                iid = 0;
            }                
                
            if(0>DeCode_Data((ST_ENCODE_INFO *)pObject->Pointer,pData,NULL,iid))
            {
                return DAR_7_TYPENOADAPT;
            }      

            /*�����µ�ַ�����ж�*/
            if(((OI_METER_ADDR==OI)||(OI_GROUP_ADDR == OI))&&(MAX_ADDR_LEN!=pData[1]))
            {
                return DAR_5_OANOADAPT;
            }    
            
        }   
    }    
#ifdef PM_MODLE       
     /*���⣬���ر������Զ�̱������д*/
	if(Action(E_LOCALOI_CHECK,&OI,NULL) != DAR_0_OK)
    {
        return DAR_6_OANOEXIST;
    }  
#endif    
    return DAR_0_OK; 
}    

/*****************************************************************************
 �� �� ��  : Get_Special_Oad_Data
 ��������  : һЩ����OAD�����ݴ�����������û��OBIS
 �������  : ST_OAD Oad    
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : DAR_0_OK: ���⴦���ˣ�ֱ�ӷ������ݣ� DAR_FF_OTHERERR:��ͨ��OBIS����ȡ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 01 16
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Get_Special_Oad_Data(ST_OAD Oad,INT8U *pdata)
{
    INT8U  iid,AId,i;   
    INT16U Oi;    
    #ifdef THREE_PHASE
    INT16U UiState;
    #endif    
    INT8U  Ret = DAR_0_OK;
    INT8U  Temp;
    INT32U TmpShowVersion;
    
    Oi  = Oad.Oi.asInt;
    iid = Oad.IId;
    AId = Oad.AId;
    
    switch ( Oi )
    {
        case OI_ELECTRI_DEVICE:
            if(5 != AId)    /*���豸�б�*/
            { 
                Ret = DAR_FF_OTHERERR;
            }  
            else
            {
                *pdata = 0; /*�����豸�б�*/     
            }    
            break;    
        case OI_DATE_TIME:
            if(3 == AId)
            {
                *pdata = 0;  /*��վ��ʱ��0����*/
            }  
            else
            {
                Ret = DAR_FF_OTHERERR;
            }    
            break;            
        case OI_CLOCK_SOURCE:
            Temp = ChkStatus(E_CLK_ERR);  /*״̬  enum{���ã�0���������ã�1��}*/
            
            *pdata = EN_CLR_SRC_RTC;    /*ʱ��Դ�̶�Ϊʱ��оƬ*/
            
            if(0 == iid)
            {
                *(pdata+1) = Temp;
            }
            else if(2 == iid)
            {
                *pdata = Temp;
            }  
            
            break;  
        case OI_METER_NUMBER:  
            /*����Ԫ���������Ϊ1���������߱�Ϊ2���������߱�Ϊ3��*/
            #ifdef THREE_PHASE
            *pdata = EN_METER_NUM_3_4_1;   
            GetSingle(E_MU_STATUS, (INT8U *)&UiState);
            if(UiState&0x01)   /*"bitֵΪ0��ʾ3P4W��Ϊ1��ʾ3P3W"*/
            {
               *pdata = EN_METER_NUM_3_3_1;    
            }
            #else
            *pdata = EN_METER_NUM_SINGLE;    
            #endif
            break;
        case OI_DISP_VERSION:
            TmpShowVersion = 0;
            for(i = 0; i < 8; i++) 
            {
                TmpShowVersion |= ((INT32U)(ShowVersion[i] - 0x30) << (7 - i) * 4);
            }
 
            LIB_MemCpy((INT8U *)&TmpShowVersion, pdata, sizeof(INT32U));

            break;
#ifdef PM_MODLE               
        case OI_REMAIN:
            if(2 == AId)
            {
                Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"ˢ��Ԥ��������״̬��"*/
                *pdata = ChkStatus(E_RELAY_PROTECT);
            } 
            else
            {
                Ret = DAR_FF_OTHERERR;
            }    
            break;
#endif            
        case OI_METER_POWER_DOWN_EVENT:
            if(6 == AId)
            {    
                *pdata = 60;    /*�̶�Ϊ60*/
            }
            else
            {
               Ret = DAR_FF_OTHERERR;
            }    
            break;
        default: 
            Ret = DAR_FF_OTHERERR;
            break;
    }   

    return Ret;
}    
/*****************************************************************************
 �� �� ��  : Get_Special_Class_Data
 ��������  : ���������ݴ���
 �������  : INT8U *pdata  
             ST_OAD Oad    
             INT8U *pRet    
 �������  : ��
 �� �� ֵ  : INT8U* �����ĵ�ַ ���������pdata�����ʾ�б���
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��21��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Get_Special_Class_Data(INT8U *pdata,SUB_OBJECT *pObject,INT8U *pRet)
{
    INT8U AId;
    INT8U *pdst = pdata;
    ST_OAD Oad;   
    INT8U  Class = 0;   
    
    Class = pObject->Class;
    LIB_MemCpy((INT8U *)&pObject->Oad,(INT8U *)&Oad,sizeof(ST_OAD));   /*������COPYһ�Σ���Get_OAD_Data_len()�У�object.Oad���ܱ���*/ 

    AId = Oad.AId;  
    
    /*1.�߼���*/
    if(1 == AId)
    {        
        pdst = EncodeAtt1LogicName(&Oad,pdst);        
    } 
    /*2.���㵥λ*/
    else if(((3 == AId)&&(Class<=6))||((5 == AId)&&(Class<=1))) /*���㵥λ*/
    {
        pdst = Encode_Scale_Unit(&Oad,pdst,pObject->element);        
    } 
    else if(17 == Class)                                        /*17������CSD�б�Ƚ����⣬��Ҫ���⴦��*/
    {
        pdst = get_class17_disp_data(Oad,pdst,pRet);        
    }   
    else if(20 == Class)
    {
        pdst = get_class20_connect_data(Oad,pdst,pRet);        
    }  
    else if(7 == Class)
    {
        if(7 == AId)                      /*��ǰֵ��¼��*/
        {
            pdst = Get_Class7_Att7(pdst,Oad);   
        }    
        if(10 == AId)                     /*ʱ��״̬��¼��*/
        {
            pdst = Get_Class7_Att10(pdst,Oad);     
        }    
    }   
    else if(24 == Class)
    {
        if(3 == AId)                                /*��ǰ��¼��*/
        {
            pdst = Get_Class24_Att3(pdst,Oad);
        } 
        else if(10 == AId)                          /*��ǰֵ��¼��*/
        {
            pdst =Get_Class24_Att10(pdst,Oad);
        }        
        #ifdef THREE_PHASE
        else if(13 == AId)
        {
            pdst =Get_Class24_Att13(pdst,Oad);        /*Ҫgetlist��ʽת�����������⴦��*/
        }    
        #endif
        else if(14 == AId)                          /*ʱ��״̬��¼��*/
        {
            pdst =Get_Class24_Att14(pdst,Oad);          
        } 
    }    
    else if ((OI_SAFE_MODE == Oad.Oi.asInt)&&(3 == AId))  /*��ȫģʽ�䳤����*/
    {
        pdst = Get_safe_mode(Oad,pdst);        
    } 

    return pdst;
    
}    


/*****************************************************************************
 �� �� ��  : SetSingle
 ��������  : ��SET�����ĵ������ã���װ��һ���������Է���ʹ��
 �������  : INT16U OBIS   
             INT8U *pData  
             INT8U len     
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 31
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U SetSingle(INT16U OBIS,INT8U *pData,INT8U len)
{
    return  (0 -Set(&OBIS,1,pData,&len));
}

/*****************************************************************************
 �� �� ��  : Set_Object_Single
 ��������  : ����ĳ��OAD������
 �������  : ST_OAD Oad    
             INT8U *pdata:ָ�����͵�����
 �������  : ��
 �� �� ֵ  : ����DAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 04
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Set_Object_Single(SUB_OBJECT *pObject,INT8U *pdata,INT16U len_data)
{
    INT8U num_obis,num_all_obis,iid;    
    INT8U Ret,CLASS,AID;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];
    ST_FMT_ATTR Format_list[MAX_OBIS_NUM_ONE_OAD];
    const ST_FUNCTION *pFunc;
    
    ST_FMT_ATTR Format_Block;  /*�洢��ʽ*/

    INT16U Obis[MAX_OBIS_NUM_ONE_OAD];
    
    INT8U temp_data_2[MAX_DATA_LEN_SET/2];
    
    INT8U len,temp_len;
    
    /*-------*/
    num_obis = PP_OadToObis_Info(pObject,Obis_Info);
    iid      = pObject->Oad.IId;
    num_all_obis = PP_GetOadToObisList(iid,num_obis,Obis,Obis_Info,Format_list);   
    if(num_all_obis>MASK_BLOCK_DATA)
    {
        num_all_obis = 1;
    }    
    
    /*2017-12-12:��������дǰ����*/
    /*����дǰ����*/
    //2018-03-14:��ʽ������������� if((FUN_NULL <Obis_Info[0].FunctionId)&&(MAX_FUN_ID >Obis_Info[0].FunctionId))

    if(FUN_NULL !=Obis_Info[0].FunctionId)
    {
        pFunc = (const ST_FUNCTION *)&stFunction[Obis_Info[0].FunctionId];
        if(pFunc->OnWrite_Beford != 0) 
        {            
            Ret = pFunc->OnWrite_Beford(pdata);
            if(DAR_0_OK != Ret)
            {
                return (Ret);
            }

            /*����ʱ�䣬ֱ����дǰ���������*/
            if((OI_DATE_TIME ==  pObject->Oad.Oi.asInt)&&(2 ==pObject->Oad.AId))
            {
                return DAR_0_OK;
            }    
        }   
    }     
    

    /*2017-12-12:�������Ӹ�ʽת��   */ /*2017-12-16:�����LEN_DATA������Ҫ�ײ㲻�жϲ��У����������ܳ���*/
    len =  len_data;

    /*ֻ�е����ݣ������ݸ�ʽ��Ҫת���Ĳ�ת*/
    if((1 == num_all_obis)&&(E_NONE_FORMAT!=Format_list[0].format))
    {
        Ret = GetFrmt(Obis[0],&Format_Block);
        if(0xff!=Ret)
        {  
            temp_len = FRMT_NdataChange(pdata,temp_data_2,Format_list[0],Format_Block,(INT16U)len_data);
            if(0 != temp_len)
            {  
                LIB_MemCpy(temp_data_2, pdata, temp_len);
                len = temp_len;
            }  
        }
    }    
#ifdef PM_MODLE   
    /*���⴦�����ý��ݵ��ʱ�������E_STEP_TABLE2��������*/
	if(Obis[0] == E_SETPVAULE_PASBLK)
	{
		num_all_obis = 1;
		Obis[0] = E_STEP_TABLE2;
	}
#endif
    Ret = (0- Set(Obis,num_all_obis,pdata,&len));

    /*2018-03-01:���ù��������巢��ʱ����ۼ�ʱ�䣬��ֹACTIONǶ��*/
    if(DAR_0_OK == Ret)
    {
        CLASS = pObject->Class;
        AID   = pObject->Oad.AId;
        
        if(((EN_CLS7_ATT_3_RELATION_OBJECT_TABLE == AID)&&(7 == CLASS))||           
           ((EN_CLS24_ATT_2_RELATION_OBJECT_TABLE == AID)&&(24 == CLASS)))
        {
           Action(E_MNT_EVNT_CLR,&Obis[0],NULL); 
        } 
    }    
    
    return Ret; 
    
}   

/*****************************************************************************
 �� �� ��  : get_oad_object
 ��������  : ����OAD����ȡԪ�����ݣ������AID��IID��С���Ա�ȷ���Ƿ�Ϸ�OAD
 �������  : ST_OAD Oad           
             SUB_OBJECT *pObject  
 �������  : ��
 �� �� ֵ  : dar:һ����δ���壬Ҳ������ʱ������ʱ������
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 07
    ��    ��   : xyt
    �޸�����   : �����ɺ���

ע:��ڲ�����pObject->classһ����ʼ��Ϊ0
*****************************************************************************/
INT8U get_oad_object(SUB_OBJECT *pObject)
{
    INT8U ret;
    INT8U Oi0,Oi1,tmp_oi0;  

    /*�ȸ�ֵ�£���ֹ�������*/
    pObject->Pointer   = NULL;
    pObject->Right_RW  = READ_ONLY;  /*�ȳ�ʼ���¶�дȨ�ޣ���������еĵط�(�����¼��)�����¸�ֵΪ0��*/
#ifdef PM_MODLE   
    /*���ж��Ƿ񱾵ر�����*/
    if(Action(E_LOCALOI_CHECK,&pObject->Oad.Oi.asInt,NULL) != DAR_0_OK)
    {
        return DAR_6_OANOEXIST;
    }    
#endif
    ret = get_special_class_object(pObject);
    if(DAR_0_OK == ret)   
    {
        return DAR_0_OK;
    }    

    /*�ҵ��ˣ���AID��IID����*/
    if(0!=pObject->Class)   /*��������(DAR_8_OVERFLOW == ret)��������*/
    {
        return ret;
    }       

    Oi0 = pObject->Oad.Oi.asByte[OIH];

    Oi1 = pObject->Oad.Oi.asByte[OIL];
    tmp_oi0 = Oi0 & 0xf0;

    switch(tmp_oi0)
    {
        case 0x00:
             ret = get_class1_6_object(pObject,1); /*����  */
             break;
             
#ifdef THREE_PHASE
       case 0x10:
            ret = get_class1_6_object(pObject,2); ;/*�������  */
            break;
#endif
       case 0x20: 
            ret = Get_Variable_Object(pObject);    /*����  */
            if(DAR_4_OANODEF == ret)
            {
                ret = get_class8_object(pObject); /*�α���  */
            }
            break;
            
      case 0x30:
            if(0x33 ==Oi0)  
            {
                ret = get_class8_object(pObject); /*�α���  */
            }
            else
            {
                ret = get_class7_object(pObject); /*7���¼�  */
                if(DAR_4_OANODEF == ret)
                {
                    ret = get_class24_object(pObject); /*24���¼�  */
                }    
            }
            break;
            
        case 0x40:
            ret = Get_Variable_Object(pObject);    /*����  */
            if(DAR_4_OANODEF == ret)
            {
                 ret = get_class8_object(pObject); /*�α��� */
            }
            break;    
        case 0x50:
            ret = get_class9_freeze_object(pObject);/*����  */
            break;
            
        case 0x80:         
            ret = get_class8_object(pObject);     /*�α���  */
            break;
            
        case 0xf0:
             /* ��ȫģʽ���� ��������ֱ�Ӹ�ֵ��Ϊ��ʡ���룬�����仯ʱ���仯�����Ի����ú���*/
             if(((0xf1 == Oi0)&&(0x01 == Oi1))||(0xff == Oi0))  
             {
                 ret = get_class8_object(pObject); /*�α���  */
             }
             else if(0xf2 == Oi0)
             {
                 ret = get_class22_input_output_object(pObject);   /*��������豸*/                   
             }
            break;
            
        default:
            ret = DAR_4_OANODEF;
            break;
    }
    
    return ret;   

}    

/*��¼����ֻ��ͨ��get record*/
INT8U check_event_freeze_get_record(SUB_OBJECT *pObject)
{   
    INT8U AID,CLASS,i;
    INT16U OI;
    const INT16U EVENT_OI[]={0x2026,0x2027,0x2029};  /*��¼OI������ֱ�Ӷ�ȡ*/

    CLASS = pObject->Class;

    AID   = pObject->Oad.AId;

    if(((2 == AID)&&(7 == CLASS))||
       ((2 == AID)&&(9 == CLASS))||
       ((24 == CLASS)&&(6<= AID)&&(9 >= AID)))
    {
        return DAR_5_OANOADAPT;
    } 

    OI = pObject->Oad.Oi.asInt;

    for(i=0;i<ARRAY_SIZE(EVENT_OI);i++)
    {
        if(OI==EVENT_OI[i])
        {
            return DAR_5_OANOADAPT;
        }    
    }    
    

    return DAR_0_OK;
    
}  

/*****************************************************************************
 �� �� ��  : Check_Object
 ��������  : �������Ƿ��壬��Ȩ�޺Ͱ�ȫģʽ�Ƿ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 03 13
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_Object(INT8U ServiceType,SUB_OBJECT *pObject,INT8U *pData)
{
    INT8U ret;
    
    ret = get_oad_object(pObject);   /*ȷ���Ƿ��ж���*/

    if(DAR_0_OK != ret)
    {
        return ret;
    }   

    ret = check_object_right(ServiceType,pObject,pData);

    return ret;    
     
}


/*****************************************************************************
 �� �� ��  : get_single_normal_process
 ��������  : ������ȡĳ��OAD�����ݣ������
 �������  : ST_OAD Oad  
             INT8U *ret  ���ش�������0:��������0�����������볤�ȣ�Ҫ�ú���֡����
             INT8U *pdata ������ʼ��ַ
 �������  : ��
 �� �� ֵ  : INT8U* �����ĵ�ַ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 08
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *get_single_normal_process(ST_OAD Oad,INT8U *pdata,INT8U *is_need_next_frame)
{
    INT8U ret = DAR_0_OK;
    SUB_OBJECT object;
    INT16U len;
    INT8U *pdst = pdata;

    *is_need_next_frame = 0;   
    
    LIB_MemCpy((INT8U * )&Oad, (INT8U *) &CurOad, sizeof(ST_OAD));
    
    /*2.��ȡʵ������*/
    if(DAR_0_OK == gDarDecode) /*�����Խ����ʽ��Ϊ����*/
    {      
        /*��ȡ���ȵ�ͬʱ�����ж�OAD�ĺϷ��ԣ����Ƿ����*/
        len = Get_OAD_Data_len(Oad,&object,&ret);

        if(DAR_0_OK == ret)
        {    
            ret = check_event_freeze_get_record(&object);
        }

        

        if(DAR_0_OK == ret)
        {    
            if(pdst+len+sizeof(ST_OAD)>pEndEncodeAddr)
            {
               *is_need_next_frame = 0xff;
               return pdst;
            }
            
            LIB_MemCpy((INT8U *)&Oad,(INT8U *)&object.Oad ,sizeof(ST_OAD));   /*������COPYһ�Σ���Get_OAD_Data_len()�У�object.Oad���ܱ���*/             
        }        
    }   
    else
    {
        ret = gDarDecode;
    }  

    /*--------------*/
    if(DAR_0_OK != ret)
    {   
       /*���DARҲҪ�жϳ����Ƿ�Խ��*/
       if(pdst+LEN_GET_DAR>pEndEncodeAddr)
       {
           *is_need_next_frame = 0xff;
           return pdst;
       }       
    
       pdst = EncodeGetDar(&Oad,ret,pdst);
    }  

    return pdst;

}    


/*****************************************************************************
 �� �� ��  : get_normal_process
 ��������  : get normal��������ݻ�ȡ
 �������  : INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 08
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *get_normal_process(INT8U *pdata)
{
    INT8U i,j;
    INT8U is_need_next_frame;//= 0;
    ST_Next_Frame_Normal_Data *pGet_Normal_Data;
    INT8U *pdst = pdata;
        
    pGet_Normal_Data = (ST_Next_Frame_Normal_Data *)gBak_data;

    i= pGet_Normal_Data->CurrentLine;

    for(;i<pGet_Normal_Data->TotalLine;i++)
    {      
        pdst=get_single_normal_process(pGet_Normal_Data->oadlist[i],pdst,&is_need_next_frame);

        if(0!=is_need_next_frame)
        {
            #if APP_NEXT_FRAME_NO_ACCESS
            pdst = pdata; 
            /*����LIST����Ϊ�ܾ���д*/
            for(j=0;j<pGet_Normal_Data->TotalLine;j++)
            {    
                pdst = EncodeGetDar(&pGet_Normal_Data->oadlist[i], DAR_3_NOACESS, pdst);
            }
            #endif
            
            return pdst;  /*������Ҫ��Ҫ����ɾܾ���д*/
        }          

        pGet_Normal_Data->CurrentLine = i; /*���£��Ա��´δ��µ�OAD��ʼ����*/  
        
    }

    return pdst;  
}    
/*****************************************************************************
 �� �� ��  : Get_Service_Process
 ��������  : ��ȡ�����������
 �������  : INT8U *pdata:�����ַ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Get_Service_Process(INT8U *pdata)
{    
    INT8U choice;
    INT8U *pdst;

    choice = gServiceChoice;

    if((EN_GET_RECORD == choice)||(EN_GET_RECORD_LIST == choice))
    {
        pdst = get_record_process(pdata);
    }    
    else
    {
        pdst = get_normal_process(pdata);
    }   

    return pdst;
}  


/*****************************************************************************
 �� �� ��  : Encode_SetThenGet_List_Dar
 ��������  : ���SetThenGetListDAR������DAR��û������
 �������  : INT8U *pSetResult  
             INT8U *pdata       
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 03 15
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U * Encode_SetThenGet_List_Dar(INT8U *pSetResult,INT8U *pdata)   
{
    INT8U *pdst = pdata;
    INT8U i; 
    ST_SET_THEN_GET_SERVICE *pSet_Then_Data;

    pSet_Then_Data  = (ST_SET_THEN_GET_SERVICE *)gBak_data;
    
    for(i=0;i<pSet_Then_Data->ListNumber;i++)
    {      
        pdst = EncodeSetDar(&pSet_Then_Data->SetOad[i], pSetResult[i], pdst);  
        pdst = EncodeGetDar(&pSet_Then_Data->GetOad[i],DAR_3_NOACESS,pdst);
    }    

     return pdst;
}    
  
/*****************************************************************************
 �� �� ��  : Set_Service_Process
 ��������  : ���������������
 �������  : INT8U *pdata :�����ַ 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Set_Service_Process(INT8U *pdata)
{
    INT8U i;
    ST_OAD Oad;
    ST_SET_SERVICE *pSet_Data;
    ST_SET_THEN_GET_SERVICE *pSet_Then_Data;
    INT8U *pdst = pdata; 
    INT8U is_need_next_frame;
    
    INT8U SetResult[MAXSETLISTNUMBER];
    INT8U choice;

    choice = gServiceChoice;

    switch ( choice )
    {
        case EN_SET_NORMAL :           
        case EN_SET_NORMAL_LIST :
            pSet_Data  = (ST_SET_SERVICE *)gBak_data;
            for(i=0;i<pSet_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pSet_Data->SetOad[i], (INT8U *)&Oad, sizeof(ST_OAD));

               if(DAR_0_OK!=gDarDecode)
               {
                  SetResult[i] = gDarDecode;
               }

               /*��ʱ�����Ƿ񻺳�Խ��,��Ϊһ�㲻��Խ�磬�������ݿ϶���Щ��*/
               pdst = EncodeSetDar(&Oad, SetResult[i], pdst);               
            }
            break;
            
        case EN_SET_THEN_GET_NORMAL_LIST :
            pSet_Then_Data  = (ST_SET_THEN_GET_SERVICE *)gBak_data;
            /*1.����ȫ��LIST����*/
            for(i=0;i<pSet_Then_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pSet_Then_Data->SetOad[i], (INT8U *)&Oad, sizeof(ST_OAD));
               if(DAR_0_OK!=gDarDecode)
               {
                  SetResult[i] = gDarDecode;
               }
            }

            is_need_next_frame = 0;  /*������0����Ȼ��get_single_normal_process��Ҳ����*/

            for(i=0;i<pSet_Then_Data->ListNumber;i++)
            {
               /*2.�ֿ����SET DAR*/ 
                /*ʡ������ʱ�����Ƿ񻺳�Խ��*/
               pdst = EncodeSetDar(&pSet_Then_Data->SetOad[i], SetResult[i], pdst);  
               
               /*3.GET���ݲ����*/              
               pdst = get_single_normal_process(pSet_Then_Data->GetOad[i],pdst,&is_need_next_frame);

               /*get����һ�£������������л���Խ��ʱ��Ԥ��5���ֽ�*/  
               if((0!=is_need_next_frame) ||(DAR_0_OK!=gDarDecode))
               {        
                  pdst = Encode_SetThenGet_List_Dar(SetResult,pdata);  /*�������֡���������в���*/  
                  
                  return pdst;  /*���濴Ҫ��Ҫ�ĳɴ���ɾܾ���д*/
               } 
            }                       
            break;
        default:
             
            break;
    } 

    return pdst;
}  


/*****************************************************************************
 �� �� ��  : get_oad_action_object
 ��������  : ��ȡ������Ķ�������
 �������  : ST_OAD Oad           
             SUB_OBJECT *pObject  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 08
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U get_oad_action_object(ST_OAD Oad,ACTION_OBJECT *pObject)
{
    INT8U i;
    INT8U AID,OI_NUM;
    INT16U OI;
    ST_OI_METHOD *pOiMethod;
 
    OI  = Oad.Oi.asInt;
    AID = Oad.AId;
    //IID = Oad.IId;

    /*1.����AID*/
    for(i=0;i<ARRAY_SIZE(OI_METHOD_NUM_POINTER_ALL);i++)
    {
        if(AID == OI_METHOD_NUM_POINTER_ALL[i].AID)
        {
            OI_NUM    = OI_METHOD_NUM_POINTER_ALL[i].OI_NUM;
            pOiMethod = (ST_OI_METHOD *)OI_METHOD_NUM_POINTER_ALL[i].Pointer;
            break;
        }    
    }   

    /*AIDû���ҵ�*/
    if(ARRAY_SIZE(OI_METHOD_NUM_POINTER_ALL)<=i)
    {
        return DAR_4_OANODEF;
    }    

    /*2.����OI*/
    for(i=0;i<OI_NUM;i++)
    {
        if(OI == pOiMethod->OI)
        {
           pObject->Pointer = pOiMethod->Pointer;
           pObject->ObjMeth = pOiMethod->ObjMeth;
           return DAR_0_OK;
        }    
        pOiMethod++;
    }  

    return DAR_4_OANODEF;
    
}    


/*****************************************************************************
 �� �� ��  : Init_Money_Packet
 ��������  : Ǯ����ʼ��
 �������  : INT8U len     
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 28
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
    

/*****************************************************************************
 �� �� ��  : Clr_Event_Process
 ��������  : �¼���0����Ϊ�ֿ�ACTION����
 �������  : INT16U Obis  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 28
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Clr_Event_Process(INT16U Obis)
{
    INT8S SRet;
    
    /*���� ˽Կ�²���������0�¼�*/
    if((OI_MTR_CLR_EVENT == CurOad.Oi.asInt)&&(FALSE == ChkStatus(E_FAC)))
    {
        return DAR_15_PSWERR;
    } 
    
    /*1.���ۼƴ���*/
    SRet = Action(E_RCRD_CLR,&Obis,NULL); 
    if(0!=SRet)
    {
        return (0-SRet);
    }  
#ifdef PM_MODLE   
	if(Obis == E_CARD_IN_ERR_EVNT_RCRD)/*"�쳣�忨�¼�����Ƿ�������"*/
	{
		unlawcnt = 0;
		SetSingle(E_UNLAW_PLUG_CNT,(INT8U *)&unlawcnt,4);
	}
#endif
    /*2.���¼�������־���ۼ�ʱ��*/
    SRet = Action(E_MNT_EVNT_CLR,&Obis,NULL); 

    /*3.�����¼��ͼ�¼�¼���0*/
    if(0==SRet)
    {   
        /*�����¼������̱�־*/
        if((OI_PRG_EVENT == CurOad.Oi.asInt)||(CLR_ALL_EVENT_OBIS == Obis))
        {    
            /*���̱�־*/
            Push(E_PRGEVENT_START,FALSE);
        }
        
        SaveRcrd(E_EVNT_CLR_EVNT_RCRD, E_EVTLST_EVNT_CLR);

    }  
	
    return (0-SRet);

}


/*****************************************************************************
 �� �� ��  : Clr_Meter_Process
 ��������  : �����0����Ϊ�����������ֿ���ACTION
 �������  : INT16U Obis  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 28
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Clr_Meter_Process(void)
{   
    /*���ر���������0*/
	

#ifdef PM_MODLE    
    GetSingle(E_METER_MODE, (INT8U *)&MeterType); 

    if((LOCALMETER == (MeterType&METERTPYE))&&(FALSE == ChkStatus(E_FAC)))
    {
        return DAR_3_NOACESS;
    }   
#endif    
    /*�ж����� END*/
    
	CLR_MTR();
    
    /*���̱�־*/
	Push(E_PRGEVENT_START,FALSE);

    return DAR_0_OK;
}



/*****************************************************************************
 �� �� ��  : Check_Relation_Oad_Ok
 ��������  : �жϹ�������OAD�ĺ� ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 01 29
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_Relation_Oad_Ok(INT8U *pdata,ST_OAD Oad,INT16U len)
{
    INT8U  Oi0,i;
    ST_OAD TmpOad;
    SUB_OBJECT object;
    INT8U ret = DAR_0_OK;

	/*ͨ��SET�������ù����������Ա�ʱ��OAD��������Ԫ����������Ϊ00�����򷵻ؾܾ���д��
      ��Ȼ�������Ҳ����������������û��IID������0�ģ����Կ���������*/
    if(0!=Oad.IId)
    {
       return DAR_3_NOACESS; 
    }    

    /*˽Կ�£����������õ����0��������*/
    if((TRUE ==ChkStatus(E_PRIVATE))&&(OI_MTR_CLR_EVENT == Oad.Oi.asInt)) 
    {
        return DAR_3_NOACESS; 
    }    
        
    Oi0 = Oad.Oi.asByte[OIH];
    
    /*����*/
    if(0x50 == Oi0)
    {
        for(i=0;i<len/8;i++)
        {
            LIB_MemCpy(pdata+i*8+2, (INT8U*)&TmpOad, sizeof(ST_OAD));
            Get_OAD_Data_len(TmpOad,&object,&ret);  /*֮���Բ�ֱ����get_oad_object������Ϊ�и�maskҪ��*/
            if(DAR_0_OK !=ret)
            {
                return DAR_3_NOACESS;
            }    
        }    
    }
    else /*�¼�*/
    {
        for(i=0;i<len/4;i++)
        {
            LIB_MemCpy(pdata+i*4, (INT8U *)&TmpOad, sizeof(ST_OAD));
            Get_OAD_Data_len(TmpOad,&object,&ret);
            if(DAR_0_OK !=ret)
            {
                return DAR_3_NOACESS;
            }  
        } 
    }    
    
    return DAR_0_OK;
}    


/*****************************************************************************
 �� �� ��  : Pp_Esam_Action
 ��������  : ESAM�෽�����⴦������ESAM�����з���ֵ�������ã����Խ������⴦��
 �������  : ST_OAD Oad       
             INT8U *psrcdata  
             INT8U *pdstdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 23
    ��    ��   : xyt
    �޸�����   : �����ɺ���

ע:���������ֱ�ӷ��صı��������ݣ����Ƿ������ݽ��
*****************************************************************************/
INT8U* Pp_Esam_Action(ST_OAD Oad,INT8U *psrcdata,INT8U *pdstdata)
{
    INT8S Ret;
    ACTION_OBJECT ActionObject;
    INT8U  DataAction[MAX_DATA_LEN_ACTION];
    
    
    INT8S SRet;
    INT8U *pdst = pdstdata;
    
    //ST_OAD Oad_Tmp;    
    
    /*1.�����Ƿ��ж��壬����ȡ�������ͺ��������ķ���ID*/
    Ret = get_oad_action_object(Oad,&ActionObject);   /*ȷ���Ƿ��ж���*/

    if(DAR_0_OK != Ret)
    {
        pdst = EncodeAcionDar(&Oad, Ret, pdst);   
        return pdst;
    }  

    SRet = Action(ActionObject.ObjMeth,psrcdata,DataAction);
    
    if(0>SRet)
    {
        pdst = EncodeAcionDar(&Oad, (0-SRet), pdst);        
    }    
    else
    {          
#ifdef PM_MODLE            
      OBIS_ESAM = ActionObject.ObjMeth;
        /*ESAM����*/
        if(E_ESAM_SET == OBIS_ESAM)
        {
           Len_data    = DataAction[0];
           pActionData = &DataAction[1];

           LIB_MemCpy((INT8U *)pActionData, (INT8U *)&object.Oad, sizeof(ST_OAD));

           object.Class = 0;                /*����Ϊ0���Ա�����ж�*/
    
           Ret_1 = get_oad_object(&object);   /*ȷ���Ƿ��ж���*/

           if(DAR_0_OK == Ret_1)
           {
               Ret_1 = Set_Object_Single(&object,pActionData+sizeof(ST_OAD),(INT16U)Len_data);
               if(DAR_0_OK == Ret_1)
               { 
                    PP_PrgEvent(object.Oad);
               }     
           }   
           /*����Ҫ��Ҫ���VALUEΪRet_1*/
  		   pdst = EncodeAcionDar(&Oad, DAR_0_OK, pdst);        
        }   
        else
        {      
			if(E_ESAM_GET == OBIS_ESAM)
			{
			   LIB_MemCpy((INT8U *)&DataAction[0],(INT8U *)&Len_data,2);
			   pActionData = &DataAction[2]; 
			}	
			else
			{
			   Len_data = (INT16U)SRet;
			   pActionData = &DataAction[0];  
			}  

			if(Len_data >0)
			{
				pdst  = EnCodeOAD(&Oad,pdst);/*OAD*/
				*pdst++  = DAR_0_OK;
				*pdst++  = 1;				 /*Data  OPTIONAL*/

				LIB_MemCpy(pActionData, pdst, Len_data);
				pdst += Len_data;	 
			}
			else
			{
				pdst = EncodeAcionDar(&Oad, DAR_0_OK, pdst);  /*"�������ݳ���Ϊ0����DATA"*/
			}  
        }    
#endif		        
    }   

    return pdst;
   
}    


/*****************************************************************************
 �� �� ��  : Pp_CodeCmp
 ��������  : �ļ��ȶԣ���ESAMһ����Ҫ������ݣ����⴦��
 �������  : INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 01 22
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U * Pp_CodeCmp(ST_OAD Oad,INT8U *psrcdata,INT8U *pdstdata)
{
    INT16S Len_data;
    INT8U  DataAction[MAX_CODE_CMP_LEN];
    INT8U  Ret = DAR_0_OK;
    INT8U *pdst = pdstdata;
    INT16U length;

    /*�Ƚ����ݺϷ���*/
	Len_data = DeCode_Data((ST_ENCODE_INFO *)ST_5_U8_U8_U32_U32_U16,psrcdata,DataAction,0/*Oad.IId*/);
	if(0>Len_data)
	{
	    Ret  = DAR_5_OANOADAPT;
	} 
    else
    {   

    }

    if(DAR_0_OK!=Ret)
    {    
        pdst = EncodeAcionDar(&Oad, Ret, pdst); 
    } 
    else
    {
		pdst  = EnCodeOAD(&Oad,pdst);/*OAD*/
		*pdst++  = DAR_0_OK;
		*pdst++  = 1;				 /*Data  OPTIONAL*/

	    if(0 == Oad.IId)
	    {
            *pdst++  = structure;
            *pdst++  = 1;
	    }
        
        *pdst++ = octetstring;
			
		length  = (((INT16U)DataAction[1])<<8)+DataAction[0];	

        pdst    += EnCode_Base(&DataAction[2],octetstring,length,pdst);    
        
    }        
    
    return pdst;
    
}    


/*****************************************************************************
 �� �� ��  : Encode_ActionThenGet_List_Dar
 ��������  : ���ActionThenGetListDAR������DAR��û������
 �������  : INT8U *pSetResult  
             INT8U *pdata       
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 03 15
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U * Encode_ActionThenGet_List_Dar(INT8U *pSetResult,INT8U *pdata)   
{
    INT8U *pdst = pdata;
    INT8U i; 
    ST_ACTION_THEN_GET_SERVICE *pAction_Then_Data;

    pAction_Then_Data  = (ST_ACTION_THEN_GET_SERVICE *)gBak_data;
    
    for(i=0;i<pAction_Then_Data->ListNumber;i++)
    {      
        pdst = EncodeAcionDar((ST_OAD*)&pAction_Then_Data->ActionOmd[i], pSetResult[i], pdst);  
        pdst = EncodeGetDar(&pAction_Then_Data->GetOad[i],DAR_3_NOACESS,pdst);
    }    

     return pdst;
}  
/*****************************************************************************
 �� �� ��  : Acion_Service_Process
 ��������  : �������������������
 �������  : INT8U *pdata:�����ַ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Acion_Service_Process(INT8U *pdata)
{
    INT8U i,AId;
    ST_OAD Oad;
    ST_ACTION_SERVICE *pAction_Data;
    ST_ACTION_THEN_GET_SERVICE *pAction_Then_Data;
    INT8U *pdst = pdata;
    INT8U is_need_next_frame;
    
    INT8U ActionResult[MAXACTIONLISTNUMBER];
    INT8U choice;
    INT8U *PointerAction;
    INT8U DataAddr_EsamGet[MAXACTIONLISTNUMBER][MAXESAMGETDATALEN];

    choice = gServiceChoice;

    switch ( choice )
    {
        case EN_ACTION_NORMAL :           
        case EN_ACTION_NORMAL_LIST :
            pAction_Data  = (ST_ACTION_SERVICE *)gBak_data;

            /*2018-03-13 ESAM get�ȱ���POINTER��������ܱ����*/
            for(i=0;i<pAction_Data->ListNumber;i++) 
            {
                LIB_MemCpy((INT8U *)&pAction_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
                if((OI_ESAM == Oad.Oi.asInt)&&(EN_METHOD_3_GET_ESAM==Oad.AId))  
                {
                   LIB_MemCpy(pAction_Data->Pointer[i], DataAddr_EsamGet[i], MAXESAMGETDATALEN);
                }    
            }
            /*ESAM get�ȱ���POINTER��������ܱ����  end*/
            
            for(i=0;i<pAction_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pAction_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
               AId = Oad.AId;
               if(DAR_0_OK!=gDarDecode)
               {
                   ActionResult[i] = gDarDecode;
               }
               else
               {                     
                   PointerAction = pAction_Data->Pointer[i];                   
                  
                   if((OI_ESAM == Oad.Oi.asInt)&&(EN_METHOD_10_INIT_PACKET_ESAM!=AId))
                   {
                       if(EN_METHOD_3_GET_ESAM == AId)   /*ESAM get*/
                       {
                           PointerAction = DataAddr_EsamGet[i];
                       }     
                       
                        /*ESAM�෽��,ֱ�ӷ��صı�����ַ*/ 
                       pdst = Pp_Esam_Action(Oad,PointerAction,pdst);
                       continue;                       
                   }    
                   else if ((OI_FILE == Oad.Oi.asInt)&&(10==AId))
                   {                       
                       pdst = Pp_CodeCmp(Oad,PointerAction,pdst);
                       continue;
                   }
               
               } 
               
               /*��ʱ�����Ƿ񻺳�Խ��*/
               pdst = EncodeAcionDar(&Oad, ActionResult[i], pdst);               
            }
            break;
            
        case EN_ACTION_THEN_GET_NORMAL_LIST :
            pAction_Then_Data  = (ST_ACTION_THEN_GET_SERVICE *)gBak_data;
            for(i=0;i<pAction_Then_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pAction_Then_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
               if(DAR_0_OK!=gDarDecode)
               {
                  ActionResult[i] = gDarDecode;
               }
            }

            is_need_next_frame = 0;  /*������0*/

            for(i=0;i<pAction_Then_Data->ListNumber;i++)
            {
                /*ʡ������ʱ�����Ƿ񻺳�Խ��*/
               pdst = EncodeAcionDar(&Oad, ActionResult[i], pdst);  
               
               pdst = get_single_normal_process(pAction_Then_Data->GetOad[i],pdst,&is_need_next_frame);

               /*get����һ�£������������л���Խ��ʱ��Ԥ��6���ֽ�*/  
               if((0!=is_need_next_frame) ||(DAR_0_OK!=gDarDecode))
               {
                  pdst = Encode_ActionThenGet_List_Dar(ActionResult,pdata);  /*�������֡���������в���*/                  

                  return pdst;  
               } 
            }                       
            break;
        default:
             
            break;
    }   

    return pdst;
}  


/*****************************************************************************
 �� �� ��  : Process_698
 ��������  : 698��������
 �������  : INT8U servicetype  
             INT8 *pdata        
 �������  : ��
 �� �� ֵ  : ���ر����ַ���Ժ������ʱ���ǩ������ϱ�����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Process_698(INT8U servicetype,INT8U *pdata)
{
    INT8U *pFlowReport = pdata;   /*���ǳ�ʼ���°ɣ���ֹ�쳣��*/
    
    switch(servicetype)
    {
        case EN_GETREQUEST:
            pFlowReport = Get_Service_Process(pdata);
            break;
        case EN_SETREQUEST:
            pFlowReport = Set_Service_Process(pdata);
            break;
        case EN_ACTIONREQUEST:
            pFlowReport = Acion_Service_Process(pdata);
            break;
        default:
            break;
    }

    return pFlowReport;
} 

#ifdef DM_DLT698
#if 0
const ST_ROAD_SHOW_INFO mstROadShowInfo[] = 
{
    	{0x0000, E_INFO_LAST_TRIFF_A_ENGY},   /*��*��  ���� �й�     ����*/
    	{0x0010, E_INFO_LAST_TRIFF_PA_ENGY},  /*��*��  ���� �����й� ����*/
    	{0x0020, E_INFO_LAST_TRIFF_NA_ENGY},  /*��*��  ���� �����й� ����*/
    	#ifdef THREE_PHASE
    	{0x0030, E_INFO_LAST_COMM_TRIFF_R_1_ENGY},/*"��*����Ϸ��� �޹� 1  ����"*/
        {0x0040, E_INFO_LAST_COMM_TRIFF_R_2_ENGY},/*"��*����Ϸ��� �޹� 2  ����"*/
        {0x0050, E_INFO_LAST_COMM_TRIFF_R_I_ENGY},  /*"��N�·��� I�����޹� ����"*/
        {0x0060, E_INFO_LAST_COMM_TRIFF_R_II_ENGY}, /*"��N�·��� II�����޹� ����"*/
        {0x0070, E_INFO_LAST_COMM_TRIFF_R_III_ENGY},/*"��N�·��� III�����޹� ����"*/
        {0x0080, E_INFO_LAST_COMM_TRIFF_R_IV_ENGY}, /*"��N�·��� IV�����޹� ����"*/        
        #endif
        {0x202C, E_INFO_REMAIN_MONEY},        /*ʣ�� ���*/
    	{0x2031, E_INFO_LAST_SUM_ENGY},       /*��*��  ���� ���� */
    	{0x2032, E_INFO_LAST_STEP_SUM_ENGY},  /*�� ���� ���õ���*/
    	//{0x201E, 0x16},/*"�¼�����ʱ��"*/
};
/*����ROAD����ʾ��Ϣ*/
static INT8U GetROadShowInfoId(UN_OI *pOi)
{
    INT8U i;

    for(i = 0; i < ARRAY_SIZE(mstROadShowInfo); i++)
    {
        if(pOi->asInt == mstROadShowInfo[i].Oi.asInt)
        {
            return (mstROadShowInfo[i].ShowinfoId);
        }
    }
    return (E_INFO_NULL); /*"����ʾ��Ϣ"*/
} 
#endif

/*****************************************************************************
 �� �� ��  : PP_GetOadToObisShow
 ��������  : ����IID��ȡ��Ӧ��OBIS�б��Լ���ʽ�б�
 �������  : INT8U IID                            
             INT16U *pObis                        
             ST_PARAMENT_OAD_OBIS_INFO *pObis_Info  
             ST_FMT_ATTR *pFormat                 
 �������  : ��
 �� �� ֵ  : OBIS����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 12
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U PP_GetOadToObisShow(INT8U IID,INT8U num_obis,INT16U *pObis,ST_PARAMENT_OAD_OBIS_INFO *pObis_Info,INT8U *format_id,INT8U *ShowInfoId,INT8U *comm_id)
{
    INT8U i,j,temp_iid,num_ms_obis,Tmp_num_obis,ShowinfoId;
    const ST_FORMAT_ID *pObisFmt;    
    INT16U temp_Obis;
    INT8U f_block_data = 0;
    INT8U Offset_Format;
    
    //num_ms_obis = pObis_Info->num;
    
    temp_iid   = 1;      
    
    for(i=0;i<num_obis;i++)
    {
        temp_Obis   = pObis_Info[i].Obis;
        Tmp_num_obis = pObis_Info[i].num;
        
        if(Tmp_num_obis>E_SEASON_SLOT_ID)
        {
           temp_Obis++;     /*�����ݣ�OBISҪ��1*/
           
           Fix_Len_Varible = 0xff;   /*PP_GetOadToObisList()�и���ComPort��ȡ���ݸ�ʽ*/
           num_ms_obis     = LenGet_PP_698((ENUM_DATALEN_ID)Tmp_num_obis);   
           Fix_Len_Varible = 0; 
           
           f_block_data = 0xff;
        } 
        else
        {
            num_ms_obis = Tmp_num_obis;
        }    
        
        for(j=0;j<num_ms_obis;j++)        
        {                                
            if(temp_iid==IID)                
            {
                *pObis = temp_Obis;    
                
                /*�������ݴ����һ����������������ʾ�봫����*/    
                /*ָ�����´��Ա�ֻȡOBIS����ȡ��ʽ*/    
                /*FormatId���´�*/ 
                if(0!=f_block_data)
                {
                   Offset_Format = 0;    /*2018-03-05:Offset_Format�ǲ���Ӧ�þ�ȡi�Ϳ�����,Ϊʲô�����?��Ҫ��Ϊ�˷�ֹԽ��*/
                }
                else
                {    
                   Offset_Format = i;                       
                }
                                            
                if(MAX_FORMAT_ID>pObis_Info[Offset_Format].FormatId)
                {    
                    pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[Offset_Format].FormatId];
                    
                    *format_id = pObisFmt->ShowId;  
                    *comm_id   = pObisFmt->TransId;
                }                
                else
                {
                    *format_id = MAX_FORMAT_ID;
                    *comm_id   = MAX_FORMAT_ID;                
                }   

                ShowinfoId = pObis_Info[Offset_Format].ShowinfoId; 
                #ifdef THREE_PHASE
                if(((E_PHASE_VARIBLE_ID == Tmp_num_obis)||(E_PHASE_VARIBLE_SUM_ID == Tmp_num_obis))&&(2<=IID))  
                {
                   *ShowInfoId = ShowinfoId + IID-1; 
                }                
                else
                #endif    
                {
                    *ShowInfoId = ShowinfoId;
                }    
                
                return 1;
            }    

            /*�Զ��ӣ���Ϊ������ֵ*/  
            temp_Obis++;
            
            temp_iid++;
            
       } 
        
    }    

     return 0;//temp_iid;    
}    

#ifdef THREE_PHASE
const  INT8U demand_disp_format[]   ={E_FORMAT_UBCD_3_KW_DATUO,E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0};
const  INT8U r_demand_disp_format[] ={E_FORMAT_BCD_3_KVAR_DATUO,E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0};
const  INT8U volt_stat_disp_format[]={E_FORMAT_UBCD_4_NONE_0,E_FORMAT_UBCD_3_NONE_2,E_FORMAT_UBCD_3_NONE_2,E_FORMAT_UBCD_4_NONE_0,E_FORMAT_UBCD_4_NONE_0};
const  INT8U volt_stat_len[]        ={0,4,2,2,4,4};
const  INT8U volt_stat_Comm_Format[]={E_FORMAT_U_4_NONE_0,E_FORMAT_U_2_NONE_2,E_FORMAT_U_2_NONE_2,E_FORMAT_U_4_NONE_0,E_FORMAT_U_4_NONE_0};
#endif

const ST_DISP_FORTMAT_SRCNO disp_format_list[]=
{
    {E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0},        /*FORMAT_DATETIME*/
    {E_FORMAT_ID_CODE_2_NONE_0,E_FORMAT_ID_CODE_4_NONE_0},  /*FORMAT_ADDR_H*/
    {E_FORMAT_U_H_INT_KWH_0,E_FORMAT_U_H_DECIMAL_KWH_MATUO},
    {E_FORMAT_S_H_INT_KWH_0,E_FORMAT_S_H_DECIMAL_KWH_MATUO},
#ifdef THREE_PHASE  
    {E_FORMAT_U_H_INT_KVARH_0,E_FORMAT_U_H_DECIMAL_KVARH_MATUO},
    {E_FORMAT_S_H_INT_KVARH_0,E_FORMAT_S_H_DECIMAL_KVARH_MATUO},
#endif  
};    


/*****************************************************************************
 �� �� ��  : check_src_no_format
 ��������  : ��������ţ���������OAD����ʾ����
 �������  : INT8U scrNo      
             INT8U format_id  
 �������  : ��
 �� �� ֵ  : ����Ӧ�ĸ�ʽ
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 26
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U check_src_no_format(INT8U scrNo,INT8U format_id)
{
    INT8U i;
        
    for(i=0;i<ARRAY_SIZE(disp_format_list);i++)
    {
        if(disp_format_list[i].format_1 == format_id)
        {               
            return *(&disp_format_list[i].format_1 + scrNo-1);
        }    
    }    

    return format_id;
}  


/*****************************************************************************
 �� �� ��  : PP_Get_Special_Show_Data
 ��������  : ����Ƿ�һЩ�������ʾ���ݣ��Ա��ʽת����������ȡ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 25
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void PP_Check_Special_Show_Data(SUB_OBJECT *pobject,INT8U scrNo,INT8U *pformat_id,INT8U *pData_len,INT8U *pdata)
{
    INT8U AId,ClsId;
    INT16U Duration_OBIS = NULL;
    #ifdef THREE_PHASE
    INT16U Record_Obis;
    INT8U TmpData[32],offset = 0,i;
    #endif
    ST_OAD Oad;
    

    LIB_MemCpy((INT8U *)&pobject->Oad, (INT8U *)&Oad, sizeof(ST_OAD));

    AId  = Oad.AId;
    ClsId= pobject->Class;

    #ifdef THREE_PHASE
    /*��ѹ�ϸ���*/
    if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
    {                  
        *pformat_id = volt_stat_disp_format[scrNo-1];
        Duration_OBIS = E_DA_VOLTHEALTH + ((Oad.Oi.asInt-OI_VOLT_STAT_A_RT)<<1) + (Oad.IId-1);
        GetSingle(Duration_OBIS,TmpData);  
        for(i=0;i<scrNo;i++)
        {    
            offset += volt_stat_len[i];
        }
        LIB_MemCpy(TmpData+offset, pdata, volt_stat_len[scrNo]);  
        *pData_len = volt_stat_len[scrNo];
        return;
    }    
    
    #endif
    
    

    /*ʱ��״̬��¼�� ���һ�η���ʱ��ͽ���ʱ��*/
    if(((24 == ClsId)&&(0x0E==AId))||((7 == ClsId)&&(0x0A==AId)))
    { 
        *pData_len = 6;
        if((3 == scrNo)||(4 == scrNo))
        {
            LIB_MemCpy(&pdata[6], &pdata[0], 6); 
            if(4 == scrNo)
            {
               *pformat_id = E_FORMAT_HHMMSS_NONE_0;    
            }    
        }    
    }  
    /*��ǰֵ��¼�� �ڶ����ۼ�ʱ��*/ /*24������10��7������7*/
    else if((2 == scrNo)&&
            (((24 == ClsId)&&(0x0A==AId))||((7 == ClsId)&&(0x07==AId))))
    {
        *pData_len = 4; 
         #ifdef SINGLE_PHASE
         if(24 == ClsId)
         {            
            Duration_OBIS = E_OVER_CURR_LA_DURATION;                            
         }
         else
         {
            Duration_OBIS = E_PWR_DOWN_DURATION;
         }  
         #else
         if(24 == ClsId)
         {                     
            Oad.AId = EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE+Oad.IId-1;                     
         }
         else
         {
            Oad.AId = EN_CLS7_ATT_2_EVENT_RECORD_TABLE;
         }   

         Oad.IId = 0;   /*Ҫ���0������PP_OADTOOBIS���ܲ���*/
         
         PP_OadToObis(&Oad, &Record_Obis, NULL, NULL);
            
         Duration_OBIS = Get_Duration_OBIS(Record_Obis);
         #endif
         
         if(NULL == Duration_OBIS)
         {
             LIB_MemSet(0, pdata, sizeof(INT32U));
         } 
         else
         {
             GetSingle(Duration_OBIS,pdata);   
         }      
    }  
}    



/*****************************************************************************
 �� �� ��  : PP_Set_Show_Char_Info
 ��������  : ����OAD��ROAD����Һ����ʾ��Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 05 14
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
#ifdef THREE_PHASE
const INT8U CHAR_INFO_ABC_MASK[]={0x40,0x80,0x08};
#endif
void PP_Set_Show_Char_Info(ST_DISP_OBJ *pdata_in,INT8U *pschar)
{
    INT16U Oi;
    #ifdef THREE_PHASE
    INT8U OIB2;
    INT8U AId,IID;
    AId = pdata_in->Oad.AId;
    IID = pdata_in->Oad.IId;
    #endif
    
    Oi  = pdata_in->Oad.Oi.asInt;
    
    /*���⴦���£���¼�����ݶ�����ʾ��ǰ*/
    if(0!=pdata_in->releOad.AId)
    {
        pschar[0] &= ~(0x10); 
    }    
    
    /*��������*/
    if((OI_BILL_FRZ==Oi)||(OI_MONTH_FRZ==Oi))       
    {
       pschar[0] |= 0x22;
    } 

    #ifdef THREE_PHASE
    /*���������ʾABC*/
    if(OI_NAP_ENGY_LC>=Oi)
    {
        OIB2 = Oi&0x000F;
        if(EN_POWER_OIB2_SUM != OIB2)
        {
            pschar[0] |= CHAR_INFO_ABC_MASK[OIB2 -EN_POWER_OIB2_A];     
        }         
    }    
    
     /*ABC��ʧѹ/���ܴ���*/ /*ABC��ʧѹ/�����ۼ�ʱ��*/
     if(((3 == AId)||(10 == AId))&&((OI_LOST_VOLT_EVENT == Oi)||(OI_LOST_CURR_EVENT == Oi)))
     {
        if(1!=IID)
        {    
            if(OI_LOST_VOLT_EVENT == Oi)
            {
               pschar[3] |= 0x2C;        /*��ʧѹ*/
            }   
            
             
            if(OI_LOST_CURR_EVENT == Oi)
            {
               pschar[3] |= 0x28;        /*��ʧ*/
               pschar[4] |= 0x40;        /*��*/
            }  

            if((10 == AId)&&(2 == pdata_in->scrNo))
            {
               pschar[4] |= 0x0c;       /*ʱ��*/ 
            }  

            pschar[0] |= CHAR_INFO_ABC_MASK[IID -2];     
        }
     }   
     
    
    /*7���¼�*/
    if((OI_MEASURE_CHIP_ERR_EVENT>=Oi)&&(OI_OVER_PA_DEMAND_EVENT<=Oi)&&
        (EN_CLS7_ATT_2_EVENT_RECORD_TABLE==AId))
    {
       pschar[0] |= 0x20;   /*��*/ 
    }
        
    
    /*�����ϣ�ABC ʧѹ�� */
    if((OI_OVER_PWR_EVENT>=Oi)&&(OI_LOST_VOLT_EVENT<=Oi)&&
        (EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE<=AId)&&
        (EN_CLS24_ATT_9_EVENT_RECORD_4_TABLE>=AId))
    {
        pschar[0] |= 0x20;   /*��*/

        /*����ABC*/
        pschar[0] |= CHAR_INFO_ABC_MASK[AId-7];        

        /*ʧѹ�¼�������ʧѹ*/
        if(OI_LOST_VOLT_EVENT == Oi)
        {
           pschar[3] |= 0x0C; 
        }   
        
        /*ʧ���¼�������ʧ��*/
        if(OI_LOST_CURR_EVENT == Oi)
        {
           pschar[3] |= 0x08;        /*ʧ*/
           pschar[4] |= 0x40;        /*��*/
        }    

        
    }   
    #endif
}    

/*��ʾ���ݸ�ʽת����������*/
/*"code size  284byte"*/
INT8U PP_Data_Transfer_Show(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff,
		                 ST_SHOWDATA_OUTPUT *Dbuff,const ST_FMT_ATTR FAR_POINTER*Communal_Show,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen)
{
    ST_FMT_ATTR saveFormat;
    ST_FMT_ATTR showFormat;
	INT8U len;
	INT8U *Data_buff;
	INT8U engyPoint;
	INT8U pwrPoint;
	
	Data_buff = (INT8U *)&Dbuff->showDataBuff;
	saveFormat = *Communal_Save;
	
       GetSingle(E_ENGY_SHOW_POINT, &engyPoint);
       GetSingle(E_PWR_SHOW_POINT, &pwrPoint);

	if(Communal_Show->exp == E_MATUO)
	{
		if(engyPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - engyPoint);
		}
		else
		{
		    showFormat.exp = -2;
		}
	}	        		
	else if(Communal_Show->exp == E_DATUO)
	{
		if(pwrPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - pwrPoint);
		}
		else
		{
		    showFormat.exp = -4;
		}
	}
	else
	{
	    showFormat.exp = Communal_Show->exp;
	}
	
	/*"û����ʾ��ʽʱ��ʹ��ͨ�Ÿ�ʽ"*/
	if(Communal_Show->format == 0) 
	{
	    showFormat.format = Communal_Trans->format;
	    showFormat.unit = Communal_Trans->unit;
	}
	else
	{
		showFormat.format = Communal_Show->format;
		showFormat.unit = Communal_Show->unit;
	}	

	len = FRMT_sizeofFormat(showFormat.format);

	if(len <= MAX_SHOWDATA)/*"��ʾ���ݽ��ջ�����Խ����"*/
    {

        len = FRMT_DataChange(Sbuff, Data_buff, saveFormat, showFormat);

        if((showFormat.format > E_TIME_FORMAT) && (showFormat.format < E_STRING))/*"time format"*/
        {/*"ʱ���ʽתBCD����ʾ"*/
            LIB_CharToBcdNByte(Data_buff, len);
        }

        Dbuff->stShowTable.stShowFormat = showFormat;
    }
	else
	{
	    len = 0;
	}

	return len;	
}
/*"*****************************************************************************"*/
/*"  Function:       PP_ShowGetData"*/
/*"  Description:    ��ʾ�����ݽӿں�������ʾģ��ͨ�����øýӿں�����ȡ���ݱ�ʶ��Ӧ����"*/
/*"  Calls:          "*/
/*"  Called By:      ��ʾ����ģ��"*/
/*"  Input:          pdata_in   ��ʾ���ݱ�ʶ��Ҷ�Ӻ�                 "*/
/*"                  "*/
/*"  Output:         pdata_out  ��ʾ�������ݼ���ʾ��Ϣ�����ݸ�ʽ"*/
/*"  "*/
/*"  Return:         0  ȡ�����ɹ�    len ȡ�����ݳ��ȣ�ȡ���ɹ�"*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
const INT8U MAX_STEP_PRICE_SRC_NO[]={6,7,TARIFF_NUM};
const INT32U BAND_DATA_COMM[]={0x300,0x600,0x1200,0x2400,0x4800,0x7200,0x9600,0x19200,0x38400};  /*BCD��ʽ��ʾ*/
INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,ST_SHOWDATA_OUTPUT *pdata_out)	            
{
    INT8U num_obis,len,data_len = 0,ShowInfoId;   /*"��ʾ���ñ�ID"*/
    INT16S SRet;
    ST_FMT_ATTR Format_show;   /*��ʾ��ʽ*/
    ST_FMT_ATTR Format_Block;  /*�洢��ʽ*/
    ST_FMT_ATTR Format_comm;   /*ͨ�Ÿ�ʽ*/
    
    INT16U Obis[MAX_ID_LIST_NUM],Oi;
    INT8U temp_data[MAX_ONE_OAD_DATA_LEN];
    
    ST_OAD Oad;        /*OAD*/
    INT8U AId;         /*"���Ա�ʶ"*/
   
    INT8U  scrNo;      /*"�����"*/
    SUB_OBJECT object;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];
    INT8U format_id;       /*��ʾ��ʽID*/
    INT8U Comm_Format_id;  /*ͨ�Ÿ�ʽID*/

    #ifdef THREE_PHASE
    INT8U i,offset = 0;
    INT8U temp_data2[8];   /*��������ĸ��ֽڣ������ĸ��ֽ�*/
    #endif

    /*ע��:�޸�OBIS�б����ʾ��ʽ��Ҫͬ���޸Ķ�Ӧ��OBIS������洢��ʽ���ԵĻ������ܸ�ʽת������������ʾ������*/
    
    scrNo   = pdata_in->scrNo;

    AId = pdata_in->Oad.AId;
    Oi  = pdata_in->Oad.Oi.asInt;
    
    /*���´�*/
    if((0 == AId)||(0xff == AId))
    {
        return 0;
    }  
    /*24�࣬�󲿷�û����*/
    if((OI_OVER_PWR_EVENT>=Oi)&&(OI_LOST_VOLT_EVENT<=Oi)&&((3 == AId)||(10 == AId)||(14 == AId)))
    {
        if((OI_PWR_REVERSE_EVENT!=Oi)&&(1>=pdata_in->Oad.IId))
        {
            return 0;
        }    
    }    


    /*2017-01-30:�̶�����ǰ��16�ֽڣ���ֹ��ASCII����л�ʱ��ΪFFʱ������������*/
    LIB_MemSet(0, temp_data, 16);
    
    /*��¼������*/
    if(0!=pdata_in->releOad.AId)
    {
        //LIB_MemCpy((INT8U*)&pdata_in->Oad, (INT8U*)&Road, sizeof(ST_OAD));
        data_len = PP_RecordColumnFilterDisp(&pdata_in->Oad,&pdata_in->releOad,temp_data);
        if(0 == data_len)
        {   
            LIB_MemSet(0, temp_data, MAX_SHOWDATA);
            data_len = MAX_SHOWDATA;
            //return MAX_SHOWDATA;   /*����ʱдΪ0*/
        }

        /*��COPY�����ʽ*/
        LIB_MemCpy((INT8U*)&pdata_in->releOad, (INT8U*)&Oad, sizeof(ST_OAD));
        Oad.AId = Oad.AId & MASK_AID_SELECT;    /*Ҫ��������ܵõ���ȷ��ʽ*/            
    }    
    else
    {
        LIB_MemCpy((INT8U*)&pdata_in->Oad, (INT8U*)&Oad, sizeof(ST_OAD));
    }

    if(0 == Oad.IId)
    {
        Oad.IId = 1;
    }    
    
    object.Class = 0;                    /*����Ϊ0���Ա�����ж�*/

    LIB_MemCpy((INT8U*)&Oad,(INT8U *)&object.Oad ,sizeof(ST_OAD));

    /*ȷ���Ƿ��ж���,ͬʱ��ȡԪ��λ��*/
    if(DAR_0_OK != get_oad_object(&object))   
    {
        return 0;
    }           
    
    num_obis = PP_OadToObis_Info(&object,Obis_Info);

    if(0 ==PP_GetOadToObisShow(Oad.IId,num_obis,Obis,Obis_Info,&format_id,&ShowInfoId,&Comm_Format_id))
    {
        return 0;
    }    

    /*�����Ƿ���������ʾ������*/
    #ifdef THREE_PHASE
    /*��ѹ�ϸ���*/
    if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
    {
        if((0==scrNo)||(5<scrNo))
        {
            scrNo = 1;
        }   
    }
    #endif

    /*��¼�����ݣ���������ȡ��*/
    if(0==pdata_in->releOad.AId)  /*��ͨ����*/
    {   
        /*���ݱ��OBISҪ���⴦����*/
        if((OI_CURRNET_STEP_PRICE == Oad.Oi.asInt)||
           (OI_BAK_STEP_PRICE == Oad.Oi.asInt))
        {       
           /*��һ������źϷ���*/
           if(scrNo>MAX_STEP_PRICE_SRC_NO[Oad.IId-1])
           {
             return 0;   
           }  
           else if(0 == scrNo)   /*�������û�п���Ϊ0?*/
           {
              scrNo = 1;  
           }             
           
           Obis[0] += scrNo;            
        } 
        
        SRet = GetSingle(Obis[0],temp_data);            
        if(0>SRet)
        {
            return 0;
        }   
        else
        {        
            /*�͹����£�˲ʱ����������ʾΪ0*/
            if(TRUE == ChkStatus(E_LOW_PWR))
            {    
                if((3 == object.Class)||(4 == object.Class))
                {
                    LIB_MemSet(0, temp_data, 16);
                } 
            }
            
            data_len = (INT8U)SRet;  

            /*��������ʾ�޸�*/
            #ifdef THREE_PHASE
            if((E_RS485_1_BAUDRATE == Obis[0])||(E_IR_BAUDRATE == Obis[0])||
               (E_RS485_1_BAUDRATE == Obis[0])||(E_RS485_2_BAUDRATE == Obis[0]))
            #else
            if((E_RS485_1_BAUDRATE == Obis[0]))
            #endif
            {
               if(temp_data[0]<=6)
               { 
                   *(INT16U *)temp_data = BAND_DATA_COMM[temp_data[0]]; 
                   data_len = 4;
               }
               else if(temp_data[0]<=8)
               {
                   *(INT32U *)temp_data = BAND_DATA_COMM[temp_data[0]]; 
                   data_len = 5; 
               } 
            }   
            
            
            PP_Check_Special_Show_Data(&object,scrNo,&format_id,&data_len,temp_data);
            
            
        }    
    }    
    else
    {
        #ifdef THREE_PHASE
        /*��ѹ�ϸ���*/
        if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
        {                  
            format_id = volt_stat_disp_format[scrNo-1];
            for(i=0;i<scrNo;i++)
            {    
                offset += volt_stat_len[i];
            }
            
            data_len = volt_stat_len[scrNo];
            
            LIB_MemCpy(temp_data+offset, temp_data2, data_len);  
            LIB_MemCpy(temp_data2, temp_data, data_len);            /*��COPY����*/ 
        }    
        
        #endif
    }    

    /*�ڶ���������Ҫ�����ʽ*/
    if(2==scrNo)
    {    
        format_id = check_src_no_format(scrNo,format_id);            
    }  

    #ifdef THREE_PHASE
    /*������ʾ��ʽת��*/
    if(2 == object.Class)
    {        
        if((3 < scrNo)||(0 == scrNo))
        {
            scrNo = 1;
        }  
        if(Oad.Oi.asByte[OIL]<0x30)
        {    
            format_id  =  demand_disp_format[scrNo-1];
        }
        else
        {    
            format_id  =  r_demand_disp_format[scrNo-1];
        }

        if(2<=scrNo)
        {
            ShowInfoId = ShowInfoId+1;    
        }    
    } 
    #endif    

    if(MAX_FORMAT_ID!=format_id)
    {        
         LIB_MemCpy((INT8U *)&stFormat[format_id], (INT8U *)(&Format_show), sizeof(ST_FMT_ATTR));
         LIB_MemCpy((INT8U *)&stFormat[Comm_Format_id], (INT8U *)(&Format_comm), sizeof(ST_FMT_ATTR));

         #ifdef THREE_PHASE
         /*��ѹ�ϸ���*/
         if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
         {
             LIB_MemCpy((INT8U *)&stFormat[volt_stat_Comm_Format[scrNo-1]], (INT8U *)(&Format_Block), sizeof(ST_FMT_ATTR));
         }
         else
         #endif         
         GetFrmt(Obis[0],&Format_Block);

          //len = FRMT_NdataChange(temp_data,pdata_out->showDataBuff,Format_Block,Format_list,(INT16U)SRet);
         if((pdata_in->Oad.Oi.asInt >= 0x4001) && (pdata_in->Oad.Oi.asInt <= 0x4003))/*"ͨ�ŵ�ַ����š� �ͻ����"*/
         {
             LIB_RvsSelf(temp_data, data_len);
         }

         len = PP_Data_Transfer_Show(&Format_Block,temp_data,pdata_out,&Format_show,&Format_comm,data_len);
          
    }
    else
    {   
        len = data_len;
        LIB_MemCpy(temp_data,pdata_out->showDataBuff,len);
        LIB_MemCpy((INT8U *)&stFormat[E_FORMAT_NONE_6], (INT8U*)&pdata_out->stShowTable.stShowFormat, sizeof(ST_FMT_ATTR));
    }   


    /*"�����ʾ������Ϣ,��ʾ��ʽ��Ϣ��Data_Transfer_Show���Ѿ�����"*/
    LIB_MemCpy((INT8U *)&stShowinfo[ShowInfoId].schar[0],
                 (INT8U *)&pdata_out->stShowTable.schar[0], MAX_SCHAR_NUM); 

    /*-----------------------------*/
    PP_Set_Show_Char_Info(pdata_in,pdata_out->stShowTable.schar);   
    
    return len;
}    

#endif


/*"*****************************************************************************"*/
/*"  Function:       SetSystemTime"*/
/*"  Description:    ��������д�����������·��������ݽ��кϷ��Լ�飬������Уʱ�¼���¼"*/
/*"  Calls:          SaveRcrd��action"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata �·��������ݵ�ַָ��  "*/
/*"  Output:         ��"*/
/*"  "*/
/*"  Return:         TRUE �������ò��ɹ�  FALSE �������óɹ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/ 
INT8U Pp_SetSystemTime(INT8U *pTime)
{
    if(TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)pTime))/*"data valid check"*/
    {
        /*"��Ϊ����Ҫ��¼Уʱ�¼�����������輰ʱ����ϵͳʱ��"*/
        SaveRcrd(E_JUST_TIME_EVNT_RCRD, E_EVTLST_JUSTTIME_S);

        Action(E_LRM_JUSTIME,pTime,NULL);   /*Уʱ,��ʱû����дʱ���Ƿ�ɹ�*/

        SaveRcrd(E_JUST_TIME_EVNT_RCRD, E_EVTLST_JUSTTIME_E);   
            
        return DAR_0_OK; 
       
    }
    else
    {
        return DAR_9_DBNOVAILD;
    }
   
}


/*****************************************************************************
 �� �� ��  : BeforeFunSetSystemTime
 ��������  : ����ʱ��дǰ����ʵ��
 �������  : INT8U *pdata  
 �������  : ��
 �� �� ֵ  : DAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 02 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U BeforeFunSetSystemTime(INT8U *pdata)
{
    INT8U rsult;
    ST_6TIME_FORM  tempT[2];  /*:Уʱǰʱ�䣬1:Уʱ��ʱ��*/
    ST_7TIME_FORM TmpstSysTime;

    GetSingle(E_SYS_TIME, (INT8U *)&TmpstSysTime);
    
    LIB_MemCpy((INT8U *)&TmpstSysTime, (INT8U *) &tempT[0], sizeof(ST_6TIME_FORM));
    
    rsult = Pp_SetSystemTime(pdata);
    if(DAR_0_OK == rsult)
    {
        LIB_MemCpy((INT8U *)pdata, (INT8U *)&tempT[1], sizeof(ST_6TIME_FORM));
#ifdef PM_MODLE        
        Action(E_TRSTEP_SWCHECK, NULL, NULL);
#endif
        Action(E_RCRD_TIME_ADJUST, (INT8U *) &tempT[0], NULL);
        
        #ifdef THREE_PHASE
        Action(E_CURRENT_DEMAND_CLR, NULL, NULL);
      	//Action(E_VOLTHEALTH_ADJUST_TIME, (INT8U *) &tempT[0], NULL);	
        #endif
    }

    return rsult;
    
}     

#ifdef THREE_PHASE
/*****************************************************************************
 �� �� ��  : BeforeFunSetDuPara
 ��������  : ������������дǰ����
 �������  : INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 10
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U BeforeFunSetDuPara(INT8U *pdata)
{
    INT8U rsult,mode_value,demand_value,DataTmp;
    
    DataTmp = *pdata;

    if((0 == DataTmp)||(DataTmp>60))
    {
        return DAR_FF_OTHERERR;
    }    
    
    if(OI_DEMAND_PERIOD == CurOad.Oi.asInt)
    {
        GetSingle(E_DEMAND_SLID_PERIOD, &demand_value);        
        mode_value = DataTmp%demand_value;
    }
    else
    {
        GetSingle(E_DEMAND_PERIOD, &demand_value);
        mode_value = demand_value%DataTmp;      
    }    

    if(mode_value)
    {    
        rsult = DAR_FF_OTHERERR;
    }
    else
    {
        rsult = DAR_0_OK;    
    }    

    return rsult;
}    
#endif

