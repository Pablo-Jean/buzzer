/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <malloc.h>

#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	EXAMPLE_BUZZER_START,
	EXAMPLE_BUZZER_STOP,
	EXAMPLE_BUZZER_RINGTONE,
	EXAMPLE_BUZZER_NO_LOOP,
	EXAMPLE_BUZZER_LOOP
}example_buzzer_e;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// if you want to use static memry allocation,
// uncomment below
//#define USE_STATIC_MEM_ALLOCATION



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define PWM_TIM		&htim3
#define PWM_CHN		TIM_CHANNEL_1

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#ifndef USE_STATIC_MEM_ALLOCATION
buzzer_t mBuzzer;
#endif

buzzer_t *Buzzer;
example_buzzer_e example = EXAMPLE_BUZZER_START;
uint8_t nextPattern;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * Start PWM
 */

void pwm_start(){
	HAL_TIM_PWM_Start(PWM_TIM, PWM_CHN);
}

/**
 * Set the Duty Cycle of the PWM
 */
void pwm_set_dc(uint32_t dc){
	uint32_t arr, comp;

	arr = __HAL_TIM_GET_AUTORELOAD(PWM_TIM);
	comp = dc*arr/100;
	__HAL_TIM_SET_COMPARE(PWM_TIM, PWM_CHN, comp);
}

/**
 * Set the Frequency of the PWM
 */
void pwm_set_freq(uint32_t freq){
	uint32_t psc, arr, sys;

	if (freq > 0){
		sys = HAL_RCC_GetSysClockFreq();
		arr = __HAL_TIM_GET_AUTORELOAD(PWM_TIM);
		psc = (sys/(freq*(arr+1)))-1;
		__HAL_TIM_SET_PRESCALER(PWM_TIM, psc);
		pwm_set_dc(50);
	}
	else
		__HAL_TIM_SET_COMPARE(PWM_TIM, PWM_CHN, 0);
}

/**
 * Timer Callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM9){
		buzzer_interrupt(Buzzer);
	}
}

/**
 * Callback from buzzer
 */

void buzzer_end_callback(buzzer_t *buzzer){
	nextPattern = 1;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#ifdef USE_STATIC_MEM_ALLOCATION
	Buzzer = malloc(sizeof(buzzer_t));
#else
	Buzzer = &mBuzzer;
#endif
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

  // Configure the Buzzer object
  Buzzer->interruptMs = 10; // interrupt will be triggered every 10ms
  Buzzer->fnx.pwmOut = pwm_set_freq; // pass set frequency function

  // initialize Buzzer
  buzzer_init(Buzzer);

  HAL_TIM_Base_Start_IT(&htim9);
  pwm_start();

  nextPattern = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (nextPattern){
		  nextPattern = 0;
		  HAL_Delay(500);
		  switch (example){
		  case EXAMPLE_BUZZER_START:
			  buzzer_turn_on(Buzzer, 1500);
			  nextPattern = 1;
			  break;
		  case EXAMPLE_BUZZER_STOP:
			  buzzer_stop(Buzzer);
			  nextPattern = 1;
			  break;
		  case EXAMPLE_BUZZER_RINGTONE:
			  buzzer_start_array(Buzzer, mario_theme_time, mario_theme_melody, mario_theme_len);
			  break;
		  case EXAMPLE_BUZZER_NO_LOOP:
			  buzzer_start(Buzzer, 500, 750, BUZZER_LOOP_OFF);
			  break;
		  case EXAMPLE_BUZZER_LOOP:
			  buzzer_start(Buzzer, 800, 250, BUZZER_LOOP_ON);
			  break;
		  }
		  example++;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
