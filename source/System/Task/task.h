#ifndef   _TASK_H
#define  _TASK_H
#include "System\sys_def.h"
#define TASK_NO_USED 0xff  /*"δʹ��״̬"*/

#define TASK_STATUS_IDLE	0   /*"δʹ��״̬"*/
#define TASK_STATUS_ACTIVE	1   /*"����״̬"*/
#define TASK_STATUS_HOLD 	2   /*"����״̬"*/

#define TASK_SECOND	0   /*"��������״̬"*/
#define TASK_MINUTE 	1   /*"����������״̬"*/


/*"��������"*/
typedef enum 
{
    MAINTASK_SC = 0x00,   /*"ϵͳʱ��"*/
    MAINTASK_MU,   /*"��������"*/
    MAINTASK_EU,   /*"��������"*/    
    MAINTASK_DC,   /*"��������"*/
    MAINTASK_TS,   /*"��ʱ�ݶ�"*/
    MAINTASK_DU,   /*"��������"*/
    MAINTASK_PM,   /*"����ģ��"*/
    MAINTASK_EM,   /*"�¼�ģ��"*/
    MAINTASK_CM,   /*"ͨ��ģ��"*/
    MAINTASK_DM,   /*"������ʾ"*/
    MAINTASK_UI,   /*"�û��ӿ�����������ģ���¼��ϱ�"*/
    MAINTASK_STM,   /*"�Լ�ģ��"*/
    MAINTASK_UM,   /*"����ģ��"*/
    MAINTASK_RU,   /*"�²�ģ��"*/
    MAINTASK_MNT,  /*"״̬���ģ��"*/
    E_MAX_MAINTASK_ID
}EN_MAINTASK_ID;

/*"��ʱ������"*/
typedef enum 
{
    TIMERTASK_ST = 0x00,  /*"��ʱ�Ӽ���"*/
    TIMERTASK_TICK,   /*"ʱ��"*/
    TIMERTASK_STM,   /*"״̬���"*/
    TIMERTASK_OC,    /*"�������"*/
    TIMERTASK_KEY,   /*"��������"*/
    TIMERTASK_PM,        /*"Ԥ���ѿ���"*/
    TIMERTASK_PLUSE,     /*"�������"*/
    TIMERTASK_CM,   /*"ͨ��ģ��"*/
    TIMERTASK_DM,   /*"��ʾ����ģ��"*/
    TIMERTASK_HOST_COMM, /*" ˫оͨ�� "*/
    E_MAX_TIMERTASK_ID
}EN_TIMERTASK_ID;


/*"��ʱ������"*/
typedef enum 
{
    E_TASK_SECOND = 0x00,  /*"�������־"*/
    E_TASK_MINUTE,   /*"���������־"*/
    E_MAX_TIMETASK_ID
}EN_TIMETASK_ID;

typedef struct
{
    INT8U taskID;   /*"������"*/
    INT8U status;           /*"״̬"*/
    void (*taskMain)(void);/*"���ͽ�����ص�����"*/
}ST_TASK_NODE;


/*"��ģ�麯������"*/
extern void Task_Init(void);
extern void Task_Task (void);
extern void Task_MainTaskListRun (void);
extern void Task_TimerTaskListRun (void);
extern void Task_MainTaskRegister (INT8U MainTaskID, void (*MainTask)(void));
extern void Task_MainTaskHoldOn (INT8U MainTaskID);
extern void Task_MainTaskActivate (INT8U MainTaskID);
extern void Task_TimerTaskRegister (INT8U TimerTaskID, void (*TimerTask)(void));
extern INT8U Task_ChkTsk_1s (INT8U MainTaskID);
extern INT8U Task_ChkTsk_1min (INT8U MainTaskID);
#endif

