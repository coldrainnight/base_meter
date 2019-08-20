/*"***********************************************************"*/		
/*"文件名: tca.c	"*/	
/*"作者:       "*/ 
/*"版本: 1.0       日期:  "*/	
/*"文件描述: 双芯认证"*/
/*"函数列表: "*/		
/*"<函数名>	"*/	
/*"***********************************************************"*/	
#ifndef _TCA_H_
#define _TCA_H_

//#define TCA_TEST

#define ID_TCA_CMD2  	0x070501FF	/*双芯认证指令2，用于管理芯与计量芯之间通信*/
#define TCA_CMD2_AUTH	0x01		/*同步认证*/
#define TCA_CMD2_INIT	0x02		/*初始值设置*/
#define TCA_CMD2_BACKUP	0x03	/*备份*/
/*RETURN*/
#define RET_OK				0            	/*"正常返回	 "*/
#define RET_OTHERERR		1          	/*"其他错误"*/
#define RET_RIGHTERR		2
#define _CODEADDR_ERR               0x80                /*""地址错误*/

#ifdef THREE_PHASE
/*"程序比对地址"*/
#define _CODESTART           0x000000               /*"MCU代码起始地址"*/
#define _CODEEND    	     0x05FFFF              /*"MCU代码结束地址[830芯片对应384K空间]"*/
//[from 0x00043000 to 0x00044000] { ro section .para_seg }
/*"保护区,放置默认参数para_seg,需要比对，但不作判断依据"*/
#define _RSVSTART1           0xFFFFFFFF             /*"保留区1起始地址"*/
#define _RSVEND1             0xFFFFFFFF             /*"保留区1起始地址"*/
#define _RSVSTART2           0xFFFFFFFF             /*"保留区2起始地址"*/
#define _RSVEND2             0xFFFFFFFF             /*"保留区2结束地址"*/
#else
/*"程序比对地址"*/
#define _CODESTART           0x000000               /*"MCU代码起始地址"*/
#define _CODEEND    	     0x03FFFF              /*"MCU代码结束地址"*/
//0x0003f000 ~ 0x0003ffff	/*"保护区,放置默认参数para_seg,需要比对，但不作判断依据"*/
#define _RSVSTART1           0xFFFFFFFF             /*"保留区1起始地址"*/
#define _RSVEND1             0xFFFFFFFF             /*"保留区1起始地址"*/
#define _RSVSTART2           0xFFFFFFFF             /*"保留区2起始地址"*/
#define _RSVEND2             0xFFFFFFFF             /*"保留区2结束地址"*/
#endif

typedef union /*"认证因子"*/
{
   	INT32U asLong;
   	INT16U asWord[2];
   	INT8U asBytes[4];
}TCA_AuthFactor;


typedef struct 
{
	INT8U ErrCnt;	/*认证错误次数*/
	INT8U AuthAllow; /*允许认证1允许0不允许*/
	INT8U LimtPermiss; /*限制权限1限制0不限制*/
}TCA_AuthPara;


typedef struct 
{
	INT8U AuthAllow; /*允许认证1允许0不允许*/
	INT8U LimtPermiss; /*限制权限1限制0不限制*/
	INT8U Flag;
	INT8U BackOffset;
	TCA_AuthFactor AuthFactor[3];    
}TCA_AuthSaveData;


/*03命令结构体(暂时放这)*/
typedef struct
{
	INT8U *DI;     /*"数据标识首地址"*/
	INT8U *pData;	/*"数据区首地址"*/
	INT32U di;     /*"将数据标识以long型保存"*/
	INT8U len;     /*"数据区长度"*/
}ST_CMD03;

/*"程序比对"*/
typedef __packed struct
{
    INT8U CpuList;              /*"CPU编号"*/     
    INT8U Index;             	/*"比对密钥索引"*/ 
    UN_DWORD_DEF FactorAddr;               	/*"比对因子起始地址"*/
    UN_DWORD_DEF CodeAddr;            		/*"比对数据起始地址"*/ 
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
