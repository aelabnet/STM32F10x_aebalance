#ifndef __ae_Control_H
#define __ae_Control_H
#include "stm32f10x.h"



#ifdef __cplusplus
extern "C" {
#endif

extern double CfgADP;      //PID比例参数
extern double CfgADI;       //PID积分参数
extern double CfgADD;       //PID微分参数
extern double CfgZeroAngle; //目标倾角 
extern double CfgControlAngleLimit; 
extern double CfgTurningLimit;

extern double CfgVDP;      //PID比例参数
extern double CfgVDI;     //PID积分参数
extern double CfgVDD;       //PID微分参数
	
extern double CfgTDP;      //PID比例参数
extern double CfgTDI;     //PID积分参数
extern double CfgTDD;       //PID微分参数	
	
extern double SppData1;
extern double SppData2;	
	
extern uint8_t SpeedControlCnt;
	
void Control(void);
void CalibMotor(void);

#ifdef __cplusplus
}
#endif

#endif  /*__MPU6050*/

