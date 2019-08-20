/*"***********************************************************"*/		
/*"文件名: SID.c	                                            "*/	
/*"作  者: PengC                                              "*/ 
/*"版  本: 1.0       日期: 2017.02.16                         "*/	
/*"文件描述: 状态输入底层驱动                                 "*/
/*"函数列表:                                                  "*/		
/*"<函数名>	                                                  "*/	
/*"***********************************************************"*/

#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\INT.h"
#include "Driver\MCU\EC_driver.h"
#include "SID.h"

#define SID_NULL 0x00

typedef struct
{
	INT8U          io;                         /*"IO口"*/
	INT8U          trvl;                      /*"返回值为TRUE的IO口电平"*/ 
	INT8U          pllcnt;                  /*"循环读的次数"*/
	INT8U          plldly;                  /*"循环读的间隔 单位ms"*/
	INT8U          cnt;                      /*"读的次数"*/
	INT8U          dly;                      /*"读的间隔 单位ms"*/  
}SID_INFO;

#ifdef THREE_PHASE
const SID_INFO mSidInfo[SID_MAX] = 
{
  {FAC_JUMP, LOW, 50, 20, 10, 100},                /*"SID_FAC"*/
  {METER_COVER_CHK,HIGH, 1,  0, 15, 100},           /*"SID_MTRCOVER"*/
  {METER_BOX_CHK,HIGH, 1,  0, 15, 100},           /*"SID_RAILCOVER"*/
  {RELAY_STA_CHK,LOW, 2,  5, 2, 5},             /*"SID_REALY"*/
  {_CPU_SW,HIGH, 2,  5, 2, 10},                   /*"SID_CARD"*/
  {MAGNETDET,LOW, 1,  0, 2, 10},                   /*"SID_MAGNET"*/
};
#else
const SID_INFO mSidInfo[SID_MAX] = 
{
  {FAC_JUMP, LOW, 50, 20, 5, 100},                /*"SID_FAC"*/
  {METER_COVER_CHK,HIGH, 2,  5, 10, 100},           /*"SID_MTRCOVER"*/
  {RELAY_STA_CHK,HIGH, 2,  5, 2, 100},             /*"SID_REALY"*/
  //{_CPU_SW,LOW, 2,  5, 3, 10},                   /*"SID_CARD"*/
};
#endif


static INT8U  mStateIn[SID_MAX];           /*"当前状态"*/

static INT16U mStateInT[SID_MAX];         /*"当前状态的时标"*/

static INT8U  mStateInP[SID_MAX];         /*"当前状态的累计时间"*/

static INT8U lastSIDReadValue[SID_MAX];

/*"*****************************************************************************"*/
/*"  Function:       SID_Init                                                   "*/
/*"  Description:    状态输入的初始化                                           "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:MCU_RUNMODE 正常上电、其他 电池上电                                  "*/
/*"  Output                                                                     "*/
/*"  Return:                                                                    "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
void SID_Init(INT8U mode,INT8U sel)
{
	INT8U i;

	for(i=0;i<SID_MAX;i++)
	{
          if((i == sel)||(SID_MAX == sel))
          {
    	       mStateIn[i]=SID_UNDEF;
              mStateInT[i]=0;
              mStateInP[i]=0;
		lastSIDReadValue[i]=SID_UNDEF;
    	       switch(i)
              {
                case SID_FAC:
		    if(mode != MCU_LPMODE_DEEP)
		    {
                     GPIO_ConfigPin(FAC_JUMP,INPUT,NULL_IN);
		    }
                break;
                
                case SID_MTRCOVER:
                   GPIO_ConfigPin(METER_COVER_CHK,INPUT,NULL_IN);
                break;
#ifdef THREE_PHASE                
                case SID_RAILCOVER:
                   GPIO_ConfigPin(METER_BOX_CHK,INPUT,NULL_IN);
                break;
#endif                
                case SID_REALY:
                   GPIO_ConfigPin(RELAY_STA_CHK,INPUT,NULL_IN);
#ifdef SINGLE_PHASE
                   GPIO_ConfigPin(RELAY_PWR_CHK,OUTPUT,NORMAL_OUT);
                   GPIO_OutputPin(RELAY_PWR_CHK,HIGH);
#endif
                break;
                
                case SID_CARD:
		     if(mode != MCU_LPMODE_DEEP)
		     {
                      GPIO_ConfigPin(_CPU_SW,INPUT,NULL_IN);
		     }
                break;
#ifdef THREE_PHASE                  
                case SID_MAGNET:
                   GPIO_ConfigPin(MAGNETDET,INPUT,NULL_IN);
                break;
#endif    
                default:
                break;
             }
         }
	}
}


/*"*****************************************************************************"*/
/*"  Function:       SID_Release                                                "*/
/*"  Description:    状态输入的释放                                         "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:                                                                     "*/
/*"  Output                                                                     "*/
/*"  Return:                                                                    "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
void SID_Release(INT8U mode,INT8U sel)
{
  if(mode==MCU_RUNMODE) /*"正常上电"*/
  {
	
  }
  else
  {
      if((sel==SID_MAX)||(sel==SID_REALY))
      {
#ifdef SINGLE_PHASE
	     GPIO_OutputPin(RELAY_PWR_CHK,LOW);
#endif
     }
  }
}


