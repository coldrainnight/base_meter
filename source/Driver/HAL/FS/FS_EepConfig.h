#ifndef FS_EEP_CONFIG_H
#define FS_EEP_CONFIG_H

#include  "App\LegallyRelevant\LRM.h"
#include  "App\monitor\MNT.h"
#include  "App\UserInterface\UI.h"
#include  "App\tariff\TR.h"
#include  "App\record\RCRD.h"
#include  "App\AutomaticProduction\AUTO.h"


typedef struct
{
    INT8U unuse;
}ST_PD_SAVE_DATA_ATTR;

#define BEGIN_ADDR 64    /*" 第1页不用"*/

/************************参数文件开始***********************/


/*" 内卡中第1份计量参数,共分配246个字节"*/
#define LRM_EEP_ADDR     (BEGIN_ADDR)	 /*" 计量参数"*/
#define LRM_FILE_SIZE    (sizeof(ST_LRM_FILE_PARA))   /*" 计量软件补偿参数,占76字节"*/
#define LRM_EEP_END	    (LRM_EEP_ADDR + LRM_FILE_SIZE)


/*预付费参数文件*/
#define PM_PARA_EEP_ADDR (LRM_EEP_END)
#define PM_PARA_SIZE 0
#define PM_PARA_END (PM_PARA_EEP_ADDR + PM_PARA_SIZE)


/*用户接口有RAM备份参数文件*/
#define UI_PARA_WITH_BACKUP_EEP_ADDR (PM_PARA_END)
#define UI_PARA_WITH_BACKUP_SIZE (sizeof(ST_UI_PARA_WITH_BACKUP))
#define UI_PARA_WITH_BACKUP_END (UI_PARA_WITH_BACKUP_EEP_ADDR + UI_PARA_WITH_BACKUP_SIZE)

/*用户接口无RAM备份参数文件*/
#define UI_PARA_NO_BACKUP_EEP_ADDR (UI_PARA_WITH_BACKUP_END)
#define UI_PARA_NO_BACKUP_SIZE     (sizeof(ST_UI_PARA_NO_BACKUP))
#define UI_PARA_NO_BACKUP_END      (UI_PARA_NO_BACKUP_EEP_ADDR + UI_PARA_NO_BACKUP_SIZE)


/*分时参数文件*/
#define TR_PARA_EEP_ADDR (UI_PARA_NO_BACKUP_END)
#define TR_PARA_SIZE (sizeof(ST_TR_FILE_PARA))
#define TR_PARA_END (TR_PARA_EEP_ADDR + TR_PARA_SIZE)

/*记录快照参数文件*/
#define RCRD_PARA_EEP_ADDR (TR_PARA_END)
#define RCRD_PARA_SIZE  (sizeof(ST_RCRD_PARA_FILE))
#define RCRD_PARA_END (RCRD_PARA_EEP_ADDR + RCRD_PARA_SIZE)

/*电表状态监测模块参数文件*/
#define MNT_PARA_EEP_ADDR (RCRD_PARA_END)
#define MNT_PARA_SIZE  (sizeof(ST_MNT_PARA))
#define MNT_PARA_END (MNT_PARA_EEP_ADDR + MNT_PARA_SIZE)

#ifdef DU_MODLE
/*需量模块参数文件*/
#define DU_PARA_EEP_ADDR (MNT_PARA_END)
#define DU_PARA_SIZE  (sizeof(ST_DU_PARA))
#define DU_PARA_END (DU_PARA_EEP_ADDR + DU_PARA_SIZE)

#define PARA_EEP_ADDR_END DU_PARA_END
#else
#define PARA_EEP_ADDR_END MNT_PARA_END
#endif

/************************参数文件结束***********************/



/*************************数据文件起始*************************/
/*"法定计量模块"*/
#define LRM_DATA_EEP_ADDR     (PARA_EEP_ADDR_END)    /*" 文件起始地址"*/
#define LRM_DATA_FILE_SIZE    (sizeof(ST_LRM_FILE_DATA))   /*"文件大小"*/
#define LRM_DATA_EEP_END      (LRM_DATA_EEP_ADDR + LRM_DATA_FILE_SIZE)/*" 文件结束地址"*/

/*掉电存数区域*/
#define PD_SAVE_EEP_ADDR (LRM_DATA_EEP_END + (128-(LRM_DATA_EEP_END%128)))
#define PD_SAVE_SIZE (sizeof(ST_PD_SAVE_DATA_ATTR))
#define PD_SAVE_END  (PD_SAVE_EEP_ADDR+PD_SAVE_SIZE)

