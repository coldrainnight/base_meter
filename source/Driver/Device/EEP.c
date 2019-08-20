/*"***********************************************************		
FileName: EEP.c		
Author: ����       
Version : 1.0       Date: 2016.12.27	
Description: eeprom�豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 
#include "Driver\MCU\IIC.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\device\EEP.h"

typedef enum
{
    E_EEP_DEV_1 = 0,
    #if defined (EEP_DEV_NUM_2)  /*��һ���ڿ�*/
    E_EEP_DEV_2,
    #endif
}ENUM_EEP_ID;

/*ҳд��С����*/
#define EEP_32KB_PAGESIZE  (64)
#define EEP_64KB_PAGESIZE  (128)

/*EEPROM��д����*/
#define EEP_WRITE    (0xFE)     /*"EEP�豸д����slave_addrִ�� �� ����"*/
#define EEP_READ     (0x01)     /*"EEP�豸������slave_addrִ�� �� ����"*/


/**/
#define EEP_ERR_TIME (100)    /*"�ж�EEP����ķ�ֵ"*/
#define EEP_TRY_TIME  (3)       /*"����EEP�豸�����Դ���"*/ 
#define EEP_POLL_TIME (30)     /*EEP ACK �ȴ�������Դ���*/



static INT16U mEepRwErrTimes ;   /*"EEp��д�������"*/

const ST_EEP_DRV stEepDrvInfo[]=
{
    {
        EEP_SLAVE_ADDR_1 , /*���豸��ַ*/
        EEP_64KB_MAX_ADDR,/*�豸�������ɶ�д��ַ*/
        EEP_64KB_PAGESIZE,/*EEP�豸���ҳд����*/
    },
  
    #if defined (EEP_DEV_NUM_2) /*����2���ڿ�*/
    {
        EEP_SLAVE_ADDR_2 , /*���豸��ַ*/
        EEP_DEV_2_MAX_ADDR,/*�豸���ɶ�д��ַ*/
        EEP_32KB_PAGESIZE,/*EEP�豸���ҳд����*/
    },
    #endif
};


static INT8U EEP_Addressing(INT8U EepId,INT16U DevAddr);
static INT8U EEP_AckPoll (INT8U EepId);
#if defined (EEP_DEV_NUM_2) /*��һ���ڿ�*/
static INT8U EEP_LogicAddrConvert(EEP_ADDR_TYPE LogicAddr,INT16U *DevAddr);
#endif
static void  EEP_WP_En(INT8U State);
static void  EEP_RwErrCount(INT8U Ret);
static void  EEP_Reset(void);


