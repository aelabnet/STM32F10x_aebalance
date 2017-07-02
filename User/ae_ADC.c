/*******************************************************************************
** 文件名: 		ae_ADC.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2016-11-21
** 功能:		  ADC模数转换
** 相关文件:	ae_ADC.h
** 修改日志：	2016-11-21   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"


/*******************************************************************************
  * @函数名称	  GPIO_Configuration
  * @函数说明   GPIO配置初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void GPIO_Configuration(void)
{
  	/* 定义 GPIO 初始化结构体 GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/* 使能各个用到的外设时钟 */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
  	/* 将 PB.0 设置为模拟输入脚 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
}

/*******************************************************************************
  * @函数名称	  ADC_Configuration
  * @函数说明   ADC配置初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void ADC_Configuration(void)
{
	/* 定义 ADC 初始化结构体 ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
	/* 配置ADC时钟分频 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/*	
	*	独立工作模式;
	*	多通道扫描模式;
	*	连续模数转换模式;
	*	转换触发方式：转换由软件触发启动;
	*	ADC 数据右对齐 ;
	*	进行规则转换的 ADC 通道的数目为1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* 设置 ADC1 使用8转换通道，转换顺序1，采样时间为 55.5 周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* 复位 ADC1 的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待 ADC1 校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
	/* 启动 ADC1 转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}

/*******************************************************************************
  * @函数名称	  VoltageADC_Init
  * @函数说明   初始化VoltageADC
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VoltageADC_Init(void)
{
	GPIO_Configuration();
 	ADC_Configuration();
}  


/***********************************文件结束***********************************/



