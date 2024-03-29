/*"***********************************************************		
FileName: SPI.h		
Author: 邹杨       
Version : 1.0       Date: 2017.01.09	
Description: SPI驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	


#ifndef SPI_H
#define SPI_H

#include "system\Sys_def.h"

#define SPI_ID_FLASH  (0)   /*SPI FLASH接口*/
#define SPI_ID_ESAM   (1)   /*SPI ESAM接口*/
#define SPI_ID_MU     (2)   /*SPI 计量接口*/
#define SPI_ID_SLAVE     (3)   /*SPI 管理芯通信 */


#define SPI_NORMAL     1		 /*"上电状态SPI状态 "*/
#define SPI_LOWPOWER   0		 /*"低功耗下SPI状态 "*/


#define	SPIx_SPICR1_MSPA_Pos	10	/* Master对MISO信号的采样位置调整，用于高速通信时补偿PCB走线延迟 */
#define	SPIx_SPICR1_MSPA_Msk	(0x1U << SPIx_SPICR1_MSPA_Pos)
#define	SPIx_SPICR1_MSPA_NORMAL	(0x0U << SPIx_SPICR1_MSPA_Pos)	/* 采样点不调整 */
#define	SPIx_SPICR1_MSPA_HALF_DELAY	(0x1U << SPIx_SPICR1_MSPA_Pos)	/* 采样点延迟半个SCK周期 */

#define	SPIx_SPICR1_SSPA_Pos	9	/* Slave MISO发送位置调整 */
#define	SPIx_SPICR1_SSPA_Msk	(0x1U << SPIx_SPICR1_SSPA_Pos)
#define	SPIx_SPICR1_SSPA_NORMAL	(0x0U << SPIx_SPICR1_SSPA_Pos)	/* 发送不提前 */
#define	SPIx_SPICR1_SSPA_HALF_AHEAD	(0x1U << SPIx_SPICR1_SSPA_Pos)	/* 提前半个SCK周期发送 */

#define	SPIx_SPICR1_MM_Pos	8	/* MASTER/SLAVE模式选择 */
#define	SPIx_SPICR1_MM_Msk	(0x1U << SPIx_SPICR1_MM_Pos)
#define	SPIx_SPICR1_MM_SLAVE	(0x0U << SPIx_SPICR1_MM_Pos)	/* Slave模式 */
#define	SPIx_SPICR1_MM_MASTER	(0x1U << SPIx_SPICR1_MM_Pos)	/* Master模式 */

#define	SPIx_SPICR1_WAIT_Pos	6
#define	SPIx_SPICR1_WAIT_Msk	(0x3U << SPIx_SPICR1_WAIT_Pos)
#define	SPIx_SPICR1_WAIT_WAIT_1	(0x0U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入1个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_2	(0x1U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入2个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_3	(0x2U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入3个sck周期等待时间再传输下一个8Bit数据 */
#define	SPIx_SPICR1_WAIT_WAIT_4	(0x3U << SPIx_SPICR1_WAIT_Pos)	/* 每发完8bit至少加入4个sck周期等待时间再传输下一个8Bit数据 */

#define	SPIx_SPICR1_BAUD_Pos	3	/* MASTER模式波特率配置 */
#define	SPIx_SPICR1_BAUD_Msk	(0x7U << SPIx_SPICR1_BAUD_Pos)
#define	SPIx_SPICR1_BAUD_PCLK_2	(0x0U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟2分频 */
#define	SPIx_SPICR1_BAUD_PCLK_4	(0x1U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟4分频 */
#define	SPIx_SPICR1_BAUD_PCLK_8	(0x2U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟8分频 */
#define	SPIx_SPICR1_BAUD_PCLK_16	(0x3U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟16分频 */
#define	SPIx_SPICR1_BAUD_PCLK_32	(0x4U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟32分频 */
#define	SPIx_SPICR1_BAUD_PCLK_64	(0x5U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟64分频 */
#define	SPIx_SPICR1_BAUD_PCLK_128	(0x6U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟128分频 */
#define	SPIx_SPICR1_BAUD_PCLK_256	(0x7U << SPIx_SPICR1_BAUD_Pos)	/* 外设时钟256分频 */

