/*"***********************************************************		
FileName: PP_Offset.h		
Author: 邹杨       
Version : 1.0       Date: 2014.04.24	
Description: 用于宏定义协议树中 数据中的源数据偏移 
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef _PP_OFFSET_H
#define _PP_OFFSET_H

#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"

#define TOTAL_OFFSET     (2)    /*"(上1)组合有功~(上12)组合有功"*/
#define POACT_OFFSET     (7)      /*"(上1)正向有功~(上12)组合有功"*/
#define NEACT_OFFSET     (12)       /*"(上1)反向有功~(上12)组合有功"*/
#define BILL_LAST_OFFSET (17)
/*"结算时间"*/
#define BILLTIME_OFFSET  (1)    /*"(上1~12)月 结算时间"*/

/*"上1月度组合有功总累计用电量"*/
#define MONBILL_POACT_OFFSET  (2)    /*"月度结算正向有功偏移"*/
#define MONBILL_NEACT_OFFSET  (3)    /*"月度结算反向有功偏移"*/


/*"年结算"*/
#define YEARBILL_TIME_OFFSET  (1)    /*"年结算时间"*/
#define YEARBILL_TOTAL_OFFSET  (2)    /*"年结算累计用电量"*/



/*"事件记录类派生数据-----数据源偏移"*/

/*"对于既有开始又有结束的事件，结束事件需要在对象标识前加0x80偏移以示区别"*/
#define END_OFFSET  (0x80)

/*"掉电事件"*/
#define _PD_BEGIN_OFFSET     (1)        /*"掉电开始时刻偏移"*/
/*南网记录添加*/
#define _PD_BPO_OFFSET       (2)        /*"开始前正向电量"*/
#define _PD_BNE_OFFSET  (3)        /*"开始前反向电量"*/

#define _PD_END_OFFSET       (1)+END_OFFSET       /*"掉电结束时刻偏移"*/
/*南网记录添加*/
#define _PD_EPO_OFFSET       (2)+END_OFFSET        /*"结束前正向电量"*/
#define _PD_ENE_OFFSET        (3)+END_OFFSET        /*"结束前反向电量"*/


/*"编程事件"*/
#define _PRG_YMD_OFFSET      (1)        /*"编程开始年月日偏移"*/
#define _PRG_HMS_OFFSET      (1)        /*"编程开始时分秒偏移"*/
#define _PRG_OPER_OFFSET     (2)        /*"编程操作者代码偏移"*/
#define _PRG_ID_OFFSET       (3)        /*"编程前10个数据代码偏移"*/


/*"电表清零事件"*/
#define _CLRMTR_YMD_OFFSET   (1)        /*"清零年月日偏移"*/
#define _CLRMTR_HMS_OFFSET   (1)        /*"清零时分秒偏移"*/
#define _CLRMTR_OPER_OFFSET  (2)        /*"清零操作者代码偏移"*/
#define _CLRMTR_POACT_OFFSET (3)        /*"清零正向总偏移"*/
#define _CLRMTR_NEACT_OFFSET (4)        /*"清零反向总偏移"*/
#define _CLRMTR_R_ENG_1_OFF    (5)                   /*"开始前无功1偏移"*/
#define _CLRMTR_R_ENG_2_OFF    (6)                   /*"开始前无功2偏移"*/
#define _CLRMTR_R_ENG_3_OFF    (7)                   /*"开始前无功3偏移"*/
#define _CLRMTR_R_ENG_4_OFF    (8)                   /*"开始前无功4偏移"*/


/*"事件清零事件"*/
#define _CLREVT_TIME_OFFSET   (1)        /*"清零时刻偏移"*/
#define _CLREVT_OPER_OFFSET   (2)        /*"清零操作者代码偏移"*/
#define _CLREVT_ID_OFFSET     (3)        /*"清零ID偏移"*/

