/*******************************************************************************
** 文件名: 		ae_Com.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5.20
** 作者: 		  aelab
** 生成日期: 	2016-12-16
** 功能:		  和aeMotion for aebalance通信
** 相关文件:	ae_usart.c
** 修改日志：	2016-12-16   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "stdlib.h" 
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "ae_usart.h"



/* 变量 ----------------------------------------------------------------------*/
bool SendAnglePID = false;          //角度PID发送标志位
bool SendVelocityPID = false;       //速度PID发送标志位
bool SendTurnPID = false;           //转向PID发送标志位
bool SendComp = false;              //互补滤波参数发送标志位
bool SendKalman = false;            //Kalman滤波参数发送标志位
bool SendInfo  = false;             //小车信息发送标志位
bool SendSetting = false;           //小车设置发送标志位
bool SendIMU = false;               //小车姿态发送标志位
bool SendPWM = false;               //小车电机PWM发送标志位

bool FlagStopMotor = false;                 //电机停止标志位
bool FlagCalibGyro = false;                 //陀螺仪校准标志位
bool FlagCalibMotor = false;                //电机校准标志位
bool FlagReset = false;                     //恢复出厂设置标志位
bool FlagSave = false;                      //保存数据标志位
bool FlagNewSample = false;                 //新的采样周期标志位
bool FlagNewPWM = false;                    //新的电机PWM标志位


double CfgADP;                  //角度PID比例参数
double CfgADI;                      //角度PID积分参数
double CfgADD;                  //角度PID微分参数

double CfgZeroAngle;              //小车目标倾角
double CfgControlAngleLimit;       //小车最大倾角
double CfgTurningLimit;            //小车最大转弯速度

double CfgVDP;                     //速度PID比例参数
double CfgVDI;                  //速度PID积分参数
double CfgVDD;                      //速度PID微分参数
uint32_t CfgVL;                   //速度积分限制值

double CfgTDP;                      //转向PID比例参数
double CfgTDI;                      //转向PID积分参数
double CfgTDD;                      //转向PID微分参数

int32_t FreqPWM;                //电机PWM频率

uint16_t SamplePeriod;             //采样周期 n*0.1ms     
double Coefficient;            //互补滤波比例系数

double Qangle;                          //Kalman参数
double Qbias;
double Rmeasure;

double data1[20];
uint32_t data2[5];


double AngleXacc,AngleXgyro,AngleComp,AngleKalman;        //4种角度计算结果

double SppData1 = 0;                    //小车转向参数              
double SppData2 = 0;                    //小车速度参数

char Firmware[] = "aebalance V1.0";     //硬件版本
double Voltage = -1;                    //电池电压值

