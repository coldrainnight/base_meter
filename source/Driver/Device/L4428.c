/*"***********************************************************"*/		
/*"FileName: L4428.c"*/		
/*"Author: quantp"*/       
/*"Version : 1.0       Date: 2017.03.01"*/	
/*"Description: SLE4428���ײ�����"*/
/*"Function List:"*/	
/*"<FunctionName>"*/
/*"<author> <time> <version > <desc>"*/		
	
/*"***********************************************************"*/	
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\Device\L4428.h"

#define L4428_1_IO            1       /*" SD�����ӷ�ʽ��1�����ߣ�0��˫��,��ʹ������IO�� "*/

/*" L4428 IO�˿ڶ���"*/
#define L4428_VCC   0x00
#define L4428_CLK   0x00
#define L4428_RST   0x00
#define L4428_RXD   0x00    /*" ��Ϊ���߷�ʽ��RXD TXD����Ϊһ��IO�� "*/
#define L4428_TXD   0x00

#define L4428_SW    0x00


#define L4428_RD_CMD    0x0e
#define L4428_WR_CMD    0x33

static void L4428_VCC_Ctr(INT8U OutPut)
{
    GPIO_OutputPin(L4428_VCC, OutPut);
}

static void L4428_CLK_Ctr(INT8U OutPut)
{
    GPIO_OutputPin(L4428_CLK, OutPut);
}

static void L4428_RST_Ctr(INT8U OutPut)
{
    GPIO_OutputPin(L4428_RST, OutPut);
}

static void L4428_IO_Direct(INT8U Dir)
{
    if(Dir == INPUT)
    {
        GPIO_ConfigPin(L4428_RXD, Dir, NULL_IN);
    }
    else
    {
        GPIO_ConfigPin(L4428_RXD, Dir, OPENDRAIN_OUT);
    }
}

static void L4428_TXD_Ctr(INT8U OutPut)
{
    GPIO_OutputPin(L4428_TXD, OutPut);
}


static INT8U L4428_RXD_State(void)
{
    return GPIO_ReadPin(L4428_RXD);
}

static INT8U L4428_Check_Occur(void)
{
    return GPIO_ReadPin(L4428_SW);
}

void L4428_Send1Byte(INT8U T_data)
{
    INT8U counter,temp;

    for(counter = 8; counter != 0; counter--)
    {
        L4428_CLK_Ctr(LOW);
        temp = T_data & 0x01;
        T_data = T_data >> 1;
        L4428_TXD_Ctr(temp);
        MCU_DelayUs(20);
        L4428_CLK_Ctr(HIGH);
        MCU_DelayUs(30);
        L4428_CLK_Ctr(LOW);
    }
}

/*" �������������л�IO�ڵķ����ɵ����߸���"*/
INT8U L4428_Recv1Byte(void)
{
    INT8U bit, temp;

    temp = 0;
    for(bit = 8; bit != 0; bit--)
    {
        MCU_DelayUs(20);
        L4428_CLK_Ctr(HIGH);
        MCU_DelayUs(30);
        temp >>= 1;
        if( L4428_RXD_State() == HIGH )
        {
            temp |= 0x80;
        }
        L4428_CLK_Ctr(LOW);
    }

    return temp;
}

/*" �������������л�IO�ڵķ����ɵ����߸���"*/
INT8U L4428_Wait_ACK(void)
{
    INT8U i;
    for( i = 0; i < 250; i++ )
    {
        L4428_CLK_Ctr(HIGH);
        MCU_DelayUs(30);
        L4428_CLK_Ctr(LOW);
        MCU_DelayUs(20);

        if( L4428_RXD_State() == LOW )
        {
            return 1;
        }
    }
    return 0;
}


void L4428_CmdSend(INT8U Cmd, INT16U Addr, INT8U T_data)
{
    INT8U temp;

    L4428_RST_Ctr(HIGH);
    temp = (INT8U)(Addr >> 2);
    temp = temp & 0xc0;
    temp = temp | Cmd;
    L4428_Send1Byte(temp);

    temp = (INT8U)Addr;
    L4428_Send1Byte(temp);
    L4428_Send1Byte(T_data);

    L4428_CLK_Ctr(LOW);
    MCU_DelayUs(20);
    L4428_RST_Ctr(LOW);

    MCU_DelayUs(20);
    L4428_CLK_Ctr(HIGH);
    MCU_DelayUs(30);
    L4428_CLK_Ctr(LOW);
    MCU_DelayUs(20);
}

void L4428_SendCmd_Wait_ACK(INT8U Cmd, INT16U Addr, INT8U T_data)
{
    L4428_CmdSend(Cmd, Addr, T_data);

#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ���� "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" �������ӷ�ʽʱ���轫TXD����ߵ�ƽ "*/
#endif
    
    MCU_DelayUs(30);
    L4428_Wait_ACK();

#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ��� "*/
    L4428_IO_Direct(OUTPUT);
#endif	
}

/*"************************************************			
Function: 	L4428_Read
Description: ��4428��ָ����ַ������
Input:  INT16U Addr�������ݵ�ַ
             INT8U *pBuff������BUFFERָ��
             INT16U Len�������ݳ���
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/
INT8U L4428_Read(INT16U Addr, INT8U *pBuff, INT16U Len)
{
    INT8U cmd;
    INT8U data;
    INT16U num;

    if(pBuff == NULL)
    {
        return FALSE;
    }
    
    /*" ���Ͷ�����"*/
    cmd = L4428_RD_CMD;
    data = Addr & 0x0001;
    L4428_CmdSend(cmd, Addr, data);

    /*" Ϊ���������л�IO��"*/
