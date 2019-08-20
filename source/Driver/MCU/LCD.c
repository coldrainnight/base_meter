/*"***********************************************************"*/		
/*"FileName: LCD.c "*/		
/*"Author: "*/     
/*"Version : 1.0       Date: 2017.01.5"*/	
/*"Description: "*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"***********************************************************"*/	
#include "Driver\MCU\LCD.h"
#include "Driver\MCU\AdcDrv.h"
#include "Driver\MCU\IIC.h"

#define LCD_PIN_MAX     (40)
#define LCD_SEG_MAX     (28)
#define LCD_REG_START (0x40010C24)
#if 0
const INT8U  gDisplayMultiSet[LCD_SEG_MAX]=
{
/*"液晶引脚  	MCU		"*/	
/*	1	*/	LCD_SEG1	,
/*	2	*/	LCD_SEG2	,
/*	3	*/	LCD_SEG3	,
/*	4	*/	LCD_SEG4	,
/*	5	*/	LCD_SEG5	,
/*	6	*/	LCD_SEG6	,
/*	7	*/	LCD_SEG7	,
/*	8	*/	LCD_SEG8	,
/*	9	*/	LCD_SEG9	,
/*	10	*/	LCD_SEG10	,
/*	11	*/	LCD_SEG11	,
/*	12	*/	LCD_SEG12	,
/*	13	*/	LCD_SEG13	,
/*	14	*/	LCD_SEG14	,
/*	15	*/	LCD_SEG15	,
/*	16	*/	LCD_SEG16	,
/*	17	*/	LCD_SEG17	,
/*	18	*/	LCD_SEG18	,
/*	19	*/	LCD_SEG19	,
/*	20	*/	LCD_SEG34	,
/*	21	*/	LCD_SEG35	,
/*	22	*/	LCD_SEG36	,
/*	23	*/	LCD_SEG37	,
/*	24	*/	LCD_SEG38	,
/*	25	*/	LCD_SEG20	,
/*	26	*/	LCD_SEG21	,
/*	27	*/	LCD_SEG22	,
/*	28	*/	LCD_SEG23	
};

/** @defgroup DISP_SEG_EN0_SEGEN0
  * @{
  */
#define DISP_SEG_EN0_SEGEN0_Pos		               				0                                             
#define DISP_SEG_EN0_SEGEN0_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN0_Pos)  
#define DISP_SEG_EN0_SEGEN0_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN0_ENABLE    						  	((INT32U)0x00000001)

/** @defgroup DISP_SEG_EN0_SEGEN1
  * @{
  */
#define DISP_SEG_EN0_SEGEN1_Pos		               				1                                             
#define DISP_SEG_EN0_SEGEN1_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN1_Pos)  
#define DISP_SEG_EN0_SEGEN1_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN1_ENABLE    						  	((INT32U)0x00000002)

/** @defgroup DISP_SEG_EN0_SEGEN2
  * @{
  */
#define DISP_SEG_EN0_SEGEN2_Pos		               				2                                             
#define DISP_SEG_EN0_SEGEN2_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN2_Pos)  
#define DISP_SEG_EN0_SEGEN2_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN2_ENABLE    						  	((INT32U)0x00000004)

/** @defgroup DISP_SEG_EN0_SEGEN3
  * @{
  */
#define DISP_SEG_EN0_SEGEN3_Pos		               				3                                             
#define DISP_SEG_EN0_SEGEN3_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN3_Pos)  
#define DISP_SEG_EN0_SEGEN3_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN3_ENABLE    						  	((INT32U)0x00000008)

/** @defgroup DISP_SEG_EN0_SEGEN4
  * @{
  */
#define DISP_SEG_EN0_SEGEN4_Pos		               				4                                            
#define DISP_SEG_EN0_SEGEN4_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN4_Pos)  
#define DISP_SEG_EN0_SEGEN4_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN4_ENABLE    						  	((INT32U)0x00000010)

/** @defgroup DISP_SEG_EN0_SEGEN5
  * @{
  */
#define DISP_SEG_EN0_SEGEN5_Pos		               				5                                            
#define DISP_SEG_EN0_SEGEN5_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN5_Pos)  
#define DISP_SEG_EN0_SEGEN5_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN5_ENABLE    						  	((INT32U)0x00000020)

/** @defgroup DISP_SEG_EN0_SEGEN6
  * @{
  */
#define DISP_SEG_EN0_SEGEN6_Pos		               				6                                            
#define DISP_SEG_EN0_SEGEN6_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN6_Pos)  
#define DISP_SEG_EN0_SEGEN6_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN6_ENABLE    						  	((INT32U)0x00000040)

