/* USER CODE BEGIN Header */
#include "Button_software.h"
#include "Software_timer.h"
#include "Light_controller.h"
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
const int red_time_reset = RED_TIME_RESET;
const int green_time_reset = GREEN_TIME_RESET;
const int yellow_time_reset = YELLOW_TIME_RESET;

int RED_TIME  =	RED_TIME_RESET;
int GREEN_TIME = GREEN_TIME_RESET;
int YELLOW_TIME = YELLOW_TIME_RESET;

int isYellow = 0;

uint8_t tuneBlinkOn = 1;

const int oneSec = timer_prop*1;
const int ledRefreshTime = timer_prop*0.5;

//int red_time = RED_TIME*timer_prop;
//int green_time = GREEN_TIME*timer_prop;
//int yellow_time = YELLOW_TIME*timer_prop;

int light_blink_time = LIGHT_BLINK_TIME*timer_prop;

int countdown_0 = RED_TIME_RESET;
int countdown_1 = GREEN_TIME_RESET;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
STATE status = INIT_1;
MODE mode = MODE_AUTO;

void UARTOutput(int inpNum){
	char str[50];
	HAL_UART_Transmit(&huart2, (uint8_t *)str, sprintf(str, "!Countdown: %ld#\r\n", inpNum), 1000);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	button_reading();
	for (int i = 0 ; i < TIMER_LIMIT; i++){
		timerRun(i);
	}
}

void redManualRun(){
	turnOnRed(0);
	if (isYellow == 0)
		turnOnGreen(1);
	else
		turnOnYellow(1);
}

void redTuningRun(){
	if (tuneBlinkOn == 1){
		turnOnRed(0);
		turnOnRed(1);
	}
	else if (tuneBlinkOn == 0 && timer_flag[TUNE_BLINK_TIMER] == 1){
		timer_flag[TUNE_BLINK_TIMER] = 0;
		tuneBlinkOn = 1;
	}

	if (is_button_pressed(TUNING_BUTTON) == 1){
		tuneBlinkOn = 0;
		setTimer(0.1*timer_prop, TUNE_BLINK_TIMER);
		turnOffAll();

		RED_TIME = ((RED_TIME <= RED_MAX_COUNTDOWN) ? RED_TIME+1 : RED_MIN_COUNTDOWN);
		GREEN_TIME = RED_TIME - YELLOW_TIME;
	}
}

void greenManualRun(){
	turnOnGreen(0);
	turnOnRed(1);
}

void greenTuningRun(){
	if (tuneBlinkOn == 1){
		turnOnGreen(0);
		turnOnGreen(1);
	}
	else if (tuneBlinkOn == 0 && timer_flag[TUNE_BLINK_TIMER] == 1){
		timer_flag[TUNE_BLINK_TIMER] = 0;
		tuneBlinkOn = 1;
	}


	if (is_button_pressed(TUNING_BUTTON) == 1){
		tuneBlinkOn = 0;
		setTimer(0.1*timer_prop, TUNE_BLINK_TIMER);
		turnOffAll();

		GREEN_TIME = ((GREEN_TIME < GREEN_MAX_COUNTDOWN) ? GREEN_TIME+1 : GREEN_MIN_COUNTDOWN);
		RED_TIME = GREEN_TIME + YELLOW_TIME;
	}
}

void yellowManualRun(){
	turnOnYellow(0);
	turnOnRed(1);
}


void redAutoRun(){
	turnOnRed(0);
	if (countdown_1 <= 0 && isYellow == 0){
		isYellow = 1;
		countdown_1 = YELLOW_TIME;
		turnOnYellow(1);
	}
	else if (isYellow == 0){
		turnOnGreen(1);
	}

	if (countdown_0 <= 0){
		status = GREEN_AUTO;
		countdown_0 = GREEN_TIME;
		countdown_1 = RED_TIME;
	}
}

void greenAutoRun(){
	turnOnGreen(0);
	turnOnRed(1);
	if (countdown_0 <= 0){
		status = YELLOW_AUTO;
		countdown_0 = YELLOW_TIME;
		countdown_1 = RED_TIME - GREEN_TIME;
	}
}

