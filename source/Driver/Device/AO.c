/*"***********************************************************		
FileName: AO.c		
Author: 关靓华       
Version : 1.0       Date: 2017.2.16	
Description: 输出控制驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 
#include "Driver\MCU\IIC.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\device\AO.h"
#include "Driver\MCU\Tmr.h"
#include "Driver\MCU\RTC.h"

/*"************************************************			
Function: 	AO_LED		
Description: LED动作控制
Input:  
Obj : LED对象名
      E_LED_ALM    报警灯
      E_LED_RELAY  继电器指示灯
      E_LED_BLIGHT 背光灯
      E_LED_COMM   通信指示灯

Act :  LED动作
       LED_ON      1    LED开启 
       LED_OFF     0    LED关闭

Output: 无	
Return: 无 
Others: 		
************************************************"*/
void AO_LED(INT8U Obj, INT8U Act)
{
  switch(Obj)
  {
  	case E_LED_RELAY_YELLOW:  	 
  	 if(Act == LED_ON)
  	 {
  	 	GPIO_OutputPin(LED_RELAY_LED, LOW);
  	 }
  	 else
  	 {
  	 	GPIO_OutputPin(LED_RELAY_LED, HIGH);
  	 }
	 GPIO_ConfigPin(LED_RELAY_LED, OUTPUT, NORMAL_OUT);
  	break;
  	
  	case E_LED_BLIGHT:
  	 if(Act == LED_ON)
  	 {
#ifdef THREE_PHASE
  	 	GPIO_OutputPin(BACK_LIGHT_CTRL, HIGH);
#else
              GPIO_OutputPin(BACK_LIGHT_CTRL, LOW);
#endif
  	 }
  	 else
  	 {
#ifdef THREE_PHASE
  	 	GPIO_OutputPin(BACK_LIGHT_CTRL, LOW);
#else
              GPIO_OutputPin(BACK_LIGHT_CTRL, HIGH);
#endif
  	 }
	  GPIO_ConfigPin(BACK_LIGHT_CTRL, OUTPUT, NORMAL_OUT);
  	break;
  	
  	default:
  	break;
  }
}

/*****************************************************************************
 函 数 名  : AO_BuzOn
 功能描述  : 蜂鸣器开启
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 04 05
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
void AO_BuzOn(void)
{
 GPIO_ConfigPin(BEEP, OUTPUT, NORMAL_OUT);
 GPIO_OutputPin(BEEP, HIGH);
}    

/*****************************************************************************
 函 数 名  : AO_BuzOff
 功能描述  : 蜂鸣器关闭
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 04 05
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
void AO_BuzOff(void)
{
 GPIO_ConfigPin(BEEP, OUTPUT, NORMAL_OUT);
 GPIO_OutputPin(BEEP, LOW);
}    

/*****************************************************************************
 函 数 名  : AO_Buzzer
 功能描述  : 蜂鸣器动作控制函数
 输入参数  : INT8U  buzAct  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 04 05
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
void AO_Buzzer(INT8U buzAct)
{
    if(BUZ_ON == buzAct)
    {
        AO_BuzOn();
    }
    else
    {
        AO_BuzOff();
    }    

}

/*"************************************************			
Function: 	AO_Relay		
Description: LED动作控制
Input:  
        Obj  目标:继电器对象名称 
            E_RELAY_ALM  报警继电器
            E_ RELAY _BREAK  阀控继电器

        Act  继电器动作     
            RELAY_ACT_CLOSE  合闸
            RELAY_ACT_TRIP   跳闸

        Mode 继电器控制模式
            E_RELAY_MODE_INSIDE  内置
            E_ RELAY_MODE_LEVEL  外置电平
            E_ RELAY_MODE_PULSE  外置脉冲

Output: 无	
Return: 无 
Others: 		
************************************************"*/	
void AO_Relay(INT8U Obj, INT8U Act, INT8U Mode)
{
#ifdef THREE_PHASE
    if(Obj == E_RELAY_RELAYALARM)
    {
        if(E_RELAY_MODE_LEVEL == Mode)  /*"外置电平"*/
       {
           GPIO_ConfigPin(RLY_ALARM, OUTPUT, NORMAL_OUT);
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"合闸"*/
           {
             GPIO_OutputPin(RLY_ALARM, LOW);
           }
           else if(RELAY_ACT_TRIP == Act)                                  /*"拉闸"*/
           {
             GPIO_OutputPin(RLY_ALARM, HIGH);
           }
           else 
           {
      
           }
       }
       else 
       {
           
       }
    }
    else if(Obj == E_RELAY_BREAK)
#endif
    {
       /*"内置继电器"*/
       if(E_RELAY_MODE_INSIDE == Mode)
       {
           GPIO_ConfigPin(TRIP_RELAY_ON, OUTPUT, NORMAL_OUT);
           GPIO_ConfigPin(TRIP_RELAY_OFF, OUTPUT, NORMAL_OUT);
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"合闸"*/
           {
             GPIO_OutputPin(TRIP_RELAY_ON, HIGH );
             GPIO_OutputPin(TRIP_RELAY_OFF,LOW);
           }
           else if(RELAY_ACT_TRIP == Act)               /*"拉闸"*/
           {              
             GPIO_OutputPin(TRIP_RELAY_ON, LOW);
             GPIO_OutputPin(TRIP_RELAY_OFF, HIGH);
           }
           else 
           {
               GPIO_OutputPin(TRIP_RELAY_ON, LOW);
               GPIO_OutputPin(TRIP_RELAY_OFF, LOW);
           }
       }
       else if((E_RELAY_MODE_LEVEL == Mode)||(E_RELAY_MODE_PULSE == Mode))  /*"外置电平"*/
       {
#ifdef THREE_PHASE
           GPIO_ConfigPin(TRIP_RELAY_OFF, OUTPUT, NORMAL_OUT);
#else
           GPIO_ConfigPin(TRIP_RELAY_ON, OUTPUT, NORMAL_OUT);
#endif
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"合闸"*/
           {
#ifdef THREE_PHASE
             GPIO_OutputPin(TRIP_RELAY_OFF, LOW);
#else
             GPIO_OutputPin(TRIP_RELAY_ON, LOW);
#endif
           }
           else if(RELAY_ACT_TRIP == Act)                                  /*"拉闸"*/
           {
#ifdef THREE_PHASE
             GPIO_OutputPin(TRIP_RELAY_OFF, HIGH);
#else
             GPIO_OutputPin(TRIP_RELAY_ON, HIGH);
#endif
           }
           else 
           {
             
           }
       }
       else 
       {
           
       }
    }
}

/*"************************************************			
Function: 	AO_MultiTermn		
Description: 多功能口动作控制
Input:  
Mode ： 多功能口模式
        E_MULTI_SECOND ,         秒脉冲
        E_MULTI _TRAFFSWITCH,  时段投切
        E_MULTI _DEMAND  需量

Act  多功能口动作     
        MULTI_ACT_ON   多功能口动作
        MULTI_ACT_OFF  多功能口结束动作


Output: 无	
Return: 无 
Others: 		
************************************************"*/
void AO_MultiTermn (INT8U Mode,INT8U Act)
{
#ifdef THREE_PHASE
    GPIO_ConfigPin(MULT_PULSE, OUTPUT, NORMAL_OUT);
    GPIO_OutputPin(MULT_PULSE, LOW);
#endif
    if(E_MULTI_SECOND == Mode)
    {
      if(Act==MULTI_ACT_ON)
      {
      	RTC_OpenSec();
      }
      else
      {
      	RTC_CloseSec();
      }
    }
    else
    {
       
    }    
}

