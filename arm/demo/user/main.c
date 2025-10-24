#include "stm32f10x.h"

#define K_UP GPIO_Pin_0//PAO
#define K_DOWN GPIO_Pin_3 //PE3
#define K_LEFT GPIO_Pin_2 //PE2
#define K_RIGHT GPIO_Pin_4 //PE4

#define k_up GPIO_ReadInputDataBit(GPIOA,K_UP) / REFERER AS
#define k_down GPIO_ReadInputDataBit(GPIOE,K_DOWN)
#define k_left GPIO_ReadInputDataBit(GPIOE,K_LEFT)
#define k_right GPIO_ReadInputDataBit(GPIOE,K_RIGHT)


void key_init();  //
void key_pros(); //°´¼ü´¦Àíº¯Êý
void SystemClock_init(); 




int main(void)
{
	SystemClock_init();
	//开启APB2 High Speed Bus 时钟 GPIO C组
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	//init IO 为通用输出开漏模式
	GPIO_InitTypeDef GPIO_initStruct;
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_initStruct)
	while(1)
	{
		
		
	}
}


void key_init()  //
{
  GPIO_InitTypeDef GPIO_InitStructure; //F24Y—7 28a, FSR 4R tL GPIO
  SystemlInit();
  /* FER GPIO MEP */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
  /* BCS GPIO hytistAl ilo */
  GPIO_InitStructure.GPIO_Pin=K_UP; /H&PREREN 10 0
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;// FAA
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; // BAER
  GPIO_Init(GPIOA,&GPIO_InitStructure); /* #4814 GPIO */
  GPIO_InitStructure.GPIO_Pin=K_DOWN | K_LEFT|K_RIGHT;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //_ Liz
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_5OMH#z;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA,K_UP);  //*t K_UP 428444714 0
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
	while(RCC_GetSYSCLKSource() != @x@8);
}
