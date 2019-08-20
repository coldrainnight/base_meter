/*"*****************************************************************************"*/
/*"  FileName: pp_code.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ����뽻���������ļ�   "*/
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
#include "app\userinterface\ProtocolStack\PS_698\AL_698.h"
#include "App\LegallyRelevant\SC.h"
#include "App\Message\MS.h"
#include "App\UserInterface\UI.h"

/*"*******************************************************************************"*/
INT8U ComPort = 0;   /*ͨ�Ŷ˿ں�*/

INT8U Fix_Len_Varible = 0;   /*�䳤���ݣ�Ҳ���ù̶�����: =0xffȡ�̶�ֵ*/

INT8U LenSingleData = 14;   /*���λ�ȡ�����ݳ���*/
ST_OAD CurOad;      /*��ǰ����(������������������)��ȫ�ֱ���OAD*/

/*��������������Ƿ�Ҫ����RECORDLISTNUM����*/
INT8U gDarDecode = DAR_0_OK;  /*�����������ʽ���󣬻�LIST NUMԽ�� ����������ȼ���ߣ����ʱ���������Ϊ׼*/
 

/*�̶����������ʾ*/
const INT8U FIX_MAX_PARAMENT_NUM[]={SEASON_SLOT_NUM,DAY_TBL_NUM,DAY_SLOT_NUM,63,SPEC_DAY_NUM};

/*������������濴�ǲ���д��ȡ������ַ���ݼ���*/
INT16S LenGet_PP_698(ENUM_DATALEN_ID Id)
{
    INT8U RetLen;
    ENUM_DATALEN_ID Id_temp;
    INT16U Oi;
    
    Oi      = CurOad.Oi.asInt;
    
    Id_temp = Id;
    if(E_TRIFF_ID_PHASE == Id)
    {
        Id_temp = E_TARIFF_ID;
    }   
    
    if(E_PHASE_VARIBLE_ID == Id_temp)   /*��������*/
    {        
        RetLen = CLASS3_MAX_IID;
    }    
    else if(E_PHASE_VARIBLE_SUM_ID == Id_temp) /*�ܼ���������*/
    {
        RetLen = CLASS4_MAX_IID;
     }    
    else if((E_RELATION_OBJECT_ID == Id_temp)||(E_TRIFF_ID_PHASE == Id_temp)) /*��������ͷ��ʵ��*/
    {
        if((OI_NEW_NEED_RPT_EVT_LIST == Oi)&&(3==CurOad.AId))  /*���ϱ��¼��б�����ΪOI�����ֽ�*/
        {
            return LenSingleData>>1;
        }
        else
        {    
            return LenSingleData>>2;
        }
    }   
    else if(E_RELATION_FREEZE_OBJECT_ID == Id_temp)
    {
        return LenSingleData>>3;
    }      
    else if(E_BILL_DATE_ID == Id_temp)
    {
        return 3;
    }    
    else
    {   
            return (-1);   /*�����ǲ��Ǹĳ�0��Щ?*/
    }

   
    /*�����ܵĸ���*/
    if(E_TARIFF_ID == Id)
    {
        if(TARIFF_NUM<RetLen)  /*ȡ��ͷ��ʲ�����С��ֵ*/
        {
            RetLen = TARIFF_NUM;
        }    
        RetLen++;        
    }  
    
    
    return RetLen;
}
 

/*****************************************************************************
 �� �� ��  : ConvertOAD
 ��������  : ����OAD�������ģʽ�ĳ�С��ģʽ��ʽ
 �������  : INT8U * src  
             INT8U *dst   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void ConvertOAD(INT8U *src,INT8U *dst)
{
    /*ͨ���Ǵ��ģʽ��CPU��С��ģʽ*/
	*dst=*(src+1);
	*(dst+1)=*src; 
    
	*(dst+2) = *(src+2);
	*(dst+3) = *(src+3);
}

/*****************************************************************************
 �� �� ��  : EnCodeOAD
 ��������  : ����OAD�������ر����ĵ�ַ
 �������  : ��
 �������  : ��
 �� �� ֵ  : INT8U* �����ĵ�ַָ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *EnCodeOAD(ST_OAD *poad,INT8U *pdata)
{
    ConvertOAD((INT8U *)poad,pdata);
    
    return (pdata+4);
}   

/*****************************************************************************
 �� �� ��  : Judge_Time_Oi
 ��������  : �ж��Ƿ���ʱ��OI��201E,2020,2021
 �������  : INT16U OI  
 �������  : ��
 �� �� ֵ  : 0:ok, (-1):err
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S Judge_Time_Oi(INT16U OI)
{
    if((OI_EVENT_START_TIME == OI)||(OI_EVENT_END_TIME == OI)||(OI_DATA_FREEZE_TIME== OI))
    {
        return 0;
    }
    else
    {
        return (-1);
    }    
}    

/*�Ѵ����2016ת��Ϊʵ�ʵ�RTC ��16*/
INT8U TransYear(INT8U * InYear)
{
    INT16U data16;
    
    data16 = *(InYear+1);
    data16<<= 8;
    data16 += *(InYear);
    
    data16 = data16 %100;   /*2017-09-29:�����ǲ���ת��%100��Щ?*/
    
    return (INT8U)data16;
}

/*�������Զ��ᣨ�붳�ᡢ���Ӷ��ᡢ�ն��ᡢ�¶��ᡢ�궳��ȣ�ʱ�����ݼ�������붳�����ڵĵ�λһ�£����򷵻ؾܾ���д��*/
/*return:0xff,�����һ�£�0:OK*/
INT8S Check_Freeze_Interval(INT16U OI,INT8U *pTI)
{
	INT8U Unit,Unit_Oi = 0xff;

	switch(OI)
	{
		case 0x5002:
			Unit_Oi = EN_TI_UNIT_MIN;
			break;
		case 0x5003:
			Unit_Oi = EN_TI_UNIT_HOUR;
			break;
	    case 0x5004:
			Unit_Oi = EN_TI_UNIT_DAY;	
			break;
	    case 0x5006:
			Unit_Oi = EN_TI_UNIT_MONTH;	
			break;
		/*	
	    case 0x5007:
			Unit_Oi = EN_TI_UNIT_YEAR;	
			break;*/
		default: /*������ �Զ��ᣬ���ж�TI ��λ*/
			return 0;   
			break;
	}

	

	Unit = *(pTI);
	
	if(Unit != Unit_Oi)
	{
		return (-1);
	}
	else
	{
		return 0;
	}
	

}

