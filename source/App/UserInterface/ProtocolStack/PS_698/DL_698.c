
#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "Lib\LIB_func.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\WDT.h"
#include "app\userinterface\UI.h"
#include "Driver\Device\chip_commu_slave.h"


const INT16U FCS_Table[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


#define PPPINITFCS16    (0xFFFF) /* Initial FCS value */

#define DL698_IDLE      (0)/*"链路空闲"*/
#define DL698_RESPONSE  (1)/*"等待应用层响应"*/
#define DL698_BLOCK  (2)/*"链路上行分帧"*/

#define REC_OVER_APDU   (-1)/*"APDU片段累加长度溢出应用层缓冲区"*/
#define REC_INVALID         (0)/*"无有效接收"*/
#define REC_BLOCK            (1)/*"接收为APDU片段"*/
#define REC_OVER              (2)/*"接收完成"*/

#define BLOCK_START         (0x00)/*"起始帧"*/
#define BLOCK_MIDDLE       (0xC0)/*"中间帧"*/
#define BLOCK_END             (0x40)/*"结束帧"*/
#define BLOCK_ACK             (0x80)/*"确认帧(客户机响应)"*/

#define INTER_OCTET_TIMEOUT (100)/*"字节间超时"*/
#define INACTIVITY_TIMEOUT (1000)/*"无活动超时 (帧超时)"*/
#define REC_RES_TIMEOUT      (10000)/*"(后续帧)接收客户机响应超时3s"*/
#define AL_RES_TIMEOUT       (500)/*"接收应用层响应超时500ms"*/

#define TYPE_OF_SERVICE      (3) /*"链路管理的服务类型"*/
#define PRM_SERVER_REQUEST   (0) /*"服务器的请求或上报"*/
#define PRM_SERVER_RESPONSE  (1) /*"服务器的响应"*/
#define PRM_CLIENT_RESPONSE  (0) /*"客户机的响应"*/
#define CTRL_CLIENT_DL_RES   (0x01) /*"客户机的链路响应"*/


//#define MIN_698_FRM_LEN    (13) /*"698: 1字节地址，1字节APDU"*/
//#define MIN_645_FRM_LEN    (12) /*"645: 无DATA"*/
#define MIN_FRM_LEN    (12) /*"645: 无DATA"*/
#define SA_OFFSET         (4)  /*"698: SA在帧中的偏移"*/
#define SECOND_68H_OFFSET  (7)  /*"645: 第2个68H在帧中的偏移"*/
#define FRM_HEAD_LEN  (16) /*"含分帧格式域2字节，6字节地址，16字节地址时帧头长度为24"*/
#define BLOCK_FMT_LEN  (2) /*"分帧格式域长度"*/
#define SEND_START      (4) /*"前导字节长度"*/
#ifdef THREE_PHASE
#define IR_WAKE_UP_LEN  (2)/*"红外唤醒字串长度"*/
#endif

#define DLT645_BUFF_LEN  (180)/*"645协议帧临时缓冲区最大长度"*/

#define FRM_DLT698     (1)
#define FRM_DLT645     (2)

#define DLT645_FIXED_REC_LEN   (12)


INT8U mBpsChangeFlg[E_MAX_COM_ID];  /*"bps修改标识"*/
#ifdef THREE_PHASE
INT8U mPdIrWakeUpFlg;/*"低功耗红外唤醒标志"*/
#endif

static ST_DLT698_DEVICE mstDlt698Device[E_MAX_COM_ID];/*"设备控制信息"*/
static ST_DL_STATUS mstDLStatus[E_MAX_COM_ID];/*"通信链路状态"*/
static ST_FRM_INFO mstFrmInfo[E_MAX_COM_ID];/*"链路帧信息"*/
static INT8U MinTimeFlg[E_MAX_COM_ID];/*"启动解析的最小间隔标志"*/
static INT8U mShowFlg[E_MAX_COM_ID];/*"通信状态显示标志"*/


#if 0
static INT16U CalcFCS16(INT16U fcs, INT8U *cp, INT16U len);
#endif
static INT16U CalcFCS16(INT16U fcs, INT8U *pBuff, INT16U head, INT16U max, INT16U len);
static void FillFCS16(INT8U *pStart, INT16U len, INT8U *output);
//static INT8U DLT698_ProcessRecBytes(INT8U *pDataBuff, INT16U length, INT8U DeviceId);
//static INT8U CheckFrmHead(INT8U *rpAddress, ST_FRM_INFO *pstFrmRst);
static void Dlt698_TimeOut (void);
static void DL698_ClrBlockInfo(INT8U DeviceId);
static void DL698_EnReceive(INT8U DeviceId);
static void DL698_SendStart(INT8U DeviceId);
static void Dlt698_Receive (INT8U DeviceId, INT8U Byte);
static void Dlt698_SendOver (INT8U DeviceId);
static void ShowStatusSet(INT8U DeviceId);
static void ShowStatusClr(INT8U DeviceId);
//static void BlockFrmAck(INT8U DeviceId);
//static void CycleDataCopy(INT8U *inBuff, INT16U inIndex, INT16U inBufSize, INT16U lenth, INT8U *outBuff, INT8U code);



#if 0
/*"*************************************************************"*/			
/*"Function: 	CalcFCS16		"*/
/*"Description: 计算698帧HCS、FCS"*/
/*"Input: "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/  
static INT16U CalcFCS16(INT16U fcs, INT8U *cp, INT16U len)
{
    while(len--)
    {
        fcs = (fcs >> 8) ^ FCS_Table[((INT8U)fcs ^ (*cp++)) & 0xFF];
    }

    return (fcs ^ 0xFFFF);
}
#endif

/*"*************************************************************"*/			
/*"Function: 	CalcFCS16		"*/
/*"Description: 计算698帧HCS、FCS"*/
/*"Input:      fcs: Initial FCS value  "*/
/*"               pBuff: 数据缓冲区首地址"*/
/*"               head: 首地址偏移"*/
/*"               max: 缓冲区最大长度，max为0xFFFF时表示不关注该项参数"*/
/*"               len: 参与校验计算的数据长度"*/
/*"Output: 	"*/
/*"Return: FCS校验值"*/
/*"Others: 		"*/
/*"*************************************************************"*/  
static INT16U CalcFCS16(INT16U fcs, INT8U *pBuff, INT16U head, INT16U max, INT16U len)
{
    INT16U i;
    INT8U *pData;

    pData = pBuff + head;
    for(i = 0; i < len; i++)
    {
        fcs = (fcs >> 8) ^ FCS_Table[((INT8U)fcs ^ (*pData++)) & 0xFF];
        if((0xFFFF != max) && (INT16U)(pData - pBuff) >= max)
        {
            pData = pBuff;
        }
    }

    return (fcs ^ 0xFFFF);
}


/*"*************************************************************"*/			
/*"Function: 	FillFCS16		"*/
/*"Description: 发送698帧的HCS、FCS位置填充校验码"*/
/*"Input: "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/  
static void FillFCS16(INT8U *pStart, INT16U len, INT8U *output)
{
    INT16U trialfcs;

    /* add on output */
#if 0
    trialfcs = CalcFCS16(PPPINITFCS16, pStart, len );
#endif
    trialfcs = CalcFCS16(PPPINITFCS16, pStart, 0, 0xFFFF, len );
    //trialfcs ^= 0xFFFF; /* complement */
    *output = (INT8U)(trialfcs & 0x00FF);  /* least significant byte first */
    *(output + 1) = (INT8U)((trialfcs >> 8) & 0x00FF);
}

