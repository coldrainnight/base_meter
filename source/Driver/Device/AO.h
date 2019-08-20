/*"***********************************************************		
FileName: AO.h		
Author: ������       
Version : 1.0       Date: 2017.2.16	
Description: �����������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#ifndef _AO_H_
#define _AO_H_

#include "system\Sys_def.h"

/*"���������ֶ���"*/
#define BUZ_ON   1    /*"����������"*/
#define BUZ_OFF  0    /*"�������ر�"*/

extern void AO_Buzzer(INT8U buzAct);
/*"LED���ֶ���"*/
typedef enum 
{
    E_LED_BLIGHT = 0x00,                 /*"�����"*/
    E_LED_RELAY_YELLOW,    /*"�̵���ָʾ��"*/
    E_LED_INIT                    /*"��ʼ��ʱȫ��LED����,ֻ���ڶ�ʱ�����Ľӿڵ���"*/
}EN_OC_LEDID;

#define  LED_ON          1        /*"LED����"*/
#define  LED_OFF         0        /*"LED�ر�"*/
#define  LED_MAX_STATUS  2        /*"���LED״̬"*/
#define  LED_NUM    E_LED_INIT    /*"LED�豸��Ŀ"*/

extern void AO_LED(INT8U Obj, INT8U Act);
/*"�๦�ܿڲ��ֶ���"*/
typedef enum 
{
    E_MULTI_SECOND = 0x00,     /*"������"*/
    E_MULTI_DEMANDPERIOD,    /*"��������"*/
    E_MULTI_TRAFFSWITCH,      /*"ʱ��Ͷ��"*/
    E_MULTI_REACTPULSE,	        /*�޹�����*/	
    E_MULTI_MAX,	                      
}EN_OC_MultiTermnMode;

#define MULTI_ACT_ON    0  /*"�๦�ܿڶ���"*/
#define MULTI_ACT_OFF   1  /*"�๦�ܿڽ�������"*/

extern void AO_MultiTermn (INT8U Mode,INT8U Act);
/*"�̵������ֶ���"*/
typedef enum 
{
    E_RELAY_BREAK = 0x00,               /*"���ؼ̵���"*/
#ifdef THREE_PHASE
    E_RELAY_RELAYALARM,                /*"�����̵���"*/
#endif
    MAX_RELAY_NUM       
}EN_OC_RELAYID;

typedef enum 
{
    E_RELAY_MODE_INSIDE = 0x00,   /*"����"*/
    E_RELAY_MODE_LEVEL,           /*"���õ�ƽ"*/
    E_RELAY_MODE_PULSE,           /*"��������"*/
    RELAY_MODE_NUM           
}EN_OC_RELAYACTMODE;

typedef enum 
{
    RELAY_ACT_CLOSE = 0x00,      /*"��բ"*/
    RELAY_ACT_TRIP,                    /*"��բ"*/
    RELAY_ACT_STOP,                   /*"ֹͣ"*/
    RELAY_ACT_FRIEND                /*"����"*/ 
}EN_OC_RELAYACT;

extern void AO_Relay(INT8U Obj, INT8U Act, INT8U Mode);

#endif
