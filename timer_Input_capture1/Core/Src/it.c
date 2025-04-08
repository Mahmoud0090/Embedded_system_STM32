/*
 * it.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */
#include "it.h"

extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void) //systick interrupt handler
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}