/** @defgroup DISP_SEG_EN0_SEGEN7
  * @{
  */
#define DISP_SEG_EN0_SEGEN7_Pos		               				7                                            
#define DISP_SEG_EN0_SEGEN7_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN7_Pos)  
#define DISP_SEG_EN0_SEGEN7_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN7_ENABLE    						  	((INT32U)0x00000080)

/** @defgroup DISP_SEG_EN0_SEGEN8
  * @{
  */
#define DISP_SEG_EN0_SEGEN8_Pos		               				8                                            
#define DISP_SEG_EN0_SEGEN8_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN8_Pos)  
#define DISP_SEG_EN0_SEGEN8_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN8_ENABLE    						  	((INT32U)0x00000100)

/** @defgroup DISP_SEG_EN0_SEGEN9
  * @{
  */
#define DISP_SEG_EN0_SEGEN9_Pos		               				9                                            
#define DISP_SEG_EN0_SEGEN9_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN9_Pos)  
#define DISP_SEG_EN0_SEGEN9_DISABLE    							((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN9_ENABLE    						  	((INT32U)0x00000200)

/** @defgroup DISP_SEG_EN0_SEGEN10
  * @{
  */
#define DISP_SEG_EN0_SEGEN10_Pos		               			10                                            
#define DISP_SEG_EN0_SEGEN10_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN10_Pos)  
#define DISP_SEG_EN0_SEGEN10_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN10_ENABLE    						  	((INT32U)0x00000400)

/** @defgroup DISP_SEG_EN0_SEGEN11
  * @{
  */
#define DISP_SEG_EN0_SEGEN11_Pos		               			11                                            
#define DISP_SEG_EN0_SEGEN11_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN11_Pos)  
#define DISP_SEG_EN0_SEGEN11_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN11_ENABLE    						  	((INT32U)0x00000800)

/** @defgroup DISP_SEG_EN0_SEGEN12
  * @{
  */
#define DISP_SEG_EN0_SEGEN12_Pos		               			12                                            
#define DISP_SEG_EN0_SEGEN12_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN12_Pos)  
#define DISP_SEG_EN0_SEGEN12_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN12_ENABLE    						  	((INT32U)0x00001000)

/** @defgroup DISP_SEG_EN0_SEGEN13
  * @{
  */
#define DISP_SEG_EN0_SEGEN13_Pos		               			13                                            
#define DISP_SEG_EN0_SEGEN13_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN13_Pos)  
#define DISP_SEG_EN0_SEGEN13_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN13_ENABLE    						  	((INT32U)0x00002000)

/** @defgroup DISP_SEG_EN0_SEGEN14
  * @{
  */
#define DISP_SEG_EN0_SEGEN14_Pos		               			14                                            
#define DISP_SEG_EN0_SEGEN14_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN14_Pos)  
#define DISP_SEG_EN0_SEGEN14_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN14_ENABLE    						  	((INT32U)0x00004000)

/** @defgroup DISP_SEG_EN0_SEGEN15
  * @{
  */
#define DISP_SEG_EN0_SEGEN15_Pos		               			15                                            
#define DISP_SEG_EN0_SEGEN15_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN15_Pos)  
#define DISP_SEG_EN0_SEGEN15_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN15_ENABLE    						  	((INT32U)0x00008000)

/** @defgroup DISP_SEG_EN0_SEGEN16
  * @{
  */
#define DISP_SEG_EN0_SEGEN16_Pos		               			16                                            
#define DISP_SEG_EN0_SEGEN16_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN16_Pos)  
#define DISP_SEG_EN0_SEGEN16_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN16_ENABLE    						  	((INT32U)0x00010000)

/** @defgroup DISP_SEG_EN0_SEGEN17
  * @{
  */
#define DISP_SEG_EN0_SEGEN17_Pos		               			17                                            
#define DISP_SEG_EN0_SEGEN17_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN17_Pos)  
#define DISP_SEG_EN0_SEGEN17_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN17_ENABLE    						  	((INT32U)0x00020000)

/** @defgroup DISP_SEG_EN0_SEGEN18
  * @{
  */
#define DISP_SEG_EN0_SEGEN18_Pos		               			18                                            
#define DISP_SEG_EN0_SEGEN18_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN18_Pos)  
#define DISP_SEG_EN0_SEGEN18_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN18_ENABLE    						  	((INT32U)0x00040000)

/** @defgroup DISP_SEG_EN0_SEGEN19
  * @{
  */
#define DISP_SEG_EN0_SEGEN19_Pos		               			19                                            
#define DISP_SEG_EN0_SEGEN19_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN19_Pos)  
#define DISP_SEG_EN0_SEGEN19_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN19_ENABLE    						  	((INT32U)0x00080000)

