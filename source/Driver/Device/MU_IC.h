#ifndef _MC_IC_H_
#define _MC_IC_H_

/*"--------校表参数及其计量控制寄存器--------"*/



/*"     register    address         R/W     length  reset_value     function"*/
#define R_URMS      0x2000  /*"     R       4	    0000h           读电压"*/
#define R_IARMS     0x2001  /*"     R       4	    0000h           读火线电流"*/
#define R_PWRPA     0x2002  /*"     R       4	    0000h           读火线有功功率"*/
#define R_PWRQA     0x2003  /*"     R       4	    0000h           读火线无功功率"*/
#define R_PWRSA     0x2004  /*"     R       4	    0000h           读火线视在功率"*/
#define R_FCTRA     0x2005  /*"     R       4	    0000h           读火线功率因素"*/
#define R_UFREQ     0x2030  /*"     R       4	    0000h           读频率"*/
#define R_IBRMS     0x2031  /*"     R       4	    0000h           读零线电流"*/
#define R_PWRPB     0x2032  /*"     R       4	    0000h           读零线有功功率"*/
#define R_PWRQB     0x2033  /*"     R       4       0000h           读零线无功功率"*/
#define R_PWRSB     0x2034  /*"     R       4	    0000h           读零线视在功率"*/
#define R_FCTRB     0x2035  /*"     R       4	    0000h           读零线功率因素"*/
#define R_ALLD      0x204F  /*"     R       40	    -----           打包读瞬时量"*/

#define R_ALLE_STA  0x205E  /*"     R       4	    -----           打包读在线监测误差状态"*/
#define R_ALLE      0x205F  /*"     R       48	    -----           打包读在线监测误差"*/

#define R_ALLH_LITE 0x206E  /*"     R       48	    -----           打包读谐波精简数据（每秒更新）"*/
#define R_ALLH      0x206F  /*"     R       48	    -----           打包读谐波数据"*/


#define R_VCOEF     0x2100  /*"     R       2	    0000h           读电压换算系数 "*/
#define R_IACOEF    0x2101  /*"     R       2	    0000h           读火线电流换算系数"*/
#define R_PACOEF    0x2102  /*"     R       2	    0000h           读火线功率换算系数"*/
#define R_GPQA      0x2103  /*"     R       2	    0000h           读火线功率增益"*/
#define R_PHSA      0x2104  /*"     R       2	    0000h           读火线相位补偿"*/
#define R_APOSA     0x2105  /*"     R       2	    0000h           读火线功率偏置"*/
#define R_IBCOEF    0x2131  /*"     R       2	    0000h           读零线电流换算系数"*/
#define R_PBCOEF    0x2132  /*"     R       2	    0000h           读零线功率换算系数"*/
#define R_GPQB      0x2133  /*"     R       2	    0000h           读零线功率增益"*/
#define R_PHSB      0x2134  /*"     R       2	    0000h           读零线相位补偿"*/
#define R_APOSB     0x2135  /*"     R       2	    0000h           读零线功率偏置"*/
#define R_HFCNST    0x2140  /*"     R       2	    0000h           读计量高频脉冲常数"*/
#define R_PSTRT     0x2141  /*"     R       2	    0000h           读有功潜动阀值"*/
#define R_QSTRT     0x2142  /*"     R       2	    0000h           读无功潜动阀值"*/
#define R_MTRCFG    0x2143  /*"     R       2	    0000h           读计量配置参数1"*/
#define R_MTRCFG2   0x2144  /*"     R       2	    0000h           读计量配置参数2"*/
#define R_ECCNST    0x2145  /*"     R       2	    0000h           读电表有功脉冲常数"*/
#define R_VERSION   0x2146  /*"     R       2	    0000h           读电表版本号"*/
#define R_ALLP      0x214F  /*"     R       58	    0000h           打包读校表参数"*/

#define R_ALLO      0x216F  /*"     R       4	    0000h           打包读负荷识别参数"*/

#define	R_I_AMPSTD	    0x2150	/*"     R       4	    -----       读自监测电流回路幅值参考"*/
#define	R_I_PHSSTD	    0x2151	/*"     R       4	    -----       读自监测电流回路相位参考"*/
#define	R_U400_AMPSTD	0x2152	/*"     R       4	    -----       读自监测电压回路幅值参考"*/
#define	R_U400_PHSSTD	0x2153	/*"     R       4	    -----       读自监测电压回路相位参考"*/
#define	R_U10_AMPSTD	0x2154	/*"     R       4	    -----       读自监测电压回路幅值参考1"*/
#define	R_U10_PHSSTD	0x2155	/*"     R       4	    -----       读自监测电压回路相位参考1"*/
#define	R_U10_AMPSTD2	0x2156	/*"     R       4	    -----       读自监测电压回路幅值参考2"*/
#define	R_U10_PHSSTD2	0x2157	/*"     R       4	    -----       读自监测电压回路相位参考2"*/


