/*******************************************************************************
** 文件名: 		ae_Flash.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.70
** 作者: 		  aelab
** 生成日期: 	2016-11-21
** 功能:		  Flash数据储存
** 相关文件:	ae_Flash.h
** 修改日志：	2016-11-21   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"


/* 宏定义 --------------------------------------------------------------------*/
#define ADDRESS_STORAGE 0x800F800                                     //储存double数据地址(页地址)
#define ADDRESS_STORAGE_UINT32 ADDRESS_STORAGE + 160                  //储存uint32数据地址


/*******************************************************************************
  * @函数名称	  Double_FlashWrite
  * @函数说明   往Flash中保存double数据
  * @输入参数   double data：需要保存的数据
								uint16_t num：保存的位置
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Double_FlashWrite(double data, uint16_t num)
{
	union
  { 
	  double fx;  
		uint32_t buf [2];
	} flashbuf ;
		
	flashbuf.fx = data;
	
	FLASH_ProgramWord((ADDRESS_STORAGE + num * 8), flashbuf.buf[0]);
	FLASH_ProgramWord((ADDRESS_STORAGE + num * 8 + 4), flashbuf.buf[1]);
	
}

/*******************************************************************************
  * @函数名称	  Double_FlashRead
  * @函数说明   从Flash读取数据
  * @输入参数   uint16_t num：保存的位置
  * @输出参数   无
  * @返回参数   Flash中的double数据
  *****************************************************************************/
double Double_FlashRead(uint16_t num)
{
	union
  { 
	  double fx;  
		uint32_t buf [2];
	} flashbuf ;
	
	flashbuf.buf[0] = *(__IO uint32_t*) (ADDRESS_STORAGE + num * 8);
	flashbuf.buf[1] = *(__IO uint32_t*) (ADDRESS_STORAGE + num * 8 + 4);
	
	return flashbuf.fx;
}

/*******************************************************************************
  * @函数名称	  uint32_FlashWrite
  * @函数说明   往Flash中保存uint32数据
  * @输入参数   uint32_t data：需要保存的数据
								uint16_t num：保存的位置
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void uint32_FlashWrite(uint32_t data, uint16_t num)
{
	FLASH_ProgramWord((ADDRESS_STORAGE_UINT32 + num * 4), data);	
}

/*******************************************************************************
  * @函数名称	  uint32_FlashRead
  * @函数说明   往Flash中保存uint32数据
  * @输入参数   uint16_t num：保存的位置
  * @输出参数   无
  * @返回参数   Flash中的uint32数据
  *****************************************************************************/
uint32_t uint32_FlashRead(uint16_t num)
{
	return *(__IO uint32_t*) (ADDRESS_STORAGE_UINT32 + num * 4);
}

/*******************************************************************************
  * @函数名称	  Storage
  * @函数说明   保存程序数据
  * @输入参数   *data1：double数据
								*data2：uint32_t数据
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Storage(double *data1, uint32_t *data2)
{
	uint16_t i;
	
	RCC_HSICmd(ENABLE);          //开启 HSI
	FLASH_Unlock();              //解锁 FLASH 控制块
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); //清除一些标志位
	FLASH_ErasePage(ADDRESS_STORAGE);  //擦除起始地址为 ADDRESS_STORAGE 的 FLASH 页 

	for(i=0;i<20;i++)
	{
		Double_FlashWrite(data1[i], i);
	}
	for(i=0;i<5;i++)
	{
		uint32_FlashWrite(data2[i], i);
	}

	FLASH_Lock();                //锁定 FLASH 控制块
		
}

/*******************************************************************************
  * @函数名称	  Reader
  * @函数说明   读取程序数据
  * @输入参数   *data1：double数据
								*data2：uint32_t数据
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Reader(double *data1, uint32_t *data2)
{
	uint16_t i;
	
	for(i=0;i<20;i++)
	{
		data1[i] = Double_FlashRead(i);
	}
	for(i=0;i<5;i++)
	{
		data2[i] = uint32_FlashRead(i);
	}
		
}



/***********************************文件结束***********************************/



