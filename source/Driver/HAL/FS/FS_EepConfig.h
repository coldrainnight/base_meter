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

#define BEGIN_ADDR 64    /*" ��1ҳ����"*/

/************************�����ļ���ʼ***********************/


/*" �ڿ��е�1�ݼ�������,������246���ֽ�"*/
#define LRM_EEP_ADDR     (BEGIN_ADDR)	 /*" ��������"*/
#define LRM_FILE_SIZE    (sizeof(ST_LRM_FILE_PARA))   /*" ���������������,ռ76�ֽ�"*/
#define LRM_EEP_END	    (LRM_EEP_ADDR + LRM_FILE_SIZE)


/*Ԥ���Ѳ����ļ�*/
#define PM_PARA_EEP_ADDR (LRM_EEP_END)
#define PM_PARA_SIZE 0
#define PM_PARA_END (PM_PARA_EEP_ADDR + PM_PARA_SIZE)


/*�û��ӿ���RAM���ݲ����ļ�*/
#define UI_PARA_WITH_BACKUP_EEP_ADDR (PM_PARA_END)
#define UI_PARA_WITH_BACKUP_SIZE (sizeof(ST_UI_PARA_WITH_BACKUP))
#define UI_PARA_WITH_BACKUP_END (UI_PARA_WITH_BACKUP_EEP_ADDR + UI_PARA_WITH_BACKUP_SIZE)

/*�û��ӿ���RAM���ݲ����ļ�*/
#define UI_PARA_NO_BACKUP_EEP_ADDR (UI_PARA_WITH_BACKUP_END)
#define UI_PARA_NO_BACKUP_SIZE     (sizeof(ST_UI_PARA_NO_BACKUP))
#define UI_PARA_NO_BACKUP_END      (UI_PARA_NO_BACKUP_EEP_ADDR + UI_PARA_NO_BACKUP_SIZE)


/*��ʱ�����ļ�*/
#define TR_PARA_EEP_ADDR (UI_PARA_NO_BACKUP_END)
#define TR_PARA_SIZE (sizeof(ST_TR_FILE_PARA))
#define TR_PARA_END (TR_PARA_EEP_ADDR + TR_PARA_SIZE)

/*��¼���ղ����ļ�*/
#define RCRD_PARA_EEP_ADDR (TR_PARA_END)
#define RCRD_PARA_SIZE  (sizeof(ST_RCRD_PARA_FILE))
#define RCRD_PARA_END (RCRD_PARA_EEP_ADDR + RCRD_PARA_SIZE)

/*���״̬���ģ������ļ�*/
#define MNT_PARA_EEP_ADDR (RCRD_PARA_END)
#define MNT_PARA_SIZE  (sizeof(ST_MNT_PARA))
#define MNT_PARA_END (MNT_PARA_EEP_ADDR + MNT_PARA_SIZE)

#ifdef DU_MODLE
/*����ģ������ļ�*/
#define DU_PARA_EEP_ADDR (MNT_PARA_END)
#define DU_PARA_SIZE  (sizeof(ST_DU_PARA))
#define DU_PARA_END (DU_PARA_EEP_ADDR + DU_PARA_SIZE)

#define PARA_EEP_ADDR_END DU_PARA_END
#else
#define PARA_EEP_ADDR_END MNT_PARA_END
#endif

/************************�����ļ�����***********************/



/*************************�����ļ���ʼ*************************/
/*"��������ģ��"*/
#define LRM_DATA_EEP_ADDR     (PARA_EEP_ADDR_END)    /*" �ļ���ʼ��ַ"*/
#define LRM_DATA_FILE_SIZE    (sizeof(ST_LRM_FILE_DATA))   /*"�ļ���С"*/
#define LRM_DATA_EEP_END      (LRM_DATA_EEP_ADDR + LRM_DATA_FILE_SIZE)/*" �ļ�������ַ"*/

/*�����������*/
#define PD_SAVE_EEP_ADDR (LRM_DATA_EEP_END + (128-(LRM_DATA_EEP_END%128)))
#define PD_SAVE_SIZE (sizeof(ST_PD_SAVE_DATA_ATTR))
#define PD_SAVE_END  (PD_SAVE_EEP_ADDR+PD_SAVE_SIZE)

