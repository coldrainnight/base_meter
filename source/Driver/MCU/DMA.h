

#ifndef _DMA_H_
#define _DMA_H_




	//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------

	//<h>DMA全局控制寄存器
	//<q.0> DMAEN, DMA使能
#define _GLOBALCTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道0控制寄存器
	//<q.0> CH0EN, 通道使能
	//<q.1> CH0HTIE, 半程传输完成中断使能
	//<q.2> CH0FTIE, 传输完成中断使能
	//<o.8..10> CH0SSEL, 外设通道选择
	//<0=> ET4_CAP
	//<1=> ET2_CAP
	//<2=> BT34_CAP
	//<3=> ET3_CAP
	//<4=> BT12_CAP
	//<5=> CRC_OUT
	//<6=> AES_IN
	//<7=> ADC_OUT
	//<o.11> CH0INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH0PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH0TSIZE, 传输长度,1-8192次传输
#define _CH0CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道1控制寄存器
	//<q.0> CH1EN, 通道使能
	//<q.1> CH1HTIE, 半程传输完成中断使能
	//<q.2> CH1FTIE, 传输完成中断使能
	//<o.8..10> CH1SSEL, 外设通道选择
	//<0=> ET1_CAP
	//<1=> AES_OUT
	//<2=> I2C_RX
	//<3=> U78160_RX
	//<4=> UART3_RX
	//<5=> UART0_RX
	//<6=> SPI0_RX
	//<7=> ADC_OUT
	//<o.11> CH1INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH1PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH1TSIZE, 传输长度,1-8192次传输
#define _CH1CTRL   ((INT32U)0x00103E00U)  /*!< */
	//</h>

	//<h>DMA通道2控制寄存器
	//<q.0> CH2EN, 通道使能
	//<q.1> CH2HTIE, 半程传输完成中断使能
	//<q.2> CH2FTIE, 传输完成中断使能
	//<o.8..10> CH2SSEL, 外设通道选择
	//<0=> ET3_CAP
	//<1=> BT12_CAP
	//<2=> AES_OUT
	//<3=> I2C_TX
	//<4=> U78160_TX
	//<5=> UART3_TX
	//<6=> UART0_TX
	//<7=> SPI0_TX
	//<o.11> CH2INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH2PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH2TSIZE, 传输长度,1-8192次传输
#define _CH2CTRL   ((INT32U)0x00082F00U)  /*!< */
	//</h>

	//<h>DMA通道3控制寄存器
	//<q.0> CH3EN, 通道使能
	//<q.1> CH3HTIE, 半程传输完成中断使能
	//<q.2> CH3FTIE, 传输完成中断使能
	//<o.8..10> CH3SSEL, 外设通道选择
	//<0=> BT34_CAP
	//<1=> ET1_CAP
	//<2=> I2C_TX
	//<3=> U78161_RX
	//<4=> UART4_RX
	//<5=> UART1_RX
	//<6=> UART0_RX
	//<7=> SPI1_RX
	//<o.11> CH3INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH3PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH3TSIZE, 传输长度,1-8192次传输
#define _CH3CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道4控制寄存器
	//<q.0> CH4EN, 通道使能
	//<q.1> CH4HTIE, 半程传输完成中断使能
	//<q.2> CH4FTIE, 传输完成中断使能
	//<o.8..10> CH4SSEL, 外设通道选择
	//<0=> ET2_CAP
	//<1=> AES_IN
	//<2=> I2C_RX
	//<3=> U78161_TX
	//<4=> UART4_TX
	//<5=> UART1_TX
	//<6=> UART0_TX
	//<7=> SPI1_TX
	//<o.11> CH4INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH4PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH4TSIZE, 传输长度,1-8192次传输
#define _CH4CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道5控制寄存器
	//<q.0> CH5EN, 通道使能
	//<q.1> CH5HTIE, 半程传输完成中断使能
	//<q.2> CH5FTIE, 传输完成中断使能
	//<o.8..10> CH5SSEL, 外设通道选择
	//<0=> LPTIM_CAP
	//<1=> U78161_RX
	//<2=> U78160_RX
	//<3=> UART5_RX
	//<4=> UART2_RX
	//<5=> UART1_RX
	//<6=> SPI2_RX
	//<7=> SPI0_RX
	//<o.11> CH5INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH5PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH5TSIZE, 传输长度,1-8192次传输
