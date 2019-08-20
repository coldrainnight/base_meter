
#ifndef GPIO_H
#define GPIO_H
#include "System\sys_def.h"
#include "INT.h"
#include "MCU.h"
#include "Lib\LIB_func.h"

/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
#define HIGH (1)
#define LOW  (0)

#define INPUT  (1)
#define OUTPUT (0)


/*"GPIO输入输出相关宏定义"*/
#define NULL_IN                 0x00      /*"普通模式输入，即CMOS输入缓冲器类型"*/
#define PULL_UP_IN              0x01     /*"选择内接上/下拉"*/
#define HIGH_IMPED_IN           0x02     /*"高阻输入，即不使能输入"*/

#define NORMAL_OUT              0x00     /*"COMS模式输出"*/
#define PULLUP_OUT              0x01     /*"上拉输出"*/
#define OPENDRAIN_OUT           0x02     /*"N通道漏极开路模式输出"*/
#define OPENDRAIN_PULLUP_OUT    0x03     /*"N通道漏极开路模式输出"*/

/*"Begin:可根据标准IO号配置其功能"*/


/*uart相关引脚配置*/
#define RS485_TXD     0xff         /*"PB3 485发送口"*/
#define RS485_RXD     0xff         /*"PB2 485接收口"*/
#define RS485_REDE    0xff         /*"PB1 485收发控制"*/
#define IR_RXD        0xFF         /*"P55 红外通信接收口"*/
#define IR_TXD        0xFF         /*"P56 红外通信发送口"*/
#define RF_TXD        0xFF         /*"P82 RF发送口"*/
#define RF_RXD        0xFF         /*"P81 RF接收口"*/
#define RF_RST        0xFF         /*"P80 RF复位脚"*/

#define PLC_ZB        0xFF         /*"P57 PLC-ZB"*/
#define PLC_RXD       0xFF         /*"PF3 PLC通信接收口"*/
#define PLC_TXD       0xFF         /*"PF4 PLC通信发送口"*/
#define PLC_RST       0xFF         /*"PA7 PLC通信发送口"*/

#define SLAVE_TXD      0xff         /*PC5 计量芯发送口*/
#define SLAVE_RXD      0xff         /*PC4 计量芯接收口*/

#define MASTER_RXD      0x24         /*PC4 计量芯接收口*/
#define MASTER_TXD      0x25         /*PC5 计量芯发送口*/

//#define MK_STA
#define MC_RXD        0x53        /*"计量模块通信接口接收端口  PF3"*/
#define MC_TXD        0x54         /*"计量模块通信接口发送端口 PF4"*/

#define ALM1          0x55         /*"ALM1 PF5"*/

//#define TM_EN         0x67			/*"PG7"*/
#define TM_MCU        0x66			/*"PG6"*/

/*"E2PROM相关引脚配置"*/
#define MEM_SCL          0x62         /*"Eeprom IIC通信时钟信号 PG2"*/
#define MEM_SDA          0x63         /*"Eeprom IIC通信数据线 PG3"*/
#define MEM_WP           0x37         /*"Eeprom写保护控制 PD7"*/
#define MEM_PWR          0xFF         /*"Eeprom电源控制  "*/

/*"基表管理板SPI通信引脚配置"*/
#define SPI_SLAVE_CS     0x26              /*片选 PC6 SPI1 */
#define SPI_SLAVE_CLK    0x27              /*时钟 PC7*/
#define SPI_SLAVE_MISO   0x28              /* MISO PC8*/
#define SPI_SLAVE_MOSI   0x29              /* MOSI PC9*/

/*"FLASH相关引脚配置"*/
#define FLASH_SCK      0x33      /*"PD3 FLASH SPI时钟信号"*/
#define FLASH_SIN      0x35      /*"PD5 FLASH SPI输入信号   MOSI"*/
#define FLASH_SOUT     0x34      /*"PD4 FLASH SPI输出信号 MISO"*/
#define FLASH_CS       0x32      /*"PD2 CPU卡复位引脚"*/     
#define FLASH_PWR      0x2A      /*"PC10 FLASH电源引脚"*/   

