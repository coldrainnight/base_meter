#include "Driver\DriverComm.h"
#include "system\Sys_def.h"
#include "APP\LegallyRelevant\MU_LIB.h"
#include "APP\LegallyRelevant\LRM.h"

//#include "Driver\Device\MU_IC.h"


/*"***********************************************************  
FileName: MU_IC.c  
Author: ���쿡
Version : 1.0       Date: 2017.11.13
Description:��������ģ��,
���ڼ����ĳ�ʼ�������У�˲ʱ�����ݶ�ȡ����ȫ�ж��Լ���������
		   ����RN8209C����оƬ
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
static volatile INT16U PluseIntNum;/*"�յ�����󣬲���������������"*/
static volatile INT16U PluseNum; /*"�յ����壬�����������ж���+1,�ϵ��ʼ��Ϊ0"*/
static volatile INT8U  PluseIsrFlag;/*"�жϼ�⵽���壬��λ��־������ȷ�Ϻ������־����ʼ��ΪFALSE"*/
static INT8U  PluseBeginFlag;/*"������ʼ��ʱ��־,��ʼ��ΪFALSE"*/
static INT16U PluseBeginTime;/*"������ʼ��ʱʱ��"*/

static volatile INT16U Q_PluseIntNum;/*"�յ�����󣬲���������������"*/
static volatile INT16U Q_PluseNum; /*"�յ����壬�����������ж���+1,�ϵ��ʼ��Ϊ0"*/
static volatile INT8U  Q_PluseIsrFlag;/*"�жϼ�⵽���壬��λ��־������ȷ�Ϻ������־����ʼ��ΪFALSE"*/
static INT8U  Q_PluseBeginFlag;/*"������ʼ��ʱ��־,��ʼ��ΪFALSE"*/
static INT16U Q_PluseBeginTime;/*"������ʼ��ʱʱ��"*/

