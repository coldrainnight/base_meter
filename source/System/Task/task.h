#ifndef   _TASK_H
#define  _TASK_H
#include "System\sys_def.h"
#define TASK_NO_USED 0xff  /*"未使用状态"*/

#define TASK_STATUS_IDLE	0   /*"未使用状态"*/
#define TASK_STATUS_ACTIVE	1   /*"激活状态"*/
#define TASK_STATUS_HOLD 	2   /*"挂起状态"*/

#define TASK_SECOND	0   /*"秒任务处理状态"*/
#define TASK_MINUTE 	1   /*"分钟任务处理状态"*/


/*"主任务定义"*/
typedef enum 
{
    MAINTASK_SC = 0x00,   /*"系统时钟"*/
    MAINTASK_MU,   /*"计量测量"*/
    MAINTASK_EU,   /*"电量计量"*/    
    MAINTASK_DC,   /*"数据中心"*/
    MAINTASK_TS,   /*"分时梯度"*/
    MAINTASK_DU,   /*"需量计量"*/
    MAINTASK_PM,   /*"付费模块"*/
    MAINTASK_EM,   /*"事件模块"*/
    MAINTASK_CM,   /*"通信模块"*/
    MAINTASK_DM,   /*"按键显示"*/
    MAINTASK_UI,   /*"用户接口类其它功能模块事件上报"*/
    MAINTASK_STM,   /*"自检模块"*/
    MAINTASK_UM,   /*"升级模块"*/
    MAINTASK_RU,   /*"温补模块"*/
    MAINTASK_MNT,  /*"状态检查模块"*/
    E_MAX_MAINTASK_ID
}EN_MAINTASK_ID;

/*"定时任务定义"*/
typedef enum 
{
    TIMERTASK_ST = 0x00,  /*"软时钟计算"*/
    TIMERTASK_TICK,   /*"时基"*/
    TIMERTASK_STM,   /*"状态检测"*/
    TIMERTASK_OC,    /*"输出控制"*/
    TIMERTASK_KEY,   /*"按键防抖"*/
    TIMERTASK_PM,        /*"预付费控制"*/
    TIMERTASK_PLUSE,     /*"脉冲防抖"*/
    TIMERTASK_CM,   /*"通信模块"*/
    TIMERTASK_DM,   /*"显示按键模块"*/
    TIMERTASK_HOST_COMM, /*" 双芯通信 "*/
    E_MAX_TIMERTASK_ID
}EN_TIMERTASK_ID;


/*"分时任务定义"*/
typedef enum 
{
    E_TASK_SECOND = 0x00,  /*"秒任务标志"*/
    E_TASK_MINUTE,   /*"分钟任务标志"*/
    E_MAX_TIMETASK_ID
}EN_TIMETASK_ID;

typedef struct
{
    INT8U taskID;   /*"任务编号"*/
    INT8U status;           /*"状态"*/
    void (*taskMain)(void);/*"发送结束后回调函数"*/
}ST_TASK_NODE;


/*"本模块函数声明"*/
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

