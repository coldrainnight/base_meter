/*"*****************************************************************************"*/
/*"  FileName: class1_enery.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �������ļ�   "*/
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

/*����б���Ҫ��Ҫд���������ˣ������*/
/*class1 ����2�б�*/
const ST_Class1_6_Object CLASS1OBISLIST[] =
{   /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/
    {0x0000,MAXTRIFF_IID,E_ELE_A_ENGY_COM},  /*����й�����*/
    {0x0010,MAXTRIFF_IID,E_ELE_A_ENGY},      /*�����й�����*/
    {0x0020,MAXTRIFF_IID,E_ELE_A_ENGY},      /*�����й�����*/
    
 #ifdef THREE_PHASE
    {0x0011,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*A�������й�����*/
    {0x0012,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*B�������й�����*/
    {0x0013,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*C�������й�����*/
    
    {0x0021,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*A�෴���й�����*/
    {0x0022,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*B�෴���й�����*/
    {0x0023,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY},      /*C�෴���й�����*/
    
    {0x0030,MAXTRIFF_IID,E_ELE_R_ENGY_COM},            /*����޹�1����*/
    {0x0031,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*A������޹�1����*/
    {0x0032,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*B������޹�1����*/
    {0x0033,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*C������޹�1����*/
    
    {0x0040,MAXTRIFF_IID,E_ELE_R_ENGY_COM},            /*����޹�2����*/
    {0x0041,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*A������޹�2����*/
    {0x0042,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*B������޹�2����*/
    {0x0043,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM},  /*C������޹�2����*/
    
    {0x0050,MAXTRIFF_IID,E_ELE_R_ENGY},                /*��һ�����޹�����*/
    {0x0051,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*A���һ�����޹�����*/
    {0x0052,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*B���һ�����޹�����*/
    {0x0053,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*C���һ�����޹�����*/
    
    {0x0060,MAXTRIFF_IID,E_ELE_R_ENGY},                 /*�ڶ������޹�����*/
    {0x0061,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*A��ڶ������޹�����*/
    {0x0062,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*B��ڶ������޹�����*/
    {0x0063,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*C��ڶ������޹�����*/

    {0x0070,MAXTRIFF_IID,E_ELE_R_ENGY},                /*���������޹�����*/
    {0x0071,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*A����������޹�����*/
    {0x0072,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*B����������޹�����*/
    {0x0073,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*C����������޹�����*/

    {0x0080,MAXTRIFF_IID,E_ELE_R_ENGY},                /*���������޹�����*/
    {0x0081,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*A����������޹�����*/
    {0x0082,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*B����������޹�����*/
    {0x0083,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY},      /*C����������޹�����*/ 
    
 #endif
};


/*-----------------------------------------------------------*/
const ST_OAD_OBIS_INFO stCls1ObisInfo[] =
{
    {E_A_ENGY_COM_BLK, E_TARIFF_ID, 0, FORMAT_COMBENGY, FUN_NULL, E_INFO_NOW_TRIFF_A_ENGY},  /*����й�����*/
    {E_PA_ENGY_BLK, E_TARIFF_ID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*�����й�����*/
    {E_NA_ENGY_BLK, E_TARIFF_ID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*�����й�����*/
    
 #ifdef THREE_PHASE
    {E_PA_ENGY_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*A�������й�����*/
    {E_PA_ENGY_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*B�������й�����*/
    {E_PA_ENGY_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*C�������й�����*/
    
    {E_NA_ENGY_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*A�෴���й�����*/
    {E_NA_ENGY_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*B�෴���й�����*/
    {E_NA_ENGY_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*C�෴���й�����*/
    
    {E_R_ENGY_COM1_BLK, E_TARIFF_ID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},         /*����޹�1����*/
    {E_R_ENGY_COM1_LA, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*A������޹�1����*/
    {E_R_ENGY_COM1_LB, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*B������޹�1����*/
    {E_R_ENGY_COM1_LC, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*C������޹�1����*/
    
    {E_R_ENGY_COM2_BLK, E_TARIFF_ID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},         /*����޹�2����*/
    {E_R_ENGY_COM2_LA, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*A������޹�2����*/
    {E_R_ENGY_COM2_LB, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*B������޹�2����*/
    {E_R_ENGY_COM2_LC, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*C������޹�2����*/
    
    {E_R_ENGY_I_BLK, E_TARIFF_ID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},          /*��һ�����޹�����*/
    {E_R_ENGY_I_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*A���һ�����޹�����*/
    {E_R_ENGY_I_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*B���һ�����޹�����*/
    {E_R_ENGY_I_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*C���һ�����޹�����*/
    
    {E_R_ENGY_II_BLK, E_TARIFF_ID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},          /*�ڶ������޹�����*/
    {E_R_ENGY_II_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*A��ڶ������޹�����*/
    {E_R_ENGY_II_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*B��ڶ������޹�����*/
    {E_R_ENGY_II_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*C��ڶ������޹�����*/

    {E_R_ENGY_III_BLK, E_TARIFF_ID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},          /*���������޹�����*/
    {E_R_ENGY_III_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*A����������޹�����*/
    {E_R_ENGY_III_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*B����������޹�����*/
    {E_R_ENGY_III_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*C����������޹�����*/

    {E_R_ENGY_IV_BLK, E_TARIFF_ID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},          /*���������޹�����*/
    {E_R_ENGY_IV_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*A����������޹�����*/
    {E_R_ENGY_IV_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*B����������޹�����*/
    {E_R_ENGY_IV_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*C����������޹�����*/

#endif
    
};


/*class1 ����4�б� �߾��ȵ���*/

const ST_Class1_6_Object CLASS1_AID_4_OBISLIST[] =
{   /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/
    {0x0000,MAXTRIFF_IID,E_ELE_A_ENGY_COM_H},  /*����й�����*/
    {0x0010,MAXTRIFF_IID,E_ELE_A_ENGY_H},      /*�����й�����*/
    {0x0020,MAXTRIFF_IID,E_ELE_A_ENGY_H},      /*�����й�����*/
    
    #ifdef THREE_PHASE
    {0x0030,MAXTRIFF_IID,E_ELE_R_ENGY_COM_H},  /*����޹�1����*/
    {0x0040,MAXTRIFF_IID,E_ELE_R_ENGY_COM_H},  /*����޹�2����*/
    {0x0050,MAXTRIFF_IID,E_ELE_R_ENGY_H},      /*��һ�����޹�����*/
    {0x0060,MAXTRIFF_IID,E_ELE_R_ENGY_H},      /*�ڶ������޹�����*/
    {0x0070,MAXTRIFF_IID,E_ELE_R_ENGY_H},      /*���������޹�����*/
    {0x0080,MAXTRIFF_IID,E_ELE_R_ENGY_H},      /*���������޹�����*/

    {0x0011,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*A�������й�����*/
    {0x0012,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*B�������й�����*/
    {0x0013,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*C�������й�����*/
    
    {0x0021,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*A�෴���й�����*/
    {0x0022,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*B�෴���й�����*/
    {0x0023,MAXTRIFF_ABC_IID,E_ELE_PHASE_A_ENGY_H},      /*C�෴���й�����*/
    
    {0x0031,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*A������޹�1����*/
    {0x0032,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*B������޹�1����*/
    {0x0033,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*C������޹�1����*/
    
    {0x0041,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*A������޹�2����*/
    {0x0042,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*B������޹�2����*/
    {0x0043,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_COM_H},  /*C������޹�2����*/
    
    {0x0051,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*A���һ�����޹�����*/
    {0x0052,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*B���һ�����޹�����*/
    {0x0053,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*C���һ�����޹�����*/
    
    {0x0061,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*A��ڶ������޹�����*/
    {0x0062,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*B��ڶ������޹�����*/
    {0x0063,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*C��ڶ������޹�����*/

    {0x0071,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*A����������޹�����*/
    {0x0072,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*B����������޹�����*/
    {0x0073,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*C����������޹�����*/

    {0x0081,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*A����������޹�����*/
    {0x0082,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*B����������޹�����*/
    {0x0083,MAXTRIFF_ABC_IID,E_ELE_PHASE_R_ENGY_H},      /*C����������޹�����*/ 
    
    #endif
};

const ST_OAD_OBIS_INFO stCls1_AID_4_ObisInfo[] =
{
    {E_A_ENGY_COM_BLK, E_TARIFF_ID, 0, FORMAT_COMBENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_A_ENGY},  /*����й�����*/
    {E_PA_ENGY_BLK,    E_TARIFF_ID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*�����й�����*/
    {E_NA_ENGY_BLK,    E_TARIFF_ID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*�����й�����*/

    #ifdef THREE_PHASE
    {E_R_ENGY_COM1_BLK,E_TARIFF_ID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*����޹�1����*/
    {E_R_ENGY_COM2_BLK,E_TARIFF_ID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*����޹�2����*/
    {E_R_ENGY_I_BLK,   E_TARIFF_ID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},     /*��һ�����޹�����*/
    {E_R_ENGY_II_BLK,  E_TARIFF_ID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},     /*�ڶ������޹�����*/
    {E_R_ENGY_III_BLK, E_TARIFF_ID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},     /*���������޹�����*/
    {E_R_ENGY_IV_BLK,  E_TARIFF_ID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},     /*���������޹�����*/

    {E_PA_ENGY_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*A�������й�����*/
    {E_PA_ENGY_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*B�������й�����*/
    {E_PA_ENGY_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_PA_ENGY},     /*C�������й�����*/
    
    {E_NA_ENGY_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*A�෴���й�����*/
    {E_NA_ENGY_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*B�෴���й�����*/
    {E_NA_ENGY_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_H, FUN_NULL, E_INFO_NOW_TRIFF_NA_ENGY},     /*C�෴���й�����*/
    
    {E_R_ENGY_COM1_LA, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*A������޹�1����*/
    {E_R_ENGY_COM1_LB, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*B������޹�1����*/
    {E_R_ENGY_COM1_LC, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_1_ENGY},     /*C������޹�1����*/
    
    {E_R_ENGY_COM2_LA, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*A������޹�2����*/
    {E_R_ENGY_COM2_LB, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*B������޹�2����*/
    {E_R_ENGY_COM2_LC, MAXTRIFF_ABC_IID, 0, FORMAT_COMBENGY_R_H, FUN_NULL, E_INFO_NOW_COMM_TRIFF_R_2_ENGY},     /*C������޹�2����*/
    
    {E_R_ENGY_I_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*A���һ�����޹�����*/
    {E_R_ENGY_I_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*B���һ�����޹�����*/
    {E_R_ENGY_I_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_I_ENGY},      /*C���һ�����޹�����*/
    
    {E_R_ENGY_II_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*A��ڶ������޹�����*/
    {E_R_ENGY_II_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*B��ڶ������޹�����*/
    {E_R_ENGY_II_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_II_ENGY},      /*C��ڶ������޹�����*/

    {E_R_ENGY_III_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*A����������޹�����*/
    {E_R_ENGY_III_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*B����������޹�����*/
    {E_R_ENGY_III_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_III_ENGY},      /*C����������޹�����*/

    {E_R_ENGY_IV_LA, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*A����������޹�����*/
    {E_R_ENGY_IV_LB, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*B����������޹�����*/
    {E_R_ENGY_IV_LC, MAXTRIFF_ABC_IID, 0, FORMAT_ENGY_R_H, FUN_NULL, E_INFO_NOW_TRIFF_R_IV_ENGY},      /*C����������޹�����*/
    #endif
};


/*"************************************************"*/
const ST_Class1_6_Object* const pCLASSOBISLIST[]=
{
    CLASS1OBISLIST,
     #ifdef THREE_PHASE
    CLASS2OBISLIST,
    #else
    NULL,    
    #endif    
    CLASS3OBISLIST,
    CLASS4OBISLIST,
    NULL,
    CLASS6OBISLIST,
};

const INT8U CLASS1_6_OI_NUM[]=
{
    ARRAY_SIZE(CLASS1OBISLIST),
        
     #ifdef THREE_PHASE
    ARRAY_SIZE(CLASS2OBISLIST),
    #else
    0,
    #endif    
    
    ARRAY_SIZE(CLASS3OBISLIST),
    ARRAY_SIZE(CLASS4OBISLIST),
    0,
    ARRAY_SIZE(CLASS6OBISLIST),
};

/*һ��Ԫ�ص�OBIS*/
const ST_OAD_OBIS_INFO* const pONEOBJECTLIST[] =
{
    stCls1ObisInfo,
    #ifdef THREE_PHASE
    stCls2ObisInfo,
    #else
    NULL,    
    #endif    
    stCls3ObisInfo,
    stCls4ObisInfo,
    NULL,
    stCls6ObisInfo,
};

/*һ��Ԫ�ص�OBIS ����*/
const INT8U pONEOBJECTLISTNUM[] =
{
    ARRAY_SIZE(stCls1ObisInfo),
    #ifdef THREE_PHASE
    ARRAY_SIZE(stCls2ObisInfo),
    #else
    NULL,    
    #endif    
    ARRAY_SIZE(stCls3ObisInfo),
    ARRAY_SIZE(stCls4ObisInfo),
    NULL,
    ARRAY_SIZE(stCls6ObisInfo),
};
  

/*1-6���¼�Ԫ�ز���*/
INT8U get_class1_6_object(SUB_OBJECT *pObject,INT8U class_num)
{
    INT8U  i,AID,IID,OI_NUM,MAX_IID;
    INT16U OI;
    ST_Class1_6_Object* pCLASS1_6_OBISLIST;
    INT8U  element;   

    /*1.�Ȳ��Ҳ��������AID*/
    if(DAR_0_OK == get_special_aid_object(pObject))
    {
        return DAR_0_OK;
    }    
    
    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*2.����ͨ��OAD*/
    pCLASS1_6_OBISLIST = (ST_Class1_6_Object*)pCLASSOBISLIST[class_num-1];

    if(NULL == pCLASS1_6_OBISLIST)
    {
        return DAR_4_OANODEF;
    }    

    OI_NUM = CLASS1_6_OI_NUM[class_num-1];

    /*��ȷ����*/
    if((1 == class_num)&&((EN_CLS1_ATT_4_HIGH_DATA == AID)||(EN_CLS1_ATT_5_HIGH_SCALER_UNIT == AID)))
    {
       pCLASS1_6_OBISLIST = (ST_Class1_6_Object*)CLASS1_AID_4_OBISLIST;
       
       OI_NUM = ARRAY_SIZE(CLASS1_AID_4_OBISLIST);
    }    
    else     
    {    
        if(EN_CLS1_ATT_3_SCALER_UNIT<AID)   
        {
            return DAR_4_OANODEF;
        }  
    }

    /*1).����OI*/
    for(i=0;i<OI_NUM;i++)
    {
        if(OI == pCLASS1_6_OBISLIST[i].OI)
        {
           break; 
        }    
    }    
    
    if(i>=OI_NUM)
    {
        return DAR_4_OANODEF;
    }   

    pObject->Class   = class_num;  /*�ҵ�OI������ʾ�ҵ���*/
    
    /*2).��IID*/
    /*����Ҫ���ƻ�DAR_8�������ָ�һ��*/
    MAX_IID = pCLASS1_6_OBISLIST[i].MAX_IID;
    if((E_PHASE_VARIBLE_ID == MAX_IID)||(E_PHASE_VARIBLE_SUM_ID == MAX_IID))
    {
       MAX_IID = LenGet_PP_698((ENUM_DATALEN_ID)MAX_IID);
    }    
    
    if(IID > MAX_IID)
    {
       return DAR_8_OVERFLOW;
    }   

    //pObject->Class   = class_num;
    pObject->Right_RW= READ_ONLY;
    pObject->seq_num = i;
    
    element = pCLASS1_6_OBISLIST[i].element;
    pObject->element = element;    
    
    pObject->Pointer = CLS_1_6_OBJECT_ELE[element].Pointer;

    /*�¶��õ�����������*/
    if((OI_ENGY_CONSUMPTION_MONTH==OI)||(OI_ENGY_CONSUMPTION_STEP_BILL==OI))
    {
         pObject->Pointer += 2;  
    }    

    return DAR_0_OK;

}
/*"************************************************"*/
/*ѭ����ȡ�������� class3-6��*/
INT8U Get_Variable_Object(SUB_OBJECT *pObject)
{
    INT8U ret = DAR_4_OANODEF;
    INT8U i;

    for(i=3;i<=6;i++)
    {    
        ret = get_class1_6_object(pObject,i); 
        if(DAR_4_OANODEF != ret)   /*����δ����Ĵ���,��ʾ���ҵ�OI,��OAD�Ƿ�,��IID����*/
        {
            return ret;
        }  
        else
        {
            
        }    
    }

    return ret;   
}  

/*****************************************************************************
 �� �� ��  : get_class1_6_Obis
 ��������  : ��ȡ1-6���OBIS��Ϣ
 �������  : SUB_OBJECT *pObject  
             INT16U *pObis        
             ST_OBIS_ID *pObisId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 11
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void get_class1_6_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info)
{
    const ST_OAD_OBIS_INFO* pOneObjectList = NULL;
    INT8U class_num,AId,seq_num;

    class_num = pObject->Class;
    AId       = pObject->Oad.AId;
    seq_num    = pObject->seq_num;
    
    if(ATT_CLS1_CLS6_ATT_2_DATA == AId)
    {
        pOneObjectList =(const ST_OAD_OBIS_INFO*) pONEOBJECTLIST[class_num-1];
    }
    else if((ATT_L_CURRENT == AId)&&(OI_CURRENT == pObject->Oad.Oi.asInt))
    {
        pOneObjectList = stCls3_AID_4_ObisInfo;
    }    
    else if((EN_CLS1_ATT_4_HIGH_DATA == AId)&&(1 == class_num))
    {
        pOneObjectList = (const ST_OAD_OBIS_INFO*)stCls1_AID_4_ObisInfo;
    }
    else
    {    
        /*AID�ĺϷ����Ѿ���get object�ж���,����������Բ����ٴ���*/
    }
    
    /*���ݾ�̬������ƴ���*/
    if(NULL != pOneObjectList)
    {    
        pObis_Info->Obis   = pOneObjectList[seq_num].Obis; 
        pObis_Info->num    = pOneObjectList[seq_num].num; 
        
        LIB_ConstMemCpy(&pOneObjectList[seq_num].FormatId, &pObis_Info->FormatId, 3); 
    }
    else
    {
        pObis_Info->num= 0;
    }    
    
} 
/*"************************************************"*/

