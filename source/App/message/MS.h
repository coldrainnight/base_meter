#ifndef MS_H
#define MS_H

#include "PlatForm.h"

#define   W_MODE   1         /*"�����������������"*/
#define   R_MODE   0         /*"���������Ļ�ȡ����"*/

#define   MS_SUCCESS         (0)  /*"���ݽ��������ɹ�"*/
#define   MS_OBIS_ERR        (-1)  /*"���ݽ��������ʶ����"*/
#define   MS_LEN_ERR         (-2)  /*"���ݽ������ݳ��ȴ���"*/
#define   MS_BUFF_OVERFLOW   (-3)  /*"���ݽ������ݻ������"*/
#define   MS_LIST_NUM_OVRE   (-4)  /*"���ݽ���LIST��Ŀ����"*/
#define   MS_W_AUTH_ERR      (-5)  /*"���ݽ���дȨ�޴���"*/
#define   MS_ACTION_AUTH_ERR (-6)  /*"���ݽ�������Ȩ�޴���"*/

#define   MS_OTHER_ERR         (-7)  /*"���ݽ�������ʧ��"*/

#define   MS_YEARZONE (-17) /*"��ʱ������"*/
#define   MS_DAILYSLOT (-18) /*"��ʱ������"*/
#define   MS_TRIFF (-19) /*"��������"*/


#define   MAX_ID_LIST_NUM    (10) /*"LIST����ʱ֧�ֵ���������"*/

