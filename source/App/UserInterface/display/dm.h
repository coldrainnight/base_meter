/*"************************************************************"*/
/*"FileName: TR.h                                              "*/
/*"Author: ����                                                "*/
/*"Version : 1.0       Date: 2017.11.05                        "*/
/*"Description: ��ʾ����ģ��              "*/
/*"Function List:                                              "*/
/*"<FunctionName>                                              "*/
/*"<author> <time> <version > <desc>                           "*/
/*"************************************************************"*/

#ifndef _DM_H_
#define _DM_H_

#include "system\Sys_def.h"
#include "Driver\Hal\DIS\DIS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include"app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include "App\MtrParamDefault\MtrParamDef.h"


typedef enum
{
    CYC_SHOW_MODE = 0,/*"ѭ��"*/
    KEY_SHOW_MODE,/*"����"*/
    //TEST_SHOW_MODE,/*"����"*/
    PD_CYC_SHOW_MODE,/*"�͹���ѭ��"*/
    PD_KEY_SHOW_MODE,/*"�͹��İ���"*/
    INIT_SHOW_MODE,/*"�ϵ��ʼ����ʾ"*/
    SPEC_MODE,/*"Һ���鿴"*/
    PD_INIT_SHOW_MODE,/*"�����ʼ����ʾ"*/

    MAX_SHOW_MODE
}EN_SHOW_MODE;/*"��ʾģʽ�б�"*/

typedef enum
{
    DISP_VER_ID = 0,
    DISP_ALL_ID,

    MAX_INIT_NUM
}EN_INIT_ITEM;/*"�ϵ���ʾ��Ŀ�б�"*/

typedef enum
{
    CYC_LIST = 0,
    KEY_LIST,

    MAX_LIST_TYPE
}EN_DISP_LIST;/*"����ʾ�б�����ģʽID"*/

typedef enum
{
    RUN_MODE_PD = 0,
    RUN_MODE_INIT,
    RUN_MODE_NORMAL,

    MAX_RUN_MODE
}EN_DISP_RUN_MODE;/*"����ģʽ"*/



#define DISP_OBJ_SIZE    sizeof(ST_DISP_OBJ)

#ifndef DM_DLT698
#define SHOW_ALL_TIME    (5)/*"ȫ��ʱ��"*/
#endif
#define SHOW_VER_TIME    (3)/*"��ʾ�汾��ʱ��"*/
#define SHOW_INTERVAL   (500) /*"��ʾˢ�¼��500ms"*/

#define PD_SHOW_TIME     (604800) /*"���糣��ʱ��7*24*3600"*/

#ifdef THREE_PHASE
#define PD_DISP_DATA_NUM (40)/*"�͹�����ʾ���ݻ��������Ŀ����"*/
#define PD_DISP_CYC_NUM (21)/*"�͹���ѭ�Զ��������Ŀ����"*/
#define PD_DISP_KEY_NUM (40)/*"�͹��İ��Զ��������Ŀ����"*/
#else
#define PD_DISP_DATA_NUM (25)/*"�͹�����ʾ���ݻ��������Ŀ����"*/
#define PD_DISP_CYC_NUM (7)/*"�͹���ѭ�Զ��������Ŀ����"*/
#define PD_DISP_KEY_NUM (27)/*"�͹��İ��Զ��������Ŀ����"*/
#endif

#define DM_BLK_LEFT_SHIFT  (7)

#ifdef THREE_PHASE
#define PD_KEY_WAKE_UP  (0xAA)/*"�͹��İ������ѱ�־"*/
#define PD_IR_COMM          (0xAA)/*"�͹��ĺ���ͨ�Ž���"*/
#define PD_IR_SHOW          (0x55)/*"�͹��ĺ��⻽����ʾ��ʱ"*/
#endif

#if defined(DM_DLT698)/*"698Э����ʾ������ض���"*/

/*"698Э����ʾ�����ʶ����ʱ��"*/
#define DISP_CLOCK_OBJ   ((mstRunData.curScrCode.Oad.Oi.asInt == 0x4000) ? 1 : 0)

typedef struct
{
    INT8U Csd;
    INT8U Oad[4];	
    INT8U scrNo;
}ST_DISP_OBJ1;

typedef struct
{
    INT8U Csd;
    INT8U Oad[4];	
    INT8U cnt;
    INT8U releOad[4];
    INT8U scrNo;
}ST_DISP_OBJ2;

typedef union
{
    ST_DISP_OBJ1 item1;
    ST_DISP_OBJ2 item2;
}UN_DISP_OBJ;

typedef UN_DISP_OBJ  ST_DISP_PARA_OBJ;

#elif defined(DM_DLT645)/*"645Э����ʾ������ض���"*/

#define DISP_CLOCK_OBJ   ((mstRunData.curScrCode.unId645.asLong == 0x04000101) || (mstRunData.curScrCode.unId645.asLong == 0x04000102))

#define DISP_CHECK_TIME   (10)

typedef  struct
{
    INT8U id645[4];
    INT8U leafNum;  /*"��ʾ����Ҷ�Ӻ�"*/
}ST_DISP_PARA_OBJ;

#elif defined(DM_IEC62056)/*"62056Э����ʾ������ض���"*/

#define DISP_CLOCK_OBJ   (0)

typedef struct    
{
    INT8U  ClassID;    /*"���ʶ"*/
    INT8U  OBIS[6];    /*"�������OBIS��"*/
    INT8U  AttriID;    /*"���Ա�ʶ"*/
    INT8U  index;      /*"���Ա�ʶ"*/
    INT8U  DisObis[8];    /*"�û�������ʾOBIS"*/
} ST_DISP_OBJ; /*"62056Э����ʾ������"*/

