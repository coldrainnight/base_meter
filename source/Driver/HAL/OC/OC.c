/*"***********************************************************		
FileName: OC.c		
Author: ������       
Version : 1.0       Date: 2017.2.16	
Description: ������Ƴ����
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
    LEDDO_PERSEC=0,                /*"ÿ�����"*/
    LEDDO_NOW                            /*"��������"*/
}LEDDO_KIND;
#define LED_OPEN   1
#define LED_CLOSE  0
volatile static OC_LED_CTRL mstLed[E_ID_LED_NUM_SC];/*"���е���ģʽ����"*/
OC_LED_STATUS mstLedRun[E_ID_LED_NUM_SC];/*"��ǰ���е�ģʽ"*/
volatile static INT8U LedFlashFlag;
/*"��Դ��ģʽ�󶨣��������µ�Դ֮ǰ����Ҫ�ڴ˱�ע��Դʹ�õ�ģʽ"*/
const  INT8U LedModeReg[E_LED_SOURCE_SC] =
{
    E_LED_ALWAYS_MODE,        /*"��ʼ��"*/
    //E_LED_ALWAYS_MODE,        /*"ͨ�� "*/
    //E_LED_ALWAYS_MODE,       /*"����"*/
    //E_LED_ALWAYS_MODE,        /*"Ԥ����"*/
    //E_LED_ALWAYS_MODE,        /*"����"*/
    E_LED_ALWAYS_MODE,       /*"�¼�"*/
    E_LED_ALWAYS_MODE,        /*"����"*/
    //E_LED_TWINKLE_MODE,       /*" Ԥ������˸ "*/
};


//volatile static OC_LED_STATUS mstLedRelay[E_LED_RELAY_SC];
//volatile static OC_LED_STATUS mstLedBgt[E_LED_BGT_SC];
void OC_LedAct(void);
void OC_LedInit(void);
void OC_LedProc(LED_TYPR_EN LedID);


/*"RELAY"*/
#define OC_RELAY_REACT_TIME    (151*60) /*"�̵�������״̬�ٴζ���ʱ��(��)"*/
volatile static ST_RELAY_ACT_STATUS mstRelayActStatus;
#ifdef THREE_PHASE
static INT8U alarmRelayActstatus[MAX_RELAY_SC_NUM];
#endif
/*"MT"*/
static ST_MULTI_STATUS mstMultiStatus;      /*"�๦�ܺ�һ״̬����"*/

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
Description:��������ʼ��
Input:  ��
Output: ��	
Return: �� 
Others: 		
************************************************"*/	 
void OC_BuzInit(void)
{
   AO_Buzzer(BUZ_OFF);

   LIB_MemSet(0, (INT8U *)&mstBuzzer[E_BUZ_SC_PREPAY], sizeof(OC_BUZZ_STATUS));
}
/*"************************************************			
Function: 	OC_BuzzerOnSingle		
Description:    ��������������
Input:  ModeID������������ģʽ��
                E_BUZ_SHORT_MODE1 ��������ģʽ1 
                E_BUZ_SHORT_MODE2 ��������ģʽ2 
Output: ��	
Return: �� 
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
Description:    ��������������
Input:  
ModeID������������ģʽ
        E_BUZ_ CONT_MODE1  ��������ģʽ1�� ���ȼ�1
        E_BUZ_ CONT_MODE2  ��������ģʽ2,  ���ȼ�2 
        E_BUZ_ CONT_MODE3  ��������ģʽ3�� ���ȼ�3

Source: ����������������Դ
        E_BUZ_ SC_ ERR     ����Դ�����ϱ���
        E_BUZ_ SC_ PREPAY  ����Դ��Ԥ����
        
Ms_Time:ModeID=0ʱ�����к�ͣ��ʱ��������������ơ�ͬ�Ƶ���˸

Output: ��	
Return: �� 
Others: 		
************************************************"*/
void OC_BuzzerOnContin (OC_BuzMode_EN ModeID , OC_BuzSource Source, INT16U Ms_Time)
{
    
}

