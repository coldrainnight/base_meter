#include "Driver\DriverComm.h"
#include "system\Sys_def.h"
#include "APP\LegallyRelevant\MU_LIB.h"
#include "APP\LegallyRelevant\LRM.h"

//#include "Driver\Device\MU_IC.h"


/*"***********************************************************  
FileName: MU_IC.c  
Author: 韩潇俊
Version : 1.0       Date: 2017.11.13
Description:计量运行模块,
用于计量的初始化，运行，瞬时量数据读取，安全判断以及电量传递
		   用于RN8209C计量芯片
Function List:     
<FunctionName>  
<author> <time> <version > <desc>  
 
***********************************************************"*/

/*"*****************************************************************
                    MeasureChip Uart device
*********************************************************************"*/
//static INT8U CheckSum(INT8U cmd,INT8U* data,INT8U length);
static void MU_IC_Wait(INT8U Mode);
static void MU_IC_Transmit(void);
static void MU_IC_Receice(void);


INT8U MU_IC_WriteReg(INT16U reg,INT8U *data,INT8U length);
INT8U MU_IC_ReadReg(INT16U reg,INT8U length);
void MU_IC_Init(INT8U mode);
void MC_Reset(void);
//INT8U MU_IC_SetPulse (INT32U pluse);
INT8U  MU_IC_PulseInit(INT8U device, INT32U const,INT8U  pulseWidth);
INT8U MU_IC_ReadPulse(INT8U *pdata);
void MC_OpenPulse(void);
void MC_ClosePulse(void);
void MU_IC_PD_Close(void);

/*"*****************************************************************
                    Pluse in detect device
*********************************************************************"*/
static INT8U MU_IC_GetPulsePIn(void);
static INT8U MU_IC_Get_Q_PulsePIn(void);
static void MU_IC_PulseIn_ISR(void);
//static void MU_IC_Q_PulseIn_ISR(void);
static void MU_IC_PulseIn_Init(void);

INT16U MU_IC_GetPulse_Val(void);
void MU_IC_PluseAffirm(void);
INT16U MU_IC_GetNoAffrimPulse_Val(void);
void MU_IC_ClrPulse(void);
void MU_IC_Clr_Q_Pulse(void);
/*"*****************************************************************
                    MeasureChip Uart device
*********************************************************************"*/
MU_IC_UART gMeaIc_Uart;
/*"*****************************************************************
                    Pluse in detect device
*********************************************************************"*/
static volatile INT16U PluseIntNum;/*"收到脉冲后，不经过防抖，计数"*/
static volatile INT16U PluseNum; /*"收到脉冲，并防抖后，在中断中+1,上电初始化为0"*/
static volatile INT8U  PluseIsrFlag;/*"中断检测到脉冲，置位标志，防抖确认后，清除标志，初始化为FALSE"*/
static INT8U  PluseBeginFlag;/*"防抖开始计时标志,初始化为FALSE"*/
static INT16U PluseBeginTime;/*"防抖开始计时时间"*/

static volatile INT16U Q_PluseIntNum;/*"收到脉冲后，不经过防抖，计数"*/
static volatile INT16U Q_PluseNum; /*"收到脉冲，并防抖后，在中断中+1,上电初始化为0"*/
static volatile INT8U  Q_PluseIsrFlag;/*"中断检测到脉冲，置位标志，防抖确认后，清除标志，初始化为FALSE"*/
static INT8U  Q_PluseBeginFlag;/*"防抖开始计时标志,初始化为FALSE"*/
static INT16U Q_PluseBeginTime;/*"防抖开始计时时间"*/

