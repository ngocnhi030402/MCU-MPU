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


void turnOnRed(int lightIndex){
	if(lightIndex > 1)
		return;

	HAL_GPIO_WritePin(lightPort[lightIndex][0], lightPin[lightIndex][0], 1);
	HAL_GPIO_WritePin(lightPort[lightIndex][1], lightPin[lightIndex][1], 0);
}


void turnOnGreen(int lightIndex){
	if(lightIndex > 1)
		return;

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
