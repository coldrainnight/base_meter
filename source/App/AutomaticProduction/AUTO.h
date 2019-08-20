/*"*****************************************************************************"*/
/*"  FileName: AUTO.h"*/
/*"  Description:     ���ļ����������Զ���ģ�鲿�����ݽṹ"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.11.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"*******************************************************************************"*/
#ifndef _AUTO_H_
#define _AUTO_H_

#include "DataCenterSoftware\Code\AutomaticProduction\AUTO_Data.h"
#include "AUTO_DataProcess.h"
#include "AUTO_EncryTion.h"

typedef struct /*"645 Э�鴦��֡��Ϣ"*/
{
	INT8U dataId[4];   /*"645ID"*/
	INT8U *data;     /*"������"*/
	INT8U dataLen;  /*"���ݳ���"*/
} ST_FRMINFO;

typedef struct
{
    INT8U cmd;
    INT8U unUsed;
    ST_6TIME_FORM validTime;
}ST_INTOFACTORY;


typedef union   /*"���ڳ�ʼ�������Ƿѹ״̬�� �Զ��壬�Ժ���չ"*/
{
    INT16U word;          
    struct
    {
    	INT8U facState:1; 	/*"����״̬            0 ����  1����"*/
    	INT8U dfultFlg:1;   /*"��ʼ����ɱ�־    0 ���  1δ���"*/    
    	INT8U clockBat:1; 	/*"ʱ�ӵ��            0 ����  1Ƿѹ"*/
    	INT8U LowPowBat:1;  /*"�͹��ĵ��       0 ����  1Ƿѹ"*/
    	INT8U elecprotec:1; /*"����           0 �Ǳ��� 1����"*/
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;

    	INT8U :1;
    	INT8U :1;
    	INT8U :1; 
    	INT8U :1;
    	INT8U :1;
    	INT8U :1; 
    	INT8U :1;
    	INT8U :1; 
    }bits;
}UN_FACWORD_FORM;

typedef INT8S (* TOn_ReadWrite)(INT8U *pdata);/*"��д����ԭ��"*/

typedef struct 
{
    INT8U ComDataId;    /*"���ݱ�ʶʶ����"*/
    TOn_ReadWrite OnReadWrite;  /*"���ݱ�ʶ��Ӧ��������"*/
}ST_COMFUNCTION;

/*"�����Զ����ļ����ݽṹ��"*/
typedef struct
{
	INT8U MtrProductID1[32];
	INT8U MtrProductID2[32];
	INT8U MtrProductID3[32];
}ST_AUTO_FILE_DATA;

#define PRODUCTID1_SHIFT       	FIND(ST_AUTO_FILE_DATA, MtrProductID1)
#define PRODUCTID2_SHIFT       	FIND(ST_AUTO_FILE_DATA, MtrProductID2)
#define PRODUCTID3_SHIFT       	FIND(ST_AUTO_FILE_DATA, MtrProductID3)

extern INT8S AUTO_ReadExtData(void *pdin, void *pdout);
extern INT8S AUTO_WriteExtData(void *pdin, void *pdout);
#ifdef THREE_PHASE
extern INT8S AUTO_KeySelfTest(void *pdin, void *pdout);
#endif

#ifdef THREE_PHASE
extern INT8S AUTO_KeySelfTest(void *pdin, void *pdout);
#endif

extern void AUTO_Main(void);


#endif
