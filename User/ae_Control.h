#ifndef __ae_Control_H
#define __ae_Control_H
#include "stm32f10x.h"



#ifdef __cplusplus
extern "C" {
#endif

extern double CfgADP;      //PID��������
extern double CfgADI;       //PID���ֲ���
extern double CfgADD;       //PID΢�ֲ���
extern double CfgZeroAngle; //Ŀ����� 
extern double CfgControlAngleLimit; 
extern double CfgTurningLimit;

extern double CfgVDP;      //PID��������
extern double CfgVDI;     //PID���ֲ���
extern double CfgVDD;       //PID΢�ֲ���
	
extern double CfgTDP;      //PID��������
extern double CfgTDI;     //PID���ֲ���
extern double CfgTDD;       //PID΢�ֲ���	
	
extern double SppData1;
extern double SppData2;	
	
extern uint8_t SpeedControlCnt;
	
void Control(void);
void CalibMotor(void);

#ifdef __cplusplus
}
#endif

#endif  /*__MPU6050*/

