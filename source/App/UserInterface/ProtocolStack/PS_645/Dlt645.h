/*"************************************************************"*/
/*"FileName: CM.h"*/
/*"Author:  ����Ȩ  Version : 1.0       Date: 2014-03-24"*/
/*" Description: ͨ��ģ�鴦��Ԫ,ʵ��ͨ����·�����"*/
/*" ������д�������������������ͨ�Ž���."*/
/*" Function List:  ��Ҫ�������书��"*/
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
       INT8U CurrentDeviceId; /*"��ǰ�豸��Ӳ���˿ں�"*/
       
	INT8U *RxBuff;/*"���ջ�����"*/
	INT8U *TxBuff;/*"���ͻ�����"*/	
	
	INT16U RxLen;/*"���ճ���"*/
	INT16U TxLen;/*"���ͳ���"*/

	INT16U InactivityTimer;/*�޻��ʱʱ�䣬�������һ��ʱ����û�л��Ͽ�����*/
	INT16U InterOctetTimer;	/*"�ֽڼ䳬ʱʱ��"*/
	
	INT8U InterruptClose:1;/*"�жϹرգ�ֻ���жϹرգ����ܽ����ڽ��յ���������ӵ����ջ�����"*/	
	INT8U TxWaiting:1;     /*"���͵ȴ�"*/
	INT8U InterOctetTimeout:1;	/*�ֽڼ䳬ʱ��־λ��1λ��*/	
	INT8U InactivityTimeout:1;  /*֡��ʱ��־λ��1λ��*/	
	INT8U Saved:4;	

}ST_DLT645_DEVICE;

#define T1_CLASS	0x97	/*"1��������뼶��(����:99. ����:97)"*/
#define T2_CLASS	0x96	/*"2��������뼶��(����:98. ����:96)"*/

/*"-------------------------------ͨ��״̬����--------------------------------------"*/
#define CM_RECEIVE     0      /*"����״̬"*/
#define CM_PREPARED 1      /*"������״̬"*/
#define CM_TRANSMIT 2       /*"����״̬"*/
#define CM_CLOSED   0xFF   /*"�ر�״̬"*/

/*"-------------------------------ͨ�ź궨��--------------------------------------"*/
#define CM_PORTS_NUM  3       /*"ͨ�ſ���"*/

#define CM_RECEIVE_DELAY  30     /*"����ת������ʱms"*/
#define CM_RESPONSE_TIMEOUT 500      /*"Ӧ��ʱ��ms"*/
#define CM_SEND_DELAY  10000     /*"����ת������ʱms"*/
#define CM_INIT_INTERVAL       4   /*"��ͨ��ʱ��ʱ��ʼ���˿�ʱ����������"*/
#define CM_DIS_DELAY  1500     /*"ͨ�ŷ�����ʾ��ʱms"*/

#define CM_DATA_MAX 196       /*"����������"*/

#define CM_NO_REPLY     0xFD  /*" ����ҪӦ��"*/

#define CM_MASTER_DATA   0xFe   /*"�쳣״̬"*/

#define CM_UNUSUAL   0xFF   /*"�쳣״̬"*/


/*"����Ӧ��״̬"*/
#define COMM_ERR_NOERR    	0x00    /*"�޴���"*/
#define COMM_ERR_OTHERS    	0x01    /*"��������"*/
#define COMM_ERR_NODATA     0x02   /*"����������"*/
#define COMM_ERR_PASSWORD  	0x04    /*"Ȩ�޴���"*/
#define COMM_ERR_BAUD    	0x08    /*"�����ʴ���"*/
#define COMM_ERR_YEARSLOT 	0x10    /*"��ʱ������"*/
#define COMM_ERR_DAILYSLOT 	0x20    /*"��ʱ������"*/
#define COMM_ERR_TRAFF    	0x40    /*"��������"*/
#define COMM_ERR_RESERVED	0x80    /*"����"*/

#define DLT645_2007 0  /*"07��Լ"*/
#define DLT645_1997 1  /*"97��Լ"*/
#define COMM_DATA_MAX 200  /*"������������ݳ���"*/

#define INVALID_DATA_ID 0xFFFFFFFF  /*"��ЧID"*/


/*"-------------------------------ͨ�����ݽṹ����--------------------------------------"*/

typedef union /*"645 �����ߴ���"*/
{
    INT8U asBytes[4];
	INT16U asWord[2];
	INT32U asLong;
} UN_OPERATOR;

typedef struct /*"645 ֡��Ϣ"*/
{
	UN_ID645 dataId;       /*"645ID"*/
	UN_OPERATOR operator;  /*"�����ߴ���"*/
	INT8U *beginData;         /*"����֡��ʼ��ַ������1��68��ַ"*/
	INT8U *data;         /*"��������Կ�����ݱ�ʶ�������ߴ��������"*/
	INT8U dataLen;      /*"����������"*/
	INT8U authority;     /*"�ܼ�"*/
	INT8U commPort;      /*"ͨ�ſں�"*/
	INT8U command;      /*"������"*/
	INT8U protocol;      /*" 07 or 97"*/
	INT8U errorCode;     /*"�������"*/
    INT8U frmLen;      /*"֡����"*/
} ST_FRAME645;

typedef struct /*"645 Э�鴦��֡��Ϣ"*/
{
	UN_ID645 dataId;   /*"645ID"*/
	INT8U *data;     /*"������"*/
	INT16U dataLen;  /*"���ݳ���"*/
	INT8U authority;  /*"�ܼ�"*/
	INT8U commPort; /*"ͨ�ſں�"*/
} ST_FRM_RESULT;

typedef struct
{
	INT8U *data;   /*"������"*/
	INT16S dataLen; /*"���ݳ���"*/
} ST_CMDATA;

typedef struct /*"645 Э��Ӧ��֡��Ϣ"*/
{
	INT8U *data;   /*"������"*/
	INT16S dataLen; /*"���ݳ���"*/
} ST_ANSWR_DATA;

typedef struct /*"645���ݶ���ӳ���"*/
{
	INT32U asLong;/*"645���ݱ�ʶ"*/
	INT16U Obis; /*"���ݶ�������"*/
	INT16U SetLen;	/*"�������ݳ���"*/
} ST_PROCESS_INFO;


typedef struct /*"����֡��Ϣ"*/
{
	UN_ID645 dataId;       /*"645ID"*/
	INT16U   bytes;       /*"��֡�����ֽ���"*/
	INT16U   prevBytes;  /*"��һ֡�����ֽ���"*/
	INT8U    seq;          /*"֡���к�"*/
} ST_CONTINUEDFRAME;

/*"������ݳ��ȶ���"*/
typedef union _COMM_DATA_LEN
{
	struct
	{
		INT16U len:15;     /*"������ݳ���"*/
		INT16U backframe:1;/*"�Ƿ��к������� 0 ��  1  ��"*/
	}bit;
	INT16U int_len;
}COMM_DATA_LEN;

/*"Э����ͨ��Ӧ�����ݽṹ����"*/
typedef struct _ANSWER_DATA
{
	INT8U              *Data_addr; /*"����������ַָ��"*/
	COMM_DATA_LEN      Data_len;   /*"������ݳ���"*/
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

