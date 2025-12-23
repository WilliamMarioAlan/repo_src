#include "stm32f10x.h"

void SystemClock_init(void);
void delay_ms(uint32_t ms);


#define K_UP GPIO_Pin_0 //PA0
#define K_DOWN GPIO_Pin_3 //PE3
#define K_LEFT GPIO_Pin_2 //PE2
#define K_RIGHT GPIO_Pin_4 //PE4
#define k_up GPIO_ReadInputDataBit(GPIOA,K_UP) //»ñÈ¡°´¼üµÄ×´Ì¬
#define k_down GPIO_ReadInputDataBit(GPIOE,K_DOWN)
#define k_left GPIO_ReadInputDataBit(GPIOE,K_LEFT)
#define k_right GPIO_ReadInputDataBit(GPIOE,K_RIGHT)


void key_pros(void);
void key_init(void);


int main(void){
	SystemClock_init();
	//开启APB2 High Speed Bus 时钟 GPIO C组

	LED_Init();		  	//³õÊ¼»¯ÓëLEDÁ¬½ÓµÄÓ²¼þ
	key_init();
	GPIO_Write(GPIOC,(u16)(0xff)); //LED Ãð
	while(1)
	{
		key_pros();
	}
}

void delay_ms(uint32_t nms) {
  
}

// void delay_ms(uint32_t nms)
// {
//     SysTick->LOAD = 9000 * nms;      // 72MHz / 8 = 9MHz，每ms需要9000个tick
//     SysTick->VAL  = 0x00;            // 清空当前计数
//     SysTick->CTRL = 0x01;            // 启动SysTick，使用HCLK/8
//
//     // 等待COUNTFLAG置1（倒计数完成）
//     while(!(SysTick->CTRL & (1 << 16)));
//
//     SysTick->CTRL = 0x00;            // 关闭SysTick
//     SysTick->VAL  = 0x00;            // 清空寄存器
// }


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




void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //ÉùÃ÷Ò»¸ö½á¹¹Ìå±äÁ¿£¬ÓÃÀ´³õÊ¼»¯ GPIO
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=K_UP; //Ñ¡ÔñÄãÒªÉèÖÃµÄ IO ¿Ú
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//ÏÂÀ­ÊäÈë
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //ÉèÖÃ´«ÊäËÙÂÊ
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ³õÊ¼»¯ GPIO */
	
	GPIO_InitStructure.GPIO_Pin=K_DOWN|K_LEFT|K_RIGHT;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //ÉÏÀ­ÊäÈë
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,K_UP); //¶Ô K_UP ³õÊ¼»¯Êä³ö 0
}

void key_pros(void) //°´¼ü´¦Àíº¯Êý
{
	if(k_up==1) //ÅÐ¶Ï°´¼ü k_up ÊÇ·ñ°´ÏÂ
	{
		delay_ms(10); //Ïû¶¶´¦Àí
		if(k_up==1) //ÔÙ´ÎÅÐ¶Ï°´¼ü k_up ÊÇ·ñ°´ÏÂ
		{
			GPIO_Write(GPIOC,(u16)0xfe);
		}
		while(k_up); //µÈ´ý°´¼üËÉ¿ª
	}
	if(k_down==0)
	{
		delay_ms(10);
		if(k_down==0)
		{
			GPIO_Write(GPIOC,(u16)(0xfd));
		}
		while(!k_down);
	}
	if(k_left==0)
	{
		delay_ms(10);
		if(k_left==0)
		{
			GPIO_Write(GPIOC,(u16)(0xfb));
		} 
		while(!k_left);
	}
	if(k_right==0)
	{
		delay_ms(10);
		if(k_right==0)
		{
			GPIO_Write(GPIOC,(u16)(0xf7));
		}
		while(!k_right);
	}
}