void DL_698_Init(void)
{
    INT8U uni;
    
    LIB_MemSet(0, (INT8U *)&mstDlt698Device[0], sizeof(mstDlt698Device));

    for(uni = 0; uni < E_MAX_COM_ID; uni ++)
    {
        mstDlt698Device[uni].CurrentDeviceId = uni;
        mstDlt698Device[uni].RxBuff = ComBuffTable[uni].RxBuffAddr;
        mstDlt698Device[uni].TxBuff = ComBuffTable[uni].RxBuffAddr;    

#ifdef THREE_PHASE
        if((TRUE == ChkStatus(E_LOW_PWR)))
        {/*"低功耗下仅支持红外通信"*/
            continue;
        }

        mPdIrWakeUpFlg = 0;
#endif

        DL698_DLInit(uni); 
    }
}
 
void DL_698_Main(void)
{
    INT8U DeviceIndex; 
    INT8U result;
    //INT16U blockNo;
    //INT8U blockFlg;
    ST_DLT698_DEVICE *pDevice;
    //ST_FRM_INFO *pFrmInfo;
    //ST_DL_STATUS *pDLStatus;
    //INT16U maxSize;
    //INT8U SCcode;/*"扰码标志"*/

    Dlt698_TimeOut();

    for(DeviceIndex = 0; DeviceIndex < E_MAX_COM_ID; DeviceIndex++)
    {
#ifdef THREE_PHASE
        if((TRUE == ChkStatus(E_LOW_PWR)))
        {/*"低功耗下仅支持红外通信"*/
            continue;
        }
#endif
        if(DeviceIndex == E_COM_SLAVE)
        {
            //slave_ans_data();
            continue;
        }
        pDevice = &mstDlt698Device[DeviceIndex];
        //pFrmInfo = &mstFrmInfo[DeviceIndex];
        //pDLStatus = &mstDLStatus[DeviceIndex];
        //maxSize = mCommBuffMax[DeviceIndex];

        if(TRUE == pDevice->InactivityTimeout)
        {/*"通信总超时"*/	
            pDevice->TxWaiting = FALSE;  
            pDevice->InactivityTimeout = FALSE;	
        }	

        if((TRUE == pDevice->InterOctetTimeout) && (TRUE == MinTimeFlg[DeviceIndex]))
        {/*"发生字节间超时, 处理接收帧,返回解析结果"*/
            if((TRUE == ChkStatus(E_LOW_PWR)))
            {
                DL698_EnReceive(DeviceIndex);
                continue;
            }

            pDevice->frmType = 0;
            /*"698协议解析"*/
            //result = DLT698_ProcessRecBytes(pDevice->RxBuff, pDevice->RxLen, DeviceIndex);	
            result = FALSE;
#if 0
            if(TRUE == result)
            {
                ShowStatusSet(DeviceIndex);

                pFrmInfo->retToAL = REC_INVALID;

                blockFlg = (INT8U)(pFrmInfo->blockFmt >> 8) & 0xC0;
                blockNo = pFrmInfo->blockFmt & 0x0FFF;

                if((DL698_BLOCK == pDLStatus->action) && /*"上行分帧，处理链路分帧确认"*/
                (pFrmInfo->flag.bits.Asw)) /*"收到分帧确认"*/
                {
                    if((blockNo == pFrmInfo->blockNo) &&
                    (((pFrmInfo->blockFmt >> 8) & 0xC0) == BLOCK_ACK))/*"分帧确认"*/
                    {
                        pFrmInfo->blockNo += 1;

                        DL698_SendStart(DeviceIndex);/*"准备下个分帧发送"*/
                    }
                    else/*"分帧序号错误"*/
                    {
                        DL698_ClrBlockInfo(DeviceIndex);
                    }
                }
                else if(pFrmInfo->flag.bits.DownNextFrm)
                {/*"下行分帧，非结束帧时由链路层返回确认"*/
                    result = FALSE;

                    switch(blockFlg)
                    {
                    case BLOCK_START:
                        if(0 == blockNo)
                        {
                            pFrmInfo->blockNo = 0;
                            pFrmInfo->rALBuffOffset = 0;
                            result = TRUE;
                        }
                        break;
                    case BLOCK_MIDDLE:
                    case BLOCK_END:
                        pFrmInfo->blockNo += 1;
                        if((pFrmInfo->rALBuffOffset + pFrmInfo->rLen) > pFrmInfo->maxApduLen)
                        {
                            DL698_DLInit(DeviceIndex);
                            pFrmInfo->retToAL = REC_OVER_APDU;
                        }
                        else if(blockNo == pFrmInfo->blockNo)
                        {
                            result = TRUE;
                        }
                        else
                        {
                        }
                        break;
                    default:
                        break;
                    }

                    if(FALSE == result)
                    {
                        DL698_ClrBlockInfo(DeviceIndex);
                    }
                    else if(BLOCK_END == blockFlg)
                    {
                        pFrmInfo->retToAL = REC_OVER;
                    }
                    else
                    {
                        pFrmInfo->retToAL = REC_BLOCK;
                    }
                }
                else
                {
                    pFrmInfo->rALBuffOffset = 0;
                    pFrmInfo->retToAL = REC_OVER;
                }

                if((NULL != pFrmInfo->pAPDU) &&
                (pFrmInfo->retToAL >= REC_BLOCK))
                {
#if 0
                    LIB_MemCpy( pDevice->RxBuff + pFrmInfo->rHeadPos, 
                                           pFrmInfo->pAPDU + pFrmInfo->rALBuffOffset, 
                                           pFrmInfo->rLen);
#endif

                    if(pFrmInfo->ctrl.bits.SC)/*"扰码标志为1，链路用户数据减0x33"*/
                    {
                        SCcode = 0x33;
                    }
                    else
                    {
                        SCcode = 0;
                    }

                    CycleDataCopy( pDevice->RxBuff, 
                                              pFrmInfo->rHeadPos, 
                                              maxSize, 
                                              pFrmInfo->rLen, 
                                              (pFrmInfo->pAPDU + pFrmInfo->rALBuffOffset),
                                              SCcode );

                    pFrmInfo->rALBuffOffset += pFrmInfo->rLen;

                    if(REC_BLOCK == pFrmInfo->retToAL)
                    {
                        BlockFrmAck(DeviceIndex);/*"下行分帧确认"*/
                        pFrmInfo->retToAL = REC_INVALID;
                    }
                    else if(REC_OVER == pFrmInfo->retToAL)/*"APDU需返回应用层"*/
                    {
#if 0 
                        if(BLOCK_END == blockFlg)
                        {/*"下行分帧结束，应用层无返回，重启接收"*/
                            DL698_EnReceive(DeviceIndex);
                        }
                        else
#endif
                        {
                            pDLStatus->action = DL698_RESPONSE; /*"等待应用层的响应"*/
                            pDLStatus->timeAL = MCU_GetTick(); /*"启动应用层响应超时"*/
                        }
                    }
                    else
                    {
                    }
                }
                else if(NULL == pFrmInfo->pAPDU)
                {
                    DL698_ClrBlockInfo(DeviceIndex);
                }
                else
                {
                }
            }
            else
#endif
            {/*"645协议解析:  先复制数据，再复位收发缓冲区"*/	
                INT8U dataBuff[DLT645_BUFF_LEN];
                INT16U dataLen;

                dataLen = pDevice->RxLen;
                if((dataLen >= DLT645_FIXED_REC_LEN) && (dataLen < DLT645_BUFF_LEN))
                {
                    LIB_MemCpy(pDevice->RxBuff, dataBuff, dataLen);

                    result = DLT645_ProcessRecBytes(dataBuff, dataLen,DeviceIndex);	
                    if(CM_MASTER_DATA == result && DeviceIndex == E_COM_RS485_1)
                    {
                    }

                    if((CM_UNUSUAL != result) && (FALSE == pDevice->TxWaiting))
                    {
                        ShowStatusSet(DeviceIndex);

                        LIB_MemCpy(dataBuff, pDevice->TxBuff, result);
                        pDevice->TxLen = result;
                        pDevice->TxWaiting = TRUE;  
                        COM_StartSend(pDevice->CurrentDeviceId, pDevice->TxBuff, pDevice->TxLen);
                    }
                }
            }

            DL698_EnReceive(DeviceIndex);
        }

        if(TRUE == pDevice->TxOver)/*"发送结束时的处理"*/
        {
            if(FALSE != mBpsChangeFlg[DeviceIndex])
            {
                DL698_DLInit(DeviceIndex);
                mBpsChangeFlg[DeviceIndex] = FALSE;
            }
            pDevice->TxOver = FALSE;
        }
    }
}

