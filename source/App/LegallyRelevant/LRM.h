#ifndef LRM_H
#define LRM_H

#include "PlatForm.h"
#include "EU.h"
#include "MU_LIB.h"
#include "Rtc_CaliApp.h"
#include "App\MtrParamDefault\MtrParamDef.h"

extern void LRM_Init(INT8U mode);
extern void LRM_Main(void);
extern INT8S LRM_Clr(void *pdin, void *pdout);
extern INT8S LRM_JustMtr(void *pdin, void *pdout);
#ifdef SINGLE_PHASE
extern INT8S LRM_PulseCtrInit(void *pdin, void *pdout);
#endif
extern INT8S LRM_PwrDown(void *pdin, void *pdout);
extern INT8U ChkLrmParaValid(void);
extern void LRM_LPMain(INT8U mode);

/*"法定计量模块存EEPROM文件数据定义"*/
typedef struct
{
    ST_BAKUP_ENGY_DATA   EngyBak1;
    ST_LRM_DATA          LrmPdSaveData;
    ST_BAKUP_ENGY_DATA   EngyBak2;
    ST_6TIME_FORM        SystimeBak;/*"系统时钟周期备份，用户上电初始化时钟容错恢复"*/
	#ifdef SINGLE_PHASE
    INT8U                MtStartFlg;
	#endif
}ST_LRM_FILE_DATA;

/*"法定计量模块存EEPROM文件参数定义"*/
typedef struct
{
    ST_MU_PARA          MuParaBak1;
    ST_RTC_PARA         RtcParaBak1;
    ST_LRM_PARA         LrmParaBak1;
    ST_MU_PARA          MuParaBak2;
    ST_RTC_PARA         RtcParaBak2;
    ST_LRM_PARA         LrmParaBak2;
}ST_LRM_FILE_PARA;



#define  MU_PARA_BAK1_SHIFT       FIND(ST_LRM_FILE_PARA, MuParaBak1)
#define  MU_PARA_BAK2_SHIFT       FIND(ST_LRM_FILE_PARA, MuParaBak2)
#define  RTC_PARA_BAK1_SHIFT      FIND(ST_LRM_FILE_PARA, RtcParaBak1)
#define  RTC_PARA_BAK2_SHIFT      FIND(ST_LRM_FILE_PARA, RtcParaBak2)
#define  ENGY_BAK1_SHIFT          FIND(ST_LRM_FILE_DATA, EngyBak1)
#define  ENGY_BAK2_SHIFT          FIND(ST_LRM_FILE_DATA, EngyBak2)
#define  LRM_PD_SAVE_DATA_SHIFT   FIND(ST_LRM_FILE_DATA, LrmPdSaveData)
#define  LRM_PARA_BAK1_SHIFT       FIND(ST_LRM_FILE_PARA, LrmParaBak1)
#define  LRM_PARA_BAK2_SHIFT       FIND(ST_LRM_FILE_PARA, LrmParaBak2)
#define  SYS_TIME_BAK              FIND(ST_LRM_FILE_DATA, SystimeBak)

extern ST_LRM_PARA  stLrmPara;

#endif
