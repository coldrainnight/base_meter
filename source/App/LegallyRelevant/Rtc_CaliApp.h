#ifndef _RTCCALIAPP_H_
#define _RTCCALIAPP_H_

#define RTCCALI_VERSION    0x20171228
//#define RTC_EeromFlag		0xAA55
 /*"RTC 错误类型"*/
#define _Err_None			0
#define _Err_EeromInit      1
#define _Err_EeromData		2
#define COMM_ERR_NOERR      0x00    /*"无错误"*/
#define COMM_ERR_OTHERS     0x01    /*"其它错误"*/
#define COMM_ERR_NODATA     0x02    /*"无数据请求"*/
#define COMM_ERR_PASSWORD   0x04    /*"权限错误"*/

#define CAL_STATUS_START    1
#define CAL_STATUS_STOP     0

#define EEP_PAR_IS_INIT    0x56/*" 内卡参数已初始化,非默认参数,ram中也为非默认参数"*/
#define EEP_PAR_IS_DEF     0xAB/*" 内卡参数已初始化,ram中为默认参数"*/
#define EEP_PAR_NOT_INIT   0xAC/*" 内卡参数未初始化"*/

#define PPM_PER_VALUE      24/*"0.119ppm*100*2"*/

#define _GROUP_SIZE		7 /*"数组大小必须为奇数"*/

#define HXVAR(object, addr)   (*((object *) (addr)))
#define const_tx_flag  		HXVAR( UINT16, 0x1ffffa20 ) //调校标志(0x3cc3为编程调校)
#define const_rtc_adj_top 	HXVAR( SINT16,  0x1ffffa36 ) //顶点误差ppm,0.01ppm
#define const_temp_TestA 	HXVAR( UINT8, 0x1FFFFC90 )	//测试时温度整数位
#define const_temp_TestB 	HXVAR( UINT8, 0x1FFFFC91 )	//测试时温度小数位
#define const_adc_Test	 	HXVAR( UINT16, 0x1FFFFC92 )	//测试时PTAT ADC值
#define const_rchf_Trim8 	HXVAR( UINT32, 0x1FFFFD40 )	//RC8M 校准值
#define const_rchf_Trim16 	HXVAR( UINT32, 0x1FFFFD3C )	//RC16M 校准值
#define const_rchf_Trim24 	HXVAR( UINT32, 0x1FFFFD38 )	//RC24M 校准值
#define const_adc_Slope 	HXVAR( UINT16, 0x1FFFFC54 )	//ADC斜率，约2.2mV/lsb
#define const_adc_Offset 	HXVAR( SINT16,  0x1FFFFC56 )	//ADC截距
#define const_ptat_Offset3V	HXVAR( UINT8, 0x1FFFFC94 )	//ptat 5v3v差值

typedef struct    /*"RTC dataflash区域参数结构体"*/
{
	INT16U temperatureTopAdc;		/*"顶点温度值ADC"*/
}ST_RTC_FLASHPARA;

typedef struct    /*"RTC 校表参数结构体"*/
{
	INT16S tempFactHigh;			/*"高温度系数"*/
	INT16S tempFactLow;			    /*"低温度系数"*/
	INT16S crystalFactHigh;			/*"高温二次系数"*/
	INT16S crystalFactLow;			/*"低度二次系数"*/
	INT16S crystalErr;				/*"初始频偏"*/
	INT16S temperatureTop;			/*"顶点温度值"*/
	INT16U tempInterval;			/*"低功耗测温补偿间隔"*/
}ST_RTC_CALIPARA;

typedef struct    /*"RTC 校表参数结构体"*/
{
	INT16U amend_flag;					/*"   修调开关标志"*/
	INT16U pata_value[_GROUP_SIZE];		/*"   温度ADC	"*/
	INT16S ppm_value[_GROUP_SIZE];		/*"   ppm误差	"*/
}ST_RTC_AMEADPARA;


typedef struct    /*"RTC补偿参数结构体"*/
{
	INT16U rtc_defpara_Flag;
	ST_RTC_CALIPARA calipara;	/*"校表 参数"*/
    ST_RTC_AMEADPARA amendpara;/*"精调参数"*/
	INT16U crc;					/*"crc检验值"*/
								
}ST_RTC_PARA;

typedef struct    /*"RTC 补偿时间结构体"*/
{
	INT16U IntPwr;/*"上电补偿时间"*/
	INT16U CntPwr;/*"上电补偿时间计数"*/
	INT16U IntLow;/*"低功耗补偿时间"*/
	INT16U CntLow;/*"低功耗补偿时间计数"*/
    INT16U mSecCnt;/*"RTC补偿每秒计数"*/
}ST_RTC_CALITIME;

typedef struct    /*"RTC 补偿时间结构体"*/
{
	INT32U temperature_ad;/*"测温AD值"*/
	INT16S temperature_value;/*"环境温度值"*/
	INT32S ppm_value;/*"补偿PPM值"*/
	INT32S temperature_ppm;/*"晶体温度PPM值"*/
    INT32S init_ppm;
    INT32S remain_ppm;
    INT32S last_calivalue;
}ST_RTC_MEADATA;

extern ST_RTC_PARA mstRtcPara;
extern ST_RTC_MEADATA mstRtcMeadata;
extern INT32U temp_ppm_value;

extern  void RTC_CaliInit (INT8U Mode);
extern  void RTC_CaliMain (INT8U Mode);
extern  INT8U RTC_CaliRead (INT8U *pBuff);
extern  INT8U RTC_CaliWrite (INT8U *pBuff);
extern  INT8U RTC_CaliCtrl (INT8U *pBuff);

#endif


