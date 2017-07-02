#ifndef __ae_COM_H
#define __ae_COM_H
#include "stm32f10x.h"
#include "stdbool.h"



#ifdef __cplusplus
extern "C" {
#endif
	
extern double CfgADP;                   //角度PID比例参数
extern double CfgADI;                   //角度PID积分参数
extern double CfgADD;                   //角度PID微分参数

extern double CfgZeroAngle;               //小车目标倾角
extern double CfgControlAngleLimit;       //小车最大倾角
extern double CfgTurningLimit;            //小车最大转弯速度

extern double CfgVDP;                     //速度PID比例参数
extern double CfgVDI;                     //速度PID积分参数
extern double CfgVDD;                     //速度PID微分参数
extern uint32_t CfgVL;                      //速度积分限制值

extern double CfgTDP;                     //转向PID比例参数
extern double CfgTDI;                     //转向PID积分参数
extern double CfgTDD;                     //转向PID微分参数


extern double SppData1;                    //小车转向参数              
extern double SppData2;                    //小车速度参数

extern int32_t FreqPWM;                    //电机PWM频率

extern double AngleXacc,AngleXgyro,AngleComp,AngleKalman;


extern uint16_t SamplePeriod;             //采样周期 n*0.1ms     
extern double Coefficient;                //互补滤波比例系数

extern double Qangle;                     //Kalman参数
extern double Qbias;
extern double Rmeasure;

extern bool FlagFlagMotor;                 //电机停止标志位
extern bool FlagCalibGyro;                 //陀螺仪校准标志位
extern bool FlagCalibMotor;                //电机校准标志位
extern bool FlagReset;                     //恢复出厂设置标志位
extern bool FlagSave;                      //保存数据标准位
extern bool FlagNewSample;                 //新的采样周期标志位
extern bool FlagNewPWM;                    //新的电机PWM标志位

extern double Voltage;                     //电池电压


extern double data1[20];
extern uint32_t data2[5];

void GetValues(char *input); 	
void SendValues(void);
void ListValues(void);
void DeListValues(void);
void DefaultValues(void);
	

#ifdef __cplusplus
}
#endif

#endif  /*__MPU6050*/

