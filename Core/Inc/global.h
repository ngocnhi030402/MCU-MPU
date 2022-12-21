/*
 * global.h
 *
 *  Created on: Dec 15, 2022
 *      Author: admin
 */
#pragma once

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>

#include "Button_software.h"
#include "buzzer.h"
#include "Software_timer.h"

static unsigned char blink = 0;

// state for traffic
extern uint8_t global_state;
extern uint8_t auto_A_state;
extern uint8_t auto_B_state;
extern uint8_t manual_state;
extern uint8_t set_state;

// state for pedestrian
//define for active pedestrian fsm
#define PD_A_INIT	60
#define PD_A_RED	61
#define PD_A_GREEN	62

//define for blinky pedestrian led
#define PD_LED_ON	71
#define PD_LED_OFF	72

extern uint8_t pd_active_state;
extern uint8_t pd_led_state;

// state for buzzer
//define for blinky buzzer
#define BZ_ON		81
#define BZ_OFF		82

//uint8_t buzzer_state;

#endif /* INC_GLOBAL_H_ */