/*"管理单元更换事件"*/
#define _REPNONLEG_YMD_OFFSET   (1)        /*"管理更换年月日偏移"*/
#define _REPNONLEG_HMS_OFFSET   (1)        /*"管理更换时分秒偏移"*/
#define _REPNONLEG_OPER_OFFSET  (2)        /*"管理更换操作者代码偏移"*/
#define _REPNONLEG_POACT_OFFSET (3)        /*"管理更换正向总偏移"*/
#define _REPNONLEG_NEACT_OFFSET (4)        /*"管理更换反向总偏移"*/
#define _REPNONLEG_R_ENG_1_OFF    (5)                   /*"开始前无功1偏移"*/
#define _REPNONLEG_R_ENG_2_OFF    (6)                   /*"开始前无功2偏移"*/
#define _REPNONLEG_R_ENG_3_OFF    (7)                   /*"开始前无功3偏移"*/
#define _REPNONLEG_R_ENG_4_OFF    (8)                   /*"开始前无功4偏移"*/

#define _REPNONLEG_EPO_OFFSET    (3+END_OFFSET)        /*"结束正向有功偏移"*/
#define _REPNONLEG_ENE_OFFSET    (4+END_OFFSET)        /*"结束反向有功偏移"*/
#define _REPNONLEG_E_R_ENG_1_OFF    (5+END_OFFSET)                   /*"无功1偏移"*/
#define _REPNONLEG_E_R_ENG_2_OFF    (6+END_OFFSET)                   /*"无功2偏移"*/
#define _REPNONLEG_E_R_ENG_3_OFF    (7+END_OFFSET)                   /*"无功3偏移"*/
#define _REPNONLEG_E_R_ENG_4_OFF    (8+END_OFFSET)                   /*"无功4偏移"*/

/*"管理单元升级事件"*/
#define _UPDNONLEG_YMD_OFFSET   (1)        /*"年月日偏移"*/
#define _UPDNONLEG_HMS_OFFSET   (1)        /*"时分秒偏移"*/
#define _UPDNONLEG_OPER_OFFSET  (2)        /*"操作者代码偏移"*/
#define _UPDNONLEG_POACT_OFFSET (3)        /*"正向总偏移"*/
#define _UPDNONLEG_NEACT_OFFSET (4)        /*"反向总偏移"*/
#define _UPDNONLEG_VER_OFFSET   (5)        /*" 版本号"*/

#define _UPDNONLEG_EPO_OFFSET    (3+END_OFFSET)        /*"结束正向有功偏移"*/
#define _UPDNONLEG_ENE_OFFSET    (4+END_OFFSET)        /*"结束反向有功偏移"*/
#define _UPDNONLEG_E_VER_OFFSET    (5+END_OFFSET)        /*" 版本号"*/

//#define _UPDNONLEG_E_R_ENG_1_OFF    (5+END_OFFSET)                   /*"无功1偏移"*/
//#define _UPDNONLEG_E_R_ENG_2_OFF    (6+END_OFFSET)                   /*"无功2偏移"*/
//#define _UPDNONLEG_E_R_ENG_3_OFF    (7+END_OFFSET)                   /*"无功3偏移"*/
//#define _UPDNONLEG_E_R_ENG_4_OFF    (8+END_OFFSET)                   /*"无功4偏移"*/


/*"管理通信异常事件"*/
#define _MGMT_COMM_ERR_YMD_OFFSET   (1)        /*"年月日偏移"*/
#define _MGMT_COMM_ERR_HMS_OFFSET   (1)        /*"时分秒偏移"*/
#define _MGMT_COMM_ERR_POACT_OFFSET (2)        /*"正向总偏移"*/
#define _MGMT_COMM_ERR_NEACT_OFFSET (3)        /*"反向总偏移"*/

#define _MGMT_COMM_ERR_EPO_OFFSET    (2+END_OFFSET)        /*"结束正向有功偏移"*/
#define _MGMT_COMM_ERR_ENE_OFFSET    (3+END_OFFSET)        /*"结束反向有功偏移"*/


