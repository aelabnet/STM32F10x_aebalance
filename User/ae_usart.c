#include "stm32f10x.h"
#include "stdio.h"
/*******************************************************************************
** 文件名: 		ae_usart.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2015-10-21
** 功能:		  串口通讯的配置
** 相关文件:	ae_usart.h
** 修改日志：	2015-10-21   创建文档
*******************************************************************************/


int32_t Baud = 9600;                //串口波特率


/*******************************************************************************
  * @函数名称	  USART1_IO_Configuration
  * @函数说明   设置各GPIO端口功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART1_IO_Configuration(void)
{
    /* 定义 GPIO 初始化结构体 GPIO_InitStructure */
    GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* 开启USART1和GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
    
    /* 设置USART1的Tx脚（PA.9）为第二功能推挽输出模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    /* 设置USART1的Rx脚（PA.10）为浮空输入脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

/*******************************************************************************
  * @函数名称	  USART2_IO_Configuration
  * @函数说明   设置各GPIO端口功能
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART2_IO_Configuration(void)
{
    /* 定义 GPIO 初始化结构体 GPIO_InitStructure */
    GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* 开启USART2和GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    	
    /* 设置USART2的Tx脚（PA.2）为第二功能推挽输出模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
    
    /* 设置USART2的Rx脚（PA.3）为浮空输入脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);	
}



/*******************************************************************************
  * @函数名称	USART1_Configuration
  * @函数说明   设置USART1
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART1_Configuration(void)
{
    /* 定义USART初始化结构体 USART_InitStructure */
    USART_InitTypeDef USART_InitStructure;
    
    /*	
    *	波特率为9600bps
    *	8位数据长度
    *	1个停止位，无校验
    *	禁用硬件流控制
    *	禁止USART时钟
    *	时钟极性低
    *	在第2个边沿捕获数据
    *	最后一位数据的时钟脉冲不从 SCLK 输出
    */ 
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 , &USART_InitStructure);
    
    /* 使能USART1 */
    USART_Cmd(USART1 , ENABLE);
}


/*******************************************************************************
  * @函数名称	USART2_Configuration
  * @函数说明   设置USART2
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART2_Configuration(void)
{
    /* 定义USART初始化结构体 USART_InitStructure */
    USART_InitTypeDef USART_InitStructure;
    
    /*	
    *	波特率为9600bps
    *	8位数据长度
    *	1个停止位，无校验
    *	禁用硬件流控制
    *	禁止USART时钟
    *	时钟极性低
    *	在第2个边沿捕获数据
    *	最后一位数据的时钟脉冲不从 SCLK 输出
    */ 
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_Init(USART2 , &USART_InitStructure);
    
    /* 使能USART2 */
		USART_Cmd(USART2 , ENABLE);
}


/*******************************************************************************
  * @函数名称	  USART1_NVIC_Configuration
  * @函数说明   设置中断NVIC参数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/

void USART1_NVIC_Configuration(void)
{								
	/* 定义 NVIC 初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif结构的作用是根据预编译条件决定中断向量表起始地址*/   
#ifdef  VECT_TAB_RAM  
  		/* 中断向量表起始地址从 0x20000000 开始 */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* 中断向量表起始地址从 0x80000000 开始 */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* 选择优先级分组0 */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
		/* 设置USART1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//USART1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

}

/*******************************************************************************
  * @函数名称	  USART2_NVIC_Configuration
  * @函数说明   设置中断NVIC参数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/

void USART2_NVIC_Configuration(void)
{								
	/* 定义 NVIC 初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif结构的作用是根据预编译条件决定中断向量表起始地址*/   
#ifdef  VECT_TAB_RAM  
  		/* 中断向量表起始地址从 0x20000000 开始 */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* 中断向量表起始地址从 0x80000000 开始 */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* 选择优先级分组0 */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
		/* 设置USART2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//USART2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART2

}

/*******************************************************************************
  * @函数名称	  USART1_Init
  * @函数说明   设置USART1
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART1_Init(void)
{

		USART1_IO_Configuration();                              //配置USART1使用的IO口
		USART1_Configuration();															    //配置USART1	
		USART_Cmd(USART1, ENABLE);		                        	//使能串口1	
		USART1_NVIC_Configuration();                            //配置USART1中断	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //使能接收中断
}

/*******************************************************************************
  * @函数名称	  USART2_Init
  * @函数说明   设置USART2
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART2_Init(void)
{

		USART2_IO_Configuration();                              //配置USART2使用的IO口
		USART2_Configuration();															    //配置USART2	
		USART_Cmd(USART2, ENABLE);		                        	//使能串口2	
		USART2_NVIC_Configuration();                            //配置USART2中断	
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //使能接收中断
}

/*******************************************************************************
  * @函数名称	  USART1_SendChar(uint16_t ch)
  * @函数说明   使用USART1发送字符
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART1_SendChar(uint16_t ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}		
}


/*******************************************************************************
  * @函数名称	  myprintf1(char *str)
  * @函数说明   使用USART1发送字符串
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void myprintf1(char *str)
{
	while(*str != '\0')
	{
		USART1_SendChar((uint8_t)*str++);
  }
}

/*******************************************************************************
  * @函数名称	  USART2_SendChar(uint16_t ch)
  * @函数说明   使用USART2发送字符
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void USART2_SendChar(uint16_t ch)
{
	USART_SendData(USART2, (uint8_t) ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}		
}


/*******************************************************************************
  * @函数名称	  myprintf2(char *str)
  * @函数说明   使用USART2发送字符串
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void myprintf2(char *str)
{
	while(*str != '\0')
	{
		USART2_SendChar((uint8_t)*str++);
  }
}


/* 加入以下代码,支持printf函数 */  
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE 
{
/* 将Printf内容发往串口 */
  USART_SendData(USART2,(uint8_t)  ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
 
  return (ch);
}

/***********************************文件结束***********************************/



