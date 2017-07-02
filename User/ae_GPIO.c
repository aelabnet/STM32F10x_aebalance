/*******************************************************************************
** �ļ���: 		ae_GPIO.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2015-11-7
** ����:		  ͨ��KEY����LED
** ����ļ�:	ae_GPIO.h
** �޸���־��	2015-11-7   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"



/*******************************************************************************
  * @��������	  GPIO_OUT_Configuration
  * @����˵��   ���ø�GPIO�˿��������
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void GPIO_OUT_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	   //����GPIO�ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;          //ѡ��ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/*******************************************************************************
  * @��������	  GPIO_IN_Configuration
  * @����˵��   ���ø�GPIO�˿����빦��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
 void GPIO_IN_Configuration(void)
 {
 	GPIO_InitTypeDef	GPIO_InitStructure;	    //����GPIO�ṹ��
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    //ʹ��GPIOCʱ��

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //ѡ��ܽ�
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           //����Ϊ����ģʽ
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //��󿪹�Ƶ��ѡ��50MHZ
 	GPIO_Init(GPIOC, &GPIO_InitStructure);                 //����GPIO�ṹ������ó�ʼ��GPIOC
 }    


/*******************************************************************************
  * @��������	  LED_Init
  * @����˵��   ��ʼ��LED
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void LED_Init(void)
{
	GPIO_OUT_Configuration();
 	GPIO_IN_Configuration();
}  


