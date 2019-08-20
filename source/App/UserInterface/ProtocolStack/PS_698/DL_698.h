#ifndef DL_698_H
#define DL_698_H

#include "system\Sys_def.h"
#include "PlatForm.h"

typedef struct                
{  	
       INT8U CurrentDeviceId; /*"当前设备的硬件端口号"*/
       
	INT8U *RxBuff;/*"接收缓冲区"*/
	INT8U *TxBuff;/*"发送缓冲区"*/	
	
	INT16U RxLen;/*"接收长度"*/
	INT16U TxLen;/*"发送长度"*/

	INT16U InactivityTimer;/*无活动超时时间，如果串口一段时间内没有活动则断开连接*/
	INT16U InterOctetTimer;	/*"字节间超时时间"*/
	
	INT8U InterruptClose:1;/*"中断关闭，只有中断关闭，才能将串口接收到的数据添加到接收缓冲区"*/	
	INT8U TxWaiting:1;     /*"发送等待"*/
	INT8U InterOctetTimeout:1;	/*字节间超时标志位，1位。*/	
	INT8U InactivityTimeout:1;  /*帧超时标志位，1位。*/	
	INT8U TxOver:1;     /*"发送结束"*/
	INT8U frmType:2;  /*"接收帧类型: 01_698; 02_645; else_invalid"*/
	INT8U Saved:1;

}ST_DLT698_DEVICE;

/*"波特率"*/
#define BPS_19200   0x40          		                   
#define BPS_9600    0x20
#define BPS_4800    0x10   
#define BPS_2400    0x08   
#define BPS_1200    0x04   
#define BPS_600     0x02
#define BPS_INVALID 0x00
#define BPS_7200    0x00 

#define MAX_ADDR_LEN      (6)
#define  mMtrAddr    StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara6Hex.CommAddr
#define  mGroupAddr    StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara6Hex.GroupAddr

#define DL698_MAX_BUFF   (RS4851_BUFF_RX_SIZE)/*"底层缓冲区最大1900"*/

/*"MaxApduLen 长度需保证最长参数数据(42字节时区表、时段表)能完整下发"*/
/*"10 + 14 * 8 = 122byte"*/
/*"17byte: 4(前导码) + 8( 帧头)+ 3(帧尾) + 2(分帧格式域)"*/
#define MaxApduLen(portNo)   (mCommBuffMax[portNo] - MAX_ADDR_LEN - 17)

#define CAL_BUFFADD(head, n, max)   ((head + n) % max)


typedef enum
{
    ADDR_UNRESOLVED    =0   ,/*"无效地址"*/
    ADDR_SELFADDR,/*"本表地址"*/
    ADDR_ABBREVIATE,       /*"通配地址"*/
    ADDR_BROADCAST,         /*"广播地址"*/
    ADDR_GROUP            /*"组地址"*/
}EN_ADDRTYPE;

typedef struct 
{
    EN_ADDRTYPE AddrType;   /*"地址类型"*/
    INT8U clientAddr; /*"客户机地址"*/
}ST_698_FRAME_INFO;

typedef enum
{
    SINGLE = 0, /*"单地址"*/
    WILDCARD,   /*"通配地址"*/
    GROUP,      /*"组地址"*/
    BROADCAST,  /*"广播地址"*/
    MAX_TYPE_ADDR
}EN_TYPE_ADDR;

typedef union /*"控制域C"*/
{
    INT8U   byte;
    
    struct
    {
        INT8U Func: 3; /*"功能码"*/
        INT8U SC:1; /*"扰码标志位"*/
        INT8U reserved:1; /*"保留"*/
        INT8U FRM:1; /*"分帧标志位"*/
        INT8U PRM:1; /*"启动标志位"*/
        INT8U DIR:1; /*"传输方向位"*/
    }bits;
}UN_CTRL_698;

typedef union
{
    INT8U byte;
    struct
    {
        INT8U DownNextFrm:1;/*"下行为分帧(APDU片段)"*/
        INT8U Asw:1;/*"主站分帧确认"*/
        INT8U reserved:4;
    }bits;
}UN_NEXT_FRAME;

typedef union
{
    INT8U byte;
    struct
    {
        INT8U nextFrm:1;/*"有后续帧"*/
        INT8U end:1;/*"0: 中间帧；1：最后帧"*/
        INT8U confirmToAL: 1;/*"确认帧返回给应用层"*/
        INT8U reserved:6;
    }bits;
}UN_DL_FRAME;

typedef struct  /*"通信链路状态"*/
{
    INT16U timeClient; /*"客户机响应数据时基"*/
    INT16U timeAL; /*"应用层响应数据时基"*/
    INT8U action;  /*"当前链路执行操作：0_空闲/1_等待响应/2_上行分帧"*/
    UN_DL_FRAME flag;/*"链路分帧标志"*/
}ST_DL_STATUS;

typedef struct
{
    INT16U rHeadPos;/*" 接收APDU在链路缓冲区起始位置"*/
    INT16U rLen;/*" 当前接收链路帧的APDU长度"*/
    INT16U rALBuffOffset;/*" 当前接收APDU的总长度"*/
    INT8U  *pAPDU;/*" 应用层APDU缓存起始地址"*/
    INT16U maxApduLen;/*" 应用层APDU最大缓存长度"*/
    INT16U sLen;/*" 发送APDU长度"*/
    INT16U blockNo;/*"分帧序号0~4095"*/
    INT16U  blockFmt;/*"分帧格式域"*/
    INT8S retToAL;/*" 返回应用层的找帧结果"*/
    UN_NEXT_FRAME flag;/*"后续帧标志"*/
    INT8U addrType; /*"地址类型"*/
    UN_CTRL_698 ctrl; /*"控制码"*/
    INT8U clientAddr; /*"客户机地址"*/
}ST_FRM_INFO;




extern void DL_698_Init(void);
extern void DL_698_Main(void);
extern INT16S DL_698_FrameCheck(INT8U ComId, 
                                INT8U * PApduBuf, 
                                INT16U MaxApdubufLen,
                                ST_698_FRAME_INFO *PReFrameInfo);
extern INT8U DL_698_FrameSend(INT8U ComId, 
                              INT8U * PApduBuf, 
                              INT16U SendApduLen);
extern void DL698_DLInit(INT8U ComId);


extern INT8U mBpsChangeFlg[E_MAX_COM_ID];
#ifdef THREE_PHASE
extern INT8U mPdIrWakeUpFlg;/*"低功耗红外唤醒标志"*/
#endif


#endif