#define _CH5CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道6控制寄存器
	//<q.0> CH6EN, 通道使能
	//<q.1> CH6HTIE, 半程传输完成中断使能
	//<q.2> CH6FTIE, 传输完成中断使能
	//<o.8..10> CH6SSEL, 外设通道选择
	//<0=> LPTIM_CAP
	//<1=> U78161_TX
	//<2=> U78160_TX
	//<3=> UART5_TX
	//<4=> UART2_TX
	//<5=> UART1_TX
	//<6=> SPI2_TX
	//<7=> SPI0_TX
	//<o.11> CH6INC, RAM地址增减设置
	//<0=> RAM地址递减
	//<1=> RAM地址递增
	//<o.12..13> CH6PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH6TSIZE, 传输长度,1-8192次传输
#define _CH6CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//<h>DMA通道7控制寄存器
	//<q.0> CH7EN, 通道使能
	//<q.1> CH7HTIE, 半程传输完成中断使能
	//<q.2> CH7FTIE, 传输完成中断使能
	//<o.8> CH7FI, Flash地址增减设置
	//<0=> FLASH地址递减
	//<1=> FLASH地址递增
	//<o.9> CH6RI, RAM地址增减设置
	//<0=> FLASH地址递减
	//<1=> FLASH地址递增
	//<o.10> CH7DIR, 通道传输方向
	//<0=> RAM->FLASH传输
	//<1=> FLASH->RAM传输
	//<o.12..13> CH7PRI, 通道优先级
	//<0=> Low
	//<1=> Medium
	//<2=> High
	//<3=> Very High
	//<o.16..28> CH7TSIZE, 传输长度,1-8192次传输,仅针对FLASH->RAM有效,RAM->FLASH固定为64次传输
#define _CH7CTRL   ((INT32U)0x00000000U)  /*!< */
	//</h>

	//*** <<< end of configuration section >>>    ***


	/* Exported constants --------------------------------------------------------*/

	/** @defgroup RCC_Exported_Constants
	    @{
	*/

	typedef enum {DmaChannel_0 = 0, DmaChannel_1 = 1, DmaChannel_2 = 2, DmaChannel_3 = 3,
	              DmaChannel_4 = 4, DmaChannel_5 = 5, DmaChannel_6 = 6, DmaChannel_7 = 7
	             } DmaChannel;

	/* Exported types ------------------------------------------------------------*/
	typedef struct
	{
		DmaChannel CHANNEL_num; 				/*!<  set dma channel 0-7 */
		INT32U Trans_Sizes;   				/*!<  trans size 1-8192   */
		INT32U Ram_Addr;  						/*!<  ram start address */
		INT32U Flash_Addr;  					/*!<  flash start address only for channel7 */
		INT32U CHPriority;  	    		/*!<  CHANNEL Priority */
		INT32U CHselect;   						/*!<  periph select only for channel0-6     */
		INT32U ram_addrincrease;   		/*!< ram address increase or decrease  */
		INT32U flash_addrincrease;   	/*!< flash address increase or decrease only for channel7  */
	} DMA_InitType;


#define DMA_CH0CTRL_CH0HTIE         ((INT32U)0x00000002)//(BIT1)
#define DMA_CH0CTRL_CH0FTIE         ((INT32U)0x00000004)//(BIT2)

#define DMA_CH1CTRL_CH1HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH1CTRL_CH1FTIE         ((INT32U)0x00000004)//(BIT2)

#define DMA_CH2CTRL_CH2HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH2CTRL_CH2FTIE         ((INT32U)0x00000004)//  (BIT2)

#define DMA_CH3CTRL_CH3HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH3CTRL_CH3FTIE         ((INT32U)0x00000004)//  (BIT2)

#define DMA_CH4CTRL_CH4HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH4CTRL_CH4FTIE         ((INT32U)0x00000004)//  (BIT2)

#define DMA_CH5CTRL_CH5HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH5CTRL_CH5FTIE         ((INT32U)0x00000004)//  (BIT2)

