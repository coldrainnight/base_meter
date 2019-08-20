/*"***********************************************************"*/		
/*"�ļ���: task.c	"*/	
/*"����: ����Ȩ     "*/ 
/*"�汾: 1.0       ����: 2017.01.13"*/	
/*"�ۺϹ��ڡ������Ƶ�������������������ģ��"*/	
/*"�ļ�����: ʵ��������,ʱ���ж�������ȷ���ʱ��Ƭ����"*/
/*"�����б�: "*/		
/*"<������>	"*/	
/*"***********************************************************"*/		
#include "System\sys_def.h"
//#include "System\Sys_macro.h"
#include "Lib\LIB_func.h"
#include "System\Task\task.h"
#include "Driver\MCU\MCU.h"
#include "Driver\HAL\ST\ST.h"

ST_TASK_NODE mainTaskList[E_MAX_MAINTASK_ID];/*"�������б�"*/
ST_TASK_NODE timerTaskList[E_MAX_TIMERTASK_ID];/*"ʱ����ʱ���ж������б�"*/

//static ST_TIME_SLICE_FLG mTimeSliceFlg;/*"ʱ��Ƭ��־"*/
static UN_DWORD_DEF  mTskTimeFlg[E_MAX_TIMETASK_ID];/*��/���������־"*/
static INT32U mTimeCal;/*��ʱ������������"*/
static INT8U mCycleFlg;/*��ʱ�������ѭ��һ�ܱ�־"*/

