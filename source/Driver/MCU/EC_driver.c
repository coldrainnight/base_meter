/*"*****************************************************************************"*/
/*"  FileName: Ec_driver.c"*/
/*"  Description:     该文件实现ISO7816 CPU卡底层驱动"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/

#include "Driver\DriverComm.h"
#include "Driver\Device\Ec.h"


EcRevIsr mEC_RevIsr;

static void EC_CClkCtl(INT8U output);
static void EC_CVccCtl(INT8U output);
static void EC_CRstCtl(INT8U output);
static void EC_CPwrOff(void);
static void EC_EVccCtl(INT8U output);

#if 0
static void EC_ERstCtl(INT8U output);
#endif
static void UART_7816_INT(void);

/*"************************************************			
Function:EC_UartInit
Description: UART或7816数据接口的初始化
Input:  None
Output: None
Return: None
Others: None	
************************************************"*/	
void EC_UartInit(void)
{   
	EC_UartRls();

	MCU_PeriphClkSet(U78161_num, ENABLE);
	
	/*"7816寄存器初始化"*/
	U78161->U7816FRC &= 0x0000;
	U78161->U7816FRC |= 0x00000044;/*"偶校验、自动重复次数3次"*/
	U78161->U7816EGT =0x00;
	U78161->U7816IE  =0x00;

    U78161->U7816ERR  =0x0F;	/*"清标志位，写1清零"*/
    U78161->U7816STA  =0x00;
    U78161->U7816IF   =0x07;

	U78161->U7816FRC &=(~1);	/*"正向编码，正向传输"*/

	U78161->U7816CLKDIV = 1;	/*"频率配置2分帧 4MHZ"*/
    U78161->U7816PDIV	=	0x173;	/*"1ETU 为372CLK"*/

	U78161->U7816CTRL &=(~2); /*"7816通道强上拉使能"*/	

	U78161->U7816CTRL &=(~0x10); /*"接收使能模块打开"*/
	U78161->U7816CTRL |=0x10;

	U78161->U7816CTRL &=(~0x20); /*"发送使能模块打开"*/
	U78161->U7816CTRL |=0x20;

	INT_VectSet(E_INT_ID_U78161, UART_7816_INT);
	INT_En(E_INT_ID_U78161);
	
	EC_Dly(6);

}

/*"************************************************			
Function:EC_UartRls
Description: 7816释放，ESAM和CPU卡端口释放为普通I/O
Input:  None
Output: None
Return: None
Others: None	
************************************************"*/	
void EC_UartRls(void)
{
    EC_CPwrOff();
	
	U78161->U7816CTRL &=(~0x10); /*"禁止接收模块"*/
	U78161->U7816CTRL |=0x00;

	U78161->U7816CTRL &=(~0x20); /*"禁止发送模块"*/
	U78161->U7816CTRL |=0x00;

	/*"CPU数据端口置为普通I/O，低电平输出"*/
	GPIO_ConfigPin(_CPU_SD,OUTPUT,NORMAL_OUT);
	GPIO_SetMulti(_CPU_SD,GPIO_Mode_OUT);
	GPIO_OutputPin(_CPU_SD,LOW);
	INT_Dis(E_INT_ID_U78161);
}

/*"************************************************			
Function:EC_Dly
Description: 7816数据延时函数
Input:当系统时钟为4M时，_EC_E_DLY_1ETU(93us，即1ETU)   _EC_E_DLY_H_ETU(0.5ETU) 
                                                        _EC_C_DLY_1ETU(93us，即1ETU)  _EC_C_DLY_H_ETU(0.5ETU) 
Output: None
Return: None
Others: 当ESAM和CPU卡时钟频率不同时要
根据当前通道计算延时;
************************************************"*/	
void EC_Dly (INT16U num)
{
    INT8U _ms,j;
    INT16U i;
    
    if(_EC_ID_ESAM == mECCurChn)
	{
        _ms = _EC_E_DLY_1ETU;
	}
	else
	{
        _ms = _EC_C_DLY_1ETU;
	}
    for(i = 0; i < num; i++)
    {
    	for(j = 0 ;j < _ms; j++)
    	{
    		NOP; 
			NOP; 
			NOP; 
    	}
    }
}


/*"************************************************			
Function:EC_CVcc_Ctl
Description: CPU卡的VCC脚输出控制
Input:  信号控制
Output: None
Return: None
Others:受_EC_C_SUP控制
************************************************"*/	
void EC_CVccCtl(INT8U output)
{
    EC_Dly(2);
}


/*"************************************************			
Function:EC_CRstCtl
Description: CPU卡Rst信号输出控制
Input:  信号控制
Output:None
Return: None
Others: 受_EC_C_SUP控制
************************************************"*/
void EC_CRstCtl(INT8U output)
{
	if(_EC_C_RST_OFF == output)
	{
		GPIO_OutputPin(_CPU_RST,LOW);
	}
	else
	{
		GPIO_OutputPin(_CPU_RST,HIGH);
	} 
	EC_Dly(2);
}

