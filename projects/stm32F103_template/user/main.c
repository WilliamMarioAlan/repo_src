#include "stm32f10x.h"

int main(void)
{
	while(1)
	{
		//开启APB2 High Speed Bus 时钟 GPIO C组
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		//init IO 为通用输出开漏模式
		GPIO_InitTypeDef GPIO_initStruct = {0};
		GPIO_initStruct.GPIO_Pin = GPIO_Pin_13;
		GPIO_initStruct.GPIO_Mode = GPIO_Speed_2MHz;

		GPIO_Init(GPIOC,&GPIO_initStruct);


	}
}

void SystemClock_init(void) {
	//set flash

	
	//1.enable HSE
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	//2.config and enable PLL
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//3.config AHB APB1 APB2 分频器

	//4.switch clock source
}
