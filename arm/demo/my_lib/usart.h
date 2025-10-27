/**
  ******************************************************************************
  * @file    usart.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月1日
  * @brief   串口头文件
  ******************************************************************************
  */
	
#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"

#define LINE_SEPERATOR_CR   0x00 // 回车 \r
#define LINE_SEPERATOR_LF   0x01 // 换行 \n
#define LINE_SEPERATOR_CRLF 0x02 // 回车+换行 \r\n

void My_USART_SendByte(USART_TypeDef *USARTx, const uint8_t Data);
void My_USART_SendBytes(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size);
void My_USART_SendChar(USART_TypeDef *USARTx, const char C);
void My_USART_SendString(USART_TypeDef *USARTx, const char *Str);
void My_USART_Printf(USART_TypeDef *USARTx, const char *Format, ...);

 uint8_t My_USART_ReceiveByte(USART_TypeDef *USARTx);
uint16_t My_USART_ReceiveBytes(USART_TypeDef *USARTx, uint8_t *pDataOut, uint16_t Size, int Timeout);
     int My_USART_ReceiveLine(USART_TypeDef *USARTx, char *pStrOut, uint16_t MaxLength, uint16_t LineSeperator, int Timeout);

#endif

