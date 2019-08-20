#include "Driver\MCU\SPI.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\DMA.h"
#include "spi_slave.h"

/*"SPI-1: PC6 - PC9"*/

#define DMA_RX_CH 3
#define DMA_TX_CH 4

#define WAIT_RX_PKT    0x00  /*" 等待接收Master报文 "*/
#define RX_PKT_HDR     0x01  /*" 接收报文头 "*/
#define RX_PKT_BODY    0x02  /*" 接收报文体 "*/

#define WAIT_ANS_DATA_OK 0X09  /*" 接收到报文，等待应答数据准备好  "*/
#define ANS_DATA_OK      0X10  /*" 应答数据准备好  "*/

#define ANS_NO_OK_FLG    0x3D
#define ANS_OK_FLG       0xCB
#define ANS_END_FLG      0xFB

INT8U dma_rx_buf[SLAVE_COM_BUF_SZ];
INT8U dma_tx_buf[SLAVE_COM_BUF_SZ];

volatile INT8U rx_len;
volatile INT8U tx_off;


volatile INT8U slave_status;
volatile INT8U bak_slave_status;


INT16U master_pkt_body_len; /*" 报文体长度 "*/

INT16U dma_tx_tick;

INT16U rx_len_err;

INT16U tx_over_tms;

INT16U dma_rx_tick;

INT16U proc_tick;
INT8U lst_rx_data;

INT8U tx_idx;
INT8U tx_off_buf[200];

INT16U dma_tx_err;
INT8U pad_d;


void start_dma_rx(INT8U ch, INT16U len)
{
    DMA->CH3CTRL &= ~(1 << 0);   // DMA Channel3关闭

    DMA->CH3CTRL &= ~DMA_CHxCTRL_CHxTSIZE_Msk;
    DMA->CH3CTRL |= ((len - 1) << 16); // 待传输数据长度

    DMA->CH3CTRL &= ~DMA_CHxCTRL_CHxPRI_Msk;
    DMA->CH3CTRL |= (3 << 12); // 通道优先级:Very High

    DMA->CH3CTRL &= ~DMA_CHxCTRL_CHxINC_Msk;
    DMA->CH3CTRL |= (1 << 11);  // RAM地址递增

    DMA->CH3CTRL &= ~DMA_CHxCTRL_CHxSSEL_Msk;
    DMA->CH3CTRL |= (7 << 8);   // DMA Channel3选择SPI1_RX

    DMA->CH3CTRL |= (1 << 2);   // DMA Channel3完成中断使能
    DMA->CH3RAMAD = (INT32U)dma_rx_buf;  // DMA Channel3 RAM地址指针
    DMA->CH3CTRL |= (0x01 << 0);  // 使能传输    
}

void start_dma_tx(INT8U ch, INT8U * buf, INT16U len)
{
    DMA->CH4CTRL &= ~(1 << 0);   // DMA Channel4关闭

    DMA->CH4CTRL &= ~DMA_CHxCTRL_CHxTSIZE_Msk;
    DMA->CH4CTRL |= ((len - 1) << 16); // 待传输数据长度

    DMA->CH4CTRL &= ~DMA_CHxCTRL_CHxPRI_Msk;
    DMA->CH4CTRL |= (2 << 12); // 通道优先级:High

    DMA->CH4CTRL &= ~DMA_CHxCTRL_CHxINC_Msk;
    DMA->CH4CTRL |= (1 << 11);  // RAM地址递增

    DMA->CH4CTRL &= ~DMA_CHxCTRL_CHxSSEL_Msk;
    DMA->CH4CTRL |= (7 << 8);   // DMA Channel4选择SPI1_TX

    DMA->CH4CTRL |= (1 << 2);   // DMA Channel4完成中断使能
    DMA->CH4RAMAD = (INT32U)buf;  // DMA Channel4 RAM地址指针
    DMA->CH4CTRL |= (0x01 << 0);  // 使能传输    
}

INT8U cfg_ans_data_no_ok(void)
{
    INT8U ans[6] = {ANS_NO_OK_FLG, ANS_NO_OK_FLG, ANS_END_FLG, ANS_END_FLG, ANS_END_FLG, ANS_END_FLG};
    LIB_MemCpy(ans, dma_tx_buf, 5);
    master_pkt_body_len = 5;
    return 5;
}