#define	SPIx_SPICR1_LSBF_Pos	2	/* 帧格式 */
#define	SPIx_SPICR1_LSBF_Msk	(0x1U << SPIx_SPICR1_LSBF_Pos)
#define	SPIx_SPICR1_LSBF_MSB	(0x0U << SPIx_SPICR1_LSBF_Pos)	/* 先发送MSB */
#define	SPIx_SPICR1_LSBF_LSB	(0x1U << SPIx_SPICR1_LSBF_Pos)	/* 先发送LSB */

#define	SPIx_SPICR1_CPHOL_Pos	1	/* 时钟极性选择 */
#define	SPIx_SPICR1_CPHOL_Msk	(0x1U << SPIx_SPICR1_CPHOL_Pos)
#define	SPIx_SPICR1_CPHOL_LOW	(0x0U << SPIx_SPICR1_CPHOL_Pos)	/* 串行时钟停止在低电平 */
#define	SPIx_SPICR1_CPHOL_HIGH	(0x1U << SPIx_SPICR1_CPHOL_Pos)	/* 串行时钟停止在高电平 */

#define	SPIx_SPICR1_CPHA_Pos	0	/* 时钟相位选择 */
#define	SPIx_SPICR1_CPHA_Msk	(0x1U << SPIx_SPICR1_CPHA_Pos)
#define	SPIx_SPICR1_CPHA_FIRST	(0x0U << SPIx_SPICR1_CPHA_Pos)	/* 第一个时钟边沿是第一个捕捉边沿 */
#define	SPIx_SPICR1_CPHA_SECOND	(0x1U << SPIx_SPICR1_CPHA_Pos)	/* 第二个时钟边沿是第一个捕捉边沿 */

#define	SPIx_SPICR2_FLTEN_Pos	6	/* Slave输入管脚滤波使能（SSN/SCK/MOSI） */
#define	SPIx_SPICR2_FLTEN_Msk	(0x1U << SPIx_SPICR2_FLTEN_Pos)
	/* 0：不使能4ns滤波 */
	/* 1：使能4ns滤波 */

#define	SPIx_SPICR2_SSNM_Pos	5	/* Master模式下SSN控制模式选择 */
#define	SPIx_SPICR2_SSNM_Msk	(0x1U << SPIx_SPICR2_SSNM_Pos)
#define	SPIx_SPICR2_SSNM_LOW	(0x0U << SPIx_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master保持SSN为低 */
#define	SPIx_SPICR2_SSNM_HIGH	(0x1U << SPIx_SPICR2_SSNM_Pos)	/* 每发送完8bit后Master拉高SSN，维持高电平时间由WAIT寄存器控制 */

#define	SPIx_SPICR2_TXO_AC_Pos	4	/* TXONLY硬件自动清空的使能 */
#define	SPIx_SPICR2_TXO_AC_Msk	(0x1U << SPIx_SPICR2_TXO_AC_Pos)
#define	SPIx_SPICR2_TXO_AC_CLR_AT_CLOSE	(0x0U << SPIx_SPICR2_TXO_AC_Pos)	/* 关闭TXONLY硬件自动清零 */
#define	SPIx_SPICR2_TXO_AC_CLR_AT_TX	(0x1U << SPIx_SPICR2_TXO_AC_Pos)	/* TXONLY硬件自动清零有效，软件使能TXO后，等待发送完毕后，硬件清零 */

#define	SPIx_SPICR2_TXO_Pos	3	/* TXONLY控制位 */
#define	SPIx_SPICR2_TXO_Msk	(0x1U << SPIx_SPICR2_TXO_Pos)
	/* 0：关闭Master的单发送模式 */
	/* 1：启动Master的单发送模式 */

#define	SPIx_SPICR2_SSN_Pos	2	/* Master模式下，软件可以通过此位控制SSN输出电平 */
#define	SPIx_SPICR2_SSN_Msk	(0x1U << SPIx_SPICR2_SSN_Pos)
#define	SPIx_SPICR2_SSN_LOW	(0x0U << SPIx_SPICR2_SSN_Pos)	/* SSN输出低电平 */
#define	SPIx_SPICR2_SSN_HIGH	(0x1U << SPIx_SPICR2_SSN_Pos)	/* SSN输出高电平 */