#define DMA_CH6CTRL_CH6HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH6CTRL_CH6FTIE         ((INT32U)0x00000004)//  (BIT2)

#define DMA_CH7CTRL_CH7HTIE         ((INT32U)0x00000002)//  (BIT1)
#define DMA_CH7CTRL_CH7FTIE         ((INT32U)0x00000004)//  (BIT2)




#define DMA_CHSTATUS_DMACHFT0         ((INT32U)0x00000100)// (BIT8)
#define DMA_CHSTATUS_DMACHFT1         ((INT32U)0x00000200)//  (BIT9)
#define DMA_CHSTATUS_DMACHFT2         ((INT32U)0x00000400)//  (BIT10)
#define DMA_CHSTATUS_DMACHFT3         ((INT32U)0x00000800)//  (BIT11)
#define DMA_CHSTATUS_DMACHFT4         ((INT32U)0x00001000)//  (BIT12)
#define DMA_CHSTATUS_DMACHFT5         ((INT32U)0x00002000)// (BIT13)
#define DMA_CHSTATUS_DMACHFT6         ((INT32U)0x00004000)// (BIT14)
#define DMA_CHSTATUS_DMACHFT7         ((INT32U)0x00008000)// (BIT15)

#define DMA_CHSTATUS_DMACHHT0          ((INT32U)0x00000001)//(BIT0)
#define DMA_CHSTATUS_DMACHHT1          ((INT32U)0x00000002)// (BIT1)
#define DMA_CHSTATUS_DMACHHT2          ((INT32U)0x00000004)// (BIT2)
#define DMA_CHSTATUS_DMACHHT3          ((INT32U)0x00000008)// (BIT3)
#define DMA_CHSTATUS_DMACHHT4          ((INT32U)0x00000010)// (BIT4)
#define DMA_CHSTATUS_DMACHHT5          ((INT32U)0x00000020)// (BIT5)
#define DMA_CHSTATUS_DMACHHT6          ((INT32U)0x00000040)// (BIT6)
#define DMA_CHSTATUS_DMACHHT7          ((INT32U)0x00000080)// (BIT7)


#define DMA_CH1_SPI0_Rx (6)
#define DMA_CH2_SPI0_Tx (7)





	/**
	    @}
	*/

	/** @defgroup DMA_GLOBALCTRL_DMAEN
	    @{
	*/
#define DMA_GLOBALCTRL_DMAEN_Pos               							0
#define DMA_GLOBALCTRL_DMAEN_Msk               							(0x01UL << DMA_GLOBALCTRL_DMAEN_Pos)
#define DMA_GLOBALCTRL_DMAEN_DIABLE        									((INT32U)0x00000000)
#define DMA_GLOBALCTRL_DMAEN_ENABLE        									((INT32U)0x00000001)

	/**
	    @}
	*/

	/** @defgroup DMA_CHxCTRL_CHxTSIZE
	    @{
	*/
#define DMA_CHxCTRL_CHxTSIZE_Pos               						  16
#define DMA_CHxCTRL_CHxTSIZE_Msk               						 	(0x1FFFUL << DMA_CHxCTRL_CHxTSIZE_Pos)

	/** @defgroup DMA_CHxCTRL_CHxPRI
	    @{
	*/
#define DMA_CHxCTRL_CHxPRI_Pos               								12
#define DMA_CHxCTRL_CHxPRI_Msk               								(0x03UL << DMA_CHxCTRL_CHxPRI_Pos)
#define DMA_CHxCTRL_CHxPRI_LOW		                					((INT32U)0x00000000)
#define DMA_CHxCTRL_CHxPRI_Medium                						((INT32U)0x00001000)
#define DMA_CHxCTRL_CHxPRI_High               							((INT32U)0x00002000)
#define DMA_CHxCTRL_CHxPRI_VeryHigh               					((INT32U)0x00003000)


	/** @defgroup DMA_CHxCTRL_CHxINC
	    @{
	*/
#define DMA_CHxCTRL_CHxINC_Pos               								11
#define DMA_CHxCTRL_CHxINC_Msk               								(0x01UL << DMA_CHxCTRL_CHxINC_Pos)
#define DMA_CHxCTRL_CHxINC_Dec                							((INT32U)0x00000000)
#define DMA_CHxCTRL_CHxINC_Inc                							((INT32U)0x00000800)

	/** @defgroup DMA_CHxCTRL_CHxFTIE
	    @{
	*/
