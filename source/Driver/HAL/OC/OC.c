/*"***********************************************************		
FileName: OC.c		
Author: 关靓华       
Version : 1.0       Date: 2017.2.16	
Description: 输出控制抽象层
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 
#include "Driver\MCU\IIC.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\device\AO.h"
#include "Driver\HAL\OC\OC.h"
#include "Lib\LIB_func.h"
#include "Driver\MCU\Tmr.h"
#include "App\message\MS.h"

volatile static INT8U     OCIntTime;
/*"Buzzer"*/
volatile static OC_BUZZ_STATUS mstBuzzer[E_BUZ_MAX_SC];
/*"LED"*/
typedef enum
{
    LEDDO_PERSEC=0,                /*"每秒调用"*/
    LEDDO_NOW                            /*"立即调用"*/
}LEDDO_KIND;
#define LED_OPEN   1
#define LED_CLOSE  0
volatile static OC_LED_CTRL mstLed[E_ID_LED_NUM_SC];/*"所有调用模式保存"*/
OC_LED_STATUS mstLedRun[E_ID_LED_NUM_SC];/*"当前运行的模式"*/
volatile static INT8U LedFlashFlag;
/*"将源和模式绑定，在增加新的源之前，需要在此表注册源使用的模式"*/
const  INT8U LedModeReg[E_LED_SOURCE_SC] =
{
    E_LED_ALWAYS_MODE,        /*"初始化"*/
    //E_LED_ALWAYS_MODE,        /*"通信 "*/
    //E_LED_ALWAYS_MODE,       /*"按键"*/
    //E_LED_ALWAYS_MODE,        /*"预付费"*/
    //E_LED_ALWAYS_MODE,        /*"报警"*/
    E_LED_ALWAYS_MODE,       /*"事件"*/
    E_LED_ALWAYS_MODE,        /*"单次"*/
    //E_LED_TWINKLE_MODE,       /*" 预付费闪烁 "*/
};


//volatile static OC_LED_STATUS mstLedRelay[E_LED_RELAY_SC];
//volatile static OC_LED_STATUS mstLedBgt[E_LED_BGT_SC];
void OC_LedAct(void);
void OC_LedInit(void);
void OC_LedProc(LED_TYPR_EN LedID);


/*"RELAY"*/
#define OC_RELAY_REACT_TIME    (151*60) /*"继电器正常状态再次动作时间(秒)"*/
volatile static ST_RELAY_ACT_STATUS mstRelayActStatus;
#ifdef THREE_PHASE
static INT8U alarmRelayActstatus[MAX_RELAY_SC_NUM];
#endif
/*"MT"*/
static ST_MULTI_STATUS mstMultiStatus;      /*"多功能合一状态变量"*/

void OC_BuzInit(void);
void OC_BuzzerProc(void);
void OC_LedInit(void);
void OC_RelayInit (void);
//void OC_LedProc(OC_ID_EN LedID, LEDDO_KIND DealKind);
void OC_RelayActPro (EN_OC_RELAYID RelayID);
void OC_RelayTaskSec(void);
void OC_MultiTermnProc(void);
/*"************************************************			
Function: 	OC_BuzInit		
Description:蜂鸣器初始化
Input:  无
Output: 无	
Return: 无 
Others: 		
************************************************"*/	 
void OC_BuzInit(void)
{
   AO_Buzzer(BUZ_OFF);

   LIB_MemSet(0, (INT8U *)&mstBuzzer[E_BUZ_SC_PREPAY], sizeof(OC_BUZZ_STATUS));
}
/*"************************************************			
Function: 	OC_BuzzerOnSingle		
Description:    开启蜂鸣器短鸣
Input:  ModeID：蜂鸣器报警模式名
                E_BUZ_SHORT_MODE1 报警短鸣模式1 
                E_BUZ_SHORT_MODE2 报警短鸣模式2 
Output: 无	
Return: 无 
Others: 		
************************************************"*/
void OC_BuzzerOnSingle (OC_BuzMode_EN ModeID)
{   
	LIB_MemSet(0, (INT8U *)&mstBuzzer[E_BUZ_SC_PREPAY], sizeof(OC_BUZZ_STATUS));
	
	if(ModeID == E_BUZ_CARD_SUC)
	{
		mstBuzzer[E_BUZ_SC_PREPAY].BuzzTimes=1;
		mstBuzzer[E_BUZ_SC_PREPAY].RunTime=6;
		mstBuzzer[E_BUZ_SC_PREPAY].PauseTime=1;
	}
	else if(ModeID == E_BUZ_CARD_ERR)
	{
		mstBuzzer[E_BUZ_SC_PREPAY].BuzzTimes=3;
		mstBuzzer[E_BUZ_SC_PREPAY].RunTime=1;
		mstBuzzer[E_BUZ_SC_PREPAY].PauseTime=1;
	}
	else
	{
	}
}
/*"************************************************			
Function: 	OC_BuzzerOnContin	----continue	
Description:    开启蜂鸣器长鸣
Input:  
ModeID：蜂鸣器报警模式
        E_BUZ_ CONT_MODE1  报警长鸣模式1， 优先级1
        E_BUZ_ CONT_MODE2  报警长鸣模式2,  优先级2 
        E_BUZ_ CONT_MODE3  报警长鸣模式3， 优先级3

Source: 触发蜂鸣器动作的源
        E_BUZ_ SC_ ERR     报警源：故障报警
        E_BUZ_ SC_ PREPAY  报警源：预付费
        
Ms_Time:ModeID=0时，鸣叫和停顿时间由这个参数控制。同灯的闪烁

Output: 无	
Return: 无 
Others: 		
************************************************"*/
void OC_BuzzerOnContin (OC_BuzMode_EN ModeID , OC_BuzSource Source, INT16U Ms_Time)
{
    
}