/** @defgroup DISP_SEG_EN0_SEGEN20
  * @{
  */
#define DISP_SEG_EN0_SEGEN20_Pos		               			20                                            
#define DISP_SEG_EN0_SEGEN20_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN20_Pos)  
#define DISP_SEG_EN0_SEGEN20_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN20_ENABLE    						  	((INT32U)0x00100000)

/** @defgroup DISP_SEG_EN0_SEGEN21
  * @{
  */
#define DISP_SEG_EN0_SEGEN21_Pos		               			21                                            
#define DISP_SEG_EN0_SEGEN21_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN21_Pos)  
#define DISP_SEG_EN0_SEGEN21_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN21_ENABLE    						  	((INT32U)0x00200000)

/** @defgroup DISP_SEG_EN0_SEGEN22
  * @{
  */
#define DISP_SEG_EN0_SEGEN22_Pos		               			22                                            
#define DISP_SEG_EN0_SEGEN22_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN22_Pos)  
#define DISP_SEG_EN0_SEGEN22_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN22_ENABLE    						  	((INT32U)0x00400000)

/** @defgroup DISP_SEG_EN0_SEGEN23
  * @{
  */
#define DISP_SEG_EN0_SEGEN23_Pos		               			23                                            
#define DISP_SEG_EN0_SEGEN23_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN23_Pos)  
#define DISP_SEG_EN0_SEGEN23_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN23_ENABLE    						  	((INT32U)0x00800000)

/** @defgroup DISP_SEG_EN0_SEGEN24
  * @{
  */
#define DISP_SEG_EN0_SEGEN24_Pos		               			24                                            
#define DISP_SEG_EN0_SEGEN24_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN24_Pos)  
#define DISP_SEG_EN0_SEGEN24_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN24_ENABLE    						  	((INT32U)0x01000000)

/** @defgroup DISP_SEG_EN0_SEGEN25
  * @{
  */
#define DISP_SEG_EN0_SEGEN25_Pos		               			25                                            
#define DISP_SEG_EN0_SEGEN25_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN25_Pos)  
#define DISP_SEG_EN0_SEGEN25_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN25_ENABLE    						  	((INT32U)0x02000000)

/** @defgroup DISP_SEG_EN0_SEGEN26
  * @{
  */
#define DISP_SEG_EN0_SEGEN26_Pos		               			26                                            
#define DISP_SEG_EN0_SEGEN26_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN26_Pos)  
#define DISP_SEG_EN0_SEGEN26_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN26_ENABLE    						  	((INT32U)0x04000000)

/** @defgroup DISP_SEG_EN0_SEGEN27
  * @{
  */
#define DISP_SEG_EN0_SEGEN27_Pos		               			27                                            
#define DISP_SEG_EN0_SEGEN27_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN27_Pos)  
#define DISP_SEG_EN0_SEGEN27_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN27_ENABLE    						  		((INT32U)0x08000000)

/** @defgroup DISP_SEG_EN0_SEGEN28
  * @{
  */
#define DISP_SEG_EN0_SEGEN28_Pos		               			28                                            
#define DISP_SEG_EN0_SEGEN28_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN28_Pos)  
#define DISP_SEG_EN0_SEGEN28_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN28_ENABLE    						  	((INT32U)0x10000000)

/** @defgroup DISP_SEG_EN0_SEGEN29
  * @{
  */
#define DISP_SEG_EN0_SEGEN29_Pos		               			29                                            
#define DISP_SEG_EN0_SEGEN29_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN29_Pos)  
#define DISP_SEG_EN0_SEGEN29_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN29_ENABLE    						  	((INT32U)0x20000000)

/** @defgroup DISP_SEG_EN0_SEGEN30
  * @{
  */
#define DISP_SEG_EN0_SEGEN30_Pos		               			30                                            
#define DISP_SEG_EN0_SEGEN30_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN30_Pos)  
#define DISP_SEG_EN0_SEGEN30_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN30_ENABLE    						  	((INT32U)0x40000000)

/** @defgroup DISP_SEG_EN0_SEGEN31
  * @{
  */
#define DISP_SEG_EN0_SEGEN31_Pos		               			31                                            
#define DISP_SEG_EN0_SEGEN31_Msk               					(0x01UL << DISP_SEG_EN0_SEGEN31_Pos)  
#define DISP_SEG_EN0_SEGEN31_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN0_SEGEN31_ENABLE    						  	((INT32U)0x80000000)


/**
* @}DISP_SEG_EN1
*/ 
  
/** @defgroup DISP_SEG_EN1_SEGEN32
  * @{
  */
