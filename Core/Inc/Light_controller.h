#ifndef INC_LIGHT_CONTROLLER_H_
#define INC_LIGHT_CONTROLLER_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"

#define	RED_INDEX		0
#define	GREEN_INDEX		1
#define	YELLOW_INDEX	2

void turnOnRed(int lightIndex);
void turnOnGreen(int lightIndex);
void turnOnYellow(int lightIndex);

#endif
