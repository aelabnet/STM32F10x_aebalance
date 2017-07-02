#include "stm32f10x.h"
#include "stdio.h"
/*******************************************************************************
** �ļ���: 		ae_usart.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2015-10-21
** ����:		  ����ͨѶ������
** ����ļ�:	ae_usart.h
** �޸���־��	2015-10-21   �����ĵ�
*******************************************************************************/


int32_t Baud = 9600;                //���ڲ�����


/*******************************************************************************
  * @��������	  USART1_IO_Configuration
  * @����˵��   ���ø�GPIO�˿ڹ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART1_IO_Configuration(void)
{
    /* ���� GPIO ��ʼ���ṹ�� GPIO_InitStructure */
    GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* ����USART1��GPIOAʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
    
    /* ����USART1��Tx�ţ�PA.9��Ϊ�ڶ������������ģʽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    /* ����USART1��Rx�ţ�PA.10��Ϊ��������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

/*******************************************************************************
  * @��������	  USART2_IO_Configuration
  * @����˵��   ���ø�GPIO�˿ڹ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART2_IO_Configuration(void)
{
    /* ���� GPIO ��ʼ���ṹ�� GPIO_InitStructure */
    GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* ����USART2��GPIOAʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    	
    /* ����USART2��Tx�ţ�PA.2��Ϊ�ڶ������������ģʽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    /* ����USART2��Rx�ţ�PA.3��Ϊ��������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);	
}



/*******************************************************************************
  * @��������	USART1_Configuration
  * @����˵��   ����USART1
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART1_Configuration(void)
{
    /* ����USART��ʼ���ṹ�� USART_InitStructure */
    USART_InitTypeDef USART_InitStructure;
    
    /*	
    *	������Ϊ9600bps
    *	8λ���ݳ���
    *	1��ֹͣλ����У��
    *	����Ӳ��������
    *	��ֹUSARTʱ��
    *	ʱ�Ӽ��Ե�
    *	�ڵ�2�����ز�������
    *	���һλ���ݵ�ʱ�����岻�� SCLK ���
    */ 
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 , &USART_InitStructure);
    
    /* ʹ��USART1 */
    USART_Cmd(USART1 , ENABLE);
}


/*******************************************************************************
  * @��������	USART2_Configuration
  * @����˵��   ����USART2
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART2_Configuration(void)
{
    /* ����USART��ʼ���ṹ�� USART_InitStructure */
    USART_InitTypeDef USART_InitStructure;
    
    /*	
    *	������Ϊ9600bps
    *	8λ���ݳ���
    *	1��ֹͣλ����У��
    *	����Ӳ��������
    *	��ֹUSARTʱ��
    *	ʱ�Ӽ��Ե�
    *	�ڵ�2�����ز�������
    *	���һλ���ݵ�ʱ�����岻�� SCLK ���
    */ 
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_Init(USART2 , &USART_InitStructure);
    
    /* ʹ��USART2 */
		USART_Cmd(USART2 , ENABLE);
}


/*******************************************************************************
  * @��������	  USART1_NVIC_Configuration
  * @����˵��   �����ж�NVIC����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/

void USART1_NVIC_Configuration(void)
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
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
		/* ����USART1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

}

/*******************************************************************************
  * @��������	  USART2_NVIC_Configuration
  * @����˵��   �����ж�NVIC����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/

void USART2_NVIC_Configuration(void)
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
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
		/* ����USART2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//USART2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART2

}

/*******************************************************************************
  * @��������	  USART1_Init
  * @����˵��   ����USART1
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART1_Init(void)
{

		USART1_IO_Configuration();                              //����USART1ʹ�õ�IO��
		USART1_Configuration();															    //����USART1	
		USART_Cmd(USART1, ENABLE);		                        	//ʹ�ܴ���1	
		USART1_NVIC_Configuration();                            //����USART1�ж�	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //ʹ�ܽ����ж�
}

/*******************************************************************************
  * @��������	  USART2_Init
  * @����˵��   ����USART2
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART2_Init(void)
{

		USART2_IO_Configuration();                              //����USART2ʹ�õ�IO��
		USART2_Configuration();															    //����USART2	
		USART_Cmd(USART2, ENABLE);		                        	//ʹ�ܴ���2	
		USART2_NVIC_Configuration();                            //����USART2�ж�	
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //ʹ�ܽ����ж�
}

/*******************************************************************************
  * @��������	  USART1_SendChar(uint16_t ch)
  * @����˵��   ʹ��USART1�����ַ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART1_SendChar(uint16_t ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}		
}


/*******************************************************************************
  * @��������	  myprintf1(char *str)
  * @����˵��   ʹ��USART1�����ַ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void myprintf1(char *str)
{
	while(*str != '\0')
	{
		USART1_SendChar((uint8_t)*str++);
  }
}

/*******************************************************************************
  * @��������	  USART2_SendChar(uint16_t ch)
  * @����˵��   ʹ��USART2�����ַ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void USART2_SendChar(uint16_t ch)
{
	USART_SendData(USART2, (uint8_t) ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}		
}


/*******************************************************************************
  * @��������	  myprintf2(char *str)
  * @����˵��   ʹ��USART2�����ַ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void myprintf2(char *str)
{
	while(*str != '\0')
	{
		USART2_SendChar((uint8_t)*str++);
  }
}


/* �������´���,֧��printf���� */  
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE 
{
/* ��Printf���ݷ������� */
  USART_SendData(USART2,(uint8_t)  ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
 
  return (ch);
}

/***********************************�ļ�����***********************************/