/*****************************************************************************
 �� �� ��  : Change_time_meter_time
 ��������  : ��ͨ�ŵ�ʱ�����ݣ�ת���ɵ��ɸѡ��ʱ���ʽ�����ʽҪת��
 �������  : INT8U *pdata     
             INT8U *pdst      
             INT8U selectnum  
 �������  : ��
 �� �� ֵ  : 0:��������0:�쳣�����ʽ���ԣ��򳤶Ȳ��Ե�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S Change_time_to_meter_time(INT8U *pdata,INT8U *pdst,INT8U selectnum)
{   
    INT8U temp[10];  /*�ݴ�ʱ�䣬ʵ��ֻҪ7���ֽڣ�Ԥ�������ֽ�*/
    
    /*1.�Ƚ��һ��ʱ���ʽ������1�ͷ���2����*/
    if(0>DeCode_Data((ST_ENCODE_INFO *)TYPE_DATES,pdata,temp,0))
    {
       return (-1); 
    }    
    
    *pdst = TransYear(temp);
    LIB_MemCpy(&temp[2], &pdst[1], 5);
        
    if(EN_SELECTOR1==selectnum)
    {
        return 0;
    }  
    
    /*2.��ڶ���ʱ���ʽ*/
    if(0>DeCode_Data((ST_ENCODE_INFO *)TYPE_DATES,pdata+8,temp,0))
    {
       return (-1); 
    }    
    
    pdst[6] = TransYear(temp);
    LIB_MemCpy(&temp[2], &pdst[7], 5);

    return 0;  
    
}    


/*****************************************************************************
 �� �� ��  : DecodeRecordData
 ��������  : ������¼����������
 �������  : ST_Next_Frame_Record_Data *pFrame_Record_Data  
             INT8U *pAPDU                                   
 �������  : ��
 �� �� ֵ  : 0:�쳣������������ֹ, >0:����������
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT16U DecodeRecordData(ST_Next_Frame_Record_Data *pFrame_Record_Data, INT8U *pdata)
{
    INT16U OI,Offset = 0,len = 0;
    INT8U i,SelectNumber,num;
    INT8U result = 0;
    INT8U type_ti= 0xff;
    INT8U  First_CSD_AID = 0xff,CSD_AID; 
    INT16U OI_CSD;
    ST_OAD Tmp_oad;
    INT8U *pdataTmp;

    /*1.OAD*/
    ConvertOAD(pdata, (INT8U *)&Tmp_oad);
    OI = Tmp_oad.Oi.asInt;
    
    LIB_MemCpy((INT8U *)&Tmp_oad, (INT8U *)&pFrame_Record_Data->Oad, sizeof(ST_OAD));

    pFrame_Record_Data->Obis_event_freeze = E_OBIS_NULL;
    
    if(0 == PP_OadToObis(&Tmp_oad, &pFrame_Record_Data->Obis_event_freeze, NULL, NULL))
    {
        gDarDecode = DAR_3_NOACESS;
    }    

	pdata += 4;
	Offset += 4;

    SelectNumber = *pdata++;
    Offset++;
    pFrame_Record_Data->SelectNumber = SelectNumber;

    /*2.RSD*/
    switch(SelectNumber)
    {
        case EN_SELECTOR1:    /*ѡ�񷽷�1*/
            /*Selector1��=SEQUENCE
                {
                  ��������������  OAD��
                  ��ֵ            Data
                }*/

            ConvertOAD(pdata, (INT8U *)&pFrame_Record_Data->Oad_Selector);
            pdata  += 4;	
			Offset += 4;  
            
            /*��֧�ֵ�OAD��ҲҪ���������ݣ��Ա�Ӧ��*/
            if(0!=Judge_Time_Oi(pFrame_Record_Data->Oad_Selector.Oi.asInt))
            {
               result = 0xff;
            }  

            /*�����Ƿ���datetimes��ʽ����Ҫ�Ƚ�һ�£��Ա����len*/
            if(0<=DeCode_Data_Len(NULL,pdata,0,(INT16U*)&len))
            {
                if(0!=Change_time_to_meter_time(pdata,&pFrame_Record_Data->DdataFilter[0],1))
                {
                    result = 0xff;                    
                }                     

                pdata  += len;	
			    Offset += len; 
            }
            else
            {   
                gDarDecode = DAR_FE_ABORNMAL;
                return 0;  
            }          
            break;           
        case EN_SELECTOR2: /*ѡ�񷽷���*/
            /*
                Selector2��=SEQUENCE
                {
                  ��������������  OAD��
                  ��ʼֵ          Data��
                  ����ֵ          Data��
                  ���ݼ��        Data
                }
             */
            ConvertOAD(pdata, (INT8U*)&pFrame_Record_Data->Oad_Selector);
            pdata  += 4;	
			Offset += 4;  

            pdataTmp = pdata;
            /*2018-02-02:������������Ҫ���룬Ȼ��RCSD���أ����Ժ���������룬����...*/
            for(i=0;i<3;i++)
            {    
                DeCode_Data_Len(NULL,pdata,0,&len);
                pdata  += len;
                Offset += len;
            }   
            
            if(0==Judge_Time_Oi(pFrame_Record_Data->Oad_Selector.Oi.asInt))
            {                                        
                if(0!=Change_time_to_meter_time(pdataTmp,&pFrame_Record_Data->DdataFilter[0],2))
                {
                   result = 0xff;                    
                }      

                pdataTmp += 16;   /*(date_time_s+7)*2 */     
                
                /*TIΪ�գ���ʾȫ��*/
				if(0 ==*pdataTmp)
				{		
				    type_ti = 0;
					LIB_MemSet(0, &pFrame_Record_Data->DdataFilter[12], 3);
					//pdataTmp++;
                    
                    //Offset += 17;
				}
				else if(TI_t==*pdataTmp)
				{				
				    pdataTmp++;
				    /*���� TI ,��TI����ҲCOPY�ˣ�����һ���ֽ�TI����ҲCOPY��*/
                    LIB_MemCpy(pdataTmp, &pFrame_Record_Data->DdataFilter[12], 3);
                    //pdataTmp  += 3;
                    
                    //Offset += 20;                    
				}
                else
                {
                    /*
                     DeCode_Data_Len(NULL,pdata,0,(INT16U*)&len));
                     pdata  += len;
                     Offset += len;   */  
                   result = 0xff;   
                }    
            }
            else /*����ֵ����Ȼ�ܾ���д��ҲҪ��RCSD���أ����Ժ���������룬����...*/
            {                                   
                result = 0xff;	                
            }
            break;
        case EN_SELECTOR9:
            /*Selector9��=SEQUENCE
                {
                  �ϵ�n�μ�¼  unsigned
                }*/
            if(0 == *pdata)
            {
                result = 0xff;	         
            }   
            else
            {
                pFrame_Record_Data->DdataFilter[0] = *pdata;
                pFrame_Record_Data->DdataFilter[1] = E_SELELTOR9_COMM;     /*���︳ֵΪ0����ʾ��ͨ�ţ���0����ʾ��ʾ*/
            }   

             pdata  += 1;                    
             Offset += 1;  
             
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            return Offset;
            break;
    }
    
    /* 2017-08-02:�ж������Զ��ᵥλ*/
	if((EN_SELECTOR2 == SelectNumber)&&(0!=type_ti)&&(0!=Check_Freeze_Interval(OI,&pFrame_Record_Data->DdataFilter[12])))
	{
		result = 0xff;  
		//return Offset;			
	} 
    
	/*3.RCSD*/
	/*RCSD    2017-09-11:�쳣ʱ��Ҫ��NUMBER������    */
	num= *pdata++;   //SEQUENCE OF CSD
	Offset++;	
	pFrame_Record_Data->OadListNumber = num;
    
	if(num>MAXCSD)  /*����Ҫ���ƣ���ֹԽ��*/
	{
	    pFrame_Record_Data->OadListNumber = 0;  /*�������ͳһ���ڳ�ʼ���н���*/
		gDarDecode = DAR_3_NOACESS;//DAR_8_OVERFLOW;
		return Offset;
	}
    	
	/*2017-08-02:���Ӷ��᣺��ȡʱRCSD������Ϊ0�����򷵻ؾܾ���д��*/
	if((OI_MIN_FRZ == OI)&&(0 == num))
	{	    
		gDarDecode = DAR_3_NOACESS;
		//return Offset;
	}

    /*����CSD: [0] OAD ��[1] ROAD*/
    /*num*/
	//pdata++;
	//Offset++;	
    
	for(i = 0;i < num;i ++)
	{
		if(0 == *pdata)   //ѡ��0  OAD
		{   
			pdata++;
			ConvertOAD(pdata, (INT8U *)&pFrame_Record_Data->oadlist[i]);
			pdata  += 4;
			
			Offset += 5;
		}
		else
		{
	        pFrame_Record_Data->OadListNumber = 0;  /*����������Ƿ�֧����ROAD LISTӦ���Ȳ�֧�ְɣ�ROAD�������鷳���ڴ��*/
			gDarDecode = DAR_3_NOACESS;	
            break;
		}

        /*2017-08-17:���Ӷ���:RCSD�е�����OAD����������ȡֵ����һ�£����򷵻ؾܾ���д��DAR=3����*/
        if(OI_MIN_FRZ == OI)
        {    
            OI_CSD  = pFrame_Record_Data->oadlist[i].Oi.asInt;
            if(OI_CSD == OI_DATA_FREEZE_TIME || OI_CSD == OI_FREEZE_RECORD_NUM)
            {
                continue;
            }    

            CSD_AID = pFrame_Record_Data->oadlist[i].AId;
            
            if(0xff == First_CSD_AID)
            { 
                First_CSD_AID = CSD_AID &  MASK_AID_SELECT_B5_B7;
            }    
            else
            {
                if(First_CSD_AID != (CSD_AID &  MASK_AID_SELECT_B5_B7))
                {    
                    gDarDecode = DAR_3_NOACESS;
                    //break;
                }
            }    
        }  
        /*END 2017-08-17:���Ӷ���:RCSD�е�����OAD����������ȡֵ����һ�£����򷵻ؾܾ���д��DAR=3����*/
	}

    if(0xff == result)
    {
       gDarDecode = DAR_3_NOACESS;
	   //return Offset; 
    }   

    return Offset;
     
}    