#define DMA_CHxCTRL_CHxFTIE_Pos               							2
#define DMA_CHxCTRL_CHxFTIE_Msk               							(0x01UL << DMA_CHxCTRL_CHxFTIE_Pos)
#define DMA_CHxCTRL_CHxFTIE_Disable                					((INT32U)0x00000000)
#define DMA_CHxCTRL_CHxFTIE_Enable	                				((INT32U)0x00000004)

	/** @defgroup DMA_CHxCTRL_CHxHTIE
	    @{
	*/
#define DMA_CHxCTRL_CHxHTIE_Pos               							1
#define DMA_CHxCTRL_CHxHTIE_Msk               							(0x01UL << DMA_CHxCTRL_CHxHTIE_Pos)
#define DMA_CHxCTRL_CHxHTIE_Disable                					((INT32U)0x00000000)
#define DMA_CHxCTRL_CHxHTIE_Enable                					((INT32U)0x00000002)

	/** @defgroup DMA_CHxCTRL_CHxEN
	    @{
	*/
#define DMA_CHxCTRL_CHxEN_Pos               								0
#define DMA_CHxCTRL_CHxEN_Msk               								(0x01UL << DMA_CHxCTRL_CHxEN_Pos)
#define DMA_CHxCTRL_CHxEN_Disable                						((INT32U)0x00000000)
#define DMA_CHxCTRL_CHxEN_Enable                						((INT32U)0x00000001)

	/**
	    @}
	*/

	/** @defgroup DMA_CH0CTRL_CH0SSEL
	    @{
	*/
#define DMA_CHxCTRL_CHxSSEL_Pos               							8
#define DMA_CHxCTRL_CHxSSEL_Msk              								(0x07UL << DMA_CHxCTRL_CHxSSEL_Pos)

#define DMA_CH0CTRL_CH0SSEL_ET4_CAP 												((INT32U)0x00000000)
#define DMA_CH0CTRL_CH0SSEL_ET2_CAP 												((INT32U)0x00000100)
#define DMA_CH0CTRL_CH0SSEL_BT34_CAP 												((INT32U)0x00000200)
#define DMA_CH0CTRL_CH0SSEL_ET3_CAP 												((INT32U)0x00000300)
#define DMA_CH0CTRL_CH0SSEL_BT12_CAP 												((INT32U)0x00000400)
#define DMA_CH0CTRL_CH0SSEL_CRC_OUT 												((INT32U)0x00000500)
#define DMA_CH0CTRL_CH0SSEL_AES_IN 													((INT32U)0x00000600)
#define DMA_CH0CTRL_CH0SSEL_ADC_OUT 												((INT32U)0x00000700)

#define DMA_CH1CTRL_CH1SSEL_ET1_CAP 												((INT32U)0x00000000)
#define DMA_CH1CTRL_CH1SSEL_AES_OUT 												((INT32U)0x00000100)
#define DMA_CH1CTRL_CH1SSEL_I2C_RX 													((INT32U)0x00000200)
#define DMA_CH1CTRL_CH1SSEL_USART0_RX 											((INT32U)0x00000300)
#define DMA_CH1CTRL_CH1SSEL_UART3_RX 												((INT32U)0x00000400)
#define DMA_CH1CTRL_CH1SSEL_UART0_RX 												((INT32U)0x00000500)
#define DMA_CH1CTRL_CH1SSEL_SPI0_RX 												((INT32U)0x00000600)
#define DMA_CH1CTRL_CH1SSEL_ADC_OUT 												((INT32U)0x00000700)

#define DMA_CH2CTRL_CH2SSEL_ET3_CAP 												((INT32U)0x00000000)
#define DMA_CH2CTRL_CH2SSEL_BT12_CAP 												((INT32U)0x00000100)
#define DMA_CH2CTRL_CH2SSEL_AES_OUT 												((INT32U)0x00000200)
#define DMA_CH2CTRL_CH2SSEL_I2C_TX 		  										((INT32U)0x00000300)
#define DMA_CH2CTRL_CH2SSEL_USART0_TX 											((INT32U)0x00000400)
#define DMA_CH2CTRL_CH2SSEL_UART3_TX 												((INT32U)0x00000500)
#define DMA_CH2CTRL_CH2SSEL_UART0_TX 												((INT32U)0x00000600)
#define DMA_CH2CTRL_CH2SSEL_SPI0_TX 												((INT32U)0x00000700)

