#ifndef __ae_LED_H    
#define __ae_LED_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

	
	
//�궨��
#define LED0_ON 		GPIO_ResetBits(GPIOA,GPIO_Pin_4)  	//����LED0
#define LED0_OFF		GPIO_SetBits(GPIOA,GPIO_Pin_4)		  //Ϩ��LED0
#define LED1_ON 		GPIO_ResetBits(GPIOA,GPIO_Pin_5)	  //����LED1
#define LED1_OFF		GPIO_SetBits(GPIOA,GPIO_Pin_5)		  //Ϩ��LED1	
	
	
void GPIO_OUT_Configuration(void);
void GPIO_IN_Configuration(void);
void LED_Init(void);


	
	
	
#ifdef __cplusplus
}
#endif

#endif