/*****************************************************************************
 �� �� ��  : Get_Service_Decode
 ��������  : ��ȡ������������
 �������  : INT8U *pAPDU       
             _CheckOBJECT *dst  
 �������  : pDstLen:�ѱ��볤��
 �� �� ֵ  : ��������ָ���ַ
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 09 05
    ��    ��   : e
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Get_Service_Decode(INT8U *pAPDU)
{
    INT8U choice;
    INT8U ListNumber = 1;  
    INT8U i;
    INT16U Len;
    INT8U *pdata = pAPDU;

    ST_Next_Frame_Normal_Data *pFrame_Normal_Data;
    ST_Next_Frame_Record_Data *pFrame_Record_Data; 
        
    choice = *pdata++;
    
    if((0<choice)&&(EN_GET_NEXT>choice))
    {
       gServiceChoice = choice;
    }  

    pdata++;   /*iid*/    
        
    switch(choice)
    {
        case EN_GET_NORMAL:          
        case EN_GET_NORMAL_LIST:
            pFrame_Normal_Data = (ST_Next_Frame_Normal_Data *)gBak_data;
            
            if(EN_GET_NORMAL_LIST == choice)
            {    
                ListNumber = *pdata++;
            }
    
            if(ListNumber>MAXGETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }    
            
            pFrame_Normal_Data->CurrentLine = 0;
            pFrame_Normal_Data->TotalLine = ListNumber;

            for(i = 0;i < ListNumber;i++)
            {    
                ConvertOAD(pdata,(INT8U *)&pFrame_Normal_Data->oadlist[i]);
                pdata += 4; /*ָ��ָ�� ʱ���ǩ*/
            }
            
            break;
        case EN_GET_RECORD: 
        case EN_GET_RECORD_LIST:
            pFrame_Record_Data = (ST_Next_Frame_Record_Data *)gBak_data;
            
            if(EN_GET_RECORD_LIST == choice)
            {    
                ListNumber = *pdata++;
            }

            if(ListNumber>MAXGETRECORDLISTNUMBER)
            {
               gDarDecode = DAR_FE_ABORNMAL;
               return pdata;  
            }    
            
            Record_Num_List = ListNumber;
            
            for(i = 0;i < ListNumber;i++)
            {
                pFrame_Record_Data = (ST_Next_Frame_Record_Data *)gBak_data[i];

                pFrame_Record_Data ->CurrentLine = 0;
                pFrame_Record_Data ->TotalLine   = 0;
                
                Len = DecodeRecordData(pFrame_Record_Data,pdata);                 

                pdata += Len;     

                if(0!=gDarDecode)
                {
                    return pdata;  
                } 
            }               
            break;
        //case EN_GET_NEXT:  /*�����ٴ���*/            
            //break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            
            break;
    }   

    return pdata;  
    
}
/*****************************************************************************
 �� �� ��  : Set_Service_Decode
 ��������  : ����������������
 �������  : INT8U *pAPDU       
 �������  : ��
 �� �� ֵ  : ��������ָ���ַ
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 09 05
    ��    ��   : e
    �޸�����   : �����ɺ���

�������棬һ�Ѵ���������
*****************************************************************************/
INT8U *Set_Service_Decode(INT8U *pAPDU)
{
    INT8U choice;
    INT8U ListNumber = 1;  
    INT8U i;
    INT16U len   = 1;  /*�����ʼ��Ϊ1���Ա���������������,��ΪDeCode_Data_Len����󣬿���lenû�иı�*/
    INT8U *pdata = pAPDU;
    ST_SET_SERVICE *p_Set_data;
    ST_SET_THEN_GET_SERVICE *p_Set_then_data;
    
    choice = *pdata++;
    pdata++;   /*iid*/

    //if((EN_SET_NORMAL<=choice)&&(EN_SET_THEN_GET_NORMAL_LIST=>choice))
    {
       gServiceChoice = choice;
    }  
    
    switch (choice)
    {
        case EN_SET_NORMAL:            
        case EN_SET_NORMAL_LIST:
            p_Set_data = (ST_SET_SERVICE *)gBak_data;  /*��Ȼ�����к���֡��������Ӱ�������˿ڣ�����Ҫ��ComPort*/

            if(EN_SET_NORMAL_LIST == choice)
            {    
                ListNumber = *pdata++;
            }
    
            if(ListNumber>MAXSETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }  

            p_Set_data->ListNumber = ListNumber;
            
			for(i = 0;i < ListNumber;i ++)
			{
				ConvertOAD(pdata,(INT8U *)&p_Set_data->SetOad[i]);
				pdata += 4;
				p_Set_data->pointer[i] = pdata;

                if(0<=DeCode_Data_Len(NULL,pdata,0,&len))
                {
                    pdata += len;
                }  
                else /*�����쳣*/
                {    
                   gDarDecode = DAR_FE_ABORNMAL;
                   return pdata;   
                }				
			}            
            break;
        case EN_SET_THEN_GET_NORMAL_LIST:
            p_Set_then_data = (ST_SET_THEN_GET_SERVICE *)gBak_data;  /*��Ȼ�����к���֡��������Ӱ�������˿ڣ�����Ҫ��ComPort*/

            ListNumber = *pdata++;
            
            if(ListNumber>MAXSETTHENGETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }  

            p_Set_then_data->ListNumber = ListNumber;

            for(i = 0;i < ListNumber;i ++)
			{
				ConvertOAD(pdata,(INT8U *)&p_Set_then_data->SetOad[i]);
				pdata += 4;
				p_Set_then_data->pointer[i] = pdata;
				if(0<=DeCode_Data_Len(NULL,pdata,0,&len))
                {
                    pdata += len;
                }  
                else /*�����쳣*/
                {    
                   gDarDecode = DAR_FE_ABORNMAL;
                   return pdata;   
                }	
                
                ConvertOAD(pdata,(INT8U *)&p_Set_then_data->GetOad[i]);
				pdata += 4;
                
				pdata++;   /*��ʱ��ȡʱ�� unsigned*/
			}
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            break;
    }

    return pdata;
    
}    