INT16S DL_698_FrameCheck(INT8U ComId, 
                                INT8U * PApduBuf, 
                                INT16U MaxApdubufLen,
                                ST_698_FRAME_INFO *PReFrameInfo)
{
    INT16S result;
    ST_FRM_INFO *pFrmInfo;

    if((NULL == PApduBuf) || (NULL == PReFrameInfo))
    {
        return REC_INVALID;
    }

    result = REC_INVALID;

    if((ComId < E_MAX_COM_ID) && (NULL != PApduBuf) && 
    (DL698_BLOCK != mstDLStatus[ComId].action))
    {
        pFrmInfo = &mstFrmInfo[ComId];
        pFrmInfo->pAPDU = PApduBuf;
        pFrmInfo->maxApduLen = MaxApdubufLen;

        if(REC_OVER == pFrmInfo->retToAL)
        {
            result = (INT16S)pFrmInfo->rALBuffOffset;

            PReFrameInfo->AddrType= (EN_ADDRTYPE)pFrmInfo->addrType;
            PReFrameInfo->clientAddr = pFrmInfo->clientAddr;

            if((ADDR_BROADCAST == pFrmInfo->addrType) || /*"广播地址无应答"*/
            (ADDR_GROUP == pFrmInfo->addrType) || /*"组地址无应答"*/
            (PRM_CLIENT_RESPONSE == pFrmInfo->ctrl.bits.PRM)) /*"客户机的响应无应答"*/
            {
                DL698_EnReceive(ComId);
                mstDLStatus[ComId].action = DL698_IDLE;
            }
            pFrmInfo->retToAL = REC_INVALID;
        }
        else
        {
            result = (INT16S)pFrmInfo->retToAL;
        }
    }

    return result;
}

