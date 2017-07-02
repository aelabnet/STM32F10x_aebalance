/*******************************************************************************
** �ļ���: 		ae_VNH5180.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2015-11-7
** ����:		  ͨ��KEY����LED
** ����ļ�:	ae_VNH5180.h
** �޸���־��	2015-11-7   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"
#include "ae_VNH5160.h"
#include "ae_Com.h"
/* �������� ------------------------------------------------------------------*/


/* �궨�� --------------------------------------------------------------------*/
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
  * @��������	  VNH5160_GPIO_Configuration
  * @����˵��   ���ø�GPIO�˿ڹ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VNH5160_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	   //����GPIO�ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //ʹ�ܸ��ù���ʱ��
		

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;           //PA8:EN2
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_11;          //PA11:CS2_Dis		
	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;           //PA2:��ʱ��
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;           //PA0:PWM1	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;           //PA1:PWM2		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;           //PB8:EN1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;           //PB1:IN1A
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_5;           //PB5:IN1B	
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_9;           //PB9:CS1_Dis		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;          //PC13:IN2A
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_14;          //PB14:IN2B
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;            //PA4��CS1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_5;            //PA5��CS2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //ģ�������
	GPIO_Init(GPIOA , &GPIO_InitStructure);
		
}


/*******************************************************************************
  * @��������	  VNH5160_TIM2_Configuration
  * @����˵��   ����PWM
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VNH5160_TIM2_Configuration(void)
{
	int32_t freqPWM = FreqPWM;  //PWMƵ��
	float ducyCycle = 0.5;//50%ռ�ձ�
	/* ���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* ���� TIM_OCInit ��ʼ���ṹ�� TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
	
	/* ---------------------------------------------------------------
	TIM3CLK ������APB1 ��APB1����ΪPLLʱ�ӵ�2��Ƶʱ ����APB1��ͨ�ö�ʱ����Ҫ����2 ��PCLK1=72MHz  
	TIM3CLK = 72 MHz
	--------------------------------------------------------------- */	
	/* 
	*  	��������ֵΪ72000000/freqPWM   Max=65535
	*  	Ԥ��ƵֵΪ0+1=1
	*  	ʱ�ӷָ�0
	*  	���ϼ���ģʽ
	*/
	TIM_TimeBaseStructure.TIM_Period = 72000000/freqPWM;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	/* 
	*  	ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	*  	�Ƚ����ʹ��
	*  	���ô�װ�벶��ȽϼĴ����ıȽ�ֵ
	*  	�������:TIM����Ƚϼ��Ը�
	*  	OC1������ʼ������TIMx
	*  	OC2������ʼ������TIMx
	*/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //���ϼ���ģʽʱ������ֵ<�Ƚ�ֵʱ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 72000000*ducyCycle/freqPWM; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  

	/* 
	*  	ʹ��OC1Ԥװ�ؼĴ���
	*  	ʹ��OC2Ԥװ�ؼĴ���
	*  	ʹ��Ԥװ�ؼĴ���
	*  	ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	*  	����ʱ��
	*/		
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_ARRPreloadConfig(TIM2, ENABLE); 
	TIM_Cmd(TIM2,ENABLE); 

}



/*******************************************************************************
* ������  		  : ADC_Configuration
* ��������    	: ��ʼ��������ADCת��
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void VNH5160_ADC_Configuration(void)
{
	/* ���� ADC ��ʼ���ṹ�� ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;

	/* ����ADC���ʱ�� */	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* ����ADCʱ�ӷ�Ƶ */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/*	
	*	��������ģʽ;
	*	��ͨ��ɨ��ģʽ;
	*	����ģ��ת��ģʽ;
	*	ת��������ʽ��ת���������������;
	*	ADC �����Ҷ��� ;
	*	���й���ת���� ADC ͨ������ĿΪ1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ���� ADC1 ʹ��1/2ת��ͨ����ת��˳��1/2������ʱ��Ϊ 55.5 ���� */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* ��λ ADC1 ��У׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* �ȴ� ADC1 У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ��ʼ ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
	/* ���� ADC1 ת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}


/*******************************************************************************
  * @��������	  VNH5160_setPWM
  * @����˵��   ����PWM
  * @�������   motor�����
								dutyratio��ռ�ձ�			
  * @�������   ��
  * @���ز���   ��
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
  * @��������	  VNH5160_moveMotor
  * @����˵��   ���Ƶ��������ٶ�
  * @�������   motor�����
								direction������
								speedRaw���ٶ�0~100
  * @�������   ��
  * @���ز���   ��
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
  * @��������	  VNH5160_stopMotor
  * @����˵��   ֹͣ���
  * @�������   motor�����
  * @�������   ��
  * @���ز���   ��
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
  * @��������	  VNH5160_CS_EN
  * @����˵��   ʹ��VNH5160 CS����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VNH5160_CS_EN(void) 
{ 
	LeftCS_EN;
	RightCS_EN;
	VNH5160_ADC_Configuration();
}

/*******************************************************************************
  * @��������	  VNH5160_CS_DIS
  * @����˵��   �ر�VNH5160 CS����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VNH5160_CS_DIS(void) 
{ 
	LeftCS_DIS;
	RightCS_DIS;
}

/*******************************************************************************
  * @��������	  VNH5160_Init
  * @����˵��   ��ʼ��VNH5160
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VNH5160_Init(void)
{
	VNH5160_GPIO_Configuration();
 	VNH5160_TIM2_Configuration();
	VNH5160_CS_DIS();
}  