#define DISP_SEG_EN1_SEGEN32_Pos		               			0                                             
#define DISP_SEG_EN1_SEGEN32_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN32_Pos)  
#define DISP_SEG_EN1_SEGEN32_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN32_ENABLE    						  	((INT32U)0x00000001)

/** @defgroup DISP_SEG_EN1_SEGEN33
  * @{
  */
#define DISP_SEG_EN1_SEGEN33_Pos		               			1                                             
#define DISP_SEG_EN1_SEGEN33_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN33_Pos)  
#define DISP_SEG_EN1_SEGEN33_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN33_ENABLE    						  	((INT32U)0x00000002)

/** @defgroup DISP_SEG_EN1_SEGEN34
  * @{
  */
#define DISP_SEG_EN1_SEGEN34_Pos		               			2                                             
#define DISP_SEG_EN1_SEGEN34_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN34_Pos)  
#define DISP_SEG_EN1_SEGEN34_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN34_ENABLE    						  	((INT32U)0x00000004)

/** @defgroup DISP_SEG_EN1_SEGEN35
  * @{
  */
#define DISP_SEG_EN1_SEGEN35_Pos		               			3                                             
#define DISP_SEG_EN1_SEGEN35_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN35_Pos)  
#define DISP_SEG_EN1_SEGEN35_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN35_ENABLE    						  	((INT32U)0x00000008)

/** @defgroup DISP_SEG_EN1_SEGEN36
  * @{
  */
#define DISP_SEG_EN1_SEGEN36_Pos		               			4                                            
#define DISP_SEG_EN1_SEGEN36_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN36_Pos)  
#define DISP_SEG_EN1_SEGEN36_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN36_ENABLE    						  	((INT32U)0x00000010)

/** @defgroup DISP_SEG_EN1_SEGEN37
  * @{
  */
#define DISP_SEG_EN1_SEGEN37_Pos		               			5                                            
#define DISP_SEG_EN1_SEGEN37_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN37_Pos)  
#define DISP_SEG_EN1_SEGEN37_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN37_ENABLE    						  	((INT32U)0x00000020)

/** @defgroup DISP_SEG_EN1_SEGEN38
  * @{
  */
#define DISP_SEG_EN1_SEGEN38_Pos		               			6                                            
#define DISP_SEG_EN1_SEGEN38_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN38_Pos)  
#define DISP_SEG_EN1_SEGEN38_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN38_ENABLE    						  	((INT32U)0x00000040)

/** @defgroup DISP_SEG_EN1_SEGEN39
  * @{
  */
#define DISP_SEG_EN1_SEGEN39_Pos		               			7                                            
#define DISP_SEG_EN1_SEGEN39_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN39_Pos)  
#define DISP_SEG_EN1_SEGEN39_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN39_ENABLE    						  	((INT32U)0x00000080)

/** @defgroup DISP_SEG_EN1_SEGEN40
  * @{
  */
#define DISP_SEG_EN1_SEGEN40_Pos		               			8                                            
#define DISP_SEG_EN1_SEGEN40_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN40_Pos)  
#define DISP_SEG_EN1_SEGEN40_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN40_ENABLE    						  	((INT32U)0x00000100)

/** @defgroup DISP_SEG_EN1_SEGEN41
  * @{
  */
#define DISP_SEG_EN1_SEGEN41_Pos		               			9                                            
#define DISP_SEG_EN1_SEGEN41_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN41_Pos)  
#define DISP_SEG_EN1_SEGEN41_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN41_ENABLE    						  	((INT32U)0x00000200)

/** @defgroup DISP_SEG_EN1_SEGEN42
  * @{
  */
#define DISP_SEG_EN1_SEGEN42_Pos		               			10                                            
#define DISP_SEG_EN1_SEGEN42_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN42_Pos)  
#define DISP_SEG_EN1_SEGEN42_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN42_ENABLE    						  	((INT32U)0x00000400)


/** @defgroup DISP_SEG_EN1_SEGEN43
  * @{
  */
#define DISP_SEG_EN1_SEGEN43_Pos		               			11                                            
#define DISP_SEG_EN1_SEGEN43_Msk               					(0x01UL << DISP_SEG_EN1_SEGEN43_Pos)  
#define DISP_SEG_EN1_SEGEN43_DISABLE    						((INT32U)0x00000000)
#define DISP_SEG_EN1_SEGEN43_ENABLE    						  	((INT32U)0x00000800)

#if 0
#define LCD_SLAVE_ADDR   (0x7C)  /*"9792 LCD设备从设备地址"*/