/*****************************************************************************
 �� �� ��  : Action_Service_Decode
 ��������  : �����������������
 �������  : INT8U *pAPDU       
             _CheckOBJECT *dst  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 09 05
    ��    ��   : e
    �޸�����   : �����ɺ���
    
�������棬һ�Ѵ�������
*****************************************************************************/
INT8U *Action_Service_Decode(INT8U *pAPDU)
{
  /*action��set��һ���ġ�����*/
  return  Set_Service_Decode(pAPDU); 
}    


/*�Ƚ����ݳ��ȵĺϷ���
 ��ʱ���ǩ���ж����ݳ��ȣ��������·�㳤�Ȳ�ͬ���򷵻�ʧ�ܣ���ʾ���ݷǷ�
return:0:ok,else:error
*/
INT8S CheckApduDatalen(ST_OBJECT *pstFrm698,INT8U *pTimeTag)
{
    INT16U len;

    len = pTimeTag - pstFrm698->pdata;
    
    if(1 == *pTimeTag)
    {
        len += LEN_TIME_TAG;
    }  
    else
    {
        len += 1;
    }

    len++;  /*��һ���������͵ĳ��ȣ�pstFrm698->Len_data�����˷������ͳ���*/
    
    if(len == pstFrm698->Len_data)
    {
        return 0;
    }    
    else
    {
        return (-1);
    }    
    
} 

/*****************************************************************************
 �� �� ��  : check_time_tag
 ��������  : �ж�ʱ���ǩ�Ϸ���
 �������  : INT8U *p_time_tag
 �������  : ��
 �� �� ֵ  : p->Value ��0���Ƿ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ������ 2017 04 13
    ��    ��   : e
    �޸�����   : �����ɺ���

  ʱ���ǩ����
  ����ʱ�� 		   	date_time_s��
  ��������ʱʱ�� 	TI   ENUMERATED,��λ:��:0,��:1,ʱ:2,��:3,��:4,��,5

*****************************************************************************/
/*ʱ������ֵ����λ��*/
const INT32U  TimeUnit[] =
{
    1, 
    60, 
    60L * 60L, 
    60L * 60L * 24L, 
    60L * 60L * 24L * 31L, 
    60L * 60L * 24L * 366L
};

/*TimeTag:SEQUENCE
{
    ����ʱ��            date_time_s,       7byte
    ��������ʱʱ��    TI,                3byte
}
*/
INT8U check_time_tag(INT8U *p_time_tag_src)
{ 
    ST_6TIME_FORM  date_time_6;
    
    INT32S Time_Interval;
    
    INT8U  Time_Unit;
    INT16U Time_int;
    INT16U year;
    INT32U Time_Interval_value,TmpLong;  
    INT8U *p_time_tag;
    ST_7TIME_FORM SysTime;

    /*ƫ�ƹ�ʱ���ǩѡ����*/
    p_time_tag = p_time_tag_src + 1;

    /*ʱ���ʽת��*/
    DeCode_Base(p_time_tag,longunsigned,(INT8U *)&year);    
    year   = year % 100;  //year = year - 2000;
    date_time_6.year = (INT8U)year; 
    LIB_MemCpy(p_time_tag+2, (INT8U * )&date_time_6.month, sizeof(ST_6TIME_FORM)-1);
    if(FALSE == LIB_ChkDateTimeHEX(&date_time_6))/*���ʱ���ʽ�Ƿ���ȷ*/
    {
        return DAR_32_TIMETAGERR;
    }
    
    /*��λ*/
    Time_Unit = *(p_time_tag+LEN_DATE_TIME_S);
    
    /* ���ֵ*/
    DeCode_Base(p_time_tag+LEN_DATE_TIME_S+1,longunsigned,(INT8U *)&Time_int);
    
    //Time_int  = (INT16U)(*(p_time_tag+LEN_DATE_TIME_S+1))<<8;
    //Time_int  = Time_int + (*(p_time_tag+LEN_DATE_TIME_S+2));

    /*ʱ����Ϊ0Ŀǰ����Ϊ���ܾ���ֱ�Ӽ���0*/
    Time_Interval_value = Time_int;

    /*ʱ��ת��*/    
    if(Time_Unit < ARRAY_SIZE(TimeUnit))      //��
    {
        TmpLong = TimeUnit[Time_Unit];
    } 
    else
    {
        return DAR_32_TIMETAGERR;
    }   

    Time_Interval_value = Time_Interval_value*TmpLong;
    
    //------
    GetSingle(E_SYS_TIME, (INT8U * )&SysTime);

    Time_Interval = LIB_SToStdDate((ST_6TIME_FORM *)&SysTime) - LIB_SToStdDate(&date_time_6);
    
    Time_Interval -= Time_Interval_value;/*��ȥ���ʱ������*/
    if(Time_Interval > 0)
    {
 
        return DAR_32_TIMETAGERR;
    }
    else
    {
        return DAR_0_OK;         
    }
    
}   