/*"电表状态监测模块"*/
#define MNT_EEP_ADDR     (PD_SAVE_END)    /*" 文件起始地址"*/
#define MNT_FILE_SIZE    (sizeof(ST_MNT_FILE_DATA))   /*"文件大小"*/
#define MNT_EEP_END      (MNT_EEP_ADDR + MNT_FILE_SIZE)/*" 文件结束地址"*/

#ifdef DU_MODLE
/*需量数据文件*/
#define DU_DATA_EEP_ADDR (MNT_EEP_END)
#define DU_DATA_SIZE (sizeof(ST_DU_FILE_DATA))
#define DU_DATA_END (DU_DATA_EEP_ADDR + DU_DATA_SIZE)

/*预付费数据文件*/
#define PM_DATA_EEP_ADDR (DU_DATA_END)
#else
#define PM_DATA_EEP_ADDR (MNT_EEP_END)
#endif
#define PM_DATA_SIZE 0
#define PM_DATA_END (PM_DATA_EEP_ADDR + PM_DATA_SIZE)


/*用户接口数据文件*/
#define UI_DATA_EEP_ADDR (PM_DATA_END)
#define UI_DATA_SIZE (sizeof(ST_UI_FILE_DATA))
#define UI_DATA_END (UI_DATA_EEP_ADDR + UI_DATA_SIZE)


/*记录快照数据文件*/
#define RCRD_DATA_EEP_ADDR (UI_DATA_END)
#define RCRD_DATA_SIZE  (sizeof(ST_RCRD_DATA_FILE))
#define RCRD_DATA_END (RCRD_DATA_EEP_ADDR + RCRD_DATA_SIZE)


/*生产自动化数据文件*/
#define AUTO_DATA_EEP_ADDR (RCRD_DATA_END)
#define AUTO_DATA_SIZE  (sizeof(ST_AUTO_FILE_DATA))
#define AUTO_DATA_END (AUTO_DATA_EEP_ADDR + AUTO_DATA_SIZE)


/*RCRD记录快照文件开始*/
#define RCRD_EEP_ADDR_BEGIN (AUTO_DATA_END)


/*记录快照文件*/
#if defined (SINGLE_PHASE)
    /*分钟冻结索引*/
    #define MIN_FRZ_IDX_ADDR RCRD_EEP_ADDR_BEGIN
    #define MIN_FRZ_IDX_END (MIN_FRZ_IDX_ADDR + MIN_FRZ_IDX_SZ)


    #define RCRD_EEP_ADDR_END (MIN_FRZ_IDX_END)

   #define _MSR_INDEX_ADDR  RCRD_EEP_ADDR_END   /*"自监测数据索引"*/
   #define _MSR_INDEX_SIZE  sizeof(ST_MSRCD_INDEX)
   #define _MSR_INDEX_END   (_MSR_INDEX_ADDR + _MSR_INDEX_SIZE)

#elif defined (THREE_PHASE)
    /*编程事件记录*/
    #define PRG_ADDR RCRD_EEP_ADDR_BEGIN
    #define PRG_END (PRG_ADDR + PRG_SZ)

    /*晶振故障事件记录*/
    #define OSCILLATOR_ERR_EVT_ADDR PRG_END
    #define OSCILLATOR_ERR_EVT_END (OSCILLATOR_ERR_EVT_ADDR + OSCILLATOR_ERR_EVT_SZ)
    
    /*复位故障事件*/
    #define SYS_RST_EVT_ADDR OSCILLATOR_ERR_EVT_END
    #define SYS_RST_EVT_END (SYS_RST_EVT_ADDR + SYS_RST_EVNT_SZ)

    /*月结算冻结*/
    #define MONTH_BILL_ADDR SYS_RST_EVT_END
    #define MONTH_BILL_END (MONTH_BILL_ADDR + MONTH_BILL_SZ)

    /*分钟冻结索引*/
    #define MIN_FRZ_IDX_ADDR MONTH_BILL_END
    #define MIN_FRZ_IDX_END (MIN_FRZ_IDX_ADDR + MIN_FRZ_IDX_SZ)


    #define RCRD_EEP_ADDR_END (MIN_FRZ_IDX_END)

#endif




#define DATA_EEP_ADDR_END RCRD_EEP_ADDR_END

#endif

