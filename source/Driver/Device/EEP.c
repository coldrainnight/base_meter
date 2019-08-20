/*"***********************************************************		
FileName: EEP.c		
Author: 邹杨       
Version : 1.0       Date: 2016.12.27	
Description: eeprom设备层驱动
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
    #if defined (EEP_DEV_NUM_2)  /*有一块内卡*/
    E_EEP_DEV_2,
    #endif
}ENUM_EEP_ID;

/*页写大小定义*/
#define EEP_32KB_PAGESIZE  (64)
#define EEP_64KB_PAGESIZE  (128)

/*EEPROM读写操作*/
#define EEP_WRITE    (0xFE)     /*"EEP设备写，与slave_addr执行 与 操作"*/
#define EEP_READ     (0x01)     /*"EEP设备读，与slave_addr执行 或 操作"*/


/**/
#define EEP_ERR_TIME (100)    /*"判断EEP错误的阀值"*/
#define EEP_TRY_TIME  (3)       /*"定义EEP设备错误尝试次数"*/ 
#define EEP_POLL_TIME (30)     /*EEP ACK 等待最大重试次数*/



static INT16U mEepRwErrTimes ;   /*"EEp读写错误次数"*/

const ST_EEP_DRV stEepDrvInfo[]=
{
    {
        EEP_SLAVE_ADDR_1 , /*从设备地址*/
        EEP_64KB_MAX_ADDR,/*设备容量最大可读写地址*/
        EEP_64KB_PAGESIZE,/*EEP设备最大页写限制*/
    },
  
    #if defined (EEP_DEV_NUM_2) /*存在2块内卡*/
    {
        EEP_SLAVE_ADDR_2 , /*从设备地址*/
        EEP_DEV_2_MAX_ADDR,/*设备最大可读写地址*/
        EEP_32KB_PAGESIZE,/*EEP设备最大页写限制*/
    },
    #endif
};


static INT8U EEP_Addressing(INT8U EepId,INT16U DevAddr);
static INT8U EEP_AckPoll (INT8U EepId);
#if defined (EEP_DEV_NUM_2) /*有一块内卡*/
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
    if(LogicAddr <= stEepDrvInfo[E_EEP_DEV_1].DevMaxAddr) /*地址在EEP DEV1 区间*/
    {
        *DevAddr = (INT16U)LogicAddr;
        EepId =  E_EEP_DEV_1;
    }
    else /*地址在EEP Dev2 区间*/
    {
        *DevAddr = (INT16U)(LogicAddr - stEepDrvInfo[E_EEP_DEV_1].DevMaxAddr);
        EepId =  E_EEP_DEV_2;
    }
    return EepId;

}

#endif


/*"************************************************			
Function: 	RwErrCount		
Description: EEP读写错误累加次数计数
Input:  EEP读写的返回值，不包括EEP读写输入参数的错误
Output: 无	
Return: TRUE:等待页写完成OK  FALSE:等待页写超时 
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
Description: 当EEPROM内部页写开始时，需要等待页写完成执行后续动作
             最长等待时间为6ms
Input:  无
Output: 无	
Return: TRUE:等待页写完成OK  FALSE:等待页写超时 
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
Description: 控制EEP的写保护引脚
Input:  State : TRUE 置为写保护开启 FALSE 关闭写保护
Output: 无	
Return: 无
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
Description: EEPROM设备复位			
Input:  无
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/	
static void EEP_Reset(void)
{
    IIC_Init(IIC_ID_EEP);
    IIC_Stop(IIC_ID_EEP);
    /*需要根据芯片手册查看是否有相关复位时序*/
    
}

/*"************************************************			
Function: 	EEP_Addressing		
Description: EEP当前指针指向addr位置，读写操作都需要这个操作					
Input:  addr：EEPROM数据存储地址，最大支持16位
Output: 无	
Return: TRUE  成功  FALSE  失败		
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




/*目前只区分32K 和 64K,其他的未做
返回TRUE 表示大小和配置一致，FALSE为不一致
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

    /*内卡大小检测程序只支持32KB 和 64KB eep的检测*/
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
        /*说明已经回滚*/
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
Description: EEP初始化 获取EEP的参数					
Input: 
Output: 无	
Return: 	
Others: 
************************************************"*/	
void EEP_Init(void)
{
    IIC_Init(IIC_ID_EEP);
    IIC_Stop(IIC_ID_EEP);
    EEP_WP_En(TRUE);
    /*"初始化EEP读写错误次数"*/
    mEepRwErrTimes = 0;

}


void EEP_Close(void)
{
    IIC_Close(IIC_ID_EEP);
    EEP_WP_En(TRUE);
}
/*"************************************************			
Function: 	EEP_Read		
Description: 从EEPROM指定存储地址读取指定长度的数据 					
Input:  addr：EEPROM数据存储地址
        pBuf：数据读取数据缓存地址指针
        len：读取数据长度
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 
  |start|slave addr(W)|1st wordaddr|2nd wordaddr|start|slave addr(R)| Read n byte|stop
************************************************"*/	

