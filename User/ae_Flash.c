/*******************************************************************************
** �ļ���: 		ae_Flash.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.70
** ����: 		  aelab
** ��������: 	2016-11-21
** ����:		  Flash���ݴ���
** ����ļ�:	ae_Flash.h
** �޸���־��	2016-11-21   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"


/* �궨�� --------------------------------------------------------------------*/
#define ADDRESS_STORAGE 0x800F800                                     //����double���ݵ�ַ(ҳ��ַ)
#define ADDRESS_STORAGE_UINT32 ADDRESS_STORAGE + 160                  //����uint32���ݵ�ַ


/*******************************************************************************
  * @��������	  Double_FlashWrite
  * @����˵��   ��Flash�б���double����
  * @�������   double data����Ҫ���������
								uint16_t num�������λ��
  * @�������   ��
  * @���ز���   ��
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
  * @��������	  Double_FlashRead
  * @����˵��   ��Flash��ȡ����
  * @�������   uint16_t num�������λ��
  * @�������   ��
  * @���ز���   Flash�е�double����
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
  * @��������	  uint32_FlashWrite
  * @����˵��   ��Flash�б���uint32����
  * @�������   uint32_t data����Ҫ���������
								uint16_t num�������λ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void uint32_FlashWrite(uint32_t data, uint16_t num)
{
	FLASH_ProgramWord((ADDRESS_STORAGE_UINT32 + num * 4), data);	
}

/*******************************************************************************
  * @��������	  uint32_FlashRead
  * @����˵��   ��Flash�б���uint32����
  * @�������   uint16_t num�������λ��
  * @�������   ��
  * @���ز���   Flash�е�uint32����
  *****************************************************************************/
uint32_t uint32_FlashRead(uint16_t num)
{
	return *(__IO uint32_t*) (ADDRESS_STORAGE_UINT32 + num * 4);
}

/*******************************************************************************
  * @��������	  Storage
  * @����˵��   �����������
  * @�������   *data1��double����
								*data2��uint32_t����
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void Storage(double *data1, uint32_t *data2)
{
	uint16_t i;
	
	RCC_HSICmd(ENABLE);          //���� HSI
	FLASH_Unlock();              //���� FLASH ���ƿ�
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); //���һЩ��־λ
	FLASH_ErasePage(ADDRESS_STORAGE);  //������ʼ��ַΪ ADDRESS_STORAGE �� FLASH ҳ 

	for(i=0;i<20;i++)
	{
		Double_FlashWrite(data1[i], i);
	}
	for(i=0;i<5;i++)
	{
		uint32_FlashWrite(data2[i], i);
	}

	FLASH_Lock();                //���� FLASH ���ƿ�
		
}

/*******************************************************************************
  * @��������	  Reader
  * @����˵��   ��ȡ��������
  * @�������   *data1��double����
								*data2��uint32_t����
  * @�������   ��
  * @���ز���   ��
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



/***********************************�ļ�����***********************************/