/*"************************************************			
Function:MU_IC_Wait
Description: ����оƬ���ڵȴ���ʱ���� 
Input:  Mode 0 �� 1д
Output: 	��
Return:��	
Others: 		
************************************************"*/
static void MU_IC_Wait(INT8U Mode)
{
    INT8U *PUartFlag;
    INT16U Time_cnt=0,flag=1;

    if(Mode ==0)/*��*/
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
Description: ����оƬ���ݷ��ͺ��� 
Input:  ��
Output: 	��
Return:��	
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
Description: ����оƬ���ݽ��պ��� 
Input:  ��
Output: 	��
Return:��	
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
Description:����д��������
Input:  reg �Ĵ�����ַ *data ���� length ����
Output: 	��
Return:��	
Others: 		
************************************************"*/
INT8U MU_IC_WriteReg(INT16U reg,INT8U *data,INT8U length)
{
    INT8U i,ret = FALSE;
    INT16U crc16;
    /*"д���� ����֡ = ������ + ������ + ���ݸ�ʽ��0x00��+ ���ݳ��� + ���ݣ�2�� + crc����� + crcУ���"*/  
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
  		gMeaIc_Uart.Uart_Rx_Data[i]=0;/* ����ձ�������*/
  	}

    /*"ͨѶʹ��"*/
  	
  	/*"��Ĵ���������"*/
    UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SO_RC);
    UART_Send1Byte(E_UART_ID_MC,gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length]);
  	/*"ͨѶʹ��"*/

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
Description: ���ڶ������� 
Input:  reg �Ĵ�����ַ length ����
Output: 	��
Return:��	
Others: 		
************************************************"*/
INT8U MU_IC_ReadReg(INT16U reg,INT8U length)
{
    INT8U i,ret = FALSE;
    INT16U crc16;
    /*������ ����֡ = �����루0x20 ���� 0x21 ���ݣ� + ������ + ���ݸ�ʽ��0x00��+ ���ݳ��ȣ�0�� + ���ݣ��գ� + crc����� + crcУ���"*/  
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
  		gMeaIc_Uart.Uart_Rx_Data[i]=0;/* ����ձ�������*/
  	}

    /*"ͨѶʹ��"*/
  	
  	/*"��Ĵ���������"*/
    UART_TranStaCtrl(E_UART_ID_MC,TRANSTA_SO_RC);
    UART_Send1Byte(E_UART_ID_MC,gMeaIc_Uart.Uart_Tx_Data[gMeaIc_Uart.Uart_recent_length]);
  	/*"ͨѶʹ��"*/

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
Description: �����оƬ��ض˿ڳ�ʼ����������SOC��ʽʱ������ؼĴ���������				
Input:  addr��Mode������ģʽ  0Ϊ�ϵ��ʼ����1Ϊ�͹��ĳ�ʼ��
Output: ��	
Return: ��			
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
Description:��λ����оƬ����
Input:  ��
Output: 	��
Return:��	
Others: 		
************************************************"*/
void MC_Reset(void)
{
	MC_ClosePulse();
	/*""����оƬ��λIO��"*/
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
Description: ͨ�����ü������峣���ӿ�				
Input:       device�������й����޹� constRatio���ڸ�Ƶ�����Ƶ�����಻��Ҫ�֣��̶�Ϊ1��pulseWidth������
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 		
************************************************"*/	
INT8U  MU_IC_PulseInit(INT8U device, INT32U pulseconst,INT8U  pulseWidth)
{
	INT8U RetVal = TRUE;

    if(device == 0)/*���й����塱*/
    {
	    if( (pulseconst >= 100) && (pulseconst <= 51200) )
    	{
    		if(pulseconst != gMU_Para.ram_adj_data.Ram_ecconst)/*""ֻ�����峣���ı��ʱ��Ÿ�д��������ֵ"*/
    		{
    		    if((gMU_Para.CRC_Val == CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val))) && (gMU_Para.MC_DefPara_Flag ==EEP_PAR_IS_INIT))/*""ֻ��crc��ȷ��ʱ������У�������¼����µ����峣��"*/
                {      
    			    gMU_Para.ram_adj_data.Ram_hfconst = (INT16U)((INT32U)gMU_Para.ram_adj_data.Ram_hfconst *(INT32U)gMU_Para.ram_adj_data.Ram_ecconst / pulseconst );
    			    gMU_Para.ram_adj_data.Ram_ecconst = (INT16U)pulseconst;
                    MU_IC_WriteReg(W_HFCNST,(INT8U  *)&gMU_Para.ram_adj_data.Ram_hfconst,2);
    		        gMU_Para.CRC_Val = CRC16((INT8U *)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
                    
                    if(gMU_Para.MC_DefPara_Flag == EEP_PAR_IS_INIT)/*""ֻ�ڲ���Ϊ��У׼�������£���д�ڿ���Ĭ�ϲ���������������д�ڿ�"*/
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
Description: ����������峣��������ָ��	
Input:    pdata      �������ָ��
Output: ��	
Return: ���ض���־0xAA	
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
Description: �򿪼���оƬ�����
Input:  ��
Output: 	��
Return:��	
Others: 		
************************************************"*/
void MC_OpenPulse(void)
{
	/*""���ù��������������IO��"*/
	GPIO_ConfigPin(PULSE_CTL,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(PULSE_CTL,0);//������"*/  
}
/*"************************************************			
Function:MC_ClosePulse
Description: �رռ���оƬ�����
Input:  ��
Output: 	��
Return:��	
Others: 		
************************************************"*/
void MC_ClosePulse(void)
{
	/*""���ù��������������IO��"*/
	GPIO_ConfigPin(PULSE_CTL,OUTPUT,NORMAL_OUT);
	GPIO_OutputPin(PULSE_CTL,1);/*"�ر�����"*/  
}

/*"************************************************			
Function:MU_IC_PD_Close
Description: �رռ���оƬ�����
Input:  ��
Output: 	��
Return:��	
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
Description: ��ȡ������������˿�״̬		
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
Description: ���������жϴ�����		
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
Description: ��ʼ����������������		
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
Description: ����ѭ���е��ã�������������������
             ���ڿ����ڸ�ֵ�����Ͻ����жϣ�plusenum++��,�������㣬
             �����ͻᶪ���壬�����ڴ˴��ر����ж�
Input:  
Output: ������
Return: �����ε���֮���������
Others: 		
************************************************"*/	
INT16U MU_IC_GetPulse_Val(void)
{
    INT16U Tmp;
    MCU_IntDisableAll();/*"�����ж�"*/
    Tmp = PluseNum ;
    PluseNum = 0;
    MCU_IntEnableAll();/*"���ж�"*/
    return Tmp;
}

/*"************************************************			
Function: 	GP_GetNoAffrimPulse_Val		
Description: �ϲ��ȡδ�����������������ڷ�����
Input:  
Output: δ������������
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
Description: �ϲ��ȡ�����忪ʼ��־
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
void MU_IC_ClrPulse(void)
{
    PluseIntNum = 0;/*"�յ�����󣬲���������������"*/
    PluseNum = 0; /*"�յ����壬�����������ж���+1,�ϵ��ʼ��Ϊ0"*/
    PluseIsrFlag = FALSE;/*"�жϼ�⵽���壬��λ��־������ȷ�Ϻ������־����ʼ��ΪFALSE"*/
    PluseBeginFlag = FALSE;/*"������ʼ��ʱ��־,��ʼ��ΪFALSE"*/
    PluseBeginTime = 0;/*"������ʼ��ʱʱ��"*/
}

/*"************************************************			
Function: 	GP_GetPulse_Val		
Description: ���жϵ���(��10ms��ʱ�ж�),ȷ������,���ۼ�
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
        else/*"�������ⲻ������"*/
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
        else/*"�������ⲻ������"*/
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
Description: ��ȡ������������˿�״̬		
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
Description: ���������жϴ�����		
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
Description: ����ѭ���е��ã�������������������
             ���ڿ����ڸ�ֵ�����Ͻ����жϣ�plusenum++��,�������㣬
             �����ͻᶪ���壬�����ڴ˴��ر����ж�
Input:  
Output: ������
Return: �����ε���֮���������
Others: 		
************************************************"*/	
INT16U MU_IC_Get_Q_Pulse_Val(void)
{
    INT16U Tmp;
    MCU_IntDisableAll();/*"�����ж�"*/
    Tmp = Q_PluseNum ;
    Q_PluseNum = 0;
    MCU_IntEnableAll();/*"���ж�"*/
    return Tmp;
}

/*"************************************************			
Function: 	MU_IC_Get_Q_NoAffrimPulse_Val		
Description: �ϲ��ȡδ�����������������ڷ�����
Input:  
Output: δ������������
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
Description: �ϲ��ȡ�����忪ʼ��־
Input:  
Output: 
Return: 
Others: 		
************************************************"*/	
void MU_IC_Clr_Q_Pulse(void)
{
    Q_PluseIntNum = 0;/*"�յ�����󣬲���������������"*/
    Q_PluseNum = 0; /*"�յ����壬�����������ж���+1,�ϵ��ʼ��Ϊ0"*/
    Q_PluseIsrFlag = FALSE;/*"�жϼ�⵽���壬��λ��־������ȷ�Ϻ������־����ʼ��ΪFALSE"*/
    Q_PluseBeginFlag = FALSE;/*"������ʼ��ʱ��־,��ʼ��ΪFALSE"*/
    Q_PluseBeginTime = 0;/*"������ʼ��ʱʱ��"*/
}