/*" CS片选中断 "*/
void cs_int_isr(void)
{
    if(slave_status == WAIT_RX_PKT)
    {
        /*" 配置DMA接收4字节报文头 "*/
        //start_dma_rx(DMA_RX_CH, 4);
        slave_status = RX_PKT_BODY;
    }
    else if(slave_status == RX_PKT_BODY)
    {
        /*" 配置DMA接收报文体 "*/
        //start_dma_rx(DMA_RX_CH, master_pkt_body_len);
        cfg_ans_data_no_ok();
        slave_status = WAIT_ANS_DATA_OK;
    }
    #if 0
    else if(slave_status == WAIT_ANS_DATA_OK)
    {
        /*" 配置DMA发送数据未准备好 "*/
        cfg_dma_tx(DMA_TX_CH, dma_tx_buf, 6);
    }
    else if(slave_status == ANS_DATA_OK)
    {
        /*" 配置DMA发送应答数据 "*/
        cfg_dma_tx(DMA_TX_CH, dma_tx_buf, master_pkt_body_len);
    }
    #endif
}

/*" DMA接收完成中断 "*/
void dma_rx_end_isr(void)
{
    if(slave_status == WAIT_RX_PKT)
    {
        /*" 接收完报文头"*/
        //master_pkt_body_len = dma_rx_buf[1] | (dma_rx_buf[2] << 8);
        //slave_status = RX_PKT_BODY;

		/*" 接收完报文体"*/
        cfg_ans_data_no_ok();
        slave_status = WAIT_ANS_DATA_OK;
        /*" 打开接收中断，用于响应查询命令 "*/
        //pad_d = SPI1->SPIRXF;
        SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
        INT_En(E_INT_ID_SPI1);
        dma_rx_tick = MCU_GetTick();
    }
    else if(slave_status == RX_PKT_BODY)
    {
        /*" 接收完报文体"*/
        cfg_ans_data_no_ok();
        slave_status = WAIT_ANS_DATA_OK;
        /*" 打开接收中断，用于响应查询命令 "*/
        //pad_d = SPI1->SPIRXF;
        SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
        INT_En(E_INT_ID_SPI1);
    }
	else if(slave_status == ANS_DATA_OK)
	{
        dma_rx_tick = MCU_GetTick();
	}
}

INT16U start_tick;
INT16U commu_timeout;

void spi_run_check(void)
{
	if((slave_status != WAIT_RX_PKT) && (start_tick != 0) && (MCU_CmpTick(start_tick, 1000) == TRUE))
	{
		/*tx_off_buf[tx_idx] = slave_status;
		tx_idx = (tx_idx + 1)%100;

		tx_off_buf[tx_idx] = rx_len;
		tx_idx = (tx_idx + 1)%100;
		
		LIB_MemCpy(dma_rx_buf, &tx_off_buf[tx_idx], rx_len);

		tx_idx = 0;
		
		commu_timeout++; */
		pad_d = SPI1->SPIRXF;
		SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
		slave_status = WAIT_RX_PKT; /*" 结束本次交互，等待下次交互 "*/
	}
}

/**
 * @brief   SPI1中断处理函数。
 * @param   无。
 * @return  无。
 */ 