#define DMA_CH3CTRL_CH3SSEL_BT34_CAP 												((INT32U)0x00000000)
#define DMA_CH3CTRL_CH3SSEL_ET1_CAP 												((INT32U)0x00000100)
#define DMA_CH3CTRL_CH3SSEL_I2C_TX  												((INT32U)0x00000200)
#define DMA_CH3CTRL_CH3SSEL_USART1_RX												((INT32U)0x00000300)
#define DMA_CH3CTRL_CH3SSEL_UART4_RX 												((INT32U)0x00000400)
#define DMA_CH3CTRL_CH3SSEL_UART1_RX 												((INT32U)0x00000500)
#define DMA_CH3CTRL_CH3SSEL_UART0_RX 												((INT32U)0x00000600)
#define DMA_CH3CTRL_CH3SSEL_SPI1_RX 												((INT32U)0x00000700)

#define DMA_CH4CTRL_CH4SSEL_ET2_CAP 												((INT32U)0x00000000)
#define DMA_CH4CTRL_CH4SSEL_AES_IN  												((INT32U)0x00000100)
#define DMA_CH4CTRL_CH4SSEL_I2C_RX  												((INT32U)0x00000200)
#define DMA_CH4CTRL_CH4SSEL_USART1_TX 											((INT32U)0x00000300)
#define DMA_CH4CTRL_CH4SSEL_UART4_TX 												((INT32U)0x00000400)
#define DMA_CH4CTRL_CH4SSEL_UART1_TX 												((INT32U)0x00000500)
#define DMA_CH4CTRL_CH4SSEL_UART0_TX 												((INT32U)0x00000600)
#define DMA_CH4CTRL_CH4SSEL_SPI1_TX 												((INT32U)0x00000700)

#define DMA_CH5CTRL_CH5SSEL_LPTIM_CAP												((INT32U)0x00000000)
#define DMA_CH5CTRL_CH5SSEL_USART1_RX 											((INT32U)0x00000100)
#define DMA_CH5CTRL_CH5SSEL_USART0_RX 											((INT32U)0x00000200)
#define DMA_CH5CTRL_CH5SSEL_UART5_RX 												((INT32U)0x00000300)
#define DMA_CH5CTRL_CH5SSEL_UART2_RX 												((INT32U)0x00000400)
#define DMA_CH5CTRL_CH5SSEL_UART1_RX 												((INT32U)0x00000500)
#define DMA_CH5CTRL_CH5SSEL_SPI2_RX													((INT32U)0x00000600)
#define DMA_CH5CTRL_CH5SSEL_SPI0_RX 												((INT32U)0x00000700)

#define DMA_CH6CTRL_CH6SSEL_LPTIM_CAP												((INT32U)0x00000000)
#define DMA_CH6CTRL_CH6SSEL_USART1_TX												((INT32U)0x00000100)
#define DMA_CH6CTRL_CH6SSEL_USART0_TX 											((INT32U)0x00000200)
#define DMA_CH6CTRL_CH6SSEL_UART5_TX 												((INT32U)0x00000300)
#define DMA_CH6CTRL_CH6SSEL_UART2_TX 												((INT32U)0x00000400)
#define DMA_CH6CTRL_CH6SSEL_UART1_TX 												((INT32U)0x00000500)
#define DMA_CH6CTRL_CH6SSEL_SPI2_TX 												((INT32U)0x00000600)
#define DMA_CH6CTRL_CH6SSEL_SPI0_TX 												((INT32U)0x00000700)


	/**
	    @}
	*/

	/** @defgroup DMA_CH7CTRL_CH7TSIZE
	    @{
	*/
#define DMA_CH7CTRL_CH7TSIZE_Pos               						  16
#define DMA_CH7CTRL_CH7TSIZE_Msk               						 	(0x1FFFUL << DMA_CH7CTRL_CH7TSIZE_Pos)

	/** @defgroup DMA_CH7CTRL_CH7PRI
	    @{
	*/