/*"************************************************			
Function:MU_IC_Wait
Description: 计量芯片串口等待延时函数 
Input:  Mode 0 读 1写
Output: 	无
Return:无	
Others: 		
************************************************"*/
static void MU_IC_Wait(INT8U Mode)
{
    INT8U *PUartFlag;
    INT16U Time_cnt=0,flag=1;

    if(Mode ==0)/*读*/
    {
        PUartFlag = &gMeaIc_Uart.Uart_Rxc_flag;
    }
    else
    {
        PUartFlag = &gMeaIc_Uart.Uart_Txc_flag;
    }
    
  	while(flag)
  	{
        if((*PUartFlag) == 1 )
        {
      		flag = 0;
        }
		MCU_DelayMs(1);
        Time_cnt++;
        if(Time_cnt >= _MU_IC_WaitTIME)
        {
            flag = 0;
        }
  	}
}
/*"************************************************			
Function:MU_IC_Transmit
Description: 计量芯片数据发送函数 
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
static void MU_IC_Transmit(void)
{
    gMeaIc_Uart.Uart_recent_length ++;
    if(gMeaIc_Uart.Uart_recent_length != gMeaIc_Uart.Uart_Tx_length)
	{	    	
        //UART1->TXREG = gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length];
        UART_Send1Byte(E_UART_ID_MC,gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length]);
    }
    else
    {
        gMeaIc_Uart.Uart_recent_length = 0;
        gMeaIc_Uart.Uart_Txc_flag = 1;
        UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SC_RO);
        
    }
}
/*"************************************************			
Function:MU_IC_Receice
Description: 计量芯片数据接收函数 
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
static void MU_IC_Receice(void)
{
	gMeaIc_Uart.Uart_Rx_Data[gMeaIc_Uart.Uart_recent_length] = UART_Receive1Byte(E_UART_ID_MC);//UART1->RXREG;
    gMeaIc_Uart.Uart_recent_length++;
    if(gMeaIc_Uart.Uart_recent_length == gMeaIc_Uart.Uart_Rx_length)
	{
	    gMeaIc_Uart.Uart_recent_length = 0;
  		gMeaIc_Uart.Uart_Rx_length = 0;
        gMeaIc_Uart.Uart_Rxc_flag = 1;
        UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SC_RC);
	}
}
/*"************************************************			
Function:UART_WriteData
Description:串口写操作函数
Input:  reg 寄存器地址 *data 数据 length 长度
Output: 	无
Return:无	
Others: 		
************************************************"*/
INT8U MU_IC_WriteReg(INT16U reg,INT8U *data,INT8U length)
{
    INT8U i,ret = FALSE;
    INT16U crc16;
    /*"写操作 数据帧 = 命令码 + 数据码 + 数据格式（0x00）+ 数据长度 + 数据（2） + crc检验高 + crc校验低"*/  
    gMeaIc_Uart.Uart_Tx_Data[0] = (INT8U)(reg>>8);
    gMeaIc_Uart.Uart_Tx_Data[1] = (INT8U)(reg>>0);
  	gMeaIc_Uart.Uart_Tx_Data[2] = 0x00;
    gMeaIc_Uart.Uart_Tx_Data[3] = length;

    for(i=0;i<length;i++)
    {
        gMeaIc_Uart.Uart_Tx_Data[i+4] = data[i];
    }

    crc16 =  CRC16((INT8U*)&gMeaIc_Uart.Uart_Tx_Data, 4+length);
    gMeaIc_Uart.Uart_Tx_Data[length+4] = (INT8U)(crc16>>8);
    gMeaIc_Uart.Uart_Tx_Data[length+5] = (INT8U)(crc16>>0);
    
  	gMeaIc_Uart.Uart_Tx_length = length+6;
  	gMeaIc_Uart.Uart_Rx_length = 6;
  
  	gMeaIc_Uart.Uart_Rxc_flag=0;
  	gMeaIc_Uart.Uart_Txc_flag=0;
  	gMeaIc_Uart.Uart_recent_length=0;

	for(i=0;i<RX_LENGTH;i++)
  	{
  		gMeaIc_Uart.Uart_Rx_Data[i]=0;/* 清接收变量缓冲*/
  	}

    /*"通讯使能"*/
  	
  	/*"清寄存器缓冲区"*/
    UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SO_RC);
    UART_Send1Byte(E_UART_ID_MC,gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length]);
  	/*"通讯使能"*/

	MU_IC_Wait(0);

    if(gMeaIc_Uart.Uart_Rx_Data[0] != (gMeaIc_Uart.Uart_Tx_Data[0]+0x80))
    {
        crc16 =  CRC16((INT8U*)&gMeaIc_Uart.Uart_Rx_Data, 4);
        if( (gMeaIc_Uart.Uart_Rx_Data[4] == (INT8U)(crc16>>8)) && (gMeaIc_Uart.Uart_Rx_Data[5] = (INT8U)(crc16>>0)) )
        {
            ret = TRUE;
        }
    }
    else
    {
    }
    return ret;
}
/*"************************************************			
Function:MU_IC_ReadReg
Description: 串口读操作数 
Input:  reg 寄存器地址 length 长度
Output: 	无
Return:无	
Others: 		
************************************************"*/
INT8U MU_IC_ReadReg(INT16U reg,INT8U length)
{
    INT8U i,ret = FALSE;
    INT16U crc16;
    /*读操作 数据帧 = 命令码（0x20 参数 0x21 数据） + 数据码 + 数据格式（0x00）+ 数据长度（0） + 数据（空） + crc检验高 + crc校验低"*/  
    gMeaIc_Uart.Uart_Tx_Data[0] = (INT8U)(reg>>8);
    gMeaIc_Uart.Uart_Tx_Data[1] = (INT8U)(reg>>0);
  	gMeaIc_Uart.Uart_Tx_Data[2] = 0x00;
    gMeaIc_Uart.Uart_Tx_Data[3] = 0x00;

    crc16 =  CRC16((INT8U*)&gMeaIc_Uart.Uart_Tx_Data, 4);
    gMeaIc_Uart.Uart_Tx_Data[4] = (INT8U)(crc16>>8);
    gMeaIc_Uart.Uart_Tx_Data[5] = (INT8U)(crc16>>0);
    
  	gMeaIc_Uart.Uart_Tx_length = 6;
  	gMeaIc_Uart.Uart_Rx_length = (INT8U)(length+6);
  
  	gMeaIc_Uart.Uart_Rxc_flag=0;
  	gMeaIc_Uart.Uart_Txc_flag=0;
  	gMeaIc_Uart.Uart_recent_length=0;

	for(i=0;i<RX_LENGTH;i++)
  	{
  		gMeaIc_Uart.Uart_Rx_Data[i]=0;/* 清接收变量缓冲*/
  	}

    /*"通讯使能"*/
  	
  	/*"清寄存器缓冲区"*/
    UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SO_RC);
    UART_Send1Byte(E_UART_ID_MC,gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length]);
  	/*"通讯使能"*/

	MU_IC_Wait(0);

    if(gMeaIc_Uart.Uart_Rx_Data[0] != (gMeaIc_Uart.Uart_Tx_Data[0]+0x80))
    {
        crc16 =  CRC16((INT8U*)&gMeaIc_Uart.Uart_Rx_Data, (length+4));
        if( (gMeaIc_Uart.Uart_Rx_Data[length+4] == (INT8U)(crc16>>8)) && (gMeaIc_Uart.Uart_Rx_Data[length+5] = (INT8U)(crc16>>0)) )
        {
            ret = TRUE;
        }
    }
    else
    {
        
    }
    return ret;
}


