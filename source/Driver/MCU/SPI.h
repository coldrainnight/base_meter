/*"***********************************************************		
FileName: SPI.h		
Author: ����       
Version : 1.0       Date: 2017.01.09	
Description: SPI����ʵ��
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	


#ifndef SPI_H
#define SPI_H

#include "system\Sys_def.h"

#define SPI_ID_FLASH  (0)   /*SPI FLASH�ӿ�*/
#define SPI_ID_ESAM   (1)   /*SPI ESAM�ӿ�*/
#define SPI_ID_MU     (2)   /*SPI �����ӿ�*/
#define SPI_ID_SLAVE     (3)   /*SPI ����оͨ�� */


#define SPI_NORMAL     1		 /*"�ϵ�״̬SPI״̬ "*/
#define SPI_LOWPOWER   0		 /*"�͹�����SPI״̬ "*/


#define	SPIx_SPICR1_MSPA_Pos	10	/* Master��MISO�źŵĲ���λ�õ��������ڸ���ͨ��ʱ����PCB�����ӳ� */
#define	SPIx_SPICR1_MSPA_Msk	(0x1U << SPIx_SPICR1_MSPA_Pos)
#define	SPIx_SPICR1_MSPA_NORMAL	(0x0U << SPIx_SPICR1_MSPA_Pos)	/* �����㲻���� */
#define	SPIx_SPICR1_MSPA_HALF_DELAY	(0x1U << SPIx_SPICR1_MSPA_Pos)	/* �������ӳٰ��SCK���� */

#define	SPIx_SPICR1_SSPA_Pos	9	/* Slave MISO����λ�õ��� */
#define	SPIx_SPICR1_SSPA_Msk	(0x1U << SPIx_SPICR1_SSPA_Pos)
#define	SPIx_SPICR1_SSPA_NORMAL	(0x0U << SPIx_SPICR1_SSPA_Pos)	/* ���Ͳ���ǰ */
#define	SPIx_SPICR1_SSPA_HALF_AHEAD	(0x1U << SPIx_SPICR1_SSPA_Pos)	/* ��ǰ���SCK���ڷ��� */

#define	SPIx_SPICR1_MM_Pos	8	/* MASTER/SLAVEģʽѡ�� */
#define	SPIx_SPICR1_MM_Msk	(0x1U << SPIx_SPICR1_MM_Pos)
#define	SPIx_SPICR1_MM_SLAVE	(0x0U << SPIx_SPICR1_MM_Pos)	/* Slaveģʽ */
#define	SPIx_SPICR1_MM_MASTER	(0x1U << SPIx_SPICR1_MM_Pos)	/* Masterģʽ */

#define	SPIx_SPICR1_WAIT_Pos	6
#define	SPIx_SPICR1_WAIT_Msk	(0x3U << SPIx_SPICR1_WAIT_Pos)
#define	SPIx_SPICR1_WAIT_WAIT_1	(0x0U << SPIx_SPICR1_WAIT_Pos)	/* ÿ����8bit���ټ���1��sck���ڵȴ�ʱ���ٴ�����һ��8Bit���� */
#define	SPIx_SPICR1_WAIT_WAIT_2	(0x1U << SPIx_SPICR1_WAIT_Pos)	/* ÿ����8bit���ټ���2��sck���ڵȴ�ʱ���ٴ�����һ��8Bit���� */
#define	SPIx_SPICR1_WAIT_WAIT_3	(0x2U << SPIx_SPICR1_WAIT_Pos)	/* ÿ����8bit���ټ���3��sck���ڵȴ�ʱ���ٴ�����һ��8Bit���� */
#define	SPIx_SPICR1_WAIT_WAIT_4	(0x3U << SPIx_SPICR1_WAIT_Pos)	/* ÿ����8bit���ټ���4��sck���ڵȴ�ʱ���ٴ�����һ��8Bit���� */

#define	SPIx_SPICR1_BAUD_Pos	3	/* MASTERģʽ���������� */
#define	SPIx_SPICR1_BAUD_Msk	(0x7U << SPIx_SPICR1_BAUD_Pos)
#define	SPIx_SPICR1_BAUD_PCLK_2	(0x0U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��2��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_4	(0x1U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��4��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_8	(0x2U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��8��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_16	(0x3U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��16��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_32	(0x4U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��32��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_64	(0x5U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��64��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_128	(0x6U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��128��Ƶ */
#define	SPIx_SPICR1_BAUD_PCLK_256	(0x7U << SPIx_SPICR1_BAUD_Pos)	/* ����ʱ��256��Ƶ */

