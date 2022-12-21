/*
 * buzzer.h
 *
 *  Created on: Dec 15, 2022
 *      Author: admin
 */
#pragma once

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"

extern TIM_HandleTypeDef htim3;

void buzzer_modify_volume(uint8_t volumeLevel);
void buzzer_turn_on();
void buzzer_turn_off();
void buzzer_blinky();

#endif /* INC_BUZZER_H_ */
