#ifndef _RTCCALIAPP_H_
#define _RTCCALIAPP_H_

#define RTCCALI_VERSION    0x20171228
//#define RTC_EeromFlag		0xAA55
 /*"RTC ��������"*/
#define _Err_None			0
#define _Err_EeromInit      1
#define _Err_EeromData		2
#define COMM_ERR_NOERR      0x00    /*"�޴���"*/
#define COMM_ERR_OTHERS     0x01    /*"��������"*/
#define COMM_ERR_NODATA     0x02    /*"����������"*/
#define COMM_ERR_PASSWORD   0x04    /*"Ȩ�޴���"*/

#define CAL_STATUS_START    1
#define CAL_STATUS_STOP     0

#define EEP_PAR_IS_INIT    0x56/*" �ڿ������ѳ�ʼ��,��Ĭ�ϲ���,ram��ҲΪ��Ĭ�ϲ���"*/
#define EEP_PAR_IS_DEF     0xAB/*" �ڿ������ѳ�ʼ��,ram��ΪĬ�ϲ���"*/
#define EEP_PAR_NOT_INIT   0xAC/*" �ڿ�����δ��ʼ��"*/

#define PPM_PER_VALUE      24/*"0.119ppm*100*2"*/

#define _GROUP_SIZE		7 /*"�����С����Ϊ����"*/

#define HXVAR(object, addr)   (*((object *) (addr)))
#define const_tx_flag  		HXVAR( UINT16, 0x1ffffa20 ) //��У��־(0x3cc3Ϊ��̵�У)
#define const_rtc_adj_top 	HXVAR( SINT16,  0x1ffffa36 ) //�������ppm,0.01ppm
#define const_temp_TestA 	HXVAR( UINT8, 0x1FFFFC90 )	//����ʱ�¶�����λ
#define const_temp_TestB 	HXVAR( UINT8, 0x1FFFFC91 )	//����ʱ�¶�С��λ
#define const_adc_Test	 	HXVAR( UINT16, 0x1FFFFC92 )	//����ʱPTAT ADCֵ
#define const_rchf_Trim8 	HXVAR( UINT32, 0x1FFFFD40 )	//RC8M У׼ֵ
#define const_rchf_Trim16 	HXVAR( UINT32, 0x1FFFFD3C )	//RC16M У׼ֵ
#define const_rchf_Trim24 	HXVAR( UINT32, 0x1FFFFD38 )	//RC24M У׼ֵ
#define const_adc_Slope 	HXVAR( UINT16, 0x1FFFFC54 )	//ADCб�ʣ�Լ2.2mV/lsb
#define const_adc_Offset 	HXVAR( SINT16,  0x1FFFFC56 )	//ADC�ؾ�
#define const_ptat_Offset3V	HXVAR( UINT8, 0x1FFFFC94 )	//ptat 5v3v��ֵ

typedef struct    /*"RTC dataflash��������ṹ��"*/
{
	INT16U temperatureTopAdc;		/*"�����¶�ֵADC"*/
}ST_RTC_FLASHPARA;

typedef struct    /*"RTC У������ṹ��"*/
{
	INT16S tempFactHigh;			/*"���¶�ϵ��"*/
	INT16S tempFactLow;			    /*"���¶�ϵ��"*/
	INT16S crystalFactHigh;			/*"���¶���ϵ��"*/
	INT16S crystalFactLow;			/*"�Ͷȶ���ϵ��"*/
	INT16S crystalErr;				/*"��ʼƵƫ"*/
	INT16S temperatureTop;			/*"�����¶�ֵ"*/
	INT16U tempInterval;			/*"�͹��Ĳ��²������"*/
}ST_RTC_CALIPARA;

typedef struct    /*"RTC У������ṹ��"*/
{
	INT16U amend_flag;					/*"   �޵����ر�־"*/
	INT16U pata_value[_GROUP_SIZE];		/*"   �¶�ADC	"*/
	INT16S ppm_value[_GROUP_SIZE];		/*"   ppm���	"*/
}ST_RTC_AMEADPARA;


typedef struct    /*"RTC���������ṹ��"*/
{
	INT16U rtc_defpara_Flag;
	ST_RTC_CALIPARA calipara;	/*"У�� ����"*/
    ST_RTC_AMEADPARA amendpara;/*"��������"*/
	INT16U crc;					/*"crc����ֵ"*/
								
}ST_RTC_PARA;

typedef struct    /*"RTC ����ʱ��ṹ��"*/
{
	INT16U IntPwr;/*"�ϵ粹��ʱ��"*/
	INT16U CntPwr;/*"�ϵ粹��ʱ�����"*/
	INT16U IntLow;/*"�͹��Ĳ���ʱ��"*/
	INT16U CntLow;/*"�͹��Ĳ���ʱ�����"*/
    INT16U mSecCnt;/*"RTC����ÿ�����"*/
}ST_RTC_CALITIME;

typedef struct    /*"RTC ����ʱ��ṹ��"*/
{
	INT32U temperature_ad;/*"����ADֵ"*/
	INT16S temperature_value;/*"�����¶�ֵ"*/
	INT32S ppm_value;/*"����PPMֵ"*/
	INT32S temperature_ppm;/*"�����¶�PPMֵ"*/
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


