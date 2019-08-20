/*"*****************************************************************************"*/
/*"  FileName: class17_display.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ��ʾ�ӿ����ļ�   "*/
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

/*INT8U iid_set_disp_list_value = 0;*/  /*0:�����Ǳ���¼�
                                          1:��ʾiid=0,Ҫ�Ǳ���¼�����OAD��IID��Ϊ0
                                          2:��ʾiid=0,�����÷ֱ����ã����üǱ���¼�*/                                          
/*"*******************************************************************************"*/
/*17�������б�*/
const ST_SPECIAL_CLASS_AID_INFO CLASS17_AID_LIST[]=
{
    /*,�߼���*/
    {2, READ_WRITE,  0xff,AR_ST_2_CSD_U8},       /*��ʾ�����б�*/
    {3, READ_WRITE,  0x01,TYPE_U16},     /*ÿ��������ʾʱ��*/        
    {4, READ_WRITE,  0x02,ST_2_U8}       /*��ʾ����: ��ǰ�ܶ�������������������*/
};

/*17��OBIS�б� 0xf300,0xf301*/
const ST_CLASS17_OI_AID_OBIS CLASS17_OI_AID_OBIS[] = 
{
    {OI_CRC_DISP, E_CYCLE_DIS_OBJ_LIST,E_CYCLE_SHOW_TIME,E_CYCLE_SHOW_NUM}, /*�Զ�����*/  
    {OI_KEY_DISP, E_KEY_DIS_OBJ_LIST,E_NOKEY_RETURN_TIME,E_KEY_SHOW_NUM} /*��������*/ 
};

/*"************************************************"*/
const ST_SPECIAL_CLASS_AID_INFO* const SPECIAL_CLASS_AID_INFO[]=
{
    CLASS17_AID_LIST,
    NULL,
    CLASS19_AID_LIST,
    CLASS20_AID_LIST,
#ifdef PM_MODLE    
    CLASS21_AID_LIST
#endif    
};

const INT8U SPECIAL_CLASS_AID_NUM[]=
{
    ARRAY_SIZE(CLASS17_AID_LIST),
    0,
    ARRAY_SIZE(CLASS19_AID_LIST),
    ARRAY_SIZE(CLASS20_AID_LIST),
#ifdef PM_MODLE    
    ARRAY_SIZE(CLASS21_AID_LIST)    
#endif
};    


/*�������Ӧ��û���õ�*/
INT8U get_class17_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  seq_num,AID;
    
    
   seq_num = pObject->seq_num;
   AID     = pObject->Oad.AId;
   pObisInfo_bak->Obis = *((INT16U *)&CLASS17_OI_AID_OBIS[seq_num].OBIS_ATT2+ AID-2); 
   pObisInfo_bak->Offset = 0;
   pObisInfo_bak->num    = 1;
   /*��ʾ��ʽ�����Ƿ�Ҫ���ⶨ��*/

   return 1;
      
}

/*"************************************************"*/
/*������Ķ����ȡ����class17,class18,class19,class20,class21*/
INT8U get_special_class_object(SUB_OBJECT *pObject)
{
    INT8U  i,AID,IID,class_num,temp;
    INT16U OI;
    const ST_SPECIAL_CLASS_AID_INFO * pSPECIAL_CLASS_AID_INFO;
        
    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    switch ( OI )
    {
        case OI_CRC_DISP:
        case OI_KEY_DISP:    
            class_num = 17;
            break;
        case OI_FILE :
            class_num = 18;
            break;
        case OI_ELECTRI_DEVICE :
            class_num = 19;
            break;
        case OI_CONNECT :
            class_num = 20;
            break;
        case OI_ESAM:
            class_num = 21;
            break;
        case OI_METER_RST_EVENT:
        case OI_OSCILLATOR_ERR_EVENT:
        case OI_PD_PON_EVENT:   
        case OI_PARA_ERR_EVENT:   
        case OI_ENG_ERR_EVENT:      
            //class_num = 7;
            return get_class7_object(pObject);  
            break;
        default:
            return DAR_4_OANODEF;
            break;
    }
    
    pObject->Class = class_num;
    temp  = class_num-17;
    pSPECIAL_CLASS_AID_INFO = (const ST_SPECIAL_CLASS_AID_INFO *)SPECIAL_CLASS_AID_INFO[temp];

    for(i=0;i<SPECIAL_CLASS_AID_NUM[temp];i++)
    {
        /*�߼���*/
        if(1 == AID)
        {    
           return DAR_0_OK;
        }   
        
        if(AID  == pSPECIAL_CLASS_AID_INFO[i].AID)
        {   
            if(IID>pSPECIAL_CLASS_AID_INFO[i].Max_IID)
            {
                return DAR_8_OVERFLOW;
            }   
            else
            {    
                pObject->Right_RW = pSPECIAL_CLASS_AID_INFO[i].Right_RW;
                pObject->seq_num  = i;
                pObject->element  = i;  /*��ʱ����һ����*/
                pObject->Pointer  = pSPECIAL_CLASS_AID_INFO[i].Pointer;
                
                return DAR_0_OK;
            }
        }           
    }
    

    return DAR_4_OANODEF;
    
}


