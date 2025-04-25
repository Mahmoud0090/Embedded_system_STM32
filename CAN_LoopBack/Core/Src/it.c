/*
 * it.c
 *
 *  Created on: Apr 24, 2025
 *      Author: User
 */
#include "it.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

