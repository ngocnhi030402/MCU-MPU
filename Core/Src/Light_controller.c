/*
 * Light_controller.c
 *
 *  Created on: Nov 19, 2022
 *      Author: phamk
 */
#include "Light_controller.h"
#include "main.h"
#include "global.h"
#define BLINK 6


// RGB
GPIO_TypeDef * lightPort[2][3] = {
		{
			LED_1_0_GPIO_Port,
			LED_1_1_GPIO_Port
		},

		{
			LED_2_0_GPIO_Port,
			LED_2_1_GPIO_Port
		}
};

uint16_t lightPin[2][3] = {
		{
			LED_1_0_Pin,
			LED_1_1_Pin,
		},

		{
			LED_2_0_Pin,
			LED_2_1_Pin
		}
};
GPIO_TypeDef * LedPortPedes[2] = {
		GPIOB,
		GPIOA
};
uint16_t LedPinPedes[2] = {
		PEDESTRIAN_LED_0_Pin,
		PEDESTRIAN_LED_1_Pin
};

void clearPedes(void){
	HAL_GPIO_WritePin(LedPortPedes[0],LedPinPedes[0],0);
	HAL_GPIO_WritePin(LedPortPedes[1],LedPinPedes[1],0);

}

void turnOnPedes(int ledColor){
	if (ledColor == LED_PEDES_GREEN){
		HAL_GPIO_WritePin(LedPortPedes[0],LedPinPedes[0],0);
		HAL_GPIO_WritePin(LedPortPedes[1],LedPinPedes[1],1);
	}
	else{
		HAL_GPIO_WritePin(LedPortPedes[0],LedPinPedes[0],1);
		HAL_GPIO_WritePin(LedPortPedes[1],LedPinPedes[1],1);
	}
}

void blinklyLight(enum StateLight currentState){
	if(timer_flag[BLINK]){
			if(blink){
				clearTrafficLight();
				blink = 0;
			}
			else{
//				turnOnLight(currentState,VER);
//				turnOnLight(currentState,HOR);
				blink = 1;
			}
			setTimer(BLINK,200);
		}
}
//void blinkyPedes(void){
//	if(timer_flag[BLINK]){
//		if(blink){
//			clearPedes();
//			blink = 0;
//		}
//		else{
//			turnOnPedes();
//			blink = 1;
//		}
//		setTimer(BLINK,1000);
//	}
//}
void turnOnRed(int lightIndex){
	if(lightIndex > 1)
		return;

	flag = 1;
	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 0);
}


void turnOnGreen(int lightIndex){
	if(lightIndex > 1)
		return;

	flag = 2;
	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 0);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 1);

}

void turnOnYellow(int lightIndex){
	if(lightIndex > 1)
		return;

	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 1);
}


void turnOffAll(){
	int lightIndex = 0;
	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 0);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 0);

	lightIndex = 1;
	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 0);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 0);
}

void led_turn_on(int led_index, int led_type) {
	switch (led_index) {
		case PEDESTRIAN:
			switch (led_type) {
				case LED_RED: //01
					HAL_GPIO_WritePin(GPIOB, PEDESTRIAN_LED_0_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, PEDESTRIAN_LED_1_Pin, GPIO_PIN_RESET);
					break;

				case LED_GREEN: //10
					HAL_GPIO_WritePin(GPIOB, PEDESTRIAN_LED_0_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, PEDESTRIAN_LED_1_Pin, GPIO_PIN_SET);
					break;

				case LED_YELLOW: //11
					HAL_GPIO_WritePin(GPIOB, PEDESTRIAN_LED_0_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, PEDESTRIAN_LED_1_Pin, GPIO_PIN_SET);
					break;
			}
			break;

		default:
			break;
	}
}

void led_turn_off(int led_index, int led_type) {
	switch (led_index) {
	case PEDESTRIAN:
		switch (led_type) {
		case LED_RED:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case LED_GREEN:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			break;
		}
		break;

	default:
		break;
	}
}



