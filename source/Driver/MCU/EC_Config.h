/*"**************************************************************************"*/
/*" CPU（ESAM）卡底层驱动配置文件                                            "*/
/*"**************************************************************************"*/
#ifndef _EC_CONFIG_H_
#define _EC_CONFIG_H_


#define _EC_E_SUP       0                   /*" 系统是否支持ESAM卡; 支持为1，不支持为0"*/

#define _EC_C_SUP	    1	                /*" 系统是否支持CPU卡，如果该位为零，所有CPU卡相关可不予设置 "*/



#define _EC_C_PWRCTL	0                /*" CPU 电源受IO控制 0不受 1 受"*/

#define _EC_SHARE_SD	1              /*" CPU卡和ESAM卡SD口是否共用，如果共用，需由硬件提供切换机制 "*/
												/*" 0：不共用,1：共用 "*/
                                                /*" _EC_C_SUP为1，该位方可有效 "*/
                                                /*" 如果_EC_C_SUP为0，该位必须置1 "*/

												/*" _EC_C_SUP为1，该位方可有效 "*/
#define _EC_UART_DRV    1            /*" CPU卡ESAM数据口（SD）是硬件MCU串口还是软件模拟实现"*/
                                        /*" 0: 软件模拟,1：硬件串口"*/

#define _EC_SHARE_OSC	0               /*" CPU卡和ESAM卡是否公用一种驱动频率"*/
												/*" 0：不共用,1：共用 "*/

#define _EC_C_CLKDRV_M	1	              /*" CPU卡CLK频率驱动模式 "*/
													/*" 0：IO口控制外部频率输出 "*/
													/*" 1：主MCU自身输出 "*/
#define _EC_E_CLKDRV_M	1	              /*" ESAM卡CLK频率驱动模式 "*/
													/*" 0：IO口控制外部频率输出 "*/
													/*" 1：主MCU自身输出 "*/		

#define _EC_C_DLY_H_ETU	 48                 /*" CPU卡数据交换半个BIT位占有时间，当驱动频率为3.58时候，该值取后， "*/
                                                /*" 延时函数测试应该为52uS宽度。该值根据不同编译系统，不同系统主晶振频率有所区别。 "*/
                                                /*" 此处频率为4M，取值为46.5us "*/
#define _EC_C_DLY_1ETU	 106                 /*" CPU卡数据交换一个BIT位占有时间，当驱动频率为3.58时候，该值取后， "*/
                                                /*" 延时函数测试应该为100uS宽度。该值根据不同编译系统，不同系统主晶振频率有所区别。 "*/
                                                /*" 此处频率为4M，取值为93us "*/
#define _EC_E_DLY_H_ETU	 48            /*" ESAM卡数据交换半个BIT位占有时间，当驱动频率为3.58时候，该值取后， "*/
                                                /*" 延时函数测试应该为52uS宽度。该值根据不同编译系统，不同系统主晶振频率有所区别。 "*/
                                                /*" 此处频率为4M，取值为46.5us "*/
#define _EC_E_DLY_1ETU	 106         	/*" ESAM卡数据交换一个BIT位占有时间，当驱动频率为3.58时候，该值取后， "*/
                                                /*" 延时函数测试应该为100uS宽度。该值根据不同编译系统，不同系统主晶振频率有所区别。 "*/
                                                /*" 此处频率为4M，取值为93us "*/

/*"通信IO端口配置"*/
		
#define _EC_C_SW_DIR              /*" 卡片到位信号方向寄存器定义（一般为输入）掉电处理为输入  "*/
#define _EC_C_VCC_DIR            /*" 卡片电源控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平  "*/
#define _EC_C_CLK_DIR            /*" 卡片振荡器控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平，"*/
																								/*" 有使用MCU本身输出振荡的，该位可不设置  "*/
#define _EC_C_RST_DIR             /*" 卡片复位控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平  "*/


#define _EC_C_TXD_DIR          /*" 卡片数据发送信号方向寄存器定义（一般为输出）掉电处理为输出低电平，"*/																								/*" 当卡片SD口受IO口驱动时候，该处即代表了TXD和RXD  "*/
#define _EC_C_RXD_DIR           /*" 卡片数据接收信号方向寄存器定义（一般为输入）掉电处理为输入，当卡片SD口受IO口驱动时候，该位无需设置  "*/


#define _EC_E_VCC_DIR          /*" ESAM电源控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平  "*/
#define _EC_E_CLK_DIR          /*" ESAM振荡器控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平，有使用MCU本身输出振荡的，该位可不设置  "*/

#define _EC_E_RST_DIR    	    /*" ESAM复位控制信号方向寄存器定义（一般为输出）掉电处理为输出低电平  "*/

#define _EC_E_TXD_DIR          /*" ESAM数据发送信号方向寄存器定义（一般为输出）掉电处理为输出低电平，当卡片SD口受IO口驱动时候，该处即代表了TXD和RXD  "*/
#define _EC_E_RXD_DIR          /*" ESAM数据接收信号方向寄存器定义（一般为输入）掉电处理为输入，当卡片SD口受IO口驱动时候，该位无需设置  "*/