/*******************************************************************************
  * @函数名称	  GetValues
  * @函数说明   对输入的字符串进行解析判断
  * @输入参数   *input：输入字符串
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void GetValues(char *input) 
{
	/* 上位机请求基本操作 */
	if (input[0] == 'A' && input[1] == ';')          //停止电机
		FlagStopMotor = true;
  else if (input[0] == 'A' && input[1] == 'C')     //陀螺仪&加速度计校准
		FlagCalibGyro = true;	
  else if (input[0] == 'M' && input[1] == 'C')     // 电机校准
    FlagCalibMotor = true;

  /* 上位机请求获得aebalance小车参数和信息 */
  else if (input[0] == 'G') {                     
    if (input[1] == 'A')                           //上位机请求获得角度PID值
      SendAnglePID = true;
    else if (input[1] == 'V')                      //上位机请求获得速度PID值
      SendVelocityPID = true;
		else if (input[1] == 'R')                      //上位机请求获得转向PID值
      SendTurnPID = true;
    else if (input[1] == 'C')                      //上位机请求获得互补滤波参数值
      SendComp = true;
		else if (input[1] == 'K')                      //上位机请求获得Kalman滤波参数值
      SendKalman = true;
    else if (input[1] == 'I')                      //上位机请求获得小车信息
      SendInfo = true;
    else if (input[1] == 'S')                      //上位机请求获得小车设置参数
      SendSetting = true;
		else if (input[1] == 'W')                      //上位机请求获得小车电机PWM参数
      SendPWM = true;
  }  
	
  /* 上位机设置aebalance小车参数 */
  else if (input[0] == 'S') {                      
    /*上位机设置aebalance 角度PID参数 */
    if (input[1] == 'P') {                         //设置小车的角度P参数
      strtok(input, ","); 
      CfgADP = atof(strtok(NULL, ";"));
    } else if (input[1] == 'I') {                  //设置小车的角度I参数
      strtok(input, ","); 
      CfgADI = atof(strtok(NULL, ";"));
    } else if (input[1] == 'D') {                  //设置小车的角度D参数
      strtok(input, ","); 
      CfgADD = atof(strtok(NULL, ";"));
    } 
				
		/*上位机设置aebalance 目标角度 */
		else if (input[1] == 'T') { 
      strtok(input, ","); 
      CfgZeroAngle = atof(strtok(NULL, ";"));
    } 

		/*上位机设置aebalance 速度PID参数 */		
		else if (input[1] == 'V') { 
      strtok(input, ","); 
      CfgVDP = atof(strtok(NULL, ","));            //设置小车的速度P参数
      CfgVDI = atof(strtok(NULL, ","));            //设置小车的速度I参数
      CfgVDD = atof(strtok(NULL, ","));            //设置小车的速度D参数
			CfgVL = atof(strtok(NULL, ";"));             //设置小车速度积分限制
    } 
		
		/*上位机设置aebalance 转向PID参数 */		
		else if (input[1] == 'R') { 
      strtok(input, ","); 
      CfgTDP = atof(strtok(NULL, ","));            //设置小车的转向P参数
      CfgTDI = atof(strtok(NULL, ","));            //设置小车的转向I参数
      CfgTDD = atof(strtok(NULL, ";"));            //设置小车的转向D参数
    } 		
		
		/*上位机设置aebalance 互补滤波参数 */	
		else if (input[1] == 'C') { 
      strtok(input, ","); 
			SamplePeriod = atof(strtok(NULL, ","));      //设置小车的采样周期参数
      Coefficient = atof(strtok(NULL, ";"));       //设置小车的互补滤波参数
			FlagNewSample = true;  
		} 

		/*上位机设置aebalance Kalman滤波参数 */	
		else if (input[1] == 'K') { 
      strtok(input, ","); 
			Qangle = atof(strtok(NULL, ","));            //设置小车的kalman滤波的Qangle参数
      Qbias = atof(strtok(NULL, ","));             //设置小车的kalman滤波的Qbias参数
			Rmeasure = atof(strtok(NULL, ","));          //设置小车的kalman滤波的Rmeasure参数
			SamplePeriod = atof(strtok(NULL, ";"));      //设置小车的采样周期参数
			FlagNewSample = true;  
		} 
		
		/*上位机设置aebalance 电机PWM参数 */	
		else if (input[1] == 'W') { 
      strtok(input, ","); 
			FreqPWM = atof(strtok(NULL, ";"));           
			FlagNewPWM = true;

		} 		
		
		/*上位机设置aebalance 控制角度最大值 */	
		else if (input[1] == 'A') { 
      strtok(input, ","); 
      CfgControlAngleLimit = atoi(strtok(NULL, ";"));
    } 
		
		
		/*上位机设置aebalance 转向最大值 */	
		else if (input[1] == 'U') { 
      strtok(input, ","); 
      CfgTurningLimit = atoi(strtok(NULL, ";"));
    }
  }
	
	
  /* 上位机设置aebalance是否发送姿态数据 */
  else if (input[0] == 'I') { 
    if (input[1] == 'B')                           //发送姿态使能
      SendIMU = true; 
    else if (input[1] == 'S')                      //发送姿态关闭
      SendIMU = false; 
  }
  /* 上位机请求恢复默认设置 */
  else if (input[0] == 'F') { 
    if (input[1] == 'R')                           //恢复默认设置
			FlagReset = true;
  }	
	  /* 上位机请求保存所有参数 */
  else if (input[0] == 'D') { 
    if (input[1] == 'S')                           //保存所有参数
			FlagSave = true;
  }	
	/* 上位机发送小车运行指令 */	
  else if (input[0] == 'C') { 
    if (input[1] == 'S'){                          //接收上位机停车指令
      SppData1 = 0;
		  SppData2 = 0;
		}
    else if (input[1] == 'J') {                    //接收上位机摇杆指令
      strtok(input, ","); 
      SppData1 = atof(strtok(NULL, ","));          // x-axis
      SppData2 = atof(strtok(NULL, ";"));          // y-axis
    }
    else if (input[1] == 'M') {                    //接收上位机倾斜遥控指令
      strtok(input, ","); 
      SppData1 = atof(strtok(NULL, ","));          // Pitch
      SppData2 = atof(strtok(NULL, ";"));          // Roll
    }
	}
	
	
	
}



