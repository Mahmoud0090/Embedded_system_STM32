/*
 * main.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */
#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void Error_handler(void);
void TIMER6_init(void);
void GPIO_init(void);

TIM_HandleTypeDef htimer6;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	TIMER6_init();
	GPIO_init();

	//let us start timer
	HAL_TIM_Base_Start(&htimer6);
	while(1)
	{
		//loop until the update event flag is set
		while(!(TIM6->SR & TIM_SR_UIF));

			TIM6->SR = 0;
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	}
	return 0;
}

void GPIO_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &ledgpio);
}

void SystemClockConfig(void)
{

}

void TIMER6_init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000-1;

	HAL_TIM_Base_Init(&htimer6);
}

void Error_handler(void)
{

	while(1);

}
