/*******************************************************************************
** �ļ���: 		ae_EnCode.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2016-10-7
** ����:		  ͨ�����������ת��
** ����ļ�:	ae_EnCode.h
** �޸���־��	2016-10-7   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"

/* �������� ------------------------------------------------------------------*/


/* �궨�� --------------------------------------------------------------------*/

/* ���� ----------------------------------------------------------------------*/
uint16_t LeftCount;
uint16_t RightCount;
uint16_t LastLeftCount;
uint16_t LastRightCount;

int16_t LeftDiff;
int16_t RightDiff;

/* �������� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/


/*******************************************************************************
  * @��������	  EnCode_GPIO_Configuration
  * @����˵��   ���ø�GPIO�˿ڹ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	                 //����GPIO�ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ�ܸ��ù���ʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PA6,PA7Ϊ���1A,1B����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PB6,PA7Ϊ���2A,2B����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
}


/*******************************************************************************
  * @��������	  EnCode_TIM3_Configuration
  * @����˵��   ����ʱ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_TIM3_Configuration(void)
{

	TIM_ICInitTypeDef TIM_ICInitStructure;                 //����TIM�ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 //ʹ��TIM3ʱ��
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //���ñ���ģʽ

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;                  //�����˲���ʽ           
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
  TIM3->CNT = 0;                                         //��ʼ������
	TIM_Cmd(TIM3,ENABLE);                                  //����ʱ��

}




/*******************************************************************************
  * @��������	  EnCode_TIM4_Configuration
  * @����˵��   ����ʱ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_TIM4_Configuration(void)
{

	TIM_ICInitTypeDef TIM_ICInitStructure;                //����TIM�ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM4ʱ��
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling); //���ñ���ģʽ

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;                 //�����˲���ʽ           
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
  TIM4->CNT = 0;                                        //��ʼ������
	TIM_Cmd(TIM4,ENABLE);                                 //����ʱ��

}

/*******************************************************************************
  * @��������	  EnCode_GetLeft
  * @����˵��   ������ֱ���ֵ
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_GetLeft(void)
{
	LeftCount =  TIM_GetCounter(TIM3);
} 

/*******************************************************************************
  * @��������	  EnCode_GetRight
  * @����˵��   ������ֱ���ֵ
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_GetRight(void)
{
	RightCount =  TIM_GetCounter(TIM4);
} 

/*******************************************************************************
  * @��������	  EnCode_Get
  * @����˵��   ��ñ������ٶȽ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
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
  * @��������	  EnCode_Init
  * @����˵��   ��ʼ��Encode
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void EnCode_Init(void)
{
	EnCode_GPIO_Configuration();
	EnCode_TIM3_Configuration();
 	EnCode_TIM4_Configuration();
}  