/*"************************************************			
Function: MU_IC_Init		
Description: 与计量芯片相关端口初始化，当采用SOC方式时包括相关寄存器的配置				
Input:  addr：Mode：功耗模式  0为上电初始化，1为低功耗初始化
Output: 无	
Return: 无			
Others: 		
************************************************"*/	
void MU_IC_Init(INT8U mode)
{
	if( mode ==  PWRMODE_UP)
	{	 
		ST_UARTCONFIG UartConfig; 
	
        UartConfig.Baud = E_BAUD9600;//E_BAUD38400;//E_BAUD9600
		UartConfig.UartForm.byte = UARTFORM_8DATA_EVEN_1STOP;
		UartConfig.TranSta.byte = TRANSTA_SO_RC;
        UartConfig.SendIsr = MU_IC_Transmit;
        UartConfig.ReceiveIsr = MU_IC_Receice; 
		UART_Open(E_UART_ID_MC,&UartConfig);
        MU_IC_PulseIn_Init();
        MU_IC_ClrPulse();
        MU_IC_Clr_Q_Pulse();
        MC_OpenPulse();
	}
}
/*"************************************************			
Function:MC_Reset
Description:复位计量芯片函数
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
void MC_Reset(void)
{
	MC_ClosePulse();
	/*""配置芯片复位IO口"*/
	GPIO_ConfigPin(MSUR_CHIP_RST,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(MSUR_CHIP_RST,1);
	MCU_DelayMs(25);
	GPIO_OutputPin(MSUR_CHIP_RST,0);
	MCU_DelayMs(40);
	GPIO_OutputPin(MSUR_CHIP_RST,1);
	MCU_DelayMs(25);
	//GPIO_SetMulti(MC_TXD,GPIO_Mode_DIG);
}