#else
#endif

typedef struct/*"��������ʾ��������"*/
{
    INT16U obj;/*"���ݶ����ʶ"*/
    INT16U time;/*"��ʾʱ��"*/
    //INT8U ShowPrio;
}ST_SHOW_SPEC;

#if 0
typedef struct
{
    ST_DM_PD_DATA data[PD_DISP_DATA_NUM]; /*"������ʾ���ݻ���"*/
    ST_DISP_OBJ cyc[PD_DISP_CYC_NUM];/*"ѭ�Զ���"*/
    ST_DISP_OBJ key[PD_DISP_KEY_NUM];/*"���Զ���"*/
/*"����max/default: 28*25 = 700 / 10*7=70 / 10*27=270   total: 1040"*/
/*"����max: 30*97 = 2910 / 10*21=210 / 10*99=990   total: 4110"*/
/*"����default: 30*40 = 1200 / 10*21=210 / 10*40=400   total: 1810"*/
}ST_PD_SHOW_DATA;
#endif

typedef struct
{
    ST_SHOW_DATA data; /*"������ʾ���ݻ�����"*/
    ST_DISP_OBJ item;/*"��ʾ����"*/
}ST_DM_PD_DATA;

typedef struct
{
    ST_DISP_OBJ cyc[PD_DISP_CYC_NUM];/*"ѭ�Զ���"*/
    ST_DISP_OBJ key[PD_DISP_KEY_NUM];/*"���Զ���"*/
}ST_PD_OBJ_LIST;/*"�͹�����ʾ�����б�"*/

typedef struct
{
    INT8U showMode;/*"��ʾģʽ"*/
    INT8U maxObjNum;/*"�����ʾ���󻺴����"*/
    INT8U *pCurrNum;/*"��ǰ��ʾ�����ȡ�������ݵ�ַ"*/
    ST_DISP_OBJ *pDispObj;/*"��ʾ����ָ��"*/
}ST_PD_DISP_INFO;

typedef struct
{
    INT16U runErr;/*"�����쳣�������,���16���������,�ο�mShowErrTab"*/
    INT8U index;/*"��ʾ��������"*/
}ST_SHOW_ERR;

typedef struct
{
    ST_DISP_OBJ curScrCode;/*"��ǰ����ʾ����"*/
#ifdef DIS_USER_CODE
    ST_USERCODE_SHOW userCode;/*"��ʾ�û�����"*/
#endif

    INT8U showMode;/*"��ǰ��ʾģʽ"*/
    INT8U runMode;/*"��ǰ����ģʽ0_����/ 1_�ϵ��ʼ��/ 2_�ϵ�����"*/
    INT8U index;/*"��ʾ����ֵ"*/
    INT8U curNum;/*"��ǰ��ʾ״̬����"*/
    INT8U showRet;/*��ʾ�ɹ���־*/
    INT8U pdObjNum;/*"�͹�����ʾ���ݻ�������Ŀ����"*/
    INT8U initShowNum;/*"��ʼ����ʾ�����"*/
    INT8U pdCycNum;/*"�͹���ѭ�Ե�ǰ��ʾ�����ȡ����"*/
    INT8U pdKeyNum;/*"�͹��İ��Ե�ǰ��ʾ�����ȡ����"*/
    INT8U validFlg;/*"��ǰ��ʾģʽ����Ч��ʾ������ڱ�־"*/
#ifdef THREE_PHASE
    INT8U keyFlg;/*"0_�·���1_�Ϸ�"*/
    INT8U pdIrTimer;/*"�͹��ĺ���ͨ�Ż�����ʱ"*/
#endif
#ifdef DIS_USER_CODE
    INT8U showUserCodeFlg;/*"��ʾ�û������־"*/
#endif
    INT16U scrTimer;/*"��ǰ����ʾʱ��"*/
    INT16U refreshTick;/*ˢ��tick*/
    //INT16U refreshInterval;/*ˢ�¼��*/

    INT32U pdShowTime;/*"�͹��ĳ�����ʱ��"*/
}ST_DM_RUN_DATA;


typedef struct
{
    INT8U *pDispNum;
    INT32U dispListAddr;
}ST_DISP_MODE_INFO;

typedef void  (* On_Key_Function)(void);

typedef struct
{
    INT16U shift;
    INT8U num;
}ST_DISP_LIST_INFO;





extern void DM_Init(INT8U mode);
extern void DM_Task(void);
extern void DM_PDMain(ST_DM_PD_DATA *pPdShowList);
extern void DM_PD1s(void);

/*"ͨ��ģ��(�û��ӿ�) ���ýӿ�"*/
extern INT16S DM_GetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pBuff);
extern INT16S DM_SetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData);


/*"�ⲿģ����÷���"*/
extern INT8S DM_ShowFlash(void *pdin, void *pdout);
extern INT8S DM_ShowSpec(void *pdin, void *pdout);
extern INT8S DM_ShowAll(void *pdin, void *pdout);
extern INT8S DM_ShowNext(void *pdin, void *pdout);
extern INT8S DM_ShowLast(void *pdin, void *pdout);
extern INT8S DM_DLT698ShowSpec(void *pdin, void *pdout);


#define stDispPara   (StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara)

#ifdef THREE_PHASE
extern INT8U mPdKeyWakeUpFlg;
extern INT8U mPdIrComFlg;
#endif

extern void DM_freshDisData(ST_SHOW_DATA *inData);
#endif