/*"校时事件"*/
#define _JUSTTIME_OPER_OFFSET   (1)      /*"操作者代码偏移偏移"*/
#define _JUSTTIME_BEFORE_OFFSET (2)      /*"校时前时间偏移"*/
#define _JUSTTIME_AFTER_OFFSET  (1 + END_OFFSET)      /*"校时后时间偏移"*/

/*"日时段表编程"*/
#define _PRGDSLOT_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _PRGDSLOT_OPER_OFFSET   (2)        /*"操作者代码偏移"*/
#define _PRGDSLOT_TAB1_OFFSET   (3)        /*"操作者代码偏移"*/
#define _PRGDSLOT_TAB2_OFFSET   (11)        /*"操作者代码偏移"*/

/*"年时区表编程"*/
#define _PRGYSLOT_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _PRGYSLOT_OPER_OFFSET   (2)        /*"操作者代码偏移"*/
#define _PRGYSLOT_TAB1_OFFSET   (3)        /*"编程前第一套偏移"*/
#define _PRGYSLOT_TAB2_OFFSET   (4)       /*"编程前第二套偏移"*/

/*"周休日编程"*/
#define _PRGWEEK_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _PRGWEEK_OPER_OFFSET   (2)        /*"操作者代码偏移"*/
#define _PRGWEEK_BEFORE_OFFSET (3)        /*"编程前时段表偏移"*/

/*"有功组合方式编程"*/
#define _PRGAEMODE_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _PRGAEMODE_OPER_OFFSET   (2)        /*"操作者代码偏移"*/
#define _PRGAEMODE_BEFORE_OFFSET (3)        /*"编程前时段表偏移"*/


/*"结算日编程"*/
#define _PRGBILL_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _PRGBILL_OPER_OFFSET   (2)        /*"操作者代码偏移"*/
#define _PRGBILL_BEFORE1_OFFSET (3)        /*"编程前第一结算日偏移"*/
#define _PRGBILL_BEFORE2_OFFSET (4)        /*"编程前第二结算日偏移"*/
#define _PRGBILL_BEFORE3_OFFSET (5)        /*"编程前第三结算日偏移"*/


/*"开盖"*/
#define _OPENCVR_BYMD_OFFSET   (1)                   /*"开始年月日偏移"*/
#define _OPENCVR_BHMS_OFFSET   (1)                   /*"开始时分秒偏移"*/
#define _OPENCVR_EYMD_OFFSET   (1+END_OFFSET)        /*"结束年月日偏移"*/
#define _OPENCVR_EHMS_OFFSET   (1+END_OFFSET)        /*"结束时分秒偏移"*/
#define _OPENCVR_BPO_OFFSET    (2)                   /*"开始前正向有功偏移"*/
#define _OPENCVR_BNE_OFFSET    (3)                   /*"开始前反向有功偏移"*/
#define _OPENCVR_R_ENG_1_OFF    (4)                   /*"开始前无功1偏移"*/
#define _OPENCVR_R_ENG_2_OFF    (5)                   /*"开始前无功2偏移"*/
#define _OPENCVR_R_ENG_3_OFF    (6)                   /*"开始前无功3偏移"*/
#define _OPENCVR_R_ENG_4_OFF    (7)                   /*"开始前无功4偏移"*/

#define _OPENCVR_EPO_OFFSET    (2+END_OFFSET)        /*"结束正向有功偏移"*/
#define _OPENCVR_ENE_OFFSET    (3+END_OFFSET)        /*"结束反向有功偏移"*/
#define _OPENCVR_E_R_ENG_1_OFF    (4+END_OFFSET)                   /*"无功1偏移"*/
#define _OPENCVR_E_R_ENG_2_OFF    (5+END_OFFSET)                   /*"无功2偏移"*/
#define _OPENCVR_E_R_ENG_3_OFF    (6+END_OFFSET)                   /*"无功3偏移"*/
#define _OPENCVR_E_R_ENG_4_OFF    (7+END_OFFSET)                   /*"无功4偏移"*/