/*****************************************************************************
 �� �� ��  : Check_Broad_Cast_OAD
 ��������  : �жϿ��Թ㲥������OAD:�㲥Уʱ���๦�������˲ʱ���ᴥ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 03 29
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_Broad_Cast_OAD(_ST_OMD Omd)
{
    INT16U OI; 
    INT8U  MTD;
    INT8U  Result = DAR_FF_OTHERERR;

    OI  = Omd.Oi.asInt;
    MTD = Omd.MTD;

    if(((OI_DATE_TIME == OI)||(OI_MULTI_DEVICE == OI))&&(127 == MTD))
    {
        Result = DAR_0_OK;
    } 
    else if((OI_MMT_FRZ == OI)&&(3 == MTD))
    {
        Result = DAR_0_OK;
    }    

    return Result;   
    
}    


/*****************************************************************************
 �� �� ��  : Check_Addr_Type_Right
 ��������  : ��������ַ��Ȩ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 02 01
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_Addr_Type_Right(EN_ADDRTYPE AddrType,INT8U ServiceType)
{
    ST_ACTION_SERVICE *p_Action_data;    
    INT8U  result = DAR_0_OK;

    if(EN_GETREQUEST != ServiceType)
    {
        /*ͨ���ַ����д�ͷ�����������Ϊ�˻�ȡOAD���Ա�Ӧ�𣬻����������������*/
        if(ADDR_ABBREVIATE == AddrType)
        {
           result = DAR_15_PSWERR;           
        } 
        /*�㲥��ַ����д�ͷ���������ֻ�ܹ㲥Уʱ�Ͷ๦�����*/
        else if(ADDR_BROADCAST == AddrType)
        {          
             p_Action_data = (ST_ACTION_SERVICE *)gBak_data;
             if(0 == ((EN_ACTIONREQUEST == ServiceType)&&(1 == p_Action_data->ListNumber)&&(DAR_0_OK == Check_Broad_Cast_OAD(p_Action_data->ActionOmd[0]))))
             {
                result  = DAR_15_PSWERR; 
             }   
             else
             {

             }           
        }    
    }    
    else
    {
        /*�����鲥��ַ�͹㲥��ַ������*/
        if((ADDR_BROADCAST == AddrType)||(ADDR_GROUP == AddrType))
        {
            result = DAR_FE_ABORNMAL;            
        }    
    }       

    if(DAR_0_OK != result)
    {
        gDarDecode = result;
    }    
    
    return  result;    
}    
    

/*****************************************************************************
 �� �� ��  : Check_OINeedTimeTag
 ��������  : �ж�Oi�Ƿ���Ҫʱ���ǩ����Զ�̿���OI,�����Զ�̿��ƣ���0��
 �������  : _CheckOBJECT *p
 �������  : ��
 �� �� ֵ  : _FALSE:����Ҫ�ж�ʱ���ǩ��_TRUE:��Ҫ�ж�ʱ���ǩ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ������ 2017 04 13
    ��    ��   : e
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_OI_Need_Time_Tag (ST_ACTION_SERVICE *p_Action_data)
{
    INT8U  i;
    INT16U Tmp_asInt;
    INT8U  Ret = FALSE;

    /*2017-11-18 ���ڣ�ֱ���˳�*/
    if (TRUE == ChkStatus(E_FAC))
    {
        return FALSE;
    }  

    for(i=0;i<p_Action_data->ListNumber;i++)
    {
        Tmp_asInt = p_Action_data->ActionOmd[i].Oi.asInt& 0xff00;;
        
        /*1.�����Զ�̿��� ,2.��0����*/
	    if((OI_REMOTE == Tmp_asInt)
	  	||(OI_ELECTRI_DEVICE == Tmp_asInt))
	    {	
            Ret = TRUE;
        }
        /*3.�����¼���0*/
        else if((0x3000 == Tmp_asInt)&&(1 == p_Action_data->ActionOmd[i].MTD))
	    {
            Ret = TRUE;
        }       
    }

    return Ret;
}



/*****************************************************************************
 �� �� ��  : CheckApFrame
 ��������  : ���Ľ����������OAD LIST���棬�Լ����洦��ʱ��
 �������  : ST_OBJECT *pObject  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void CheckApFrame(ST_OBJECT *pObject,INT8U *pTimeTagTmp)
{
    INT8U ServiceType;
    INT8U *pTimeTag = NULL;  /*ָ��ʱ���ǩ*/  
    INT8U *pdata = pObject->pdata;

    ServiceType = pObject->servicetype;
    //choice =pdata++;   /*ԭ������*/
    //IID    =pdata++;   /*ԭ������*/
  
    gDarDecode = 0;    
    
    switch(ServiceType)
	{
        case EN_GETREQUEST:
            pTimeTag = Get_Service_Decode(pdata);     
    	    break;
    	case EN_SETREQUEST:
            pTimeTag = Set_Service_Decode(pdata);  		    
    	    break;
    	case EN_ACTIONREQUEST: 
            pTimeTag = Action_Service_Decode(pdata); 
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            break;
    }   
    
    LIB_MemCpy(pTimeTag, pTimeTagTmp, LEN_TIME_TAG);

    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return;  
    }    
    
    /*�����������࣬�����ж����ݳ��ȵĺϷ���*/
    //if((SETREQUEST == Type_Service)||(ACTIONREQUEST == Type_Service))
    if(EN_GETREQUEST != ServiceType)
    {    
        if(0 != CheckApduDatalen(pObject,pTimeTag))
        {
            /*�ĳ����Ͳ�ƥ��ɣ��������ݳ����ǶԵģ������Ͳ��ԣ�����ʾ����VALUE9����̫��*/
            gDarDecode = DAR_5_OANOADAPT;   
            return;
        }   
    }  

    /*����ַ��Ȩ��*/
    if(DAR_0_OK!= Check_Addr_Type_Right(pObject->AddrType,ServiceType))
    {
        return;
    }

    /*ʱ���ǩ����  pdata��ƫ�Ƶ���ʱ���ǩλ��*/
    if(1 == *pTimeTag)
    {
       if(DAR_0_OK != check_time_tag(pTimeTag))
       {
          gDarDecode = DAR_32_TIMETAGERR;
       }
    }
    else
    {
        if(EN_ACTIONREQUEST == ServiceType)
        {    
            if(TRUE==Check_OI_Need_Time_Tag((ST_ACTION_SERVICE *)gBak_data))
            {
                gDarDecode = DAR_32_TIMETAGERR;  
            }
        }
    } 
    
}    


/*****************************************************************************
 �� �� ��  : check_list_choice
 ��������  : �ж��Ƿ���getLIST��actinolist��setlist,�Ա�ƫ�����ݳ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void check_list_choice(INT8U servicetype,INT8U *LenEncode)
{
    INT8U choice;

    choice = gServiceChoice;
    
    if(EN_GETREQUEST == servicetype)
    {
        if((EN_GET_NORMAL_LIST == choice)||(EN_GET_RECORD_LIST == choice))
        {
            *LenEncode += 1;
        }    
    }    
    else /*set action��list choice��һ����*/
    {
        if((EN_SET_NORMAL_LIST == choice)||(EN_SET_THEN_GET_NORMAL_LIST == choice))
        {
            *LenEncode += 1;
        }   
    }    
}