INT8U DL_698_FrameSend(INT8U ComId, 
                              INT8U * PApduBuf, 
                              INT16U SendApduLen)
{
    if((ComId >= E_MAX_COM_ID) || (NULL == PApduBuf) ||
    (ADDR_BROADCAST == mstFrmInfo[ComId].addrType) ||/*"广播地址类型不应答"*/
    (ADDR_GROUP == mstFrmInfo[ComId].addrType))/*"组地址类型不应答"*/
    {
        return FALSE;
    }
    
    mstFrmInfo[ComId].blockNo = 0;/*"分帧序号初始化"*/
    mstDLStatus[ComId].flag.bits.confirmToAL = 0;

    mstFrmInfo[ComId].sLen = SendApduLen;
    mstFrmInfo[ComId].pAPDU = PApduBuf;
    mstFrmInfo[ComId].ctrl.bits.FRM = 0;

    DL698_SendStart(ComId);

    return TRUE;
}

/*"*************************************************************"*/
/*"Function: 	DL698_DLInit		"*/
/*"Description: 通信链路初始化"*/
/*"Input:  ComId ：通信口编号"*/
/*"Output: 无	"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"*************************************************************"*/
void DL698_DLInit(INT8U ComId)
{
    UN_UARTFORM UartFormTmp;
    INT8U bpstemp;

    /*"通信设备初始化"*/
    switch (ComId)
    {
    case E_COM_RS485_1:
        GetSingle(E_RS485_1_BAUDRATE, &bpstemp);
        break;
#ifdef THREE_PHASE
    case E_COM_RS485_2:
        GetSingle(E_RS485_2_BAUDRATE, &bpstemp);
        break;
#endif
    case E_COM_SLAVE:
        bpstemp = E_BAUD460800;/*115200*/
        break;
    default:
        return;
    }

    UartFormTmp.bits.DataBits = 3;
    UartFormTmp.bits.Parity = 2;
    UartFormTmp.bits.End = 0;
    UartFormTmp.bits.Unused = 0;
    COM_Init (ComId, bpstemp, UartFormTmp, Dlt698_Receive, Dlt698_SendOver);

    /*"通信模块内部数据初始化"*/       
    mBpsChangeFlg[ComId] = FALSE;

    mstDLStatus[ComId].action = DL698_IDLE;
    mstDLStatus[ComId].flag.byte = 0;

    LIB_MemSet(0, (INT8U *)&mstFrmInfo[ComId], sizeof(ST_FRM_INFO));
    MinTimeFlg[ComId] = FALSE;
    mShowFlg[ComId] = FALSE;
}

/*"接收中断函数，用于实现接收一个字节"*/
static void Dlt698_Receive (INT8U DeviceId, INT8U Byte)
{
    ST_DLT698_DEVICE *pDevice;

    if(DeviceId >= E_MAX_COM_ID)
    {
        return;
    }

    pDevice = &mstDlt698Device[DeviceId];
    if(pDevice->InterruptClose == FALSE)/*"只有串口的接受中断标志位是关闭的，才能够将接收到的数据添加到接收缓冲区"*/
    {	 
        pDevice->InactivityTimer = MCU_GetTick();
        if(0 == pDevice->InactivityTimer)
        {
            pDevice->InactivityTimer = 1;			
        }


        if(pDevice->RxLen >= mCommBuffMax[DeviceId])
        {   
                pDevice->RxLen = 0; 
        }	
        pDevice->RxBuff[pDevice->RxLen] = Byte; /*"将其他的字节赋给接收缓冲区"*/
        pDevice->RxLen++;                        /*"接收到的长度+1"*/	
        
        pDevice->InterOctetTimer = MCU_GetTick();/*"重新给字节间超时时间赋值"*/    
        if(0 == pDevice->InterOctetTimer)
        {
            pDevice->InterOctetTimer = 1;			
        }
    } 
    else
    {
    }
}

/*"发送结束回调函数，即串口已经将该帧数据发送出去之后将要回调该函数"*/
static void Dlt698_SendOver (INT8U DeviceId)
{
    ST_DLT698_DEVICE *pDevice;
    ST_DL_STATUS *pDLStatus;

    if(DeviceId < E_MAX_COM_ID)
    {
        pDevice = &mstDlt698Device[DeviceId];

        pDevice->TxLen = 0;	       /*将发送长度请0*/  	 
        pDevice->TxWaiting = FALSE;   /*"当链路层发送帧时，将该标志位置TRUE，当串口已经发送完该帧数据，将回调该函数，将该标志清0"*/
        pDevice->TxOver = TRUE;

        pDLStatus = &mstDLStatus[DeviceId];
        if(1 == pDLStatus->flag.bits.confirmToAL)
        {/*"当前链路分帧结束(不考虑应用层APDU有多个分帧)"*/
            pDLStatus->flag.bits.confirmToAL = 0;
            DL698_ClrBlockInfo(DeviceId);
        }

        DL698_EnReceive(DeviceId);
    }
    else
    {
    }
}

