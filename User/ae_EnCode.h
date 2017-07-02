#ifndef __ae_EnCode_H    
#define __ae_EnCode_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t LeftCount;
extern uint16_t RightCount;

extern int16_t LeftDiff;
extern int16_t RightDiff;

void EnCode_Init(void);
void EnCode_Get(void);
	
#ifdef __cplusplus
}
#endif

#endif

