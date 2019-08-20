/*"*****************************************************************************"*/
/*"  FileName: class20_connect.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  Ӧ���������ļ�   "*/
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
#include "App\UserInterface\ProtocolStack\PS_698\AL_698.h"
/*"*******************************************************************************"*/
const INT8U C20_Att3_FactoryVersion[]=
{	
	longunsigned,0x00,0x14,/*Э��汾��Ϣ       long-unsigned*/
	longunsigned,0x07,0xD0,/*������APDU�ߴ�   long-unsigned*/
	longunsigned,0x07,0xD0,/*�����APDU�ߴ�   long-unsigned*/
	longunsigned,0x07,0xD0,/*���ɴ���APDU�ߴ�   long-unsigned*/
	bitstring,64,0xff,0xff,0xff,0xff,0xC0,0x00,0x00,0x00,/*Э��һ���Կ�        bit-string(64)*/
	bitstring,0x81,0x80,0xff,0xFE,0xC4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*����һ���Կ�        bit-string(128)*/
};


/*Ӧ�������ֻ࣬��һ��OI=4400*/
/*20��Ӧ�����������б�*/
const ST_SPECIAL_CLASS_AID_INFO CLASS20_AID_LIST[3]=
{
    /*1,�߼���*/
    /*2.�����б�*/
    {3, READ_ONLY,  0x07,TYPE_CONNECT_INFO},   /*3.Ӧ���ﾳ��Ϣ*/        
    {4, READ_ONLY,  0x02,TYPE_U8},             /*4.��ǰ���ӵĿͻ�����ַ*/
    {5, READ_ONLY,  0xFF,TYPE_ENUM}           /*5.�����֤����*/
};

/*20��OBIS�б�*/
const ST_CLASS20_OI_AID_OBIS CLASS20_OI_AID_OBIS[1] = 
{
    {OI_CONNECT, E_LRM_JUSTMETER,E_LRM_JUSTMETER,E_LRM_JUSTMETER} /*�����豸*/  
};

/*CLASS20 ����3ÿ��IID��Ӧ��ƫ�ƺͳ���*/
const INT8U C20_ATT3_IID_OFFSET[7]={0,3,6,9,12,22};
const INT8U C20_ATT3_IID_LEN[7]   ={3,3,3,3,10,19};


/*û�ã���ȡ��OBIS���Ǽٵģ����Ƿ񷵻ظ���Ϊ0?*/
INT8U get_class20_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  seq_num,AID;
     
    
   seq_num = pObject->seq_num;
   AID     = pObject->Oad.AId;
   pObisInfo_bak->Obis = *((INT16U *)&CLASS20_OI_AID_OBIS[seq_num] + AID-2); 
   pObisInfo_bak->Offset = 0;
   pObisInfo_bak->num = 1;  //

   /*��ʾ��ʽ�����Ƿ�Ҫ���ⶨ��*/

   return 1;
      
}



/*****************************************************************************
 �� �� ��  : get_class20_connect_data
 ��������  : Ӧ�������ദ����
 �������  : ST_OAD Oad    
             INT8U *pdata  
             INT8U *pRet   
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 11
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *get_class20_connect_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet)
{
	INT8U i,j;
	INT32U time;
    INT8U  IId;
    INT8U  AID;
    INT8U *pdst=pdata;
	
    AID = Oad.AId;
    IId = Oad.IId;

    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*��ʾ���ص�������*/
    
	if(EN_CLS20_ATT_3_APP_INFO == AID)   /* Ӧ���ﾳ��Ϣ  */
	{			
        IId = Oad.IId;
        
        if(0!=IId)
        {
            i = IId;
            j = IId;                
        }
        else /*IID=0����������7��IIDҲ�����ʡ����*/
        {
            *pdst++ = structure;
            *pdst++ = 7;
            i = 1;
            j = 7;
        }                
        
        for(;i<=j;i++)
        {              
            if(i<7)
            {      
			    LIB_MemCpy((INT8U *)C20_Att3_FactoryVersion + C20_ATT3_IID_OFFSET[i-1], pdst, C20_ATT3_IID_LEN[i-1]);
			    pdst += C20_ATT3_IID_LEN[i-1];	
            }
            else //(7 ==i)  /*��̬��ʱʱ��*/
            {    
			    time=ConnectInfo[ComPort].ConnectOverTime;
                pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_U32,(INT8U*)&time,pdst,0); 
            }                 
        }            
	}
	else if(EN_CLS20_ATT_4_CLIENT_ADDR == AID)   /* ��ǰ���ӵĿͻ�����ַ  */
	{		 
	    pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_U8,(INT8U *)&ConnectInfo[ComPort].ClientAddr,pdst,0); 
	}
	else //if(5 == AID)            /* ��ǰ����״̬  */
	{		
	    pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_ENUM,(INT8U *)&ConnectInfo[ComPort].ConnectLevel,pdst,0); 
	}	

    return pdst;
}




/*"************************************************"*/