/*"费率表编程"*/
#define _PRGTARIFF_TIME_OFFSET   (1)                        /*"时刻偏移"*/
#define _PRGTARIFF_OPER_OFFSET   (2)                        /*"操作者代码偏移"*/
#define _PRGTARIFF_TAB1_OFFSET   (3)                        /*"套1代码偏移"*/
#define _PRGTARIFF_TAB2_OFFSET   (_PRGTARIFF_TAB1_OFFSET + _TARIFF_PRC_NUM)      /*"套2代码偏移"*/


/*"阶梯表编程"*/
#define _PRGSTEP_TIME_OFFSET   (1)         /*"时刻偏移"*/
#define _PRGSTEP_OPER_OFFSET   (2)         /*"操作者代码偏移"*/

#define _PRGSTEP_T11S1_OFFSET   (3)         /*"第1套第1阶梯第1阶梯代码偏移"*/
#define _PRGSTEP_T11S2_OFFSET   (4)         /*"第1套第1阶梯第2阶梯代码偏移"*/
#define _PRGSTEP_T11S3_OFFSET   (5)         /*"第1套第1阶梯第3阶梯代码偏移"*/
#define _PRGSTEP_T11S4_OFFSET   (6)         /*"第1套第1阶梯第4阶梯代码偏移"*/
#define _PRGSTEP_T11S5_OFFSET   (7)         /*"第1套第1阶梯第5阶梯代码偏移"*/
#define _PRGSTEP_T11S6_OFFSET   (8)         /*"第1套第1阶梯第6阶梯代码偏移"*/

#define _PRGSTEP_T11P1_OFFSET   (15)          /*"第1套第1阶梯电价1代码偏移"*/
#define _PRGSTEP_T11P2_OFFSET   (16)         /*"第1套第1阶梯电价2代码偏移"*/
#define _PRGSTEP_T11P3_OFFSET   (17)         /*"第1套第1阶梯电价3代码偏移"*/
#define _PRGSTEP_T11P4_OFFSET   (18)         /*"第1套第1阶梯电价4代码偏移"*/
#define _PRGSTEP_T11P5_OFFSET   (19)         /*"第1套第1阶梯电价5代码偏移"*/
#define _PRGSTEP_T11P6_OFFSET   (20)         /*"第1套第1阶梯电价6代码偏移"*/
#define _PRGSTEP_T11P7_OFFSET   (21)         /*"第1套第1阶梯电价7代码偏移"*/

#define _PRGSTEP_T12S1_OFFSET   (9)         /*"第1套第2阶梯第1阶梯代码偏移"*/
#define _PRGSTEP_T12S2_OFFSET   (10)        /*"第1套第2阶梯第2阶梯代码偏移"*/
#define _PRGSTEP_T12S3_OFFSET   (11)        /*"第1套第2阶梯第3阶梯代码偏移"*/
#define _PRGSTEP_T12S4_OFFSET   (12)        /*"第1套第2阶梯第4阶梯代码偏移"*/
#define _PRGSTEP_T12S5_OFFSET   (13)        /*"第1套第2阶梯第5阶梯代码偏移"*/
#define _PRGSTEP_T12S6_OFFSET   (14)        /*"第1套第2阶梯第6阶梯代码偏移"*/

#define _PRGSTEP_T12P1_OFFSET   (22)          /*"第1套第2阶梯电价1代码偏移"*/
#define _PRGSTEP_T12P2_OFFSET   (23)         /*"第1套第2阶梯电价2代码偏移"*/
#define _PRGSTEP_T12P3_OFFSET   (24)         /*"第1套第2阶梯电价3代码偏移"*/
#define _PRGSTEP_T12P4_OFFSET   (25)         /*"第1套第2阶梯电价4代码偏移"*/
#define _PRGSTEP_T12P5_OFFSET   (26)         /*"第1套第2阶梯电价5代码偏移"*/
#define _PRGSTEP_T12P6_OFFSET   (27)         /*"第1套第2阶梯电价6代码偏移"*/
#define _PRGSTEP_T12P7_OFFSET   (28)         /*"第1套第2阶梯电价7代码偏移"*/


