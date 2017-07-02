/*******************************************************************************
** 文件名: 		ae_Angle.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5.20
** 作者: 		  aelab
** 生成日期: 	2015-07-03
** 功能:		  MPU6050直接获得角度值
** 相关文件:	ae_I2C.c
							ae_usart.c
							SysTickDelay.c
							ae_mpu6050.c
** 修改日志：	2015-07-03   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "math.h"
#include "SysTickDelay.h"
#include "ae_mpu6050.h"
#include "ae_Com.h"


/* 宏定义 --------------------------------------------------------------------*/
#define RAD_TO_DEG 57.29578049        //弧度转角度
/* 变量 ----------------------------------------------------------------------*/

uint16_t CalibAngleCnt = 0;
double XgyroRateCalibBuffer = 0;
double XgyroRateZero = 0;

double XgyroRate = 0;



double XaccAngle,XaccAngle1;	
float			E1 = 0;	//滤波算法中间变量
float			E2 = 0;	//滤波算法中间变量
double    Car_Angle;//滤波算法中间变量
	
/*******************************************************************************
  * @函数名称	  TIM1_Configuration
  * @函数说明   设置TIME1  20*0.1ms
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void TIM1_Configuration(void)
{
	/* 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);	
	
	
	/* ---------------------------------------------------------------
	TIM2LK 即PCLK1=36MHz
	TIM2CLK = 36 MHz, Prescaler = 72, TIM2 counter clock = 500K,即改变一次为500K,周期就为0.001ms
	--------------------------------------------------------------- */	
	/* 
	*  	计数重载值为50000   Max=65535
	*  	预分频值为(71 + 1 = 72)
	*  	时钟分割0
	*  	向上计数模式
	*/
	TIM_TimeBaseStructure.TIM_Period = SamplePeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM1 , &TIM_TimeBaseStructure);
	/* 
	*  	清除溢出中断标志
	*  	使能 TIM1 中断 
	*  	开启时钟
	*/
  TIM_ClearFlag(TIM1,TIM_FLAG_Update);  
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE);   
}

/*******************************************************************************
  * @函数名称	  TIM1_NVIC_Configuration
  * @函数说明   设置TIM1中断NVIC参数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/

void TIM1_NVIC_Configuration(void)
{								
	/* 定义 NVIC 初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif结构的作用是根据预编译条件决定中断向量表起始地址*/   
#ifdef  VECT_TAB_RAM  
  		/* 中断向量表起始地址从 0x20000000 开始 */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* 中断向量表起始地址从 0x80000000 开始 */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* 选择优先级分组0 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* 开启 TIM1 中断, 0级先占优先级，0级后占优先级 */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/*******************************************************************************
  * @函数名称	  Cycle_Init
  * @函数说明   初始化采样周期
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Cycle_Init(void)
{
	TIM1_Configuration();
	TIM1_NVIC_Configuration();
}
/*******************************************************************************
  * @函数名称	  Calib_Gyro
  * @函数说明   角速度补偿
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Calib_Gyro(void)
{
		if(CalibAngleCnt < 100)
		{
			CalibAngleCnt++;
			XgyroRateCalibBuffer += XgyroRate;			
		}
		if(CalibAngleCnt == 100)
		{
			XgyroRateZero = XgyroRateCalibBuffer/CalibAngleCnt;
			CalibAngleCnt++;
		}	
}

/*******************************************************************************
  * @函数名称	  Angle
  * @函数说明   直接通过MPU6050获得角度
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Angle(void)
{

		int16_t yacc;
		int16_t zacc;
		int16_t xgyro;
		uint16_t temp1[3];
		uint16_t temp2[3];
		
		MPU6050ReadAcc(temp1);
		MPU6050ReadGyro(temp2);
	
		yacc = temp1[1];
		zacc = temp1[2];		
		xgyro = temp2[0];

		XgyroRate = (double)xgyro / 16.4f;     //读到的值转化为角度
		
		AngleXacc = atan2((double)yacc , (double)zacc) * RAD_TO_DEG;		                 //通过加速度获得倾斜角度
    AngleXgyro = AngleXgyro + (XgyroRate - XgyroRateZero)* SamplePeriod * 0.0001 ;   //通过角速度获得倾斜角度
		if(AngleXgyro < -180) AngleXgyro += 360;
	  if(AngleXgyro > 180) AngleXgyro -= 360;
	
			
}