/*******************************************************************************
  * @函数名称	  SendValues()
  * @函数说明   向上位机发送信息
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void SendValues(void) 
{
	
	if (SendAnglePID){                               //向上位机发送角度PID参数，目标角度
    printf("A,%.1f,%.1f,%.2f,%0.1f;",CfgADP,CfgADI,CfgADD,CfgZeroAngle);	
		SendAnglePID = false;	
	}
	else if(SendVelocityPID){                        //向上位机发送速度PID参数
		printf("V,%.1f,%.3f,%.1f,%d;",CfgVDP,CfgVDI,CfgVDD,CfgVL);	
		SendVelocityPID = false;
	}
	else if(SendTurnPID){                            //向上位机发送速度PID参数
		printf("R,%.4f,%.4f,%.4f;",CfgTDP,CfgTDI,CfgTDD);	
		SendTurnPID = false;
	}
	else if(SendComp){                               //向上位机发送周期，互补滤波参数
		printf("C,%d,%.4f;",SamplePeriod,Coefficient);	
		SendComp = false;
	}
		else if(SendKalman){                           //向上位机发送周期，Kalman滤波参数
		printf("K,%.5f,%.5f,%.5f,%d;",Qangle,Qbias,Rmeasure,SamplePeriod);	
		SendKalman = false;
	}
	else if(SendInfo){                               //向上位机发送小车信息,电压
		printf("I,%s,%.2f;",Firmware,Voltage);	
		SendInfo = false;
	}
	else if(SendSetting){                            //向上位机发送小车设置参数
		printf("S,%.1f,%.1f;",CfgControlAngleLimit,CfgTurningLimit);
		SendSetting = false;
	}	
	else if(SendPWM){                                //向上位机发送小车电机PWM
		printf("W,%d;",FreqPWM);
		SendPWM = false;
	}			

	if(SendIMU)                                       //向上位机发送小车姿态
	{
		printf("P,%.1f,%.1f,%.1f,%.1f;",AngleXacc,AngleXgyro,AngleComp,AngleKalman);

	}
	
	
}

/*******************************************************************************
  * @函数名称	  ListValues
  * @函数说明   数据队列化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void ListValues(void) 
{
	data1[0] = CfgADP;
	data1[1] = CfgADI;
	data1[2] = CfgADD;
	
	data1[3] = CfgZeroAngle;
	data1[4] = CfgControlAngleLimit;
	data1[5] = CfgTurningLimit;	
	
	data1[6] = CfgVDP;
	data1[7] = CfgVDI;
	data1[8] = CfgVDD;	
	data2[0] = CfgVL;

	data1[9] = CfgTDP;
	data1[10] = CfgTDI;
	data1[11] = CfgTDD;	

	data2[1] = FreqPWM;

	data2[2] = SamplePeriod;

	data1[12] = Coefficient;	
	
	data1[13] = Qangle;	
  data1[14] = Qbias;	
	data1[15] = Rmeasure;	

}

 
/*******************************************************************************
  * @函数名称	  DeListValues
  * @函数说明   数据去队列化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void DeListValues(void) 
{
	CfgADP = data1[0];
	CfgADI = data1[1];
	CfgADD = data1[2];
	
	CfgZeroAngle = data1[3];
	CfgControlAngleLimit = data1[4];
	CfgTurningLimit = data1[5];	
	
	CfgVDP = data1[6];
	CfgVDI = data1[7];
	CfgVDD = data1[8];	
	CfgVL = data2[0];

	CfgTDP = data1[9];
	CfgTDI = data1[10];
	CfgTDD = data1[11];	

	FreqPWM = data2[1];

	SamplePeriod = data2[2];

	Coefficient = data1[12];	
	
	Qangle = data1[13];	
  Qbias = data1[14];	
	Rmeasure = data1[15];	

}

/*******************************************************************************
  * @函数名称	  DefaultValues
  * @函数说明   恢复默认值
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void DefaultValues(void) 
{
	CfgADP = -10;                  //角度PID比例参数
	CfgADI = 0;                      //角度PID积分参数
	CfgADD = -0.12;                  //角度PID微分参数

	CfgZeroAngle = -15;              //小车目标倾角
	CfgControlAngleLimit = 15;       //小车最大倾角
	CfgTurningLimit = 40;            //小车最大转弯速度

	CfgVDP = -1;//-3;                     //速度PID比例参数
	CfgVDI = -0.09;//-0.25;                  //速度PID积分参数
	CfgVDD = 0;                      //速度PID微分参数
	CfgVL = 300;                   //速度积分限制值

	CfgTDP = 0;                      //转向PID比例参数
	CfgTDI = 0;                      //转向PID积分参数
	CfgTDD = 0;                      //转向PID微分参数

	FreqPWM = 20000;                //电机PWM频率

	SamplePeriod = 30;             //采样周期 n*0.1ms     
	Coefficient = 0.9984;            //互补滤波比例系数

	Qangle = 0;                          //Kalman参数
	Qbias = 0;
	Rmeasure = 0;
	
	ListValues(); 


}
