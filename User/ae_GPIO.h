#ifndef __ae_LED_H    
#define __ae_LED_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

	
	
//宏定义
#define LED0_ON 		GPIO_ResetBits(GPIOA,GPIO_Pin_4)  	//点亮LED0
#define LED0_OFF		GPIO_SetBits(GPIOA,GPIO_Pin_4)		  //熄灭LED0
#define LED1_ON 		GPIO_ResetBits(GPIOA,GPIO_Pin_5)	  //点亮LED1
#define LED1_OFF		GPIO_SetBits(GPIOA,GPIO_Pin_5)		  //熄灭LED1	
	
	
void GPIO_OUT_Configuration(void);
void GPIO_IN_Configuration(void);
void LED_Init(void);


	
	
	
#ifdef __cplusplus
}
#endif

#endif

