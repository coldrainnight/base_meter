#ifndef DL_698_H
#define DL_698_H

#include "system\Sys_def.h"
#include "PlatForm.h"

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
	INT8U TxOver:1;     /*"���ͽ���"*/
	INT8U frmType:2;  /*"����֡����: 01_698; 02_645; else_invalid"*/
	INT8U Saved:1;

}ST_DLT698_DEVICE;

/*"������"*/
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

#define DL698_MAX_BUFF   (RS4851_BUFF_RX_SIZE)/*"�ײ㻺�������1900"*/

/*"MaxApduLen �����豣֤���������(42�ֽ�ʱ����ʱ�α�)�������·�"*/
/*"10 + 14 * 8 = 122byte"*/
/*"17byte: 4(ǰ����) + 8( ֡ͷ)+ 3(֡β) + 2(��֡��ʽ��)"*/
#define MaxApduLen(portNo)   (mCommBuffMax[portNo] - MAX_ADDR_LEN - 17)

#define CAL_BUFFADD(head, n, max)   ((head + n) % max)


typedef enum
{
    ADDR_UNRESOLVED    =0   ,/*"��Ч��ַ"*/
    ADDR_SELFADDR,/*"�����ַ"*/
    ADDR_ABBREVIATE,       /*"ͨ���ַ"*/
    ADDR_BROADCAST,         /*"�㲥��ַ"*/
    ADDR_GROUP            /*"���ַ"*/
}EN_ADDRTYPE;

typedef struct 
{
    EN_ADDRTYPE AddrType;   /*"��ַ����"*/
    INT8U clientAddr; /*"�ͻ�����ַ"*/
}ST_698_FRAME_INFO;

typedef enum
{
    SINGLE = 0, /*"����ַ"*/
    WILDCARD,   /*"ͨ���ַ"*/
    GROUP,      /*"���ַ"*/
    BROADCAST,  /*"�㲥��ַ"*/
    MAX_TYPE_ADDR
}EN_TYPE_ADDR;

typedef union /*"������C"*/
{
    INT8U   byte;
    
    struct
    {
        INT8U Func: 3; /*"������"*/
        INT8U SC:1; /*"�����־λ"*/
        INT8U reserved:1; /*"����"*/
        INT8U FRM:1; /*"��֡��־λ"*/
        INT8U PRM:1; /*"������־λ"*/
        INT8U DIR:1; /*"���䷽��λ"*/
    }bits;
}UN_CTRL_698;

typedef union
{
    INT8U byte;
    struct
    {
        INT8U DownNextFrm:1;/*"����Ϊ��֡(APDUƬ��)"*/
        INT8U Asw:1;/*"��վ��֡ȷ��"*/
        INT8U reserved:4;
    }bits;
}UN_NEXT_FRAME;

typedef union
{
    INT8U byte;
    struct
    {
        INT8U nextFrm:1;/*"�к���֡"*/
        INT8U end:1;/*"0: �м�֡��1�����֡"*/
        INT8U confirmToAL: 1;/*"ȷ��֡���ظ�Ӧ�ò�"*/
        INT8U reserved:6;
    }bits;
}UN_DL_FRAME;

typedef struct  /*"ͨ����·״̬"*/
{
    INT16U timeClient; /*"�ͻ�����Ӧ����ʱ��"*/
    INT16U timeAL; /*"Ӧ�ò���Ӧ����ʱ��"*/
    INT8U action;  /*"��ǰ��·ִ�в�����0_����/1_�ȴ���Ӧ/2_���з�֡"*/
    UN_DL_FRAME flag;/*"��·��֡��־"*/
}ST_DL_STATUS;

typedef struct
{
    INT16U rHeadPos;/*" ����APDU����·��������ʼλ��"*/
    INT16U rLen;/*" ��ǰ������·֡��APDU����"*/
    INT16U rALBuffOffset;/*" ��ǰ����APDU���ܳ���"*/
    INT8U  *pAPDU;/*" Ӧ�ò�APDU������ʼ��ַ"*/
    INT16U maxApduLen;/*" Ӧ�ò�APDU��󻺴泤��"*/
    INT16U sLen;/*" ����APDU����"*/
    INT16U blockNo;/*"��֡���0~4095"*/
    INT16U  blockFmt;/*"��֡��ʽ��"*/
    INT8S retToAL;/*" ����Ӧ�ò����֡���"*/
    UN_NEXT_FRAME flag;/*"����֡��־"*/
    INT8U addrType; /*"��ַ����"*/
    UN_CTRL_698 ctrl; /*"������"*/
    INT8U clientAddr; /*"�ͻ�����ַ"*/
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
extern INT8U mPdIrWakeUpFlg;/*"�͹��ĺ��⻽�ѱ�־"*/
#endif


#endif