#define _PRGSTEP_T1B1_OFFSET   (29)        /*"第1套第1结算日代码偏移"*/
#define _PRGSTEP_T1B2_OFFSET   (30)        /*"第1套第2结算日代码偏移"*/
#define _PRGSTEP_T1B3_OFFSET   (31)        /*"第1套第3结算日电价代码偏移"*/
#define _PRGSTEP_T1B4_OFFSET   (32)        /*"第1套第4结算日代码偏移"*/
#define _PRGSTEP_T1B5_OFFSET   (33)        /*"第1套第5结算日电价代码偏移"*/
#define _PRGSTEP_T1B6_OFFSET   (34)        /*"第1套第6结算日代码偏移"*/


#define _PRGSTEP_T21S1_OFFSET   (35)        /*"第2套第1阶梯1代码偏移"*/
#define _PRGSTEP_T21S2_OFFSET   (36)        /*"第2套第1阶梯2代码偏移"*/
#define _PRGSTEP_T21S3_OFFSET   (37)        /*"第2套第1阶梯3代码偏移"*/
#define _PRGSTEP_T21S4_OFFSET   (38)        /*"第2套第1阶梯4代码偏移"*/
#define _PRGSTEP_T21S5_OFFSET   (39)        /*"第2套第1阶梯5代码偏移"*/
#define _PRGSTEP_T21S6_OFFSET   (40)        /*"第2套第1阶梯6代码偏移"*/

#define _PRGSTEP_T21P1_OFFSET   (47)       /*"第2套第1阶梯电价1代码偏移"*/
#define _PRGSTEP_T21P2_OFFSET   (48)        /*"第2套第1阶梯电价2代码偏移"*/
#define _PRGSTEP_T21P3_OFFSET   (49)        /*"第2套第1阶梯电价3代码偏移"*/
#define _PRGSTEP_T21P4_OFFSET   (50)        /*"第2套第1阶梯电价4代码偏移"*/
#define _PRGSTEP_T21P5_OFFSET   (51)        /*"第2套第1阶梯电价5代码偏移"*/
#define _PRGSTEP_T21P6_OFFSET   (52)        /*"第2套第1阶梯电价6代码偏移"*/
#define _PRGSTEP_T21P7_OFFSET   (53)        /*"第2套第1阶梯电价7代码偏移"*/

#define _PRGSTEP_T22S1_OFFSET   (41)        /*"第2套第2阶梯1代码偏移"*/
#define _PRGSTEP_T22S2_OFFSET   (42)        /*"第2套第2阶梯2代码偏移"*/
#define _PRGSTEP_T22S3_OFFSET   (43)        /*"第2套第2阶梯3代码偏移"*/
#define _PRGSTEP_T22S4_OFFSET   (44)        /*"第2套第2阶梯4代码偏移"*/
#define _PRGSTEP_T22S5_OFFSET   (45)        /*"第2套第2阶梯5代码偏移"*/
#define _PRGSTEP_T22S6_OFFSET   (46)        /*"第2套第2阶梯6代码偏移"*/

#define _PRGSTEP_T22P1_OFFSET   (54)       /*"第2套第1阶梯电价代码偏移"*/
#define _PRGSTEP_T22P2_OFFSET   (55)        /*"第2套第2阶梯电价代码偏移"*/
#define _PRGSTEP_T22P3_OFFSET   (56)        /*"第2套第3阶梯电价代码偏移"*/
#define _PRGSTEP_T22P4_OFFSET   (57)        /*"第2套第4阶梯电价代码偏移"*/
#define _PRGSTEP_T22P5_OFFSET   (58)        /*"第2套第5阶梯电价代码偏移"*/
#define _PRGSTEP_T22P6_OFFSET   (59)        /*"第2套第6阶梯电价代码偏移"*/
#define _PRGSTEP_T22P7_OFFSET   (60)        /*"第2套第7阶梯电价代码偏移"*/

