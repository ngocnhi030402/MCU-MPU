#ifndef INC_LIGHT_CONTROLLER_H_
#define INC_LIGHT_CONTROLLER_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"

void turnOnRed(int lightIndex);
void turnOnGreen(int lightIndex);
void turnOnYellow(int lightIndex);

void turnOffAll();

#endif