/*"************************************************			
Function: 	OC_BuzzerOff		
Description:   蜂鸣器报警关闭
Input:  
ModeID：蜂鸣器报警模式

Source: 触发蜂鸣器动作的源


Output: 无	
Return: 无 
Others: 		
************************************************"*/
void OC_BuzzerOff(OC_BuzMode_EN ModeID, OC_BuzSource Source)
{
   
}

/*"************************************************			
Function: 	OC_BuzzerProc		
Description:   蜂鸣器报警关闭
Input:  
ModeID：蜂鸣器报警模式

Source: 触发蜂鸣器动作的源


Output: 无	
Return: 无 
Others: 		
************************************************"*/
void OC_BuzzerProc(void)
{
    INT8U i;

    i=mstBuzzer[E_BUZ_SC_PREPAY].Repeat;
    if(i < mstBuzzer[E_BUZ_SC_PREPAY].BuzzTimes)
    {
	    mstBuzzer[E_BUZ_SC_PREPAY].Times+=1;
	    if(0 == mstBuzzer[E_BUZ_SC_PREPAY].Step)
	    {
	        AO_Buzzer(BUZ_ON);
		 i=mstBuzzer[E_BUZ_SC_PREPAY].Times;
	        if(i >= mstBuzzer[E_BUZ_SC_PREPAY].RunTime)
	  	 {	  	        
	  	        mstBuzzer[E_BUZ_SC_PREPAY].Times = 0;
	  	        mstBuzzer[E_BUZ_SC_PREPAY].Step = 1;
	  	 }
	    }  
	    else
	    {
	        AO_Buzzer(BUZ_OFF);
		 i=mstBuzzer[E_BUZ_SC_PREPAY].Times;
	        if(i >= mstBuzzer[E_BUZ_SC_PREPAY].PauseTime)
	  	 {   
	  	        mstBuzzer[E_BUZ_SC_PREPAY].Times = 0;
	  	        mstBuzzer[E_BUZ_SC_PREPAY].Step = 0;
	  	        mstBuzzer[E_BUZ_SC_PREPAY].Repeat ++;   	  	          	  	        	  	
	  	  }
	    }  	                 
    }    
}

/*****************************************************************************
函 数 名  : OC_LedInit
功能描述  : 单次触发LED灯，即定时亮一下或闪烁一下，闪几下
输入参数  : INT8U LedID: 报警灯、继电器灯、背光灯
              E_ID_LED_ALARM、  E_ID_LED_RELAY 、E_ID_LED_BGD
         INT8U ModeID: 单次亮的模式 
         INT8U SecTime: 单位秒     
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 星期三 2017 03 01
作    者   : e
修改内容   : 新生成函数

*****************************************************************************/
void OC_LedInit(void)
{
    LedFlashFlag = 0;
    AO_LED(E_LED_RELAY_YELLOW,LED_OFF);
    LIB_MemSet(0xff, (INT8U *)&mstLed, sizeof(mstLed));
    LIB_MemSet(0xff, (INT8U *)&mstLedRun, sizeof(mstLedRun));
    OC_LedProc(E_ID_LED_ALL);

}