#define   SPEC_DAY_SET_SIZE  (6)    /*�������ճ���*/  
/*"----------���ܱ�����״̬����---------"*/
typedef enum
{
    E_FAC = 0,/*"����״̬"*/
    E_PRIVATE, /*"˽Կ״̬"*/
    E_PRGEVENT_START,/*"���״̬"*/
    E_FIRST_PWR_ON,/*"�����ϵ�"*/
    E_PLM_WORKING,/*"�ز�ģ������ͨ��/����"*/
    E_OPEN_COVER,/*"�����"*/
    E_OPEN_TAIL, /*"���˸�"*/
    E_CLK_ERR, /*"ʱ�ӹ���"*/
    
    E_CLK_BAT_LOSS, /*"ʱ�ӵ��Ƿѹ"*/
    E_DIS_BAT_LOSS, /*"�͹��ĵ��Ƿѹ"*/
    E_MAGNET, /*"�ų�����"*/
    E_MTR_RST,/*"���λ"*/
    E_LRM_PD_DATA_ERR,/*"��������������ݲ��Ϸ�"*/
    E_OSCILLATOR_ERR,/*"�������"*/
    E_RAM_VALID,/*"RAM����ά�ֺϷ�"*/
    E_MTR_INIT,/*"����ʼ��"*/
    
    E_MU_JUST_MTR,/*"У��״̬ 0 δУ��  1��У��"*/
    E_RTC_CALIB, /*"RTC����У׼״̬ 0 δУ׼  1��У׼"*/
    E_LOW_PWR,/*"�͹���״̬"*/
    E_MON_STEP,/*"�½���"*/
    E_YEAR_STEP,/*"�����"*/
    E_TIME_SLOT_NOW,/*"��ǰ����ʱ��  0��һ�� 1�ڶ���"*/
    E_STEP_SET_NOW,/*"��ǰ���н���  0��һ�� 1�ڶ���"*/
    E_TARIFF_SET_NOW,/*"��ǰ���з��ʵ��  0��һ�� 1�ڶ���"*/
    
    E_OVERDRAFT,/*"͸֧"*/
    E_ESAM_ERR,/*"ESAM����"*/
    E_EEP_ERR,/*"EEPROM����"*/
    E_FLASH_ERR,/*"�ⲿFLASH�洢����"*/
    E_PREPAY,/*"Ԥ���ѱ�   0��Ԥ����   1Ԥ����"*/
    E_PREPAY_ENGY,/*"Ԥ��������   1Ԥ������   0 ��Ԥ������"*/
    E_PREPAY_MONEY,/*"Ԥ����ѱ�   1Ԥ�����   0 ��Ԥ�����"*/
    E_MU_IC_ERR,/*"����оƬ����"*/
    E_MU_DATA_READY,/*"��������׼������״̬  1����  0δ����"*/
    
    E_DEMAND_MODE,/*"��������ģʽ  1 ����ʽ   0 ����ʽ "*/
    E_AUX_PWR,  /*"������Դ����  1 ������Դ����    0 �е繩��"*/
    E_BAT_PWR, /*"��ع���  1 ��ع���    0 ����Դ����"*/
    E_RELAY_CMD_STATUS,/*"�̵�������״̬  1 ��բ    0 ��բ"*/
    E_RELAY_ALARM,/*"�̵���Ԥ��բ״̬  1 ����    0 �ޱ���"*/
    E_RELAY_PROTECT,/*"����״̬  1 ����    0 �Ǳ���"*/
    E_AUTH_STATUS,/*"��ȫ��֤  1 ��Ч    0 ʧЧ"*/
    E_IRAUTH_STATUS,/*"������֤��Ч״̬0��Ч 1��Ч"*/
    
    E_LOCAL_CREDIT,/*"���ؿ���  1 δ����    0 ����"*/
    E_REMOTE_CREDIT,/*"Զ�̿���  1 δ����    0 ����"*/
    E_VOLT_NORMAL,/*"��ѹ���ڵ������ѹ���ٽ��ѹ��"*/
    E_VOLT_RELAY_OK,/*"��ѹ���ڼ̵���������ѹ"*/
    E_VOLT_ICCARD_OK,/*"��ѹ����IC��������ѹ"*/
    E_VOLT_LCD_OK,/*"��ѹ������ʾ���⹤����ѹ"*/
    E_VOLT_COM_OK,/*"��ѹ����ͨ�Ź�����ѹ"*/
    E_CURR_PROTECT,/*"�������ڱ�����������������բ��"*/
    
    E_PWR_DOWN, /*"����"*/
    E_RELAY_ERR, /*"�̵�������"*/
    E_RELAY_OFF_SUCCESS,/*"�̵�����բ�ɹ�"*/
    E_RELAY_ON_SUCCESS,/*"�̵�����բ�ɹ�"*/
    E_RELAY_STATUS,/*"�̵���״̬ͨ�������źź͹����жϼ̵���ͨ��  1 �� 0 ͨ"*/
    E_PWR_Q_LA_DIR,/*"A���޹����ʷ���0 ����   1  ���� ����ͬ��"*/
    E_PWR_Q_LB_DIR,/*"B���޹����ʷ���"*/
    E_PWR_Q_LC_DIR,/*"C���޹����ʷ���"*/
    
    E_PWR_Q_DIR,   /*"���޹����ʷ���"*/
    E_PWR_P_LA_DIR,/*"A���й����ʷ���"*/
    E_PWR_P_LB_DIR,/*"B���й����ʷ���"*/
    E_PWR_P_LC_DIR,/*"C���й����ʷ���"*/
    E_PWR_P_DIR,   /*"���й����ʷ��� "*/
    E_LOSS_VOLT_LA, /*"A��ʧѹ"*/
    E_LOSS_VOLT_LB, /*"B��ʧѹ"*/
    E_LOSS_VOLT_LC, /*"C��ʧѹ"*/
    
    E_LOSS_VOLT_ALL,/*"ȫʧѹ"*/
    E_LOSS_CURR_LA, /*"A��ʧ��"*/
    E_LOSS_CURR_LB, /*"B��ʧ��"*/
    E_LOSS_CURR_LC, /*"C��ʧ��"*/
    E_BRK_PHASE_LA,/*"A�����"*/
    E_BRK_PHASE_LB,/*"B�����"*/
    E_BRK_PHASE_LC,/*"C�����"*/
    E_OVER_CURR_LA, /*"A�����"*/
    
    E_OVER_CURR_LB, /*"B�����"*/
    E_OVER_CURR_LC, /*"C�����"*/
    E_CUT_CURR_LA, /*"A�����"*/
    E_CUT_CURR_LB, /*"B�����"*/
    E_CUT_CURR_LC, /*"C�����"*/
    E_OVER_VOLT_LA, /*"A���ѹ"*/
    E_OVER_VOLT_LB, /*"B���ѹ"*/
    E_OVER_VOLT_LC, /*"C���ѹ"*/
    
    E_UNDER_VOLT_LA, /*"A��Ƿѹ"*/
    E_UNDER_VOLT_LB, /*"B��Ƿѹ"*/
    E_UNDER_VOLT_LC, /*"C��Ƿѹ"*/
    E_PWR_REVERSE, /*"��������"*/
    E_PWR_REVERSE_LA,/*"A�๦�ʷ���"*/
    E_PWR_REVERSE_LB,/*"B�๦�ʷ���"*/
    E_PWR_REVERSE_LC,/*"C�๦�ʷ���"*/
    E_OVER_LOAD_LA, /*"A�����"*/
    
    E_OVER_LOAD_LB, /*"B�����"*/
    E_OVER_LOAD_LC, /*"C�����"*/
    E_OVER_DEMAND,/*"��������"*/
    E_OVER_DEMAND_PA,/*"�����й���������"*/
    E_OVER_DEMAND_NA,/*"�����й���������"*/
    E_OVER_DEMAND_I, /*"I�����޹���������"*/
    E_OVER_DEMAND_II,/*"II�����޹���������"*/
    E_OVER_DEMAND_III,/*"III�����޹���������"*/
    
    E_OVER_DEMAND_IV, /*"IV�����޹���������"*/
    E_VOLT_IMBAL, /*"��ѹ��ƽ��"*/
    E_CURR_IMBAL, /*"������ƽ��"*/
    E_CURR_BADLY_IMBAL, /*"�������ز�ƽ��"*/
    E_OVER_FACTOR, /*"�ܹ�������������"*/
    E_OVER_FACTOR_LA, /*"A�๦������������"*/
    E_OVER_FACTOR_LB, /*"B�๦������������"*/
    E_OVER_FACTOR_LC, /*"C�๦������������"*/
    
    E_VOLT_REVERSE, /*"��ѹ������"*/
    E_CURR_REVERSE, /*"����������"*/
    E_PERMISS_LIMIT,/*����Ȩ��*/
    E_MGMT_COMM_ERR, /*" ˫оͨ�Ź���"*/
    
    MAX_MTR_STA,
    E_STA_NO_DEF=0xFF/*δ����*/
}EN_MTR_STATUS;