/*""************************************************			
Function: MU_IC_SetPulse		
Description: 通信设置计量脉冲常数接口				
Input:       device是区分有功、无功 constRatio用于高频脉冲分频（单相不需要分，固定为1）pulseWidth脉冲宽度
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/	
INT8U  MU_IC_PulseInit(INT8U device, INT32U pulseconst,INT8U  pulseWidth)
{
	INT8U RetVal = TRUE;

    if(device == 0)/*“有功脉冲”*/
    {
	    if( (pulseconst >= 100) && (pulseconst <= 51200) )
    	{
    		if(pulseconst != gMU_Para.ram_adj_data.Ram_ecconst)/*""只在脉冲常数改变的时候才改写能量门限值"*/
    		{
    		    if((gMU_Para.CRC_Val == CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val))) && (gMU_Para.MC_DefPara_Flag ==EEP_PAR_IS_INIT))/*""只在crc正确的时候、且已校表，才重新计算新的脉冲常数"*/
                {      
    			    gMU_Para.ram_adj_data.Ram_hfconst = (INT16U)((INT32U)gMU_Para.ram_adj_data.Ram_hfconst *(INT32U)gMU_Para.ram_adj_data.Ram_ecconst / pulseconst );
    			    gMU_Para.ram_adj_data.Ram_ecconst = (INT16U)pulseconst;
                    MU_IC_WriteReg(W_HFCNST,(INT8U  *)&gMU_Para.ram_adj_data.Ram_hfconst,2);
    		        gMU_Para.CRC_Val = CRC16((INT8U *)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
                    
                    if(gMU_Para.MC_DefPara_Flag == EEP_PAR_IS_INIT)/*""只在参数为已校准的条件下，才写内卡，默认参数或错误参数不能写内卡"*/
                    {      
                        FS_WriteFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
                        FS_WriteFile(E_FILE_LRM,MU_PARA_BAK2_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
                    }
               }
    		}
    	}
	    else
    	{
    		RetVal = FALSE;
    	}
    }

	return RetVal;
} 
/*""************************************************			
Function: MU_IC_ReadPulse		
Description: 传输计量脉冲常数到输入指针	
Input:    pdata      输入参数指针
Output: 无	
Return: 返回读标志0xAA	
Others: 		
************************************************"*/	
INT8U MU_IC_ReadPulse(INT8U *pdata)
{
	INT32U ec_const;

	ec_const = gMU_Para.ram_adj_data.Ram_ecconst;

	//LIB_LongToBcd((INT8U *)&ec_const, pdata);
    LIB_MemCpy((INT8U *)&ec_const, pdata, sizeof(INT32U));
    return 0xAA;

}
/*"************************************************			
Function:MC_OpenPulse
Description: 打开计量芯片脉冲口
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
void MC_OpenPulse(void)
{
	/*""配置功率脉冲输出控制IO口"*/
	GPIO_ConfigPin(PULSE_CTL,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(PULSE_CTL,0);//打开脉冲"*/  
}
/*"************************************************			
Function:MC_ClosePulse
Description: 关闭计量芯片脉冲口
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
void MC_ClosePulse(void)
{
	/*""配置功率脉冲输出控制IO口"*/
	GPIO_ConfigPin(PULSE_CTL,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(PULSE_CTL,1);/*"关闭脉冲"*/  
}

/*"************************************************			
Function:MU_IC_PD_Close
Description: 关闭计量芯片脉冲口
Input:  无
Output: 	无
Return:无	
Others: 		
************************************************"*/
void MU_IC_PD_Close(void)
{
    GPIO_SetMulti(MC_TXD, GPIO_Mode_OUT);
    GPIO_SetMulti(MC_RXD, GPIO_Mode_OUT);
    GPIO_OutputPin(MC_TXD,LOW);
    GPIO_OutputPin(MC_RXD,LOW); 
}

