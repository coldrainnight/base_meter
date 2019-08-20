/*"***********************************************************"*/		
/*"文件名: task.c	"*/	
/*"作者: 扶忠权     "*/ 
/*"版本: 1.0       日期: 2017.01.13"*/	
/*"综合国内、海外表计的需求重新设计任务调度模块"*/	
/*"文件描述: 实现主任务,时基中断任务调度服务，时间片分配"*/
/*"函数列表: "*/		
/*"<函数名>	"*/	
/*"***********************************************************"*/		
#include "System\sys_def.h"
//#include "System\Sys_macro.h"
#include "Lib\LIB_func.h"
#include "System\Task\task.h"
#include "Driver\MCU\MCU.h"
#include "Driver\HAL\ST\ST.h"

ST_TASK_NODE mainTaskList[E_MAX_MAINTASK_ID];/*"主任务列表"*/
ST_TASK_NODE timerTaskList[E_MAX_TIMERTASK_ID];/*"时基定时器中断任务列表"*/

//static ST_TIME_SLICE_FLG mTimeSliceFlg;/*"时间片标志"*/
static UN_DWORD_DEF  mTskTimeFlg[E_MAX_TIMETASK_ID];/*秒/分钟任务标志"*/
static INT32U mTimeCal;/*分时任务分配计算量"*/
static INT8U mCycleFlg;/*分时任务分配循环一周标志"*/

void Task_GenerateTimeFlg (INT8U taskType);
INT8U Task_ChkTimeFlg (INT8U MainTaskID,INT8U taskType);
/*"*****************************************************************************"*/
/*"  Function:       Task_Init"*/
/*"  Description:   模块相关变量初始化"*/
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
/*"列表初始化"*/	
    LIB_MemSet(0,(INT8U *)&mainTaskList[0],sizeof(ST_TASK_NODE));
    LIB_MemSet(0,(INT8U *)&timerTaskList[0],sizeof(ST_TASK_NODE));
    mTimeSliceFlg.byte=0;
    mTskTimeFlg[E_TASK_SECOND].dword=0;
    mTskTimeFlg[E_TASK_MINUTE].dword=0;
    mTimeCal=0x01;
    mCycleFlg=TASK_SECOND;
    /*"特殊数据初始化"*/	
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
/*"  Description:   模块主任务，用于分时任务标识生成，模块主任务调度函数执行"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/
/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/

void Task_Task (void)
{  
    if((mTimeSliceFlg.bit.flg_1s>0)&&(mCycleFlg==TASK_SECOND))/*"秒任务到"*/	
    {
        Task_GenerateTimeFlg(E_TASK_SECOND);
    }
    else
    {
        if((mTimeSliceFlg.bit.flg_1min==1)&&(mCycleFlg==TASK_MINUTE))/*"分钟任务到"*/	
        {
            Task_GenerateTimeFlg(E_TASK_MINUTE);
            if(mTimeCal==0)/*"防错设计，防止被异常清0导致分时任务分配失效"*/	
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
/*"产生分时任务标志"*/	
void Task_GenerateTimeFlg (INT8U taskType)
{

        mTskTimeFlg[taskType].dword |=mTimeCal;
        mTimeCal=mTimeCal<<1;
        if(mTimeCal>=((INT32U)0x01<<E_MAX_MAINTASK_ID))/*"达到最大值"*/	
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
/*"  Description:   模块主任务调度"*/
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
/*"  Description:   定时中断任务调度"*/
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
/*"  Description:   主任务注册"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:MainTaskID,主任务id；MainTask，主任务          "*/
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
/*"  Description:   某任务挂起"*/
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
/*"  Description:   某任务激活"*/
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
/*"  Description:   定时任务注册"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:MainTaskID,定时任务id；TimerTask，定时任务          "*/
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
/*"  Description:    检测功能模块秒任务状态"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块主接口函数"*/
/*"  Input:          MainTaskID  功能模块编号"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE 每秒分时任务到   _FALSE  每秒分时任务未到"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Task_ChkTsk_1s (INT8U MainTaskID)
{
    return Task_ChkTimeFlg(MainTaskID,E_TASK_SECOND);
}
/*"*****************************************************************************"*/
/*"  Function:       Task_ChkTsk_1min"*/
/*"  Description:    检测功能模块分钟任务状态"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块主接口函数"*/
/*"  Input:          MainTaskID  功能模块编号"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE 每分分时任务到   FALSE  每分分时任务未到"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Task_ChkTsk_1min (INT8U MainTaskID)
{
    return Task_ChkTimeFlg(MainTaskID,E_TASK_MINUTE);
}
/*"分时任务标志检测"*/	
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

    if(0 != (mTskTimeFlg[taskType].byte[shift] & mask))/*"对应模块的分时任务标志位为1"*/
    {
        ret = TRUE;
        mTskTimeFlg[taskType].byte[shift] &= (~mask);/*"清对应模块的分时任务标志位"*/
    }
    return ret;
}
