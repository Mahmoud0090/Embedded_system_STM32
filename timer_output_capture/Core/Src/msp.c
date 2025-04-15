/*
 * msp.c
 *
 *  Created on: Apr 11, 2025
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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//here we are going to do the low level Inits of the USART2 peripheral

	//1- enable the clock for tha USART2 peripheral as well as for GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2- Do the pin muxing configuration
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2; //UART2_Tx
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3; //UART2_Rx
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	//3- enable the IRQ and set up the priority(NVIC setting)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2OC_ch_gpios;
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//configure gpios to behave as timer2 channels 1,2,3,4

	tim2OC_ch_gpios.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Pull = GPIO_PULLUP;
	tim2OC_ch_gpios.Speed = GPIO_SPEED_FREQ_LOW;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2OC_ch_gpios);

	tim2OC_ch_gpios.Pin = GPIO_PIN_2 | GPIO_PIN_10;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Pull = GPIO_PULLUP;
	tim2OC_ch_gpios.Speed = GPIO_SPEED_FREQ_LOW;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &tim2OC_ch_gpios);

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