/*"************************************************			
Function: 	GetPulsePIn		
Description: 获取计量脉冲输入端口状态		
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
static INT8U MU_IC_GetPulsePIn(void)
{
    return GPIO_ReadPin(LED_PULSE_A);
}

/*"************************************************			
Function: 	PulseIn_ISR		
Description: 计量脉冲中断处理函数		
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
static void MU_IC_PulseIn_ISR(void)
{
    if(LOW == MU_IC_GetPulsePIn())  
    {
        PluseIntNum ++;
        PluseIsrFlag = TRUE;
    }
    else
    {
    }
}
/*"************************************************			
Function: 	PulseIn_Init		
Description: 初始化计量脉冲输入检测		
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
static void MU_IC_PulseIn_Init(void)
{
    ST_GPIO_EXTICONF TmpExtiConf;
    TmpExtiConf.PinId = LED_PULSE_A;
    TmpExtiConf.TrigModule = GPIO_EXTI_EDGE_FALLING;
    TmpExtiConf.IntFuction= MU_IC_PulseIn_ISR;
    GPIO_EXTI_Enable(&TmpExtiConf);

	
#if 0
    TmpExtiConf.PinId = LED_PULSE_R;
    TmpExtiConf.TrigModule = GPIO_EXTI_EDGE_FALLING;
    TmpExtiConf.IntFuction= MU_IC_Q_PulseIn_ISR;
    GPIO_EXTI_Disable(&TmpExtiConf);
	#endif
}

/*"***************************************************************
    The interface of "Pluse in detect" to the upper layer start
****************************************************************"*/

/*"************************************************			
Function: 	GP_GetPulse_Val		
Description: 在主循环中调用，返回脉冲数，并清零
             由于可能在赋值后，马上进入中断，plusenum++后,继续清零，
             这样就会丢脉冲，所以在此处关闭总中断
Input:  
Output: 脉冲数
Return: 在两次调用之间的脉冲数
Others: 		
************************************************"*/	
INT16U MU_IC_GetPulse_Val(void)
{
    INT16U Tmp;
    MCU_IntDisableAll();/*"关总中断"*/
    Tmp = PluseNum ;
    PluseNum = 0;
    MCU_IntEnableAll();/*"开中断"*/
    return Tmp;
}

/*"************************************************			
Function: 	GP_GetNoAffrimPulse_Val		
Description: 上层获取未防抖的脉冲数，用于防飞走
Input:  
Output: 未防抖的脉冲数
Return: 
Others: 		
************************************************"*/	
INT16U MU_IC_GetNoAffrimPulse_Val(void)
{
    INT16U Tmp;
    Tmp = PluseIntNum ; 
    PluseIntNum = 0;
    return Tmp;
}

/*"************************************************			
Function: 	MU_IC_ClrPulse		
Description: 上层获取清脉冲开始标志
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
void MU_IC_ClrPulse(void)
{
    PluseIntNum = 0;/*"收到脉冲后，不经过防抖，计数"*/
    PluseNum = 0; /*"收到脉冲，并防抖后，在中断中+1,上电初始化为0"*/
    PluseIsrFlag = FALSE;/*"中断检测到脉冲，置位标志，防抖确认后，清除标志，初始化为FALSE"*/
    PluseBeginFlag = FALSE;/*"防抖开始计时标志,初始化为FALSE"*/
    PluseBeginTime = 0;/*"防抖开始计时时间"*/
}

