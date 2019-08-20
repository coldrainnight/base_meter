/*"***********************************************************		
FileName: AO.c		
Author: ������       
Version : 1.0       Date: 2017.2.16	
Description: �����������
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
Description: LED��������
Input:  
Obj : LED������
      E_LED_ALM    ������
      E_LED_RELAY  �̵���ָʾ��
      E_LED_BLIGHT �����
      E_LED_COMM   ͨ��ָʾ��

Act :  LED����
       LED_ON      1    LED���� 
       LED_OFF     0    LED�ر�

Output: ��	
Return: �� 
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
 �� �� ��  : AO_BuzOn
 ��������  : ����������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 04 05
    ��    ��   : e
    �޸�����   : �����ɺ���

*****************************************************************************/
void AO_BuzOn(void)
{
 GPIO_ConfigPin(BEEP, OUTPUT, NORMAL_OUT);
 GPIO_OutputPin(BEEP, HIGH);
}    

/*****************************************************************************
 �� �� ��  : AO_BuzOff
 ��������  : �������ر�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 04 05
    ��    ��   : e
    �޸�����   : �����ɺ���

*****************************************************************************/
void AO_BuzOff(void)
{
 GPIO_ConfigPin(BEEP, OUTPUT, NORMAL_OUT);
 GPIO_OutputPin(BEEP, LOW);
}    

/*****************************************************************************
 �� �� ��  : AO_Buzzer
 ��������  : �������������ƺ���
 �������  : INT8U  buzAct  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 04 05
    ��    ��   : e
    �޸�����   : �����ɺ���

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
Description: LED��������
Input:  
        Obj  Ŀ��:�̵����������� 
            E_RELAY_ALM  �����̵���
            E_ RELAY _BREAK  ���ؼ̵���

        Act  �̵�������     
            RELAY_ACT_CLOSE  ��բ
            RELAY_ACT_TRIP   ��բ

        Mode �̵�������ģʽ
            E_RELAY_MODE_INSIDE  ����
            E_ RELAY_MODE_LEVEL  ���õ�ƽ
            E_ RELAY_MODE_PULSE  ��������

Output: ��	
Return: �� 
Others: 		
************************************************"*/	
void AO_Relay(INT8U Obj, INT8U Act, INT8U Mode)
{
#ifdef THREE_PHASE
    if(Obj == E_RELAY_RELAYALARM)
    {
        if(E_RELAY_MODE_LEVEL == Mode)  /*"���õ�ƽ"*/
       {
           GPIO_ConfigPin(RLY_ALARM, OUTPUT, NORMAL_OUT);
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"��բ"*/
           {
             GPIO_OutputPin(RLY_ALARM, LOW);
           }
           else if(RELAY_ACT_TRIP == Act)                                  /*"��բ"*/
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
       /*"���ü̵���"*/
       if(E_RELAY_MODE_INSIDE == Mode)
       {
           GPIO_ConfigPin(TRIP_RELAY_ON, OUTPUT, NORMAL_OUT);
           GPIO_ConfigPin(TRIP_RELAY_OFF, OUTPUT, NORMAL_OUT);
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"��բ"*/
           {
             GPIO_OutputPin(TRIP_RELAY_ON, HIGH );
             GPIO_OutputPin(TRIP_RELAY_OFF,LOW);
           }
           else if(RELAY_ACT_TRIP == Act)               /*"��բ"*/
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
       else if((E_RELAY_MODE_LEVEL == Mode)||(E_RELAY_MODE_PULSE == Mode))  /*"���õ�ƽ"*/
       {
#ifdef THREE_PHASE
           GPIO_ConfigPin(TRIP_RELAY_OFF, OUTPUT, NORMAL_OUT);
#else
           GPIO_ConfigPin(TRIP_RELAY_ON, OUTPUT, NORMAL_OUT);
#endif
           if((RELAY_ACT_CLOSE == Act) || (RELAY_ACT_FRIEND == Act))                   /*"��բ"*/
           {
#ifdef THREE_PHASE
             GPIO_OutputPin(TRIP_RELAY_OFF, LOW);
#else
             GPIO_OutputPin(TRIP_RELAY_ON, LOW);
#endif
           }
           else if(RELAY_ACT_TRIP == Act)                                  /*"��բ"*/
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
Description: �๦�ܿڶ�������
Input:  
Mode �� �๦�ܿ�ģʽ
        E_MULTI_SECOND ,         ������
        E_MULTI _TRAFFSWITCH,  ʱ��Ͷ��
        E_MULTI _DEMAND  ����

Act  �๦�ܿڶ���     
        MULTI_ACT_ON   �๦�ܿڶ���
        MULTI_ACT_OFF  �๦�ܿڽ�������


Output: ��	
Return: �� 
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

