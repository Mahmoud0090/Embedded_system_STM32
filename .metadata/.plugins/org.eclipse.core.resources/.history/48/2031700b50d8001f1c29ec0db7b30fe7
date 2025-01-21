/*
 * it.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */
#include "main.h"

extern UART_HandleTypeDef huart2;

void SysTick_Handler(void) //systick interrupt handler
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

}

void USART2_IRQHandler(void)
{

	HAL_UART_IRQHandler(&huart2);
}