/*"************************************************			
Function: 	GP_GetPulse_Val		
Description: 在中断调用(如10ms定时中断),确认脉冲,并累加
Input:  
Output: 
Return: 
Others: 
************************************************"*/	
void MU_IC_PluseAffirm(void)
{
    if(PluseIsrFlag == TRUE)
    {
        if(MU_IC_GetPulsePIn() == _PLUSE_DETECT)
        {
            if(PluseBeginFlag == FALSE)
            {
                PluseBeginFlag = TRUE;
                PluseBeginTime = MCU_GetTick();
            }
            else
            {
                if(TRUE == MCU_CmpTick(PluseBeginTime,(INT16U)_PLU_AFFIRM_TIME))
                {         
                    PluseNum++;
                    PluseIsrFlag = FALSE;
                    PluseBeginFlag = FALSE;
                }
                else
                {
                    
                }
            }
        }
        else/*"防抖后检测不到脉冲"*/
        {
            PluseIsrFlag = FALSE;
            PluseBeginFlag = FALSE;
        }
    }
    else
    {
    }

    if(Q_PluseIsrFlag == TRUE)
    {
        if(MU_IC_Get_Q_PulsePIn() == _PLUSE_DETECT)
        {
            if(Q_PluseBeginFlag == FALSE)
            {
                Q_PluseBeginFlag = TRUE;
                Q_PluseBeginTime = MCU_GetTick();
            }
            else
            {
                if(TRUE == MCU_CmpTick(Q_PluseBeginTime,(INT16U)_PLU_AFFIRM_TIME))
                {         
                    Q_PluseNum++;
                    Q_PluseIsrFlag = FALSE;
                    Q_PluseBeginFlag = FALSE;
                }
                else
                {
                    
                }
            }
        }
        else/*"防抖后检测不到脉冲"*/
        {
            Q_PluseIsrFlag = FALSE;
            Q_PluseBeginFlag = FALSE;
        }
    }
    else
    {
    }
    
}


/*"************************************************			
Function: 	GetPulsePIn		
Description: 获取计量脉冲输入端口状态		
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
static INT8U MU_IC_Get_Q_PulsePIn(void)
{
    return GPIO_ReadPin(LED_PULSE_R);
}

/*"************************************************			
Function: 	PulseIn_ISR		
Description: 计量脉冲中断处理函数		
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
#if 0
static void MU_IC_Q_PulseIn_ISR(void)
{
    if(LOW == MU_IC_Get_Q_PulsePIn())  
    {
        Q_PluseIntNum ++;
        Q_PluseIsrFlag = TRUE;
    }
    else
    {
    }
}
#endif 
/*"***************************************************************
    The interface of "Pluse in detect" to the upper layer start
****************************************************************"*/

/*"************************************************			
Function: 	MU_IC_Get_Q_Pulse_Val		
Description: 在主循环中调用，返回脉冲数，并清零
             由于可能在赋值后，马上进入中断，plusenum++后,继续清零，
             这样就会丢脉冲，所以在此处关闭总中断
Input:  
Output: 脉冲数
Return: 在两次调用之间的脉冲数
Others: 		
************************************************"*/	
INT16U MU_IC_Get_Q_Pulse_Val(void)
{
    INT16U Tmp;
    MCU_IntDisableAll();/*"关总中断"*/
    Tmp = Q_PluseNum ;
    Q_PluseNum = 0;
    MCU_IntEnableAll();/*"开中断"*/
    return Tmp;
}

/*"************************************************			
Function: 	MU_IC_Get_Q_NoAffrimPulse_Val		
Description: 上层获取未防抖的脉冲数，用于防飞走
Input:  
Output: 未防抖的脉冲数
Return: 
Others: 		
************************************************"*/	
INT16U MU_IC_Get_Q_NoAffrimPulse_Val(void)
{
    INT16U Tmp;
    Tmp = Q_PluseIntNum ; 
    Q_PluseIntNum = 0;
    return Tmp;
}

/*"************************************************			
Function: 	MU_IC_Clr_Q_Pulse		
Description: 上层获取清脉冲开始标志
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
void MU_IC_Clr_Q_Pulse(void)
{
    Q_PluseIntNum = 0;/*"收到脉冲后，不经过防抖，计数"*/
    Q_PluseNum = 0; /*"收到脉冲，并防抖后，在中断中+1,上电初始化为0"*/
    Q_PluseIsrFlag = FALSE;/*"中断检测到脉冲，置位标志，防抖确认后，清除标志，初始化为FALSE"*/
    Q_PluseBeginFlag = FALSE;/*"防抖开始计时标志,初始化为FALSE"*/
    Q_PluseBeginTime = 0;/*"防抖开始计时时间"*/
}

