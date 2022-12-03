#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"
#include "software_timer.h"

//we aim to work with more than one buttons
#define NO_OF_BUTTONS 				       3
//time to register hold
#define DURATION_FOR_BUTTON_HOLD		   500
//time interval inbetween each press to detect double press
#define DURATION_FOR_DOUBLE_PRESS		   200
#define BUTTON_IS_PRESSED                  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED                 GPIO_PIN_SET
//the buffer that the final result is stored after

extern uint8_t flagForButtonPressed[NO_OF_BUTTONS];
extern uint8_t flagForButtonDoublePressed[NO_OF_BUTTONS];
extern uint8_t flagForButtonHold[NO_OF_BUTTONS];

void button_reading(void);
int is_button_pressed(int index);
int is_button_double_pressed(int index);
int is_button_hold(int index);


#endif /* INC_INPUT_READING_H_ */