#define DMA_CH7CTRL_CH7PRI_Pos               								12
#define DMA_CH7CTRL_CH7PRI_Msk               								(0x03UL << DMA_CH7CTRL_CH7PRI_Pos)
#define DMA_CH7CTRL_CH7PRI_LOW		                					((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7PRI_Medium                						((INT32U)0x00001000)
#define DMA_CH7CTRL_CH7PRI_High               							((INT32U)0x00002000)
#define DMA_CH7CTRL_CH7PRI_VeryHigh               					((INT32U)0x00003000)


	/** @defgroup DMA_CH7CTRL_CH7DIR
	    @{
	*/
#define DMA_CH7CTRL_CH7DIR_Pos                							10
#define DMA_CH7CTRL_CH7DIR_Msk               								(0x01UL << DMA_CH7CTRL_CH7DIR_Pos)
#define DMA_CH7CTRL_CH7DIR_Ram2Flash           							((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7DIR_Flash2Ram           							((INT32U)0x00000400)

	/** @defgroup DMA_CH7CTRL_CH7RI
	    @{
	*/
#define DMA_CH7CTRL_CH7RI_Pos               								9
#define DMA_CH7CTRL_CH7RI_Msk               								(0x01UL << DMA_CH7CTRL_CH7RI_Pos)
#define DMA_CH7CTRL_CH7RI_Dec                							  ((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7RI_Inc                							  ((INT32U)0x00000200)

	/** @defgroup DMA_CH7CTRL_CH7FI
	    @{
	*/
#define DMA_CH7CTRL_CH7FI_Pos               								8
#define DMA_CH7CTRL_CH7FI_Msk               								(0x01UL << DMA_CH7CTRL_CH7FI_Pos)
#define DMA_CH7CTRL_CH7FI_Dec                							  ((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7FI_Inc                							  ((INT32U)0x00000100)

	/** @defgroup DMA_CH7CTRL_CH7FTIE
	    @{
	*/
#define DMA_CH7CTRL_CH7FTIE_Pos               							2
#define DMA_CH7CTRL_CH7FTIE_Msk               							(0x01UL << DMA_CH7CTRL_CH7FTIE_Pos)
#define DMA_CH7CTRL_CH7FTIE_Disable                					((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7FTIE_Enable	                				((INT32U)0x00000004)

	/** @defgroup DMA_CH7CTRL_CH7HTIE
	    @{
	*/
#define DMA_CH7CTRL_CH7HTIE_Pos               							1
#define DMA_CH7CTRL_CH7HTIE_Msk               							(0x01UL << DMA_CH7CTRL_CH7HTIE_Pos)
#define DMA_CH7CTRL_CH7HTIE_Disable                					((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7HTIE_Enable                					((INT32U)0x00000002)

	/** @defgroup DMA_CH7CTRL_CH7EN
	    @{
	*/
#define DMA_CH7CTRL_CH7EN_Pos               								0
#define DMA_CH7CTRL_CH7EN_Msk               								(0x01UL << DMA_CH7CTRL_CH7EN_Pos)
#define DMA_CH7CTRL_CH7EN_Disable                						((INT32U)0x00000000)
#define DMA_CH7CTRL_CH7EN_Enable                						((INT32U)0x00000001)

	/**
	    @}
	*/

	/** @defgroup DMA_CH7FLSAD_CH7FLSAD
	    @{
	*/
#define DMA_CH7FLSAD_CH7FLSAD_Pos               						  0
#define DMA_CH7FLSAD_CH7FLSAD_Msk               						 	(0x1FFFFUL << DMA_CH7FLSAD_CH7FLSAD_Pos)

	/**
	    @}
	*/

	/** @defgroup DMA_CH7RAMAD_CH7RAMAD
	    @{
	*/
#define DMA_CH7RAMAD_CH7RAMAD_Pos               						  0
#define DMA_CH7RAMAD_CH7RAMAD_Msk               						 	(0x3FFFUL << DMA_CH7RAMAD_CH7RAMAD_Pos)


	/** @defgroup DMA_CHSTATUS_CH0_HT
	    @{
	*/
