/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
	INIT_1,

	RED_AUTO,
	YELLOW_AUTO,
	GREEN_AUTO,

	RED_MAN,
	GREEN_MAN,
	YELLOW_MAN,

	RED_TUNING,
	GREEN_TUNING,

	PEDES

} STATE;

typedef enum {
	MODE_AUTO,
	MODE_MANUAL,
	MODE_TUNING
} TRAFFIC_MODE;

enum StateLight{
	INIT,
	RED,
	YELLOW,
	GREEN
};

typedef enum{
	PEDES_RESET,
	PEDES_ON,
	PEDES_OFF,
	PEDES_INIT,
	PEDES_RED,
	PEDES_GREEN
} PEDES_MODE;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON0_Pin GPIO_PIN_0
#define BUTTON0_GPIO_Port GPIOA
#define BUTTON1_Pin GPIO_PIN_1
#define BUTTON1_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_4
#define BUTTON2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define BUTTON3_Pin GPIO_PIN_0
#define BUTTON3_GPIO_Port GPIOB
#define PEDESTRIAN_LED_0_Pin GPIO_PIN_10
#define PEDESTRIAN_LED_0_GPIO_Port GPIOB
#define PEDESTRIAN_LED_1_Pin GPIO_PIN_8
#define PEDESTRIAN_LED_1_GPIO_Port GPIOA
#define LED_1_0_Pin GPIO_PIN_10
#define LED_1_0_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LED_1_1_Pin GPIO_PIN_3
#define LED_1_1_GPIO_Port GPIOB
#define LED_2_1_Pin GPIO_PIN_4
#define LED_2_1_GPIO_Port GPIOB
#define LED_2_0_Pin GPIO_PIN_5
#define LED_2_0_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define SYSTEM_DELAY 	1

// timer index define
#define COUNTDOWN_TIMER		0
#define TUNE_BLINK_TIMER	1
#define PEDES_BLINK_TIMER	2
#define TIMER_PD			3
#define TIMER_DUR			4
#define TIMER_BLINK			5

// conutdown define
#define  RED_TIME_RESET		5
#define  GREEN_TIME_RESET	3
#define  YELLOW_TIME_RESET	2

#define RED_MAX_COUNTDOWN 		20
#define GREEN_MAX_COUNTDOWN 	15

#define RED_MIN_COUNTDOWN 	 	5
#define GREEN_MIN_COUNTDOWN		3

// button index define
#define STATE_CHANGE_BUTTON			0
#define INSIDE_STATE_CHANGE_BUTTON	1
#define TUNING_BUTTON				2
#define PEDES_BUTTON				3

#define timer_prop  1000
#define LIGHT_BLINK_TIME 2
#define PEDES_BLINK_TIME 3
#define PEDES_TIME_OUT	 5


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
