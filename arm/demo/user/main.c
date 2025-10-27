#include "stm32f10x.h"

void SystemClock_init(void);
void delay_ms(uint32_t ms);


int main(void){
	SystemClock_init();
	//开启APB2 High Speed Bus 时钟 GPIO C组
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	//init IO 为通用输出开漏模式
	GPIO_InitTypeDef GPIO_initStruct;
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_initStruct);


	while(1){
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 输出低电平
		delay_ms(1000);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);  // 释放为高阻态外部上拉到高电平
		delay_ms(1000);
		
	}
}


void delay_ms(uint32_t nms)
{
    SysTick->LOAD = 9000 * nms;      // 72MHz / 8 = 9MHz，每ms需要9000个tick
    SysTick->VAL  = 0x00;            // 清空当前计数
    SysTick->CTRL = 0x01;            // 启动SysTick，使用HCLK/8

    // 等待COUNTFLAG置1（倒计数完成）
    while(!(SysTick->CTRL & (1 << 16)));

    SysTick->CTRL = 0x00;            // 关闭SysTick
    SysTick->VAL  = 0x00;            // 清空寄存器
}


void SystemClock_init(void) {
	//set flash
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_SetLatency(FLASH_Latency_2);
	
	//1.enable HSE
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	//2.config and enable PLL
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//3.config AHB APB1 APB2 分频器
	RCC_HCLKConfig(RCC_SYSCLK_Div1); // HCLK = SYSCLK / 1
	RCC_PCLK1Config(RCC_HCLK_Div2); // PCLK1 = HCLK / 2
	RCC_PCLK2Config(RCC_HCLK_Div1); // PCLK2 = HCLK / 1
	//4.switch clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
}
