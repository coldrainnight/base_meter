/*"**********************************************************"*/		
/*"FileName: Tmr.c"*/		
/*"Author: "*/      
/*"Version : 1.0       Date: 2017.01.05	"*/
/*"Description: ��ʱ����ϵͳ��"*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"**********************************************************"*/
#include "Driver\MCU\Tmr.h"
#include "Driver\MCU\MCU.h"


/****************����ӿ�**********************/
void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr);
void TMR_Enable(INT8U TmrId);
void TMR_Disable(INT8U TmrId);

/***************�ڲ�����**********************/

/*********************************************/

void TMR_Init(INT8U TmrId,  ST_TMRCONFIG * TmrConfigStr)
{
    if(E_TMR_ID_SYS_TICK==TmrId)
    {
        MCU_PeriphClkSet(ET1_num,ENABLE);
        ETIMER1->ETxCR = 0x0000;/*16bit ������ģʽ ֹͣ����*/
        ETIMER1->ETxINSEL = 0x0000; /*�ڲ��ź�1ѡ��group1 APB CLK(8M)*/
        ETIMER1->ETxPESCALE1 = 249;/*Ԥ��Ƶ 250,��Ƶ��Ϊ8M/250*/
        ETIMER1->ETxPESCALE2 = 0;/*��Ԥ��Ƶ*/

        ETIMER1->ETxIVR = 0xFFFF-(((APBClock/250)/1000)*(TmrConfigStr->Count));
        ETIMER1->ETxCMP = 0x0000;
        ETIMER1->ETxIE =  0x0000;/*����ж�ʹ��*/
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
        ETIMER1->ETxIE =  0x0001;/*����ж�ʹ��*/
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



