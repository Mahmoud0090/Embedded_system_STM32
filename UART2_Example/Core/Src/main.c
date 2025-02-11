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
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t);

UART_HandleTypeDef huart2;

char* User_data = "The application is running \r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	uint16_t len_data = strlen(User_data);

	if (HAL_UART_Transmit(&huart2 , (uint8_t *)User_data , len_data , HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

	uint8_t rcvd_data;
	uint8_t data_buffer[100];
	uint32_t count = 0;

	while(1)
	{
		HAL_UART_Receive(&huart2 , &rcvd_data , 1, HAL_MAX_DELAY);
		if(rcvd_data == '\r')
		{
			break;
		}
		else
		{
			data_buffer[count++] = convert_to_capital(rcvd_data);
		}
	}

	data_buffer[count++] = '\r';

	HAL_UART_Transmit(&huart2 , data_buffer , count , HAL_MAX_DELAY);

	while(1);

	return 0;
}

void SystemClockConfig(void)
{



}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//there is a problem
		Error_handler();
	}
}

uint8_t convert_to_capital(uint8_t data)
{
	if(data >= 'a' && data<= 'z')
	{
		data = data - ('a' - 'A');
	}
	return data;
}

void Error_handler(void)
{

	while(1);

}
