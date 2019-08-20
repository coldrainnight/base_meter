
#ifndef FS_DEF_H
#define FS_DEF_H


#include "system\Sys_def.h"

typedef enum
{
    /*EEP ���ļ�����*/

    /*�����ļ�*/
    E_FILE_LRM = 0,/*�������������ļ�*/
    E_FILE_UI_PARA_WITH_BACKUP,/*�û��ӿ���RAM���ݵĲ����ļ�*/
    E_FILE_UI_PARA_NO_BACKUP,/*�û��ӿڲ����ļ�*/
    E_FILE_PM_PARA,/*Ԥ���Ѳ����ļ�*/
    E_FILE_TR_PARA,/*��ʱ�����ļ�*/
    E_FILE_RCRD_PARA,/*��¼���ղ����ļ�*/
    E_FILE_MNT_PARA,/*���״̬�������ļ�*/
    
    #if defined (THREE_PHASE)
    E_FILE_DU_PARA, /*���������ļ�*/
    #endif
    
    /*�����ļ�*/
    E_FILE_LRM_DATA,    /*����ģ�������ļ�*/
    E_FILE_PD_DATA,/*����洢�ļ�*/
    E_FILE_UI_DATA,     /*����ģ�������ļ�*/
    E_FILE_MNT,         /*���״̬���ģ��洢�ļ�*/
    
    #if defined (THREE_PHASE)  
    E_FILE_DU_DATA,     /*���������ļ�*/
    #endif
    
    E_FILE_PM_DATA,     /*Ԥ���������ļ�*/
    E_FILE_RCRD_DATA,     /*��¼���������ļ�*/
    E_FILE_AUTO_DATA,     /*�Զ����������ļ�*/

    
    /*��¼���մ洢�ļ�*/
    #if defined (SINGLE_PHASE)
    E_FILE_MIN_FRZ_IDX,/*���Ӷ�������*/

    #elif defined (THREE_PHASE)
    E_FILE_PRG_EVNT,/*����¼���¼*/
    E_FILE_OSCILLATOR_ERR_EVNT,/*" ��������¼� "*/
    E_FILE_SYS_RST_EVNT,/*��λ�����¼�*/
    E_FILE_MONTH_BILL_FRZ,
    E_FILE_MIN_FRZ_IDX,/*���Ӷ�������*/

    #endif

    E_FILE_EEP_END,/*EEP ID����*/
/////////////////////////////////////////////////////////////////
/////FLASH ���ļ�����/////////////////////

    E_FILE_FLASH_START = 0x1000,



    /*��¼���մ洢�ļ�*/
    #if defined (SINGLE_PHASE)
    E_FILE_OVER_CURR_LA_EVNT,/*����A*/
    E_FILE_PWR_DOWN_EVNT, /*����*/  

    E_FILE_MTR_COVER_OPEN_EVNT,/*�����¼�*/
    E_FILE_RELAY_TRIP_EVNT,/*��բ�¼��¼�*/
    E_FILE_RELAY_CLOSE_EVNT,/*��բ�¼��¼�*/
    
    E_FILE_PRG_EVNT,/*���*/
    E_FILE_MTR_CLR_EVNT, /*�������*/
    E_FILE_EVNT_CLR_EVNT,/*�¼�����*/
    E_FILE_JUST_TIME_EVNT,/*Уʱ�¼�*/
    E_FILE_BROADCAST_JUST_TIME_EVNT,/*�㲥Уʱ�¼�*/
    E_FILE_DAY_TBL_PRG_EVNT,/*��ʱ�α���*/
    E_FILE_YEAR_TBL_PRG_EVNT,/*��ʱ������*/
    E_FILE_WEEKEND_PRG_EVNT,/*�����ձ���¼�*/
    E_FILE_HOLIDAY_PRG_EVNT,/*�ڼ��ձ��*/
    E_FILE_OSCILLATOR_ERR_EVNT,/*" ��������¼� "*/
    E_FILE_SYS_RST_EVNT,/*��λ�����¼�*/
    E_FILE_BILL_DAY_PRG_EVNT ,/*�����ձ���¼�*/
    E_FILE_A_ENGY_COM_PRG_EVNT,/*�й����ģʽ�ֱ���¼�*/
    E_FILE_TRIFF_PRG_EVNT,/*���ʱ����¼�*/
    E_FILE_STEP_PRG_EVNT,/*���ݱ����¼�*/
    E_FILE_KEY_UPDATE_EVNT,/*��Կ�����¼�*/
    E_FILE_CARD_PLUG_IN_ERR_EVNT,/*�忨�쳣�¼�*/
    E_FILE_BUY_ENGY_EVNT,/*�����¼�*/
    E_FILE_REFUND_EVNT,/*�˷��¼�*/
    E_FILE_RELAY_ERR_EVNT,/*�̵�������*/
    E_FILE_PWR_FAILURE_EVNT,/*��Դ�쳣�¼�*/
    E_FILE_RTC_ERR_EVNT,/*ʱ�ӹ����¼�*/
    E_FILE_MS_ERR_EVNT,/*����оƬ�����¼�*/
	E_FILE_MGMT_COMM_ERR_EVNT,/*ͨ���쳣�¼�*/
	E_FILE_UPGRADE_EVNT,/*�Ƿ�����Ԫ�����¼�*/
	E_FILE_NOLEG_REPALCE_EVNT,/*�Ƿ�����Ԫ�����¼�*/
	

    E_FILE_MMT_FRZ,/*˲ʱ����*/
    
    E_FILE_MIN_FRZ,/*���Ӷ���*/
    E_FILE_HOUR_FRZ,/*Сʱ����*/
    E_FILE_DAY_FRZ,/*�ն���*/
    E_FILE_MONTH_BILL_FRZ,/*�½��㶳��*/
    E_FILE_MONTH_FRZ ,/*�¶���*/
    E_FILE_DAY_TBL_SW_FRZ,/*��ʱ�α��л�����*/
    E_FILE_YEAR_TBL_SW_FRZ,/*���������л�*/
    E_FILE_TRIFF_SW_FRZ,/*���ʵ���л�*/
    E_FILE_STEP_SW_FRZ, /*" �����л����� "*/
    E_FILE_STEP_BILL_FRZ, /*" ���ݽ��㶳�� "*/

    
    E_FILE_PD_PON_EVNT,/*" ���ϵ��¼ "*/
    E_FILE_MSRL_EVNT,
	E_FILE_MSRN_EVNT,



    #elif defined (THREE_PHASE)
    E_FILE_PD_PON_EVNT,/*" ���ϵ��¼ "*/
    E_FILE_PARA_ERR_EVNT, /*" ���������¼ "*/
    E_FILE_ENG_ERR_EVNT, /*" ���������¼ "*/
    
    E_FILE_LOST_VOLT_LA_EVNT      ,/*A��ʧѹ*/
    E_FILE_LOST_VOLT_LB_EVNT      ,/*B��ʧѹ*/
    E_FILE_LOST_VOLT_LC_EVNT      ,/*C��ʧѹ*/

    E_FILE_LACK_VOLT_LA_EVNT      ,/*A��Ƿѹ*/
    E_FILE_LACK_VOLT_LB_EVNT      ,/*B��Ƿѹ*/
    E_FILE_LACK_VOLT_LC_EVNT      ,/*C��Ƿѹ*/

    E_FILE_OVER_VOLT_LA_EVNT  ,/*A���ѹ*/
    E_FILE_OVER_VOLT_LB_EVNT  ,/*B���ѹ*/
    E_FILE_OVER_VOLT_LC_EVNT  ,/*C���ѹ*/

    E_FILE_BRK_PHASE_LA_EVNT  ,/*A�����*/
    E_FILE_BRK_PHASE_LB_EVNT  ,/*B�����*/
    E_FILE_BRK_PHASE_LC_EVNT  ,/*C�����*/

    E_FILE_LOST_CURR_LA_EVNT   ,/*A��ʧ��*/
    E_FILE_LOST_CURR_LB_EVNT    ,/*B��ʧ��*/
    E_FILE_LOST_CURR_LC_EVNT    ,/*C��ʧ��*/

    E_FILE_OVER_CURR_LA_EVNT    ,/*A�����*/
    E_FILE_OVER_CURR_LB_EVNT    ,/*B�����*/
    E_FILE_OVER_CURR_LC_EVNT    ,/*C�����*/

    E_FILE_BRK_CURR_LA_EVNT     ,/*A�����*/
    E_FILE_BRK_CURR_LB_EVNT     ,/*B�����*/
    E_FILE_BRK_CURR_LC_EVNT     ,/*C�����*/

    E_FILE_PWR_REVERSE_EVNT         ,/*���ʷ���*/
    E_FILE_PWR_REVERSE_LA_EVNT      ,/*A���ʷ���*/
    E_FILE_PWR_REVERSE_LB_EVNT      ,/*B���ʷ���*/
    E_FILE_PWR_REVERSE_LC_EVNT      ,/*C���ʷ���*/

    E_FILE_OVER_LOAD_LA_EVNT        ,
    E_FILE_OVER_LOAD_LB_EVNT        ,
    E_FILE_OVER_LOAD_LC_EVNT        ,

    E_FILE_OVER_PA_DEMAND_EVNT      ,
    E_FILE_OVER_NA_DEMAND_EVNT      ,

    E_FILE_OVER_R_DEMAND_I_EVNT     ,
    E_FILE_OVER_R_DEMAND_II_EVNT    ,
    E_FILE_OVER_R_DEMAND_III_EVNT   ,
    E_FILE_OVER_R_DEMAND_IV_EVNT    ,

    E_FILE_FACTOR_LOW_EVNT      ,
    E_FILE_ALL_LOST_VOLT_EVNT   ,
    E_FILE_AUX_PWR_DOWN_EVNT    , 
    E_FILE_VOLT_INVERSE_EVNT    ,
    E_FILE_CURR_INVERSE_EVNT    ,

    E_FILE_PWR_DOWN_EVNT        ,
    E_FILE_MTR_CLR_EVNT         ,

    E_FILE_DEMAND_CLR_EVNT      ,
    E_FILE_EVNT_CLR_EVNT        ,
    E_FILE_JUST_TIME_EVNT       ,
    E_FILE_DAY_TBL_PRG_EVNT     ,
    E_FILE_YEAR_TBL_PRG_EVNT  ,
    E_FILE_WEEKEND_PRG_EVNT     ,
    E_FILE_BILL_DAY_PRG_EVNT    ,

    E_FILE_MTR_COVER_OPEN_EVNT  ,

    E_FILE_TAIL_COVER_OPEN_EVNT ,

    E_FILE_VOLT_IMBALANCE_EVNT  ,
    E_FILE_CURR_IMBALANCE_EVNT  ,

    E_FILE_RELAY_TRIP_EVNT      ,
    E_FILE_RELAY_CLOSE_EVNT     ,
    E_FILE_HOLIDAY_PRG_EVNT     ,
    E_FILE_A_ENGY_COM_PRG_EVNT  ,
    E_FILE_R_ENGY_COM_PRG_EVNT  ,

    E_FILE_TRIFF_PRG_EVNT               ,
    E_FILE_STEP_PRG_EVNT                ,
    E_FILE_KEY_UPDATE_EVNT              ,
    E_FILE_CARD_PLUG_IN_ERR_EVNT        ,
    E_FILE_BUY_ENGY_EVNT                ,
    E_FILE_REFUND_EVNT                  ,

    E_FILE_CONSTANT_MGNT_DISTURB_EVNT   ,

    E_FILE_RELAY_ERR_EVNT               ,
    E_FILE_PWR_FAILURE_EVNT             ,

    E_FILE_CURR_IMBALANCE_BADLY_EVNT    ,
    E_FILE_RTC_ERR_EVNT,/*ʱ�ӹ����¼�*/
    E_FILE_MS_ERR_EVNT,/*����оƬ�����¼�*/
    E_FILE_SPI_ERR_EVNT,
    E_FILE_UPGRADE_EVNT,
    E_FILE_NOLEG_REPALCE_EVNT,

    /*" �����ļ� "*/

    E_FILE_MMT_FRZ              ,
    E_FILE_MIN_FRZ              ,
    E_FILE_HOUR_FRZ,/*Сʱ����*/
    E_FILE_DAY_FRZ              ,
    E_FILE_MONTH_FRZ            ,
    E_FILE_YEAR_TBL_SW_FRZ    ,    
    E_FILE_DAY_TBL_SW_FRZ       ,
    E_FILE_TRIFF_SW_FRZ     ,
    E_FILE_STEP_SW_FRZ      , /*" �����л����� "*/
    E_FILE_STEP_BILL_FRZ        , /*" ���ݽ��㶳�� "*/
    
    #endif

    
    
    E_FILE_FLASH_END


}ENUM_FS_ID;




#endif

