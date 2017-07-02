#ifndef __ae_usart_H    
#define __ae_usart_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

extern int32_t Baud;                //´®¿Ú²¨ÌØÂÊ
	
void USART1_IO_Configuration(void);
void USART1_Configuration(void);
void USART1_NVIC_Configuration(void);
void USART1_Init(void);
void USART1_SendChar(uint16_t ch);
void myprintf1(char *str);

void USART2_IO_Configuration(void);
void USART2_Configuration(void);
void USART2_NVIC_Configuration(void);
void USART2_Init(void);
void USART2_SendChar(uint16_t ch);
void myprintf1(char *str);
	
void USART1_Configuration1(void);
	
#ifdef __cplusplus
}
#endif

#endif
