#pragma once

#ifndef INC_LIGHT_CONTROLLER_H_
#define INC_LIGHT_CONTROLLER_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"

#define	TRAFFIC_1	5
#define	TRAFFIC_2	6
#define PEDESTRIAN	7

#define LED_RED		0
#define	LED_GREEN 	1
#define LED_YELLOW	2

#define LED_PEDES_GREEN	0
#define LED_PEDES_RED	1

void turnOnRed(int lightIndex);
void turnOnGreen(int lightIndex);
void turnOnYellow(int lightIndex);
void turnOnPedes(int ledColor);

void turnOffAll();

#endif
