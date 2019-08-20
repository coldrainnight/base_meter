
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


/*"GPIO���������غ궨��"*/
#define NULL_IN                 0x00      /*"��ͨģʽ���룬��CMOS���뻺��������"*/
#define PULL_UP_IN              0x01     /*"ѡ���ڽ���/����"*/
#define HIGH_IMPED_IN           0x02     /*"�������룬����ʹ������"*/

#define NORMAL_OUT              0x00     /*"COMSģʽ���"*/
#define PULLUP_OUT              0x01     /*"�������"*/
#define OPENDRAIN_OUT           0x02     /*"Nͨ��©����·ģʽ���"*/
#define OPENDRAIN_PULLUP_OUT    0x03     /*"Nͨ��©����·ģʽ���"*/

/*"Begin:�ɸ��ݱ�׼IO�������书��"*/


/*uart�����������*/
#define RS485_TXD     0xff         /*"PB3 485���Ϳ�"*/
#define RS485_RXD     0xff         /*"PB2 485���տ�"*/
#define RS485_REDE    0xff         /*"PB1 485�շ�����"*/
#define IR_RXD        0xFF         /*"P55 ����ͨ�Ž��տ�"*/
#define IR_TXD        0xFF         /*"P56 ����ͨ�ŷ��Ϳ�"*/
#define RF_TXD        0xFF         /*"P82 RF���Ϳ�"*/
#define RF_RXD        0xFF         /*"P81 RF���տ�"*/
#define RF_RST        0xFF         /*"P80 RF��λ��"*/

#define PLC_ZB        0xFF         /*"P57 PLC-ZB"*/
#define PLC_RXD       0xFF         /*"PF3 PLCͨ�Ž��տ�"*/
#define PLC_TXD       0xFF         /*"PF4 PLCͨ�ŷ��Ϳ�"*/
#define PLC_RST       0xFF         /*"PA7 PLCͨ�ŷ��Ϳ�"*/

#define SLAVE_TXD      0xff         /*PC5 ����о���Ϳ�*/
#define SLAVE_RXD      0xff         /*PC4 ����о���տ�*/

#define MASTER_RXD      0x24         /*PC4 ����о���տ�*/
#define MASTER_TXD      0x25         /*PC5 ����о���Ϳ�*/

//#define MK_STA
#define MC_RXD        0x53        /*"����ģ��ͨ�Žӿڽ��ն˿�  PF3"*/
#define MC_TXD        0x54         /*"����ģ��ͨ�Žӿڷ��Ͷ˿� PF4"*/

#define ALM1          0x55         /*"ALM1 PF5"*/

//#define TM_EN         0x67			/*"PG7"*/
#define TM_MCU        0x66			/*"PG6"*/

/*"E2PROM�����������"*/
#define MEM_SCL          0x62         /*"Eeprom IICͨ��ʱ���ź� PG2"*/
#define MEM_SDA          0x63         /*"Eeprom IICͨ�������� PG3"*/
#define MEM_WP           0x37         /*"Eepromд�������� PD7"*/
#define MEM_PWR          0xFF         /*"Eeprom��Դ����  "*/

/*"��������SPIͨ����������"*/
#define SPI_SLAVE_CS     0x26              /*Ƭѡ PC6 SPI1 */
#define SPI_SLAVE_CLK    0x27              /*ʱ�� PC7*/
#define SPI_SLAVE_MISO   0x28              /* MISO PC8*/
#define SPI_SLAVE_MOSI   0x29              /* MOSI PC9*/

/*"FLASH�����������"*/
#define FLASH_SCK      0x33      /*"PD3 FLASH SPIʱ���ź�"*/
#define FLASH_SIN      0x35      /*"PD5 FLASH SPI�����ź�   MOSI"*/
#define FLASH_SOUT     0x34      /*"PD4 FLASH SPI����ź� MISO"*/
#define FLASH_CS       0x32      /*"PD2 CPU����λ����"*/     
#define FLASH_PWR      0x2A      /*"PC10 FLASH��Դ����"*/   

#define MAIN_IN    	     0x23    /*PC3,����嵽λ���*/
/*"LCD�����������"*/
#define LCD_SCL          0x43         /*"PB15 LCD IICͨ��ʱ���ź�PE3 "*/
#define LCD_SDA          0x44         /*"PB14 LCD IICͨ��ʱ��������PE4 "*/
#define LCD_PWR          0xff         /*"PE3 LCD��Դ����"*/
#define BACK_LIGHT_CTRL  0x42         /*"PE2 ������� "*/

/*"RTC�����������"*/
#define RTC_SCL          0x30         /*"PD0 RTC IICͨ��ʱ���ź�"*/
#define RTC_SDA          0x31   	  /*"PD1 RTC IICͨ������"*/

/*"���������������"*/
#define TH_SCL          0x2E         /*"PC14 TH IICͨ��ʱ���ź�"*/
#define TH_SDA          0x2D   	  	 /*"PC13 TH IICͨ������"*/

/*"�̵��������������"*/
#define TRIP_RELAY_ON   0x19         /*"������բ�̵�������1 ON --PB9"*/
#define TRIP_RELAY_OFF   0x18         /*"������բ�̵�������2 OFF --PB8"*/
#define RELAY_STA_CHK      0x17         /*"���ü̵���״̬����ź����� PB7 "*/
#define RELAY_PWR_CHK      0xff         /*"���ü̵���״̬����Դ��� PB11"*/
#define ALARM_RELAY_ON   0xFF         /*"������բ�̵�������1 ON --PD6"*/
#define ALARM_RELAY_OFF   0xFF         /*"������բ�̵�������2 OFF --PD7"*/

