/*******************************************************************************
** �ļ���: 		ae_ADC.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2016-11-21
** ����:		  ADCģ��ת��
** ����ļ�:	ae_ADC.h
** �޸���־��	2016-11-21   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"


/*******************************************************************************
  * @��������	  GPIO_Configuration
  * @����˵��   GPIO���ó�ʼ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void GPIO_Configuration(void)
{
  	/* ���� GPIO ��ʼ���ṹ�� GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/* ʹ�ܸ����õ�������ʱ�� */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
  	/* �� PB.0 ����Ϊģ������� */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOB , &GPIO_InitStructure);
}

/*******************************************************************************
  * @��������	  ADC_Configuration
  * @����˵��   ADC���ó�ʼ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void ADC_Configuration(void)
{
	/* ���� ADC ��ʼ���ṹ�� ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
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
	
	/* ���� ADC1 ʹ��8ת��ͨ����ת��˳��1������ʱ��Ϊ 55.5 ���� */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
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
  * @��������	  VoltageADC_Init
  * @����˵��   ��ʼ��VoltageADC
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void VoltageADC_Init(void)
{
	GPIO_Configuration();
 	ADC_Configuration();
}  


/***********************************�ļ�����***********************************/