/*****************************************************************************
 函 数 名  : OC_LedOnSingle
 功能描述  : 单次触发LED灯，即定时亮一下或闪烁一下，闪几下
 输入参数  : INT8U LedID: 报警灯、继电器灯、背光灯
                  E_ID_LED_ALARM、  E_ID_LED_RELAY 、E_ID_LED_BGD
             INT8U ModeID: 单次亮的模式 
             INT8U SecTime: 单位秒     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 03 01
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
void OC_LedOnSingle (LED_TYPR_EN  LedID,LED_MODE_EN ModeID,INT8U SecTime)
{

    INT8U i;
    if((LedID < E_ID_LED_NUM_SC )||(LedID == E_ID_LED_ALL))
    {
        if(E_ID_LED_ALL == LedID)
        {
            for(i = 0;i<E_ID_LED_NUM_SC;i++)
            {
                mstLedRun[i].ModeId = ModeID;
                mstLedRun[i].ActTime = SecTime;
            }
        }
        else
        {
            if(0 == SecTime)
            {
                mstLedRun[LedID].ModeId = E_LED_CLOSE_MODE;
            }
            else
            {
               mstLedRun[LedID].ModeId = ModeID;
            }
                          mstLedRun[LedID].ActTime = SecTime;
        }
        OC_LedAct();
    }
   
}

/*"***************************************************************************
 函 数 名  : OC_LedOnContin
 功能描述  : 持续亮或闪烁控制
 输入参数  : INT8U LedID :报警灯、继电器灯、背光灯    
             INT8U ModeID:常亮模式   
             INT16U Source:源，以便在关闭时，清除  
             INT16U Ms_Time:ModeID=0时，闪烁时间由这个参数控制。
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 03 01
    作    者   : e
    修改内容   : 新生成函数

****************************************************************************"*/
void OC_LedOnContin (LED_TYPR_EN LedID, LED_SOURCE_EN SourceID)
{

    INT8U i,j,LedNum;
    
   if((LedID < E_ID_LED_NUM_SC )||(LedID == E_ID_LED_ALL))
    {
        if(E_ID_LED_ALL == LedID)
        {
            LedNum = E_ID_LED_NUM_SC;
            j = 0;
        }
        else
        {
            LedNum = LedID+1;
            j = LedID;
        }

         for(;j < LedNum;j++)
        {
                for(i = 0;i< E_LED_SOURCE_SC;i++)
                {
                    /*"找到未使用的缓存"*/
                    if(mstLed[j].LedCtrl[i] == E_LED_SC_NONE)
                    {
                        mstLed[j].LedCtrl[i] = SourceID;
                        OC_LedProc((LED_TYPR_EN)j);
                        break;
                    }
                     /*"找到同一个源已注册使用的缓存"*/
                    else if(mstLed[j].LedCtrl[i] == SourceID)
                    {
                        break;
                    }
                }
        }
    }

}

/*"****************************************************************************
 函 数 名  : OC_LedOff
 功能描述  : 持续亮（或闪烁）的源，关闭
 输入参数  : INT8U LedID    
             INT8U ModeID   
             INT16U Source  ,如果大于16，则则表示关闭所有源，建议输入0xffff
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 03 01
    作    者   : e
    修改内容   : 新生成函数

****************************************************************************"*/
void OC_LedOff(LED_TYPR_EN LedID, LED_SOURCE_EN SourceID)
{

    INT8U i,j,LedNum;

   if((LedID < E_ID_LED_NUM_SC )||(LedID == E_ID_LED_ALL))
    {
        if(E_ID_LED_ALL == LedID)
        {
            LedNum = E_ID_LED_NUM_SC;
            j = 0;
        }
        else
        {
            LedNum = LedID+1;
            j = LedID;
        }

         for(;j < LedNum;j++)
        {
            for(i = 0;i< E_LED_SOURCE_SC;i++)
            {
                    if(mstLed[j].LedCtrl[i] == SourceID)
                    {
                        if(i < (E_LED_SOURCE_SC-1))
                        {
                            LIB_MemCpy((INT8U *)&mstLed[j].LedCtrl[i+1],(INT8U *)&mstLed[j].LedCtrl[i], 
                            (sizeof(OC_LED_CTRL) -i-1));
                        }
                        mstLed[j].LedCtrl[E_LED_SOURCE_SC-1]= E_LED_SC_NONE;
                        OC_LedProc((LED_TYPR_EN)j);
                        break;
                    }
                }
       }
   }
   
}  

