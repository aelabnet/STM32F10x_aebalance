#ifndef __ae_Angle_H
#define __ae_Angle_H
#include "stm32f10x.h"



#ifdef __cplusplus
extern "C" {
#endif


extern double XgyroRate;
extern double XgyroRateZero;
	

void Cycle_Init(void);
void Angle(void);
void Calib_Gyro(void);

#ifdef __cplusplus
}
#endif

#endif  /*__MPU6050*/