#define DMA_CHSTATUS_CH0_HT_Pos                                          0
#define DMA_CHSTATUS_CH0_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH0_HT_Pos)
#define DMA_CHSTATUS_CH0_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH0_HT_FINISHED                                     ((INT32U)0x00000001)


	/** @defgroup DMA_CHSTATUS_CH1_HT
	    @{
	*/
#define DMA_CHSTATUS_CH1_HT_Pos                                          1
#define DMA_CHSTATUS_CH1_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH1_HT_Pos)
#define DMA_CHSTATUS_CH1_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH1_HT_FINISHED                                     ((INT32U)0x00000002)



	/** @defgroup DMA_CHSTATUS_CH2_HT
	    @{
	*/
#define DMA_CHSTATUS_CH2_HT_Pos                                          2
#define DMA_CHSTATUS_CH2_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH2_HT_Pos)
#define DMA_CHSTATUS_CH2_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH2_HT_FINISHED                                     ((INT32U)0x00000004)




	/** @defgroup DMA_CHSTATUS_CH3_HT
	    @{
	*/
#define DMA_CHSTATUS_CH3_HT_Pos                                          3
#define DMA_CHSTATUS_CH3_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH3_HT_Pos)
#define DMA_CHSTATUS_CH3_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH3_HT_FINISHED                                     ((INT32U)0x00000008)



	/** @defgroup DMA_CHSTATUS_CH4_HT
	    @{
	*/
#define DMA_CHSTATUS_CH4_HT_Pos                                          4
#define DMA_CHSTATUS_CH4_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH4_HT_Pos)
#define DMA_CHSTATUS_CH4_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH4_HT_FINISHED                                     ((INT32U)0x00000010)



	/** @defgroup DMA_CHSTATUS_CH5_HT
	    @{
	*/
#define DMA_CHSTATUS_CH5_HT_Pos                                          5
#define DMA_CHSTATUS_CH5_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH5_HT_Pos)
#define DMA_CHSTATUS_CH5_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH5_HT_FINISHED                                     ((INT32U)0x00000020)


	/** @defgroup DMA_CHSTATUS_CH6_HT
	    @{
	*/
#define DMA_CHSTATUS_CH6_HT_Pos                                          6
#define DMA_CHSTATUS_CH6_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH6_HT_Pos)
#define DMA_CHSTATUS_CH6_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH6_HT_FINISHED                                     ((INT32U)0x00000040)



	/** @defgroup DMA_CHSTATUS_CH7_HT
	    @{
	*/
#define DMA_CHSTATUS_CH7_HT_Pos                                          7
#define DMA_CHSTATUS_CH7_HT_Msk                                          (0x01UL << DMA_CHSTATUS_CH7_HT_Pos)
#define DMA_CHSTATUS_CH7_HT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH7_HT_FINISHED                                     ((INT32U)0x00000080)



	/** @defgroup DMA_CHSTATUS_CH0_FT
	    @{
	*/
#define DMA_CHSTATUS_CH0_FT_Pos                                          8
#define DMA_CHSTATUS_CH0_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH0_FT_Pos)
#define DMA_CHSTATUS_CH0_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH0_FT_FINISHED                                     ((INT32U)0x00000100)


	/** @defgroup DMA_CHSTATUS_CH1_FT
	    @{
	*/
#define DMA_CHSTATUS_CH1_FT_Pos                                          9
#define DMA_CHSTATUS_CH1_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH1_FT_Pos)
#define DMA_CHSTATUS_CH1_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH1_FT_FINISHED                                     ((INT32U)0x00000200)



	/** @defgroup DMA_CHSTATUS_CH2_FT
	    @{
	*/
#define DMA_CHSTATUS_CH2_FT_Pos                                          10
#define DMA_CHSTATUS_CH2_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH2_FT_Pos)
#define DMA_CHSTATUS_CH2_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH2_FT_FINISHED                                     ((INT32U)0x00000400)




	/** @defgroup DMA_CHSTATUS_CH3_FT
	    @{
	*/