#define	SPIx_SPICR1_LSBF_Pos	2	/* ֡��ʽ */
#define	SPIx_SPICR1_LSBF_Msk	(0x1U << SPIx_SPICR1_LSBF_Pos)
#define	SPIx_SPICR1_LSBF_MSB	(0x0U << SPIx_SPICR1_LSBF_Pos)	/* �ȷ���MSB */
#define	SPIx_SPICR1_LSBF_LSB	(0x1U << SPIx_SPICR1_LSBF_Pos)	/* �ȷ���LSB */

#define	SPIx_SPICR1_CPHOL_Pos	1	/* ʱ�Ӽ���ѡ�� */
#define	SPIx_SPICR1_CPHOL_Msk	(0x1U << SPIx_SPICR1_CPHOL_Pos)
#define	SPIx_SPICR1_CPHOL_LOW	(0x0U << SPIx_SPICR1_CPHOL_Pos)	/* ����ʱ��ֹͣ�ڵ͵�ƽ */
#define	SPIx_SPICR1_CPHOL_HIGH	(0x1U << SPIx_SPICR1_CPHOL_Pos)	/* ����ʱ��ֹͣ�ڸߵ�ƽ */

#define	SPIx_SPICR1_CPHA_Pos	0	/* ʱ����λѡ�� */
#define	SPIx_SPICR1_CPHA_Msk	(0x1U << SPIx_SPICR1_CPHA_Pos)
#define	SPIx_SPICR1_CPHA_FIRST	(0x0U << SPIx_SPICR1_CPHA_Pos)	/* ��һ��ʱ�ӱ����ǵ�һ����׽���� */
#define	SPIx_SPICR1_CPHA_SECOND	(0x1U << SPIx_SPICR1_CPHA_Pos)	/* �ڶ���ʱ�ӱ����ǵ�һ����׽���� */

#define	SPIx_SPICR2_FLTEN_Pos	6	/* Slave����ܽ��˲�ʹ�ܣ�SSN/SCK/MOSI�� */
#define	SPIx_SPICR2_FLTEN_Msk	(0x1U << SPIx_SPICR2_FLTEN_Pos)
	/* 0����ʹ��4ns�˲� */
	/* 1��ʹ��4ns�˲� */

#define	SPIx_SPICR2_SSNM_Pos	5	/* Masterģʽ��SSN����ģʽѡ�� */
#define	SPIx_SPICR2_SSNM_Msk	(0x1U << SPIx_SPICR2_SSNM_Pos)
#define	SPIx_SPICR2_SSNM_LOW	(0x0U << SPIx_SPICR2_SSNM_Pos)	/* ÿ������8bit��Master����SSNΪ�� */
#define	SPIx_SPICR2_SSNM_HIGH	(0x1U << SPIx_SPICR2_SSNM_Pos)	/* ÿ������8bit��Master����SSN��ά�ָߵ�ƽʱ����WAIT�Ĵ������� */

#define	SPIx_SPICR2_TXO_AC_Pos	4	/* TXONLYӲ���Զ���յ�ʹ�� */
#define	SPIx_SPICR2_TXO_AC_Msk	(0x1U << SPIx_SPICR2_TXO_AC_Pos)
#define	SPIx_SPICR2_TXO_AC_CLR_AT_CLOSE	(0x0U << SPIx_SPICR2_TXO_AC_Pos)	/* �ر�TXONLYӲ���Զ����� */
#define	SPIx_SPICR2_TXO_AC_CLR_AT_TX	(0x1U << SPIx_SPICR2_TXO_AC_Pos)	/* TXONLYӲ���Զ�������Ч�����ʹ��TXO�󣬵ȴ�������Ϻ�Ӳ������ */

#define	SPIx_SPICR2_TXO_Pos	3	/* TXONLY����λ */
#define	SPIx_SPICR2_TXO_Msk	(0x1U << SPIx_SPICR2_TXO_Pos)
	/* 0���ر�Master�ĵ�����ģʽ */
	/* 1������Master�ĵ�����ģʽ */

#define	SPIx_SPICR2_SSN_Pos	2	/* Masterģʽ�£��������ͨ����λ����SSN�����ƽ */
#define	SPIx_SPICR2_SSN_Msk	(0x1U << SPIx_SPICR2_SSN_Pos)
#define	SPIx_SPICR2_SSN_LOW	(0x0U << SPIx_SPICR2_SSN_Pos)	/* SSN����͵�ƽ */
#define	SPIx_SPICR2_SSN_HIGH	(0x1U << SPIx_SPICR2_SSN_Pos)	/* SSN����ߵ�ƽ */

#define	SPIx_SPICR2_SSNSEN_Pos	1	/* Masterģʽ�£�SSN�Ŀ��Ʒ�ʽ */
#define	SPIx_SPICR2_SSNSEN_Msk	(0x1U << SPIx_SPICR2_SSNSEN_Pos)
#define	SPIx_SPICR2_SSNSEN_HARD	(0x0U << SPIx_SPICR2_SSNSEN_Pos)	/* Masterģʽ��SSN�����Ӳ���Զ����� */
#define	SPIx_SPICR2_SSNSEN_SOFT	(0x1U << SPIx_SPICR2_SSNSEN_Pos)	/* Masterģʽ��SSN������������ */