void SPI1_Int_Isr(void)
{
    INT8U data;
    // 接收完成中断
	if ((SPI1->SPIIE & (1 << 0)) && (SPI1->SPISTA & (1 << 0)))
	{
        data = SPI1->SPIRXF;
        lst_rx_data = data;
        if(slave_status == WAIT_RX_PKT)
        {
#if 0
			slave_status = RX_PKT_BODY;
			master_pkt_body_len = data;
			rx_len = 0;
#else
            if(data > 0)
            {
                SPI1->SPIIE &= ~(SPIx_SPIIE_RXIE_Msk);
                rx_len = data + 2; /*" 2个pad"*/
                start_tick = MCU_GetTick();
                start_dma_rx(3, data + 2);
            }
            else
            {
                //slave_status = ANS_DATA_OK;
            }

            //if(data != 82)
            //{
            //	rx_len_err++;
            //}
#endif
		}
		else if(slave_status == RX_PKT_BODY)
		{
            dma_rx_buf[rx_len] = data;
            rx_len = (rx_len + 1) % SLAVE_COM_BUF_SZ;
            if(rx_len >= master_pkt_body_len)
            {
                slave_status = WAIT_ANS_DATA_OK;
                tx_off = 0;
                cfg_ans_data_no_ok();
            }
		}
		else if(slave_status == WAIT_ANS_DATA_OK)
		{
#if 0
			//SPI1->SPIIE |= (SPIx_SPIIE_TXIE_Msk); // TODO 测试
			SPI1->SPITXF = dma_tx_buf[tx_off];
			tx_off++;
			if(tx_off >= master_pkt_body_len)
			{
				tx_off = 0;
			}
#else
			SPI1->SPIIE &= ~(SPIx_SPIIE_RXIE_Msk);
			bak_slave_status = slave_status;
			start_dma_tx(DMA_TX_CH, dma_tx_buf, master_pkt_body_len + 1);
#endif
		}
		else if(slave_status == ANS_DATA_OK)
		{
#if 0
            SPI1->SPITXF = dma_tx_buf[tx_off];
            tx_off++;
            if(tx_off >= master_pkt_body_len)
            {
                tx_off = 0;
                slave_status = WAIT_RX_PKT;
            }
#else
            SPI1->SPIIE &= ~(SPIx_SPIIE_RXIE_Msk);
            //start_dma_rx(3, master_pkt_body_len);
            bak_slave_status = slave_status;
            start_dma_tx(DMA_TX_CH, dma_tx_buf, master_pkt_body_len + 1);	
#endif
		}
        /*" 禁止接收中断,中断只进一次，用于同步 "*/
        //SPI1->SPIIE &= ~(SPIx_SPIIE_RXIE_Msk);
        //INT_Dis(E_INT_ID_SPI1);
  #if 0     
        if(slave_status == WAIT_ANS_DATA_OK)
        {
            /*" 配置DMA发送数据未准备好 "*/
            start_dma_tx(DMA_TX_CH, dma_tx_buf, 6);
        }
        else if(slave_status == ANS_DATA_OK)
        {
            /*" 配置DMA发送应答数据 "*/
            start_dma_tx(DMA_TX_CH, dma_tx_buf, master_pkt_body_len);
			slave_status = WAIT_RX_PKT;
        }
#endif
	}
    // 发送完成中断
    if ((SPI1->SPIIE & (1 << 1)) && (SPI1->SPISTA & (1 << 1)))
    {
    }
    // Slave通信错误处理（Slave下传未满8位，SSN脚被拉高，SERR置位）。
	if ((SPI1->SPIIE & (1 << 2)) && (SPI1->SPISTA & (1 << 5)))
	{
        // 清除Slave Error错误标志
        SPI1->SPICR3 |= (1 << 0);
	}
    // 发送缓存区数据溢出标志
    if (SPI1->SPISTA & (1 << 3))
    {
        // 清除发送缓存区溢出标志
        SPI1->SPISTA |= (1 << 3);
    }
    // 接收缓存区数据溢出
    if (SPI1->SPISTA & (1 << 4))
    {
        // 清除接收缓存区溢出标志
        SPI1->SPISTA |= (1 << 4);
    }
}

/*" DMA接收完成中断 "*/
void dma_tx_end_isr(void)
{
    if(bak_slave_status == WAIT_ANS_DATA_OK)
    {
        /*" 打开接收中断，用于响应查询命令 "*/
        pad_d = SPI1->SPIRXF;
        SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
        //INT_En(E_INT_ID_SPI1);
    }
    else if(bak_slave_status == ANS_DATA_OK)
    {
        //dma_tx_tick = MCU_GetTick();
        dma_tx_tick = 0;
        pad_d = SPI1->SPIRXF;
        SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
        if(((DMA->CH4RAMAD & 0xfffff) - ((INT32U)dma_tx_buf & 0xfffff) ) == (master_pkt_body_len + 1))
        {
        slave_status = WAIT_RX_PKT; /*" 结束本次交互，等待下次交互 "*/
        }
        else
        {
        	dma_tx_err++;	/*" 如果一直失败导致变不成接收状态TODO "*/
        }
    }
}

void DMA_tx_end_check(void)
{
	if((dma_tx_tick != 0) && (MCU_CmpTick(dma_tx_tick, 10) == TRUE))
	{
		//dma_tx_tick = 0;
		//SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
		//slave_status = WAIT_RX_PKT; /*" 结束本次交互，等待下次交互 "*/
	}
}

void DMA_ClrITStatus(INT32U DMA_IT)
{
	DMA->CHSTATUS = DMA_IT;
}

