/*******************************************************************************
** 文件名: 		ae_VNH5180.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2015-11-7
** 功能:		  通过KEY控制LED
** 相关文件:	ae_VNH5180.h
** 修改日志：	2015-11-7   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "ae_VNH5160.h"
#include "ae_Com.h"
/* 类型声明 ------------------------------------------------------------------*/


/* 宏定义 --------------------------------------------------------------------*/
#define LeftA_H   GPIO_SetBits(GPIOB, GPIO_Pin_1) 
#define LeftA_L   GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define LeftB_H   GPIO_SetBits(GPIOB, GPIO_Pin_5) 
#define LeftB_L   GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define RightA_H  GPIO_SetBits(GPIOC, GPIO_Pin_13) 
#define RightA_L  GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define RightB_H  GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define RightB_L  GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define Left_EN        GPIO_SetBits(GPIOB, GPIO_Pin_8) 
#define Left_DIS			 GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define Right_EN       GPIO_SetBits(GPIOA, GPIO_Pin_8) 
#define Right_DIS      GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LeftCS_EN      GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define LeftCS_DIS     GPIO_SetBits(GPIOB, GPIO_Pin_9) 
#define RightCS_EN     GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define RightCS_DIS    GPIO_SetBits(GPIOA, GPIO_Pin_11) 






/*******************************************************************************
  * @函数名称	  VNH5160_GPIO_Configuration
  * @函数说明   设置各GPIO端口功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	   //定义GPIO结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIOC时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //使能复用功能时钟
		

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;           //PA8:EN2
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_11;          //PA11:CS2_Dis		
	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;           //PA2:测时间
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;           //PA0:PWM1	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;           //PA1:PWM2		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;           //PB8:EN1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;           //PB1:IN1A
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_5;           //PB5:IN1B	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_9;           //PB9:CS1_Dis		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;          //PC13:IN2A
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_14;          //PB14:IN2B
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;            //PA4：CS1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_5;            //PA5：CS2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //模拟输入脚
	GPIO_Init(GPIOA , &GPIO_InitStructure);
		
}


/*******************************************************************************
  * @函数名称	  VNH5160_TIM2_Configuration
  * @函数说明   设置PWM
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_TIM2_Configuration(void)
{
	int32_t freqPWM = FreqPWM;  //PWM频率
	float ducyCycle = 0.5;//50%占空比
	/* 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* 定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
	
	/* ---------------------------------------------------------------
	TIM3CLK 挂在了APB1 当APB1设置为PLL时钟的2分频时 挂在APB1的通用定时器需要乘以2 即PCLK1=72MHz  
	TIM3CLK = 72 MHz
	--------------------------------------------------------------- */	
	/* 
	*  	计数重载值为72000000/freqPWM   Max=65535
	*  	预分频值为0+1=1
	*  	时钟分割0
	*  	向上计数模式
	*/
	TIM_TimeBaseStructure.TIM_Period = 72000000/freqPWM;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	/* 
	*  	选择定时器模式:TIM脉冲宽度调制模式2
	*  	比较输出使能
	*  	设置待装入捕获比较寄存器的比较值
	*  	输出极性:TIM输出比较极性高
	*  	OC1参数初始化外设TIMx
	*  	OC2参数初始化外设TIMx
	*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //向上计数模式时，计数值<比较值时高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 72000000*ducyCycle/freqPWM; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  

	/* 
	*  	使能OC1预装载寄存器
	*  	使能OC2预装载寄存器
	*  	使能预装载寄存器
	*  	使能TIMx在ARR上的预装载寄存器
	*  	开启时钟
	*/		
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_ARRPreloadConfig(TIM2, ENABLE); 
	TIM_Cmd(TIM2,ENABLE); 

}



/*******************************************************************************
* 函数名  		  : ADC_Configuration
* 函数描述    	: 初始化并启动ADC转换
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void VNH5160_ADC_Configuration(void)
{
	/* 定义 ADC 初始化结构体 ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;

	/* 配置ADC相关时钟 */	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
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
	
	/* 设置 ADC1 使用1/2转换通道，转换顺序1/2，采样时间为 55.5 周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
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
  * @函数名称	  VNH5160_setPWM
  * @函数说明   设置PWM
  * @输入参数   motor：电机
								dutyratio：占空比			
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_setPWM(enum Command motor, double dutyratio) 
{ 
  uint16_t Cn = TIM2->ARR;
  if (motor == left)
  	TIM_SetCompare1(TIM2,Cn*dutyratio/100);
  else
    TIM_SetCompare2(TIM2,Cn*dutyratio/100);
}

/*******************************************************************************
  * @函数名称	  VNH5160_moveMotor
  * @函数说明   控制电机方向和速度
  * @输入参数   motor：电机
								direction：方向
								speedRaw：速度0~100
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_moveMotor(enum Command motor, enum Command direction, double speedRaw) 
{ 
	Left_EN;
	Right_EN;
  if (speedRaw > 100)
		speedRaw = 100;
  VNH5160_setPWM(motor, speedRaw); 
  if (motor == left) {
    if (direction == forward) {
      LeftA_H;
      LeftB_L;
    }
		else if(direction == backward){
      LeftA_L;
      LeftB_H;			
		}		
    else {
      LeftA_L;
      LeftB_L;
    }
  }
  else {
    if (direction == backward) {
      RightA_H;
      RightB_L;
    }
		else if(direction == forward){
      RightA_L;
      RightB_H;			
		}		
    else {
      RightA_L;
      RightB_L;
    }
  }
}


/*******************************************************************************
  * @函数名称	  VNH5160_stopMotor
  * @函数说明   停止电机
  * @输入参数   motor：电机
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_stopMotor(enum Command motor) 
{ 
  if (motor == left) {
		Left_DIS;
  }
  else {
		Right_DIS;
	}
}

/*******************************************************************************
  * @函数名称	  VNH5160_CS_EN
  * @函数说明   使能VNH5160 CS功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_CS_EN(void) 
{ 
	LeftCS_EN;
	RightCS_EN;
	VNH5160_ADC_Configuration();
}

/*******************************************************************************
  * @函数名称	  VNH5160_CS_DIS
  * @函数说明   关闭VNH5160 CS功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_CS_DIS(void) 
{ 
	LeftCS_DIS;
	RightCS_DIS;
}

/*******************************************************************************
  * @函数名称	  VNH5160_Init
  * @函数说明   初始化VNH5160
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void VNH5160_Init(void)
{
	VNH5160_GPIO_Configuration();
 	VNH5160_TIM2_Configuration();
	VNH5160_CS_DIS();
}  