#define DMA_CHSTATUS_CH3_FT_Pos                                          11
#define DMA_CHSTATUS_CH3_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH3_FT_Pos)
#define DMA_CHSTATUS_CH3_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH3_FT_FINISHED                                     ((INT32U)0x00000800)



	/** @defgroup DMA_CHSTATUS_CH4_FT
	    @{
	*/
#define DMA_CHSTATUS_CH4_FT_Pos                                          12
#define DMA_CHSTATUS_CH4_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH4_FT_Pos)
#define DMA_CHSTATUS_CH4_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH4_FT_FINISHED                                     ((INT32U)0x00001000)



	/** @defgroup DMA_CHSTATUS_CH5_FT
	    @{
	*/
#define DMA_CHSTATUS_CH5_FT_Pos                                          13
#define DMA_CHSTATUS_CH5_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH5_FT_Pos)
#define DMA_CHSTATUS_CH5_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH5_FT_FINISHED                                     ((INT32U)0x00002000)


	/** @defgroup DMA_CHSTATUS_CH6_FT
	    @{
	*/
#define DMA_CHSTATUS_CH6_FT_Pos                                          14
#define DMA_CHSTATUS_CH6_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH6_FT_Pos)
#define DMA_CHSTATUS_CH6_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH6_FT_FINISHED                                     ((INT32U)0x00004000)



	/** @defgroup DMA_CHSTATUS_CH7_FT
	    @{
	*/
#define DMA_CHSTATUS_CH7_FT_Pos                                          15
#define DMA_CHSTATUS_CH7_FT_Msk                                          (0x01UL << DMA_CHSTATUS_CH7_FT_Pos)
#define DMA_CHSTATUS_CH7_FT_UNFINISHED                                   ((INT32U)0x00000000)
#define DMA_CHSTATUS_CH7_FT_FINISHED                                     ((INT32U)0x00008000)




	/**
	    @}
	*/

	/** @defgroup DMA_CHSTATUS_DMACHFT
	    @{
	*/
#define DMA_CHSTATUS_DMACHFT_7               						  ((INT32U)0x00008000)
#define DMA_CHSTATUS_DMACHFT_6               						  ((INT32U)0x00004000)
#define DMA_CHSTATUS_DMACHFT_5               						  ((INT32U)0x00002000)
#define DMA_CHSTATUS_DMACHFT_4               						  ((INT32U)0x00001000)
#define DMA_CHSTATUS_DMACHFT_3               						  ((INT32U)0x00000800)
#define DMA_CHSTATUS_DMACHFT_2               						  ((INT32U)0x00000400)
#define DMA_CHSTATUS_DMACHFT_1               						  ((INT32U)0x00000200)
#define DMA_CHSTATUS_DMACHFT_0               						  ((INT32U)0x00000100)

	/** @defgroup DMA_CHSTATUS_DMACHHT
	    @{
	*/
#define DMA_CHSTATUS_DMACHHT_7               						  ((INT32U)0x00000080)
#define DMA_CHSTATUS_DMACHHT_6               						  ((INT32U)0x00000040)
#define DMA_CHSTATUS_DMACHHT_5               						  ((INT32U)0x00000020)
#define DMA_CHSTATUS_DMACHHT_4               						  ((INT32U)0x00000010)
#define DMA_CHSTATUS_DMACHHT_3               						  ((INT32U)0x00000008)
#define DMA_CHSTATUS_DMACHHT_2               						  ((INT32U)0x00000004)
#define DMA_CHSTATUS_DMACHHT_1               						  ((INT32U)0x00000002)
#define DMA_CHSTATUS_DMACHHT_0               						  ((INT32U)0x00000001)


#define SPI_IDLE  0
#define SPI_SND   1
#define SPI_RCV   2

#define CALL_TPYE_LOOP 0
#define CALL_TPYE_INT 1

extern volatile INT8U SPIDMA_STATUS;


extern void MU_SPI_Init(void);
extern void DMA_Globle_Disable(void);
extern BOOL MU_TxFrame(INT8U CalTyp,INT8U* SndBuf, INT16U Sndlen,INT8U* RcvBuf, INT16U Rcvlen);
extern void MU_RxFrame(INT8U* RcvBuf, INT16U Rcvlen);
extern void MU_SPI_RcvInt(void);
extern void DMA_INT_init(void);

extern void MU_SPI_Test(void);

#endif



















