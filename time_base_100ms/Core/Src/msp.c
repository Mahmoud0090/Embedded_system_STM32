/*
 * msp.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */

#include "main.h"


void HAL_MspInit(void)
{
  //here will do low level processor specific init

	//1- set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2- enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x07 << 16; // usg fault , memory fault and bus fault system exceptions

	//3- configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn , 0 , 0);
	HAL_NVIC_SetPriority(BusFault_IRQn , 0 , 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn , 0 , 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htimer)
{
	//1 - enable the clock for TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2 - enable the IRQ for TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	// set up the priority TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
}
