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
			LED_1_2_GPIO_Port
		},

		{
			LED_2_0_GPIO_Port,
			LED_2_1_GPIO_Port,
			LED_2_2_GPIO_Port
		}
};

uint16_t * lightPin[2][3] = {
		{
			LED_1_0_Pin,
			LED_1_1_Pin,
			LED_1_2_Pin
		},

		{
			LED_2_0_Pin,
			LED_2_1_Pin,
			LED_2_2_Pin
		}
};


void turnOnRed(int lightIndex){
	if(lightIndex > 1)
		return;

	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][RED_INDEX], 0);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][GREEN_INDEX], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][YELLOW_INDEX], lightPin[lightIndex][YELLOW_INDEX], 1);
}


void turnOnGreen(int lightIndex){
	if(lightIndex > 1)
		return;

	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][RED_INDEX], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][GREEN_INDEX], 0);
	HAL_GPIO_WritePin(lightPort[lightIndex][YELLOW_INDEX], lightPin[lightIndex][YELLOW_INDEX], 1);
}

void turnOnYellow(int lightIndex){
	if(lightIndex > 1)
		return;

	HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][RED_INDEX], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][GREEN_INDEX], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][YELLOW_INDEX], lightPin[lightIndex][YELLOW_INDEX], 0);
}

void toggleRed(int lightIndex){
	if(lightIndex > 1)
		return;
	HAL_GPIO_TogglePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][RED_INDEX]);
}

void toggleGreen(int lightIndex){
	if(lightIndex > 1)
		return;
	HAL_GPIO_TogglePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][GREEN_INDEX]);
}

void toggleYellow(int lightIndex){
	if(lightIndex > 1)
		return;
	HAL_GPIO_TogglePin(lightPort[lightIndex][YELLOW_INDEX], lightPin[lightIndex][YELLOW_INDEX]);
}

void turnOffAll(){
	for (int lightIndex = 0; lightIndex <= 1; lightIndex++){
		HAL_GPIO_WritePin(lightPort[lightIndex][RED_INDEX], lightPin[lightIndex][RED_INDEX], 1);
		HAL_GPIO_WritePin(lightPort[lightIndex][GREEN_INDEX], lightPin[lightIndex][GREEN_INDEX], 1);
		HAL_GPIO_WritePin(lightPort[lightIndex][YELLOW_INDEX], lightPin[lightIndex][YELLOW_INDEX], 1);
	}
}
