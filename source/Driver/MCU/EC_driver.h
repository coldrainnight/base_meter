/*"**************************************************************************"*/
/*" CPU（ESAM）卡底层驱动输出文件                                            "*/
/*" 适用于外部调用                                                          "*/
/*"**************************************************************************"*/
#ifndef _EC_DRIVER_H_
#define _EC_DRIVER_H_

#define _ESAM_CLK        0xFF          /*"PG2 ESAM时钟"*/
#define _ESAM_RST        0xff          /*"PB14 ESAM复位引脚"*/
#define _ESAM_SD         0xFF          /*"PG3 ESAM数据引脚"*/
#define _ESAM_VCC        0xff          /*"PB15 ESAM电源引脚"*/

#define _CPU_CLK         0xFF          /*"PC2 CPU卡时钟引脚"*/
#define _CPU_RST         0xff          /*"PB5 CPU卡复位引脚"*/
#define _CPU_SD          0xFF          /*"PC3 CPU卡数据引脚"*/
#define _CPU_SW          0xff          /*"PB4 CPU卡插入检测引脚"*/
#define _CPU_VCC         0xFF          /*" CPU卡电源控制引脚"*/

#define U7816_RX_IE                         ((INT32U)0x00000004)
#define U7816_TX_IE                         ((INT32U)0x00000002)

#define U7816_RX_IF                         ((INT32U)0x00000001)
#define U7816_TX_IF                         ((INT32U)0x00000002)
#define U7816_ERR_IF                        ((INT32U)0x00000004)

typedef void (*EcRevIsr)(INT8U);

extern EcRevIsr mEC_RevIsr;

extern void EC_UartInit(void);

extern void EC_UartRls(void);

extern void EC_Dly (INT16U num);

extern void EC_CRstCool(void);

extern void EC_ERstCool(void);

extern void EC_ChnSel(void);
extern void EC_HardwareInit(INT8U Mode);
extern void EC_SupCpuCfg(INT8U mtrType);

extern void EC_RevData(INT8U data);

extern void EC_UartTx(INT8U T_data);
extern void EC_UartEnTx(void);
extern void EC_UartEnRx(void);
extern void EC_EPwrOff(void);
extern void EC_EInitIO(void);


#endif


