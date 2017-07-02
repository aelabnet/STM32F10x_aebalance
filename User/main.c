/*******************************************************************************
** �ļ���: 		mian.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 5.20
** ����: 		  aelab
** ��������: 	2015-04-10
** ����:		  ģ������û������������˵�����̵Ĺ��ܣ�
** ����ļ�:	��
** �޸���־��	2015-04-10   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
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

/* ���� ----------------------------------------------------------------------*/
uint8_t RxBuff[50] = {0};
uint8_t RxCnt = 0;

uint32_t LowVolCnt = 0;
uint32_t LedCnt = 0;
uint8_t LowVolFlag = 0;


/*******************************************************************************
  * @��������	  main
  * @����˵��   ������ 
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
int main(void)
{
	
	
	SysTick_Initaize(); //��ʼ���δ�ʱ�ӣ���Ϊ��ʱʱ��
	delay_ms(3000);     //�ϵ���ʱ
		
	Reader(data1, data2); //��Flash�ж�ȡС������
	DeListValues();       //����data1 data2����С������
	
	LED_Init();
		
	MPU6050_Init();     //��ʼ��MPU6050
	Cycle_Init();       //��ʼ��MPU6050��������
	USART2_Init();      //��ʼ������2
	VNH5160_Init();
	EnCode_Init();
	VoltageADC_Init();
	
	
	printf("AT+BAUD9");
	Baud = 230400;
	USART2_Init();      //��ʼ������2
	
	delay_ms(1000);     //�ϵ���ʱ
   
    while (1)
    {

			
    }
}


/*******************************************************************************
  * @��������	  TIM1_UP_IRQHandler
  * @����˵��   ʱ��1�жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void TIM1_UP_IRQHandler(void) 
{
	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
	
		/* ÿ10*�������ڻ�õ������������ֵ��ת�� */
		SpeedControlCnt++;
		if(SpeedControlCnt > 9)
		{
			Voltage = 4.3 * 3.2 * ADC_GetConversionValue(ADC1) / 0x0FFF;
			EnCode_Get();	
			SpeedControlCnt = 0;
		}
		
		/* �Ե�ص�ѹ�����жϣ���ѹʱ����LED��˸���� */
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
		
		
    Angle();           //���нǶȼ���
		Comp_Angle();      //���л����˲�����		
		Control();				 //����С��
		SendValues();      //������ؽ��
		
		if(FlagSave)       //���ݴ���
		{
			FlagSave = false;
			ListValues();
			Storage(data1,data2);
		}
		if(FlagReset)      //����ָ�Ĭ������
		{
			FlagReset = false;
			DefaultValues();
			Storage(data1,data2);
		}
		if(FlagNewSample)  //�µĲ�������
		{
			FlagNewSample = false;
			Cycle_Init();       //��ʼ��MPU6050��������
		}
		if(FlagNewPWM)       //�µĵ��PWMƵ��
		{
			FlagNewPWM = false;
			VNH5160_Init();
		}		

		
	}
}

/*******************************************************************************
  * @��������	  USART1_IRQHandler
  * @����˵��   ����1�жϺ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{		

		RxBuff[RxCnt++] = USART_ReceiveData(USART1);  //��ȡ���յ�������
		
		if(RxBuff[RxCnt-1] == ';')
		{			
			RxCnt = 0;
			GetValues((char*)RxBuff);             //�жϽ��յ�������
		}	
	 
	}
}


/*******************************************************************************
  * @��������	  USART2_IRQHandler
  * @����˵��   ����2�жϺ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
	{		

		RxBuff[RxCnt++] = USART_ReceiveData(USART2);  //��ȡ���յ�������
		
		if(RxBuff[RxCnt-1] == ';')
		{			
			RxCnt = 0;
			GetValues((char*)RxBuff);             //�жϽ��յ�������
		}	
	 
	}
}


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
  * @��������	assert_failed
  * @����˵��   �����ڼ�������������ʱ��Դ�ļ����ʹ�������
  * @�������   file: Դ�ļ���
  				line: ������������ 
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* �û����������Լ��Ĵ������ڱ��������ļ�������������,
       ���磺printf("�������ֵ: �ļ��� %s �� %d��\r\n", file, line) */

    /* ����ѭ�� */
    while (1)
    {
    }
}
#endif

/***********************************�ļ�����***********************************/