/*"执行soft reset,并使用内部震荡,会清除LCD的RAM(实践验证好像不会清除RAM)"*/
#define LCD_REST            (0xEA)  
/*"执行soft reset,使用内部震荡,不会清除LCD的RAM"*/
#define LCD_REST_NOCLR_RAM  (0xE8) 
#define LCD_POWER_NORMAL    (0xA0)  /*"正常功耗模式，line 刷新"*/
#define LCD_POWER_LOW     (0xBC)   /*"低功耗模式，line|frame刷新"*/
#define LCD_DIS_ON        (0xC8)   /*"开启显示"*/
#define LCD_DIS_OFF        (0xC0)
#define LCD_APCTRL        (0xFC)
#define LCD_APCTRL_PD        (0xF8)
#define LCD_EVRSET        (0xE0)
#define LCD_EVRSET_PD        (0xE0)

#define LCD_BASE_ADDR     (0x00) /*"定义液晶的显示缓存起始基址"*/
#endif

//static INT8U Lcd_PowerMode;/*"调用LCD init后,存储LCD的掉电模式配置字"*/

#define LCD_INTT_MODE   0
#define LCD_RUN_MODE    1
#if 0
static void Send_Cmd(INT8U Mode,INT8U *ShowBuffer)
{
    INT8U ResetMode;
    INT8U Count;
    INT8U TmpVal;
    
    IIC_Stop(IIC_ID_LCD);      /*"STOP"*/

    IIC_Start(IIC_ID_LCD);   /*"时序开始"*/

    TmpVal =  LCD_SLAVE_ADDR;   
    IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
    if(LCD_INTT_MODE ==  Mode)
    {
        ResetMode = LCD_REST;                    
    }
    else
    {
        ResetMode = LCD_REST_NOCLR_RAM;
    }
    IIC_SendChar(IIC_ID_LCD,&ResetMode,1); /*"发送不清RAM复位指令,防止静电打死恢复不过来"*/ 
    IIC_SendChar(IIC_ID_LCD,&Lcd_PowerMode,1);
    	if(LCD_INTT_MODE == Mode)
	{
	       TmpVal= LCD_DIS_OFF;
		IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
        
              TmpVal = LCD_REST_NOCLR_RAM;
		IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
	}
	else
	{

        	TmpVal= LCD_DIS_ON;
              IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
	}
    TmpVal = LCD_BASE_ADDR;
    IIC_SendChar(IIC_ID_LCD,&TmpVal,1);/*"设定RAM地址"*/ 

    if(ShowBuffer != NULL)
    {
        IIC_SendChar(IIC_ID_LCD,ShowBuffer,LCD_RAM_SIZE);
    }
    else
    {   
        TmpVal = 0;
        for(Count=0;Count < LCD_RAM_SIZE;Count++)
        {
            IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
        }
    }
    
    IIC_Stop(IIC_ID_LCD);      /*"STOP"*/
    if(LCD_INTT_MODE ==  Mode)
    {
         IIC_Start(IIC_ID_LCD);
         TmpVal = LCD_SLAVE_ADDR;
			 IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
		 if(Lcd_PowerMode == LCD_POWER_NORMAL)
		 {
		        TmpVal = LCD_APCTRL;
			 IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
                      TmpVal = LCD_EVRSET;
			 IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
		 }
		 else
		 {  
		        TmpVal = LCD_APCTRL_PD;
			 IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
                      TmpVal = LCD_EVRSET_PD;
			 IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
		 }
        TmpVal = LCD_DIS_ON;
        IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
        IIC_Stop(IIC_ID_LCD); 	 /*"STOP"*/
    }
	if(Lcd_PowerMode == LCD_POWER_NORMAL)
    {
        IIC_Start(IIC_ID_LCD);
            TmpVal = LCD_SLAVE_ADDR;
        IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
            TmpVal = LCD_APCTRL;
        IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
            TmpVal = LCD_EVRSET;
        IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
            TmpVal = LCD_DIS_ON;
        IIC_SendChar(IIC_ID_LCD,&TmpVal,1);
        IIC_Stop(IIC_ID_LCD); 	 /*"STOP"*/
    }




}
#endif

/*"********************************************************
    The  interface of LCD to the upper layer start
*********************************************************"*/

