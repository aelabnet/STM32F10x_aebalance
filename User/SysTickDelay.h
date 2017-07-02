
#ifndef __SysTickDelay_H
#define __SysTickDelay_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/* SysTick constants */
#define SYSTICK_ENABLE              0                                          /* Config-Bit to start or stop the SysTick Timer                         */
#define SYSTICK_TICKINT             1                                          /* Config-Bit to enable or disable the SysTick interrupt                 */
#define SYSTICK_CLKSOURCE           2                                          /* Clocksource has the offset 2 in SysTick Control and Status Register   */
#define SYSTICK_MAXCOUNT       ((1<<24) -1)                                    /* SysTick MaxCount                                                      */


void SysTick_Initaize(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
	
#ifdef __cplusplus
}
#endif

#endif 
  

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
