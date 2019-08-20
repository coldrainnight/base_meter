/*"*****************************************************************************"*/
/*"  FileName: class3_phase_varible.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ����������ļ�   "*/
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
/************************************************************/
/*class3 ����2�б�*/
const ST_Class1_6_Object CLASS3OBISLIST[MAX_CLASS3_LIST_NUM] =
{
#ifdef THREE_PHASE
    {0x2002,CLASS3_MAX_IID,E_ELE_PHASE},  /*��ѹ���*/
    {0x2003,CLASS3_MAX_IID,E_ELE_PHASE},  /*��ѹ�������*/
    {0xFF75,CLASS3_MAX_IID,E_ELE_PHASE},  /*�������*/
#endif	
    /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/
    {0x2000,CLASS3_MAX_IID,E_ELE_VOLT},   /*��ѹ*/
    {0x2001,CLASS3_MAX_IID,E_ELE_CURR}    /*����*/
};
/*---------------------------------------------------------------------*/
#ifdef THREE_PHASE
#define E_INFO_V  E_INFO_A_V
#define E_INFO_I  E_INFO_A_I
#define E_INFO_$  E_INFO_A_$
#else
#define E_INFO_V  E_INFO_NULL
#define E_INFO_I  E_INFO_L
#define E_INFO_$  E_INFO_NULL
#endif
/*-----------------------------------------------------------*/
/*num ��E_PHASE_VARIBLE_ID ����CLASS3_MAX_IID������Ϊ��OBIS��*/
const ST_OAD_OBIS_INFO stCls3ObisInfo[MAX_CLASS3_LIST_NUM] =
{
#ifdef THREE_PHASE
    {E_VOLT_ANGLE_BLK, E_PHASE_VARIBLE_ID, 0, FORMAT_U2_1_UBCD2_1, FUN_NULL, E_INFO_$},  /*��ѹ���*/	
    {E_VOLT_CURR_ANGLE_BLK, E_PHASE_VARIBLE_ID, 0, FORMAT_U2_1_UBCD2_1, FUN_NULL, E_INFO_$},  /*��ѹ�������*/	
    {E_CURR_BLK, E_PHASE_VARIBLE_ID, 0, FORMAT_U2_1_UBCD2_1, FUN_NULL, E_INFO_$},  /*�������*/
#endif
    {E_VOLT_BLK, E_PHASE_VARIBLE_ID, 0, FORMAT_V, FUN_NULL, E_INFO_V},       /*��ѹ*/
    {E_CURR_BLK, E_PHASE_VARIBLE_ID, 0, FORMAT_A, FUN_NULL, E_INFO_I}        /*����*/
};

/*�������Ա�������*/
const ST_Class1_6_Object CLASS3_AID_4_OBISLIST[] =
{
    {0x2001,1,E_ELE_CURR},   /*1.���ߵ���*/
};    

const ST_OAD_OBIS_INFO stCls3_AID_4_ObisInfo[] =
{
    {E_CURR_LN, 1, 0, FORMAT_A, FUN_NULL, E_INFO_N},  /*1.���ߵ���*/
};

/*"************************************************"*/
/*��������������������OAD�಻�࣬�����Ļ����ٽ�����OAD������鴦��*/
/*ʵ��ֻ�����ߵ�������������������OI*/
INT8U get_special_aid_object(SUB_OBJECT *pObject)
{
    INT8U  AID,IID;
    INT16U OI;
    //ST_Class1_6_Object* pCLASS1_6_OBISLIST;
    INT8U  element;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*1.���ߵ���*/
    if((OI_CURRENT == OI)&&(ATT_L_CURRENT == AID))
    {
        pObject->seq_num = 0; //ARRAY_SIZE(CLASS3OBISLIST)+10;  /*���⴦�� +10���Ա�����ж�*/
        
        element = E_ELE_CURR;//CLASS3_AID_4_OBISLIST[0].element;
    }  
    #ifdef THREE_PHASE
    /*�������*/
    else if((OI_CURRENT_PHASE == OI)&&(2 == AID))   
    {
        pObject->seq_num = 2;  /*ֱ�Ӹ�ֵ*/        
        element = E_ELE_PHASE;//CLASS3OBISLIST[2].element;
    }         
    #endif
    else
    {
        return DAR_4_OANODEF;
    }   

    if(CLASS3_MAX_IID >= IID)
    {    
        pObject->Class   = 3;
        pObject->Right_RW= READ_ONLY;
        pObject->element = element;            
        pObject->Pointer = CLS_1_6_OBJECT_ELE[element].Pointer;       

        /*���ߵ�����������*/  
        if(OI_CURRENT == OI)
        {
            pObject->Pointer = pObject->Pointer +2;
        }    
        
        return DAR_0_OK;
    }
    else
    {
        return DAR_8_OVERFLOW;
    }    
    
}