/*"************************************************			
Function: 	OC_BuzzerOff		
Description:   �����������ر�
Input:  
ModeID������������ģʽ

Source: ����������������Դ


Output: ��	
Return: �� 
Others: 		
************************************************"*/
void OC_BuzzerOff(OC_BuzMode_EN ModeID, OC_BuzSource Source)
{
   
}

/*"************************************************			
Function: 	OC_BuzzerProc		
Description:   �����������ر�
Input:  
ModeID������������ģʽ

Source: ����������������Դ


Output: ��	
Return: �� 
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
�� �� ��  : OC_LedInit
��������  : ���δ���LED�ƣ�����ʱ��һ�»���˸һ�£�������
�������  : INT8U LedID: �����ơ��̵����ơ������
              E_ID_LED_ALARM��  E_ID_LED_RELAY ��E_ID_LED_BGD
         INT8U ModeID: ��������ģʽ 
         INT8U SecTime: ��λ��     
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : ������ 2017 03 01
��    ��   : e
�޸�����   : �����ɺ���

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
 �� �� ��  : OC_LedOnSingle
 ��������  : ���δ���LED�ƣ�����ʱ��һ�»���˸һ�£�������
 �������  : INT8U LedID: �����ơ��̵����ơ������
                  E_ID_LED_ALARM��  E_ID_LED_RELAY ��E_ID_LED_BGD
             INT8U ModeID: ��������ģʽ 
             INT8U SecTime: ��λ��     
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 01
    ��    ��   : e
    �޸�����   : �����ɺ���

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
 �� �� ��  : OC_LedOnContin
 ��������  : ����������˸����
 �������  : INT8U LedID :�����ơ��̵����ơ������    
             INT8U ModeID:����ģʽ   
             INT16U Source:Դ���Ա��ڹر�ʱ�����  
             INT16U Ms_Time:ModeID=0ʱ����˸ʱ��������������ơ�
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 01
    ��    ��   : e
    �޸�����   : �����ɺ���

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
                    /*"�ҵ�δʹ�õĻ���"*/
                    if(mstLed[j].LedCtrl[i] == E_LED_SC_NONE)
                    {
                        mstLed[j].LedCtrl[i] = SourceID;
                        OC_LedProc((LED_TYPR_EN)j);
                        break;
                    }
                     /*"�ҵ�ͬһ��Դ��ע��ʹ�õĻ���"*/
                    else if(mstLed[j].LedCtrl[i] == SourceID)
                    {
                        break;
                    }
                }
        }
    }

}