/*"****************************************************
Function: 	LCD_Init		
Description:1、  LCD初始化,包括电源控制、引脚状态，驱动芯片寄存器初始化
              考虑到使用IIC模式，需要有重启机制 ,
            2、因为每次液晶刷新都会复位，并开启显示，所以在初始化时可以不用开启显示    	                 					
Input:  mode：
        LCD_RUN               液晶正常工作模式
        LCD_LOWPOWER          液晶低功耗模式
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/	
#if 0
void LCD_Init(INT8U Mode)
{

    /*"初始化LCD的电源控制"*/
    GPIO_ConfigPin(LCD_PWR,OUTPUT,NORMAL_OUT);
    GPIO_OutputPin(LCD_PWR,HIGH);

    MCU_DelayUs(104);

    /*"初始化LCD的管脚状态"*/
    IIC_Init(IIC_ID_LCD);

    /*"初始化液晶的功耗模式"*/
    if(Mode == MCU_RUNMODE) /*"display ctrl set"*/
    {
        Lcd_PowerMode  = LCD_POWER_NORMAL;
    }
    else /*"LCD_LOWPOWER"*/
    {
        Lcd_PowerMode  = LCD_POWER_LOW;
    }

    #if 0 
    IIC_Stop(_IIC_ID_LCD);               /*"STOP"*/

    IIC_Start(_IIC_ID_LCD);              /*"START"*/
    IIC_SendChar(_IIC_ID_LCD,_LCD_SLAVE_ADDR);/*"SEND Slave addr"*/
    IIC_SendChar(_IIC_ID_LCD,_LCD_REST);/*software reset*/
    
    IIC_SendChar(_IIC_ID_LCD,Lcd_PowerMode);
    IIC_SendChar(_IIC_ID_LCD,_LCD_BASE_ADDR);
    for(Count=0;Count < _LCD_RAM_SIZE;Count++)
    {
        IIC_SendChar(_IIC_ID_LCD,0x00);/*"初始化时，清显示区"*/
    }

    IIC_Stop(_IIC_ID_LCD);      /*"STOP"*/
    #endif
    Send_Cmd(LCD_INTT_MODE,NULL);

}
#endif

void LCD_SEG_Enable_0(INT32U seg_enable_para)//LCD SEG端允许输出
{
	DISP->SEG_EN0 &= (~seg_enable_para);
	DISP->SEG_EN0 |= seg_enable_para;
}

void LCD_SEG_Enable_1(INT32U seg_enable_para)//LCD SEG端允许输出
{
	DISP->SEG_EN1 &= (~seg_enable_para);
	DISP->SEG_EN1 |= seg_enable_para;
}
#endif
#define  LCDSEGNUM  LCD_RAM_SIZE

#define  LCDADDR    0x7C       /*"  slave地址       "*/                         
#define  ICSET         0xF6       //CLOSE Display; soft reset;Frame mode;                                       
#define  ICSET1        0xF5			 //ON display;;Frame mode;
#define  ICSET2     0xF1			 //ON display;;Line mode;                                                                  
      
 
#define  DISCTL0     0xE2       //Normal   80Hz       
#define  DISCTL00    0xE0       //PS1      80Hz    
#define  DISCTL01    0xE1       //PS2      80Hz      
#define  DISCTL02    0xE3       //HP      80Hz      

                                                                      
#define  DISCTL1     0xE6       //Normal   71Hz  
#define  DISCTL10    0xE4       //PS1      71Hz     
#define  DISCTL11    0xE5       //PS2      71Hz     
#define  DISCTL12    0xE7       //HP       71Hz     

                                
#define  DISCTL2     0xEA       //Normal   64Hz                
#define  DISCTL20    0xE8       //PS1      64Hz             
#define  DISCTL21    0xE9       //PS2      64Hz             
#define  DISCTL22    0xEB       //HP       64Hz             

                            
#define  DISCTL3    0xEE       //Normal    50Hz             
#define  DISCTL30   0xEC       //PS1       50Hz                  
#define  DISCTL31   0xED       //PS2       50Hz                  
#define  DISCTL32   0xEF       //HP        50Hz                  


#define  EVRSET     0xc0       //VCC = 5V,Vping = 4.1V; 
#define  PDEVRSET   0xc2                                                                                        
    
#define  ADSET      0x00       /*"//ram起始地址"*/                         
                         
#define  APCTL      0xF8       //Normal                           
#define  APCTL1     0xFA       //ALL ON Display                                                                   
#define  APCTL2     0xF9       //ALL OFF Display   

