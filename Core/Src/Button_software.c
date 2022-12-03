#include "Button_software.h"

//button ports and pins array
GPIO_TypeDef * buttonPort[NO_OF_BUTTONS] = {
	BUTTON0_GPIO_Port,
	BUTTON1_GPIO_Port,
	BUTTON2_GPIO_Port
};

uint16_t buttonPin[NO_OF_BUTTONS] = {
	BUTTON0_Pin,
	BUTTON1_Pin,
	BUTTON2_Pin
};

//debouncing
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
//we define three buffers for debouncing
static GPIO_PinState debounceButtonBuffer0[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
//we define a flag for a button pressed more than 1 second.
uint8_t flagForButtonPressed[NO_OF_BUTTONS];
uint8_t flagForButtonDoublePressed[NO_OF_BUTTONS];
uint8_t flagForButtonHold[NO_OF_BUTTONS];
//we define counter for automatically increasing the value
//after the button is pressed more than 1 second.
static uint16_t counterForButtonHold[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t waitForSecondPress[NO_OF_BUTTONS] = {0, 0, 0};

void button_reading(void){
	for(int i = 0; i < NO_OF_BUTTONS; i++) {
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = debounceButtonBuffer0[i];
		debounceButtonBuffer0[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);
		if((debounceButtonBuffer2[i] == debounceButtonBuffer1[i]) || (debounceButtonBuffer1[i] == debounceButtonBuffer0[i])) {
			if(buttonBuffer[i] != debounceButtonBuffer0[i]) {
				buttonBuffer[i] = debounceButtonBuffer0[i];
				if(buttonBuffer[i] == BUTTON_IS_PRESSED) {
					counterForButtonHold[i] = DURATION_FOR_BUTTON_HOLD;
					if(SYSTEM_DELAY > 0) counterForButtonHold[i] /= SYSTEM_DELAY;

					//If the wait time is previously set
					if(waitForSecondPress[i] > 0 && waitForSecondPress[i] < DURATION_FOR_DOUBLE_PRESS / SYSTEM_DELAY) {
						flagForButtonDoublePressed[i] = 1;
						waitForSecondPress[i] = 0;
					}
					else {
						waitForSecondPress[i] = DURATION_FOR_DOUBLE_PRESS;
						if(SYSTEM_DELAY > 0) waitForSecondPress[i] /= SYSTEM_DELAY;
					}
				}
				else {
					flagForButtonHold[i] = 0;
					waitForSecondPress[i]--;
				}
			}
			else {
				if(buttonBuffer[i] == BUTTON_IS_PRESSED) {
						counterForButtonHold[i]--;
						if(counterForButtonHold[i] == 0) {
							flagForButtonHold[i] = 1;
							waitForSecondPress[i] = 0;
						}
				}
				else {
					waitForSecondPress[i]--;
					if(waitForSecondPress[i] == 0) {
						flagForButtonPressed[i] = 1;
					}
				}
			}
		}
	}
}

int is_button_pressed(int index) {
	if(index < 0 || index > NO_OF_BUTTONS) return 0;
	if(flagForButtonPressed[index] == 1) {
		flagForButtonPressed[index] = 0;
		return 1;
	}
	return 0;
}

int is_button_double_pressed(int index) {
	if(index < 0 || index > NO_OF_BUTTONS) return 0;
	if(flagForButtonDoublePressed[index] == 1) {
		flagForButtonDoublePressed[index] = 0;
		return 1;
	}
	return 0;
}

int is_button_hold(int index) {
	if(index < 0 || index > NO_OF_BUTTONS) return 0;
	if(flagForButtonHold[index] == 1) {
		return 1;
	}
	return 0;
}