/*"************************************************         
Function:   OC_LedProc      
Description:   LED根据各个源的情况计算当前需要运行的模式和时间
Input:  LedID 需要计算的源

Output: 无  
Return: 无 
Others:         
************************************************"*/
void OC_LedProc(LED_TYPR_EN LedID)
{

   INT8U j;
   INT8S i;
   INT8U LedNum;
   INT8U ledMode[E_ID_LED_NUM_SC];

   if((LedID < E_ID_LED_NUM_SC )||(LedID == E_ID_LED_ALL))
   {
        LIB_MemSet(E_LED_CLOSE_MODE, (INT8U *)&ledMode, E_ID_LED_NUM_SC);
        
        if(E_ID_LED_ALL == LedID)
        {
            LedNum = E_ID_LED_NUM_SC;
            j = 0;
        }
        else
        {
            LedNum = LedID+1;
            j = LedID;
        }
        for(;j < LedNum;j++)
        {
            for(i = (E_LED_SOURCE_SC-1);i>=0;i--)
            {
                /*"没有源被注册"*/
                if(mstLed[j].LedCtrl[i] >= E_LED_SOURCE_SC)
                {
                    ;
                }
                else if(LedModeReg[mstLed[j].LedCtrl[i]] == E_LED_TWINKLE_MODE)
                {
                    ledMode[j] = E_LED_TWINKLE_MODE;
                    break;
                }
                else if(LedModeReg[mstLed[j].LedCtrl[i]] == E_LED_ALWAYS_MODE)
                {
                    ledMode[j] = E_LED_ALWAYS_MODE;
                }
            }
             if(mstLedRun[j].ActTime == 0xffff)
            {
                mstLedRun[j].ModeId = ledMode[j];
            }
            else/*"目前已经有单次执行的动作了"*/
            {
                /*"闪烁优先级高于常显"*/
                if((E_LED_ALWAYS_MODE == mstLedRun[j].ModeId)\
                    &&(E_LED_TWINKLE_MODE == ledMode[j]))
                {
                    mstLedRun[j].ModeId = ledMode[j];
                    mstLedRun[j].ActTime = 0xffff;
                }
            }

        }
        OC_LedAct();
    }

}

/*"************************************************         
Function:   OC_LedAct       
Description:   led动作
Input:  


Output: 无  
Return: 无 
Others:         
************************************************"*/
void OC_LedAct(void)
{

   INT8U i;

    for(i = 0;i< E_ID_LED_NUM_SC;i++)
    {
        if(E_LED_TWINKLE_MODE == mstLedRun[i].ModeId)
        {
            if(0 == LedFlashFlag)
            {
                AO_LED(i,LED_OFF);
            }
            else
            {
                AO_LED(i,LED_ON);
            }
        }
        else if(E_LED_ALWAYS_MODE == mstLedRun[i].ModeId)
        {
             AO_LED(i,LED_ON);
        }
        else if(E_LED_CLOSE_MODE == mstLedRun[i].ModeId)
        {
            AO_LED(i,LED_OFF);
        }
    }

}

/*"************************************************         
Function:   OC_LedTask      
Description:   led定时循环
Input:  


Output: 无  
Return: 无 
Others:         
************************************************"*/
void OC_LedTask(void)
{
   INT8U i;

    LedFlashFlag = ~LedFlashFlag;
    for(i = 0;i< E_ID_LED_NUM_SC;i++)
    {
        if((mstLedRun[i].ActTime > 0)&&(0xffff != mstLedRun[i].ActTime))
        {
            mstLedRun[i].ActTime --;
        }
        else
        {
            mstLedRun[i].ActTime = 0xffff;
            OC_LedProc((LED_TYPR_EN)i);
        }
    }
    OC_LedAct();
}




