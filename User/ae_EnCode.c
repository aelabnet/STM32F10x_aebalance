/*******************************************************************************
** 文件名: 		ae_EnCode.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2016-10-7
** 功能:		  通过编码器获得转速
** 相关文件:	ae_EnCode.h
** 修改日志：	2016-10-7   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"

/* 类型声明 ------------------------------------------------------------------*/


/* 宏定义 --------------------------------------------------------------------*/

/* 变量 ----------------------------------------------------------------------*/
uint16_t LeftCount;
uint16_t RightCount;
uint16_t LastLeftCount;
uint16_t LastRightCount;

int16_t LeftDiff;
int16_t RightDiff;

/* 函数声明 ------------------------------------------------------------------*/
/* 函数功能 ------------------------------------------------------------------*/


/*******************************************************************************
  * @函数名称	  EnCode_GPIO_Configuration
  * @函数说明   设置各GPIO端口功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	                 //定义GPIO结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能复用功能时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PA6,PA7为电机1A,1B输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PB6,PA7为电机2A,2B输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
}


/*******************************************************************************
  * @函数名称	  EnCode_TIM3_Configuration
  * @函数说明   配置时钟
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_TIM3_Configuration(void)
{

	TIM_ICInitTypeDef TIM_ICInitStructure;                 //定义TIM结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 //使能TIM3时钟
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //配置编码模式

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;                  //配置滤波方式           
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
  TIM3->CNT = 0;                                         //初始化计数
	TIM_Cmd(TIM3,ENABLE);                                  //开启时钟

}




/*******************************************************************************
  * @函数名称	  EnCode_TIM4_Configuration
  * @函数说明   配置时钟
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_TIM4_Configuration(void)
{

	TIM_ICInitTypeDef TIM_ICInitStructure;                //定义TIM结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能TIM4时钟
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling); //配置编码模式

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;                 //配置滤波方式           
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
  TIM4->CNT = 0;                                        //初始化计数
	TIM_Cmd(TIM4,ENABLE);                                 //开启时钟

}

/*******************************************************************************
  * @函数名称	  EnCode_GetLeft
  * @函数说明   获得左轮编码值
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_GetLeft(void)
{
	LeftCount =  TIM_GetCounter(TIM3);
} 

/*******************************************************************************
  * @函数名称	  EnCode_GetRight
  * @函数说明   获得右轮编码值
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_GetRight(void)
{
	RightCount =  TIM_GetCounter(TIM4);
} 

/*******************************************************************************
  * @函数名称	  EnCode_Get
  * @函数说明   获得编码器速度结果
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_Get(void)
{
	EnCode_GetLeft();
	EnCode_GetRight();
	
	LeftDiff = LeftCount - LastLeftCount;
	RightDiff = RightCount - LastRightCount ;
	LastLeftCount = LeftCount;
	LastRightCount = RightCount;
	
}  


/*******************************************************************************
  * @函数名称	  EnCode_Init
  * @函数说明   初始化Encode
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void EnCode_Init(void)
{
	EnCode_GPIO_Configuration();
	EnCode_TIM3_Configuration();
 	EnCode_TIM4_Configuration();
}  


