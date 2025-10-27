/**
  ******************************************************************************
  * @file    button.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2022年9月7日
  * @brief   按钮驱动程序
  ******************************************************************************
  */

#include "button.h"
#include "delay.h"

#define BUTTON_SETTLING_TIME             10   // 按钮消抖延迟
#define BUTTON_CLICK_INTERVAL            200  // 按钮多击时每次点击的时间最大时间间隔
#define BUTTON_LONG_PRESS_THRESHOLD      1000 // 按钮长按最小时间
#define BUTTON_LONG_PRESS_TICK_INTERNVAL 100  // 长按后持续触发的时间间隔

static void OnButtonPressed(Button_TypeDef *Button);
static void OnButtonReleased(Button_TypeDef *Button);
static void OnButtonEveryPolled(Button_TypeDef *Button, uint8_t State, uint32_t currentTime);
static void GPIOClockCmd(GPIO_TypeDef *GPIOx, uint8_t Enable);

// 
// @简介：用于初始化按钮的驱动
// @参数：Button - 按钮的名称
// @返回值：无
//
void My_Button_Init(Button_TypeDef *Button, Button_InitTypeDef *Button_InistStruct)
{
	Button->GPIOx = Button_InistStruct->GPIOx;
	Button->GPIO_Pin = Button_InistStruct->GPIO_Pin;
	Button->button_pressed_cb = Button_InistStruct->button_pressed_cb;
	Button->button_released_cb = Button_InistStruct->button_released_cb;
	Button->button_clicked_cb = Button_InistStruct->button_clicked_cb;
	Button->button_long_pressed_cb = Button_InistStruct->button_long_pressed_cb;
	Button->LongPressThreshold = Button_InistStruct->LongPressTime;
	Button->ClickInterval = Button_InistStruct->ClickInterval;
	Button->LongPressTickInterval = Button_InistStruct->LongPressTickInterval;
	
	// #1. 使能GPIOx的时钟
	GPIOClockCmd(Button->GPIOx, 1);
	
	// #2. 初始化IO引脚
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = Button->GPIO_Pin;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(Button->GPIOx, &gpio_init_struct);
	
	if(Button->LongPressThreshold == 0)
	{
		Button->LongPressThreshold = BUTTON_LONG_PRESS_THRESHOLD;
	}
	
	if(Button->LongPressTickInterval == 0)
	{
		Button->LongPressTickInterval = BUTTON_LONG_PRESS_TICK_INTERNVAL;
	}
	
	if(Button->ClickInterval == 0)
	{
		Button->ClickInterval = BUTTON_CLICK_INTERVAL;
	}
	
	Button->LastState = 0; // 初始状态下假设按钮是松开的
	Button->ChangePending = 0; 
	Button->PendingTime = 0;
	Button->LastPressedTime = 0;
	Button->LastReleasedTime = 0;
	Button->LongPressTicks = 0;
	Button->ClickCnt = 0;
}

// 
// @简介：按钮的进程函数
// @参数：Button - 按钮的名称
// @注意：该方法需要在main函数的while循环中调用
//
void My_Button_Proc(Button_TypeDef *Button)
{
	uint8_t currentState;
	
	uint32_t currentTime = GetTick(); // 获取当前时间
	
	// 按键消抖
	if(Button->ChangePending)
	{
		if (currentTime >= Button->PendingTime + BUTTON_SETTLING_TIME) // 已渡过按钮抖动时间
		{
			currentState = GPIO_ReadInputDataBit(Button->GPIOx, Button->GPIO_Pin) == Bit_RESET ? 1 : 0;
			
			if(currentState != Button->LastState)
			{
				if(currentState == 1) 
					OnButtonPressed(Button); // #1. 按钮按下
				else 
					OnButtonReleased(Button); // #2. 按钮松开
			}
			Button->LastState = currentState;
			Button->ChangePending = 0;
		}
	}
	else
	{
		currentState = GPIO_ReadInputDataBit(Button->GPIOx, Button->GPIO_Pin) == Bit_RESET ? 1 : 0;
		
		if(currentState != Button->LastState)
		{
			Button->PendingTime = currentTime;
			Button->ChangePending = 1;
		}
	}
	
	OnButtonEveryPolled(Button, Button->LastState, currentTime); // #3. 按钮状态被检测
}

//
// @简介：返回按钮的当前状态
//
// @返回值：0 - 按钮松开  1 - 按钮按下
//
uint8_t MyButton_GetState(Button_TypeDef *Button)
{
	return Button->LastState;
}

//
// @简介：处理按钮按下的动作
//
static void OnButtonPressed(Button_TypeDef *Button)
{
	Button->LastPressedTime = GetTick();
	
	// 调用按钮按下的回调函数
	if(Button->button_pressed_cb != 0)
	{
		Button->button_pressed_cb();
	}
}

//
// @简介：处理按钮松开的动作
//
static void OnButtonReleased(Button_TypeDef *Button)
{
	Button->LastReleasedTime = GetTick();
	
	// 调用按钮松开的回调函数
	if(Button->button_released_cb != 0)
	{
		Button->button_released_cb();
	}
	
	// 松开后长按计数清零
	Button->LongPressTicks = 0;
	
	if(Button->LastReleasedTime - Button->LastPressedTime < Button->LongPressThreshold)
	{
		Button->ClickCnt++;
	}
	else
	{
		Button->ClickCnt = 0;
	}
}

//
// @简介：处理每一次按钮轮询的动作
//
static void OnButtonEveryPolled(Button_TypeDef *Button, uint8_t State, uint32_t CurrentTime)
{
	/* 处理按钮长按的动作 */
	
	if(Button->LastState == 1)
	{
		if(Button->LongPressTicks == 0) // 如果长按未被触发
		{
			if(Button->LastPressedTime!= 0 
				&& CurrentTime - Button->LastPressedTime > Button->LongPressThreshold) // 且已超过触发时间
			{
				Button->LongPressTicks = 1;
			
				if(Button->button_long_pressed_cb)
				{
					Button->button_long_pressed_cb(Button->LongPressTicks); // 触发长按回调函数
				}
				
				Button->LastLongPressTickTime = GetTick();
			}
		}
		else
		{
			if(CurrentTime - Button->LastLongPressTickTime > Button->LongPressTickInterval) // 超过Tick间隔
			{
				Button->LastLongPressTickTime = GetTick();
				
				Button->LongPressTicks++;
				
				if(Button->button_long_pressed_cb)
				{
					Button->button_long_pressed_cb(Button->LongPressTicks); // 触发长按回调函数
				}
			}
		}
	}
	
	/* 处理按钮连击动作 */
	
	if(Button->ClickCnt > 0 && Button->LastState == 0 && (GetTick() - Button->LastReleasedTime) > Button->ClickInterval)
	{
		if(Button->button_clicked_cb)
		{
			Button->button_clicked_cb(Button->ClickCnt);
		}
		
		Button->ClickCnt = 0; // 清除连击记录
	}
}

static void GPIOClockCmd(GPIO_TypeDef *GPIOx, uint8_t Enable)
{
	FunctionalState newState = Enable ? ENABLE : DISABLE;
	
	if(GPIOx == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, newState);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, newState);
	}
	else if(GPIOx == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, newState);
	}
	else if(GPIOx == GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, newState);
	}
}