#define	SPIx_SPICR2_SPIEN_Pos	0	/* SPIʹ�� */
#define	SPIx_SPICR2_SPIEN_Msk	(0x1U << SPIx_SPICR2_SPIEN_Pos)
	/* 0���ر�SPI */
	/* 1��ʹ��SPI */

#define	SPIx_SPICR3_TXBFC_Pos	3
#define	SPIx_SPICR3_TXBFC_Msk	(0x1U << SPIx_SPICR3_TXBFC_Pos)

#define	SPIx_SPICR3_RXBFC_Pos	2
#define	SPIx_SPICR3_RXBFC_Msk	(0x1U << SPIx_SPICR3_RXBFC_Pos)

#define	SPIx_SPICR3_MERRC_Pos	1
#define	SPIx_SPICR3_MERRC_Msk	(0x1U << SPIx_SPICR3_MERRC_Pos)

#define	SPIx_SPICR3_SERRC_Pos	0
#define	SPIx_SPICR3_SERRC_Msk	(0x1U << SPIx_SPICR3_SERRC_Pos)

#define	SPIx_SPIIE_ERRIE_Pos	2
#define	SPIx_SPIIE_ERRIE_Msk	(0x1U << SPIx_SPIIE_ERRIE_Pos)
	/* 0��HSPI�����жϲ�ʹ�� */
	/* 1��HSPI�����ж�ʹ�� */

#define	SPIx_SPIIE_TXIE_Pos	1
#define	SPIx_SPIIE_TXIE_Msk	(0x1U << SPIx_SPIIE_TXIE_Pos)
	/* 0����������жϲ�ʹ�� */
	/* 1����������ж�ʹ�� */

#define	SPIx_SPIIE_RXIE_Pos	0
#define	SPIx_SPIIE_RXIE_Msk	(0x1U << SPIx_SPIIE_RXIE_Pos)
	/* 0����������жϲ�ʹ�� */
	/* 1����������ж�ʹ�� */

#define	SPIx_SPIIF_MERR_Pos	6	/* Master Error��־ */
#define	SPIx_SPIIF_MERR_Msk	(0x1U << SPIx_SPIIF_MERR_Pos)

#define	SPIx_SPIIF_SERR_Pos	5	/* Slave Error��־ */
#define	SPIx_SPIIF_SERR_Msk	(0x1U << SPIx_SPIIF_SERR_Pos)

#define	SPIx_SPIIF_RXCOL_Pos	4	/* ���ջ�����������д1���� */
#define	SPIx_SPIIF_RXCOL_Msk	(0x1U << SPIx_SPIIF_RXCOL_Pos)

#define	SPIx_SPIIF_TXCOL_Pos	3	/* ���ͻ�����������д1���� */
#define	SPIx_SPIIF_TXCOL_Msk	(0x1U << SPIx_SPIIF_TXCOL_Pos)

#define	SPIx_SPIIF_BUSY_Pos	2	/* SPI���б�־��ֻ�� */
#define	SPIx_SPIIF_BUSY_Msk	(0x1U << SPIx_SPIIF_BUSY_Pos)

#define	SPIx_SPIIF_TXBE_Pos	1	/* TX Buffer Empty��־λ */
#define	SPIx_SPIIF_TXBE_Msk	(0x1U << SPIx_SPIIF_TXBE_Pos)

#define	SPIx_SPIIF_RXBF_Pos	0	/* RX Buffer Full��־λ */
#define	SPIx_SPIIF_RXBF_Msk	(0x1U << SPIx_SPIIF_RXBF_Pos)

#define	SPIx_SPITXBUF_TXBUF_Pos	0	/* SPI���ͻ��� */
#define	SPIx_SPITXBUF_TXBUF_Msk	(0xffU << SPIx_SPITXBUF_TXBUF_Pos)

#define	SPIx_SPIRXBUF_RXBUF_Pos	0	/* SPI���ջ��� */
#define	SPIx_SPIRXBUF_RXBUF_Msk	(0xffU << SPIx_SPIRXBUF_RXBUF_Pos)
//Macro_End


extern void EX_Flash_Spi_MI_OUT_0(void);
extern void EX_Flash_Spi_MI_Fun_Mode(void);
extern void SPI_Init(INT8U SPI_ID,INT8U mode,INT8U *SPI_Cfg);			/*��SPI��ʼ��������*/
extern INT8U SPI_ReadNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*��SPI�����ݺ�����*/
extern INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*��SPIд���ݺ�����*/
extern void SPI_Start (INT8U SPI_ID);
extern void SPI_End (INT8U SPI_ID);

#endif