/*"时基比较函数，模块task中调用一次"*/
static void Dlt698_TimeOut (void)
{
    INT8U DeviceIndex; 
    ST_DLT698_DEVICE *pDevice;
    ST_DL_STATUS *pDLStatus;

    for(DeviceIndex = 0; DeviceIndex < E_MAX_COM_ID; DeviceIndex++)
    {
        pDevice = &mstDlt698Device[DeviceIndex];

        /*"字节间超时"*/
        if((FALSE == pDevice->InterruptClose) && (pDevice->InterOctetTimer > 0))
        {	
            //if(TRUE == MCU_CmpTick(pDevice->InterOctetTimer, 30))
            if(TRUE == MCU_CmpTick(pDevice->InterOctetTimer, INTER_OCTET_TIMEOUT))
            {
                pDevice->InterOctetTimeout = TRUE;

                pDevice->InterruptClose = TRUE;/*"将该串口的中断关闭，即串口接收到的字节不能够在赋给Device[i]的接收缓冲区"*/						
                pDevice->InterOctetTimer = 0;       /*"将字节间超时置0"*/  
                MinTimeFlg[DeviceIndex] = TRUE;
            }		
            else
            {
                MinTimeFlg[DeviceIndex] = FALSE;
            }
        }	
        
        if(pDevice->InactivityTimer > 0)/*"帧超时"*/
        {
            if(MCU_CmpTick(pDevice->InactivityTimer, INACTIVITY_TIMEOUT) == TRUE)
            {		
                pDevice->InactivityTimer = 0;
                pDevice->InactivityTimeout = TRUE;			

                ShowStatusClr(DeviceIndex);
            }	
        }

        pDLStatus = &mstDLStatus[DeviceIndex];
        if((DL698_BLOCK == pDLStatus->action) && 
        (TRUE == MCU_CmpTick(pDLStatus->timeClient, REC_RES_TIMEOUT)))
        {/*"处理链路分帧确认超时"*/
            DL698_ClrBlockInfo(DeviceIndex);
        }
        else if((DL698_RESPONSE == pDLStatus->action) &&
        (TRUE == MCU_CmpTick(pDLStatus->timeAL, AL_RES_TIMEOUT)))
        {/*"应用层响应超时，重新启动接收"*/
            pDLStatus->action = DL698_IDLE;
            DL698_EnReceive(DeviceIndex);
        }
        else
        {
        }
    }
}

/*"*************************************************************"*/
/*"Function: 	DL698_ClrBlockInfo"*/
/*"Description: 清除链路分帧信息"*/
/*"Input:  DeviceId  设备号"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"*************************************************************"*/
static void DL698_ClrBlockInfo(INT8U DeviceId)
{
    mstDLStatus[DeviceId].action = DL698_IDLE;
    mstDLStatus[DeviceId].flag.byte = 0;
    //mstFrmInfo[DeviceId].flag.bits.Asw = 0;/*"确认标志清零，避免应用层发送下个分帧"*/
    mstFrmInfo[DeviceId].blockNo = 0;
    mstFrmInfo[DeviceId].rALBuffOffset = 0;

    DL698_EnReceive(DeviceId);
}

static void DL698_EnReceive(INT8U DeviceId)
{
    ST_DLT698_DEVICE *pDevice;

    /*"接收缓冲区处理"*/
    pDevice = &mstDlt698Device[DeviceId];
    pDevice->InterOctetTimer = 0;
    pDevice->InterOctetTimeout = FALSE;
    pDevice->RxLen = 0;
    pDevice->InterruptClose = FALSE;	
}

