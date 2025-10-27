/**
  ******************************************************************************
  * @file    spi.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月12日
  * @brief   spi驱动源文件
  ******************************************************************************
  */
	
#include "spi.h"

//
// @简介：使用SPI总线以主机的身份收发数据
//
// @参数：SPIx    - 所用的SPI接口的名称，可以填SPI1或SPI2
// @参数：pDataTx - 要发送的数据（数组）
// @参数：pDataRx - 接收数据缓冲区（数组），从从机接收到的数据被保存在这个参数当中
// @参数：Size    - 要收发数据的数量，以字节为单位
//
void My_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size)
{
	if(Size == 0) return;
	
	// #1. 闭合总开关
	SPI_Cmd(SPIx, ENABLE);
	
	// #2. 写入第一个字节
	SPI_I2S_SendData(SPIx, pDataTx[0]);
	
	// #3. 读写Size-1个字节
	for(uint16_t i=0; i<Size-1; i++)
	{
		// 向TDR写数据
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		
		SPI_I2S_SendData(SPIx, pDataTx[i+1]);
		
		// 从RDR读数据
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		
		pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
	}
	
	// #4. 读取最后一个字节
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	
	pDataRx[Size-1] = SPI_I2S_ReceiveData(SPIx);
	
	// #5. 断开总开关
	SPI_Cmd(SPIx, DISABLE);
}
