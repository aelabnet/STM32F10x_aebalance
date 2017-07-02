/*******************************************************************************
** 文件名: 		ae_GPIO.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2015-11-7
** 功能:		  通过KEY控制LED
** 相关文件:	ae_GPIO.h
** 修改日志：	2015-11-7   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"



/*******************************************************************************
  * @函数名称	  GPIO_OUT_Configuration
  * @函数说明   设置各GPIO端口输出功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void GPIO_OUT_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	   //定义GPIO结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;          //选择管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/*******************************************************************************
  * @函数名称	  GPIO_IN_Configuration
  * @函数说明   设置各GPIO端口输入功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
 void GPIO_IN_Configuration(void)
 {
 	GPIO_InitTypeDef	GPIO_InitStructure;	    //定义GPIO结构体
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    //使能GPIOC时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //选择管脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //配置为输入模式
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //最大开关频率选择50MHZ
 	GPIO_Init(GPIOC, &GPIO_InitStructure);                 //按照GPIO结构体的配置初始化GPIOC
 }    


/*******************************************************************************
  * @函数名称	  LED_Init
  * @函数说明   初始化LED
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void LED_Init(void)
{
	GPIO_OUT_Configuration();
 	GPIO_IN_Configuration();
}  