/*"*************************************************************"*/			
/*"Function: 	DL698_SendStart		"*/
/*"Description: 链路层准备发送数据"*/
/*"Input:  nChannel：通道号"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"*************************************************************"*/
static void DL698_SendStart(INT8U DeviceId)
{
    INT8U *pSendBuff;
    INT16U frmLen, allLen, apduLen, headFcs, i;
    ST_FRM_INFO *pFrmInfo;
    ST_DLT698_DEVICE *pDevice;
    ST_DL_STATUS *pDLStatus;
    INT8U SCcode;

    if(DeviceId >= E_MAX_COM_ID)
    {
        return;
    }

    pFrmInfo = &mstFrmInfo[DeviceId];
    pDevice = &mstDlt698Device[DeviceId];
    pDLStatus = &mstDLStatus[DeviceId];

    if((NULL == pFrmInfo->pAPDU) || (0 == pFrmInfo->sLen))
    {
        DL698_DLInit(DeviceId);
    }
    else
    {
        pSendBuff = pDevice->TxBuff;
        pSendBuff[0] = 0xFE;
        pSendBuff[1] = 0xFE;
        pSendBuff[2] = 0xFE;
        pSendBuff[3] = 0xFE;

        pSendBuff[SEND_START + 0] = 0x68;
    
        (pFrmInfo->ctrl).bits.DIR = 1;

        if(pFrmInfo->sLen > MaxApduLen(DeviceId)) /*"数据长度超链路发送尺寸"*/
        {
            pDLStatus->flag.bits.nextFrm = 1;
        }
        else
        {
            if((2 == pFrmInfo->sLen) && (0x80 == (*(pFrmInfo->pAPDU + 1) & 0xC0)))
            {/*"应用层对下行分帧的确认"*/
                (pFrmInfo->ctrl).bits.FRM = 1;
            }
            else
            {
                (pFrmInfo->ctrl).bits.FRM = 0;
            }
        }

        if(1 == pDLStatus->flag.bits.nextFrm)
        {
            (pFrmInfo->ctrl).bits.FRM = 1;
        }

        pDLStatus->flag.bits.end = 0;
        if(pFrmInfo->sLen > MaxApduLen(DeviceId))
        {
            apduLen = MaxApduLen(DeviceId);
        }
        else
        {
            apduLen = pFrmInfo->sLen;
            if(1 == pDLStatus->flag.bits.nextFrm)
            {/*"分帧的最后帧"*/
                pDLStatus->flag.bits.end = 1;
            }
        }

        pSendBuff[SEND_START + 3] = pFrmInfo->ctrl.byte;
        
        /*"本表地址"*/
        pSendBuff[SEND_START + 4] = MAX_ADDR_LEN - 1; 
        LIB_Reverse(mMtrAddr, &pSendBuff[SEND_START + 5], MAX_ADDR_LEN);

        frmLen = 6 + pSendBuff[SEND_START + 4];
        allLen = apduLen + frmLen + 4;

        pSendBuff[SEND_START + frmLen] = pFrmInfo->clientAddr;
        headFcs = frmLen;

        if(pFrmInfo->ctrl.bits.SC)/*"扰码标志为1"*/  
        {
            SCcode = 0x33;
        }
        else
        {
            SCcode = 0;
        }        

        frmLen += 3;
        if(1 == pDLStatus->flag.bits.nextFrm)
        {/*"分帧格式域"*/
            pSendBuff[SEND_START + frmLen] = (INT8U)((pFrmInfo->blockNo & 0x00FF) + SCcode); //from 0 to 4095
            pSendBuff[SEND_START + frmLen + 1] = (INT8U)(((pFrmInfo->blockNo >> 8) & 0x0F) + SCcode); 

            if(1 == pDLStatus->flag.bits.end)/*"最后帧"*/
            {
                pSendBuff[SEND_START + frmLen + 1] |= BLOCK_END;
                pDLStatus->flag.bits.nextFrm = 0;
                pDLStatus->flag.bits.confirmToAL = 1;
            }
            else if(pFrmInfo->blockNo > 0)/*"中间帧"*/
            {
                pSendBuff[SEND_START + frmLen + 1] |= BLOCK_MIDDLE;
            }
            else/*"起始帧"*/
            {
            }

            frmLen += 2;
            allLen += 2;
        }

        LIB_MemCpy((INT8U *)&allLen, &pSendBuff[SEND_START + 1], 2);    
        FillFCS16(&pSendBuff[SEND_START + 1], headFcs, &pSendBuff[SEND_START + headFcs + 1]); 	

        //LIB_MemCpy(pFrmInfo->pAPDU, &pSendBuff[SEND_START + frmLen], apduLen);
        for(i = 0; i < apduLen; i++)
        {
            pSendBuff[SEND_START + frmLen + i] = pFrmInfo->pAPDU[i] + SCcode;
        }

        frmLen += apduLen - 1;
        FillFCS16(&pSendBuff[SEND_START + 1], frmLen, &pSendBuff[SEND_START + frmLen + 1]); 
        pSendBuff[SEND_START + frmLen + 3] = 0x16;

        if(1 == pDLStatus->flag.bits.nextFrm)/*"有后续帧时，除最后1帧外必须等待客户机确认信息"*/
        {
            pDLStatus->action = DL698_BLOCK;
        }
        else
        {
            pDLStatus->action = DL698_IDLE;
        }

        pFrmInfo->sLen -= apduLen;
        if(pFrmInfo->sLen > 0)
        {
            pFrmInfo->pAPDU += apduLen;
        }
    
        if(pDevice->TxWaiting == FALSE)
        {
            //ShowStatusSet(DeviceId);

            pDevice->TxLen = SEND_START + frmLen + 4;
            pDevice->TxWaiting = TRUE;  
            COM_StartSend(pDevice->CurrentDeviceId, pDevice->TxBuff, pDevice->TxLen);

            if(1 == pDLStatus->flag.bits.nextFrm)
            {
                pDLStatus->timeClient = MCU_GetTick(); /*"启动响应超时"*/ 
            }
        }
    }
}

#if 0
static void BlockFrmAck(INT8U DeviceId)
{
    ST_FRM_INFO *pFrmInfo;
    INT8U sendBuff[BLOCK_FMT_LEN];

    pFrmInfo = &mstFrmInfo[DeviceId];
    pFrmInfo->pAPDU = &sendBuff[0];
    pFrmInfo->sLen = BLOCK_FMT_LEN;

    sendBuff[0] = pFrmInfo->blockNo & 0xFF;
    sendBuff[1] = ((INT8U)(pFrmInfo->blockNo >> 8) & 0x0F) | BLOCK_ACK;

    DL698_SendStart(DeviceId);
}
#endif

