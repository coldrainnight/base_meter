/*"*****************************************************************************"*/
/*"  FileName: Ec_driver.c"*/
/*"  Description:     ���ļ�ʵ��ISO7816 CPU���ײ�����"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
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
Description: UART��7816���ݽӿڵĳ�ʼ��
Input:  None
Output: None
Return: None
Others: None	
************************************************"*/	
void EC_UartInit(void)
{   
	EC_UartRls();

	MCU_PeriphClkSet(U78161_num, ENABLE);
	
	/*"7816�Ĵ�����ʼ��"*/
	U78161->U7816FRC &= 0x0000;
	U78161->U7816FRC |= 0x00000044;/*"żУ�顢�Զ��ظ�����3��"*/
	U78161->U7816EGT =0x00;
	U78161->U7816IE  =0x00;

    U78161->U7816ERR  =0x0F;	/*"���־λ��д1����"*/
    U78161->U7816STA  =0x00;
    U78161->U7816IF   =0x07;

	U78161->U7816FRC &=(~1);	/*"������룬������"*/

	U78161->U7816CLKDIV = 1;	/*"Ƶ������2��֡ 4MHZ"*/
    U78161->U7816PDIV	=	0x173;	/*"1ETU Ϊ372CLK"*/

	U78161->U7816CTRL &=(~2); /*"7816ͨ��ǿ����ʹ��"*/	

	U78161->U7816CTRL &=(~0x10); /*"����ʹ��ģ���"*/
	U78161->U7816CTRL |=0x10;

	U78161->U7816CTRL &=(~0x20); /*"����ʹ��ģ���"*/
	U78161->U7816CTRL |=0x20;

	INT_VectSet(E_INT_ID_U78161, UART_7816_INT);
	INT_En(E_INT_ID_U78161);
	
	EC_Dly(6);

}

/*"************************************************			
Function:EC_UartRls
Description: 7816�ͷţ�ESAM��CPU���˿��ͷ�Ϊ��ͨI/O
Input:  None
Output: None
Return: None
Others: None	
************************************************"*/	
void EC_UartRls(void)
{
    EC_CPwrOff();
	
	U78161->U7816CTRL &=(~0x10); /*"��ֹ����ģ��"*/
	U78161->U7816CTRL |=0x00;

	U78161->U7816CTRL &=(~0x20); /*"��ֹ����ģ��"*/
	U78161->U7816CTRL |=0x00;

	/*"CPU���ݶ˿���Ϊ��ͨI/O���͵�ƽ���"*/
	GPIO_ConfigPin(_CPU_SD,OUTPUT,NORMAL_OUT);
	GPIO_SetMulti(_CPU_SD,GPIO_Mode_OUT);
	GPIO_OutputPin(_CPU_SD,LOW);
	INT_Dis(E_INT_ID_U78161);
}

/*"************************************************			
Function:EC_Dly
Description: 7816������ʱ����
Input:��ϵͳʱ��Ϊ4Mʱ��_EC_E_DLY_1ETU(93us����1ETU)   _EC_E_DLY_H_ETU(0.5ETU) 
                                                        _EC_C_DLY_1ETU(93us����1ETU)  _EC_C_DLY_H_ETU(0.5ETU) 
Output: None
Return: None
Others: ��ESAM��CPU��ʱ��Ƶ�ʲ�ͬʱҪ
���ݵ�ǰͨ��������ʱ;
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
Description: CPU����VCC���������
Input:  �źſ���
Output: None
Return: None
Others:��_EC_C_SUP����
************************************************"*/	
void EC_CVccCtl(INT8U output)
{
    EC_Dly(2);
}


