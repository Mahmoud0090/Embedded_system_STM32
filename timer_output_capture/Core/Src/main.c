#include "main.h"
#include <stdio.h>
#include <string.h>


void Gpio_Init(void);
void Error_handler(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void Timer2_init(void);
void UART2_Init(void);

TIM_HandleTypeDef htimer2;

UART_HandleTypeDef huart2;

/*for computing 500Hz ouput
 * the t=1/f = 1/500 = 0.002s so the pulse is 0.002/2 = 0.001
 * so that mean the freq is 1/0.001 = 1Khz
 * with 25Mhz timer clock so pulse1_value = 25Mhz/1khz = 25khz*/



uint32_t pulse1_value = 25000; //for 500Hz output
uint32_t pulse2_value = 12500;
uint32_t pulse3_value = 6250;
uint32_t pulse4_value = 3125;

uint32_t ccr_content;

int main(void)
{
	HAL_Init();

	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);

	Gpio_Init();

	UART2_Init();

	Timer2_init();

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}

	while(1);

	return 0;
}


void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;

	RCC_ClkInitTypeDef clk_init;

	uint32_t Flatency = 0;


	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	osc_init.HSEState = RCC_HSE_ON;
	osc_init.LSEState = RCC_LSE_ON;
	//osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;


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
	TIM_OC_InitTypeDef tim2_OC_init;

	memset(&tim2_OC_init , 0 , sizeof(TIM_OC_InitTypeDef));

	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;

	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK)
	{
		Error_handler();
	}

	tim2_OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2_OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	tim2_OC_init.Pulse = pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_OC_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	tim2_OC_init.Pulse = pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_OC_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	tim2_OC_init.Pulse = pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_OC_init, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	tim2_OC_init.Pulse = pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_OC_init, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}
}



void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*TIM2 CH1 toggling with frequency 500Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim , TIM_CHANNEL_1 , ccr_content + pulse1_value);
	}

	/*TIM2 CH2 toggling with frequency 1000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim , TIM_CHANNEL_2 , ccr_content + pulse2_value);
	}

	/*TIM2 CH3 toggling with frequency 2000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim , TIM_CHANNEL_3 , ccr_content + pulse3_value);
	}

	/*TIM2 CH4 toggling with frequency 4000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim , TIM_CHANNEL_4 , ccr_content + pulse4_value);
	}
}

void Error_handler(void)
{
	while(1);
}