#define	SPIx_SPICR2_SSNSEN_Pos	1	/* Master模式下，SSN的控制方式 */
#define	SPIx_SPICR2_SSNSEN_Msk	(0x1U << SPIx_SPICR2_SSNSEN_Pos)
#define	SPIx_SPICR2_SSNSEN_HARD	(0x0U << SPIx_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由硬件自动控制 */
#define	SPIx_SPICR2_SSNSEN_SOFT	(0x1U << SPIx_SPICR2_SSNSEN_Pos)	/* Master模式下SSN输出由软件控制 */

#define	SPIx_SPICR2_SPIEN_Pos	0	/* SPI使能 */
#define	SPIx_SPICR2_SPIEN_Msk	(0x1U << SPIx_SPICR2_SPIEN_Pos)
	/* 0：关闭SPI */
	/* 1：使能SPI */

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
	/* 0：HSPI错误中断不使能 */
	/* 1：HSPI错误中断使能 */

#define	SPIx_SPIIE_TXIE_Pos	1
#define	SPIx_SPIIE_TXIE_Msk	(0x1U << SPIx_SPIIE_TXIE_Pos)
	/* 0：发送完成中断不使能 */
	/* 1：发送完成中断使能 */

#define	SPIx_SPIIE_RXIE_Pos	0
#define	SPIx_SPIIE_RXIE_Msk	(0x1U << SPIx_SPIIE_RXIE_Pos)
	/* 0：接收完成中断不使能 */
	/* 1：接收完成中断使能 */

#define	SPIx_SPIIF_MERR_Pos	6	/* Master Error标志 */
#define	SPIx_SPIIF_MERR_Msk	(0x1U << SPIx_SPIIF_MERR_Pos)

#define	SPIx_SPIIF_SERR_Pos	5	/* Slave Error标志 */
#define	SPIx_SPIIF_SERR_Msk	(0x1U << SPIx_SPIIF_SERR_Pos)

#define	SPIx_SPIIF_RXCOL_Pos	4	/* 接收缓存溢出，软件写1清零 */
#define	SPIx_SPIIF_RXCOL_Msk	(0x1U << SPIx_SPIIF_RXCOL_Pos)

#define	SPIx_SPIIF_TXCOL_Pos	3	/* 发送缓存溢出，软件写1清零 */
#define	SPIx_SPIIF_TXCOL_Msk	(0x1U << SPIx_SPIIF_TXCOL_Pos)

#define	SPIx_SPIIF_BUSY_Pos	2	/* SPI空闲标志，只读 */
#define	SPIx_SPIIF_BUSY_Msk	(0x1U << SPIx_SPIIF_BUSY_Pos)

#define	SPIx_SPIIF_TXBE_Pos	1	/* TX Buffer Empty标志位 */
#define	SPIx_SPIIF_TXBE_Msk	(0x1U << SPIx_SPIIF_TXBE_Pos)

#define	SPIx_SPIIF_RXBF_Pos	0	/* RX Buffer Full标志位 */
#define	SPIx_SPIIF_RXBF_Msk	(0x1U << SPIx_SPIIF_RXBF_Pos)

#define	SPIx_SPITXBUF_TXBUF_Pos	0	/* SPI发送缓存 */
#define	SPIx_SPITXBUF_TXBUF_Msk	(0xffU << SPIx_SPITXBUF_TXBUF_Pos)

#define	SPIx_SPIRXBUF_RXBUF_Pos	0	/* SPI接收缓存 */
#define	SPIx_SPIRXBUF_RXBUF_Msk	(0xffU << SPIx_SPIRXBUF_RXBUF_Pos)
//Macro_End


extern void EX_Flash_Spi_MI_OUT_0(void);
extern void EX_Flash_Spi_MI_Fun_Mode(void);
extern void SPI_Init(INT8U SPI_ID,INT8U mode,INT8U *SPI_Cfg);			/*”SPI初始化函数”*/
extern INT8U SPI_ReadNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*”SPI读数据函数”*/
extern INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*”SPI写数据函数”*/
extern void SPI_Start (INT8U SPI_ID);
extern void SPI_End (INT8U SPI_ID);

#endif

