/**
  ******************************************************************************
  * @file    spi.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月12日
  * @brief   spi驱动头文件
  ******************************************************************************
  */
	
#include "stm32f10x.h"	

//@ 使用SPI总线收发数据
void My_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size);
