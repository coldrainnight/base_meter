#ifndef LRM_CONFIG_H
#define LRM_CONFIG_H

/*"--------电量数据控制宏定义--------"*/


//#define  QUAD_ACTIVE_ENGY_EN   0  /*"有功四象限计量使能"*/

#if defined (THREE_PHASE)
//#define REACTIVE_ENGY_EN  1    /*"无功电能量计量使能"*/
//#define APPARENT_ENGY_EN  0    /*"视在电能量计量使能"*/
//#define PHASE_ENGY_EN     1    /*"分相电能量计量使能"*/
//#define TARIFF_NUM 8
//#define PWR_DECIMAL_POINT   (-6)/*"功率小数位数"*/
//#define VOLT_DECIMAL_POINT   (-2)/*"电压小数位数"*/
//#define CURR_DECIMAL_POINT   (-4)/*"电流小数位数"*/
//#define FREQ_DECIMAL_POINT   (-6)/*"频率小数位数"*/
//#define FACTOR_DECIMAL_POINT (-6)/*"功率因数小数位数"*/
//#define AMP_HOUR_EN            /*"安时值功能使能开关"*/
#define HARM_DECIMAL_POINT   (-2)/*"谐波含有量小数位数"*/

#elif defined (SINGLE_PHASE)
#define REACTIVE_ENGY_EN  1
//#define APPARENT_ENGY_EN  0
#define TARIFF_NUM 0
//#define AMP_HOUR_EN          /*"安时值功能使能开关"*/
#define PWR_DECIMAL_POINT   (-4)/*"功率小数位数"*/
#define VOLT_DECIMAL_POINT   (-1)/*"电压小数位数"*/
#define CURR_DECIMAL_POINT   (-4)/*"电流小数位数"*/
#define FREQ_DECIMAL_POINT   (-4)/*"频率小数位数"*/
#define FACTOR_DECIMAL_POINT (-4)/*"功率因数小数位数"*/
#define HARM_DECIMAL_POINT   (-2)/*"谐波含有量小数位数"*/
#endif

#ifdef  IEC62056
#if (REACTIVE_ENGY_EN == 1)
//#define  REACTIVE_ENGY_COM
#endif
#endif

/*"----瞬时量数据 控制宏定义------"*/
#if defined (SINGLE_PHASE)
#define MU_ELEMENT_NUM  1       /*"电表计量单元数"*/
#elif defined (THREE_PHASE)
#define MU_ELEMENT_NUM  3       /*"电表计量单元数"*/
#endif

#define HARMONIC_MODE           1       /*" 谐波功能开关"*/

#define MU_HARMONIC_NUM         15      /*"谐波分析次数"*/

//#define  ENGY_REGISTER_ACTIVE_FUN   /*"分时费率电量寄存器激活功能宏开关"*/
#endif
