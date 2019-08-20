/*"***********************************************************		
FileName: OC.h		
Author: 关靓华       
Version : 1.0       Date: 2017.2.16	
Description: 输出控制驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#ifndef _OC_H_
#define _OC_H_

#include "system\Sys_def.h"
#include "Driver\device\AO.h"

/*"输出控制的ID"*/
typedef enum 
{
    E_ID_BUZZER          = 0,      /*"蜂鸣器"*/    
    //E_ID_LED_BGD         ,          /*"背光灯"*/
    //E_ID_LED_RELAY     ,           /*"拉闸灯"*/
    E_ID_RELAY,                        /*"继电器"*/
    E_ID_MT ,                            /*"多功能输出口"*/
    E_ID_MAX_AO                     /*""*/
}OC_ID_EN;

/*"蜂鸣器"*/
typedef enum 
{
    E_BUZ_CLOSE = 0x00,               /*"关闭"*/
    E_BUZ_CARD_SUC ,                  /*"插卡成功"*/
    E_BUZ_CARD_ERR ,                  /*"插卡失败"*/
    OC_BUZ_MODE_NUM                /*"蜂鸣器模式个数"*/
}OC_BuzMode_EN;

typedef enum
{
    E_BUZ_SC_PREPAY = 0x00,        /*"报警源：预付费 "*/
    E_BUZ_MAX_SC                          /*"报警源个数"*/
}OC_BuzSource;

typedef struct
{   
    INT8U   BuzzTimes;	                  /*"循环次数"*/    
    INT8U   Repeat;                         /*"已经循环的次数"*/
    INT8U   Step;                             /*"运行到的步骤"*/
    INT8U   RunTime;                      /*"每一轮蜂鸣器鸣叫的时间"*/
    INT8U   PauseTime;                  /*"每一轮蜂鸣器停止的时间"*/
    INT8U	  Times;                          /*"每一轮时间"*/
}OC_BUZZ_STATUS;

/*"LED"*/
typedef enum 
{
    E_LED_CONT_CLOSE_MODE = 0,        /*"关闭模式 "*/
    E_LED_CONT_TWINKLE_MODE,          /*"闪烁模式"*/
    E_LED_CONT_ALWAYS_MODE,           /*"长亮模式"*/
    E_LED_CONT_MODE_NUM                  /*"模式个数"*/        
}LED_CONTINUE_MODE_EN;

typedef enum 
{
    E_LED_CLOSE_MODE = 0,        /*"关闭模式 "*/
    E_LED_ALWAYS_MODE,           /*"长亮模式"*/
    E_LED_TWINKLE_MODE,          /*"闪烁模式"*/    
    E_LED_MODE_NUM                  /*"模式个数"*/        
}LED_MODE_EN;

typedef enum
{
//    E_LED_RELAY=0,         /*"继电器led "*/
    E_ID_LED_BGD = 0,       /*"背光"*/
    E_ID_LED_RELAY = 1,           /*"拉闸灯"*/
    //E_ID_LED_POWER,                  /*"电源led "*/
    //E_ID_LED_ALARM,                   /*"报警led"*/
    //E_ID_LED_PREPAY,               /*"预付费报警led"*/
    E_ID_LED_NUM_SC,                            /*"个数"*/

    //E_ID_LED_RELAY,
    
     E_ID_LED_ALL = 0xff,     /*"所覩源的动作，G杼厥獯?*/
}LED_TYPR_EN;

typedef enum
{
    E_LED_SC_INIT = 0,                     /*"初始化 "*/
    //E_LED_SC_COMM,                   /*"通信 "*/
    //E_LED_SC_KEY,                      /*"按键"*/
    //E_LED_SC_REPAY,                  /*"预付费"*/
    //E_LED_SC_ALARM,                  /*"报警"*/
    E_LED_SC_EVENT,                  /*"事件"*/
    E_LED_SC_SNG,                      /*"单次"*/
    //E_LED_SC_REPAY_TWINKLE,                  /*"预付费闪烁"*/
    
    E_LED_SOURCE_SC,                 /*"个数"*/
    
    E_LED_SC_NONE = 0xff,                 /*"取消源，或者参蔵初始化状态"*/

}LED_SOURCE_EN;


typedef struct
{
    INT8U   ModeId;                        /*"当前模式"*/    
    INT16U ActTime;                       /*"剩余时间 单位s"*/
}OC_LED_STATUS;

typedef struct
{
    INT8U   LedCtrl[E_LED_SOURCE_SC];
}OC_LED_CTRL;


typedef enum
{
    E_LED_RELAY_SC_PREPAY=0,         /*"预付费 "*/
    E_LED_RELAY_SC_INIT,                  /*"初始化 "*/
    E_LED_RELAY_SC_SNG,                   /*"单次"*/
    E_LED_RELAY_SC                            /*"个数"*/
}LED_RELAY_SOURCE_EN;