/*****************************************************************************
 �� �� ��  : Action_PP_PrgEvent
 ��������  : ������������ɾ���ȱ���¼�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 06
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void Action_PP_PrgEvent(void)
{
    //ST_OAD Oad;
    INT16U Oi;
    INT8U Oi0,Aid;
    INT8U F_action_prg = 0;  /*��Ҫ��¼����¼���ACTION*/
        
    Oi0 = CurOad.Oi.asByte[OIH];
    Oi  = CurOad.Oi.asInt;
    Aid = CurOad.AId;
    
    /*24��*/
    #if 0
    if((OI_OVER_PWR_EVENT>=Oi) ||(OI_OVER_R_DEMAND_EVENT == Oi))
    {
        if((4 == Aid)||(5 == Aid))
        {
            F_Relation_Obj = 0xff;
        }    
    }
    else 
    #endif
    
    if((OI_MEASURE_CHIP_ERR_EVENT>=Oi)&&(0x30 == Oi0)) /*7���24��*/
    {
       if((4 == Aid)||(5 == Aid))
       {
           F_action_prg = 0xff;
       }     
    }  
    else if(0x50 == Oi0)                               /*����*/
    {
        if((4 <= Aid)||(8 >= Aid))
        {
            F_action_prg = 0xff;
        }    
    }
    else if(((OI_RS485_DEVICE == Oi)&&(0x7f==Aid)) || /*�޸Ĳ�����*/
            ((OI_CARRIE_DEVICE == Oi)&&(0x80==Aid)))
    {    
        F_action_prg = 0xff;
    }
    
    if(0xff==F_action_prg)
    {
       PP_PrgEvent(CurOad);
    }       
    
}    
/*"----------------------------�����Ǳ�����غ���-------------------------------------------"*/

/*****************************************************************************
 �� �� ��  : Cal_EnCode_Data_Len
 ��������  : ����IID�������󳤶ȣ���װһ�£�����������˼��ȷЩ���ÿ���
 �������  : ST_ENCODE_INFO * pInfo  
             INT8U IID               
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 31
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT16U Cal_EnCode_Data_Len(ST_ENCODE_INFO * pInfo,INT8U IID)
{
   return EnCode_Data(pInfo,NULL,NULL,IID);
}    

/*****************************************************************************
 �� �� ��  : Get_OAD_Data_len
 ��������  : ����OAD����ȡ������ݵĳ���,ͬʱ��������Ƿ��ж���
 �������  : ST_OAD Oad  
 �������  : ��
 �� �� ֵ  : *pret ��ΪDAR_0_OK����ʾδ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT16U Get_OAD_Data_len(ST_OAD Oad,SUB_OBJECT *pobject,INT8U  *pret)
{
    //SUB_OBJECT object;
    INT16U len = 0;
    
    INT8U AId;/*"���Ա�ʶ"*/
    INT8U Class;      /*��*/

    Oad.AId = Oad.AId & MASK_AID_SELECT;   /*֮������OAD.AID�룬��Ҫ����Ϊget_oad_object��Ҫ��*/
    AId = Oad.AId;
    
    pobject->Class = 0;                    /*����Ϊ0���Ա�����ж�*/

    LIB_MemCpy((INT8U *)&Oad,(INT8U *)&pobject->Oad ,sizeof(ST_OAD));
    
    *pret = get_oad_object(pobject);   /*ȷ���Ƿ��ж���*/

    Class = pobject->Class;
    
    if(DAR_0_OK == *pret)
    {
        if(1 == AId)
        {
           len = LEN_LOGIC_NAME;
        }   
        else if(((3 == AId)&&(Class<=6))||((5 == AId)&&(1 == Class))) /*���㵥λ*/
        {
           len = LEN_SCALE_UNIT; 
        }    
        else
        {
           if(17 == Class)
           {
              len = 4;   /*17���ȹ̶�Ϊ4*/
           }
           else
           { 
              len = Cal_EnCode_Data_Len((ST_ENCODE_INFO *)pobject->Pointer,Oad.IId); 
           }
        }   
    }
    else
    {
        
    }  

    return len;
}    


/*���������ǰ��Ҫ�ж��Ƿ��Խ��*/
/*���GET DAR,�����6�ֽ� ��return:�����ĵ�ַ*/
INT8U *EncodeGetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
   
    pdst = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DAR;     /*��ʾ���ص���DAR*/
    *pdst++ =  dar_value;
    
    return pdst;
}  


/*������ý�� 5���ֽ�*/
INT8U *EncodeSetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
    
    pdst    = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  dar_value;

    if(DAR_0_OK == dar_value)
    {
       PP_PrgEvent(*pOad);
    }  

    return pdst;
}    

/*����������� 6���ֽ�*/
INT8U *EncodeAcionDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
    
    pdst = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  dar_value;
    
    *pdst++ =  0;         /*Data  OPTIONAL*/

    if(DAR_0_OK == dar_value)
    {   
        Action_PP_PrgEvent();
    }    
    
    return pdst;
}    

/*����AID����1��һ�����߼�����������OI��ʾ.������OI:OAD+OCTSTRING+2+OI*/
INT8U * EncodeAtt1LogicName(ST_OAD*pOad,INT8U *pdata)
{	
    INT8U * pdst = pdata;

    pdst = EnCodeOAD(pOad,pdst);
    
    *pdst++ = E_RESULT_CHOICE_DATA;        /*���Ϊ����*/
        
    *pdst++ = octetstring;                 /* OI����  */
    *pdst++ = 2;  

    *pdst++ = pOad->Oi.asByte[OIH];        /* OIֵ  */
	*pdst++ = pOad->Oi.asByte[OIL]; 

    return pdst;
}


/*****************************************************************************
 �� �� ��  : Encode_Scale_Unit
 ��������  : ������㵥λ
 �������  : ST_OAD*pOad    
             INT8U *pdata   
             INT8U element  
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 19
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Encode_Scale_Unit(ST_OAD*pOad,INT8U *pdata,INT8U element)
{
    INT8U * pdst = pdata;

    ConvertOAD((INT8U *)pOad,pdst);    
    pdst    += 4;

    *pdst++ = E_RESULT_CHOICE_DATA;  /*���Ϊ����*/
    
    *pdst++ = TScaler_Unit;          /* ����  */

    *pdst++ = CLS_1_6_OBJECT_ELE[element].Scaler;        
	*pdst++ = CLS_1_6_OBJECT_ELE[element].UNIT; 

    return pdst;
}    
 
/*****************************************************************************
 �� �� ��  : Get_Crypt_Type
 ��������  : ������������ת���ɰ�ȫģʽʶ��ļ��� EN_CRYPT_TYPE_SAFE_MODE
 �������  : INT8U Crypt_Type_Src  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���
    
1.����
  1)����+MAC���� ����+��֤��
  2)������������
2.����
  1)������������
  2)������������+��֤��
*****************************************************************************/
INT8U Get_Crypt_Type(INT8U Crypt_Type_Src)
{
    INT8U Crypt_Type_Tmp;    
    
    if(ENCRYPTED_WITH_SIDMAC == Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_ENCRYPT_MAC_TYPE;
    }    
    else if(ENCRYPTED <= Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_ENCRYPT_TYPE;
    }    
    else if(UN_ENCRYPTED == Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_UN_ENCRYPT_TYPE;
    }    
    else
    {
        Crypt_Type_Tmp = E_UN_ENCRYPT_MAC_TYEP;
    }    

    return Crypt_Type_Tmp;
        
}


