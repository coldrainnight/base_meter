/*"***********************************************************		
FileName: OC.h		
Author: ������       
Version : 1.0       Date: 2017.2.16	
Description: �����������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#ifndef _OC_H_
#define _OC_H_

#include "system\Sys_def.h"
#include "Driver\device\AO.h"

/*"������Ƶ�ID"*/
typedef enum 
{
    E_ID_BUZZER          = 0,      /*"������"*/    
    //E_ID_LED_BGD         ,          /*"�����"*/
    //E_ID_LED_RELAY     ,           /*"��բ��"*/
    E_ID_RELAY,                        /*"�̵���"*/
    E_ID_MT ,                            /*"�๦�������"*/
    E_ID_MAX_AO                     /*""*/
}OC_ID_EN;

/*"������"*/
typedef enum 
{
    E_BUZ_CLOSE = 0x00,               /*"�ر�"*/
    E_BUZ_CARD_SUC ,                  /*"�忨�ɹ�"*/
    E_BUZ_CARD_ERR ,                  /*"�忨ʧ��"*/
    OC_BUZ_MODE_NUM                /*"������ģʽ����"*/
}OC_BuzMode_EN;

typedef enum
{
    E_BUZ_SC_PREPAY = 0x00,        /*"����Դ��Ԥ���� "*/
    E_BUZ_MAX_SC                          /*"����Դ����"*/
}OC_BuzSource;

typedef struct
{   
    INT8U   BuzzTimes;	                  /*"ѭ������"*/    
    INT8U   Repeat;                         /*"�Ѿ�ѭ���Ĵ���"*/
    INT8U   Step;                             /*"���е��Ĳ���"*/
    INT8U   RunTime;                      /*"ÿһ�ַ��������е�ʱ��"*/
    INT8U   PauseTime;                  /*"ÿһ�ַ�����ֹͣ��ʱ��"*/
    INT8U	  Times;                          /*"ÿһ��ʱ��"*/
}OC_BUZZ_STATUS;

/*"LED"*/
typedef enum 
{
    E_LED_CONT_CLOSE_MODE = 0,        /*"�ر�ģʽ "*/
    E_LED_CONT_TWINKLE_MODE,          /*"��˸ģʽ"*/
    E_LED_CONT_ALWAYS_MODE,           /*"����ģʽ"*/
    E_LED_CONT_MODE_NUM                  /*"ģʽ����"*/        
}LED_CONTINUE_MODE_EN;

typedef enum 
{
    E_LED_CLOSE_MODE = 0,        /*"�ر�ģʽ "*/
    E_LED_ALWAYS_MODE,           /*"����ģʽ"*/
    E_LED_TWINKLE_MODE,          /*"��˸ģʽ"*/    
    E_LED_MODE_NUM                  /*"ģʽ����"*/        
}LED_MODE_EN;

typedef enum
{
//    E_LED_RELAY=0,         /*"�̵���led "*/
    E_ID_LED_BGD = 0,       /*"����"*/
    E_ID_LED_RELAY = 1,           /*"��բ��"*/
    //E_ID_LED_POWER,                  /*"��Դled "*/
    //E_ID_LED_ALARM,                   /*"����led"*/
    //E_ID_LED_PREPAY,               /*"Ԥ���ѱ���led"*/
    E_ID_LED_NUM_SC,                            /*"����"*/

    //E_ID_LED_RELAY,
    
     E_ID_LED_ALL = 0xff,     /*"���GԴ�Ķ�����G����⴦�?*/
}LED_TYPR_EN;

typedef enum
{
    E_LED_SC_INIT = 0,                     /*"��ʼ�� "*/
    //E_LED_SC_COMM,                   /*"ͨ�� "*/
    //E_LED_SC_KEY,                      /*"����"*/
    //E_LED_SC_REPAY,                  /*"Ԥ����"*/
    //E_LED_SC_ALARM,                  /*"����"*/
    E_LED_SC_EVENT,                  /*"�¼�"*/
    E_LED_SC_SNG,                      /*"����"*/
    //E_LED_SC_REPAY_TWINKLE,                  /*"Ԥ������˸"*/
    
    E_LED_SOURCE_SC,                 /*"����"*/
    
    E_LED_SC_NONE = 0xff,                 /*"ȡ��Դ�����߲��i��ʼ��״̬"*/

}LED_SOURCE_EN;


typedef struct
{
    INT8U   ModeId;                        /*"��ǰģʽ"*/    
    INT16U ActTime;                       /*"ʣ��ʱ�� ��λs"*/
}OC_LED_STATUS;

typedef struct
{
    INT8U   LedCtrl[E_LED_SOURCE_SC];
}OC_LED_CTRL;


typedef enum
{
    E_LED_RELAY_SC_PREPAY=0,         /*"Ԥ���� "*/
    E_LED_RELAY_SC_INIT,                  /*"��ʼ�� "*/
    E_LED_RELAY_SC_SNG,                   /*"����"*/
    E_LED_RELAY_SC                            /*"����"*/
}LED_RELAY_SOURCE_EN;