#define _PRGSTEP_T2B1_OFFSET   (61)        /*"第2套第1结算日代码偏移"*/
#define _PRGSTEP_T2B2_OFFSET   (62)        /*"第2套第2结算日代码偏移"*/
#define _PRGSTEP_T2B3_OFFSET   (63)        /*"第2套第3结算日电价代码偏移"*/
#define _PRGSTEP_T2B4_OFFSET   (64)        /*"第2套第4结算日代码偏移"*/
#define _PRGSTEP_T2B5_OFFSET   (65)        /*"第2套第5结算日代码偏移"*/
#define _PRGSTEP_T2B6_OFFSET   (66)        /*"第2套第6结算日代码偏移"*/


/*"密钥更新事件"*/
#define _KEYUPDATE_TIME_OFFSET  (1)        /*"时刻偏移"*/
#define _KEYUPDATE_OPER_OFFSET  (2)        /*"操作者代码偏移"*/
#define _KEYUPDATE_NUM_OFFSET   (3)        /*"密码条数代码偏移"*/
#define _KEYUPDATE_STA_OFFSET   (4)        /*"更新前状态偏移"*/

/*"异常插卡"*/
#define _CARDERR_TIME_OFFSET   (1)        /*"时刻偏移"*/
#define _CARDERR_SERNUM_OFFSET (2)        /*"卡序列"*/
#define _CARDERR_INFO_OFFSET   (3)        /*"错误信息"*/
#define _CARDERR_HEAD_OFFSET   (4)        /*"操作命令头"*/
#define _CARDERR_STA_OFFSET    (5)        /*"错误响应状态"*/
#define _CARDERR_TIMES_OFFSET  (6)       /*"购电数"*/
#define _CARDERR_REM_OFFSET    (7)          /*"剩余金额"*/
#define _CARDERR_PO_OFFSET     (8)         /*"正向有功总"*/
#define _CARDERR_NE_OFFSET     (9)        /*"反向有功总"*/


/*"退费事件"*/
#define _REFUND_TIME_OFFSET  (1)         /*"时刻偏移"*/
#define _REFUND_TIMES_OFFSET  (2)        /*"退费前购电次数偏移"*/
#define _REFUND_MONEY_OFFSET  (3)        /*"退费金额偏移"*/
#define _REFUND_REMB_OFFSET  (4)        /*"退费前剩余金额偏移"*/
#define _REFUND_REMA_OFFSET  (5)        /*"退费后剩余金额偏移"*/


/*"负荷开关误动作"*/
#define _LOADERR_TIMEB_OFFSET  (1)                      /*"开始时刻偏移"*/
#define _LOADERR_TIMEE_OFFSET  (1+END_OFFSET)           /*"结束时刻偏移"*/
#define _LOADERR_STA_OFFSET    (2)                      /*"误动作后状态偏移"*/
#define _LOADERR_POB_OFFSET    (3)                      /*"开始前正向有功偏移"*/
#define _LOADERR_NEB_OFFSET    (4)                      /*"开始反向有功偏移"*/
#define _LOADERR_POE_OFFSET    (2+END_OFFSET)           /*"结束时正向有功偏移"*/
#define _LOADERR_NEE_OFFSET    (3+END_OFFSET)           /*"结束时反向有功偏移"*/

/*"电源异常"*/
#define _PWRERR_TIMEB_OFFSET  (1)                   /*"开始时刻偏移"*/
#define _PWRDERR_TIMEE_OFFSET  (1+END_OFFSET)       /*"结束时刻偏移"*/
#define _PWRERR_PO_OFFSET  (2)                      /*"正向有功偏移"*/
#define _PWRERR_NE_OFFSET  (3)                      /*"反向有功偏移"*/

/*"购电事件"*/
#define _BUYENGY_TIME_OFFSET  (1)         /*"时刻偏移"*/
#define _BUYENGY_TIMES_OFFSET  (2)         /*"购电后总次数偏移"*/
#define _BUYENGY_BUY_OFFSET  (3)         /*"购电量偏移"*/
#define _BUYENGY_REMB_OFFSET  (4)         /*"购电前剩余偏移"*/
#define _BUYENGY_REMA_OFFSET  (5)         /*"购电后剩余偏移"*/
#define _BUYENGY_TOTAL_OFFSET  (6)         /*"购电累计购电量偏移"*/