/*"*****************************************************************************"*/
/*"  Function:       SID_PollRead                                               "*/
/*"  Description:    状态输入的IO口读函数                                       "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:状态输入类型                                                         "*/
/*"  Output：                                                                   "*/
/*"  Return: 当前状态                                                           "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
INT8U SID_PollRead(INT8U sel)
{
	INT8U result;
	INT8U i;
	INT8U PollCnt;           /*"读IO口的次数"*/
	INT8U PollDly;           /*"读IO口的间隔，单位ms"*/
	INT8U PollIO;            /*"读的IO口"*/
	INT8U TrueValue;     /*"返回值为true的IO口电平"*/
	
	if(sel>=SID_MAX)
	{
		return SID_FALSE;
	}
	
	if(mSidInfo[sel].io==SID_NULL)
	{
		return SID_FALSE;
	}
	
	result=SID_TRUE;
	i=0;
	PollCnt=mSidInfo[sel].pllcnt;
	PollDly=mSidInfo[sel].plldly;
	PollIO=mSidInfo[sel].io;
	TrueValue=mSidInfo[sel].trvl;
	
	for(i=0;i<PollCnt;i++)
	{
		if(GPIO_ReadPin(PollIO)!=TrueValue)
		{
			result=SID_FALSE;
			
			break;
		}
		else
		{
			if(i<(PollCnt-1))
			{
			   MCU_DelayMs(PollDly);
			}
		}
	}
	
	return result;
}


/*"*****************************************************************************"*/
/*"  Function:       SID_Read                                                   "*/
/*"  Description:    状态输入的读函数                                           "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:状态输入类型                                                         "*/
/*"  Output：                                                                   "*/
/*"  Return: 当前状态                                                           "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
INT8U SID_Read(INT8U sel)
{
	INT8U ReadCnt;       /*"读IO口的次数"*/
	INT8U ReadDly;       /*"读IO口的间隔，单位ms"*/
	INT8U ReadIO;        /*"读的IO口"*/
	INT8U TrueValue;     /*"返回值为true的IO口电平"*/
	INT8U nowReadState;
	
	if(sel>=SID_MAX)
	{
		return SID_FALSE;
	}
	
	if(mSidInfo[sel].io==SID_NULL)
	{
		return SID_FALSE;
	}
	
	ReadCnt=mSidInfo[sel].cnt;
	ReadDly=mSidInfo[sel].dly;
	ReadIO=mSidInfo[sel].io;
	TrueValue=mSidInfo[sel].trvl;
	
	if(MCU_CmpTick(mStateInT[sel],ReadDly)) /*"更新时间到"*/
	{
         nowReadState=GPIO_ReadPin(ReadIO);
	  if(mStateIn[sel]==SID_FALSE)
	  {
		   if(nowReadState==TrueValue)
		   {
			   mStateInP[sel]++;
			   
			   if(mStateInP[sel]>=ReadCnt)
			   {
			   	 mStateInP[sel]=0;
			   	 
			   	 mStateIn[sel]=SID_TRUE;
			   }
		   }
		   else
		   {
			    mStateInP[sel]=0;
		   }
	  }
	  else if(mStateIn[sel]==SID_TRUE)
	  {
		   if(nowReadState!=TrueValue)
		   {
			   mStateInP[sel]++;
			   
			   if(mStateInP[sel]>=ReadCnt)
			   {
			   	 mStateInP[sel]=0;
			   	 
			   	 mStateIn[sel]=SID_FALSE;
			   }
		   }
		   else
		   {
			    mStateInP[sel]=0;
		   }
	  }
	  else
	  {
		   if(nowReadState == lastSIDReadValue[sel])
		   {
			   mStateInP[sel]++;
			   
			   if(mStateInP[sel]>=ReadCnt)
			   {
			   	 mStateInP[sel]=0;
			   	 if(nowReadState==TrueValue)
			   	 {
			   	    mStateIn[sel]=SID_TRUE;
			   	 }
				 else
				 {
				    mStateIn[sel]=SID_FALSE;
				 }
			   }
		   }
		   else
		   {
			    mStateInP[sel]=0;
		   }
		   
	  }
	  
	  lastSIDReadValue[sel]=nowReadState;
	  
	  mStateInT[sel]=MCU_GetTick();  /*"更新当前的时标"*/
  }
	
	return mStateIn[sel];
}

/*"*****************************************************************************"*/
/*"  Function:       SID_IntVectSet                                           "*/
/*"  Description:                                       "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:                           "*/
/*"  Output                                                                     "*/
/*"  Return:                                                                    "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
INT8U SID_IntVectSet(INT8U sel, ISR_FNCT_VOID intfunc)
{
    return FALSE;
}

/*"*****************************************************************************"*/
/*"  Function:       SID_NullIntFunc                                            "*/
/*"  Description:    状态输入的空的中断函数                                     "*/
/*"  Calls:                                                                     "*/
/*"  Called By:                                                                 "*/
/*"  Input:MCU_RUNMODE 正常上电、其他 电池上电                                  "*/
/*"  Output                                                                     "*/
/*"  Return:                                                                    "*/
/*"  Others: PengC        code size  byte                                       "*/
/*"*****************************************************************************"*/
void SID_NullIntFunc(void)
{
}