INT8U EEP_Read(EEP_ADDR_TYPE LogicAddr, INT8U *pBuf, INT16U Len)
{
    INT32U Tmp_Addr;/*"入口参数暂时量"*/
    INT8U Ret;/*"返回值"*/
    INT8U TryTime;/*"重试次数"*/
    INT8U EepId; 
    INT16U DevAddr;
    INT8U Tmp;


    /*"入参合法性判断"*/
    Tmp_Addr = (INT32U)(Len + LogicAddr);
    if((Tmp_Addr > EEP_DEV_MAX_ADDR)  ||
        (Len == 0)                    ||
        (pBuf == NULL))
    {
        return FALSE;
    }

    #if defined (EEP_DEV_NUM_1)  /*有一块内卡*/
        DevAddr = (INT16U)LogicAddr;
        EepId = E_EEP_DEV_1;    
    #elif defined (EEP_DEV_NUM_2) /*存在2块内卡*/
        EepId= EEP_LogicAddrConvert(LogicAddr,&DevAddr);
    #endif
    
    #if defined (EEP_DEV_NUM_2) /*有2块内卡*/
    /*不支持跨设备操作*/
    if(DevAddr + Len > stEepDrvInfo[EepId].DevMaxAddr)
    {
        return FALSE;
    }
    #endif

    /*"关闭掉电检测中断"*/
    MCU_PwrChkCloseINT();

    Ret = FALSE;
    /*"数据传输入"*/
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
    /*"开启掉电检测中断"*/
    MCU_PwrChkOpenINT();

    EEP_RwErrCount(Ret);
    return Ret;
}
    
    

/*"************************************************			
Function: 	EEP_Write		
Description: 从EEPROM指定存储地址写指定长度的数据
             当 Puf= _NULL时，写入数据为0 					
Input:  Addr：EEPROM数据存储地址,从地址0开始
        pBuf：数据读取数据缓存地址指针
        len：读取数据长度
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 
   |start|Addressing|Write one page end|ACK|Addressing|stop  	    	
************************************************"*/	

INT8U EEP_Write(EEP_ADDR_TYPE LogicAddr, INT8U *pBuf, INT16U Len)
{
    INT32U Tmp_Addr;/*"入口参数暂时量"*/
    INT8U Ret;/*"返回值"*/
    INT8U TryTime;/*"重试次数"*/
    INT16U Count;/*"字节传输个数"*/
    INT8U EepId; 
    INT16U DevAddr;
    INT8U PageSize;
    INT8U NullData;

    NullData = 0;
    /*"入参合法性判断"*/
    Tmp_Addr = Len + LogicAddr;
    if((Tmp_Addr > EEP_DEV_MAX_ADDR) ||
       (Len == 0))
    {
        return FALSE;
    }
    
    #if defined (EEP_DEV_NUM_1)  /*有一块内卡*/
        DevAddr = (INT16U)LogicAddr;
        EepId = E_EEP_DEV_1;    
    #elif defined (EEP_DEV_NUM_2) /*存在2块内卡*/
        EepId= EEP_LogicAddrConvert(LogicAddr,&DevAddr);
    #endif
    
    #if defined (EEP_DEV_NUM_2)  /*有2块内卡*/
    /*不支持跨设备操作*/
    if(DevAddr + Len > stEepDrvInfo[EepId].DevMaxAddr)
    {
        return FALSE;
    }
    #endif
    
    PageSize = stEepDrvInfo[EepId].PageSize;
        
    /*"关闭掉电检测中断"*/
    MCU_PwrChkCloseINT();

    EEP_WP_En(FALSE);
    /*"数据传输"*/

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
                /*"翻页动作，达到页尾时"*/
                if((Ret== TRUE) && (((DevAddr+1+Count) % PageSize)== 0))
                {
                    IIC_Stop(IIC_ID_EEP);/*"停止上一页的写动作"*/
                    Ret = EEP_AckPoll(EepId);/*"等待上一页写完"*/
                    if((Ret == TRUE) && (Count != (Len-1)))/*"不是最后一个字节"*/
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
        IIC_Stop(IIC_ID_EEP);/*"最后一个字节不是页尾，需要停止页写动作"*/
        if(Ret == TRUE)
        {
            Ret = EEP_AckPoll(EepId);/*"等待页写完成"*/
            IIC_Stop(IIC_ID_EEP);
            break;
        }

    }

    EEP_WP_En(TRUE);

    /*"开启掉电检测中断"*/
    MCU_PwrChkOpenINT();

    EEP_RwErrCount(Ret);
    return Ret;
}


/*"************************************************			
Function: 	EEP_GetErrSta		
Description: 返回EEP设备是否为错误状态
             当EEP读写时 连续错误EEP_ERR_TIMES次,就判定为EEP错误					
Input:  
Return: TRUE  正常状态  _FALSE  错误状态		
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