/*"���״̬���ģ��"*/
#define MNT_EEP_ADDR     (PD_SAVE_END)    /*" �ļ���ʼ��ַ"*/
#define MNT_FILE_SIZE    (sizeof(ST_MNT_FILE_DATA))   /*"�ļ���С"*/
#define MNT_EEP_END      (MNT_EEP_ADDR + MNT_FILE_SIZE)/*" �ļ�������ַ"*/

#ifdef DU_MODLE
/*���������ļ�*/
#define DU_DATA_EEP_ADDR (MNT_EEP_END)
#define DU_DATA_SIZE (sizeof(ST_DU_FILE_DATA))
#define DU_DATA_END (DU_DATA_EEP_ADDR + DU_DATA_SIZE)

/*Ԥ���������ļ�*/
#define PM_DATA_EEP_ADDR (DU_DATA_END)
#else
#define PM_DATA_EEP_ADDR (MNT_EEP_END)
#endif
#define PM_DATA_SIZE 0
#define PM_DATA_END (PM_DATA_EEP_ADDR + PM_DATA_SIZE)


/*�û��ӿ������ļ�*/
#define UI_DATA_EEP_ADDR (PM_DATA_END)
#define UI_DATA_SIZE (sizeof(ST_UI_FILE_DATA))
#define UI_DATA_END (UI_DATA_EEP_ADDR + UI_DATA_SIZE)


/*��¼���������ļ�*/
#define RCRD_DATA_EEP_ADDR (UI_DATA_END)
#define RCRD_DATA_SIZE  (sizeof(ST_RCRD_DATA_FILE))
#define RCRD_DATA_END (RCRD_DATA_EEP_ADDR + RCRD_DATA_SIZE)


/*�����Զ��������ļ�*/
#define AUTO_DATA_EEP_ADDR (RCRD_DATA_END)
#define AUTO_DATA_SIZE  (sizeof(ST_AUTO_FILE_DATA))
#define AUTO_DATA_END (AUTO_DATA_EEP_ADDR + AUTO_DATA_SIZE)


/*RCRD��¼�����ļ���ʼ*/
#define RCRD_EEP_ADDR_BEGIN (AUTO_DATA_END)


/*��¼�����ļ�*/
#if defined (SINGLE_PHASE)
    /*���Ӷ�������*/
    #define MIN_FRZ_IDX_ADDR RCRD_EEP_ADDR_BEGIN
    #define MIN_FRZ_IDX_END (MIN_FRZ_IDX_ADDR + MIN_FRZ_IDX_SZ)


    #define RCRD_EEP_ADDR_END (MIN_FRZ_IDX_END)

   #define _MSR_INDEX_ADDR  RCRD_EEP_ADDR_END   /*"�Լ����������"*/
   #define _MSR_INDEX_SIZE  sizeof(ST_MSRCD_INDEX)
   #define _MSR_INDEX_END   (_MSR_INDEX_ADDR + _MSR_INDEX_SIZE)

#elif defined (THREE_PHASE)
    /*����¼���¼*/
    #define PRG_ADDR RCRD_EEP_ADDR_BEGIN
    #define PRG_END (PRG_ADDR + PRG_SZ)

    /*��������¼���¼*/
    #define OSCILLATOR_ERR_EVT_ADDR PRG_END
    #define OSCILLATOR_ERR_EVT_END (OSCILLATOR_ERR_EVT_ADDR + OSCILLATOR_ERR_EVT_SZ)
    
    /*��λ�����¼�*/
    #define SYS_RST_EVT_ADDR OSCILLATOR_ERR_EVT_END
    #define SYS_RST_EVT_END (SYS_RST_EVT_ADDR + SYS_RST_EVNT_SZ)

    /*�½��㶳��*/
    #define MONTH_BILL_ADDR SYS_RST_EVT_END
    #define MONTH_BILL_END (MONTH_BILL_ADDR + MONTH_BILL_SZ)

    /*���Ӷ�������*/
    #define MIN_FRZ_IDX_ADDR MONTH_BILL_END
    #define MIN_FRZ_IDX_END (MIN_FRZ_IDX_ADDR + MIN_FRZ_IDX_SZ)


    #define RCRD_EEP_ADDR_END (MIN_FRZ_IDX_END)

#endif




#define DATA_EEP_ADDR_END RCRD_EEP_ADDR_END

#endif