/*"****************************************************************************
 �� �� ��  : OC_LedOff
 ��������  : ������������˸����Դ���ر�
 �������  : INT8U LedID    
             INT8U ModeID   
             INT16U Source  ,�������16�������ʾ�ر�����Դ����������0xffff
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 01
    ��    ��   : e
    �޸�����   : �����ɺ���

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
Description:   LED���ݸ���Դ��������㵱ǰ��Ҫ���е�ģʽ��ʱ��
Input:  LedID ��Ҫ�����Դ

Output: ��  
Return: �� 
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
                /*"û��Դ��ע��"*/
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
            else/*"Ŀǰ�Ѿ��е���ִ�еĶ�����"*/
            {
                /*"��˸���ȼ����ڳ���"*/
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
Description:   led����
Input:  


Output: ��  
Return: �� 
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
Description:   led��ʱѭ��
Input:  


Output: ��  
Return: �� 
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
 �� �� ��  : OC_RelayInit
 ��������  : 
 �������  :
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
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
 �� �� ��  : OC_RelayAct
 ��������  : �̵������������������̵���ʱ����
 �������  :
            Source :�̵���Դ
            
            RelayID: �̵���ID ���̵���ID�̶�����ģʽ������/���ã�
                    E_RELAY_ALM,      �����̵���
                    E_ RELAY _BREAK,  ���ؼ̵���

            RelayActMode:  �̵�����������
            E_RELAY_MODE_INSIDE,   ����
            E_ RELAY_MODE_LEVEL�����õ�ƽ
            E_ RELAY_MODE_PULSE,   ��������

            Act:  �̵�������  
            RELAY_ACT_CLOSE    ��բ
            RELAY_ACT_TRIP    ��բ
            �˽ӿ�����Ҫ�����̵�������ʱ���á�

            MsPulseTime:�̵��������ȣ���λ����
 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 01
    ��    ��   : e
    �޸�����   : �����ɺ���

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
 �� �� ��  : OC_RelayActReDo
 ��������  : �̵�������
 �������  :
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ  :

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
 �� �� ��  : OC_RelayActPro
 ��������  : �̵���������ʱ����,��10ms�жϷ����е��ã�����̵�������ʱ�䡣
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 02
    ��    ��   : e
    �޸�����   : �����ɺ���

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
Description: ��ȡ�̵�����������״̬
Input:  ��
Output: ��
Return: �̵�������״̬
Others:
************************************************"*/
EN_OC_RELAYACT OC_GetRelayCtrlStatus(void)
{
    return mstRelayActStatus.cmdAct;
}

/*"************************************************
Function:   OC_SetRelayCtrlStatus
Description: ֱ�����ü̵�������״̬
Input:  ��
Output: ��
Return: �̵�������״̬
Others:�˺�������ʱ���ر�С�ģ�����������
************************************************"*/
void OC_SetRelayCtrlStatus(EN_OC_RELAYACT Act)
{
    mstRelayActStatus.cmdAct = Act;
}

#ifdef THREE_PHASE
/*"************************************************
Function:   OC_GetAlmRelayCmd
Description: ��ȡ�����̵����̵�����������״̬
Input:  ��
Output: ��
Return: �����̵�������״̬
Others:
************************************************"*/
INT8U OC_GetAlmRelayCmd(void)
{
	INT8U i;
	for(i=0;i<MAX_RELAY_SC_NUM;i++)
	{
	  	if(alarmRelayActstatus[i] == RELAY_ACT_TRIP)/*"�ж��Ƿ�����բԴ"*/
	  	{
	     	break;
	  	}
	}
	if( i== MAX_RELAY_SC_NUM)/*"û�� ��բԴ��Ϊ��բ״̬"*/
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
Input:  ��
Output: ��
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
 �� �� ��  : OC_MultiTermnSw
 ��������  : �๦��ģʽ�л�
 �������  : INT8U Mode  �๦�ܿ�ģʽ
                 E_MULTI_SECOND,         ������
                 E_MULTI_TRAFFSWITCH,   ʱ��Ͷ��
                 E_MULTI_DEMAND         ����

 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 02
    ��    ��   : e
    �޸�����   : �����ɺ���

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
 �� �� ��  : OC_MultiTermnProc
 ��������  : �๦�ܿڶ�ʱ���� ��10ms�жϷ�������е��ã����ڴ���๦�ܿ������ȶ�ʱ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 03 02
    ��    ��   : e
    �޸�����   : �����ɺ���

****************************************************************************"*/
void OC_MultiTermnProc(void)
{
    ///*""�����岻�ÿ���*/
   // if(E_MULTI_SECOND == mstMultiStatus.Mode)
   // {
   //     return;      
   // }
  //  
  //  if(0 == mstMultiStatus.ActDelayTimer)
  //  {
   //     /*"������ʱ����"*/
   //     if (mstMultiStatus.ActTimer > 0)
    //    {
    //        mstMultiStatus.ActTimer--;
    //        if (0 == mstMultiStatus.ActTimer)
    //       {
    //            AO_MultiTermn(mstMultiStatus.Mode,MULTI_ACT_OFF);  
    //        }
   //     }
   // } 
   // else  /*"��ʱ����"*/
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
  //       if((E_MULTI_TRAFFSWITCH == Mode)||(E_MULTI_DEMANDPERIOD == Mode)) /*"������ʱʱ��"*/
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
Description: ������Ƴ�����ʼ��
Input:  ��
Output: ��	
Return: �� 
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
Description: ������Ƴ����10ms����
Input:  ��
Output: ��	
Return: �� 
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
Input:  ��
Output: ��	
Return: �� 
Others: 		
************************************************"*/	
void OC_TaskSec(void)
{
	//OC_LedProc(E_ID_LED_RELAY, LEDDO_PERSEC);
	//OC_LedProc(E_ID_LED_BGD, LEDDO_PERSEC);
	OC_LedTask();
	OC_RelayTaskSec();
}

