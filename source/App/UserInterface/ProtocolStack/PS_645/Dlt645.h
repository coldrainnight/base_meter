/*"************************************************************"*/
/*"FileName: CM.h"*/
/*"Author:  扶忠权  Version : 1.0       Date: 2014-03-24"*/
/*" Description: 通信模块处理单元,实现通信链路层解析"*/
/*" 及除读写命令外其它控制命令的通信解析."*/
/*" Function List:  主要函数及其功能"*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#ifndef _DLT645_H
#define _DLT645_H

#include "System\sys_def.h"

#define DLT645_BUFF_LENTH (180)
#define MAX_DLT645_DEVICE_COUNT		(4)

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
	INT8U Saved:4;	

}ST_DLT645_DEVICE;

#define T1_CLASS	0x97	/*"1类参数密码级别(国网:99. 南网:97)"*/
#define T2_CLASS	0x96	/*"2类参数密码级别(国网:98. 南网:96)"*/

/*"-------------------------------通信状态定义--------------------------------------"*/
#define CM_RECEIVE     0      /*"接收状态"*/
#define CM_PREPARED 1      /*"待发送状态"*/
#define CM_TRANSMIT 2       /*"发送状态"*/
#define CM_CLOSED   0xFF   /*"关闭状态"*/

/*"-------------------------------通信宏定义--------------------------------------"*/
#define CM_PORTS_NUM  3       /*"通信口数"*/

#define CM_RECEIVE_DELAY  30     /*"接收转发送延时ms"*/
#define CM_RESPONSE_TIMEOUT 500      /*"应答超时，ms"*/
#define CM_SEND_DELAY  10000     /*"接收转发送延时ms"*/
#define CM_INIT_INTERVAL       4   /*"无通信时定时初始化端口时间间隔，分钟"*/
#define CM_DIS_DELAY  1500     /*"通信符号显示延时ms"*/

#define CM_DATA_MAX 196       /*"数据区长度"*/

#define CM_NO_REPLY     0xFD  /*" 不需要应答"*/

#define CM_MASTER_DATA   0xFe   /*"异常状态"*/

#define CM_UNUSUAL   0xFF   /*"异常状态"*/


/*"错误应答状态"*/
#define COMM_ERR_NOERR    	0x00    /*"无错误"*/
#define COMM_ERR_OTHERS    	0x01    /*"其它错误"*/
#define COMM_ERR_NODATA     0x02   /*"无数据请求"*/
#define COMM_ERR_PASSWORD  	0x04    /*"权限错误"*/
#define COMM_ERR_BAUD    	0x08    /*"波特率错误"*/
#define COMM_ERR_YEARSLOT 	0x10    /*"年时区数超"*/
#define COMM_ERR_DAILYSLOT 	0x20    /*"日时段数超"*/
#define COMM_ERR_TRAFF    	0x40    /*"费率数超"*/
#define COMM_ERR_RESERVED	0x80    /*"保留"*/

#define DLT645_2007 0  /*"07规约"*/
#define DLT645_1997 1  /*"97规约"*/
#define COMM_DATA_MAX 200  /*"数据区最大数据长度"*/

#define INVALID_DATA_ID 0xFFFFFFFF  /*"无效ID"*/


/*"-------------------------------通信数据结构定义--------------------------------------"*/

typedef union /*"645 操作者代码"*/
{
    INT8U asBytes[4];
	INT16U asWord[2];
	INT32U asLong;
} UN_OPERATOR;

typedef struct /*"645 帧信息"*/
{
	UN_ID645 dataId;       /*"645ID"*/
	UN_OPERATOR operator;  /*"操作者代码"*/
	INT8U *beginData;         /*"数据帧起始地址，即第1个68地址"*/
	INT8U *data;         /*"不包括密钥，数据标识，操作者代码的数据"*/
	INT8U dataLen;      /*"数据区长度"*/
	INT8U authority;     /*"密级"*/
	INT8U commPort;      /*"通信口号"*/
	INT8U command;      /*"控制码"*/
	INT8U protocol;      /*" 07 or 97"*/
	INT8U errorCode;     /*"错误代码"*/
    INT8U frmLen;      /*"帧长度"*/
} ST_FRAME645;

typedef struct /*"645 协议处理帧信息"*/
{
	UN_ID645 dataId;   /*"645ID"*/
	INT8U *data;     /*"数据区"*/
	INT16U dataLen;  /*"数据长度"*/
	INT8U authority;  /*"密级"*/
	INT8U commPort; /*"通信口号"*/
} ST_FRM_RESULT;

typedef struct
{
	INT8U *data;   /*"数据区"*/
	INT16S dataLen; /*"数据长度"*/
} ST_CMDATA;

typedef struct /*"645 协议应答帧信息"*/
{
	INT8U *data;   /*"数据区"*/
	INT16S dataLen; /*"数据长度"*/
} ST_ANSWR_DATA;

typedef struct /*"645数据对象映射表"*/
{
	INT32U asLong;/*"645数据标识"*/
	INT16U Obis; /*"数据对象名称"*/
	INT16U SetLen;	/*"设置数据长度"*/
} ST_PROCESS_INFO;


typedef struct /*"后续帧信息"*/
{
	UN_ID645 dataId;       /*"645ID"*/
	INT16U   bytes;       /*"本帧数据字节数"*/
	INT16U   prevBytes;  /*"上一帧数据字节数"*/
	INT8U    seq;          /*"帧序列号"*/
} ST_CONTINUEDFRAME;

/*"输出数据长度定义"*/
typedef union _COMM_DATA_LEN
{
	struct
	{
		INT16U len:15;     /*"输出数据长度"*/
		INT16U backframe:1;/*"是否有后续数据 0 无  1  有"*/
	}bit;
	INT16U int_len;
}COMM_DATA_LEN;

/*"协议树通信应答数据结构定义"*/
typedef struct _ANSWER_DATA
{
	INT8U              *Data_addr; /*"输出数据域地址指针"*/
	COMM_DATA_LEN      Data_len;   /*"输出数据长度"*/
}ST_ANSWER_DATA;

extern INT8U reportComBegin[E_MAX_COM_ID];


extern INT8U DLT645_ProcessRecBytes(INT8U *pDataBuff, INT16U length, INT8U DeviceIndex);
extern void Ans_MtrClr (void);
extern INT8U ReplyUserIdData(ST_FRAME645 *res, ST_ANSWER_DATA *ans);
extern INT8U check_ReadList(ST_FRAME645 *framedata);
extern void cfg_read_peer_list_data_to_peer(ST_FRAME645 *res);
extern INT8U ListDataIsAllPeerData(void);
extern INT8U ListDataHasPeerData(void);
extern void ParseFrameNoMdf(INT8U *rpFrame, ST_FRAME645 *rpResult);
extern void FrameDataDec0x33(ST_FRAME645 *rpResult);
extern INT8U SetParaToMter(ST_FRAME645 *rpResult);
extern void slave_ans_data(void);
#endif