/*****************************************************************************
 �� �� ��  : CalClass17Att2length
 ��������  : ��������2�ĳ���
 �������  : INT8U *pbuf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 24
    ��    ��   : xyt
    �޸�����   : �����ɺ���

��ʾ������������=structure
��
   ��ʾ����   CSD��
   �����     unsigned
��

CSD��=CHOICE
{
    ��������������       [0] OAD��
    ��¼�Ͷ������������� [1] ROAD
}

*****************************************************************************/
INT8U CalClass17Att2length(INT8U *pbuf)
{
	INT8U rtn ;
	INT8U Selecter,number= 0;

    //rtn = 2; //struct num
	//rtn ++; /* CSD ���� */
	//rtn ++; /*   CSDѡ���    */
	rtn = 4;
    
	Selecter = *pbuf;
	rtn += 4;   //OAD
	if(1 == Selecter)  /*ROAD*/
	{
		rtn ++;              /*sequence num*/
		number = *(pbuf+5);  /*5��choice+OAD,��sequence num*/
	}
		
	rtn = rtn +4*number;

    rtn += 2;    /*�����*/
    
	return rtn;

}

/*
    ��ʾ�����б��=array ��ʾ����������
    ��ʾ������������=structure
    {
       ��ʾ����   CSD��
       �����     unsigned
    ��

    CSD��=CHOICE
    {
    ��������������       [0] OAD��
    ��¼�Ͷ������������� [1] ROAD
    }

    ROAD��=SEQUENCE
    {
    ��������������      OAD��
    ������������������  SEQUENCE OF OAD
    }

*/
INT8U *ENCODECLASS17ATT2(INT8U *pdata, INT8U *pbuf)
{
	INT8U i,val,num;
    INT8U *pdst = pdata;
    //INT16U len;
    
    *pdst++= structure;
    *pdst++= 2;
    *pdst++= CSD;
    
	val = *pbuf++;
    *pdst++= val;                /*  CSDѡ���  OAD OR ROAD  */
    
    /*���OAD������OAD��ROAD����Ҫ���һ��OAD�������ȴ��һ��*/
    pdst = EnCodeOAD((ST_OAD *)pbuf,pdst);
    pbuf +=4;

	if(1 == val)  //ROAD
	{
		num = *pbuf++;
        
        /*SEQUENCE NUM*/
        *pdst++= num; 
        
		for(i = 0;i < num;i ++)
		{
            pdst = EnCodeOAD((ST_OAD *)pbuf,pdst);
            pbuf +=4;
		}
	}

    /*�����*/
    pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_U8,pbuf,pdst,0); 
    
    return pdst;

}


const INT16U disp_num_obis[]={E_CYCLE_SHOW_NUM,E_KEY_SHOW_NUM};
const INT16U disp_time_obis[]={E_CYCLE_SHOW_TIME,E_NOKEY_RETURN_TIME}; 
const INT8U  disp_max_num[]={CYC_DISP_MAX,KEY_DISP_MAX};
const INT16U disp_obj_obis[]={E_CYCLE_DIS_OBJ_LIST,E_KEY_DIS_OBJ_LIST};
const INT8U TYPE_CSD[]={CSD,5}; 

const INT8U ST_2_CSD_U8[]={structure,2,CSD,0,unsigned_t,1}; 
/*****************************************************************************
 �� �� ��  : get_class17_disp_data
 ��������  : 17����ʾ���ݣ����⴦��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 27
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *get_class17_disp_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet)
{
	INT8U  i,j;
	INT16U len = 0;
    INT8U  CurrentNumber;   /*��ǰ�ܶ�����  */
	INT8U  AId,IID;
    INT8U  temp_data[MAX_ONE_OAD_DATA_LEN/2];
    INT8U  *pdst=pdata;    
    INT8U  OI1;
    const INT8U *pointer;
    
    AId = Oad.AId;
    IID = Oad.IId;
    OI1 = Oad.Oi.asByte[OIL];
    
    /*2017-01-24:�̶�����ǰ��8�ֽڣ���ֹ�ײ����ݳ��Ȳ���ʱ�����ݴ���*/
    LIB_MemSet(0, temp_data, 8);

    /*��ȡ��ʾ������*/
    //obis_temp = disp_num_obis[OI1];
    GetSingle(disp_num_obis[OI1],&CurrentNumber);
    
    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/

    /*2.��ʾ�����б�  */
    if(EN_CLS17_ATT_2_DISP_LIST == AId)       
	{	
		if(0 == IID)      /* ȫ���� */
		{
			i = 1;	   
            j = CurrentNumber;
            *pdst++ =  array;     
            *pdst++ =  CurrentNumber;    
		}
		else              /*ĳһ��  */
		{
			i = IID;
            j = i;
		}		
        
		for(;i <= j; i++)
		{				 
            GetSingle(disp_obj_obis[OI1]+ i,temp_data);
			len = CalClass17Att2length(temp_data);;
			if((pdst+len)>pEndEncodeAddr)
			{
			    /*���濴�Ƿ�Ҫ�ú���֡*/
				*pRet = DAR_3_NOACESS;
                
				return pdata;
			}
            
			pdst = ENCODECLASS17ATT2(pdst,temp_data);             
		}

        return pdst;
	
	}
    else if(EN_CLS17_ATT_3_DISP_TIME == AId)  /*ÿ��������ʾʱ��*/
    {
         GetSingle(disp_time_obis[OI1],temp_data);
         pointer = TYPE_U16;
    }       
    else //if(4 == AId)
    {
         /*
          ��ʾ������=structure
          {
            ��ǰ�ܶ�����    unsigned��
            ������������  unsigned
          }
          */
         GetSingle(disp_num_obis[OI1],temp_data);
         
         if(2==IID)
         {
            temp_data[0]=disp_max_num[OI1];
         }   
         else
         {
            temp_data[1]=disp_max_num[OI1];
         }    
         
         pointer = ST_2_U8;         

    }  
    
    pdst += EnCode_Data((ST_ENCODE_INFO *)pointer,temp_data,pdst,IID); 
    
    return pdst;

		
}  

