/*******************************************************************************
** 文件名: 		ae_mpu6050.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5.20
** 作者: 		  aelab
** 生成日期: 	2015-06-11
** 功能:		  读取mpu6050结果
** 相关文件:	I2C.c
							SysTickDelay.c
** 修改日志：	2015-06-11   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "SysTickDelay.h"
#include "ae_I2C.h"
#include "ae_mpu6050.h"



/*******************************************************************************
  * @函数名称	  PMU6050_WriteReg
  * @函数说明   往MPU6050写入数据
  * @输入参数   uint8_t reg_add：MPU6050寄存器地址
								uint8_t reg_dat：数据
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void PMU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	I2C_Start();
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS); //I2C发送MPU6050写地址
	I2C_Wait_Ack();
	I2C_Send_Byte(reg_add);               //I2C发送MPU6050寄存器地址
	I2C_Wait_Ack();
	I2C_Send_Byte(reg_dat);               //I2C发送数据
	I2C_Wait_Ack();
	I2C_Stop();
}

/*******************************************************************************
  * @函数名称	  PMU6050_ReadData
  * @函数说明   从MPU6050读数据
  * @输入参数   uint8_t reg_add：MPU6050寄存器地址								
								uint8_t num：连续都num个数据
  * @输出参数   uint8_t*Read：数据
  * @返回参数   无
*******************************************************************************/
void PMU6050_ReadData(uint8_t reg_add,uint8_t*Read,uint8_t num)
{
	unsigned char i;
	
	I2C_Start();
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS);   //I2C发送MPU6050寄存器写地址
	I2C_Wait_Ack();
	I2C_Send_Byte(reg_add);
	I2C_Wait_Ack();
	
	I2C_Start();
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS+1);  //I2C发送MPU6050寄存器读地址
	I2C_Wait_Ack();
	
	for(i=0;i<(num-1);i++){                  //连续读num个数据
		*Read=I2C_Read_Byte(1);
		Read++;
	}
	*Read=I2C_Read_Byte(0);
	I2C_Stop();
}

/*******************************************************************************
  * @函数名称	  MPU6050_Init
  * @函数说明   初始化MPU6050
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void MPU6050_Init(void)
{
	myI2C_Init();       //初始化I2C
	
	PMU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //解除休眠状态
	PMU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	     //设置陀螺仪采样率
	PMU6050_WriteReg(MPU6050_RA_CONFIG , 0x00);	         //配置MPU6050
	PMU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	   //配置加速度传感器工作模式
	PMU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);      //陀螺仪自检及测量范围
}

/*******************************************************************************
  * @函数名称	  MPU6050ReadAcc
  * @函数说明   读取加速度值
  * @输入参数   无
  * @输出参数   uint16_t *accData：加速度值的数组
  * @返回参数   无
*******************************************************************************/
void MPU6050ReadAcc(uint16_t *accData)
{
    uint8_t buf[6];
    PMU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
	  accData[2] = (buf[4] << 8) | buf[5];
}

/*******************************************************************************
  * @函数名称	  MPU6050ReadGyro
  * @函数说明   读取陀螺仪值
  * @输入参数   无
  * @输出参数   uint16_t *gyroData：角速度值的数组
  * @返回参数   无
*******************************************************************************/
void MPU6050ReadGyro(uint16_t *gyroData)
{
    uint8_t buf[6];
    PMU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
		gyroData[1] = (buf[2] << 8) | buf[3];
		gyroData[2] = (buf[4] << 8) | buf[5];
}

/*******************************************************************************
  * @函数名称	  MPU6050_ReturnTemp
  * @函数说明   读取温度值
  * @输入参数   无
  * @输出参数   uint16_t *Temperature：温度值
  * @返回参数   无
*******************************************************************************/
void MPU6050_ReturnTemp(uint16_t *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	PMU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13; //转化为可读温度值
}

