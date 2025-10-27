/**
  ******************************************************************************
  * @file    usart.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2024年9月1日
  * @brief   串口源文件
  ******************************************************************************
  */

#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "delay.h"

//
// @简介：使用串口发送一个字节的数据
// 
// @参数 USARTx：串口名称，如USART1, USART2, USART3 ...
// @参数 Data  : 要发送的数据
//
void My_USART_SendByte(USART_TypeDef *USARTx, const uint8_t Data)
{
	My_USART_SendBytes(USARTx, &Data, 1);
}

//
// @简介：使用串口发送多个字节的数据
// 
// @参数 USARTx：串口名称，如USART1, USART2, USART3 ...
// @参数 pData : 要发送的数据（数组）
// @参数 Size  ：要发送数据的数量，单位是字节
//
__weak void My_USART_SendBytes(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size)
{
	if(Size == 0) return;
	
	for(uint16_t i=0; i < Size; i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		
		USART_SendData(USARTx, pData[i]);
	}
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

//
// @简介：通过串口发送一个字符
// 
// @参数 USARTx：串口名称，如USART1, USART2, USART3 ...
// @参数 C     ：要发送的字符
//
void My_USART_SendChar(USART_TypeDef *USARTx, const char C)
{
	My_USART_SendBytes(USARTx, (const uint8_t *)&C, 1);
}

//
// @简介：通过串口发送字符串
// 
// @参数 USARTx：串口名称，如USART1, USART2, USART3 ...
// @参数 Str   ：要发送的字符串
//
void My_USART_SendString(USART_TypeDef *USARTx, const char *Str)
{
	My_USART_SendBytes(USARTx, (const uint8_t *)Str, strlen(Str));
}

//
// @简介：通过串口格式化打印字符串
// 
// @参数 USARTx：串口名称，如USART1, USART2, USART3 ...
// @参数 Format：字符串的格式
// @参数 ...   ：可变参数
//
void My_USART_Printf(USART_TypeDef *USARTx, const char *Format, ...)
{
	char format_buffer[128];
	va_list argptr;
	
	__va_start(argptr, Format);
	
	vsprintf(format_buffer, Format, argptr);
	
	__va_end(argptr);
	
	My_USART_SendString(USARTx, format_buffer);
}


//
// @简介：通过串口读取一字节的数据
// 
// @参数 USARTx  ：串口名称，如USART1, USART2, USART3 ...
// 
// @返回值：读取到的字节
//
uint8_t My_USART_ReceiveByte(USART_TypeDef *USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	
	return USART_ReceiveData(USARTx);
}

//
// @简介：通过串口读取多个字节的数据
// 
// @参数 USARTx  ：串口名称，如USART1, USART2, USART3 ...
// @参数 pDataOut：输出参数，读取到的数据将输出到此数组当中
// @参数 Size    ：需要读取的字节数量
// @参数 Timeout ：超时时间，单位是毫秒，负数表示无限长。如果超时时间内没有读取完成则返回。
// 
// @返回值：实际读取到的数据数量
//
__weak uint16_t My_USART_ReceiveBytes(USART_TypeDef *USARTx, uint8_t *pDataOut, uint16_t Size, int Timeout)
{
	uint32_t expireTime;
	
	Delay_Init();
	
	if(Timeout >= 0)
	{
		expireTime = GetTick() + Timeout; // 计算过期时间，过期时间 = 当前时间+Timeout
	}
	
	uint16_t i = 0;
	
	do
	{
		if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET)
		{
			pDataOut[i++] = USART_ReceiveData(USARTx);
			
			if(i==Size) break;
		}
	}
	while(Timeout < 0 || GetTick() < expireTime); // 判断是否超时
	
	return i;
}


//
// @简介：通过串口读取一行字符串
// 
// @参数 USARTx       ：串口名称，如USART1, USART2, USART3 ...
// @参数 pStrOut      ：输出参数，读取到的数据将输出到此数组当中
// @参数 MaxLength    ：字符串的最大长度
// @参数 LineSeperator：行分隔符 LINE_SEPERATOR_CR   - 回车 \r
//                               LINE_SEPERATOR_LF   - 换行 \n
//                               LINE_SEPERATOR_CRLF - 回车+换行 \r\n
// @参数 Timeout      ：超时时间，单位是毫秒，负数表示无限长。如果超时时间内没有读取完成则返回
// 
// @返回值：0 - 成功读到一行字符串
//         -1 - 超时（Timeout内未读到一行完整的字符串）
//         -2 - 超过字符串的最大长度（字符串的最大长度用MaxLength参数设置）
//
int My_USART_ReceiveLine(USART_TypeDef *USARTx, char *pStrOut, uint16_t MaxLength, uint16_t LineSeperator, int Timeout)
{
	// 如果最大长度都不足以装下行分隔符
	// 就直接返回失败
	if(MaxLength < 2 || ((LineSeperator == LINE_SEPERATOR_CRLF) && (MaxLength < 1)))
	{
		return -2;
	}
	
	int ret = -1;
	uint32_t expireTime;
	
	Delay_Init(); // 要用到单片机当前时间，所以初始化延迟函数
	
	if(Timeout >= 0)
	{
		expireTime = GetTick() + Timeout; // 计算过期时间，过期时间 = 当前时间+Timeout
	}
	
	uint16_t i = 0;
	
	do
	{
		if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET)
		{
			char c = (char)USART_ReceiveData(USARTx);
			pStrOut[i++] = c;
			
			if(LineSeperator == LINE_SEPERATOR_CR && c == '\r') // \r
			{
				ret = 0;
				break;
			}
			else if(LineSeperator == LINE_SEPERATOR_LF && c == '\n') // \n
			{
				ret = 0;
				break;
			}
			else if(i >= 2 && pStrOut[i-2] == '\r' && c == '\n') // \r\n
			{
				ret = 0;
				break;
			}
			
			if(i == MaxLength) // 超过最大长度
			{
				ret = -2;
				break;
			}
		}
	}
	while(Timeout < 0 || GetTick() < expireTime); // 判断是否超时
	
	// 在字符串末尾增加'\0'
	if(i == MaxLength)
	{
		pStrOut[i-1] = '\0';
	}
	else
	{
		pStrOut[i] = '\0';
	}
	
	return ret;
}