void LCD_Init(INT8U Mode)
{
    INT8U cmd[6]={LCDADDR,ICSET,EVRSET,ICSET1,DISCTL20,ADSET};
    INT8U i;
    INT8U data=0;
	
    //GPIO_SetMulti(LCD_PWR,GPIO_MODE1);
    GPIO_ConfigPin(LCD_PWR, OUTPUT, NORMAL_OUT);
    GPIO_OutputPin(LCD_PWR, HIGH);
    MCU_DelayMs(5);
    IIC_Init(IIC_ID_LCD);
    IIC_Stop(IIC_ID_LCD);
    IIC_Start(IIC_ID_LCD);
    IIC_SendChar(IIC_ID_LCD,&cmd[0],6);
    for(i=0;i<LCDSEGNUM;i++)
    {
       IIC_SendChar(IIC_ID_LCD,&data,1);
    }
    IIC_Stop(IIC_ID_LCD);
    
#if 0
    INT8U i;
    INT32S tmpVal;


    /*"设置为模拟模式"*/
    for(i = 0;i<LCD_SEG_MAX;i++)
    {
            GPIO_SetMulti(gDisplayMultiSet[i],GPIO_Mode_ANA);
    }
    GPIO_SetMulti(0x00, GPIO_Mode_ANA);/*COM0*/
    GPIO_SetMulti(0x01, GPIO_Mode_ANA);/*COM1*/
    GPIO_SetMulti(0x02, GPIO_Mode_ANA);/*COM2*/
    GPIO_SetMulti(0x03, GPIO_Mode_ANA);/*COM3*/
    GPIO_SetMulti(0x04, GPIO_Mode_ANA);/*COM4*/
    GPIO_SetMulti(0x05, GPIO_Mode_ANA);/*COM5*/

    MCU_PeriphClkSet(LCD_num,ENABLE); /*"使能LCD时钟"*/
   

    DISP->DF = 42;  /*"32KHz  64刷新频率 A类"*/
    DISP->LCDSET = 0x00000011;  /*"1/3 bias  6com A类"*/
    DISP->LCDDRV = 0x00000001;

    if(MCU_RUNMODE == Mode)
    {
		DISP->LCDBIAS = 0x0000000E;
    }
    else 
    {
        tmpVal = Cal_VolVal(AC_ADCID_VDD);
        if(tmpVal > 4700)
        {
            DISP->LCDBIAS = 0x0000000E;
        }
        else if(tmpVal > 4000)
        {
            DISP->LCDBIAS = 0x0000000E;
        }
        else if(tmpVal > 3300)
        {
            DISP->LCDBIAS = 0x0000000E;
        }
        else if(tmpVal > 2800)
        {
            DISP->LCDBIAS = 0x0000000E;
        }
        else
        {
            DISP->LCDBIAS = 0x0000000E;
        }
    }


    LIB_MemSet(0xFF, (INT8U*)&DISP->DISPDATA0, 40); 
    DISP->COM_EN = 0x0000000F; /*"COM4和COM5的EN?"*/
    //DISP->SEG_EN0 = 0xC0E41E3F; /*"SEG0 -5 9-12 18 21-23 30-36 38 "*/
    //DISP->SEG_EN1 = 0x00000C5f;
    LCD_SEG_Enable_0(DISP_SEG_EN0_SEGEN1_Msk|DISP_SEG_EN0_SEGEN2_Msk|
                    DISP_SEG_EN0_SEGEN3_Msk|DISP_SEG_EN0_SEGEN4_Msk|
                    DISP_SEG_EN0_SEGEN5_Msk|DISP_SEG_EN0_SEGEN6_Msk|
                    DISP_SEG_EN0_SEGEN7_Msk|DISP_SEG_EN0_SEGEN8_Msk|
                    DISP_SEG_EN0_SEGEN9_Msk|DISP_SEG_EN0_SEGEN10_Msk|
                    DISP_SEG_EN0_SEGEN11_Msk|DISP_SEG_EN0_SEGEN12_Msk|
                    DISP_SEG_EN0_SEGEN13_Msk|DISP_SEG_EN0_SEGEN14_Msk|
                    DISP_SEG_EN0_SEGEN15_Msk|DISP_SEG_EN0_SEGEN16_Msk|
                    DISP_SEG_EN0_SEGEN17_Msk|DISP_SEG_EN0_SEGEN18_Msk|
                    DISP_SEG_EN0_SEGEN19_Msk|DISP_SEG_EN0_SEGEN20_Msk|
                    DISP_SEG_EN0_SEGEN21_Msk|DISP_SEG_EN0_SEGEN22_Msk|
                    DISP_SEG_EN0_SEGEN23_Msk);


    LCD_SEG_Enable_1(DISP_SEG_EN1_SEGEN34_Msk|DISP_SEG_EN1_SEGEN35_Msk|
                     DISP_SEG_EN1_SEGEN36_Msk|DISP_SEG_EN1_SEGEN37_Msk|
                     DISP_SEG_EN1_SEGEN38_Msk|DISP_SEG_EN1_SEGEN42_Msk|
                     DISP_SEG_EN1_SEGEN43_Msk);

    DISP->DISPCTRL = 0x000000c0;  
    //MCU_DelayMs(5);			/*"wait the boostting valtage stable"*/
#endif
}

