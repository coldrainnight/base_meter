/*"*****************************************************************************"*/
/*"  FileName: AUTO.h"*/
/*"  Description:     该文件定义生产自动化模块部分数据结构"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.11.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"*******************************************************************************"*/
#ifndef _AUTO_H_
#define _AUTO_H_

#include "DataCenterSoftware\Code\AutomaticProduction\AUTO_Data.h"
#include "AUTO_DataProcess.h"
#include "AUTO_EncryTion.h"

typedef struct /*"645 协议处理帧信息"*/
{
	INT8U dataId[4];   /*"645ID"*/
	INT8U *data;     /*"数据区"*/
	INT8U dataLen;  /*"数据长度"*/
} ST_FRMINFO;

typedef struct
{
    INT8U cmd;
    INT8U unUsed;
    ST_6TIME_FORM validTime;
}ST_INTOFACTORY;


typedef union   /*"厂内初始化，电池欠压状态字 自定义，以后扩展"*/
{
    INT16U word;          
    struct
    {
    	INT8U facState:1; 	/*"厂内状态            0 厂外  1厂内"*/
    	INT8U dfultFlg:1;   /*"初始化完成标志    0 完成  1未完成"*/    
    	INT8U clockBat:1; 	/*"时钟电池            0 正常  1欠压"*/
    	INT8U LowPowBat:1;  /*"低功耗电池       0 正常  1欠压"*/
    	INT8U elecprotec:1; /*"保电           0 非保电 1保电"*/
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

typedef INT8S (* TOn_ReadWrite)(INT8U *pdata);/*"读写函数原型"*/

typedef struct 
{
    INT8U ComDataId;    /*"数据标识识别码"*/
    TOn_ReadWrite OnReadWrite;  /*"数据标识对应操作函数"*/
}ST_COMFUNCTION;

/*"生产自动化文件数据结构体"*/
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