typedef enum
{
    E_LED_BGT_SC_INIT,                     /*"��ʼ�� "*/
    E_LED_BGT_SC_COMM,                   /*"ͨ�� "*/
    E_LED_BGT_SC_KEY,                      /*"����"*/
    E_LED_BGT_SC_REPAY,                  /*"Ԥ����"*/
    E_LED_BGT_SC_ALARM,                  /*"����"*/
    E_LED_BGT_SC_EVENT,                  /*"�¼�"*/
    E_LED_BGT_SC_SNG,                      /*"����"*/
    E_LED_BGT_SC                               /*"����"*/
}LED_BGD_SOURCE_EN;

/*"�̵���"*/
typedef enum
{
    E_RELAY_SC_PREPAY=0,                /*"Ԥ����"*/
#ifdef THREE_PHASE
    E_RELAY_SC_EVENT,                      /*"�¼�"*/  
#endif
    MAX_RELAY_SC_NUM                     /*"����"*/
}ST_RELAY_SOURCE_EN;
typedef  struct
{
    EN_OC_RELAYACTMODE  Mode;     /*"�̵�������ģʽ  ���á����õ�ƽ����������"*/
    EN_OC_RELAYACT  cmdAct;   /*"�̵��������  ����բ"*/
    INT8U  Flag;     /*"�̵�����ǰ�Ƿ��ڶ����� 1Ϊ������ 0Ϊ�Ƕ�����"*/
    INT16U  ActTimer; /*"�̵�����������ʱ�� ��λ������ "*/
    INT16U  PulseWidth; /*"�̵����������� ��λ������ "*/
    INT16U  ReActTimer; /*"�̵������¶���ʱ�� ��λ ��"*/
}ST_RELAY_ACT_STATUS;
#define _HOLD      0/*"�̵�����բ"*/
#define _RELEASE   1/*"�̵�����բ"*/

/*"�๦�����"*/
typedef struct
{
    INT8U  Mode;            /*"��ǰ�๦�ܿ�ģʽ"*/
    INT8U  ActDelayTimer;   /*"�๦�ܿڶ�����ʱʱ�� ��λ��10ms"*/
    INT8U  ActTimer;        /*"�๦�ܿڶ���ʱ�� ��λ��10ms"*/
}ST_MULTI_STATUS;
/*"��λ10ms"*/
#define MULTI_TARIFF_DELAY  1    /*"�յ�ʱ��Ͷ���ź����Ҫ����ʱ10ms���"*/
#define MULTI_TARIFF_TIME    8     /*"ʱ��Ͷ���ź����80ms"*/

#define MAX_LED_CMD  10
typedef struct
{
    INT8U intf; /*" ���ƽӿڣ�0ΪOC_LedOff�� 1Ϊ OC_LedOnContin "*/
    OC_ID_EN LedID;
    LED_CONTINUE_MODE_EN ModeID;
    INT8U Source;
    INT32U Ms_Time32U;
}OC_LED_CMD;

/*"�������ⲿҪ���õĺ���"*/
extern void OC_Init(void);       /*"�ϵ����"*/
extern void OC_Int(void);        /*"10�������"*/
extern void OC_TaskSec(void);    /*"��ѭ������"*/

/*"����������"*/
extern void OC_BuzzerOff(OC_BuzMode_EN ModeID, OC_BuzSource Source);
extern void OC_BuzzerOnSingle (OC_BuzMode_EN ModeID);
extern void OC_BuzzerOnContin (OC_BuzMode_EN ModeID , OC_BuzSource Source, INT16U Ms_Time);

/*"LED����"*/
extern void OC_LedOnSingle (LED_TYPR_EN  LedID,LED_MODE_EN ModeID,INT8U SecTime);
extern void OC_LedOnContin (LED_TYPR_EN LedID, LED_SOURCE_EN SourceID);
extern void OC_LedOff(LED_TYPR_EN LedID, LED_SOURCE_EN SourceID);

/*"�̵�������"*/
extern void OC_RelayAct (ST_RELAY_SOURCE_EN Source,EN_OC_RELAYID RelayID, EN_OC_RELAYACTMODE RelayActMode, EN_OC_RELAYACT Act,INT16U MsPulseTime);
extern void OC_RelayActReDo (void);
extern EN_OC_RELAYACT OC_GetRelayCtrlStatus(void);
extern void OC_SetRelayCtrlStatus(EN_OC_RELAYACT Act);
#ifdef THREE_PHASE
extern INT8U OC_GetAlmRelayCmd(void);
#endif

/*"�๦�ܿڿ���"*/
extern void OC_MultiTermnSw(EN_OC_MultiTermnMode Mode);
extern INT8U OC_ReadMultiMode(void);
extern void OC_MultiTermn(EN_OC_MultiTermnMode Mode);
extern void OC_MultiTermn_Chng(void);

#endif  