/*"************************************************			
Function:EC_CPwrOff
Description: CPU卡下电
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_CPwrOff(void)
{
    EC_CRstCtl(_EC_C_RST_OFF);
    EC_CClkCtl(_EC_C_CLK_OFF);
    EC_CVccCtl(_EC_C_VCC_OFF);
}

/*"************************************************			
Function:EC_CRstCool
Description: 冷复位
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_CRstCool(void)
{
    EC_CRstCtl(_EC_C_RST_OFF);
    EC_CVccCtl(_EC_C_VCC_ON);
    EC_CClkCtl(_EC_C_CLK_ON);
    EC_CRstCtl(_EC_C_RST_ON);
}


/*"************************************************			
Function:EC_CClk_Ctl
Description: CPU卡CLK控制
Input:  信号控制
Output:None
Return: None
Others: 受_EC_C_SUP和_EC_SHARE_OSC控制
************************************************"*/
void EC_CClkCtl(INT8U output)
{
	if( output == _EC_C_CLK_ON)
	{
		U78161->U7816CTRL &=(~0x08); 
		U78161->U7816CTRL |= 0x08;
		EC_Dly(106);
	}
	else
	{
		U78161->U7816CTRL &=(~0x08); 
		U78161->U7816CTRL |= 0x00;

	}

	EC_Dly(2);
}



/*"************************************************			
Function:EC_EVccCtl
Description: ESAM卡VCC控制
Input:  信号控制
Output:None
Return: None
Others: None
************************************************"*/
void EC_EVccCtl(INT8U output)
{
    if(_EC_E_VCC_OFF == output)
	{
		GPIO_OutputPin(SPI_ESAM_POWER,HIGH);
		
	}
	else
	{
		GPIO_OutputPin(SPI_ESAM_POWER,LOW);
	} 

    EC_Dly(4);
}

#if 0
/*"************************************************			
Function:EC_ERstCtl
Description: ESAM卡RST信号控制
Input:  信号控制
Output:None
Return: None
Others: None
************************************************"*/
void EC_ERstCtl(INT8U output)
{
	if(_EC_E_RST_OFF == output)
	{
		GPIO_OutputPin(_ESAM_RST,LOW);
	}
	else
	{
		GPIO_OutputPin(_ESAM_RST,HIGH);
	}
	EC_Dly(2);
}
#endif

/*"************************************************			
Function:EC_EInitIO
Description: ESAM IO 初始化
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_EInitIO(void)
{
	GPIO_SetMulti(SPI_ESAM_POWER, GPIO_Mode_OUT);
	GPIO_SetMulti(SPI_ESAM_CS, GPIO_Mode_OUT);
}

/*"************************************************			
Function:EC_EPwrOff
Description: ESAM下电
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_EPwrOff(void)
{
    EC_EVccCtl(_EC_E_VCC_OFF);
	
	GPIO_ConfigPin(SPI_ESAM_MISO,OUTPUT,NORMAL_OUT);
	GPIO_SetMulti(SPI_ESAM_MISO,GPIO_Mode_OUT);
	GPIO_OutputPin(SPI_ESAM_MISO,LOW);

	GPIO_ConfigPin(SPI_ESAM_MOSI,OUTPUT,NORMAL_OUT);
	GPIO_SetMulti(SPI_ESAM_MOSI,GPIO_Mode_OUT);
	GPIO_OutputPin(SPI_ESAM_MOSI,LOW);

	GPIO_ConfigPin(SPI_ESAM_CLK,OUTPUT,NORMAL_OUT);
	GPIO_SetMulti(SPI_ESAM_CLK,GPIO_Mode_OUT);
	GPIO_OutputPin(SPI_ESAM_CLK,LOW);

	GPIO_ConfigPin(SPI_ESAM_CS,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(SPI_ESAM_CS,LOW);

}

/*"************************************************			
Function:EC_ERstCool
Description: ESAM冷复位
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_ERstCool(void)
{
  	EC_EVccCtl(_EC_E_VCC_ON);
}


/*"************************************************			
Function:EC_ChnSel
Description: ESAM与CPU卡通道切换
(当ESAM和CPU卡共用一个数据通道时有用)
Input:  ECID为需切换的通道(_EC_ID_CPU/_EC_ID_ESAM)
Output:None
Return: None
Others: None
************************************************"*/
void EC_ChnSel(void)
{

	if(_EC_ID_CPU == mECCurChn)
	{
		GPIO_ConfigPin(_CPU_CLK,OUTPUT,NORMAL_OUT);/*"配置CLK PC2口"*/
		GPIO_SetMulti(_CPU_CLK, GPIO_Mode_DIG);

		GPIO_ConfigPin(_CPU_SD,OUTPUT,OPENDRAIN_OUT);/*"配置CPU卡IO PC3口"*/
		GPIO_SetMulti(_CPU_SD, GPIO_Mode_DIG);

		GPIO_ConfigPin(_CPU_RST,OUTPUT,NORMAL_OUT);/*"配置CPU卡复位脚PB5口"*/
		GPIO_SetMulti(_CPU_RST, GPIO_Mode_OUT);
	}

		EC_Dly(20);

}