typedef enum
{
    E_LED_BGT_SC_INIT,                     /*"初始化 "*/
    E_LED_BGT_SC_COMM,                   /*"通信 "*/
    E_LED_BGT_SC_KEY,                      /*"按键"*/
    E_LED_BGT_SC_REPAY,                  /*"预付费"*/
    E_LED_BGT_SC_ALARM,                  /*"报警"*/
    E_LED_BGT_SC_EVENT,                  /*"事件"*/
    E_LED_BGT_SC_SNG,                      /*"单次"*/
    E_LED_BGT_SC                               /*"个数"*/
}LED_BGD_SOURCE_EN;

/*"继电器"*/
typedef enum
{
    E_RELAY_SC_PREPAY=0,                /*"预付费"*/
#ifdef THREE_PHASE
    E_RELAY_SC_EVENT,                      /*"事件"*/  
#endif
    MAX_RELAY_SC_NUM                     /*"个数"*/
}ST_RELAY_SOURCE_EN;
typedef  struct
{
    EN_OC_RELAYACTMODE  Mode;     /*"继电器动作模式  内置、外置电平、外置脉冲"*/
    EN_OC_RELAYACT  cmdAct;   /*"继电器命令动作  拉合闸"*/
    INT8U  Flag;     /*"继电器当前是否在动作中 1为动作中 0为非动作中"*/
    INT16U  ActTimer; /*"继电器动作持续时间 单位：毫秒 "*/
    INT16U  PulseWidth; /*"继电器动作脉宽 单位：毫秒 "*/
    INT16U  ReActTimer; /*"继电器重新动作时间 单位 秒"*/
}ST_RELAY_ACT_STATUS;
#define _HOLD      0/*"继电器合闸"*/
#define _RELEASE   1/*"继电器跳闸"*/

/*"多功能输出"*/
typedef struct
{
    INT8U  Mode;            /*"当前多功能口模式"*/
    INT8U  ActDelayTimer;   /*"多功能口动作延时时间 单位：10ms"*/
    INT8U  ActTimer;        /*"多功能口动作时间 单位：10ms"*/
}ST_MULTI_STATUS;
/*"单位10ms"*/
#define MULTI_TARIFF_DELAY  1    /*"收到时段投切信号输出要求，延时10ms输出"*/
#define MULTI_TARIFF_TIME    8     /*"时段投切信号输出80ms"*/

#define MAX_LED_CMD  10
typedef struct
{
    INT8U intf; /*" 控制接口，0为OC_LedOff， 1为 OC_LedOnContin "*/
    OC_ID_EN LedID;
    LED_CONTINUE_MODE_EN ModeID;
    INT8U Source;
    INT32U Ms_Time32U;
}OC_LED_CMD;

/*"下面是外部要调用的函数"*/
extern void OC_Init(void);       /*"上电调用"*/
extern void OC_Int(void);        /*"10毫秒调用"*/
extern void OC_TaskSec(void);    /*"主循环调用"*/

/*"蜂鸣器控制"*/
extern void OC_BuzzerOff(OC_BuzMode_EN ModeID, OC_BuzSource Source);
extern void OC_BuzzerOnSingle (OC_BuzMode_EN ModeID);
extern void OC_BuzzerOnContin (OC_BuzMode_EN ModeID , OC_BuzSource Source, INT16U Ms_Time);

/*"LED控制"*/
extern void OC_LedOnSingle (LED_TYPR_EN  LedID,LED_MODE_EN ModeID,INT8U SecTime);
extern void OC_LedOnContin (LED_TYPR_EN LedID, LED_SOURCE_EN SourceID);
extern void OC_LedOff(LED_TYPR_EN LedID, LED_SOURCE_EN SourceID);

/*"继电器控制"*/
extern void OC_RelayAct (ST_RELAY_SOURCE_EN Source,EN_OC_RELAYID RelayID, EN_OC_RELAYACTMODE RelayActMode, EN_OC_RELAYACT Act,INT16U MsPulseTime);
extern void OC_RelayActReDo (void);
extern EN_OC_RELAYACT OC_GetRelayCtrlStatus(void);
extern void OC_SetRelayCtrlStatus(EN_OC_RELAYACT Act);
#ifdef THREE_PHASE
extern INT8U OC_GetAlmRelayCmd(void);
#endif

/*"多功能口控制"*/
extern void OC_MultiTermnSw(EN_OC_MultiTermnMode Mode);
extern INT8U OC_ReadMultiMode(void);
extern void OC_MultiTermn(EN_OC_MultiTermnMode Mode);
extern void OC_MultiTermn_Chng(void);

#endif  

