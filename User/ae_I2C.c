/*******************************************************************************
** 文件名: 		ae_I2C.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5.20
** 作者: 		  aelab
** 生成日期: 	2015-06-11
** 功能:		  软件I2C程序
** 相关文件:	SysTickDelay.c
** 修改日志：	2015-06-11   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "SysTickDelay.h"
#include "ae_I2C.h"


/*******************************************************************************
  * @函数名称	  myI2C_Init
  * @函数说明   I2C设置初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void myI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
  GPIO_RCC;                                           //配置I2C pin口的时钟
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL|I2C_SDA;        //配置I2C的IO口资源
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //配置IO口时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //配置IO口为强上拉
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);
	
	I2C_SCL_H;
	I2C_SDA_H;
}

/*******************************************************************************
  * @函数名称	  I2C_SDA_OUT
  * @函数说明   配置SDA引脚为输出
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

/*******************************************************************************
  * @函数名称	  I2C_SDA_IN
  * @函数说明   配置SDA引脚为输入
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*******************************************************************************
  * @函数名称	  I2C_Start
  * @函数说明   产生初始信号
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_Start(void)
{
  I2C_SDA_OUT();	  //配置SDA为输出
	
	I2C_SDA_H;        //SDA输出为高
	I2C_SCL_H;        //SCL输出为高
	delay_us(2);      //延时
	I2C_SDA_L;        //SDA输出为低
	delay_us(2);      //延时
	I2C_SCL_L;        //SCL输出为低
}

/*******************************************************************************
  * @函数名称	  I2C_Stop
  * @函数说明   产生停止信号
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_Stop(void)
{
   I2C_SDA_OUT();   //配置SDA为输出

   I2C_SCL_L;       //SCL输出为低
   I2C_SDA_L;       //SDA输出为低
   I2C_SCL_H;       //SCL输出为高
   delay_us(2);     //延时
   I2C_SDA_H;       //SDA输出为高
   delay_us(2);     //延时
}

/*******************************************************************************
  * @函数名称	  I2C_Ack
  * @函数说明   主机产生应答信号ACK
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_Ack(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_L;
   delay_us(2);
   I2C_SCL_H;
   delay_us(2);
   I2C_SCL_L;
}

/*******************************************************************************
  * @函数名称	  I2C_NAck
  * @函数说明   主机不产生应答信号NACK
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_NAck(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_H;
   delay_us(2);
   I2C_SCL_H;
   delay_us(2);
   I2C_SCL_L;
}

/*******************************************************************************
  * @函数名称	  I2C_Wait_Ack
  * @函数说明   等待从机应答信号
  * @输入参数   无
  * @输出参数   无
  * @返回参数   1 接收应答失败
                0 接收应答成功
*******************************************************************************/
uint8_t I2C_Wait_Ack(void)
{
	uint8_t tempTime=0;       //超时计数

	I2C_SDA_IN();             //配置SDA为输出

	I2C_SDA_H;                //SDA为高
	delay_us(1);              
	I2C_SCL_H;
	delay_us(1);

	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))  //等待应答信号
	{
		tempTime++;
		if(tempTime>250)        //超时250
		{
			I2C_Stop();
			return 1;             //超时后返回1
		}	 
	}

	I2C_SCL_L;
	return 0;                 //接收到应答信号后返回0
}

/*******************************************************************************
  * @函数名称	  I2C_Send_Byte
  * @函数说明   发送一个字节
  * @输入参数   uint8_t txd:带发送字节
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void I2C_Send_Byte(uint8_t txd)
{
	uint8_t i=0;

	I2C_SDA_OUT();
	I2C_SCL_L;              //拉低时钟开始数据传输

	for(i=0;i<8;i++)        //发送一个字节 8位
	{
		if((txd&0x80)>0)      //0x80  1000 0000 高位先发
			I2C_SDA_H;          //操作数据线
		else
			I2C_SDA_L;
		txd<<=1;              //一位位发送
		I2C_SCL_H;            //产生时钟发送数据
		delay_us(2);          
		I2C_SCL_L;
		delay_us(2);
	}
}

/*******************************************************************************
  * @函数名称	  I2C_Read_Byte
  * @函数说明   读一个字节
  * @输入参数   uint8_t ack:应答信号
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
uint8_t I2C_Read_Byte(uint8_t ack)
{
   uint8_t i=0,receive=0;

   I2C_SDA_IN();
   for(i=0;i<8;i++)
   {
   	I2C_SCL_L;            //产生时钟接收数据
		delay_us(2);
		I2C_SCL_H;
		receive<<=1;          
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))//判断接收数据信号
		   receive++;
		delay_us(1);	
   }

   	if(ack==0)            //是否有应答信号
	   	I2C_NAck();
	  else
			I2C_Ack();

	return receive;         //返回接收到的字节
}



