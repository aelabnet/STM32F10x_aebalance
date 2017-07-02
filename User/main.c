/*******************************************************************************
** 文件名: 		mian.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5.20
** 作者: 		  aelab
** 生成日期: 	2015-04-10
** 功能:		  模版程序（用户可以在这里简单说明工程的功能）
** 相关文件:	无
** 修改日志：	2015-04-10   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "ae_usart.h"
#include "SysTickDelay.h"
#include "ae_mpu6050.h"
#include "ae_Angle.h"
#include "ae_Com.h"
#include "ae_Comp.h"
#include "ae_Control.h"
#include "ae_VNH5160.h"
#include "ae_Encode.h"
#include "ae_Flash.h"
#include "ae_GPIO.h"
#include "ae_ADC.h"

/* 变量 ----------------------------------------------------------------------*/
uint8_t RxBuff[50] = {0};
uint8_t RxCnt = 0;

uint32_t LowVolCnt = 0;
uint32_t LedCnt = 0;
uint8_t LowVolFlag = 0;


/*******************************************************************************
  * @函数名称	  main
  * @函数说明   主函数 
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
int main(void)
{
	
	
	SysTick_Initaize(); //初始化滴答时钟，作为延时时钟
	delay_ms(3000);     //上电延时
		
	Reader(data1, data2); //从Flash中读取小车参数
	DeListValues();       //根据data1 data2设置小车参数
	
	LED_Init();
		
	MPU6050_Init();     //初始化MPU6050
	Cycle_Init();       //初始化MPU6050采样周期
	USART2_Init();      //初始化串口2
	VNH5160_Init();
	EnCode_Init();
	VoltageADC_Init();
	
	
	printf("AT+BAUD9");
	Baud = 230400;
	USART2_Init();      //初始化串口2
	
	delay_ms(1000);     //上电延时
   
    while (1)
    {

			
    }
}


/*******************************************************************************
  * @函数名称	  TIM1_UP_IRQHandler
  * @函数说明   时钟1中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void TIM1_UP_IRQHandler(void) 
{
	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清空中断标志位
	
		/* 每10*采样周期获得电机编码器编码值和转速 */
		SpeedControlCnt++;
		if(SpeedControlCnt > 9)
		{
			Voltage = 4.3 * 3.2 * ADC_GetConversionValue(ADC1) / 0x0FFF;
			EnCode_Get();	
			SpeedControlCnt = 0;
		}
		
		/* 对电池电压进行判断，低压时进行LED闪烁警告 */
		if(Voltage < 11)
		{
			LowVolCnt++;
		}
		if(LowVolCnt > 500)
		{
			LowVolFlag = 1;
		}
		if(LowVolFlag == 1)
		{
			LedCnt ++;
			if(LedCnt == 100)
				GPIO_SetBits(GPIOA, GPIO_Pin_12); 
			if(LedCnt > 200)
			{
				GPIO_ResetBits(GPIOA, GPIO_Pin_12); 
				LedCnt = 0;
			}
		}
		
		
    Angle();           //进行角度计算
		Comp_Angle();      //进行互补滤波计算		
		Control();				 //控制小车
		SendValues();      //发送相关结果
		
		if(FlagSave)       //数据储存
		{
			FlagSave = false;
			ListValues();
			Storage(data1,data2);
		}
		if(FlagReset)      //输出恢复默认设置
		{
			FlagReset = false;
			DefaultValues();
			Storage(data1,data2);
		}
		if(FlagNewSample)  //新的采样周期
		{
			FlagNewSample = false;
			Cycle_Init();       //初始化MPU6050采样周期
		}
		if(FlagNewPWM)       //新的电机PWM频率
		{
			FlagNewPWM = false;
			VNH5160_Init();
		}		

		
	}
}

/*******************************************************************************
  * @函数名称	  USART1_IRQHandler
  * @函数说明   串口1中断函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{		

		RxBuff[RxCnt++] = USART_ReceiveData(USART1);  //读取接收到的数据
		
		if(RxBuff[RxCnt-1] == ';')
		{			
			RxCnt = 0;
			GetValues((char*)RxBuff);             //判断接收到的数据
		}	
	 
	}
}


/*******************************************************************************
  * @函数名称	  USART2_IRQHandler
  * @函数说明   串口2中断函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
	{		

		RxBuff[RxCnt++] = USART_ReceiveData(USART2);  //读取接收到的数据
		
		if(RxBuff[RxCnt-1] == ';')
		{			
			RxCnt = 0;
			GetValues((char*)RxBuff);             //判断接收到的数据
		}	
	 
	}
}


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
  * @函数名称	assert_failed
  * @函数说明   报告在检查参数发生错误时的源文件名和错误行数
  * @输入参数   file: 源文件名
  				line: 错误所在行数 
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    /* 无限循环 */
    while (1)
    {
    }
}
#endif

/***********************************文件结束***********************************/