/*���ش����� return :-1:�쳣��>=0:���������ݳ���*/
INT16S PP_process(ST_OBJECT *pObject)
{    
    INT8U LenEncode = 2;   /*2���� choice��PIID*/
    
    INT8U TimeTag[LEN_TIME_TAG];
    
    INT8U *pdata = pObject->pdata;

    class_comm_build();
    
    ComPort = NowComId;
    
    Crypt_Type   = Get_Crypt_Type(pObject->Crypt_Type);
    pEndEncodeAddr = pObject->pdata + MAX_APDU_LEN-LEN_TIME_TAG-MAX_LEN_FOLLOW_REPORT-LEN_RESERVE;

    /*��֡*/
    CheckApFrame(pObject,TimeTag);

    check_list_choice(pObject->servicetype,&LenEncode);

    pdata += LenEncode;  /*ƫ�ƹ�choice��PIID���Լ�list num ����ԭ������*/

    /*����choice���Ͳ���*/
    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return (-1);   /*���ǲ����Լ�����*/
    } 
    
    pdata = Process_698(pObject->servicetype,pdata);

    /*�쳣����LIST����Խ��,�����choice���Ͳ���*/
    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return (-1);   /*���ǲ����Լ�����*/
    }  


    /*2.���ʱ���ǩ*/
    if(1 == TimeTag[0])
    {
        LIB_MemCpy(TimeTag, pdata, LEN_TIME_TAG);
        pdata += LEN_TIME_TAG;
    }
    else
    {    
       *pdata++ = 0;
    }
    
    return (pdata - (pObject->pdata)); 
    
}

/*"*****************************************************************************"*/
/*"  Function:       PP_OadToObis"*/
/*"  Description:    ����ӳ������ָ��OAD��Ӧ��OBIS����ƫ��"*/
/*"  Calls:          "*/
/*"  Called By:      ��������"*/
/*"  Input:          pOad: Oad��ַ "*/
/*"  Output:        pObis: Obis�洢��ַ��pOffet: Obisƫ�Ƶ�ַ��pFormatId: Obisͨ�Ÿ�ʽID"*/
/*"  Return:        OAD��ӦOBIS�е�Ԫ�ظ���(��ģ�����������OBIS�ָ�)��Ϊ0ʱ��ʾ����ʧ��"*/
/*"*****************************************************************************"*/
INT8U PP_OadToObis_Info(SUB_OBJECT *pobject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info)
{
    INT8U i,class_num;
    
    class_num = pobject->Class;

    /*�ݴ��ȶ���ʼ��һ�£���ֹ�����е�û�и�ֵ*/
    for(i=0;i<MAX_OBIS_NUM_ONE_OAD;i++)
    {    
        pObis_Info[i].FormatId   = MAX_FORMAT_ID;   /*�ȳ�ʼ��Ϊ�޸�ʽ*/
        pObis_Info[i].FunctionId = FUN_NULL;      /*�ȳ�ʼ��Ϊ����*/
        pObis_Info[i].ShowinfoId = E_INFO_NULL;   /*�ȳ�ʼ��Ϊ�޸�ʽ*/
        pObis_Info[i].num        = 0;
    }

    /*������������OBISֻ����һ�����α����ſ��ܶ����������ж��������Ϊ�α����ദ����������⴦��*/
    if(7>class_num)
    {
        get_class1_6_Obis(pobject, pObis_Info);        
        
        return 1;    
    }    

    /*����������룬��ʱ��һ�������б���棬������д���Ա����*/
    switch ( class_num )
    {       
        case 7 :
            return get_class7_Obis(pobject, pObis_Info);
            break;
        case 8 :
            return get_class8_Obis(pobject, pObis_Info);
            break;
        case 9:
            return get_class9_Obis(pobject, pObis_Info);
            break;    
        case 17 : 
            return get_class17_Obis(pobject, pObis_Info);
            break;
        case 19 :
            return get_class19_Obis(pobject, pObis_Info);
            break;
        case 20 :
            return get_class20_Obis(pobject, pObis_Info);
            break;
#ifdef PM_MODLE            
        case 21 :
            return get_class21_Obis(pobject, pObis_Info);
            break;
#endif            
        case 22 :
            return get_class22_Obis(pobject, pObis_Info);
            break;
        case 24 :
            return get_class24_Obis(pobject, pObis_Info);
            break;
        default:
            break;
    }

    return 0;
}