#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ���� "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" �������ӷ�ʽʱ���轫TXD����ߵ�ƽ "*/
#endif

    /*" ѭ����������"*/
    for(num = 0; num < Len; num++)
    {
        if( L4428_Check_Occur() != TRUE )
        {
            return FALSE;
        }
    		
        pBuff[num] = L4428_Recv1Byte();
	}

    /*" �л�IO��Ϊ����״̬, ������ʱ��"*/
#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ��� "*/
    L4428_IO_Direct(OUTPUT);
#endif	

    MCU_DelayUs(20);
    L4428_RST_Ctr(HIGH);
    MCU_DelayUs(30);
    L4428_RST_Ctr(LOW);
    return TRUE;
}

/*"************************************************			
Function: 	L4428_Write
Description: д4428��ָ����ַ������
Input:  INT32U Addr��д���ݵ�ַ
             INT8U *pBuff������BUFFERָ��
             INT16U Len��д���ݳ���  
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U L4428_Write(INT16U Addr, INT8U *pBuff, INT16U Len)
{
    INT8U cmd;
    INT8U data;
    INT16U num;

    if(pBuff == NULL)
    {
        return FALSE;
    }

    cmd = L4428_WR_CMD;
    for(num = 0; num < Len; num++)
    {
        if( L4428_Check_Occur() != TRUE )
        {
            return FALSE;
        }

        data = pBuff[num];
        L4428_SendCmd_Wait_ACK(cmd, Addr + num, data);
    }
    
    return TRUE;
}

/*"************************************************			
Function: 	L4428_Password_Check
Description: 4428��������֤
Input: INT8U *PcsKey : ���ֽ�����ָ��
Output: ��	
Return: �ɹ�: TRUE; ʧ��: FALSE
Others: 		
************************************************"*/	
INT8U L4428_Password_Check(INT8U *PcsKey)
{
    INT8U i,j,k,temp;

    if(PcsKey == NULL)
    {
        return FALSE;
    }


    L4428_Read(0x03fd, &i, 1);

    if(i <= 0x60)  /*" ÿ�ſ�����������Ч����� "*/
    {
        return FALSE;
    }

    for(j = 0; j < 8; j++) /*" �ҵ� "*/
    {
        if((i & 0x01) == 0x01) 
        {
            i = i & 0xfe;
            i = i << j;
            break;
        }
        i = i >> 1;
    }
    i = i | 0x80;

    L4428_SendCmd_Wait_ACK(0xf2, 0x03fd, i);
    temp = PcsKey[1];
    L4428_SendCmd_Wait_ACK(0xcd, 0x03fe, temp);
    temp = PcsKey[0];
    L4428_SendCmd_Wait_ACK(0xcd, 0x03ff, temp);
    L4428_SendCmd_Wait_ACK(L4428_WR_CMD, 0x03fd, 0xff);

    L4428_Read(0x03fd, &i, 1);

    k = 0;
    for (j = 0; j < 8; j++)
    {
        if((i & 0x01) == 0x01)
        {
            break;
        }
        else
        {
            k++;
        }
        i = i >> 1;
    }
    
    if(k == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*"************************************************			
Function: 	L4428_ATR
Description: 4428����λ
Input: ��	
Output: ��	
Return: �ɹ�: TRUE; ʧ��: FALSE
Others: 		
************************************************"*/	
INT8U L4428_ATR(void)
{
    INT8U num, S_Temp[4];
    
    L4428_RST_Ctr(LOW);
    L4428_CLK_Ctr(LOW);
    MCU_DelayUs(60);
    L4428_RST_Ctr(HIGH);
    MCU_DelayUs(80);
    L4428_CLK_Ctr(HIGH);
    MCU_DelayUs(80);
    L4428_CLK_Ctr(LOW);
    MCU_DelayUs(60);
    L4428_RST_Ctr(LOW);
    MCU_DelayUs(60);
    
#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ���� "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" �������ӷ�ʽʱ���轫TXD����ߵ�ƽ "*/
#endif    
    
    for(num = 0; num < 4; num++)
    {
        S_Temp[num] = L4428_Recv1Byte();       
    }
        
#if L4428_1_IO /*" �������ӷ�ʽʱ���轫TXDRXD����Ϊ��� "*/
	L4428_IO_Direct(OUTPUT);
#endif			
        
    MCU_DelayUs(10);
    for(num = 0; num < 4; num++)
    {
        if((S_Temp[num] != 0x0) && (S_Temp[num] != 0xFF))
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*"************************************************			
Function: 	L4428_PowOff
Description: 4428���µ�
Input: ��	
Output: ��	
Return: ��	
Others: 		
************************************************"*/	
void L4428_PowOff(void)
{
    L4428_RST_Ctr(LOW);
    L4428_VCC_Ctr(LOW);
    L4428_CLK_Ctr(LOW);
}


/*"************************************************			
Function: 	L4428_Hardware_Init
Description: �ϵ�Ӳ����ʼ��
Input: Mode �ϵ�ģʽ
        MCU_RUNMODE: �����ϵ�
Output: ��	
Return: ��
Others: 		
************************************************"*/	
void L4428_Hardware_Init(INT8U Mode)
{
    if( Mode == MCU_RUNMODE)
    {
        GPIO_ConfigPin(L4428_RST, OUTPUT, NULL_IN);
        GPIO_ConfigPin(L4428_CLK, OUTPUT, NULL_IN);
        GPIO_ConfigPin(L4428_VCC, OUTPUT, NULL_IN);
        GPIO_ConfigPin(L4428_RXD, INPUT, NULL_IN);
        GPIO_ConfigPin(L4428_TXD, OUTPUT, NULL_IN);
        GPIO_ConfigPin(L4428_SW, INPUT, NULL_IN);
    }
    else
    {
        ;
    }
}