#define MAIN_IN    	     0x23    /*PC3,管理板到位检测*/
/*"LCD相关引脚配置"*/
#define LCD_SCL          0x43         /*"PB15 LCD IIC通信时钟信号PE3 "*/
#define LCD_SDA          0x44         /*"PB14 LCD IIC通信时钟数据线PE4 "*/
#define LCD_PWR          0xff         /*"PE3 LCD电源控制"*/
#define BACK_LIGHT_CTRL  0x42         /*"PE2 背光控制 "*/

/*"RTC相关引脚配置"*/
#define RTC_SCL          0x30         /*"PD0 RTC IIC通信时钟信号"*/
#define RTC_SDA          0x31   	  /*"PD1 RTC IIC通信数据"*/

/*"测温相关引脚配置"*/
#define TH_SCL          0x2E         /*"PC14 TH IIC通信时钟信号"*/
#define TH_SDA          0x2D   	  	 /*"PC13 TH IIC通信数据"*/

/*"继电器相关引脚配置"*/
#define TRIP_RELAY_ON   0x19         /*"内置跳闸继电器控制1 ON --PB9"*/
#define TRIP_RELAY_OFF   0x18         /*"内置跳闸继电器控制2 OFF --PB8"*/
#define RELAY_STA_CHK      0x17         /*"内置继电器状态检测信号输入 PB7 "*/
#define RELAY_PWR_CHK      0xff         /*"内置继电器状态检测电源输出 PB11"*/
#define ALARM_RELAY_ON   0xFF         /*"内置跳闸继电器控制1 ON --PD6"*/
#define ALARM_RELAY_OFF   0xFF         /*"内置跳闸继电器控制2 OFF --PD7"*/

/*"计量相关引脚配置"*/
#define MSUR_CHIP_RST        0xFF           /*"PC2计量芯片复位 无"*/
#define MSUR_CHIP_CLK        0xFF          /*"PF14计量芯片SPI时钟信号(无)"*/
#define MSUR_CHIP_DI         0xFF        /*"PF13计量芯片SPI数据输入口(无)"*/
#define MSUR_CHIP_DO         0xFF          /*"PF12计量芯片SPI数据输出口(MOSI)"*/
//#define A_PULSE_IN           0x06            /*"有功脉冲输入 PA6"*/
#define PULSE_CTL            0x07           /*计量芯片脉冲输出控制引脚 PA7*/
#define PULSE_VCC            0xff           /*计量脉冲电源控制引脚，用于切换秒脉冲 PB10*/


/*开盖检测*/
#define METER_COVER_CHK     0x56         /*"开盖检测按键 PF6,使用管理板插拔判断开盖"*/
#define METER_COVER_PWR     0xFF         /*"开盖检测电源(暂无)"*/
#define METER_BOX_CHK       0xFF           /*"开端盖检测按键"*/
#define METER_BOX_PWR       0xFF           /*"开端盖检测电源(暂无)"*/

/*ESAM相关引脚配置*/
#define SPI_ESAM_CS    0xff               /*ESAM片选 PF15*/
#define SPI_ESAM_CLK   0xff              /*ESAM时钟 PF14*/
#define SPI_ESAM_MISO  0xff              /*ESAM MISO PF13*/
#define SPI_ESAM_MOSI  0xff             /*ESAM MOSI PF12*/
#define SPI_ESAM_POWER  0xff             /* PB15 ESAM MOSI POWER*/

/*"键盘相关引脚配置"*/
#define KEYPAD_ROWA       0xFF          /*Keypad ROWA*/
#define KEYPAD_ROWB       0xFF          /*Keypad ROWB*/
#define KEYPAD_ROWC       0xFF          /*Keypad ROWC*/
#define KEYPAD_ROWD       0xFF          /*Keypad ROWD*/
#define KEYPAD_COLUMN0    0xFF           /*Keypad ROW0*/
#define KEYPAD_COLUMN1    0xFF           /*Keypad ROW1*/
#define KEYPAD_COLUMN2    0xFF          /*Keypad ROW2*/
#define KEYPAD_COLUMN3    0xFF          /*Keypad ROW3*/


