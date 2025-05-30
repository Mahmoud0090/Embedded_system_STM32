/*
 * main.c
 *
 *  Created on: Jan 17, 2025
 *      Author: User
 */

#include "main.h"
#include <stdio.h>
#include <string.h>


void Gpio_Init(void);
void Error_handler(void);
void SystemClock_Config(uint8_t clock_freq);
void Timer2_init(void);
void LSE_Configuration(void);
void UART2_Init(void);

TIM_HandleTypeDef htimer2;

UART_HandleTypeDef huart2;

uint32_t input_capture[2] = {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;
double capture_difference = 0;



int main(void)
{
	double timer2_cnt_freq = 0;
	double timer2_cnt_res = 0;
	double user_signal_time_period = 0;
	double user_signal_freq=0;
	char user_msg[100];

	HAL_Init();

	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);

	Gpio_Init();

	UART2_Init();

	Timer2_init();

	LSE_Configuration();

	HAL_TIM_IC_Start_IT(&htimer2, TIM_CHANNEL_1);

	while(1)
	{
		if(is_capture_done)
		{
			if(input_capture[1] > input_capture[0])
				capture_difference = input_capture[1] - input_capture[0];
			else
				capture_difference = (0xFFFFFFFF - input_capture[0]) + input_capture[1];

			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2)/ (htimer2.Init.Prescaler + 1);
			timer2_cnt_res = 1/timer2_cnt_freq;
			user_signal_time_period = capture_difference * timer2_cnt_res;
			user_signal_freq = 1/user_signal_time_period;

			sprintf(user_msg , "frequency of the signal applied = %f\r\n", user_signal_freq);

			HAL_UART_Transmit(&huart2, (uint8_t*)user_msg, strlen(user_msg), HAL_MAX_DELAY);

			is_capture_done = FALSE;
		}
	}

	return 0;
}


void SystemClock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;

	RCC_ClkInitTypeDef clk_init;

	uint32_t Flatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.LSEState = RCC_LSE_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;
			clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
								 RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_1WS;

			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
								 RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
								 RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_3WS;

			break;
		}

		default:
			return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK)
	{
		Error_handler();
	}

	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void LSE_Configuration(void)
{
	//for getting the clock as output from gpio pin (LSE clock in this case)
	//gpio config is already is inside the below func
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
	{
	    // LSE not ready — maybe crystal not connected?
	    Error_handler();
	}
}

void Gpio_Init(void)
{
	GPIO_InitTypeDef gpioLed;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpioLed.Pin = GPIO_PIN_5;
	gpioLed.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLed.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &gpioLed);
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


void Timer2_init(void)
{
	TIM_IC_InitTypeDef timer2IC_Config;

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP; //this is default
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1; // time count clk = (tim clk)/(prescaler + 1)

	if(HAL_TIM_IC_Init(&htimer2)!= HAL_OK)
	{
		Error_handler();
	}

	timer2IC_Config.ICFilter = 0;
	timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if(HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1)!= HAL_OK)
	{
		Error_handler();
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(!is_capture_done)
	{
		if(count == 1)
		{
			input_capture[0] = __HAL_TIM_GET_COMPARE(htim , TIM_CHANNEL_1);
			count++;
		}
		else if(count == 2)
		{
			input_capture[1] = __HAL_TIM_GET_COMPARE(htim , TIM_CHANNEL_1);
			count = 1;
			is_capture_done = TRUE;
		}
	}
}

void Error_handler(void)
{
	while(1);
}

