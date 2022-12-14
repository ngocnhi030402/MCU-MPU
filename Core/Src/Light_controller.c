/*
 * Light_controller.c
 *
 *  Created on: Nov 19, 2022
 *      Author: phamk
 */
#include "Light_controller.h"

// RGB
GPIO_TypeDef * lightPort[2][3] = {
		{
			LED_1_0_GPIO_Port,
			LED_1_1_GPIO_Port,
		},

		{
			LED_2_0_GPIO_Port,
			LED_2_1_GPIO_Port,
		}
};

uint16_t lightPin[2][3] = {
		{
			LED_1_0_Pin,
			LED_1_1_Pin,
		},

		{
			LED_2_0_Pin,
			LED_2_1_Pin,
		}
};


void turnOnRed(int lightIndex){
	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][1], 0);
}


void turnOnGreen(int lightIndex){
	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][1], 0);
}

void turnOnYellow(int lightIndex){
	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][1], 1);
}


void turnOffAll(){
	HAL_GPIO_WritePin(lightPort[0][RED_INDEX], lightPin[0][0], 0);
	HAL_GPIO_WritePin(lightPort[0][GREEN_INDEX], lightPin[0][1], 0);

	HAL_GPIO_WritePin(lightPort[1][RED_INDEX], lightPin[1][0], 0);
	HAL_GPIO_WritePin(lightPort[1][GREEN_INDEX], lightPin[1][1], 0);
}
