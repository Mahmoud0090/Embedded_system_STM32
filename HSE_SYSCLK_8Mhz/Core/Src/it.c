/*
 * it.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */
#include "main.h"

void SysTick_Handler(void) //systick interrupt handler
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}

