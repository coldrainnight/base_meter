#ifndef TMR_H
#define TMR_H
#include "system\Sys_def.h"

enum TMRID
{
    E_TMR_ID_SYS_TICK =0,  /*上电运行系统时钟滴答（时基）*/
    E_TMR_ID_SYS_TICK_PD,  /*低功耗运行系统时钟滴答（时基）*/
    E_TMR_ID_PWM_4K,     /*蜂鸣器输出4K信号*/
    E_TMR_ID_A_COUNT,     /*有功脉冲计数（分频）*/
    E_TMR_ID_R_COUNT,     /*无功脉冲计数（分频）*/
    E_TMR_ID_A_PULSE_W,   /*有功脉冲宽度定时*/
    E_TMR_ID_R_PULSE_W,   /*无功脉冲宽度定时*/
    E_TMR_ID_MULTI_PULSE_W, /*多功能脉冲宽度定时*/
    E_TMR_ID_ESAM_CLK, /* ESAM时钟源*/
    E_TMR_ID_REMOTE_IR, /*红外遥控脉宽测量*/
    E_TMR_ID_PD_WAKEUP_COUNT, /*低功耗红外唤醒信号脉冲计数*/
    E_TMR_ID_PD_WAKEUP_TIME, /*低功耗红外唤醒定时（固定时长的脉冲数）*/
};

#define TMR_TIMING  (0) /*定时模式*/
#define TMR_COUNTER (1) /*计数模式*/
#define TMR_PWM     (2) /*PWM模式*/

typedef struct {
    INT8U  Mode;     /* TIMING	   定时模式
                        COUNTER	计数模式
                       PWM模式  */
    INT8U  TMR_Id;            /*"定时器中断资源号"*/
    
    INT16U Count;    /*定时模式下，定时长度值，单位ms，如5，表示定时5ms
                      计数模式下，计数值，如10，表示计数到10产生中断
                    PWM模式下，表示占空比，取值 0~100，单位%*/   
}ST_TMRCONFIG;




extern void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr);

extern void TMR_Enable(INT8U TmrId);

extern void TMR_Disable(INT8U TmrId);



#endif
