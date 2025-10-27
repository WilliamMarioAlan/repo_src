/**
  ******************************************************************************
  * @file    i2c.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月3日
  * @brief   i2c驱动头文件
  ******************************************************************************
  */

#include "stm32f10x.h"

int My_I2C_SendBytes(I2C_TypeDef *I2Cx, uint8_t Addr, const uint8_t *pData, uint16_t Size);
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pBuffer, uint16_t Size);
