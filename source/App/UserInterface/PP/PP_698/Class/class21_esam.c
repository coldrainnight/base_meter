/*"*****************************************************************************"*/
/*"  FileName: class21_esam.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ESAM�ӿ����ļ�   "*/
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

#ifdef PM_MODLE


/*21�� ESAM�࣬Ҳֻ��һ��OI=F100*/
/*21�������б�*/
const ST_SPECIAL_CLASS_AID_INFO CLASS21_AID_LIST[8]=
{
    /*1.�߼���*/
    {2, READ_ONLY,  0x01,TYPE_OCTS_8},        /*2.ESAM���к�*/
    {3, READ_ONLY,  0x01,TYPE_OCTS_5},        /*3.ESAM�汾��*/        
    {4, READ_ONLY,  0x01,TYPE_OCTS_16},       /*4.�Գ���Կ�汾*/
    {5, READ_ONLY,  0x01,TYPE_U32},           /*5���ỰʱЧ����*/
    {6, READ_WRITE, 0x01,TYPE_U32},           /*6���ỰʱЧʣ��ʱ��*/
    {7, READ_WRITE, 0x03,ST_3_U32},           /*7����ǰ������*/
    {14,READ_WRITE, 0x01,TYPE_U32},           /*14. ������֤ʱЧ����*/
    {15,READ_WRITE, 0x01,TYPE_U32}            /*15. ������֤ʣ��ʱ��*/
};

/*21��OBIS�б�*/
const ST_CLASS21_OI_AID_OBIS CLASS21_OI_AID_OBIS[1] = 
{
    {0xF100,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_PASS_STATUS,E_IRINEDT_TIME ,E_IRAUTH_RMN_TIME} /*ESAM��*/  
};

/*�����⴦���ˣ�û������*/
INT8U get_class21_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  seq_num;
    
   seq_num = pObject->seq_num;
   
   pObisInfo_bak->Obis = *((INT16U *)&CLASS21_OI_AID_OBIS[0] + seq_num+1); 
   pObisInfo_bak->Offset = 0;
   pObisInfo_bak->num  = 1;
   pObisInfo_bak->FormatId = MAX_FORMAT_ID;

   /*��ʾ��ʽ�����Ƿ�Ҫ���ⶨ��*/

   return 1;
      
}
#endif

/*"************************************************"*/