/*按键相关*/
#define DISP_KEY         0x1E           /*"显示按键 PB14"*/
#define PROG_KEY         0xFF           /*"编程按键 "*/
#define KEY_VCC			 0x13          /*"显示按键电源 PB3"*/
#define MAIN_KEY         0xFF          /*"PC3 BUSY引脚 透传按键信号 "*/

/*报警，指示灯、脉冲灯*/
#define LED_PULSE_A      0x05          /*"PA5 有功脉冲灯 "*/
#define LED_PULSE_R      0x08          /*"PA7无功脉冲灯 "*/
#define LED_RELAY_LED    0x1F           /*"继电器指示灯 PB15"*/
#define LED_ALM_GREEN    0xFF           /*"报警灯绿色"*/
#define LED_ALM_RED      0xFF          /*"报警灯红色"*/


/*"其它功能相关引脚配置"*/

#define BEEP             0xFF          /*" 蜂鸣器 "*/

#define MULT_PULSE       0xFF         /*"多功能端子"*/

#define RTC_BVT_VOL      0x2F         /*PC15 时钟电池电压检测*/

#define PD_BVT_VOL       0xFF         /*低功耗电池电压检测*/    

#define PD_DECTECT       0x5B         /*PF11掉电检测*/    

#define EVENT_OUT        0xFF           /*"事件主动上报 PF5"*/

#define TEMP_CHECK       0xFF          /*"温度检测使能 "*/

#define RTC_1HZ      0x66          /*"秒脉冲输出 或者 外部8025T脉冲输入 PG6"*/

#define RTC_EN 			 0x09			/*"8025T秒脉冲输出使能，低电平使能 PA9 "*/


#define FAC_JUMP         0x67          /*厂内跳线 PG7*/

#define VTC_DECTECT      0xFF          /*"VTC功能检测 "*/

#define MSD              0xFF          /*" 磁检测"*/

#define SPRG             0xFF          /*"编程状态检测 "*/

//#define TH_SENSOR_SCL    0x62          /*PG2 温湿度传感器SCL*/

//#define TH_SENSOR_SDA    0x63          /*PG3 温湿度传感器SDA*/

#define TL_SENSOR      0xff          /*PG2火线温度传感器*/

#define T_SENSOR       0xff          /*PG3零线温度传感器*/

#define COM_RQ          0x23           /*管理芯到位信号 PC3*/


typedef enum
{ 
    GPIO_IN   	= 0x00, /*!< GPIO Input Mode */
    GPIO_Mode_OUT  	= 0x01, /*!< GPIO Output Mode */
    GPIO_Mode_DIG   = 0x02, /*!< GPIO Alternate function Mode */
    GPIO_Mode_ANA   = 0x03  /*!< GPIO Analog Mode */
}GPIOFun_TypeDef;

typedef enum
{
    GPIO_EXTI_EDGE_RISING = 0x00,
    GPIO_EXTI_EDGE_FALLING = 0x01,
    GPIO_EXTI_EDGE_BOTH = 0x02,
    GPIO_EXTI_EDGE_DISABLE = 0x03
}EN_GPIO_EXTI_EDGE;

typedef struct
{
    INT8U PinId;     		
    EN_GPIO_EXTI_EDGE TrigModule;  		/*"外部中断触发方式"*/
    ISR_FNCT_VOID IntFuction;       /*"外部中断响应功能处理函数"*/
}ST_GPIO_EXTICONF;


typedef struct
{
    INT8U PinID;                  /*"GPIO端口号"*/
    INT8U Dir;                    /*"GPIO方向配置"*/
    INT8U Mode;                   /*"GPIO模式配置"*/
    INT8U Output;                 /*"GPIO输出电平"*/
} ST_GPIO_CONFIG;
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    

extern void GPIO_Init(INT8U Mode);

extern void GPIO_SetMulti(INT8U PinId,GPIOFun_TypeDef Mode);

extern void GPIO_ConfigPin(INT8U PinId,INT8U Dir,INT8U Mode);

extern void GPIO_OutputPin(INT8U PinId,INT8U OutPut);

extern INT8U GPIO_ReadPin(INT8U PinId);

extern void GPIO_EXTI_Enable(ST_GPIO_EXTICONF *EXTI_GpioConf);

extern void GPIO_EXTI_Disable(INT8U PinId);

#endif  