/*****************************************************************************
 函 数 名  : OC_RelayInit
 功能描述  : 
 输入参数  :
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
void OC_RelayInit (void)
{
    LIB_MemSet(0, (INT8U *)&mstRelayActStatus, sizeof(ST_RELAY_ACT_STATUS));
#ifdef THREE_PHASE
    LIB_MemSet(0, (INT8U *)&alarmRelayActstatus[0], MAX_RELAY_SC_NUM);
    AO_Relay(E_RELAY_RELAYALARM , RELAY_ACT_CLOSE ,E_RELAY_MODE_LEVEL);
#endif
}
/*****************************************************************************
 函 数 名  : OC_RelayAct
 功能描述  : 继电器动作函数，驱动继电器时调用
 输入参数  :
            Source :继电器源
            
            RelayID: 继电器ID （继电器ID固定动作模式（内置/外置）
                    E_RELAY_ALM,      报警继电器
                    E_ RELAY _BREAK,  阀控继电器

            RelayActMode:  继电器动作类型
            E_RELAY_MODE_INSIDE,   内置
            E_ RELAY_MODE_LEVEL，外置电平
            E_ RELAY_MODE_PULSE,   外置脉冲

            Act:  继电器动作  
            RELAY_ACT_CLOSE    合闸
            RELAY_ACT_TRIP    跳闸
            此接口在需要驱动继电器动作时调用。

            MsPulseTime:继电器脉冲宽度，单位毫秒
 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 03 01
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
void OC_RelayAct (ST_RELAY_SOURCE_EN Source,EN_OC_RELAYID RelayID, EN_OC_RELAYACTMODE RelayActMode, EN_OC_RELAYACT Act,INT16U MsPulseTime)
{
    
    if((Act == RELAY_ACT_STOP)||(Act == RELAY_ACT_FRIEND))
    {
       return;
    }
#ifdef THREE_PHASE
    INT8U i;

    if(RelayID == E_RELAY_RELAYALARM)
    {
       	alarmRelayActstatus[Source]=Act;
	 
		if(Act == RELAY_ACT_CLOSE)
		{
			for(i=0;i<MAX_RELAY_SC_NUM;i++)
			{
			  	if(alarmRelayActstatus[i] == RELAY_ACT_TRIP)
			  	{
			     	break;
			  	}
			}
			if( i>= MAX_RELAY_SC_NUM)
			{
			  	AO_Relay(E_RELAY_RELAYALARM , RELAY_ACT_CLOSE ,E_RELAY_MODE_LEVEL);
			}
		}
		else
		{
			AO_Relay(E_RELAY_RELAYALARM , RELAY_ACT_TRIP ,E_RELAY_MODE_LEVEL);
		}
    }
    else
#endif
    {
         mstRelayActStatus.Flag=0;
         mstRelayActStatus.Mode=RelayActMode;
         mstRelayActStatus.cmdAct=Act;
         mstRelayActStatus.ActTimer=1;
         mstRelayActStatus.PulseWidth=MsPulseTime/10;
         mstRelayActStatus.ReActTimer=OC_RELAY_REACT_TIME;
         mstRelayActStatus.Flag=1;
    }
}
/*****************************************************************************
 函 数 名  : OC_RelayActReDo
 功能描述  : 继电器补掉
 输入参数  :
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史  :

*****************************************************************************/
void OC_RelayActReDo (void)
{
   EN_OC_RELAYACTMODE RelayActMode;
   EN_OC_RELAYACT Act;
   
   if(mstRelayActStatus.Flag == 0)
   {
      RelayActMode=mstRelayActStatus.Mode;
      Act=mstRelayActStatus.cmdAct;
      OC_RelayAct (E_RELAY_SC_PREPAY,E_RELAY_BREAK, RelayActMode, Act , mstRelayActStatus.PulseWidth*10);
   }
}
/*"****************************************************************************
 函 数 名  : OC_RelayActPro
 功能描述  : 继电器动作定时控制,在10ms中断服务中调用，计算继电器动作时间。
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 03 02
    作    者   : e
    修改内容   : 新生成函数

****************************************************************************"*/
void OC_RelayActPro (EN_OC_RELAYID RelayID)
{
    if(mstRelayActStatus.Flag!=0)
    {
       if(mstRelayActStatus.ActTimer > 0)
       {
          mstRelayActStatus.ActTimer--;
       }
	if(mstRelayActStatus.ActTimer == 0)
       {
          if(mstRelayActStatus.Mode == E_RELAY_MODE_INSIDE)
          {
             if( (mstRelayActStatus.Flag == 1) || (mstRelayActStatus.Flag == 3))
             {
               mstRelayActStatus.Flag++;
               mstRelayActStatus.ActTimer=20;
               AO_Relay(E_RELAY_BREAK, mstRelayActStatus.cmdAct, E_RELAY_MODE_INSIDE);
             }
             else if(mstRelayActStatus.Flag == 2)
             {
               mstRelayActStatus.Flag=3;
               mstRelayActStatus.ActTimer=60;
               AO_Relay(E_RELAY_BREAK, RELAY_ACT_STOP, E_RELAY_MODE_INSIDE);
             }
             else
             {
               mstRelayActStatus.Flag=0;
               AO_Relay(E_RELAY_BREAK, RELAY_ACT_STOP, E_RELAY_MODE_INSIDE);
             }
          }
          else if(mstRelayActStatus.Mode == E_RELAY_MODE_LEVEL)
          {
              mstRelayActStatus.Flag=0;
              AO_Relay(E_RELAY_BREAK, mstRelayActStatus.cmdAct, E_RELAY_MODE_LEVEL);
          }
          else if(mstRelayActStatus.Mode == E_RELAY_MODE_PULSE)
          {
             if(mstRelayActStatus.Flag == 1)
             {
               mstRelayActStatus.Flag=2;
               mstRelayActStatus.ActTimer= mstRelayActStatus.PulseWidth;
               AO_Relay(E_RELAY_BREAK, mstRelayActStatus.cmdAct, E_RELAY_MODE_PULSE);
             }
             else
             {
               mstRelayActStatus.Flag=0;
               AO_Relay(E_RELAY_BREAK, RELAY_ACT_CLOSE, E_RELAY_MODE_PULSE);
             }
          }
          else
          {
          }
       }
    }
    else
    {
      
    }
}    

