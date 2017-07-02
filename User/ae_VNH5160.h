#ifndef __ae_VNH5160_H    
#define __ae_VNH5160_H
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif


	
enum Command 
{
	breaks,
  forward,
  backward,
  left,
  right,
};
	
void VNH5160_Init(void);
void VNH5160_moveMotor(enum Command motor, enum Command direction, double speedRaw); 
void VNH5160_stopMotor(enum Command motor);
void VNH5160_CS_EN(void);
void VNH5160_CS_DIS(void); 	
	
	
#ifdef __cplusplus
}
#endif

#endif

