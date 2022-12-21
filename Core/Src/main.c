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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global.h"

#include "Button_software.h"
#include "Software_timer.h"
#include "Light_controller.h"
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

/* USER CODE BEGIN PV */
const int red_time_reset = RED_TIME_RESET;
const int green_time_reset = GREEN_TIME_RESET;
const int yellow_time_reset = YELLOW_TIME_RESET;

int RED_TIME  =	RED_TIME_RESET;
int GREEN_TIME = GREEN_TIME_RESET;
int YELLOW_TIME = YELLOW_TIME_RESET;

int isYellow = 0;

uint8_t tuneBlinkOn = 1;
#define BLINK 6
const int oneSec = timer_prop*1;
const int ledRefreshTime = timer_prop*0.5;
PEDES_MODE STATUS_PEDES = PEDES_RESET;

//int red_time = RED_TIME*timer_prop;
//int green_time = GREEN_TIME*timer_prop;
//int yellow_time = YELLOW_TIME*timer_prop;

int light_blink_time = LIGHT_BLINK_TIME*timer_prop;
int pedes_blink_time = PEDES_BLINK_TIME*timer_prop;
int pedes_time_out = PEDES_TIME_OUT * timer_prop;
int pedes_state;
int countdown_0 = RED_TIME_RESET;
int countdown_1 = GREEN_TIME_RESET;

int flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
STATE status = INIT_1;
TRAFFIC_MODE mode = MODE_AUTO;

void UARTOutput(int inpNum){
	char str[50];
	HAL_UART_Transmit(&huart2, (uint8_t *)str, sprintf(str, "!Countdown: %ld#\r\n", inpNum), 1000);
}

int buzzerCounter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	button_reading();
	for (int i = 0 ; i < TIMER_LIMIT; i++){
		timerRun(i);
	}

	if (buzzerOn == 1){
		if (buzzerCounter < 500){
			buzzer_turn_off();
			buzzerCounter++;
		}
		else
		{
			buzzer_turn_on();
			buzzerCounter++;
			if (buzzerCounter >= 600)
				buzzerCounter = 0;
		}
	}
	else{
		buzzer_turn_off();
		buzzerCounter = 0;
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
void blinkyLight(enum StateLight currentState){
	if(timer_flag[BLINK]){
		if(blink){
			turnOffAll();
			blink = 0;
		}
		else{
			redAutoRun();
			greenAutoRun();
//			turnOnLight(currentState,HOR);
			blink = 1;
		}
		setTimer(BLINK,200);
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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  setTimer(oneSec, COUNTDOWN_TIMER);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  status = INIT_1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	 /*
	  * -------------- FSM ----------------
	  */

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
			// status = INIT_1;
			break;
	  }


	  /*
	   * ---------------- PEDESTRIAN LIGHT FSM -----------------
	   */
	  switch(STATUS_PEDES){
		  case PEDES_RESET:
			  clearPedes();
			  STATUS_PEDES = PEDES_OFF;
			  break;

		  case PEDES_OFF:
			  clearPedes();
			  if(is_button_pressed(PEDES_BUTTON)){
				  STATUS_PEDES = PEDES_ON;
				  pedes_state = PEDES_INIT;
				  setTimer(pedes_time_out, TIMER_PD);
			  }
			  break;

		  case PEDES_ON:
			  switch(pedes_state){
				  case PEDES_INIT:
					  if (status == RED_MAN ||status == RED_AUTO || status == RED_TUNING)
						  pedes_state = PEDES_GREEN;
					  else
						  pedes_state = PEDES_RED;
					  break;

				  case PEDES_RED:
					turnOnPedes(LED_PEDES_RED);

					if (status == RED_MAN ||status == RED_AUTO || status == RED_TUNING)
						pedes_state = PEDES_GREEN;
					else
						pedes_state = PEDES_RED;

					if(is_button_pressed(PEDES_BUTTON)){
						setTimer(pedes_time_out, TIMER_PD);
					}
					break;

				  case PEDES_GREEN:
					turnOnPedes(LED_PEDES_GREEN);

					if (status == RED_MAN ||status == RED_AUTO || status == RED_TUNING)
						pedes_state = PEDES_GREEN;
					else
						pedes_state = PEDES_RED;

					if(is_button_pressed(PEDES_BUTTON)){
						setTimer(pedes_time_out, TIMER_PD);
					}
					break;

				  default:
					pedes_state = PEDES_INIT;
					break;
			  }
			  break;

		  default:
			  STATUS_PEDES = PEDES_RESET;
			  break;
	  }

	  if (timer_flag[TIMER_PD] == 1 && STATUS_PEDES == PEDES_ON) {
		  STATUS_PEDES = PEDES_RESET;
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
