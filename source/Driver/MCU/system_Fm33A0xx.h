/**************************************************************************//**
 * @file     system_FM32L0XX.h
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Header File for
 *           Device FM32L0XX
 * @version  V1.00
 * @date     11. November 2016
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef SYSTEM_FM32L0XX_H
#define SYSTEM_FM32L0XX_H

#ifdef __cplusplus
extern "C" {
#endif

  
  
  
  
#include <stdint.h>
#include <stdio.h>
#include "FM33A0XX.h"


extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);
#define __SYSTEM_CLOCK    (8000000)
#define DELAY_US      (__SYSTEM_CLOCK/1000000)
#define DELAY_MS      (__SYSTEM_CLOCK/1000)


__INLINE uint32_t SystemTicksInterval(uint32_t LastTick)
{
  return (LastTick - SysTick->VAL)&0xFFFFFF;
}
__INLINE uint32_t GetSystemTicks()
{
  return SysTick->VAL;
}
#define Do_DelayStart()  { \
                        uint32_t LastTick = GetSystemTicks();   do {
                        
#define While_DelayMsEnd(Count) }while(SystemTicksInterval(LastTick)<DELAY_MS*Count); \
                      }
                      
#define While_DelayUsEnd(Count) }while(SystemTicksInterval(LastTick)<DELAY_US*Count); \
                      }            

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_MVCM3_H */
