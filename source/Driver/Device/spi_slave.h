#ifndef _H_SPI_SLAVE
#define _H_SPI_SLAVE

#define SLAVE_COM_BUF_SZ 256     /*" "*/	


extern void Slave_Spi_Init(void);
extern INT8U get_slave_spi_pkt(INT8U *buf);
extern void send_slave_ans_data(INT8U *data, INT16U len);
#endif
