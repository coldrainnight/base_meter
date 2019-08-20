/*"***********************************************************"*/		
/*"�ļ���: tca.c	"*/	
/*"����:       "*/ 
/*"�汾: 1.0       ����:  "*/	
/*"�ļ�����: ˫о��֤"*/
/*"�����б�: "*/		
/*"<������>	"*/	
/*"***********************************************************"*/	
#ifndef _TCA_H_
#define _TCA_H_

//#define TCA_TEST

#define ID_TCA_CMD2  	0x070501FF	/*˫о��ָ֤��2�����ڹ���о�����о֮��ͨ��*/
#define TCA_CMD2_AUTH	0x01		/*ͬ����֤*/
#define TCA_CMD2_INIT	0x02		/*��ʼֵ����*/
#define TCA_CMD2_BACKUP	0x03	/*����*/
/*RETURN*/
#define RET_OK				0            	/*"��������	 "*/
#define RET_OTHERERR		1          	/*"��������"*/
#define RET_RIGHTERR		2
#define _CODEADDR_ERR               0x80                /*""��ַ����*/

#ifdef THREE_PHASE
/*"����ȶԵ�ַ"*/
#define _CODESTART           0x000000               /*"MCU������ʼ��ַ"*/
#define _CODEEND    	     0x05FFFF              /*"MCU���������ַ[830оƬ��Ӧ384K�ռ�]"*/
//[from 0x00043000 to 0x00044000] { ro section .para_seg }
/*"������,����Ĭ�ϲ���para_seg,��Ҫ�ȶԣ��������ж�����"*/
#define _RSVSTART1           0xFFFFFFFF             /*"������1��ʼ��ַ"*/
#define _RSVEND1             0xFFFFFFFF             /*"������1��ʼ��ַ"*/
#define _RSVSTART2           0xFFFFFFFF             /*"������2��ʼ��ַ"*/
#define _RSVEND2             0xFFFFFFFF             /*"������2������ַ"*/
#else
/*"����ȶԵ�ַ"*/
#define _CODESTART           0x000000               /*"MCU������ʼ��ַ"*/
#define _CODEEND    	     0x03FFFF              /*"MCU���������ַ"*/
//0x0003f000 ~ 0x0003ffff	/*"������,����Ĭ�ϲ���para_seg,��Ҫ�ȶԣ��������ж�����"*/
#define _RSVSTART1           0xFFFFFFFF             /*"������1��ʼ��ַ"*/
#define _RSVEND1             0xFFFFFFFF             /*"������1��ʼ��ַ"*/
#define _RSVSTART2           0xFFFFFFFF             /*"������2��ʼ��ַ"*/
#define _RSVEND2             0xFFFFFFFF             /*"������2������ַ"*/
#endif

typedef union /*"��֤����"*/
{
   	INT32U asLong;
   	INT16U asWord[2];
   	INT8U asBytes[4];
}TCA_AuthFactor;


typedef struct 
{
	INT8U ErrCnt;	/*��֤�������*/
	INT8U AuthAllow; /*������֤1����0������*/
	INT8U LimtPermiss; /*����Ȩ��1����0������*/
}TCA_AuthPara;


typedef struct 
{
	INT8U AuthAllow; /*������֤1����0������*/
	INT8U LimtPermiss; /*����Ȩ��1����0������*/
	INT8U Flag;
	INT8U BackOffset;
	TCA_AuthFactor AuthFactor[3];    
}TCA_AuthSaveData;


/*03����ṹ��(��ʱ����)*/
typedef struct
{
	INT8U *DI;     /*"���ݱ�ʶ�׵�ַ"*/
	INT8U *pData;	/*"�������׵�ַ"*/
	INT32U di;     /*"�����ݱ�ʶ��long�ͱ���"*/
	INT8U len;     /*"����������"*/
}ST_CMD03;

/*"����ȶ�"*/
typedef __packed struct
{
    INT8U CpuList;              /*"CPU���"*/     
    INT8U Index;             	/*"�ȶ���Կ����"*/ 
    UN_DWORD_DEF FactorAddr;               	/*"�ȶ�������ʼ��ַ"*/
    UN_DWORD_DEF CodeAddr;            		/*"�ȶ�������ʼ��ַ"*/ 
}ST_CODECMP;

#define TCA_SAVE_DATA_SHIFT		(FIND(ST_UI_FILE_DATA,TcaSaveData))
#define TCA_AUTHALLOW_SHIFT		(TCA_SAVE_DATA_SHIFT+(FIND(TCA_AuthSaveData,AuthAllow)))
#define TCA_LIMITPERMISS_SHIFT		(TCA_SAVE_DATA_SHIFT+(FIND(TCA_AuthSaveData,LimtPermiss)))
#define TCA_FLAG_SHIFT				(TCA_SAVE_DATA_SHIFT + (FIND(TCA_AuthSaveData,Flag)))
#define TCA_BACKOFFSET_SHIFT		(TCA_SAVE_DATA_SHIFT+(FIND(TCA_AuthSaveData,BackOffset)))
#define TCA_AUTHFACTOR1_SHIFT		(TCA_SAVE_DATA_SHIFT+(FIND(TCA_AuthSaveData,AuthFactor)))
#define TCA_AUTHFACTOR2_SHIFT		(TCA_AUTHFACTOR1_SHIFT+sizeof(TCA_AuthFactor))
#define TCA_AUTHFACTOR3_SHIFT		(TCA_AUTHFACTOR2_SHIFT+sizeof(TCA_AuthFactor))

extern void TCA_Init(INT8U mode);
extern void TCA_OnEvyMin(void);
extern INT8U TCA_AuthHandle(ST_CMD03 *pPkg);
extern INT8U FarCodeCmp(ST_CMD03 *pPkg);
extern INT8S TCA_AuthAllow_KeepS(void *pdin, void *pdout);
#endif