/*****************************************************************************
 �� �� ��  : set_class17_disp_data
 ��������  : 17��������������
 �������  : ST_OAD Oad    
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 27
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U set_class17_disp_data(ST_OAD Oad,INT8U *srcpdata)
{        
    INT8U i,u;
	INT8U len_8;
    //INT8U CurrentNumber;   /*��ǰ�ܶ�����  */
	INT8U AId,IID;
    INT8U temp_data[MAX_ONE_OAD_DATA_LEN/2];
    //INT8U *pdst=pdata;
    INT16U obis_temp,len_16;
    INT8U OI1;
    INT8U *pdata = srcpdata;
    INT8S SRet = 0;
    
    //const INT8U *pointer;
    
    AId = Oad.AId;
    IID = Oad.IId;
    OI1 = Oad.Oi.asByte[OIL];
    
	if(EN_CLS17_ATT_2_DISP_LIST == AId)
	{		    
		
        i = IID;
        u = IID;
		if(0 == IID)/*  �����ʾ����      */
		{
		    
		    /*iid_set_disp_list_value = 1;*/
            pdata++;
			u = *pdata++;  /*����*/ /*2Ϊ:array+num*/  

            if(0<=DeCode_Data_Len(NULL,pdata,0,&len_16))
            {
                SRet = DeCode_Data((ST_ENCODE_INFO *)ST_2_CSD_U8,pdata,temp_data,0);                    
                len_8 = SRet;
            }
            else
            {
                SRet = -1;
            }             

            if(0>SRet)
            {
                return DAR_5_OANOADAPT;
            }               
            
            pdata +=len_16;
            
            /*��飬�����0*/
            obis_temp = disp_obj_obis[OI1];
            
            SetSingle(obis_temp,temp_data,len_8);

            i = 2;
		}
        
		for(;i <= u;i ++)
		{
		    /*
            ��ʾ������������=structure
            ��
               ��ʾ����   CSD��
               �����     unsigned
            ��
            */
            if(0<=DeCode_Data_Len(NULL,pdata,0,&len_16))
            {
                SRet = DeCode_Data((ST_ENCODE_INFO *)ST_2_CSD_U8,pdata,temp_data,0);                    
                len_8 = SRet;
            }
            else
            {
                SRet = -1;
            }             

            if(0>SRet)
            {
                return DAR_5_OANOADAPT;
            }   
                        
            pdata +=len_16;
            /* �����     unsigned*/
			//temp_data[len] = *pdata++;
            //len++;
            
            obis_temp = disp_obj_obis[OI1]+ i;   
            
            if(DAR_0_OK !=SetSingle(obis_temp,temp_data,len_8))
            {
                return DAR_FF_OTHERERR;    /*��ֹ���������ʾ��*/
            }    
            
            /*iid_set_disp_list_value = 2;*/
            
		}
        
        /*iid_set_disp_list_value = 0;*/
        
        return DAR_0_OK;
	}
	else if(EN_CLS17_ATT_3_DISP_TIME == AId)  /*ÿ��������ʾʱ��*/
    {
       len_8 = DeCode_Data((ST_ENCODE_INFO *)TYPE_U16,pdata,temp_data,0);
       obis_temp = disp_time_obis[OI1];
       
       return SetSingle(obis_temp,temp_data,len_8); 
    }       
    else //if(4 == AId)
    {
         /*
          ��ʾ������=structure
          {
            ��ǰ�ܶ�����    unsigned��
            ������������  unsigned
          }
          */
         if(1 !=IID)    /*2018-01-24 ������Ҫ��Ҫ�ſ�?*/
         {
            return DAR_3_NOACESS;
         }   
         
         DeCode_Data((ST_ENCODE_INFO *)ST_2_U8,pdata,temp_data,1);
         
         len_8 = 1;
         obis_temp = disp_num_obis[OI1];
         
         return SetSingle(obis_temp,temp_data,len_8); 
    }       

    //return DAR_3_NOACESS;
}    
    

/*"************************************************"*/