/*"���������������"*/
#define MSUR_CHIP_RST        0xFF           /*"PC2����оƬ��λ ��"*/
#define MSUR_CHIP_CLK        0xFF          /*"PF14����оƬSPIʱ���ź�(��)"*/
#define MSUR_CHIP_DI         0xFF        /*"PF13����оƬSPI���������(��)"*/
#define MSUR_CHIP_DO         0xFF          /*"PF12����оƬSPI���������(MOSI)"*/
//#define A_PULSE_IN           0x06            /*"�й��������� PA6"*/
#define PULSE_CTL            0x07           /*����оƬ��������������� PA7*/
#define PULSE_VCC            0xff           /*���������Դ�������ţ������л������� PB10*/


/*���Ǽ��*/
#define METER_COVER_CHK     0x56         /*"���Ǽ�ⰴ�� PF6,ʹ�ù�������жϿ���"*/
#define METER_COVER_PWR     0xFF         /*"���Ǽ���Դ(����)"*/
#define METER_BOX_CHK       0xFF           /*"���˸Ǽ�ⰴ��"*/
#define METER_BOX_PWR       0xFF           /*"���˸Ǽ���Դ(����)"*/

/*ESAM�����������*/
#define SPI_ESAM_CS    0xff               /*ESAMƬѡ PF15*/
#define SPI_ESAM_CLK   0xff              /*ESAMʱ�� PF14*/
#define SPI_ESAM_MISO  0xff              /*ESAM MISO PF13*/
#define SPI_ESAM_MOSI  0xff             /*ESAM MOSI PF12*/
#define SPI_ESAM_POWER  0xff             /* PB15 ESAM MOSI POWER*/

/*"���������������"*/
#define KEYPAD_ROWA       0xFF          /*Keypad ROWA*/
#define KEYPAD_ROWB       0xFF          /*Keypad ROWB*/
#define KEYPAD_ROWC       0xFF          /*Keypad ROWC*/
#define KEYPAD_ROWD       0xFF          /*Keypad ROWD*/
#define KEYPAD_COLUMN0    0xFF           /*Keypad ROW0*/
#define KEYPAD_COLUMN1    0xFF           /*Keypad ROW1*/
#define KEYPAD_COLUMN2    0xFF          /*Keypad ROW2*/
#define KEYPAD_COLUMN3    0xFF          /*Keypad ROW3*/


/*�������*/
#define DISP_KEY         0x1E           /*"��ʾ���� PB14"*/
#define PROG_KEY         0xFF           /*"��̰��� "*/
#define KEY_VCC			 0x13          /*"��ʾ������Դ PB3"*/
#define MAIN_KEY         0xFF          /*"PC3 BUSY���� ͸�������ź� "*/

/*������ָʾ�ơ������*/
#define LED_PULSE_A      0x05          /*"PA5 �й������ "*/
#define LED_PULSE_R      0x08          /*"PA7�޹������ "*/
#define LED_RELAY_LED    0x1F           /*"�̵���ָʾ�� PB15"*/
#define LED_ALM_GREEN    0xFF           /*"��������ɫ"*/
#define LED_ALM_RED      0xFF          /*"�����ƺ�ɫ"*/


/*"�������������������"*/

#define BEEP             0xFF          /*" ������ "*/

#define MULT_PULSE       0xFF         /*"�๦�ܶ���"*/

#define RTC_BVT_VOL      0x2F         /*PC15 ʱ�ӵ�ص�ѹ���*/

#define PD_BVT_VOL       0xFF         /*�͹��ĵ�ص�ѹ���*/    

#define PD_DECTECT       0x5B         /*PF11������*/    

#define EVENT_OUT        0xFF           /*"�¼������ϱ� PF5"*/

#define TEMP_CHECK       0xFF          /*"�¶ȼ��ʹ�� "*/

#define RTC_1HZ      0x66          /*"��������� ���� �ⲿ8025T�������� PG6"*/

#define RTC_EN 			 0x09			/*"8025T���������ʹ�ܣ��͵�ƽʹ�� PA9 "*/


#define FAC_JUMP         0x67          /*�������� PG7*/

#define VTC_DECTECT      0xFF          /*"VTC���ܼ�� "*/

#define MSD              0xFF          /*" �ż��"*/

#define SPRG             0xFF          /*"���״̬��� "*/

//#define TH_SENSOR_SCL    0x62          /*PG2 ��ʪ�ȴ�����SCL*/

//#define TH_SENSOR_SDA    0x63          /*PG3 ��ʪ�ȴ�����SDA*/

#define TL_SENSOR      0xff          /*PG2�����¶ȴ�����*/

#define T_SENSOR       0xff          /*PG3�����¶ȴ�����*/

#define COM_RQ          0x23           /*����о��λ�ź� PC3*/


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
    EN_GPIO_EXTI_EDGE TrigModule;  		/*"�ⲿ�жϴ�����ʽ"*/
    ISR_FNCT_VOID IntFuction;       /*"�ⲿ�ж���Ӧ���ܴ�����"*/
}ST_GPIO_EXTICONF;


typedef struct
{
    INT8U PinID;                  /*"GPIO�˿ں�"*/
    INT8U Dir;                    /*"GPIO��������"*/
    INT8U Mode;                   /*"GPIOģʽ����"*/
    INT8U Output;                 /*"GPIO�����ƽ"*/
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