void Task_GenerateTimeFlg (INT8U taskType);
INT8U Task_ChkTimeFlg (INT8U MainTaskID,INT8U taskType);
/*"*****************************************************************************"*/
/*"  Function:       Task_Init"*/
/*"  Description:   ģ����ر�����ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_Init(void)
{
    INT8U uni;
/*"�б��ʼ��"*/	
    LIB_MemSet(0,(INT8U *)&mainTaskList[0],sizeof(ST_TASK_NODE));
    LIB_MemSet(0,(INT8U *)&timerTaskList[0],sizeof(ST_TASK_NODE));
    mTimeSliceFlg.byte=0;
    mTskTimeFlg[E_TASK_SECOND].dword=0;
    mTskTimeFlg[E_TASK_MINUTE].dword=0;
    mTimeCal=0x01;
    mCycleFlg=TASK_SECOND;
    /*"�������ݳ�ʼ��"*/	
    for(uni=0;uni<E_MAX_MAINTASK_ID;uni++)
    {
        mainTaskList[uni].taskID=TASK_NO_USED;
    }
    for(uni=0;uni<E_MAX_TIMERTASK_ID;uni++)
    {
        timerTaskList[uni].taskID=TASK_NO_USED;
    } 
}
/*"*****************************************************************************"*/
/*"  Function:       Task_Task"*/
/*"  Description:   ģ�����������ڷ�ʱ�����ʶ���ɣ�ģ����������Ⱥ���ִ��"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/

void Task_Task (void)
{  
    if((mTimeSliceFlg.bit.flg_1s>0)&&(mCycleFlg==TASK_SECOND))/*"������"*/	
    {
        Task_GenerateTimeFlg(E_TASK_SECOND);
    }
    else
    {
        if((mTimeSliceFlg.bit.flg_1min==1)&&(mCycleFlg==TASK_MINUTE))/*"��������"*/	
        {
            Task_GenerateTimeFlg(E_TASK_MINUTE);
            if(mTimeCal==0)/*"������ƣ���ֹ���쳣��0���·�ʱ�������ʧЧ"*/	
            {
                mTimeCal = 0x01;
            }
        }
        else
        {
            mCycleFlg=TASK_SECOND;
        }
    }
    Task_MainTaskListRun();
}
/*"������ʱ�����־"*/	
void Task_GenerateTimeFlg (INT8U taskType)
{

        mTskTimeFlg[taskType].dword |=mTimeCal;
        mTimeCal=mTimeCal<<1;
        if(mTimeCal>=((INT32U)0x01<<E_MAX_MAINTASK_ID))/*"�ﵽ���ֵ"*/	
        {
            if(taskType==E_TASK_MINUTE)
            {
                mTimeSliceFlg.bit.flg_1min=0;
                mCycleFlg=TASK_SECOND;
            }
            else
            {
                mTimeSliceFlg.bit.flg_1s -= 1;
                mCycleFlg=TASK_MINUTE;
            }
            mTimeCal=0x01; 
        }
        else
        {
        }
}
/*"*****************************************************************************"*/
/*"  Function:       Task_MainTaskListRun"*/
/*"  Description:   ģ�����������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_MainTaskListRun (void)
{
    INT8U uni;
    for(uni=0;uni<E_MAX_MAINTASK_ID;uni++)
    {
        if((mainTaskList[uni].status!=TASK_STATUS_IDLE)&&(mainTaskList[uni].status!=TASK_STATUS_HOLD))
        {
            if(mainTaskList[uni].taskMain != NULL)
            {
                mainTaskList[uni].taskMain();
            }
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       Task_TimerTaskListRun"*/
/*"  Description:   ��ʱ�ж��������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_TimerTaskListRun (void)
{
    INT8U uni;
    for(uni=0;uni<E_MAX_TIMERTASK_ID;uni++)
    {
        if((timerTaskList[uni].status!=TASK_STATUS_IDLE)&&(timerTaskList[uni].status!=TASK_STATUS_HOLD))
        {
            if(timerTaskList[uni].taskMain != NULL)
            {
                timerTaskList[uni].taskMain();
            }
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       Task_MainTaskRegister"*/
/*"  Description:   ������ע��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:MainTaskID,������id��MainTask��������          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_MainTaskRegister (INT8U MainTaskID, void (*MainTask)(void))
{
    if(MainTaskID>=E_MAX_MAINTASK_ID) 
    {
        return;
    }
    mainTaskList[MainTaskID].taskMain=MainTask;
    mainTaskList[MainTaskID].taskID=MainTaskID;
    mainTaskList[MainTaskID].status=TASK_STATUS_ACTIVE;
}
/*"*****************************************************************************"*/
/*"  Function:       Task_MainTaskHoldOn"*/
/*"  Description:   ĳ�������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_MainTaskHoldOn (INT8U MainTaskID)
{
    if(MainTaskID>=E_MAX_MAINTASK_ID) 
    {
        return;
    }
    mainTaskList[MainTaskID].status=TASK_STATUS_HOLD;
}
/*"*****************************************************************************"*/
/*"  Function:       Task_MainTaskActivate"*/
/*"  Description:   ĳ���񼤻�"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_MainTaskActivate (INT8U MainTaskID)
{
    if(MainTaskID>=E_MAX_MAINTASK_ID) 
    {
        return;
    }
    mainTaskList[MainTaskID].status=TASK_STATUS_ACTIVE;
}
/*"*****************************************************************************"*/
/*"  Function:       Task_TimerTaskRegister"*/
/*"  Description:   ��ʱ����ע��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:MainTaskID,��ʱ����id��TimerTask����ʱ����          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void Task_TimerTaskRegister (INT8U TimerTaskID, void (*TimerTask)(void))
{
    if(TimerTaskID>=E_MAX_TIMERTASK_ID) 
    {
        return;
    }
    timerTaskList[TimerTaskID].taskMain=TimerTask;
    timerTaskList[TimerTaskID].taskID=TimerTaskID;
    timerTaskList[TimerTaskID].status=TASK_STATUS_ACTIVE;
}
/*"*****************************************************************************"*/
/*"  Function:       Task_ChkTsk_1s"*/
/*"  Description:    ��⹦��ģ��������״̬"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�����ӿں���"*/
/*"  Input:          MainTaskID  ����ģ����"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE ÿ���ʱ����   _FALSE  ÿ���ʱ����δ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Task_ChkTsk_1s (INT8U MainTaskID)
{
    return Task_ChkTimeFlg(MainTaskID,E_TASK_SECOND);
}
/*"*****************************************************************************"*/
/*"  Function:       Task_ChkTsk_1min"*/
/*"  Description:    ��⹦��ģ���������״̬"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�����ӿں���"*/
/*"  Input:          MainTaskID  ����ģ����"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE ÿ�ַ�ʱ����   FALSE  ÿ�ַ�ʱ����δ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Task_ChkTsk_1min (INT8U MainTaskID)
{
    return Task_ChkTimeFlg(MainTaskID,E_TASK_MINUTE);
}
/*"��ʱ�����־���"*/	
INT8U Task_ChkTimeFlg (INT8U MainTaskID,INT8U taskType)
{
    INT8U ret;
    INT8U shift;
    INT8U mask;
    INT8U bitNum;

    ret = FALSE;
    shift = (INT8U)(MainTaskID >> 3);
    bitNum = (INT8U)(MainTaskID & 0x07);
    mask = (INT8U)(0x01 << bitNum);

    if(0 != (mTskTimeFlg[taskType].byte[shift] & mask))/*"��Ӧģ��ķ�ʱ�����־λΪ1"*/
    {
        ret = TRUE;
        mTskTimeFlg[taskType].byte[shift] &= (~mask);/*"���Ӧģ��ķ�ʱ�����־λ"*/
    }
    return ret;
}