/*****************************************************************************
 �� �� ��  : PP_GetOadToObisList
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
INT8U PP_GetOadToObisList(INT8U IID,INT8U num_obis,INT16U *pObis,ST_PARAMENT_OAD_OBIS_INFO *pObis_Info,ST_FMT_ATTR *pFormat)
{
    INT8U i,j,num_ms_obis,TransId;  /*TransId:ͨ�Ÿ�ʽID*/
    const ST_FORMAT_ID *pObisFmt;    
    INT16U temp_Obis;
    INT8U f_block_data = 0;         /*��0��������*/
    INT8U Offset_Format,Offset_Format_Tran;
    INT8U temp_iid;                 /*ʵ�����ڼ���IID=0ʱ��OBIS����*/

    *pObis = E_OBIS_NULL;   /*��Ĭ��һ����Ч��OBIS*/
   
    if(0==IID)
    {   
        num_ms_obis = pObis_Info[0].num;
        /*������*/
        if(num_ms_obis>=E_SEASON_SLOT_ID)
        {
            *pObis = pObis_Info->Obis;         /*ȡ��OBIS,��ֻҪȡ��һ������*/
            num_ms_obis = LenGet_PP_698((ENUM_DATALEN_ID)num_ms_obis);
            
            pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[0].FormatId];
            
            TransId = pObisFmt->TransId;
               
            if(NULL!=pFormat)
            {    
                LIB_MemCpy((INT8U *)&stFormat[TransId], (INT8U *)(pFormat), sizeof(ST_FMT_ATTR));
            }

            /*2018-01-19:���´���ֹԽ��*/
            if(num_ms_obis>=64)   /*��������������63�ݴ� ��ʾ�б���ܱȽϴ󣬵������ߵ����������������⴦����*/
            {
               return 0; 
            }
            
            return (num_ms_obis+MASK_BLOCK_DATA);  /*�����ݷ���Ϊ0x80+����*/
        }    
    }  

    /*-----------------------------------------------------------------*/
    
    /*2018-01-19:���´���ֹԽ��*/
    if(num_obis>MAX_OBIS_NUM_ONE_OAD)
    {
        return 0; 
    }  

    temp_iid   = 0;
    
    for(i=0;i<num_obis;i++)
    {
        temp_Obis   = pObis_Info[i].Obis;
        num_ms_obis = pObis_Info[i].num;
        
        if(num_ms_obis>=E_SEASON_SLOT_ID)     /*�����ݣ��Ա�򵽾����IID��Ӧ��IID=0�ģ��������洦������Խ��*/
        {
           temp_Obis++;
           num_ms_obis = 63; // LenGet_PP_698(num_ms_obis);  /*����������ǵ����ҵĻ����������ֵ������IID�ĺϷ��ԣ���get_oad���жϣ������ٿ����������д������Ҳ��*/
           f_block_data = 0xff;
        }        
        
        for(j=0;j<num_ms_obis;j++)        
        {                
            /*�������ݴ����һ����������������ʾ�봫����*/    
            /*ָ�����´��Ա�ֻȡOBIS����ȡ��ʽ*/    
            if(NULL!=pFormat)
            {    
                /*FormatId���´�*/ 
                /*���������Ҫ����IID=0�ĵ�OBIS���ܴ���MAX_OBIS_NUM_ONE_OAD��ʵ�ʣ���Ϊ��OBIS�ģ������и�ʽת�������Կ���ֱ�Ӹ�0��ֵ���ɣ���������ʱ������*/
                if(0==IID)  
                {
                    Offset_Format_Tran = temp_iid;
                    if(Offset_Format_Tran>=MAX_OBIS_NUM_ONE_OAD)  /*2018-02-22:�������´��*/
                    {
                        Offset_Format_Tran = 0;
                    }    
                }    
                else
                {
                    Offset_Format_Tran = 0;
                }  
                
                if(MAX_FORMAT_ID>pObis_Info[i].FormatId)
                {    
                    if(0!=f_block_data) /*�����ݣ��̶�ȡ��һ����ʽ,��Ҫ��Ϊ�˷�ֹԽ��*/
                    {
                       Offset_Format = 0;
                    }
                    else
                    {    
                       Offset_Format = i;                       
                    }
                    
                    pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[Offset_Format].FormatId];                
                                           
                    TransId = pObisFmt->TransId;                     
                    
                    LIB_MemCpy((INT8U *)&stFormat[TransId], (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR));

                }                
                else
                {    
                    LIB_MemCpy((INT8U *)&stFormat[E_FORMAT_NONE_6], (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR)); 
                    //LIB_MemSet(E_NONE_FORMAT, (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR)); 
                }   
            }   
               
            temp_iid++;
            
            if((temp_iid==IID)||(0==IID))
            {
                *pObis = temp_Obis;                
            }  
                         
            /*������IID���ڼ������Զ��ӣ���Ϊ������ֵ*/    
            temp_Obis++;
            
            if((temp_iid==IID)&&(0!=IID))
            {
                return 1;
            }    
            
             /*IID=0,�ſ����ж��������*/
            if(0 == IID)
            {
                pObis++;                
            }               
       } 
        
    }    

     return temp_iid;    
}    

/*"*****************************************************************************"*/
/*"  Function:       PP_OadToObis"*/
/*"  Description:    ����ӳ������ָ��OAD��Ӧ��OBIS����ƫ��"*/
/*"  Calls:          "*/
/*"  Called By:      ��������"*/
/*"  Input:          pOad: Oad��ַ "*/
/*"  Output:        pObis: Obis�洢��ַ��pOffet: Obisƫ�Ƶ�ַ��pFormatId: Obisͨ�Ÿ�ʽID"*/
/*"  Return:         OAD��ӦOBIS�е�Ԫ�ظ�����Ϊ0ʱ��ʾ����ʧ��,
  1.>0x80����ʾ�ǿ����ݣ�Ԫ�ظ���Ϊ����ֵ-0x80��pObis��Ϊһ����OBIS ID
  2.<=0x80,���ص��Ǿ���Ԫ�ظ�����pObis�����OBIS�б�"*/

 /*ע:pOffset��pFormat ע���Խ�磬����Ϊ��*/
/*"*****************************************************************************"*/
INT8U PP_OadToObis(ST_OAD *pOad, INT16U *pObis, INT16U *pOffset, ST_FMT_ATTR *pFormat)
{
    INT8U num_obis,num_obis_list;
    SUB_OBJECT object;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];  /*ֻ�������ݴ�*/
    ST_OAD CurOadTmp;  /*�����ݴ�*/
    
    object.Class = 0;                    /*����Ϊ0���Ա�����ж�*/

    LIB_MemCpy((INT8U*)pOad,(INT8U *)&object.Oad ,sizeof(ST_OAD));

    *pObis = E_OBIS_NULL;  /*��Ĭ��һ����Ч��OBIS*/
    
    /*ȷ���Ƿ��ж���,ͬʱ��ȡԪ��λ��*/
    if(DAR_0_OK != get_oad_object(&object))   
    {
        return 0;
    }    
    
    num_obis = PP_OadToObis_Info(&object,Obis_Info);

    /*��Ҫ�ݴ�һ�£��Ա���LenGet_PP_698()����CurOad*/
    LIB_MemCpy((INT8U*) &CurOad, (INT8U*)&CurOadTmp, sizeof(ST_OAD));

    LIB_MemCpy((INT8U*)pOad, (INT8U*)&CurOad, sizeof(ST_OAD));

    num_obis_list = PP_GetOadToObisList(pOad->IId,num_obis,pObis,Obis_Info,pFormat); 
        
    LIB_MemCpy((INT8U*) &CurOadTmp, (INT8U*)&CurOad, sizeof(ST_OAD));

    return num_obis_list;
    
    
}


/*****************************************************************************
 �� �� ��  : PP_ObisToOad
 ��������  : OBISתOAD�������ܣ�ֻ�������OAD�ģ����¼��ģ����������OAD
 �������  : ST_OAD Oad  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 29
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U PP_ObisToOad(INT16U Obis,ST_OAD *pOad)
{
    if(DAR_0_OK == PP_Obis_Oad_class7(Obis,pOad))
    {
        return DAR_0_OK;
    }
    else if(DAR_0_OK == PP_Obis_Oad_class24(Obis,pOad))
    {
        return DAR_0_OK;
    }    
    else
    {
        return DAR_FF_OTHERERR;
    }    
}    
/*****************************************************************************
 �� �� ��  : test_pp_oad_obis
 ��������  : ����OAD תOBIS����
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 12 26
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void test_pp_oad_obis(void)
{
    INT8U i;    
    ST_FMT_ATTR Format_list[MAX_OBIS_NUM_ONE_OAD];
    INT16U Obis[MAX_OBIS_NUM_ONE_OAD];
    INT8U temp_data[MAX_ONE_OAD_DATA_LEN];
    INT16S SRet;
    
    ST_OAD Oad ={0x2004,0x02,0x00};        /*OAD*/

    PP_OadToObis(&Oad,Obis,NULL,Format_list);

    SRet = GetSingle(Obis[0], temp_data);

    if(0>SRet)
    {
      for(i=0;i<20;i++)
      {
          ;
      }   
    } 
    else
    {    
        for(i=0;i<10;i++)
        {
            ;
        } 
    }
    
    
    
}    
/*"*****************************************************************************"*/



