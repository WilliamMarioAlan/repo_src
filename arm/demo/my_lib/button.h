/**
  ******************************************************************************
  * @file    button.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2022年9月7日
  * @brief   按钮驱动程序
  ******************************************************************************
  */
	
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "stm32f10x.h"

typedef struct
{
	GPIO_TypeDef *GPIOx;   /* 按钮的端口号。
	                              可以取GPIOA..D中的一个*/
	
	uint16_t GPIO_Pin;     /* 按钮的引脚编号。
	                              可以取GPIO_Pin_0..15中的一个 */
	
	/* 回调函数 */
	void (*button_pressed_cb)(void);           // 按钮按下的回调函数
	void (*button_released_cb)(void);          // 按钮松开的回调函数
	void (*button_clicked_cb)(uint8_t clicks); // 按钮敲击的回调函数
	void (*button_long_pressed_cb)(uint8_t ticks);     // 按钮长按的回调函数
	
	uint32_t LongPressTime; // 按钮长按时间阈值，单位毫秒
	uint32_t LongPressTickInterval; // 长按后持续触发的间隔
	uint32_t ClickInterval; // 连击时间间隔，单位毫秒
	
} Button_InitTypeDef;

typedef struct 
{
	/* 初始化参数 */
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	void (*button_pressed_cb)(void);
	void (*button_released_cb)(void);
	void (*button_clicked_cb)(uint8_t clicks);
	void (*button_long_pressed_cb)(uint8_t ticks);
	uint32_t LongPressThreshold;
	uint32_t LongPressTickInterval;
	uint32_t ClickInterval; 
	
	uint8_t  LastState;     // 按钮上次的状态，0 - 松开，1 - 按下
	uint8_t  ChangePending; // 按钮的状态是否正在发生改变
	uint32_t PendingTime;   // 按钮状态开始变化的时间
	
	uint32_t LastPressedTime;  // 按钮上次按下的时间
	uint32_t LastReleasedTime; // 按钮上次松开的时间
	
	uint8_t LongPressTicks;
	uint32_t LastLongPressTickTime; 
	
	uint8_t ClickCnt;
	
} Button_TypeDef;

   void My_Button_Init(Button_TypeDef *Button, Button_InitTypeDef *Button_InistStruct);
   void My_Button_Proc(Button_TypeDef *Button);
uint8_t MyButton_GetState(Button_TypeDef *Button);

#endif