/*DMA中断处理*/
void DMA_Int_Isr(void)
{
    INT32U Dmachstatus;

    Dmachstatus = DMA->CHSTATUS;
    if(Dmachstatus & DMA_CHSTATUS_DMACHFT_3) //全程中断被置起 接收中断
    {
        DMA_ClrITStatus(DMA_CHSTATUS_DMACHFT_3);//清除中断标志
        dma_rx_end_isr();
    }
    if(Dmachstatus & DMA_CHSTATUS_DMACHHT_3) //半程中断
    {
        DMA_ClrITStatus(DMA_CHSTATUS_DMACHHT_3);//清除中断标志
    }

    if(Dmachstatus & DMA_CHSTATUS_DMACHFT_4) //全程中断被置起,发送完中断
    {
        DMA_ClrITStatus(DMA_CHSTATUS_DMACHFT_4);//清除中断标志  
        dma_tx_end_isr();
    }
    if(Dmachstatus & DMA_CHSTATUS_DMACHHT_4) //半程中断
    {
        DMA_ClrITStatus(DMA_CHSTATUS_DMACHHT_4 );//清除中断标志
    }
}


void dma_init(void)
{
    MCU_PeriphClkSet(DMA_num, ENABLE);
    
    //使能DMA中断
    INT_VectSet(E_INT_ID_DMA, DMA_Int_Isr);
    INT_En(E_INT_ID_DMA);
        
    DMA->GLOBALCTRL |= 1 << 0; /*" 使能全局DMA "*/
}

void cs_int_init(void)
{
    ST_GPIO_EXTICONF cfg;
    //GPIO_ConfigPin(SPI_SLAVE_CS, INPUT, NULL_IN);

    cfg.PinId = SPI_SLAVE_CS;/*" 配置为数字功能还能中断?? "*/
    cfg.TrigModule = GPIO_EXTI_EDGE_FALLING;
    cfg.IntFuction= cs_int_isr;
    GPIO_EXTI_Enable(&cfg);
}

void slave_spi_int_init(void)
{
    //注册SPI1中断
    INT_VectSet(E_INT_ID_SPI1, SPI1_Int_Isr);
    INT_En(E_INT_ID_SPI1);

	/*" 打开接收中断，用于响应查询命令 "*/
    SPI1->SPIIE |= (SPIx_SPIIE_RXIE_Msk | SPIx_SPIIE_ERRIE_Msk); // TODO 测试

    SPI1->SPICR3 = 0x000F;  /*清除发送接收缓冲错误寄存器*/
    SPI1->SPISTA = 0x0018;  /*清除错误*/
}

void Slave_Spi_Init(void)
{
    //cs_int_init();
    slave_status = WAIT_RX_PKT;

    dma_init();
    SPI_Init(SPI_ID_SLAVE, SPI_NORMAL,NULL);
    slave_spi_int_init();
}



/*" 提交接收报文给上层协议，无报文时返回NULL，有报文时返回报文指针及长度 "*/
INT8U get_slave_spi_pkt(INT8U *buf)
{
    if(slave_status != WAIT_ANS_DATA_OK)
    {
        return 0;
    }
    else
    {
        LIB_MemCpy(dma_rx_buf, buf, rx_len);

        return rx_len;
    }
}

/*" 上层发送应答报文接口 "*/
void send_slave_ans_data(INT8U *data, INT16U len)
{
    //INT16U tms;

    if(tx_off != 0)
    {
        MCU_DelayMs(1);
    }

    if(len < (SLAVE_COM_BUF_SZ - 2 - 3))
    {
#if 1
        master_pkt_body_len = len + 2 + 3;
        dma_tx_buf[0] = ANS_OK_FLG;
        dma_tx_buf[1] = ANS_OK_FLG;
        LIB_MemCpy(data, dma_tx_buf + 2, len);
        LIB_MemSet(ANS_END_FLG, dma_tx_buf + 2 + len, 3);
#else
        LIB_MemCpy(data, dma_tx_buf, len);
        master_pkt_body_len = len;
#endif
        if(tx_off != 0)
        {
            tx_over_tms++;
        }
        tx_off = 0;
        slave_status = ANS_DATA_OK;
        /*
            tms = MCU_GetTick() - dma_rx_tick;
            if(tms > proc_tick)
            {
                proc_tick = tms;
            }
            */
    }
}

