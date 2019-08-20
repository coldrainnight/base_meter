/*"***********************************************************		
FileName: AO.h		
Author: 关靓华       
Version : 1.0       Date: 2017.2.16	
Description: 输出控制驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#ifndef _AO_H_
#define _AO_H_

#include "system\Sys_def.h"

/*"蜂鸣器部分定义"*/
#define BUZ_ON   1    /*"蜂鸣器开启"*/
#define BUZ_OFF  0    /*"蜂鸣器关闭"*/

extern void AO_Buzzer(INT8U buzAct);
/*"LED部分定义"*/
typedef enum 
{
    E_LED_BLIGHT = 0x00,                 /*"背光灯"*/
    E_LED_RELAY_YELLOW,    /*"继电器指示灯"*/
    E_LED_INIT                    /*"初始化时全部LED点亮,只能在定时开启的接口调用"*/
}EN_OC_LEDID;

#define  LED_ON          1        /*"LED开启"*/
#define  LED_OFF         0        /*"LED关闭"*/
#define  LED_MAX_STATUS  2        /*"最大LED状态"*/
#define  LED_NUM    E_LED_INIT    /*"LED设备数目"*/

extern void AO_LED(INT8U Obj, INT8U Act);
/*"多功能口部分定义"*/
typedef enum 
{
    E_MULTI_SECOND = 0x00,     /*"秒脉冲"*/
    E_MULTI_DEMANDPERIOD,    /*"需量周期"*/
    E_MULTI_TRAFFSWITCH,      /*"时段投切"*/
    E_MULTI_REACTPULSE,	        /*无功脉冲*/	
    E_MULTI_MAX,	                      
}EN_OC_MultiTermnMode;

#define MULTI_ACT_ON    0  /*"多功能口动作"*/
#define MULTI_ACT_OFF   1  /*"多功能口结束动作"*/

extern void AO_MultiTermn (INT8U Mode,INT8U Act);
/*"继电器部分定义"*/
typedef enum 
{
    E_RELAY_BREAK = 0x00,               /*"阀控继电器"*/
#ifdef THREE_PHASE
    E_RELAY_RELAYALARM,                /*"报警继电器"*/
#endif
    MAX_RELAY_NUM       
}EN_OC_RELAYID;

typedef enum 
{
    E_RELAY_MODE_INSIDE = 0x00,   /*"内置"*/
    E_RELAY_MODE_LEVEL,           /*"外置电平"*/
    E_RELAY_MODE_PULSE,           /*"外置脉冲"*/
    RELAY_MODE_NUM           
}EN_OC_RELAYACTMODE;

typedef enum 
{
    RELAY_ACT_CLOSE = 0x00,      /*"合闸"*/
    RELAY_ACT_TRIP,                    /*"跳闸"*/
    RELAY_ACT_STOP,                   /*"停止"*/
    RELAY_ACT_FRIEND                /*"保电"*/ 
}EN_OC_RELAYACT;

extern void AO_Relay(INT8U Obj, INT8U Act, INT8U Mode);

#endif
