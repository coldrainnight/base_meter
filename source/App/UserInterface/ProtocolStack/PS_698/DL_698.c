
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

#define DL698_IDLE      (0)/*"��·����"*/
#define DL698_RESPONSE  (1)/*"�ȴ�Ӧ�ò���Ӧ"*/
#define DL698_BLOCK  (2)/*"��·���з�֡"*/

#define REC_OVER_APDU   (-1)/*"APDUƬ���ۼӳ������Ӧ�ò㻺����"*/
#define REC_INVALID         (0)/*"����Ч����"*/
#define REC_BLOCK            (1)/*"����ΪAPDUƬ��"*/
#define REC_OVER              (2)/*"�������"*/

#define BLOCK_START         (0x00)/*"��ʼ֡"*/
#define BLOCK_MIDDLE       (0xC0)/*"�м�֡"*/
#define BLOCK_END             (0x40)/*"����֡"*/
#define BLOCK_ACK             (0x80)/*"ȷ��֡(�ͻ�����Ӧ)"*/

#define INTER_OCTET_TIMEOUT (100)/*"�ֽڼ䳬ʱ"*/
#define INACTIVITY_TIMEOUT (1000)/*"�޻��ʱ (֡��ʱ)"*/
#define REC_RES_TIMEOUT      (10000)/*"(����֡)���տͻ�����Ӧ��ʱ3s"*/
#define AL_RES_TIMEOUT       (500)/*"����Ӧ�ò���Ӧ��ʱ500ms"*/

#define TYPE_OF_SERVICE      (3) /*"��·����ķ�������"*/
#define PRM_SERVER_REQUEST   (0) /*"��������������ϱ�"*/
#define PRM_SERVER_RESPONSE  (1) /*"����������Ӧ"*/
#define PRM_CLIENT_RESPONSE  (0) /*"�ͻ�������Ӧ"*/
#define CTRL_CLIENT_DL_RES   (0x01) /*"�ͻ�������·��Ӧ"*/


//#define MIN_698_FRM_LEN    (13) /*"698: 1�ֽڵ�ַ��1�ֽ�APDU"*/
//#define MIN_645_FRM_LEN    (12) /*"645: ��DATA"*/
#define MIN_FRM_LEN    (12) /*"645: ��DATA"*/
#define SA_OFFSET         (4)  /*"698: SA��֡�е�ƫ��"*/
#define SECOND_68H_OFFSET  (7)  /*"645: ��2��68H��֡�е�ƫ��"*/
#define FRM_HEAD_LEN  (16) /*"����֡��ʽ��2�ֽڣ�6�ֽڵ�ַ��16�ֽڵ�ַʱ֡ͷ����Ϊ24"*/
#define BLOCK_FMT_LEN  (2) /*"��֡��ʽ�򳤶�"*/
#define SEND_START      (4) /*"ǰ���ֽڳ���"*/
#ifdef THREE_PHASE
#define IR_WAKE_UP_LEN  (2)/*"���⻽���ִ�����"*/
#endif

#define DLT645_BUFF_LEN  (180)/*"645Э��֡��ʱ��������󳤶�"*/

#define FRM_DLT698     (1)
#define FRM_DLT645     (2)

#define DLT645_FIXED_REC_LEN   (12)


INT8U mBpsChangeFlg[E_MAX_COM_ID];  /*"bps�޸ı�ʶ"*/
#ifdef THREE_PHASE
INT8U mPdIrWakeUpFlg;/*"�͹��ĺ��⻽�ѱ�־"*/
#endif

