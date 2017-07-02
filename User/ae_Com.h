#ifndef __ae_COM_H
#define __ae_COM_H
#include "stm32f10x.h"
#include "stdbool.h"



#ifdef __cplusplus
extern "C" {
#endif
	
extern double CfgADP;                   //�Ƕ�PID��������
extern double CfgADI;                   //�Ƕ�PID���ֲ���
extern double CfgADD;                   //�Ƕ�PID΢�ֲ���

extern double CfgZeroAngle;               //С��Ŀ�����
extern double CfgControlAngleLimit;       //С��������
extern double CfgTurningLimit;            //С�����ת���ٶ�

extern double CfgVDP;                     //�ٶ�PID��������
extern double CfgVDI;                     //�ٶ�PID���ֲ���
extern double CfgVDD;                     //�ٶ�PID΢�ֲ���
extern uint32_t CfgVL;                      //�ٶȻ�������ֵ

extern double CfgTDP;                     //ת��PID��������
extern double CfgTDI;                     //ת��PID���ֲ���
extern double CfgTDD;                     //ת��PID΢�ֲ���


extern double SppData1;                    //С��ת�����              
extern double SppData2;                    //С���ٶȲ���

extern int32_t FreqPWM;                    //���PWMƵ��

extern double AngleXacc,AngleXgyro,AngleComp,AngleKalman;


extern uint16_t SamplePeriod;             //�������� n*0.1ms     
extern double Coefficient;                //�����˲�����ϵ��

extern double Qangle;                     //Kalman����
extern double Qbias;
extern double Rmeasure;

extern bool FlagFlagMotor;                 //���ֹͣ��־λ
extern bool FlagCalibGyro;                 //������У׼��־λ
extern bool FlagCalibMotor;                //���У׼��־λ
extern bool FlagReset;                     //�ָ��������ñ�־λ
extern bool FlagSave;                      //�������ݱ�׼λ
extern bool FlagNewSample;                 //�µĲ������ڱ�־λ
extern bool FlagNewPWM;                    //�µĵ��PWM��־λ

extern double Voltage;                     //��ص�ѹ


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