/*"************************************************			
Function: 	LCD_Refresh		
Description:  显示信息数据送液晶驱动显示缓存            					
Input:  
    *showbuffer:发送到显示的数据缓存区指针
    len:        发送到显示的数据长度
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/
/*"***************************************************"*/
INT8U LCD_Refresh (INT8U *ShowBuffer)
{
#if 0
    INT8U i;
    
    if(ShowBuffer == NULL) /*"???????????"*/      
    {
        return FALSE;
    }
    else
    {

        for(i = 0; i < LCD_PIN_MAX; i++)
        {

           *((INT8U *)tmpData + i) =  ShowBuffer[i];
        }

        for(i = 0;i<10;i++)
        {
           *((INT32U *)LCD_REG_START +  i) =  tmpData[i];
        }
    }
    /*"RAM Write Sequence end"*/
    return TRUE;
    #endif
        INT8U cmd[5]={LCDADDR,ICSET1,DISCTL20,EVRSET,ADSET};
	
    IIC_Stop(IIC_ID_LCD);
    IIC_Start(IIC_ID_LCD);
    IIC_SendChar(IIC_ID_LCD,&cmd[0],5);
    IIC_SendChar(IIC_ID_LCD,ShowBuffer,35);    
    //IIC_SendChar(IIC_ID_LCD,ShowBuffer,LCD_RAM_SIZE);
    IIC_Stop(IIC_ID_LCD);

    return TRUE;
}


#if 0
INT8U LCD_Refresh     (INT8U *ShowBuffer)
                       
{

    if (ShowBuffer == NULL) /*"显示数据缓冲区为空指针"*/      
    {
        return FALSE;
    }
    #if 0
    /*"RAM Write Sequence start"*/
    IIC_Start(_IIC_ID_LCD);   /*"时序开始"*/
    IIC_SendChar(_IIC_ID_LCD,_LCD_SLAVE_ADDR);
    IIC_SendChar(_IIC_ID_LCD,_LCD_REST_NOCLR_RAM); /*"发送不清RAM复位指令,防止静电打死恢复不过来"*/ 
    IIC_SendChar(_IIC_ID_LCD,Lcd_PowerMode);
    IIC_SendChar(_IIC_ID_LCD,_LCD_DIS_ON);
    IIC_SendChar(_IIC_ID_LCD,_LCD_BASE_ADDR);/*"设定RAM地址"*/ 

    for(Count=0;Count < _LCD_RAM_SIZE;Count++) /*发送显示数据*/
    {
        IIC_SendChar(_IIC_ID_LCD,*(ShowBuffer + Count));
    }
    IIC_Stop(_IIC_ID_LCD);
    #endif
    Send_Cmd(LCD_RUN_MODE,ShowBuffer);

    /*"RAM Write Sequence end"*/

    return TRUE;
}
#endif
/*"************************************************			
Function: 	LCD_Close		
Description:  LCD关闭，包括电源控制、驱动芯片寄存器反初始化  
              用于关闭LCD并降低功耗              					
Input:  无
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/	
#if 0
void LCD_Close (void)
{
    /*"关闭LCD电源控制"*/
	GPIO_OutputPin(LCD_PWR,LOW);

}
#endif
void LCD_Close(void)
{
#if 0
    INT8U i;
	//LCD_Refresh(NULL);  
    DISP->COM_EN = 0; /*"COM4和COM5的EN?"*/
    DISP->SEG_EN0 = 0; /*"SEG0 -5 9-12 18 21-23 30-39"*/
    DISP->SEG_EN1 = 0;
    DISP->DISPCTRL = 0x00000080;
    //MCU_PeriphClkSet(LCD_num,DISABLE); /*"使能LCD时钟"*/
    for(i = 0;i<LCD_SEG_MAX;i++)
    {
        GPIO_ConfigPin(gDisplayMultiSet[i], OUTPUT, NORMAL_OUT);
        GPIO_OutputPin(gDisplayMultiSet[i], LOW);
    }

    GPIO_ConfigPin(0x00, OUTPUT, NORMAL_OUT);    
    GPIO_ConfigPin(0x01, OUTPUT, NORMAL_OUT);
    GPIO_ConfigPin(0x02, OUTPUT, NORMAL_OUT);
    GPIO_ConfigPin(0x03, OUTPUT, NORMAL_OUT);
    GPIO_ConfigPin(0x04, OUTPUT, NORMAL_OUT);
    GPIO_ConfigPin(0x05, OUTPUT, NORMAL_OUT);
    GPIO_OutputPin(0x00, LOW);
    GPIO_OutputPin(0x01, LOW);
    GPIO_OutputPin(0x02, LOW);
    GPIO_OutputPin(0x03, LOW);
    GPIO_OutputPin(0x04, LOW);
    GPIO_OutputPin(0x05, LOW);
    #endif
}