#define W_VCOEF     0x2300  /*"     W       2	    0000h           写电压换算系数 "*/
#define W_IACOEF    0x2301  /*"     W       2	    0000h           写火线电流换算系数"*/
#define W_PACOEF    0x2302  /*"     W       2	    0000h           写火线功率换算系数"*/
#define W_GPQA      0x2303  /*"     W       2	    0000h           写火线功率增益"*/
#define W_PHSA      0x2304  /*"     W       2	    0000h           写火线相位补偿"*/
#define W_APOSA     0x2305  /*"     W       2	    0000h           写火线功率偏置"*/
#define W_IBCOEF    0x2331  /*"     W       2	    0000h           写零线电流换算系数"*/
#define W_PBCOEF    0x2332  /*"     W       2	    0000h           写零线功率换算系数"*/
#define W_GPQB      0x2333  /*"     W       2	    0000h           写零线功率增益"*/
#define W_PHSB      0x2334  /*"     W       2	    0000h           写零线相位补偿"*/
#define W_APOSB     0x2335  /*"     W       2	    0000h           写零线功率偏置"*/
#define W_HFCNST    0x2340  /*"     W       2	    0000h           写计量高频脉冲常数"*/
#define W_PSTRT     0x2341  /*"     W       2	    0000h           写有功潜动阀值"*/
#define W_QSTRT     0x2342  /*"     W       2	    0000h           写无功潜动阀值"*/
#define W_MTRCFG    0x2343  /*"     W       2	    0000h           写计量配置参数1"*/
#define W_MTRCFG2   0x2344  /*"     W       2	    0000h           写计量配置参数2"*/
#define W_ECCNST    0x2345  /*"     W       2	    0000h           写电表有功脉冲常数"*/
#define W_ALLP      0x234F  /*"     W       58	    0000h           打包写校表参数"*/

#define	W_I_AMPSTD	    0x2350	/*"     W       4	    -----       写自监测电流回路幅值参考"*/
#define	W_I_PHSSTD	    0x2351	/*"     W       4	    -----       写自监测电流回路相位参考"*/
#define	W_U400_AMPSTD	0x2352	/*"     W       4	    -----       写自监测电压回路幅值参考"*/
#define	W_U400_PHSSTD	0x2353	/*"     W       4	    -----       写自监测电压回路相位参考"*/
#define	W_U10_AMPSTD	0x2354	/*"     W       4	    -----       写自监测电压回路幅值参考1"*/
#define	W_U10_PHSSTD	0x2355	/*"     W       4	    -----       写自监测电压回路相位参考1"*/
#define	W_U10_AMPSTD2	0x2356	/*"     W       4	    -----       写自监测电压回路幅值参考2"*/
#define	W_U10_PHSSTD2	0x2357	/*"     W       4	    -----       写自监测电压回路相位参考2"*/


#define W_SHAKE     0x24F0  /*"     W       2	    0000h           握手命令"*/
#define W_START     0x24F1  /*"     W       2	    0000h           校表开始"*/
#define W_CLEAR     0x24F2  /*"     W       2	    0000h           校表参数清零"*/
#define W_SAVE      0x24F3  /*"     W       2	    0000h           校表参数保存"*/
#define W_STOP      0x24F4  /*"     W       2	    0000h           校表结束"*/
#define W_RESET     0x24F5  /*"     W       2	    0000h           校表参数复归"*/
#define W_NILM		0x24F6	/*"     W       2	    0000h           nilm spi 启停"*/

#define _MU_IC_WaitTIME     500//50		/*" 串口读写等待最大时间"*/

/*"pluse in interface"*/
#define _PLU_AFFIRM_TIME    (20)   /*"脉冲防抖时间"*/
#define _PLUSE_DETECT       (LOW) /*"检测到脉冲输入电平"*/

#define TX_LENGTH   96
#define RX_LENGTH   96


typedef struct
{
    INT8U   Uart_Tx_Data[TX_LENGTH];
    INT8U   Uart_Tx_length;
    INT8U   Uart_TX_Chksum;
    INT8U   Uart_Txc_flag;
    INT8U   Uart_Rx_Data[RX_LENGTH];
    INT8U   Uart_Rx_length;
    INT8U   Uart_RX_Chksum;
    INT8U   Uart_Rxc_flag;
    INT8U   Uart_recent_length;
}MU_IC_UART;

extern MU_IC_UART gMeaIc_Uart;

extern INT8U MU_IC_WriteReg(INT16U reg,INT8U *data,INT8U length);
extern INT8U MU_IC_ReadReg(INT16U reg,INT8U length);
extern void MU_IC_Init(INT8U mode);
extern void MC_Reset(void);
extern INT8U  MU_IC_PulseInit(INT8U device, INT32U constRatio,INT8U  pulseWidth);
extern INT8U MU_IC_ReadPulse(INT8U *pdata);
extern void MC_OpenPulse(void);
extern void MC_ClosePulse(void);
extern void MU_IC_PD_Close(void);
extern INT16U MU_IC_GetPulse_Val(void);
extern void MU_IC_PluseAffirm(void);
extern INT16U MU_IC_GetNoAffrimPulse_Val(void);
extern void MU_IC_ClrPulseBeginFlg(void);
extern INT16U MU_IC_Get_Q_Pulse_Val(void);
extern INT16U MU_IC_Get_Q_NoAffrimPulse_Val(void);

#endif