static ST_DLT698_DEVICE mstDlt698Device[E_MAX_COM_ID];/*"�豸������Ϣ"*/
static ST_DL_STATUS mstDLStatus[E_MAX_COM_ID];/*"ͨ����·״̬"*/
static ST_FRM_INFO mstFrmInfo[E_MAX_COM_ID];/*"��·֡��Ϣ"*/
static INT8U MinTimeFlg[E_MAX_COM_ID];/*"������������С�����־"*/
static INT8U mShowFlg[E_MAX_COM_ID];/*"ͨ��״̬��ʾ��־"*/


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
/*"Description: ����698֡HCS��FCS"*/
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
/*"Description: ����698֡HCS��FCS"*/
/*"Input:      fcs: Initial FCS value  "*/
/*"               pBuff: ���ݻ������׵�ַ"*/
/*"               head: �׵�ַƫ��"*/
/*"               max: ��������󳤶ȣ�maxΪ0xFFFFʱ��ʾ����ע�������"*/
/*"               len: ����У���������ݳ���"*/
/*"Output: 	"*/
/*"Return: FCSУ��ֵ"*/
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
/*"Description: ����698֡��HCS��FCSλ�����У����"*/
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
        {/*"�͹����½�֧�ֺ���ͨ��"*/
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
    //INT8U SCcode;/*"�����־"*/

    Dlt698_TimeOut();

    for(DeviceIndex = 0; DeviceIndex < E_MAX_COM_ID; DeviceIndex++)
    {
#ifdef THREE_PHASE
        if((TRUE == ChkStatus(E_LOW_PWR)))
        {/*"�͹����½�֧�ֺ���ͨ��"*/
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
        {/*"ͨ���ܳ�ʱ"*/	
            pDevice->TxWaiting = FALSE;  
            pDevice->InactivityTimeout = FALSE;	
        }	

        if((TRUE == pDevice->InterOctetTimeout) && (TRUE == MinTimeFlg[DeviceIndex]))
        {/*"�����ֽڼ䳬ʱ, �������֡,���ؽ������"*/
            if((TRUE == ChkStatus(E_LOW_PWR)))
            {
                DL698_EnReceive(DeviceIndex);
                continue;
            }

            pDevice->frmType = 0;
            /*"698Э�����"*/
            //result = DLT698_ProcessRecBytes(pDevice->RxBuff, pDevice->RxLen, DeviceIndex);	
            result = FALSE;
#if 0
            if(TRUE == result)
            {
                ShowStatusSet(DeviceIndex);

                pFrmInfo->retToAL = REC_INVALID;

                blockFlg = (INT8U)(pFrmInfo->blockFmt >> 8) & 0xC0;
                blockNo = pFrmInfo->blockFmt & 0x0FFF;

                if((DL698_BLOCK == pDLStatus->action) && /*"���з�֡��������·��֡ȷ��"*/
                (pFrmInfo->flag.bits.Asw)) /*"�յ���֡ȷ��"*/
                {
                    if((blockNo == pFrmInfo->blockNo) &&
                    (((pFrmInfo->blockFmt >> 8) & 0xC0) == BLOCK_ACK))/*"��֡ȷ��"*/
                    {
                        pFrmInfo->blockNo += 1;

                        DL698_SendStart(DeviceIndex);/*"׼���¸���֡����"*/
                    }
                    else/*"��֡��Ŵ���"*/
                    {
                        DL698_ClrBlockInfo(DeviceIndex);
                    }
                }
                else if(pFrmInfo->flag.bits.DownNextFrm)
                {/*"���з�֡���ǽ���֡ʱ����·�㷵��ȷ��"*/
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

                    if(pFrmInfo->ctrl.bits.SC)/*"�����־Ϊ1����·�û����ݼ�0x33"*/
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
                        BlockFrmAck(DeviceIndex);/*"���з�֡ȷ��"*/
                        pFrmInfo->retToAL = REC_INVALID;
                    }
                    else if(REC_OVER == pFrmInfo->retToAL)/*"APDU�践��Ӧ�ò�"*/
                    {
#if 0 
                        if(BLOCK_END == blockFlg)
                        {/*"���з�֡������Ӧ�ò��޷��أ���������"*/
                            DL698_EnReceive(DeviceIndex);
                        }
                        else
#endif
                        {
                            pDLStatus->action = DL698_RESPONSE; /*"�ȴ�Ӧ�ò����Ӧ"*/
                            pDLStatus->timeAL = MCU_GetTick(); /*"����Ӧ�ò���Ӧ��ʱ"*/
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
            {/*"645Э�����:  �ȸ������ݣ��ٸ�λ�շ�������"*/	
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

        if(TRUE == pDevice->TxOver)/*"���ͽ���ʱ�Ĵ���"*/
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

            if((ADDR_BROADCAST == pFrmInfo->addrType) || /*"�㲥��ַ��Ӧ��"*/
            (ADDR_GROUP == pFrmInfo->addrType) || /*"���ַ��Ӧ��"*/
            (PRM_CLIENT_RESPONSE == pFrmInfo->ctrl.bits.PRM)) /*"�ͻ�������Ӧ��Ӧ��"*/
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
    (ADDR_BROADCAST == mstFrmInfo[ComId].addrType) ||/*"�㲥��ַ���Ͳ�Ӧ��"*/
    (ADDR_GROUP == mstFrmInfo[ComId].addrType))/*"���ַ���Ͳ�Ӧ��"*/
    {
        return FALSE;
    }
    
    mstFrmInfo[ComId].blockNo = 0;/*"��֡��ų�ʼ��"*/
    mstDLStatus[ComId].flag.bits.confirmToAL = 0;

    mstFrmInfo[ComId].sLen = SendApduLen;
    mstFrmInfo[ComId].pAPDU = PApduBuf;
    mstFrmInfo[ComId].ctrl.bits.FRM = 0;

    DL698_SendStart(ComId);

    return TRUE;
}

/*"*************************************************************"*/
/*"Function: 	DL698_DLInit		"*/
/*"Description: ͨ����·��ʼ��"*/
/*"Input:  ComId ��ͨ�ſڱ��"*/
/*"Output: ��	"*/
/*"Return: ��"*/
/*"Others: 		"*/
/*"*************************************************************"*/
void DL698_DLInit(INT8U ComId)
{
    UN_UARTFORM UartFormTmp;
    INT8U bpstemp;

    /*"ͨ���豸��ʼ��"*/
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

    /*"ͨ��ģ���ڲ����ݳ�ʼ��"*/       
    mBpsChangeFlg[ComId] = FALSE;

    mstDLStatus[ComId].action = DL698_IDLE;
    mstDLStatus[ComId].flag.byte = 0;

    LIB_MemSet(0, (INT8U *)&mstFrmInfo[ComId], sizeof(ST_FRM_INFO));
    MinTimeFlg[ComId] = FALSE;
    mShowFlg[ComId] = FALSE;
}

/*"�����жϺ���������ʵ�ֽ���һ���ֽ�"*/
static void Dlt698_Receive (INT8U DeviceId, INT8U Byte)
{
    ST_DLT698_DEVICE *pDevice;

    if(DeviceId >= E_MAX_COM_ID)
    {
        return;
    }

    pDevice = &mstDlt698Device[DeviceId];
    if(pDevice->InterruptClose == FALSE)/*"ֻ�д��ڵĽ����жϱ�־λ�ǹرյģ����ܹ������յ���������ӵ����ջ�����"*/
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
        pDevice->RxBuff[pDevice->RxLen] = Byte; /*"���������ֽڸ������ջ�����"*/
        pDevice->RxLen++;                        /*"���յ��ĳ���+1"*/	
        
        pDevice->InterOctetTimer = MCU_GetTick();/*"���¸��ֽڼ䳬ʱʱ�丳ֵ"*/    
        if(0 == pDevice->InterOctetTimer)
        {
            pDevice->InterOctetTimer = 1;			
        }
    } 
    else
    {
    }
}

/*"���ͽ����ص��������������Ѿ�����֡���ݷ��ͳ�ȥ֮��Ҫ�ص��ú���"*/
static void Dlt698_SendOver (INT8U DeviceId)
{
    ST_DLT698_DEVICE *pDevice;
    ST_DL_STATUS *pDLStatus;

    if(DeviceId < E_MAX_COM_ID)
    {
        pDevice = &mstDlt698Device[DeviceId];

        pDevice->TxLen = 0;	       /*�����ͳ�����0*/  	 
        pDevice->TxWaiting = FALSE;   /*"����·�㷢��֡ʱ�����ñ�־λ��TRUE���������Ѿ��������֡���ݣ����ص��ú��������ñ�־��0"*/
        pDevice->TxOver = TRUE;

        pDLStatus = &mstDLStatus[DeviceId];
        if(1 == pDLStatus->flag.bits.confirmToAL)
        {/*"��ǰ��·��֡����(������Ӧ�ò�APDU�ж����֡)"*/
            pDLStatus->flag.bits.confirmToAL = 0;
            DL698_ClrBlockInfo(DeviceId);
        }

        DL698_EnReceive(DeviceId);
    }
    else
    {
    }
}

/*"ʱ���ȽϺ�����ģ��task�е���һ��"*/
static void Dlt698_TimeOut (void)
{
    INT8U DeviceIndex; 
    ST_DLT698_DEVICE *pDevice;
    ST_DL_STATUS *pDLStatus;

    for(DeviceIndex = 0; DeviceIndex < E_MAX_COM_ID; DeviceIndex++)
    {
        pDevice = &mstDlt698Device[DeviceIndex];

        /*"�ֽڼ䳬ʱ"*/
        if((FALSE == pDevice->InterruptClose) && (pDevice->InterOctetTimer > 0))
        {	
            //if(TRUE == MCU_CmpTick(pDevice->InterOctetTimer, 30))
            if(TRUE == MCU_CmpTick(pDevice->InterOctetTimer, INTER_OCTET_TIMEOUT))
            {
                pDevice->InterOctetTimeout = TRUE;

                pDevice->InterruptClose = TRUE;/*"���ô��ڵ��жϹرգ������ڽ��յ����ֽڲ��ܹ��ڸ���Device[i]�Ľ��ջ�����"*/						
                pDevice->InterOctetTimer = 0;       /*"���ֽڼ䳬ʱ��0"*/  
                MinTimeFlg[DeviceIndex] = TRUE;
            }		
            else
            {
                MinTimeFlg[DeviceIndex] = FALSE;
            }
        }	
        
        if(pDevice->InactivityTimer > 0)/*"֡��ʱ"*/
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
        {/*"������·��֡ȷ�ϳ�ʱ"*/
            DL698_ClrBlockInfo(DeviceIndex);
        }
        else if((DL698_RESPONSE == pDLStatus->action) &&
        (TRUE == MCU_CmpTick(pDLStatus->timeAL, AL_RES_TIMEOUT)))
        {/*"Ӧ�ò���Ӧ��ʱ��������������"*/
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
/*"Description: �����·��֡��Ϣ"*/
/*"Input:  DeviceId  �豸��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: 		"*/
/*"*************************************************************"*/
static void DL698_ClrBlockInfo(INT8U DeviceId)
{
    mstDLStatus[DeviceId].action = DL698_IDLE;
    mstDLStatus[DeviceId].flag.byte = 0;
    //mstFrmInfo[DeviceId].flag.bits.Asw = 0;/*"ȷ�ϱ�־���㣬����Ӧ�ò㷢���¸���֡"*/
    mstFrmInfo[DeviceId].blockNo = 0;
    mstFrmInfo[DeviceId].rALBuffOffset = 0;

    DL698_EnReceive(DeviceId);
}

static void DL698_EnReceive(INT8U DeviceId)
{
    ST_DLT698_DEVICE *pDevice;

    /*"���ջ���������"*/
    pDevice = &mstDlt698Device[DeviceId];
    pDevice->InterOctetTimer = 0;
    pDevice->InterOctetTimeout = FALSE;
    pDevice->RxLen = 0;
    pDevice->InterruptClose = FALSE;	
}

/*"*************************************************************"*/			
/*"Function: 	DL698_SendStart		"*/
/*"Description: ��·��׼����������"*/
/*"Input:  nChannel��ͨ����"*/
/*"Output: ��"*/
/*"Return: ��"*/
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

        if(pFrmInfo->sLen > MaxApduLen(DeviceId)) /*"���ݳ��ȳ���·���ͳߴ�"*/
        {
            pDLStatus->flag.bits.nextFrm = 1;
        }
        else
        {
            if((2 == pFrmInfo->sLen) && (0x80 == (*(pFrmInfo->pAPDU + 1) & 0xC0)))
            {/*"Ӧ�ò�����з�֡��ȷ��"*/
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
            {/*"��֡�����֡"*/
                pDLStatus->flag.bits.end = 1;
            }
        }

        pSendBuff[SEND_START + 3] = pFrmInfo->ctrl.byte;
        
        /*"�����ַ"*/
        pSendBuff[SEND_START + 4] = MAX_ADDR_LEN - 1; 
        LIB_Reverse(mMtrAddr, &pSendBuff[SEND_START + 5], MAX_ADDR_LEN);

        frmLen = 6 + pSendBuff[SEND_START + 4];
        allLen = apduLen + frmLen + 4;

        pSendBuff[SEND_START + frmLen] = pFrmInfo->clientAddr;
        headFcs = frmLen;

        if(pFrmInfo->ctrl.bits.SC)/*"�����־Ϊ1"*/  
        {
            SCcode = 0x33;
        }
        else
        {
            SCcode = 0;
        }        

        frmLen += 3;
        if(1 == pDLStatus->flag.bits.nextFrm)
        {/*"��֡��ʽ��"*/
            pSendBuff[SEND_START + frmLen] = (INT8U)((pFrmInfo->blockNo & 0x00FF) + SCcode); //from 0 to 4095
            pSendBuff[SEND_START + frmLen + 1] = (INT8U)(((pFrmInfo->blockNo >> 8) & 0x0F) + SCcode); 

            if(1 == pDLStatus->flag.bits.end)/*"���֡"*/
            {
                pSendBuff[SEND_START + frmLen + 1] |= BLOCK_END;
                pDLStatus->flag.bits.nextFrm = 0;
                pDLStatus->flag.bits.confirmToAL = 1;
            }
            else if(pFrmInfo->blockNo > 0)/*"�м�֡"*/
            {
                pSendBuff[SEND_START + frmLen + 1] |= BLOCK_MIDDLE;
            }
            else/*"��ʼ֡"*/
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

        if(1 == pDLStatus->flag.bits.nextFrm)/*"�к���֡ʱ�������1֡�����ȴ��ͻ���ȷ����Ϣ"*/
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
                pDLStatus->timeClient = MCU_GetTick(); /*"������Ӧ��ʱ"*/ 
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
/*"������յ�һ֡���ݣ��Ը�֡���д���"*/
static INT8U DLT698_ProcessRecBytes(INT8U *pDataBuff, INT16U length, INT8U DeviceId)
{
    INT16U FrameLen;
    INT8U nPosData;
    INT16U CheckFCS;
    ST_FRM_INFO *pFrmInfo;

    /*"Э��ʶ��"*/
    if(pDataBuff[0] == 0x68)
    {/*"68 17 00 43 05 11 11 11 11 11 11 10 6A 36 05 01 00 40 00 02 00 00 56 1F 16"*/
        pFrmInfo = &mstFrmInfo[DeviceId];
        FrameLen = (INT16U)pDataBuff[1] + ((INT16U)(pDataBuff[2] & 0x3F) << 8);

        if(((FrameLen + 2) <= length) && (pDataBuff[FrameLen + 1] == 0x16))
        {
            pFrmInfo->rLen = FrameLen;

            /*"���698֡ͷ"*/ 
            nPosData = CheckFrmHead(&pDataBuff[1], pFrmInfo);
            if(0 == nPosData)
            {
                return FALSE;   /*"֡ͷ��Ч"*/ 
            }

            /*"���FCSУ��"*/  
            *((INT8U *)&CheckFCS) = pDataBuff[FrameLen - 1];
            *((INT8U *)&CheckFCS + 1) = pDataBuff[FrameLen];
                    
            if( CheckFCS != CalcFCS16(PPPINITFCS16, (pDataBuff + 1), FrameLen - 2))
            {
                return FALSE;   /*"FCSУ���"*/ 
            }

            /*"APDU������ʼλ��"*/ 
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

#define FRM_HEAD_MAX  (16) /*"����֡��ʽ��2�ֽڣ�6�ֽڵ�ַ"*/

#if 0
/*"������յ�һ֡���ݣ��Ը�֡���д���"*/
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
        if(pDataBuff[i] == 0x68)/*"Э��ʶ��"*/
        {/*"68 17 00 43 05 11 11 11 11 11 11 10 6A 36 05 01 00 40 00 02 00 00 56 1F 16"*/
            pFrmInfo = &mstFrmInfo[DeviceId];

            CycleDataCopy(pDataBuff, i, maxSize, FRM_HEAD_MAX, frmHeadBuff, 0);
            FrameLen = (INT16U)frmHeadBuff[1] + ((INT16U)(frmHeadBuff[2] & 0x3F) << 8);

            nPosTmp = i + FrameLen + 1;/*"֡β"*/
            if(nPosTmp >= maxSize)
            {
                nPosTmp %= maxSize;
            }

            if(((FrameLen + 2) <= length) && (pDataBuff[nPosTmp] == 0x16))
            {
                pFrmInfo->rLen = FrameLen;

                nPosData = CheckFrmHead(&frmHeadBuff[1], pFrmInfo);/*"���698֡ͷ"*/ 
                if(0 == nPosData)
                {
                    //WDT_FeedWdg();
                    continue;   /*"֡ͷ��Чʱ�˳���������֡ͷ"*/ 
                }

                for(j = 0; j < sizeof(INT16U); j++)
                {
                    nPosTmp = CAL_BUFFADD(i, (FrameLen - 1 + j), maxSize);
                    *((INT8U *)&CheckFCS + j) = pDataBuff[nPosTmp];/*"FCSУ��"*/  
                }

                if( CheckFCS != CalcFCS16( PPPINITFCS16, pDataBuff, (i + 1), maxSize, (FrameLen - 2)))
                {
                    continue;   /*"FCSУ���ʱ�˳���������֡ͷ"*/ 
                }

                pFrmInfo->rHeadPos = CAL_BUFFADD(i, nPosData, maxSize);/*"APDU������ʼλ��"*/ 

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
/*"Description: ����ָ��ͨ�ŵ�ַ����"*/
/*"Input:  inBuff�����뻺������ַ"*/
/*"           inIndex�����뻺�����׵�ַ����"*/
/*"           inBufSize�����뻺������󳤶�(����ʱ��ת)"*/
/*"           lenth���������ݳ���"*/
/*"           code������Ϊ33Hʱ������"*/
/*"Output: outBuff�������������ַ"*/
/*"Return: ��"*/
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
/*"Description: ���֡ͷ�Ϸ���"*/
/*"Input:  *rpAddress�����ݵ�ַ��"*/
/*"Output: *pstFrmRst��֡������Ϣ"*/
/*"Return: �û����ݵĵ�ַƫ��"*/
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
    addrLen = (rpAddress[3] & 0x0F) + 1;/*"��ַ�ֽڳ���"*/
    addrPro = (rpAddress[3] & 0xC0) >> 6;/*"��ַ���: 0 - 3"*/
    logicAddr = (rpAddress[3] & 0x30) >> 4;/*"�߼���ַ"*/ 
    pAddr = &rpAddress[4];
    pCtrl = (UN_CTRL_698 *)&rpAddress[2];

    if((0 != pCtrl->bits.DIR) || (addrLen > MAX_ADDR_LEN) || /*"���ǿͻ������������Ӧ����ַ���ȳ�"*/
    (0 != logicAddr))/*"�߼���ַ����Ϊ0"*/ 
    {
        return (0);
    }
    
    if(BROADCAST == addrPro)/*"�㲥��ַ"*/
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

        if(addrPro <= WILDCARD)/*"����ַ_0 or ͨ���ַ_1"*/
        {
            if(SINGLE == addrPro) /*"��鱾���ַ"*/
            {
                if(EQUAL == LIB_MemCmp(mMtrAddr, addr, MAX_ADDR_LEN))
                {
                    pstFrmRst->addrType = ADDR_SELFADDR;
                }
            }
            else/*"���ͨ���ַ"*/
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
        else //if(GROUP == addrPro)/*"���ַ"*/
        {
            if(EQUAL == LIB_MemCmp(mGroupAddr, addr, MAX_ADDR_LEN)) /*"������ַ"*/
            {
                pstFrmRst->addrType = ADDR_GROUP;
            }
        }
    }
    
    if(ADDR_UNRESOLVED != pstFrmRst->addrType)/*"��ַ��Ч"*/
    {
        if(3 == pCtrl->bits.Func)/*"�û����ݽ�������(�ͻ����������Ӧ)"*/
        {
            addrLen += 5;
            
            *((INT8U *)&hcs) = rpAddress[addrLen];
            *((INT8U *)&hcs + 1) = rpAddress[addrLen + 1];
            
            if(hcs == CalcFCS16(PPPINITFCS16, rpAddress, 0, 0xFFFF, addrLen))/*"֡У����ȷ"*/
            {
                pstFrmRst->clientAddr = rpAddress[addrLen - 1];/*"�ͻ�����ַ"*/
                pstFrmRst->rLen -= (addrLen + 4);
                pstFrmRst->ctrl.byte = rpAddress[2];

                pstFrmRst->flag.bits.DownNextFrm = 0;
                pstFrmRst->flag.bits.Asw = 0;
                if(1 == pCtrl->bits.FRM)/*"��֡��־"*/
                {
                    addrLen += BLOCK_FMT_LEN;/*"��֡��ʽ�򳤶�Ϊ2"*/
                    if(pstFrmRst->rLen > BLOCK_FMT_LEN)/*"��֡��־"*/
                    {
                        pstFrmRst->flag.bits.DownNextFrm = 1;
                        pstFrmRst->rLen -= BLOCK_FMT_LEN;
                    }
                    else if(BLOCK_FMT_LEN == pstFrmRst->rLen)
                    {/*"��վ��֡ȷ��"*/  
                        pstFrmRst->flag.bits.Asw = 1;
                    }
                    else
                    {
                        return (0);
                    }

                    if(pstFrmRst->ctrl.bits.SC)/*"�����־Ϊ1"*/
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