#if 0
/*"如果接收到一帧数据，对该帧进行处理"*/
static INT8U DLT698_ProcessRecBytes(INT8U *pDataBuff, INT16U length, INT8U DeviceId)
{
    INT16U FrameLen;
    INT8U nPosData;
    INT16U CheckFCS;
    ST_FRM_INFO *pFrmInfo;

    /*"协议识别"*/
    if(pDataBuff[0] == 0x68)
    {/*"68 17 00 43 05 11 11 11 11 11 11 10 6A 36 05 01 00 40 00 02 00 00 56 1F 16"*/
        pFrmInfo = &mstFrmInfo[DeviceId];
        FrameLen = (INT16U)pDataBuff[1] + ((INT16U)(pDataBuff[2] & 0x3F) << 8);

        if(((FrameLen + 2) <= length) && (pDataBuff[FrameLen + 1] == 0x16))
        {
            pFrmInfo->rLen = FrameLen;

            /*"检查698帧头"*/ 
            nPosData = CheckFrmHead(&pDataBuff[1], pFrmInfo);
            if(0 == nPosData)
            {
                return FALSE;   /*"帧头无效"*/ 
            }

            /*"检查FCS校验"*/  
            *((INT8U *)&CheckFCS) = pDataBuff[FrameLen - 1];
            *((INT8U *)&CheckFCS + 1) = pDataBuff[FrameLen];
                    
            if( CheckFCS != CalcFCS16(PPPINITFCS16, (pDataBuff + 1), FrameLen - 2))
            {
                return FALSE;   /*"FCS校验错"*/ 
            }

            /*"APDU数据起始位置"*/ 
            pFrmInfo->rHeadPos = nPosData;

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;
}
#endif

#define FRM_HEAD_MAX  (16) /*"含分帧格式域2字节，6字节地址"*/

#if 0
/*"如果接收到一帧数据，对该帧进行处理"*/
static INT8U DLT698_ProcessRecBytes(INT8U *pDataBuff, INT16U length, INT8U DeviceId)
{
    INT16U FrameLen;
    INT16U nPosData, nPosTmp;
    INT16U CheckFCS;
    ST_FRM_INFO *pFrmInfo;
    INT16U i, j, maxSize;
    INT8U frmHeadBuff[FRM_HEAD_MAX];

    if(NULL == pDataBuff)
    {
        return FALSE;
    }

    maxSize = mCommBuffMax[DeviceId];

    for(i = 0; i < length; i++)
    {
        if(pDataBuff[i] == 0x68)/*"协议识别"*/
        {/*"68 17 00 43 05 11 11 11 11 11 11 10 6A 36 05 01 00 40 00 02 00 00 56 1F 16"*/
            pFrmInfo = &mstFrmInfo[DeviceId];

            CycleDataCopy(pDataBuff, i, maxSize, FRM_HEAD_MAX, frmHeadBuff, 0);
            FrameLen = (INT16U)frmHeadBuff[1] + ((INT16U)(frmHeadBuff[2] & 0x3F) << 8);

            nPosTmp = i + FrameLen + 1;/*"帧尾"*/
            if(nPosTmp >= maxSize)
            {
                nPosTmp %= maxSize;
            }

            if(((FrameLen + 2) <= length) && (pDataBuff[nPosTmp] == 0x16))
            {
                pFrmInfo->rLen = FrameLen;

                nPosData = CheckFrmHead(&frmHeadBuff[1], pFrmInfo);/*"检查698帧头"*/ 
                if(0 == nPosData)
                {
                    //WDT_FeedWdg();
                    continue;   /*"帧头无效时退出继续查找帧头"*/ 
                }

                for(j = 0; j < sizeof(INT16U); j++)
                {
                    nPosTmp = CAL_BUFFADD(i, (FrameLen - 1 + j), maxSize);
                    *((INT8U *)&CheckFCS + j) = pDataBuff[nPosTmp];/*"FCS校验"*/  
                }

                if( CheckFCS != CalcFCS16( PPPINITFCS16, pDataBuff, (i + 1), maxSize, (FrameLen - 2)))
                {
                    continue;   /*"FCS校验错时退出继续查找帧头"*/ 
                }

                pFrmInfo->rHeadPos = CAL_BUFFADD(i, nPosData, maxSize);/*"APDU数据起始位置"*/ 

                return TRUE;
            }
            else
            {
            }
        }
    }

    return FALSE;
}


/*"*************************************************************"*/			
/*"Function: 	CycleDataCopy	"*/
/*"Description: 拷贝指定通信地址数据"*/
/*"Input:  inBuff：输入缓冲区地址"*/
/*"           inIndex：输入缓冲区首地址索引"*/
/*"           inBufSize：输入缓冲区最大长度(超过时翻转)"*/
/*"           lenth：拷贝数据长度"*/
/*"           code：扰码为33H时减处理"*/
/*"Output: outBuff：输出缓冲区地址"*/
/*"Return: 无"*/
/*"Others: "*/
/*"*************************************************************"*/
static void CycleDataCopy(INT8U *inBuff, INT16U inIndex, INT16U inBufSize, INT16U lenth, INT8U *outBuff, INT8U code)
{
    INT16U i;

    if((NULL == inBuff) || (NULL == outBuff))
    {
        return;
    }

    for(i = 0; i < lenth; i++)
    {
        outBuff[i] = inBuff[(inIndex + i) % inBufSize];
        if(0x33 == code)
        {
            outBuff[i] -= code;
        }
    }
}

/*"*************************************************************"*/			
/*"Function: 	CheckFrmHead		"*/
/*"Description: 检查帧头合法性"*/
/*"Input:  *rpAddress：数据地址；"*/
/*"Output: *pstFrmRst：帧解析信息"*/
/*"Return: 用户数据的地址偏移"*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CheckFrmHead(INT8U *rpAddress, ST_FRM_INFO *pstFrmRst)
{
    INT8U addrPro, addrLen, logicAddr;
    INT8U *pAddr;
    INT16U hcs;
    INT8U offset;
    INT8U i, nIndex;
    INT8U addr[MAX_ADDR_LEN];
    INT8U code1, code2;
    INT8U isMyAddr;
    UN_CTRL_698  *pCtrl;
    
    offset = 0;
    pstFrmRst->addrType = ADDR_UNRESOLVED;
    addrLen = (rpAddress[3] & 0x0F) + 1;/*"地址字节长度"*/
    addrPro = (rpAddress[3] & 0xC0) >> 6;/*"地址类别: 0 - 3"*/
    logicAddr = (rpAddress[3] & 0x30) >> 4;/*"逻辑地址"*/ 
    pAddr = &rpAddress[4];
    pCtrl = (UN_CTRL_698 *)&rpAddress[2];

    if((0 != pCtrl->bits.DIR) || (addrLen > MAX_ADDR_LEN) || /*"不是客户机的请求或响应，地址长度超"*/
    (0 != logicAddr))/*"逻辑地址必须为0"*/ 
    {
        return (0);
    }
    
    if(BROADCAST == addrPro)/*"广播地址"*/
    {
        if((1 == addrLen) && (0xAA == *pAddr))
        {
            pstFrmRst->addrType = ADDR_BROADCAST;
        }
    }
    else //if(addrPro <= GROUP)
    {
        LIB_MemSet(0x00, addr, MAX_ADDR_LEN);
        LIB_Reverse(pAddr, addr, addrLen);

        if(addrPro <= WILDCARD)/*"单地址_0 or 通配地址_1"*/
        {
            if(SINGLE == addrPro) /*"检查本表地址"*/
            {
                if(EQUAL == LIB_MemCmp(mMtrAddr, addr, MAX_ADDR_LEN))
                {
                    pstFrmRst->addrType = ADDR_SELFADDR;
                }
            }
            else/*"检查通配地址"*/
            {
                isMyAddr = TRUE;

                for (i = 0; i < (MAX_ADDR_LEN << 1); i++)
                {
                    nIndex = i >> 1;
                    if((i & 0x01) == 0)
                    {
                        code1 = addr[nIndex] & 0x0F;
                        code2 = mMtrAddr[nIndex] & 0x0F;
                    }
                    else
                    {
                        code1 = (addr[nIndex] >> 4) & 0x0F;
                        code2 = (mMtrAddr[nIndex] >> 4) & 0x0F;
                    }

                    if((code1 != 0x0A) && (code1!= code2))
                    {
                        isMyAddr = FALSE;
                        break;
                    }
                }

                if(TRUE == isMyAddr)
                {
                    pstFrmRst->addrType = ADDR_ABBREVIATE;
                }
            }
        }
        else //if(GROUP == addrPro)/*"组地址"*/
        {
            if(EQUAL == LIB_MemCmp(mGroupAddr, addr, MAX_ADDR_LEN)) /*"检查组地址"*/
            {
                pstFrmRst->addrType = ADDR_GROUP;
            }
        }
    }
    
    if(ADDR_UNRESOLVED != pstFrmRst->addrType)/*"地址有效"*/
    {
        if(3 == pCtrl->bits.Func)/*"用户数据交换服务(客户机请求或响应)"*/
        {
            addrLen += 5;
            
            *((INT8U *)&hcs) = rpAddress[addrLen];
            *((INT8U *)&hcs + 1) = rpAddress[addrLen + 1];
            
            if(hcs == CalcFCS16(PPPINITFCS16, rpAddress, 0, 0xFFFF, addrLen))/*"帧校验正确"*/
            {
                pstFrmRst->clientAddr = rpAddress[addrLen - 1];/*"客户机地址"*/
                pstFrmRst->rLen -= (addrLen + 4);
                pstFrmRst->ctrl.byte = rpAddress[2];

                pstFrmRst->flag.bits.DownNextFrm = 0;
                pstFrmRst->flag.bits.Asw = 0;
                if(1 == pCtrl->bits.FRM)/*"分帧标志"*/
                {
                    addrLen += BLOCK_FMT_LEN;/*"分帧格式域长度为2"*/
                    if(pstFrmRst->rLen > BLOCK_FMT_LEN)/*"分帧标志"*/
                    {
                        pstFrmRst->flag.bits.DownNextFrm = 1;
                        pstFrmRst->rLen -= BLOCK_FMT_LEN;
                    }
                    else if(BLOCK_FMT_LEN == pstFrmRst->rLen)
                    {/*"主站分帧确认"*/  
                        pstFrmRst->flag.bits.Asw = 1;
                    }
                    else
                    {
                        return (0);
                    }

                    if(pstFrmRst->ctrl.bits.SC)/*"扰码标志为1"*/
                    {
                        rpAddress[addrLen] -= 0x33;
                        rpAddress[addrLen + 1] -= 0x33;
                    }

                    pstFrmRst->blockFmt =  ((INT16U)rpAddress[addrLen + 1] << 8) | rpAddress[addrLen];
                }
                else
                {
                    pstFrmRst->blockFmt =  0;
                }

                offset = addrLen + 3;
            }
        }
    }

    return offset;
}
#endif

static void ShowStatusSet(INT8U DeviceId)
{
    mShowFlg[DeviceId] = TRUE;

    if(E_COM_SLAVE == DeviceId)
    {
        DIS_SetStauSeg(STA_WAVE);

        Push(E_PLM_WORKING, TRUE);
    }
    else
    {
        DIS_SetStauSeg(STA_PHONE);


#ifdef THREE_PHASE
        else if(E_COM_RS485_1 == DeviceId)
        {
            DIS_SetStauSeg(STA_RS485_1);
        }
        else//E_COM_RS485_2
        {
            DIS_SetStauSeg(STA_RS485_2);
        }
#endif
    }


}

static void ShowStatusClr(INT8U DeviceId)
{
    mShowFlg[DeviceId] = FALSE;

    if(E_COM_SLAVE == DeviceId)
    {
        DIS_ClrStauSeg(STA_WAVE);
    }
#ifdef THREE_PHASE
    else if(E_COM_RS485_1 == DeviceId)
    {
        DIS_ClrStauSeg(STA_RS485_1);
    }
    else if(E_COM_RS485_2 == DeviceId)
    {
        DIS_ClrStauSeg(STA_RS485_2);
    }
    else
    {
    }

    if((FALSE == mShowFlg[E_COM_RS485_1]) && 
    (FALSE == mShowFlg[E_COM_RS485_2]))
    {
        DIS_ClrStauSeg(STA_PHONE);
    }
#else
    else if((FALSE == mShowFlg[E_COM_RS485_1]) )
    {
        DIS_ClrStauSeg(STA_PHONE);
    }
#endif
    else
    {
    }
}

void Com_Send_data(INT8U com, INT8U *pkt, INT8U len)
{
    ST_DLT698_DEVICE *pDevice;
    
    pDevice = &mstDlt698Device[com];
    LIB_MemCpy(pkt, pDevice->TxBuff, len);
    pDevice->TxLen = len;
    pDevice->TxWaiting = TRUE;  
    COM_StartSend(com, pDevice->TxBuff, pDevice->TxLen);
}