void yellowAutoRun(){
	turnOnYellow(0);
	turnOnRed(1);
	if (countdown_0 <= 0 && countdown_1 <= 0){
		status = RED_AUTO;
		countdown_0 = RED_TIME;
		countdown_1 = GREEN_TIME;
		isYellow = 0;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  setTimer(oneSec, COUNTDOWN_TIMER);

  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  int counter = 1;
  while (1)
  {
//	  /*
//	   * -------------- FSM ----------------
//	   */
	  switch(status){
		  case (INIT_1):
			mode = MODE_AUTO;
			status = RED_AUTO;
			countdown_0 = RED_TIME;
			countdown_1 = GREEN_TIME;
			isYellow = 0;
			break;


		  case (RED_AUTO):
			redAutoRun();

		    // State transition auto->manual
		  	if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
		  		mode = MODE_MANUAL;
		  		status = RED_MAN;
		  		countdown_0 = RED_TIME;
		  		countdown_1 = 2;
		  		turnOffAll();
		  		isYellow = 0;
		  	}
			break;


		  case (GREEN_AUTO):
			greenAutoRun();

			// State transition auto->manual
			if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
				mode = MODE_MANUAL;
				status = RED_MAN;
				countdown_0 = RED_TIME;
				countdown_1 = 2;
				turnOffAll();
				isYellow = 0;
			}
			break;


		  case (YELLOW_AUTO):
			yellowAutoRun();

			// State transition auto->manual
			if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
				mode = MODE_MANUAL;
				status = RED_MAN;
				turnOffAll();
				isYellow = 0;
			}
			break;


		  case(RED_MAN):
			redManualRun();

			// State transition red_man -> green_man
			if (is_button_pressed(INSIDE_STATE_CHANGE_BUTTON) == 1){
				if (isYellow == 0){
					isYellow = 1;
				}
				else{
					mode = MODE_MANUAL;
					status = GREEN_MAN;
					turnOffAll();
				}
			}
			else if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
				mode = MODE_TUNING;
				status = RED_TUNING;
				turnOffAll();
			}

			break;


		  case(GREEN_MAN):
			greenManualRun();

		  	// State transition green_man -> yellow_man
		  	if (is_button_pressed(INSIDE_STATE_CHANGE_BUTTON) == 1){
		  		mode = MODE_MANUAL;
		  		status = YELLOW_MAN;
		  		turnOffAll();
		  	}
			else if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
				mode = MODE_TUNING;
				status = RED_TUNING;
				turnOffAll();
			}
			break;


		  case(YELLOW_MAN):
			yellowManualRun();
			// State transition green_man -> yellow_man
			if (is_button_pressed(INSIDE_STATE_CHANGE_BUTTON) == 1){
				mode = MODE_MANUAL;
				status = RED_MAN;
				isYellow = 0;
				turnOffAll();
			}
			else if (is_button_pressed(STATE_CHANGE_BUTTON) == 1){
				mode = MODE_TUNING;
				status = RED_TUNING;
				turnOffAll();
			}
			break;


		  case(RED_TUNING):
			redTuningRun();
		    if (is_button_pressed(INSIDE_STATE_CHANGE_BUTTON)){
		    	mode = MODE_TUNING;
		    	status = GREEN_TUNING;
		    }
		    else if (is_button_pressed(STATE_CHANGE_BUTTON)){
		    	mode = MODE_AUTO;
		    	status = INIT_1;
		    }
			break;


		  case(GREEN_TUNING):
			greenTuningRun();
		  	if (is_button_pressed(INSIDE_STATE_CHANGE_BUTTON)){
				mode = MODE_TUNING;
				status = RED_TUNING;
			}
		  	else if (is_button_pressed(STATE_CHANGE_BUTTON)){
				mode = MODE_AUTO;
				status = INIT_1;
			}
			break;


		  default:
			status = INIT_1;
			break;
	  }

	  /*
	   * -------------- TIMER THING ----------------
	   */
	  if (mode == MODE_AUTO){
		  if (timer_flag[COUNTDOWN_TIMER] == 1){
			  setTimer(oneSec,COUNTDOWN_TIMER);
			  countdown_0--;
			  countdown_1--;
		  }
	  }
	  else{

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|SPEAKER_Pin|PEDESTRIAN_LED_1_Pin|LED_1_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PEDESTRIAN_LED_0_Pin|LED_1_1_Pin|LED_2_1_Pin|LED_2_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON0_Pin BUTTON1_Pin BUTTON2_Pin */
  GPIO_InitStruct.Pin = BUTTON0_Pin|BUTTON1_Pin|BUTTON2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin SPEAKER_Pin PEDESTRIAN_LED_1_Pin LED_1_0_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|SPEAKER_Pin|PEDESTRIAN_LED_1_Pin|LED_1_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON3_Pin */
  GPIO_InitStruct.Pin = BUTTON3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PEDESTRIAN_LED_0_Pin LED_1_1_Pin LED_2_1_Pin LED_2_0_Pin */
  GPIO_InitStruct.Pin = PEDESTRIAN_LED_0_Pin|LED_1_1_Pin|LED_2_1_Pin|LED_2_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