#define  MTR_STA_BYTE    ((MAX_MTR_STA >> 3) + 1)
#define     DC_BUF_SIZE  64

#define MSADDR_IN_RAM(MSAddr,Result) {if(MSAddr>RAM){Result=TRUE;}else{Result=FALSE;}}

/*�����������õĵ�ַת�ļ�ϵͳ�Ĳ���*/
#define MSADDR_TO_FILEINFO(Addr,FileName,Offset) {FileName = (ENUM_FS_ID)(Addr>>16);Offset=(INT16U)Addr;}

/*�����������õ�ַתRAM��ַ*/
#define MSADDR_TO_RAMADDR(MSAddr,RamAddr)  {RamAddr = MSAddr - RAM;}

extern INT8U DCDataBuf[DC_BUF_SIZE];/*"�������ļ�¼�ļ����ݻ������������¼���¼"*/
extern INT8U oprateCode[4];
#ifdef THREE_PHASE
/*"�����������궨��"*/
#define CLR_MTR()    {\
                     SaveRcrd(E_MTR_CLR_EVNT_RCRD, E_EVTLST_CLRMTR);\
                     Action(E_LRM_CLR, NULL, NULL);\
                     DCDataBuf[0] = 0xFF; \
                     DCDataBuf[1] = 0xFF;\
                     Action(E_MNT_EVNT_CLR, &DCDataBuf[0], NULL);\
                     Action(E_RCRD_CLR, &DCDataBuf[0], NULL);\
                     Action(E_UIDATA_CLR, NULL, NULL);\
                     Action(E_MAX_DEMAND_CLR, NULL, NULL);\
                     }
#else
/*"�����������궨��"*/
#define CLR_MTR()    {\
                     SaveRcrd(E_MTR_CLR_EVNT_RCRD, E_EVTLST_CLRMTR);\
                     Action(E_LRM_CLR, NULL, NULL);\
                     DCDataBuf[0] = 0xFF; \
                     DCDataBuf[1] = 0xFF;\
                     Action(E_MNT_EVNT_CLR, &DCDataBuf[0], NULL);\
                     Action(E_RCRD_CLR, &DCDataBuf[0], NULL);\
                     Action(E_UIDATA_CLR, NULL, NULL);\
                     }
#endif


/*"������ݳ�ʼ���궨��"*/
#define  MTR_INIT() {\
                     if(TRUE != FS_Init(FIRST_PWR_ON)){\
                        MCU_Reset();\
                     };\
                     LRM_Init(FIRST_PWR_ON);\
                     MNT_Init(FIRST_PWR_ON);\
                     UI_Init(FIRST_PWR_ON);\
                     RCRD_Init(FIRST_PWR_ON);\
        			 Action(E_FIRST_PWR_UP_OVER, NULL, NULL);\
        			 Action(E_RESET_FAC_REMAIN_TIME, NULL, NULL);\
        			 CLR_MTR()\
        			 LCD_Close();\
        			 Ans_MtrClr();\
                     MCU_Reset();\
                    }
#define  FRMT_NUM  (sizeof(stFormatList)/sizeof(ST_FMT_ATTR))
extern INT16S GetSingle(INT16U id, INT8U *pBuff);
extern INT16S GetList (INT16U *pIdList, INT8U IdNum, INT8U *pBuff, INT16U BuffSize);
extern INT8S Set (INT16U *pIdList, INT8U IdNum, INT8U *pData, INT8U *pLenList);
extern INT8S Action (INT16U id, void *pdata, void *pbuff);
extern INT16S GetRcrd (INT16U id, void *pdin, void *pdout, INT16U len);
extern INT8S SaveRcrd(INT16U file, INT16U ListObis);
extern INT8U GetFrmt(INT16U id, ST_FMT_ATTR *pformat);
extern void Push(EN_MTR_STATUS StaId, INT8U Status);
extern INT8U ChkStatus(EN_MTR_STATUS StaId);

extern INT8U FRMT_sizeofFormat(INT8U format);
extern INT8U FRMT_DataChange (INT8U *srcData, INT8U *destData, ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat);
extern INT16U FRMT_NdataChange(INT8U *srcData, INT8U *destData, ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat,INT16U Len);
#endif