/*"************************************************
Function:   OC_GetRelayCtrlStatus
Description: 获取继电器控制理论状态
Input:  无
Output: 无
Return: 继电器控制状态
Others:
************************************************"*/
EN_OC_RELAYACT OC_GetRelayCtrlStatus(void)
{
    return mstRelayActStatus.cmdAct;
}

/*"************************************************
Function:   OC_SetRelayCtrlStatus
Description: 直接设置继电器控制状态
Input:  无
Output: 无
Return: 继电器控制状态
Others:此函数调用时，特别小心，不能随便调用
************************************************"*/
void OC_SetRelayCtrlStatus(EN_OC_RELAYACT Act)
{
    mstRelayActStatus.cmdAct = Act;
}

#ifdef THREE_PHASE
/*"************************************************
Function:   OC_GetAlmRelayCmd
Description: 获取报警继电器继电器控制理论状态
Input:  无
Output: 无
Return: 报警继电器控制状态
Others:
************************************************"*/
INT8U OC_GetAlmRelayCmd(void)
{
	INT8U i;
	for(i=0;i<MAX_RELAY_SC_NUM;i++)
	{
	  	if(alarmRelayActstatus[i] == RELAY_ACT_TRIP)/*"判断是否有跳闸源"*/
	  	{
	     	break;
	  	}
	}
	if( i== MAX_RELAY_SC_NUM)/*"没有 跳闸源则为合闸状态"*/
	{
		return RELAY_ACT_CLOSE;
	}
	else
	{
		return RELAY_ACT_TRIP;
	}

}
#endif

/*"************************************************
Function:   
Description: 
Input:  无
Output: 无
Return:
Others:
************************************************"*/
void OC_RelayTaskSec(void)
{
    if(mstRelayActStatus.ReActTimer>0)
    {
        mstRelayActStatus.ReActTimer--;
	 if(mstRelayActStatus.ReActTimer == 0)
	 {
	    OC_RelayActReDo();
	 }
    }
}