/*"************************************************			
Function:EC_CRstCtl
Description: CPU��Rst�ź��������
Input:  �źſ���
Output:None
Return: None
Others: ��_EC_C_SUP����
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
Description: CPU���µ�
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
Description: �临λ
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
Description: CPU��CLK����
Input:  �źſ���
Output:None
Return: None
Others: ��_EC_C_SUP��_EC_SHARE_OSC����
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
Description: ESAM��VCC����
Input:  �źſ���
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
Description: ESAM��RST�źſ���
Input:  �źſ���
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
Description: ESAM IO ��ʼ��
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
Description: ESAM�µ�
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
Description: ESAM�临λ
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
Description: ESAM��CPU��ͨ���л�
(��ESAM��CPU������һ������ͨ��ʱ����)
Input:  ECIDΪ���л���ͨ��(_EC_ID_CPU/_EC_ID_ESAM)
Output:None
Return: None
Others: None
************************************************"*/
void EC_ChnSel(void)
{

	if(_EC_ID_CPU == mECCurChn)
	{
		GPIO_ConfigPin(_CPU_CLK,OUTPUT,NORMAL_OUT);/*"����CLK PC2��"*/
		GPIO_SetMulti(_CPU_CLK, GPIO_Mode_DIG);

		GPIO_ConfigPin(_CPU_SD,OUTPUT,OPENDRAIN_OUT);/*"����CPU��IO PC3��"*/
		GPIO_SetMulti(_CPU_SD, GPIO_Mode_DIG);

		GPIO_ConfigPin(_CPU_RST,OUTPUT,NORMAL_OUT);/*"����CPU����λ��PB5��"*/
		GPIO_SetMulti(_CPU_RST, GPIO_Mode_OUT);
	}

		EC_Dly(20);

}

/*"************************************************			
Function:EC_HardwareInit
Description: �ϵ�ʱ�˿�����
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

		GPIO_ConfigPin(_CPU_SW,OUTPUT,NORMAL_OUT);/*"����SW ��ʼΪ���ģʽ"*/
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
Description: ���ݱ��Ͷ�CPU�����˿ڽ�������
Input:  meterType: ����
        _NOPREPAYMETER      0      ��Ԥ���ѱ�
        _TELMETER             1      Զ�̱�
        _LOCALMETER          2      ���ر�
Output:None
Return: None
Others: None
************************************************"*/
void EC_SupCpuCfg(INT8U mtrType)
{
	 if(mtrType == LOCALMETER)
	 {
		 GPIO_ConfigPin(_CPU_SW,INPUT,NULL_IN);/*"����SWΪ����ģʽ"*/
		 GPIO_SetMulti(_CPU_SW, GPIO_IN);

		 GPIO_ConfigPin(_CPU_CLK,OUTPUT,NORMAL_OUT);/*"����CLK PC2��"*/
		 GPIO_SetMulti(_CPU_CLK, GPIO_Mode_OUT);
		 
		 GPIO_ConfigPin(_CPU_SD,OUTPUT,NORMAL_OUT);/*"����CPU��IO PC3��"*/
		 GPIO_SetMulti(_CPU_SD, GPIO_Mode_OUT);
		 
		 GPIO_ConfigPin(_CPU_RST,OUTPUT,NORMAL_OUT);/*"����CPU����λ��PB5��"*/
		 GPIO_SetMulti(_CPU_RST, GPIO_Mode_OUT);

		 EC_CPwrOff();
	 }
	 else
	 {
		 GPIO_ConfigPin(_CPU_SW,OUTPUT,NORMAL_OUT);/*"����SW ��ʼΪ���ģʽ"*/
		 GPIO_SetMulti(_CPU_SW, GPIO_Mode_OUT);
	 } 
}


/*"************************************************			
Function:EC_RevFromCpu
Description: CPU �����жϳ��� ����
Input:  None
Output:None
Return: None
Others: ��_EC_SHARE_SD����
************************************************"*/
void EC_RevData(INT8U data)
{
	EC_DataToBuf(data);
}


/*"************************************************			
Function:EC_UartEnTx
Description: CPU���ڹؽ����жϣ�ת����
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


/*7816�ж�*/
static void UART_7816_INT(void)
{
	if(U78161->U7816IF & U7816_RX_IF)
	{
        mEC_RevIsr(U78161->U7816RXBUF);
	}
	
	if(U78161->U7816IF & U7816_ERR_IF)/*"���մ���ʱ�����־"*/
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
	for(;(U78161->U7816STA & (0x02))!= 0;)/*"���ݷ��Ϳ���"*/
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
			if(U78161->U7816IF & U7816_ERR_IF)/*"���������˳�"*/
			{
				U78161->U7816ERR = U78161->U7816ERR;
				result = _PAY_ERR;
				break;
			}
		}
	}
	/*"�ȴ��ֽڷ������"*/
    EC_Dly(11);

}





