/*"*****************************************************************************"*/
/*"  FileName: class6_data_varible.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  ���ݱ������ļ�   "*/
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
/*class6 ����2�б�*/
const ST_Class1_6_Object CLASS6OBISLIST[MAX_CLASS6_LIST_NUM] =
{
     /*��3��Ҫ��ӦCLS_1_6_OBJECT_ELE��ͬʱ�仯*/    
    {0x200f,1,E_ELE_FEQ},            /* 1.����Ƶ�� */
    {0x2011,1,E_ELE_CLK_VOLT},       /* 2.ʱ�ӵ�ص�ѹ */    
    {0x2014,7,E_ELE_STATUS_8},       /* 3.���ܱ�����״̬�� */
    {0x201A,1,E_ELE_PRICE},          /* 4.��ǰ��� */
    {0x201B,1,E_ELE_PRICE},          /* 5.��ǰ���ʵ�� */
    {0x201C,1,E_ELE_PRICE},          /* 6.��ǰ���ݵ�� */
    {0x202d,1,E_ELE_MONEY},          /* 7.����ǰ��͸֧���*/ 
    {0x202E,1,E_ELE_MONEY},          /* 8.�ۼƹ����� */
    {0x2031,1,E_ELE_A_ENGY},         /*9.�¶��õ���*/
    {0x2032,1,E_ELE_A_ENGY},         /*10.���ݽ����õ���*/
    {0x2040,1,E_ELE_STATUS_16},      /*11.��������ִ��״̬��*/
    {0x2041,1,E_ELE_STATUS_16},      /*12.�����������״̬��*/
    {0x2042,1,E_ELE_STATUS_32},      /*13.��������ִ��״̬��*/
    {0x4022,1,E_ELE_STATUS_16},      //14�忨״̬��		
    {0x2013,1,E_ELE_CLK_WORK_TIME},  /* 15.ʱ�ӵ�ع���ʱ�� */   

#ifdef THREE_PHASE
    {0x2010,1,E_ELE_TEMP},           /* 16.�����¶�  */
    {0x2012,1,E_ELE_LOW_POWER_VOLT}, /* 17.ͣ�糭���ص�ѹ  */ 
    {0x2017,1,E_ELE_CURR_A_DEMAND},  /* 18.��ǰ�й����� */    
    {0x2018,1,E_ELE_CURR_R_DEMAND},  /* 19.��ǰ�޹����� */
    {0x2026,1,E_ELE_IMBANLCE_RATE},  /* 20.��ѹ��ƽ���� */
    {0x2027,1,E_ELE_IMBANLCE_RATE},  /* 21.������ƽ���� */
    {0x2029,4,E_ELE_VAH},            /* 22.��ʱֵ */    
    {0x2131,2,E_ELE_VOLT_RATE},      /*23.A��ѹ�ϸ��� ���յ�ѹ�ϸ����뵱�µ�ѹ�ϸ���*/
    {0x2132,2,E_ELE_VOLT_RATE},      /*24.B��ѹ�ϸ���*/
    {0x2133,2,E_ELE_VOLT_RATE}       /*25.C��ѹ�ϸ���*/       
#endif
};


/*class6 ����2 OBIS�б�*/
const ST_OAD_OBIS_INFO stCls6ObisInfo[MAX_CLASS6_LIST_NUM] =
{
    {E_FREQ,           1, 0, FORMAT_FREQ,  FUN_NULL,     E_INFO_NULL},   /* 1.����Ƶ�� */
    {E_CLK_BAT_VOLT,   1, 0, FORMAT_BV,    FUN_NULL,     E_INFO_NULL},   /* 2.ʱ�ӵ�ص�ѹ */    
    {E_RUN_STA_1,      7, 0, FORMAT_WORD,  FUN_RUNSTATUS,E_INFO_NULL},   /* 3.���ܱ�����״̬�� */
#ifdef PM_MODLE    
    {E_CUR_PRICE,      1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_PRICE},   /* 4.��ǰ��� */
    {E_TARIFF_PRICE,   1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_TRIFF_PRICE},   /* 5.��ǰ���ʵ�� */
    {E_STEPLEVEL_PRICE,1, 0, FORMAT_PRICE, FUN_NULL,     E_INFO_NOW_STEP_PRICE},   /* 6.��ǰ���ݵ�� */
    {E_OVERDRAFT_MONEY,1, 0, FORMAT_MONEY, FUN_NULL,     E_INFO_OVERDRAW_MONEY},   /* 7.����ǰ��͸֧���*/ 
    {E_BUY_ENGY_TOTAL, 1, 0, FORMAT_MONEY, FUN_NULL,     E_INFO_SUM},   /* 8.�ۼƹ����� */
    {E_ENGY_CONSUMPTION_MONTH,     1, 0, FORMAT_BILLENGY, FUN_NULL, E_INFO_NOW_LAST_SUM_ENGY},   /*9 .�¶��õ���*/
    {E_ENGY_CONSUMPTION_STEP_BILL, 1, 0, FORMAT_BILLENGY, FUN_NULL, E_INFO_NOW_STEP_SUM_ENGY},   /*10.���ݽ����õ���*/
    {E_CTRCMD_STATUS,  1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*11.��������ִ��״̬��*/
    {E_CTRERR_STATUS,  1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*12.�����������״̬��*/
    {E_CTRCMD_STATUS,  2, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},          /*13.��������ִ��״̬�� ����״̬*/
    {E_INSETCARD_STATUS,1, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},         /*14�忨״̬�� */
#endif    
    {E_CLK_BAT_TIME,  1, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_TIME},       /*15.ʱ�ӵ�ع���ʱ�� */   

#ifdef THREE_PHASE
    {E_MTR_TEMPERATRUE, 1, 0, FORMAT_S2_1_UBCD2_1, FUN_NULL, E_INFO_NULL}, /* 16.�����¶�  */
    {E_DISP_BAT_VOLT, 1, 0, FORMAT_BV, FUN_NULL, E_INFO_NULL},             /* 17.ͣ�糭���ص�ѹ  */
    {E_CURRENT_A_DEMAND, 1, 0, FORMAT_PW, FUN_NULL, E_INFO_NOW_PA_DEMAND}, /* 18.��ǰ�й����� */    
    {E_CURRENT_R_DEMAND, 1, 0, FORMAT_R_PW, FUN_NULL, E_INFO_NOW_R_DEMAND},/* 19.��ǰ�޹����� */	
    {E_VOLTIMB_RATE, 1, 0, FORMAT_U2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},    /* 20.��ѹ��ƽ���� */
    {E_CURRIMB_RATE, 1, 0, FORMAT_U2_2_UBCD2_0, FUN_NULL, E_INFO_NULL},    /* 21.������ƽ���� */
    {E_AH_BLK, E_PHASE_VARIBLE_SUM_ID, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},  /* 22.��ʱֵ */    
    {E_DA_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL/*��ʾ���������⴦��*/},  /*23.A��ѹ�ϸ��� ���յ�ѹ�ϸ����뵱�µ�ѹ�ϸ���*/
    {E_DB_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*24.B��ѹ�ϸ���*/
    {E_DC_VOLTHEALTH, 2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*25.C��ѹ�ϸ���*/        
#endif	
};
/*"************************************************"*/

