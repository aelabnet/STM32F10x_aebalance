#ifndef __ae_Flash_H    
#define __ae_Flash_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

void Double_FlashWrite(double data, uint16_t num);
double Double_FlashRead(uint16_t num);
void uint32_FlashWrite(uint32_t data, uint16_t num);
uint32_t uint32_FlashRead(uint16_t num);
void Storage(double *data1, uint32_t *data2);	
void Reader(double *data1, uint32_t *data2);
	


	
	
	
#ifdef __cplusplus
}
#endif

#endif