void EEP_Init(void);
INT8U EEP_Read(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
INT8U EEP_Write(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
INT8U EEP_GetErrSta(void);
void EEP_Close(void);

#if defined (EEP_DEV_NUM_2)
static INT8U EEP_LogicAddrConvert(EEP_ADDR_TYPE LogicAddr,INT16U *DevAddr)
{
    
    INT8U EepId; 
    if(LogicAddr <= stEepDrvInfo[E_EEP_DEV_1].DevMaxAddr) /*��ַ��EEP DEV1 ����*/
    {
        *DevAddr = (INT16U)LogicAddr;
        EepId =  E_EEP_DEV_1;
    }
    else /*��ַ��EEP Dev2 ����*/
    {
        *DevAddr = (INT16U)(LogicAddr - stEepDrvInfo[E_EEP_DEV_1].DevMaxAddr);
        EepId =  E_EEP_DEV_2;
    }
    return EepId;

}

#endif


/*"************************************************			
Function: 	RwErrCount		
Description: EEP��д�����ۼӴ�������
Input:  EEP��д�ķ���ֵ��������EEP��д��������Ĵ���
Output: ��	
Return: TRUE:�ȴ�ҳд���OK  FALSE:�ȴ�ҳд��ʱ 
Others: 		
************************************************"*/	

static void EEP_RwErrCount(INT8U Ret)
{
    if(Ret == FALSE)
    {
        if(mEepRwErrTimes >  2*EEP_ERR_TIME)
        {
        }
        else
        {
            mEepRwErrTimes++;
        }
    }
    else
    {
        if(mEepRwErrTimes > EEP_ERR_TIME)
        {
            mEepRwErrTimes--;
        }
        else
        {
            mEepRwErrTimes = 0;
        }
    }
}


/*"************************************************			
Function: 	EEP_WP_On		
Description: ��EEPROM�ڲ�ҳд��ʼʱ����Ҫ�ȴ�ҳд���ִ�к�������
             ��ȴ�ʱ��Ϊ6ms
Input:  ��
Output: ��	
Return: TRUE:�ȴ�ҳд���OK  FALSE:�ȴ�ҳд��ʱ 
Others: 		
************************************************"*/	


static INT8U EEP_AckPoll (INT8U EepId)
{
    INT8U Poll_Times;
    INT8U Tmp;
    
    for(Poll_Times = 0; Poll_Times < EEP_POLL_TIME; Poll_Times++ )
    {
        MCU_DelayMs(1);
        IIC_Start(IIC_ID_EEP);
        Tmp = (stEepDrvInfo[EepId].SlaveAddr) & EEP_WRITE;
        if (TRUE == IIC_SendChar(IIC_ID_EEP,&Tmp,1))
        {
            return TRUE;
        }
        else
        {
        }
    }
    return FALSE ;
}


/*"************************************************			
Function: 	EEP_WP_On		
Description: ����EEP��д��������
Input:  State : TRUE ��Ϊд�������� FALSE �ر�д����
Output: ��	
Return: ��
Others: 		
************************************************"*/	
static void EEP_WP_En(INT8U State)
{
    INT8U PinSta;
    
    GPIO_ConfigPin(MEM_WP,OUTPUT,NORMAL_OUT);

	if(TRUE == State)
	{
		PinSta= HIGH;
	}
	else
	{
        PinSta= LOW;
    }
    GPIO_OutputPin(MEM_WP,PinSta);
}


/*"************************************************			
Function: 	EEP_Reset		
Description: EEPROM�豸��λ			
Input:  ��
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 		
************************************************"*/	
static void EEP_Reset(void)
{
    IIC_Init(IIC_ID_EEP);
    IIC_Stop(IIC_ID_EEP);
    /*��Ҫ����оƬ�ֲ�鿴�Ƿ�����ظ�λʱ��*/
    
}

/*"************************************************			
Function: 	EEP_Addressing		
Description: EEP��ǰָ��ָ��addrλ�ã���д��������Ҫ�������					
Input:  addr��EEPROM���ݴ洢��ַ�����֧��16λ
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 		
************************************************"*/	
static INT8U EEP_Addressing(INT8U EepId,INT16U DevAddr)
{
    INT8U Buf[3]; 
    
    Buf[0] = (stEepDrvInfo[EepId].SlaveAddr) & EEP_WRITE;
    Buf[1] = (INT8U)((DevAddr&0xFF00)>>8);
    Buf[2] = (INT8U)(DevAddr&0x00FF);

    return IIC_SendChar(IIC_ID_EEP, Buf, 3);
}


/*"********************************************************

    The  interface of EEPROM to the upper layer start

*********************************************************"*/




/*Ŀǰֻ����32K �� 64K,������δ��
����TRUE ��ʾ��С������һ�£�FALSEΪ��һ��
*/
INT8U  EEPSize_Compara(void)
{
    EEP_ADDR_TYPE DetectSize;
    EEP_ADDR_TYPE TestAddr;
    INT8U Addr0Val;
    INT8U AddrTestVal;
    INT8U Tmp;

    #if defined (EEP_DEV_NUM_2)
        no suport now!
    #endif

    /*�ڿ���С������ֻ֧��32KB �� 64KB eep�ļ��*/
    BUILD_BUG_ON ((EEP_DEV_MAX_ADDR != EEP_64KB_MAX_ADDR) && 
                    (EEP_DEV_MAX_ADDR != EEP_32KB_MAX_ADDR));

    
    DetectSize = 0; 
    TestAddr = (EEP_DEV_MAX_ADDR+1)/2;

    EEP_Read(0x0000, (INT8U * )&Addr0Val, 1);
    EEP_Read(TestAddr, (INT8U * )&AddrTestVal, 1);

    Tmp = 0xA5;
    EEP_Write(0x0000, (INT8U * )&Tmp, 1);

    Tmp = 0xCC;
    EEP_Write(TestAddr, (INT8U * )&Tmp, 1);

    Tmp= 0x00;
    EEP_Read(0x0000, (INT8U * )&Tmp, 1);
    if(Tmp == 0xCC)
    {
        /*˵���Ѿ��ع�*/
        DetectSize = 0;
    }
    else
    {
        if(Tmp == 0xA5)
        {
            DetectSize = EEP_DEV_MAX_ADDR;
        }
    }

    EEP_Write(0x0000, (INT8U * )&Addr0Val, 1);
    EEP_Write(TestAddr, (INT8U * )&AddrTestVal, 1);
    
    if(DetectSize != stEepDrvInfo[0].DevMaxAddr)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    
    }
}

/*"************************************************			
Function: 	EEP_Init		
Description: EEP��ʼ�� ��ȡEEP�Ĳ���					
Input: 
Output: ��	
Return: 	
Others: 
************************************************"*/	
void EEP_Init(void)
{
    IIC_Init(IIC_ID_EEP);
    IIC_Stop(IIC_ID_EEP);
    EEP_WP_En(TRUE);
    /*"��ʼ��EEP��д�������"*/
    mEepRwErrTimes = 0;

}


void EEP_Close(void)
{
    IIC_Close(IIC_ID_EEP);
    EEP_WP_En(TRUE);
}
/*"************************************************			
Function: 	EEP_Read		
Description: ��EEPROMָ���洢��ַ��ȡָ�����ȵ����� 					
Input:  addr��EEPROM���ݴ洢��ַ
        pBuf�����ݶ�ȡ���ݻ����ַָ��
        len����ȡ���ݳ���
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 
  |start|slave addr(W)|1st wordaddr|2nd wordaddr|start|slave addr(R)| Read n byte|stop
************************************************"*/	

INT8U EEP_Read(EEP_ADDR_TYPE LogicAddr, INT8U *pBuf, INT16U Len)
{
    INT32U Tmp_Addr;/*"��ڲ�����ʱ��"*/
    INT8U Ret;/*"����ֵ"*/
    INT8U TryTime;/*"���Դ���"*/
    INT8U EepId; 
    INT16U DevAddr;
    INT8U Tmp;


    /*"��κϷ����ж�"*/
    Tmp_Addr = (INT32U)(Len + LogicAddr);
    if((Tmp_Addr > EEP_DEV_MAX_ADDR)  ||
        (Len == 0)                    ||
        (pBuf == NULL))
    {
        return FALSE;
    }

    #if defined (EEP_DEV_NUM_1)  /*��һ���ڿ�*/
        DevAddr = (INT16U)LogicAddr;
        EepId = E_EEP_DEV_1;    
    #elif defined (EEP_DEV_NUM_2) /*����2���ڿ�*/
        EepId= EEP_LogicAddrConvert(LogicAddr,&DevAddr);
    #endif
    
    #if defined (EEP_DEV_NUM_2) /*��2���ڿ�*/
    /*��֧�ֿ��豸����*/
    if(DevAddr + Len > stEepDrvInfo[EepId].DevMaxAddr)
    {
        return FALSE;
    }
    #endif

    /*"�رյ������ж�"*/
    MCU_PwrChkCloseINT();

    Ret = FALSE;
    /*"���ݴ�����"*/
    for(TryTime=0;TryTime < EEP_TRY_TIME ;TryTime++)
    {
        if(TryTime > 0)
        {
            EEP_Reset();
        }
        IIC_Stop(IIC_ID_EEP);
        IIC_Start(IIC_ID_EEP);
        if(TRUE == EEP_Addressing(EepId,DevAddr))
        {
            IIC_Start(IIC_ID_EEP);
            Tmp = (stEepDrvInfo[EepId].SlaveAddr)|EEP_READ;
            Ret = IIC_SendChar(IIC_ID_EEP,&Tmp,1); 
            if(Ret == TRUE)
            {
                IIC_ReadChar(IIC_ID_EEP, pBuf, Len);

            }
        }
        else
        {
        }
        IIC_Stop(IIC_ID_EEP);
        
        if(Ret == TRUE)
        {
            break;
        }
    }
    /*"�����������ж�"*/
    MCU_PwrChkOpenINT();

    EEP_RwErrCount(Ret);
    return Ret;
}
    
    

/*"************************************************			
Function: 	EEP_Write		
Description: ��EEPROMָ���洢��ַдָ�����ȵ�����
             �� Puf= _NULLʱ��д������Ϊ0 					
Input:  Addr��EEPROM���ݴ洢��ַ,�ӵ�ַ0��ʼ
        pBuf�����ݶ�ȡ���ݻ����ַָ��
        len����ȡ���ݳ���
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 
   |start|Addressing|Write one page end|ACK|Addressing|stop  	    	
************************************************"*/	

INT8U EEP_Write(EEP_ADDR_TYPE LogicAddr, INT8U *pBuf, INT16U Len)
{
    INT32U Tmp_Addr;/*"��ڲ�����ʱ��"*/
    INT8U Ret;/*"����ֵ"*/
    INT8U TryTime;/*"���Դ���"*/
    INT16U Count;/*"�ֽڴ������"*/
    INT8U EepId; 
    INT16U DevAddr;
    INT8U PageSize;
    INT8U NullData;

    NullData = 0;
    /*"��κϷ����ж�"*/
    Tmp_Addr = Len + LogicAddr;
    if((Tmp_Addr > EEP_DEV_MAX_ADDR) ||
       (Len == 0))
    {
        return FALSE;
    }
    
    #if defined (EEP_DEV_NUM_1)  /*��һ���ڿ�*/
        DevAddr = (INT16U)LogicAddr;
        EepId = E_EEP_DEV_1;    
    #elif defined (EEP_DEV_NUM_2) /*����2���ڿ�*/
        EepId= EEP_LogicAddrConvert(LogicAddr,&DevAddr);
    #endif
    
    #if defined (EEP_DEV_NUM_2)  /*��2���ڿ�*/
    /*��֧�ֿ��豸����*/
    if(DevAddr + Len > stEepDrvInfo[EepId].DevMaxAddr)
    {
        return FALSE;
    }
    #endif
    
    PageSize = stEepDrvInfo[EepId].PageSize;
        
    /*"�رյ������ж�"*/
    MCU_PwrChkCloseINT();

    EEP_WP_En(FALSE);
    /*"���ݴ���"*/

    Ret = FALSE;
    for(TryTime=0;TryTime < EEP_TRY_TIME;TryTime++)
    {
        if(TryTime > 0)
        {
            MCU_DelayMs(10);
            EEP_Reset();
        }
        else
        {
        }
        IIC_Stop(IIC_ID_EEP);
        IIC_Start(IIC_ID_EEP);
        Ret = EEP_Addressing(EepId,DevAddr);
        if(Ret == TRUE)
        {
            for(Count = 0; Count < Len; Count++)
            {
                if(pBuf != NULL)
                {
                    Ret = IIC_SendChar(IIC_ID_EEP,pBuf+Count,1);
                }
                else
                {
                    Ret = IIC_SendChar(IIC_ID_EEP,&NullData,1);
                }
                /*"��ҳ�������ﵽҳβʱ"*/
                if((Ret== TRUE) && (((DevAddr+1+Count) % PageSize)== 0))
                {
                    IIC_Stop(IIC_ID_EEP);/*"ֹͣ��һҳ��д����"*/
                    Ret = EEP_AckPoll(EepId);/*"�ȴ���һҳд��"*/
                    if((Ret == TRUE) && (Count != (Len-1)))/*"�������һ���ֽ�"*/
                    {
                        IIC_Start(IIC_ID_EEP);
                        Ret = EEP_Addressing(EepId,DevAddr+Count+1);
                    }
                    else
                    {
                    }
                }
                else
                {

                }
                if(Ret == FALSE)
                {
                    break;
                }
            }
        }
        IIC_Stop(IIC_ID_EEP);/*"���һ���ֽڲ���ҳβ����Ҫֹͣҳд����"*/
        if(Ret == TRUE)
        {
            Ret = EEP_AckPoll(EepId);/*"�ȴ�ҳд���"*/
            IIC_Stop(IIC_ID_EEP);
            break;
        }

    }

    EEP_WP_En(TRUE);

    /*"�����������ж�"*/
    MCU_PwrChkOpenINT();

    EEP_RwErrCount(Ret);
    return Ret;
}


/*"************************************************			
Function: 	EEP_GetErrSta		
Description: ����EEP�豸�Ƿ�Ϊ����״̬
             ��EEP��дʱ ��������EEP_ERR_TIMES��,���ж�ΪEEP����					
Input:  
Return: TRUE  ����״̬  _FALSE  ����״̬		
Others: 
   	    	
************************************************"*/	
INT8U EEP_GetErrSta(void)
{
    if(mEepRwErrTimes >EEP_ERR_TIME)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}


/*"********************************************************
    The  interface of EEPROM to the upper layer end
*********************************************************"*/
