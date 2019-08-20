/*"***********************************************************"*/		
/*"FileName: L4428.c"*/		
/*"Author: quantp"*/       
/*"Version : 1.0       Date: 2017.03.01"*/	
/*"Description: SLE4428卡底层驱动"*/
/*"Function List:"*/	
/*"<FunctionName>"*/
/*"<author> <time> <version > <desc>"*/		
	
/*"***********************************************************"*/	
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\Device\L4428.h"

#define L4428_1_IO            1       /*" SD口连接方式，1：单线，0：双线,即使用两个IO口 "*/

/*" L4428 IO端口定义"*/
#define L4428_VCC   0x00
#define L4428_CLK   0x00
#define L4428_RST   0x00
#define L4428_RXD   0x00    /*" 当为单线方式，RXD TXD配置为一个IO口 "*/
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

/*" 函数本身不负责切换IO口的方向，由调用者负责"*/
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

/*" 函数本身不负责切换IO口的方向，由调用者负责"*/
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

#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输入 "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" 两线连接方式时候，需将TXD输出高电平 "*/
#endif
    
    MCU_DelayUs(30);
    L4428_Wait_ACK();

#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输出 "*/
    L4428_IO_Direct(OUTPUT);
#endif	
}

/*"************************************************			
Function: 	L4428_Read
Description: 读4428卡指定地址的数据
Input:  INT16U Addr：读数据地址
             INT8U *pBuff：数据BUFFER指针
             INT16U Len：读数据长度
Output: 无	
Return: TRUE  成功  FALSE失败 
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
    
    /*" 发送读命令"*/
    cmd = L4428_RD_CMD;
    data = Addr & 0x0001;
    L4428_CmdSend(cmd, Addr, data);

    /*" 为接收数据切换IO口"*/
#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输入 "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" 两线连接方式时候，需将TXD输出高电平 "*/
#endif

    /*" 循环接收数据"*/
    for(num = 0; num < Len; num++)
    {
        if( L4428_Check_Occur() != TRUE )
        {
            return FALSE;
        }
    		
        pBuff[num] = L4428_Recv1Byte();
	}

    /*" 切换IO口为发送状态, 发结束时序"*/
#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输出 "*/
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
Description: 写4428卡指定地址的数据
Input:  INT32U Addr：写数据地址
             INT8U *pBuff：数据BUFFER指针
             INT16U Len：写数据长度  
Output: 无	
Return: TRUE  成功  FALSE失败 
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
Description: 4428卡密码验证
Input: INT8U *PcsKey : 两字节密码指针
Output: 无	
Return: 成功: TRUE; 失败: FALSE
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

    if(i <= 0x60)  /*" 每张卡留两次密码效验机会 "*/
    {
        return FALSE;
    }

    for(j = 0; j < 8; j++) /*" 找到 "*/
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
Description: 4428卡复位
Input: 无	
Output: 无	
Return: 成功: TRUE; 失败: FALSE
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
    
#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输入 "*/
    L4428_IO_Direct(INPUT);
#else
    L4428_TXD_Ctr(HIGH);/*" 两线连接方式时候，需将TXD输出高电平 "*/
#endif    
    
    for(num = 0; num < 4; num++)
    {
        S_Temp[num] = L4428_Recv1Byte();       
    }
        
#if L4428_1_IO /*" 单线连接方式时候，需将TXDRXD定义为输出 "*/
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
Description: 4428卡下电
Input: 无	
Output: 无	
Return: 无	
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
Description: 上电硬件初始化
Input: Mode 上电模式
        MCU_RUNMODE: 正常上电
Output: 无	
Return: 无
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

