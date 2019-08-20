/*"************************************************************"*/
/*"FileName: MNTConfig.h"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: 需量模块对象列表定义"*/
/*" 总字节数:          "*/
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
/*"-------状态监测配置项设计---------------"*/
//#define     PRG_STATUS_CHK       1    /*"编程状态检查"*/
//#define     OPEN_COVER_CHK       1    /*"开表盖检查"*/
//#define     RELAY_ERR_CHK        1    /*"继电器误动作检查"*/
//#define     PWR_ERR_CHK          1    /*"电源异常检查"*/

//#define     PD_OPEN_COVER_CHK    1    /*"低功耗下开盖检查"*/
#define     MAGNETIC_CHK         1    /*"强磁检查"*/
//#define     BAT_VOLT_CHK         1    /*"电池欠压检测"*/
//#define     MU_ERR_CHK         1    /*"计量芯片异常检测"*/

#define     VOLT_STATUS_CHK      1    /*"工作电压状态检查"*/
#define     CURR_STATUS_CHK      1    /*"工作电流状态检查"*/

/*"-------电网事件配置项设计---------------"*/
#define     LOSS_VOLT_CHK        1    /*"失压检测使能"*/
#define     VOLT_SAG_CHK         1    /*"欠压检测使能"*/
#define     VOLT_SWELL_CHK       1    /*"过压检测使能"*/
#define     BREAK_PHASE_CHK      1    /*"断相检测使能"*/
#define     ALL_VOLT_LOSS_CHK    1    /*"全失压检测使能"*/

#define     LOSS_CURR_CHK        1    /*"失流检测使能"*/
//#define     OVER_CURR_CHK        1    /*"过流检测使能"*/
#define     CURR_CUT_CHK         1    /*"断流检测使能"*/

#define     PWR_REVERSE_CHK      1    /*"功率反向检测使能"*/
#define     OVER_PWR_CHK         1    /*"过载检测使能"*/
#define     OVER_DEMAND_CHK      1    /*"需量超限检测使能"*/
#define     FACTOR_LOW_CHK       1    /*"功率因数超下限检测使能"*/

#define     AUX_PWR_LOSS_CHK     1    /*"辅助电源失电检测使能"*/

#define     VOLT_REVERSE_CHK     1    /*"电压逆相序检测使能"*/
#define     CURR_REVERSE_CHK     1    /*"电流逆相序检测使能"*/

#define     VOLT_IMBALANCE_CHK   1    /*"电压不平衡检测使能"*/
#define     CURR_IMBALANCE_CHK   1    /*"电流不平衡检测使能"*/
#define     CURR_BADLY_IMBALANCE_CHK   1    /*"电流严重不平衡检测使能"*/

//#define     BAT_VOLT_CAL   1  /*"电池电压测量（用于电池欠压检测）"*/

#ifdef THREE_PHASE
#define     BAT_NUM      2    /*"表内电池数"*/
#define     COVER_NUM    2    /*"表盖子数"*/
#else
#define     BAT_NUM      2    /*"表内电池数"*/
#define     COVER_NUM    1    /*"表盖子数"*/
#endif

#elif(MNT_VERSION == SOUTH_GRID)
/*"-------状态监测配置项设计---------------"*/
//#define     PRG_STATUS_CHK       1    /*"编程状态检查"*/
#define     OPEN_COVER_CHK       1    /*"开表盖检查"*/
//#define     RELAY_ERR_CHK        1    /*"继电器误动作检查"*/
//#define     PWR_ERR_CHK          1    /*"电源异常检查"*/

//#define     PD_OPEN_COVER_CHK    1    /*"低功耗下开盖检查"*/
#define     MAGNETIC_CHK         1    /*"强磁检查"*/
#define     BAT_VOLT_CHK         1    /*"电池欠压检测"*/
//#define     MU_ERR_CHK         1    /*"计量芯片异常检测"*/

#define     VOLT_STATUS_CHK      1    /*"工作电压状态检查"*/
#define     CURR_STATUS_CHK      1    /*"工作电流状态检查"*/

/*"-------电网事件配置项设计---------------"*/
#define     LOSS_VOLT_CHK        1    /*"失压检测使能"*/
#define     VOLT_SAG_CHK         1    /*"欠压检测使能"*/
#define     VOLT_SWELL_CHK       1    /*"过压检测使能"*/
#define     BREAK_PHASE_CHK      1    /*"断相检测使能"*/
#define     ALL_VOLT_LOSS_CHK    1    /*"全失压检测使能"*/

#define     LOSS_CURR_CHK        1    /*"失流检测使能"*/
//#define     OVER_CURR_CHK        1    /*"过流检测使能"*/
#define     CURR_CUT_CHK         1    /*"断流检测使能"*/

#define     PWR_REVERSE_CHK      1    /*"功率反向检测使能"*/
#define     OVER_PWR_CHK         1    /*"过载检测使能"*/
#define     OVER_DEMAND_CHK      1    /*"需量超限检测使能"*/
#define     FACTOR_LOW_CHK       1    /*"功率因数超下限检测使能"*/

#define     AUX_PWR_LOSS_CHK     1    /*"辅助电源失电检测使能"*/

#define     VOLT_REVERSE_CHK     1    /*"电压逆相序检测使能"*/
#define     CURR_REVERSE_CHK     1    /*"电流逆相序检测使能"*/

#define     VOLT_IMBALANCE_CHK   1    /*"电压不平衡检测使能"*/
#define     CURR_IMBALANCE_CHK   1    /*"电流不平衡检测使能"*/
#define     CURR_BADLY_IMBALANCE_CHK   1    /*"电流严重不平衡检测使能"*/

#define     BAT_VOLT_CAL   1  /*"电池电压测量（用于电池欠压检测）"*/

#ifdef THREE_PHASE
#define     BAT_NUM      2    /*"表内电池数"*/
#define     COVER_NUM    2    /*"表盖子数"*/
#else
#define     BAT_NUM      2    /*"表内电池数"*/
#define     COVER_NUM    1    /*"表盖子数"*/
#endif

#define     TMNL_TMP_CHK   1    /*"端子排温度检测使能"*/

#define     GLX_CHG_CHK   1       /*"管理芯插拔检测使能"*/

#define     GLX_COMM_CHK 1      /*"管理芯通信异常监测使能"*/

#endif

#endif