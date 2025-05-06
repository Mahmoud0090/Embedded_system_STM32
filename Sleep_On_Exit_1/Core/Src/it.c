#include "it.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htimer6;


void SysTick_Handler(void) //systick interrupt handler
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();
}


void TIM6_DAC_IRQHandler(void)
{
	//__HAL_RCC_USART2_CLK_ENABLE();
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_TIM_IRQHandler(&htimer6);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	//__HAL_RCC_USART2_CLK_DISABLE();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
