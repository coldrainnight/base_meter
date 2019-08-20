/*"*****************************************************************************"*/
/*"  FileName: class19_device.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �豸�������ļ�   "*/
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
/*"*******************************************************************************"*/

/*19�������б�*/
const ST_SPECIAL_CLASS_AID_INFO CLASS19_AID_LIST[9]=
{
    /*,�߼���*/
    {2, READ_ONLY,    0x01,TYPE_VIS_32},         /*�豸������*/
    {3, READ_ONLY,    0x06,TYPE_VERSION_INFO},   /*�汾��Ϣ*/        
    {4, READ_ONLY,    0x02,TYPE_DATES},          /*��������*/
    {5, READ_ONLY,    0xFF,AR_OI},               /*���豸�б�*/
    {6, READ_ONLY,    0xFF,AR_VIS},              /*֧�ֹ�Լ�б�*/
    {7, READ_WRITE,   0x01,TYPE_BOOL},           /*��������ϱ�*/
    {8, READ_WRITE,   0x01,TYPE_BOOL},           /*���������ϱ�*/
    {9, NO_READ_WRITE,0x01,TYPE_BOOL},           /*��������վͨ��*/
    {10,READ_WRITE,   0x03,AR_OAD}               /*�ϱ�ͨ��*/
};

/*19��OBIS�б�*/
const ST_CLASS19_OI_AID_OBIS CLASS19_OI_AID_OBIS[] = 
{
    {0x4300, /*�����豸*/     
     E_METER_TYPE,          /*�豸������*/
     E_FAC_NUM,             /*�汾��Ϣ  ��getobisʱ,��obis num��Ϊ6*/      
     E_PRODUCT_DATE,        /*��������*/
     E_OBIS_NULL,           /*���豸�б�*/
     E_SUP_PROFILE_LIST,    /*֧�ֹ�Լ�б�*/
     E_OBIS_NULL,        /*��������ϱ�*/
     E_OBIS_NULL,          /*���������ϱ�*/
     E_OBIS_NULL,           /*��������վͨ��*/
     E_OBIS_NULL,           /*�ϱ�ͨ��*/
    } 
};

/*�汾��Ϣ��6��OBIS���޸�Ϊ�������壬��Ϊ�����ǲ�������*/ 
const INT16U OBIS_VERSION[]=
{E_FAC_NUM,E_SOFTWARE_VER,E_SOFT_VER_DATE,
 E_HARDWARE_VER,E_HARD_VER_DATE,E_MANUFACTOR_EXT_INFO}; 

INT8U get_class19_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  AID,i;
   ST_PARAMENT_OAD_OBIS_INFO *ptmpObisInfo_bak;

   AID    = pObject->Oad.AId;   
  
   pObisInfo_bak->Offset = 0;

   if(EN_CLS19_ATT_3_VERSION_INFO == AID)
   {
      ptmpObisInfo_bak = pObisInfo_bak;
      for(i=0;i<ARRAY_SIZE(OBIS_VERSION);i++)
      {
         ptmpObisInfo_bak->num  = 1;   /*�汾��Ϣ��6��������OBIS*/
         ptmpObisInfo_bak->Obis = OBIS_VERSION[i];
         ptmpObisInfo_bak++;
      }  
      
      return 6;
   }
   else
   { 
      pObisInfo_bak->Obis = *((INT16U *)&CLASS19_OI_AID_OBIS[0].OBIS_ATT2 + AID-2); 
      pObisInfo_bak->num = 1;
   }
   
   /*��ʾ��ʽ�����Ƿ�Ҫ���ⶨ��*/

   return 1;
      
}
/*"************************************************"*/
