#ifndef _LCD_H_
#define _LCD_H_

#include "system\Sys_def.h"
#include "Driver\MCU\MCU.h"

#define LCD_RAM_SIZE  (35)/*"定义液晶的最大显存"*/
/*"仅定义，使用在表格中，定义的不一定会使用"*/
#define  LCD_SEG0    29
#define  LCD_SEG1    34
#define  LCD_SEG2    33
#define  LCD_SEG3    32
#define  LCD_SEG4    31
#define  LCD_SEG5    30

#define  LCD_SEG6    29
#define  LCD_SEG7    28
#define  LCD_SEG8    27
#define  LCD_SEG9    26
#define  LCD_SEG10  25
#define  LCD_SEG11  24
#define  LCD_SEG12  23
#define  LCD_SEG13  22 
#define  LCD_SEG14  17
#define  LCD_SEG15  16
#define  LCD_SEG16  15
#define  LCD_SEG17  14

#define  LCD_SEG18  13
#define  LCD_SEG19  12

#define  LCD_SEG20  11
#define  LCD_SEG21  10
#define  LCD_SEG22  9
#define  LCD_SEG23  8
#define  LCD_SEG24  7
#define  LCD_SEG25  6
#define  LCD_SEG26  5 
#define  LCD_SEG27  4 
#define  LCD_SEG28  3 
#define  LCD_SEG29  2 




extern void LCD_Init(INT8U Mode);
extern INT8U LCD_Refresh (INT8U *ShowBuffer);
extern void LCD_Close(void);

#endif
