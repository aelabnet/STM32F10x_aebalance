/*******************************************************************************
** �ļ���: 		ae_Angle.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 5.20
** ����: 		  aelab
** ��������: 	2015-07-03
** ����:		  MPU6050ֱ�ӻ�ýǶ�ֵ
** ����ļ�:	ae_I2C.c
							ae_usart.c
							SysTickDelay.c
							ae_mpu6050.c
** �޸���־��	2015-07-03   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"
#include "math.h"
#include "SysTickDelay.h"
#include "ae_mpu6050.h"
#include "ae_Com.h"


/* �궨�� --------------------------------------------------------------------*/
#define RAD_TO_DEG 57.29578049        //����ת�Ƕ�
/* ���� ----------------------------------------------------------------------*/

uint16_t CalibAngleCnt = 0;
double XgyroRateCalibBuffer = 0;
double XgyroRateZero = 0;

double XgyroRate = 0;



double XaccAngle,XaccAngle1;	
float			E1 = 0;	//�˲��㷨�м����
float			E2 = 0;	//�˲��㷨�м����
double    Car_Angle;//�˲��㷨�м����
	
/*******************************************************************************
  * @��������	  TIM1_Configuration
  * @����˵��   ����TIME1  20*0.1ms
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void TIM1_Configuration(void)
{
	/* ���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);	
	
	
	/* ---------------------------------------------------------------
	TIM2LK ��PCLK1=36MHz
	TIM2CLK = 36 MHz, Prescaler = 72, TIM2 counter clock = 500K,���ı�һ��Ϊ500K,���ھ�Ϊ0.001ms
	--------------------------------------------------------------- */	
	/* 
	*  	��������ֵΪ50000   Max=65535
	*  	Ԥ��ƵֵΪ(71 + 1 = 72)
	*  	ʱ�ӷָ�0
	*  	���ϼ���ģʽ
	*/
	TIM_TimeBaseStructure.TIM_Period = SamplePeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM1 , &TIM_TimeBaseStructure);
	/* 
	*  	�������жϱ�־
	*  	ʹ�� TIM1 �ж� 
	*  	����ʱ��
	*/
  TIM_ClearFlag(TIM1,TIM_FLAG_Update);  
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE);   
}

/*******************************************************************************
  * @��������	  TIM1_NVIC_Configuration
  * @����˵��   ����TIM1�ж�NVIC����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/

void TIM1_NVIC_Configuration(void)
{								
	/* ���� NVIC ��ʼ���ṹ�� */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif�ṹ�������Ǹ���Ԥ�������������ж���������ʼ��ַ*/   
#ifdef  VECT_TAB_RAM  
  		/* �ж���������ʼ��ַ�� 0x20000000 ��ʼ */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* �ж���������ʼ��ַ�� 0x80000000 ��ʼ */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* ѡ�����ȼ�����0 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* ���� TIM1 �ж�, 0����ռ���ȼ���0����ռ���ȼ� */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/*******************************************************************************
  * @��������	  Cycle_Init
  * @����˵��   ��ʼ����������
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void Cycle_Init(void)
{
	TIM1_Configuration();
	TIM1_NVIC_Configuration();
}
/*******************************************************************************
  * @��������	  Calib_Gyro
  * @����˵��   ���ٶȲ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void Calib_Gyro(void)
{
		if(CalibAngleCnt < 100)
		{
			CalibAngleCnt++;
			XgyroRateCalibBuffer += XgyroRate;			
		}
		if(CalibAngleCnt == 100)
		{
			XgyroRateZero = XgyroRateCalibBuffer/CalibAngleCnt;
			CalibAngleCnt++;
		}	
}

/*******************************************************************************
  * @��������	  Angle
  * @����˵��   ֱ��ͨ��MPU6050��ýǶ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void Angle(void)
{

		int16_t yacc;
		int16_t zacc;
		int16_t xgyro;
		uint16_t temp1[3];
		uint16_t temp2[3];
		
		MPU6050ReadAcc(temp1);
		MPU6050ReadGyro(temp2);
	
		yacc = temp1[1];
		zacc = temp1[2];		
		xgyro = temp2[0];

		XgyroRate = (double)xgyro / 16.4f;     //������ֵת��Ϊ�Ƕ�
		
		AngleXacc = atan2((double)yacc , (double)zacc) * RAD_TO_DEG;		                 //ͨ�����ٶȻ����б�Ƕ�
    AngleXgyro = AngleXgyro + (XgyroRate - XgyroRateZero)* SamplePeriod * 0.0001 ;   //ͨ�����ٶȻ����б�Ƕ�
		if(AngleXgyro < -180) AngleXgyro += 360;
	  if(AngleXgyro > 180) AngleXgyro -= 360;
	
			
}