/*"跳闸事件"*/
#define _TRIPSW_TIME_OFFSET  (1)         /*"时刻偏移"*/
#define _TRIPSW_OPER_OFFSET  (2)         /*"操作者代码偏移"*/
#define _TRIPSW_PO_OFFSET  (3)           /*"正向有功总偏移"*/
#define _TRIPSW_NE_OFFSET  (4)         /*"反向有功总偏移"*/


/*"合闸事件"*/
#define _CLSSW_TIME_OFFSET  (1)         /*"时刻偏移"*/
#define _CLSSW_OPER_OFFSET  (2)         /*"操作者代码偏移"*/
#define _CLSSW_PO_OFFSET  (3)           /*"正向有功总偏移"*/
#define _CLSSW_NE_OFFSET  (4)         /*"反向有功总偏移"*/


/*"过流事件"*/
#define _OVERCURR_BTIME_OFFSET  (1)        /*"开始时刻偏移"*/
#define _OVERCURR_BPO_OFFSET  (2)          /*"开始时正向有功总偏移"*/
#define _OVERCURR_BNE_OFFSET  (3)          /*"开始时反向有功总偏移"*/
#define _OVERCURR_BV_OFFSET  (4)           /*"开始时电压偏移"*/
#define _OVERCURR_BCURR_OFFSET  (5)        /*"开始时电流偏移"*/
#define _OVERCURR_BPW_OFFSET  (6)          /*"开始时功率偏移"*/
#define _OVERCURR_BFACT_OFFSET  (7)          /*"开始时功率因数偏移"*/
#define _OVERCURR_ETIME_OFFSET  (1+END_OFFSET)        /*"结束时刻偏移"*/
#define _OVERCURR_EPO_OFFSET  (2 +END_OFFSET)         /*"结束时正向有功总偏移"*/
#define _OVERCURR_ENE_OFFSET  (3 +END_OFFSET)        /*"结束时反向有功总偏移"*/



/*"威胜扩展事件记录"*/
#define _RESET_TIME_OFFSET  (1)         /*"时刻偏移"*/


/*"冻结类派生数据----"*/

/*"定时冻结"*/
#define TIMEFRZ_TIME_OFFSET   (1)    /*"时刻偏移"*/
#define TIMEFRZ_PO_OFFSET     (2)    /*"正向有功偏移"*/
#define TIMEFRZ_NE_OFFSET     (7)    /*"反向有功偏移"*/
#define TIMEFRZ_PW_OFFSET     (12)    /*"总有功功率偏移"*/

/*"瞬时冻结"*/
#define MMTFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define MMTFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define MMTFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define MMTFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

    
/*"两套年时区表切换冻结"*/
#define YSLOTSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define YSLOTSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define YSLOTSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define YSLOTSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"两套日时段表切换冻结"*/
#define DSLOTSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define DSLOTSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define DSLOTSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define DSLOTSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"周期冻结"*/
#define PERIODFRZ_TIME_OFFSET   (1)   
#define PERIODFRZ_PO_OFFSET     (2)                   
#define PERIODFRZ_NE_OFFSET     (3)   

/*"分时费率切换冻结"*/
#define TARIFFSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define TARIFFSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define TARIFFSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define TARIFFSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET


/*"日冻结"*/
#define DAYFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define DAYFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define DAYFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define DAYFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"两套阶梯切换冻结"*/
#define STEPSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define STEPSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define STEPSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define STEPSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET


/*"剩余金额日冻结"*/
#define PREPAYFRZ_TIME_OFFSET    (1)
#define PREPAYFRZ_RM_OFFSET      (2)  /*"剩余金额"*/               
#define PREPAYFRZ_OD_OFFSET      (3)  /*"透支金额"*/


#endif