#define _EC_TestIO_DIR    		  /*"  调试引脚，在测试模式下，在邋IO口方式下，每接收或发送一个字节，改引脚会翻转一次  "*/


#define _EC_C_SW                    /*" 卡片到位信号寄存器定义（一般为输入）掉电处理为输入  "*/
#define _EC_C_VCC                   /*" 卡片电源控制信号寄存器定义（一般为输出）掉电处理为输出低电平  "*/
#define _EC_C_CLK                   /*" 卡片振荡器控制信号寄存器定义（一般为输出）掉电处理为输出低电平 "*/


#define _EC_C_CLK_PWM        /*" 有使用MCU本身输出振荡的，该位定义为该振荡器开关  "*/

#define _EC_C_RST                   /*" 卡片复位控制信号寄存器定义（一般为输出）掉电处理为输出低电平  "*/
#define _EC_C_TXD                   /*" 卡片数据发送信号寄存器定义（一般为输出）掉电处理为输出低电平，当卡片SD口受IO口驱动时候，该处即代表了TXD和RXD  "*/
#define _EC_C_RXD                   /*" 卡片数据接收信号寄存器定义（一般为输入）掉电处理为输入，当卡片SD口受IO口驱动时候，该位无需设置  "*/

	
#define _EC_E_VCC                  /*" ESAM电源控制信号寄存器定义（一般为输出）掉电处理为输出低电平  "*/
#define _EC_E_CLK                  /*" ESAM振荡器控制信号寄存器定义（一般为输出）掉电处理为输出低电平， "*/

#define _EC_E_CLK_PWM    		/*" 有使用MCU本身输出振荡的，该位定义为该振荡器开关  "*/

#define _EC_E_RST                    /*" ESAM复位控制信号寄存器定义（一般为输出）掉电处理为输出低电平  "*/

#define _EC_E_TXD                   /*" ESAM数据发送信号寄存器定义（一般为输出）掉电处理为输出低电平，当卡片SD口受IO口驱动时候，该处即代表了TXD和RXD  "*/
#define _EC_E_RXD                   /*" ESAM数据接收信号寄存器定义（一般为输入）掉电处理为输入，当卡片SD口受IO口驱动时候，该处即代表了TXD和RXD  "*/


#define _EC_TestIO					 /*"  调试引脚，在测试模式下，在IO口方式下，每接收或发送一个字节，改引脚会翻转一次  "*/


#define _EC_C_IN_OK	1             /*" 定义卡到位输入硬件输入为高判断到位 "*/
#define _EC_C_VCC_ON 1        /*" 定义CPU卡电源打开输出控制 "*/
#define _EC_C_VCC_OFF 0
#define _EC_C_CLK_ON 1        /*" 定义CPU卡晶体振荡器打开输出控制，如果由MCU直接输出可无需设置 "*/
#define _EC_C_CLK_OFF 0
#define _EC_C_RST_ON 1         /*" 定义CPU卡复位打开输出控制 "*/
#define _EC_C_RST_OFF 0
#define _EC_C_TXD_ON 1         /*" 定义CPU卡SD输出打开输出控制，UART口无需设置，和ESAM共用时，统一由ESAM设置 "*/
#define _EC_C_TXD_OFF 0

#define _EC_C_RXD_ON 1          /*" 定义CPU卡SD输入接收控制，，UART口无需设置，和ESAM共用时，统一由ESAM设置 "*/


#define _EC_E_VCC_ON 0                 /*" 定义ESAM卡电源打开输出控制 "*/
#define _EC_E_VCC_OFF 1
#define _EC_E_CLK_ON 1                 /*" 定义ESAM卡晶体振荡器打开输出控制 "*/
#define _EC_E_CLK_OFF 0
#define _EC_E_RST_ON 1                 /*" 定义ESAM卡复位打开输出控制 "*/
#define _EC_E_RST_OFF 0
#define _EC_E_TXD_ON 1                 /*" 定义ESAM卡SD输出打开控制 "*/
#define _EC_E_TXD_OFF 0

#define _EC_E_RXD_ON 1                  /*" 定义ESAM卡SD输入接收控制 "*/


#define  pECRevBuff          (mECBuff+4) /*" 当SD口由串口驱动时候，需要定义一个指针用于串口接收 "*/
#define _ECBUFFCAP           2060         /*" 需要定义该缓冲区长度 "*/
#define  CARDBUFF1			 (pECRevBuff+7)                  /*"卡缓存区"*/
#define _CARDFILEBUF         (mECBuff+300)           /*"在卡交互过程中，存储应用信息文件"*/
#define  CARDBUFF2           (mECBuff+1400)           /*"在卡交互过程中，存储应用信息文件"*/

#define _MAX_SPIlEN 		  2048

#endif


