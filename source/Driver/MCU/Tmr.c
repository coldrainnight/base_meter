/*"**********************************************************"*/		
/*"FileName: Tmr.c"*/		
/*"Author: "*/      
/*"Version : 1.0       Date: 2017.01.05	"*/
/*"Description: 定时器，系统层"*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"**********************************************************"*/
#include "Driver\MCU\Tmr.h"
#include "Driver\MCU\MCU.h"


/****************对外接口**********************/
void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr);
void TMR_Enable(INT8U TmrId);
void TMR_Disable(INT8U TmrId);

/***************内部函数**********************/

/*********************************************/

void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr)
{
    if(E_TMR_ID_SYS_TICK==TmrId)
    {
        MCU_PeriphClkSet(ET1_num,ENABLE);
        ETIMER1->ETxCR = 0x0000;/*16bit 计数器模式 停止计数*/
        ETIMER1->ETxINSEL = 0x0000; /*内部信号1选择group1 APB CLK(8M)*/
        ETIMER1->ETxPESCALE1 = 249;/*预分频 250,主频变为8M/250*/
        ETIMER1->ETxPESCALE2 = 0;/*不预分频*/

        ETIMER1->ETxIVR = 0xFFFF-(((APBClock/250)/1000)*(TmrConfigStr->Count));
        ETIMER1->ETxCMP = 0x0000;
        ETIMER1->ETxIE =  0x0000;/*溢出中断使能*/
        ETIMER1->ETxIF =  0x0000;
    }
    else
    {

    }

}


void TMR_Enable(INT8U TmrId)
{
    if(E_TMR_ID_SYS_TICK==TmrId)
    {
        ETIMER1->ETxIF =  0x0000;
        ETIMER1->ETxIE =  0x0001;/*溢出中断使能*/
        ETIMER1->ETxCR |= 0x80;
    }
    else
    {

    }

}

void TMR_Disable(INT8U TmrId)
{
    if(E_TMR_ID_SYS_TICK==TmrId)
    {
        ETIMER1->ETxCR &= ~0x80;
    }
    else
    {

    }
}



