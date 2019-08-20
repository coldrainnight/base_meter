#ifndef TMR_H
#define TMR_H
#include "system\Sys_def.h"

enum TMRID
{
    E_TMR_ID_SYS_TICK =0,  /*�ϵ�����ϵͳʱ�ӵδ�ʱ����*/
    E_TMR_ID_SYS_TICK_PD,  /*�͹�������ϵͳʱ�ӵδ�ʱ����*/
    E_TMR_ID_PWM_4K,     /*���������4K�ź�*/
    E_TMR_ID_A_COUNT,     /*�й������������Ƶ��*/
    E_TMR_ID_R_COUNT,     /*�޹������������Ƶ��*/
    E_TMR_ID_A_PULSE_W,   /*�й������ȶ�ʱ*/
    E_TMR_ID_R_PULSE_W,   /*�޹������ȶ�ʱ*/
    E_TMR_ID_MULTI_PULSE_W, /*�๦�������ȶ�ʱ*/
    E_TMR_ID_ESAM_CLK, /* ESAMʱ��Դ*/
    E_TMR_ID_REMOTE_IR, /*����ң���������*/
    E_TMR_ID_PD_WAKEUP_COUNT, /*�͹��ĺ��⻽���ź��������*/
    E_TMR_ID_PD_WAKEUP_TIME, /*�͹��ĺ��⻽�Ѷ�ʱ���̶�ʱ������������*/
};

#define TMR_TIMING  (0) /*��ʱģʽ*/
#define TMR_COUNTER (1) /*����ģʽ*/
#define TMR_PWM     (2) /*PWMģʽ*/

typedef struct {
    INT8U  Mode;     /* TIMING	   ��ʱģʽ
                        COUNTER	����ģʽ
                       PWMģʽ  */
    INT8U  TMR_Id;            /*"��ʱ���ж���Դ��"*/
    
    INT16U Count;    /*��ʱģʽ�£���ʱ����ֵ����λms����5����ʾ��ʱ5ms
                      ����ģʽ�£�����ֵ����10����ʾ������10�����ж�
                    PWMģʽ�£���ʾռ�ձȣ�ȡֵ 0~100����λ%*/   
}ST_TMRCONFIG;




extern void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr);

extern void TMR_Enable(INT8U TmrId);

extern void TMR_Disable(INT8U TmrId);



#endif