/*"************************************************			
Function:EC_HardwareInit
Description: 上电时端口配置
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_HardwareInit(INT8U Mode)
{
    if(Mode == MCU_RUNMODE)
    {
		GPIO_ConfigPin(_ESAM_VCC,OUTPUT,NORMAL_OUT);
		GPIO_SetMulti(_ESAM_VCC, GPIO_Mode_OUT);

		GPIO_ConfigPin(_CPU_SW,OUTPUT,NORMAL_OUT);/*"配置SW 初始为输出模式"*/
		GPIO_SetMulti(_CPU_SW, GPIO_Mode_OUT);

		EC_EPwrOff();
    	mEC_RevIsr = EC_RevData;
    	mECCurChn = _EC_ID_OTHER;
    }
    else
    {
    }
}

/*"************************************************			
Function:EC_SupCpuCfg
Description: 根据表型对CPU卡各端口进行配置
Input:  meterType: 表型
        _NOPREPAYMETER      0      非预付费表
        _TELMETER             1      远程表
        _LOCALMETER          2      本地表
Output:None
Return: None
Others: None
************************************************"*/
void EC_SupCpuCfg(INT8U mtrType)
{
	 if(mtrType == LOCALMETER)
	 {
		 GPIO_ConfigPin(_CPU_SW,INPUT,NULL_IN);/*"配置SW为输入模式"*/
		 GPIO_SetMulti(_CPU_SW, GPIO_IN);

		 GPIO_ConfigPin(_CPU_CLK,OUTPUT,NORMAL_OUT);/*"配置CLK PC2口"*/
		 GPIO_SetMulti(_CPU_CLK, GPIO_Mode_OUT);
		 
		 GPIO_ConfigPin(_CPU_SD,OUTPUT,NORMAL_OUT);/*"配置CPU卡IO PC3口"*/
		 GPIO_SetMulti(_CPU_SD, GPIO_Mode_OUT);
		 
		 GPIO_ConfigPin(_CPU_RST,OUTPUT,NORMAL_OUT);/*"配置CPU卡复位脚PB5口"*/
		 GPIO_SetMulti(_CPU_RST, GPIO_Mode_OUT);

		 EC_CPwrOff();
	 }
	 else
	 {
		 GPIO_ConfigPin(_CPU_SW,OUTPUT,NORMAL_OUT);/*"配置SW 初始为输出模式"*/
		 GPIO_SetMulti(_CPU_SW, GPIO_Mode_OUT);
	 } 
}


/*"************************************************			
Function:EC_RevFromCpu
Description: CPU 串口中断程序 接收
Input:  None
Output:None
Return: None
Others: 受_EC_SHARE_SD控制
************************************************"*/
void EC_RevData(INT8U data)
{
	EC_DataToBuf(data);
}


/*"************************************************			
Function:EC_UartEnTx
Description: CPU串口关接收中断，转发送
Input:  None
Output:None
Return: None
Others: None
************************************************"*/
void EC_UartEnTx(void)
{
	U78161->U7816IE &=	~U7816_RX_IE;
	U78161->U7816IE &=	~U7816_TX_IE;
}


/*7816中断*/
static void UART_7816_INT(void)
{
	if(U78161->U7816IF & U7816_RX_IF)
	{
        mEC_RevIsr(U78161->U7816RXBUF);
	}
	
	if(U78161->U7816IF & U7816_ERR_IF)/*"接收错误时，清标志"*/
	{
		U78161->U7816ERR &= 0xFFFF;
	}
}


void EC_UartEnRx(void)
{
	U78161->U7816IE |=	U7816_RX_IE;
	U78161->U7816IE &=	~U7816_TX_IE;
}


void EC_UartTx(INT8U T_data)
{
	INT8U result;
	INT16U Time;

	result = _PAY_OK;
	
	Time = 40000;
	for(;(U78161->U7816STA & (0x02))!= 0;)/*"数据发送空闲"*/
	{
		Time--;
		if( 0 == Time)
		{
			result = _PAY_ERR;
			break;
		}
	}
    
	if(result == _PAY_OK)
	{
		U78161->U7816TXBUF  = T_data;
		Time = 40000;
		for(;((U78161->U7816IF & U7816_TX_IF) == 0);)
		{
			Time--;
			if(0 == Time)
			{
				result = _PAY_ERR;
				break;
			}
			if(U78161->U7816IF & U7816_ERR_IF)/*"发生错误退出"*/
			{
				U78161->U7816ERR = U78161->U7816ERR;
				result = _PAY_ERR;
				break;
			}
		}
	}
	/*"等待字节放送完成"*/
    EC_Dly(11);

}