/*"****************************************************************************
 函 数 名  : OC_MultiTermnSw
 功能描述  : 多功能模式切换
 输入参数  : INT8U Mode  多功能口模式
                 E_MULTI_SECOND,         秒脉冲
                 E_MULTI_TRAFFSWITCH,   时段投切
                 E_MULTI_DEMAND         需量

 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 03 02
    作    者   : e
    修改内容   : 新生成函数

****************************************************************************"*/
void OC_MultiTermnSw(EN_OC_MultiTermnMode Mode) 
{
    mstMultiStatus.Mode = Mode;
    mstMultiStatus.ActDelayTimer = 0;
    mstMultiStatus.ActTimer      = 0;
    if(E_MULTI_SECOND == Mode)
    { 
        AO_MultiTermn(E_MULTI_SECOND,MULTI_ACT_ON);
    }    
    else
    {
        AO_MultiTermn(E_MULTI_SECOND,MULTI_ACT_OFF);
	 AO_MultiTermn(mstMultiStatus.Mode,MULTI_ACT_OFF);  
    }
}

/*"****************************************************************************
 函 数 名  : OC_MultiTermnProc
 功能描述  : 多功能口定时处理 在10ms中断服务程序中调用，用于处理多功能口脉冲宽度定时。
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 03 02
    作    者   : e
    修改内容   : 新生成函数

****************************************************************************"*/
void OC_MultiTermnProc(void)
{
    ///*""秒脉冲不用控制*/
   // if(E_MULTI_SECOND == mstMultiStatus.Mode)
   // {
   //     return;      
   // }
  //  
  //  if(0 == mstMultiStatus.ActDelayTimer)
  //  {
   //     /*"动作延时控制"*/
   //     if (mstMultiStatus.ActTimer > 0)
    //    {
    //        mstMultiStatus.ActTimer--;
    //        if (0 == mstMultiStatus.ActTimer)
    //       {
    //            AO_MultiTermn(mstMultiStatus.Mode,MULTI_ACT_OFF);  
    //        }
   //     }
   // } 
   // else  /*"延时动作"*/
    //{
   //     mstMultiStatus.ActDelayTimer--;
   //     if(0 == mstMultiStatus.ActDelayTimer)
    //    {
     //       AO_MultiTermn(mstMultiStatus.Mode,MULTI_ACT_ON);    
    //    }    
    //}       
}

INT8U OC_ReadMultiMode(void)
{
	return mstMultiStatus.Mode;
}

void OC_MultiTermn(EN_OC_MultiTermnMode Mode)
{
  //if(Mode != mstMultiStatus.Mode)
 // {
  //  return;
  //}
  //
  // if(E_MULTI_SECOND == Mode)
  // { 
  //  }    
  //  else
  // {
  //       if((E_MULTI_TRAFFSWITCH == Mode)||(E_MULTI_DEMANDPERIOD == Mode)) /*"设置延时时间"*/
  //       {
  //            mstMultiStatus.ActDelayTimer = MULTI_TARIFF_DELAY;
  //           mstMultiStatus.ActTimer      = MULTI_TARIFF_TIME;
  //      }
  //      else
  //      {
  //     }    
  //}
}

void OC_MultiTermn_Chng(void)
{
    if(mstMultiStatus.Mode == E_MULTI_SECOND)
    {
       OC_MultiTermnSw(E_MULTI_REACTPULSE);
    }
    else
    {
       OC_MultiTermnSw(E_MULTI_SECOND);
    }
}

/*"************************************************			
Function: OC_init		
Description: 输出控制抽象层初始化
Input:  无
Output: 无	
Return: 无 
Others: 		
************************************************"*/	
void OC_Init(void)
{
    OCIntTime=0;

    OC_BuzInit();

    OC_LedInit();

    OC_RelayInit();

    OC_MultiTermnSw(E_MULTI_REACTPULSE);
}
/*"************************************************			
Function: 	OC_Int		
Description: 输出控制抽象层10ms调用
Input:  无
Output: 无	
Return: 无 
Others: 		
************************************************"*/	
void OC_Int(void)
{
    OC_RelayActPro(E_RELAY_BREAK);
    
    OC_MultiTermnProc();

    OCIntTime++;
    if(OCIntTime >= 10)  /*"100MS"*/
    {
        OCIntTime=0;

	 OC_BuzzerProc();
    }
}

/*"************************************************			
Function: 	OC_Main		
Description: 
Input:  无
Output: 无	
Return: 无 
Others: 		
************************************************"*/	
void OC_TaskSec(void)
{
	//OC_LedProc(E_ID_LED_RELAY, LEDDO_PERSEC);
	//OC_LedProc(E_ID_LED_BGD, LEDDO_PERSEC);
	OC_LedTask();
	OC_RelayTaskSec();
}

