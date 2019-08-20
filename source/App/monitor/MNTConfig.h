/*"************************************************************"*/
/*"FileName: MNTConfig.h"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: ����ģ������б���"*/
/*" ���ֽ���:          "*/
/*" Function List:                     "*/
/*"                                    "*/
/*" History:                           "*/
/*" <author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#ifndef MNT_CONFIG_H
#define MNT_CONFIG_H

#include "PlatForm.h"

#define  STATE_GRID   (1)
#define  SOUTH_GRID   (2)
#define  MNT_VERSION  SOUTH_GRID

#if(MNT_VERSION == STATE_GRID)
/*"-------״̬������������---------------"*/
//#define     PRG_STATUS_CHK       1    /*"���״̬���"*/
//#define     OPEN_COVER_CHK       1    /*"����Ǽ��"*/
//#define     RELAY_ERR_CHK        1    /*"�̵����������"*/
//#define     PWR_ERR_CHK          1    /*"��Դ�쳣���"*/

//#define     PD_OPEN_COVER_CHK    1    /*"�͹����¿��Ǽ��"*/
#define     MAGNETIC_CHK         1    /*"ǿ�ż��"*/
//#define     BAT_VOLT_CHK         1    /*"���Ƿѹ���"*/
//#define     MU_ERR_CHK         1    /*"����оƬ�쳣���"*/

#define     VOLT_STATUS_CHK      1    /*"������ѹ״̬���"*/
#define     CURR_STATUS_CHK      1    /*"��������״̬���"*/

/*"-------�����¼����������---------------"*/
#define     LOSS_VOLT_CHK        1    /*"ʧѹ���ʹ��"*/
#define     VOLT_SAG_CHK         1    /*"Ƿѹ���ʹ��"*/
#define     VOLT_SWELL_CHK       1    /*"��ѹ���ʹ��"*/
#define     BREAK_PHASE_CHK      1    /*"������ʹ��"*/
#define     ALL_VOLT_LOSS_CHK    1    /*"ȫʧѹ���ʹ��"*/

#define     LOSS_CURR_CHK        1    /*"ʧ�����ʹ��"*/
//#define     OVER_CURR_CHK        1    /*"�������ʹ��"*/
#define     CURR_CUT_CHK         1    /*"�������ʹ��"*/

#define     PWR_REVERSE_CHK      1    /*"���ʷ�����ʹ��"*/
#define     OVER_PWR_CHK         1    /*"���ؼ��ʹ��"*/
#define     OVER_DEMAND_CHK      1    /*"�������޼��ʹ��"*/
#define     FACTOR_LOW_CHK       1    /*"�������������޼��ʹ��"*/

#define     AUX_PWR_LOSS_CHK     1    /*"������Դʧ����ʹ��"*/

#define     VOLT_REVERSE_CHK     1    /*"��ѹ��������ʹ��"*/
#define     CURR_REVERSE_CHK     1    /*"������������ʹ��"*/

#define     VOLT_IMBALANCE_CHK   1    /*"��ѹ��ƽ����ʹ��"*/
#define     CURR_IMBALANCE_CHK   1    /*"������ƽ����ʹ��"*/
#define     CURR_BADLY_IMBALANCE_CHK   1    /*"�������ز�ƽ����ʹ��"*/

//#define     BAT_VOLT_CAL   1  /*"��ص�ѹ���������ڵ��Ƿѹ��⣩"*/

#ifdef THREE_PHASE
#define     BAT_NUM      2    /*"���ڵ����"*/
#define     COVER_NUM    2    /*"�������"*/
#else
#define     BAT_NUM      2    /*"���ڵ����"*/
#define     COVER_NUM    1    /*"�������"*/
#endif

#elif(MNT_VERSION == SOUTH_GRID)
/*"-------״̬������������---------------"*/
//#define     PRG_STATUS_CHK       1    /*"���״̬���"*/
#define     OPEN_COVER_CHK       1    /*"����Ǽ��"*/
//#define     RELAY_ERR_CHK        1    /*"�̵����������"*/
//#define     PWR_ERR_CHK          1    /*"��Դ�쳣���"*/

//#define     PD_OPEN_COVER_CHK    1    /*"�͹����¿��Ǽ��"*/
#define     MAGNETIC_CHK         1    /*"ǿ�ż��"*/
#define     BAT_VOLT_CHK         1    /*"���Ƿѹ���"*/
//#define     MU_ERR_CHK         1    /*"����оƬ�쳣���"*/

#define     VOLT_STATUS_CHK      1    /*"������ѹ״̬���"*/
#define     CURR_STATUS_CHK      1    /*"��������״̬���"*/

/*"-------�����¼����������---------------"*/
#define     LOSS_VOLT_CHK        1    /*"ʧѹ���ʹ��"*/
#define     VOLT_SAG_CHK         1    /*"Ƿѹ���ʹ��"*/
#define     VOLT_SWELL_CHK       1    /*"��ѹ���ʹ��"*/
#define     BREAK_PHASE_CHK      1    /*"������ʹ��"*/
#define     ALL_VOLT_LOSS_CHK    1    /*"ȫʧѹ���ʹ��"*/

#define     LOSS_CURR_CHK        1    /*"ʧ�����ʹ��"*/
//#define     OVER_CURR_CHK        1    /*"�������ʹ��"*/
#define     CURR_CUT_CHK         1    /*"�������ʹ��"*/

#define     PWR_REVERSE_CHK      1    /*"���ʷ�����ʹ��"*/
#define     OVER_PWR_CHK         1    /*"���ؼ��ʹ��"*/
#define     OVER_DEMAND_CHK      1    /*"�������޼��ʹ��"*/
#define     FACTOR_LOW_CHK       1    /*"�������������޼��ʹ��"*/

#define     AUX_PWR_LOSS_CHK     1    /*"������Դʧ����ʹ��"*/

#define     VOLT_REVERSE_CHK     1    /*"��ѹ��������ʹ��"*/
#define     CURR_REVERSE_CHK     1    /*"������������ʹ��"*/

#define     VOLT_IMBALANCE_CHK   1    /*"��ѹ��ƽ����ʹ��"*/
#define     CURR_IMBALANCE_CHK   1    /*"������ƽ����ʹ��"*/
#define     CURR_BADLY_IMBALANCE_CHK   1    /*"�������ز�ƽ����ʹ��"*/

#define     BAT_VOLT_CAL   1  /*"��ص�ѹ���������ڵ��Ƿѹ��⣩"*/

#ifdef THREE_PHASE
#define     BAT_NUM      2    /*"���ڵ����"*/
#define     COVER_NUM    2    /*"�������"*/
#else
#define     BAT_NUM      2    /*"���ڵ����"*/
#define     COVER_NUM    1    /*"�������"*/
#endif

#define     TMNL_TMP_CHK   1    /*"�������¶ȼ��ʹ��"*/

#define     GLX_CHG_CHK   1       /*"����о��μ��ʹ��"*/

#define     GLX_COMM_CHK 1      /*"����оͨ���쳣���ʹ��"*/

#endif

#endif